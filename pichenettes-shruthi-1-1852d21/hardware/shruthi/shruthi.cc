// Copyright 2009 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "hardware/hal/adc.h"
#include "hardware/hal/devices/external_eeprom.h"
#include "hardware/hal/devices/input_array.h"
#include "hardware/hal/devices/output_array.h"
#include "hardware/hal/devices/rotary_encoder.h"
#include "hardware/hal/devices/switch_array.h"
#include "hardware/hal/gpio.h"
#include "hardware/hal/init_atmega.h"
#include "hardware/hal/serial.h"
#include "hardware/hal/time.h"
#include "hardware/hal/timer.h"
#include "hardware/midi/midi.h"
#include "hardware/shruthi/audio_out.h"
#include "hardware/shruthi/display.h"
#include "hardware/shruthi/editor.h"
#include "hardware/shruthi/midi_dispatcher.h"
#include "hardware/shruthi/storage.h"
#include "hardware/shruthi/synthesis_engine.h"
#include "hardware/utils/task.h"

using namespace hardware_hal;
using namespace hardware_midi;
using namespace hardware_shruthi;

using hardware_utils::NaiveScheduler;
using hardware_utils::Task;

// Midi input.
Serial<MidiPort, 31250, BUFFERED, POLLED> midi_io;

// Input event handlers.
typedef AdcInputScanner AnalogInputs;
typedef InputArray<AnalogInputs, kNumEditingPots, 8> Pots;

Pots pots;
AnalogInputs analog_inputs;

// LED array.
OutputArray<
    Gpio<kPinLatch>,
    Gpio<kPinClk>,
    Gpio<kPinData>, kNumLeds, 4, MSB_FIRST, false> leds;

// Switches array
SwitchArray<
  Gpio<kPinLatch>,
  Gpio<kPinClk>,
  Gpio<kPinDigitalInput>,
  kNumSwitches,
  KEY_LOAD_SAVE> switches;

RotaryEncoderBuffer<RotaryEncoder<
    Gpio<kPinEncoderA>,
    Gpio<kPinEncoderB>,
    Gpio<kPinEncoderClick> > > encoder;

PwmOutput<kPinVcfCutoffOut> vcf_cutoff_out;
PwmOutput<kPinVcfResonanceOut> vcf_resonance_out;
PwmOutput<kPinVcaOut> vca_out;
PwmOutput<kPinCv1Out> cv_1_out;
PwmOutput<kPinCv2Out> cv_2_out;

MidiStreamParser<MidiDispatcher> midi_parser;

static uint8_t programmer_active_pot = 0;

// What follows is a list of "tasks" - short functions handling a particular
// aspect of the synth (rendering audio, updating the LCD display, etc). they
// are called in sequence, with some tasks being more frequently called than
// others, by the Scheduler.
void UpdateLedsTask() {
  // Since the LCD and the shift register are sharing some lines, make sure that
  // the LCD is not currently writing some data before doing some stuff with
  // the shift register.
  leds.Clear();
  if (editor.current_page() == PAGE_PERFORMANCE) {
    // x0x-style chasing lights.
    leds.set_value(engine.voice_controller().step() & 0x07, 15);
  } else {
    // Sequencer blinky on MODE LED.
    if (editor.current_mode() == EDITOR_MODE_SEQUENCE) {
      if (engine.voice_controller().active()) {
        if (!(engine.voice_controller().step() & 3)) {
          leds.set_value(LED_MODE, engine.voice_controller().step() ? 1 : 15);
        }
      }
    } else {
      // MODE LED is on, but the modulation matrix LED can go blink...
      leds.set_value(LED_MODE, 15);
      if (editor.current_page() == PAGE_MOD_MATRIX) {
        uint8_t current_modulation_source_value = engine.modulation_source(0,
            engine.patch().modulation_matrix.modulation[
                editor.subpage()].source);
        leds.set_value(LED_6, current_modulation_source_value >> 4);
      }
    }
    uint8_t pattern = editor.leds_pattern();
    uint8_t mask = 1;
    for (uint8_t i = 0; i < 8; ++i) {
      if (pattern & mask) {
        leds.set_value(i, 15);
      }
      mask <<= 1;
    }
  }
  leds.Begin();
  if (engine.system_settings().expansion_cv_mode == CV_MODE_PROGRAMMER) {
    leds.ShiftOutByte(programmer_active_pot);
  }
  leds.ShiftOut();
  leds.End();
}

void UpdateDisplayTask() {
  // Check if some patch settings have been modified by a SysEx or NRPN. If this
  // is the case, refresh the content of the LCD display.
  if (engine.dirty()) {
    editor.Refresh();
  }
  // The display can now blink the cursor and/or output modified characters.
  display.Tick();
}

void InputTask() {
  KeyEvent switch_event;
  Pots::Event pot_event;
  static uint8_t idle;
  static int8_t delta;
  static uint8_t previous_page;
TASK_BEGIN_NEAR
  while (1) {
    previous_page = editor.current_page();
    idle = 0;

    // Read the switches.
    switches.Read();
    
    // If a button was pressed, perform the action. Otherwise, if nothing
    // happened for 1.5s, update the idle flag.
    if (switches.idle_time() > (engine.system_settings().display_delay << 7)) {
      idle = 1;
    } else {
      if (switches.released()) {
        editor.HandleKeyEvent(switches.key_event());
      }
    }

    // Read the ADC.
    analog_inputs.Scan();
    pot_event = pots.Read();

    // Update the editor if something happened.
    // Revert back to the main page when nothing happened for 1.5s.
    if (pot_event.event == EVENT_NONE) {
      if (idle &&
          pot_event.time > (engine.system_settings().display_delay << 7)) {
        editor.Relax();
      }
    } else {
      editor.HandleInput(pot_event.id, pot_event.value);
    }
    TASK_SWITCH;

    if (encoder.increment()) {
      switches.Touch();
      int8_t increment = encoder.increment();
      if (switches.shifted()) {
        switches.InhibitShiftRelease();
        increment *= 10;
      }
      editor.HandleIncrement(increment);
    } else {
      if (encoder.clicked()) {
        switches.Touch();
        editor.HandleClick();
      }
    }
    encoder.Flush();
    // In case we have moved to a different page, make the pots less sensitive
    // to changes to make sure that a subtle change to a pot won't create a
    // discontinuity.
    if (editor.current_page() != previous_page) {
      pots.Lock(32);
    }
    TASK_SWITCH;
  }
TASK_END
}

uint8_t current_cv;
uint8_t programmer_counter = 0;
uint8_t currently_tweaked_pot;
int16_t pots_value[32];

void CvTask() {
  analog_inputs.Scan();
  if (engine.system_settings().expansion_cv_mode == CV_MODE_4CV_IN) {
    analog_inputs.set_num_inputs(8);
    ++current_cv;
    if (current_cv >= kNumCvInputs) {
      current_cv = 0;
    }
    engine.set_cv(
        current_cv,
        analog_inputs.Read(kPinCvInput + current_cv) >> 2);
  } else if (engine.system_settings().expansion_cv_mode == CV_MODE_PROGRAMMER) {
    analog_inputs.set_num_inputs(5);
    int16_t value = analog_inputs.Read(kPinCvInput);
    // Read the pot selected on the multiplexer. If it has been touched, 
    // change the corresponding parameter in the editor, and instruct the
    // scanner to spend more time scanning this pot.
    if ((pots_value[programmer_active_pot] - value > 8) ||
        (pots_value[programmer_active_pot] - value < -8)) {
      engine.SetScaledParameter(programmer_active_pot, value >> 3);
      currently_tweaked_pot = programmer_active_pot;
      pots_value[programmer_active_pot] = value;
    }
    ++programmer_counter;
    if (programmer_counter & 1) {
      programmer_active_pot = currently_tweaked_pot;
    } else {
      programmer_active_pot = (programmer_counter >> 1) & 0x1f;
    }
  } else {
    analog_inputs.set_num_inputs(7);
    uint8_t value = analog_inputs.Read(kPinCvInput + current_cv) >> 2;
    if (current_cv <= 1) {
      value >>= 1;
      uint8_t offset = current_cv * 4;
      engine.SetParameter(PRM_OSC_PARAMETER_1 + offset, value);
      ++current_cv;
    } else {
      engine.set_unregistered_modulation(0, value);
      current_cv = 0;
    }
  }
}

void MidiTask() {
  // Try to process as much data as possible from the MIDI buffer.
  while (midi_io.readable()) {
    midi_parser.PushByte(midi_io.ImmediateRead());
  }
  // This is also a good place to do some MIDI output. But because the MIDI
  // output data rate can get intense when tweaking knobs, it's easier to
  // flush the data at a faster rate in the audio sample interrupt.
}

void AudioRenderingTask() {
  // Run only when there's a block of 40 samples to fill...
  if (audio_out.writable_block()) {
    engine.ProcessBlock();
    vcf_cutoff_out.Write(engine.voice(0).cutoff());
    vcf_resonance_out.Write(engine.voice(0).resonance());
    vca_out.Write(engine.voice(0).vca());
    cv_1_out.Write(engine.voice(0).cv_1());
    cv_2_out.Write(engine.voice(0).cv_2());
  }
}

uint16_t previous_num_glitches;

// This task displays a '!' in the status area of the LCD whenever
// a discontinuity occurred in the audio rendering. Even if the code is
// optimized in such a way that it never occurs, I'd rather keep it here in
// case new features are implemented and need performance monitoring.
// This code uses 42 bytes.
void AudioGlitchMonitoringTask() {
  uint16_t num_glitches = audio_out.num_glitches();
  if (num_glitches != previous_num_glitches) {
    previous_num_glitches = num_glitches;
    display.set_status('!');
  }
}

typedef NaiveScheduler<kSchedulerNumSlots> Scheduler;

Scheduler scheduler;

/* static */
template<>
Task Scheduler::tasks_[] = {
    { &AudioRenderingTask, 16 },
    { &MidiTask, 6 },
    { &UpdateLedsTask, 4 },
    { &UpdateDisplayTask, 2 },
    { &AudioGlitchMonitoringTask, 1 },
    { &InputTask, 2 },
    { &CvTask, 1 },
};

inline void FlushMidiOut() {
  if (midi_dispatcher.readable()) {
    if (midi_io.writable()) {
      midi_io.Overwrite(midi_dispatcher.ImmediateRead());
    }
  }
}

uint8_t sub_clock = 0;
uint8_t sub_clock_2 = 0;

TIMER_2_TICK {
  audio_out.EmitSample();
  sub_clock = (sub_clock + 1) & 0x0f;
  if (sub_clock == 0) {
    lcd.Tick();
    encoder.Read();
    // Flush to the output the buffered MIDI data.
    FlushMidiOut();
    sub_clock_2 = ~sub_clock_2;
    if (sub_clock_2) {
      TickSystemClock();
    }
  }
}

void Init() {
  sei();
  UCSR0B = 0;
  UCSR1B = 0;
  
  audio_out.Init();
  midi_io.Init();
  pots.Init();
  switches.Init();
  encoder.Init();
  leds.Init();
  
  // Initialize all the PWM outputs to 39kHz, phase correct mode.
  Timer<0>::set_prescaler(1);
  Timer<0>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<1>::set_prescaler(1);
  Timer<1>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::set_prescaler(1);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::Start();
  
  vcf_cutoff_out.Init();
  vcf_resonance_out.Init();
  vca_out.Init();
  cv_1_out.Init();
  cv_2_out.Init();

  lcd.Init();
  display.Init();
  lcd.SetCustomCharMapRes(character_table[0], 7, 1);
  editor.Init();
  Storage::Init();

  engine.Init();

  // If the encoder is pressed at startup, toggle the "Boot on patch page"
  // flag.
  if (encoder.immediate_value() == 0) {
    engine.mutable_system_settings()->patch_restore_on_boot ^= 0x80;
    engine.system_settings().EepromSave();
  }
  
  // When booting on the patch page, jump to the patch page and restore the
  // previously loaded patch.
  if (engine.system_settings().patch_restore_on_boot & 0x80) {
    editor.BootOnPatchBrowsePage(
        engine.system_settings().patch_restore_on_boot & 0x7f);
  } else {
    editor.DisplaySplashScreen(STR_RES_V + 1);
  }
  
  scheduler.Init();
}

int main(void) {
  Init();
  scheduler.Run();
}
