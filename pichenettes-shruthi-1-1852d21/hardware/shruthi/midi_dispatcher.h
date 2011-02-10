// Copyright 2010 Olivier Gillet.
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
//
// -----------------------------------------------------------------------------
//
// This class dispatches received MIDI events to the different components of the
// synth (synthesis engine, display, editor) that might react to them.

#ifndef HARDWARE_SHRUTHI_MIDI_DISPATCHER_H_
#define HARDWARE_SHRUTHI_MIDI_DISPATCHER_H_

#include "hardware/base/base.h"
#include "hardware/hal/ring_buffer.h"
#include "hardware/midi/midi.h"
#include "hardware/shruthi/display.h"
#include "hardware/shruthi/editor.h"
#include "hardware/shruthi/storage.h"
#include "hardware/shruthi/synthesis_engine.h"

namespace hardware_shruthi {

class MidiDispatcher : public hardware_midi::MidiDevice {
 public:
  enum {
    buffer_size = 128,
    data_size = 8,
  };
  typedef hardware_hal::Buffer<MidiDispatcher> OutputBuffer;
  typedef hardware_hal::DataTypeForSize<data_size>::Type Value;
  MidiDispatcher() { }

  // ------ MIDI in handling ---------------------------------------------------

  // Forwarded to the controller.
  static inline void NoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    display.set_status('\x01');
    if (!editor.HandleNoteOn(note, velocity)) {
      engine.NoteOn(channel, note, velocity);
    }
  }
  static inline void NoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    engine.NoteOff(channel, note, velocity);
  }

  // Handled.
  static inline void ControlChange(
      uint8_t channel,
      uint8_t controller,
      uint8_t value) {
    if (controller == hardware_midi::kBankLsb) {
      current_bank_ = value;
    } else {
      display.set_status('\x05');
      engine.ControlChange(channel, controller, value);
    }
  }
  static inline void PitchBend(uint8_t channel, uint16_t pitch_bend) {
    display.set_status('\x02');
    engine.PitchBend(channel, pitch_bend);
  }
  static void Aftertouch(uint8_t channel, uint8_t note, uint8_t velocity) {
    engine.Aftertouch(channel, note, velocity);
  }
  static void Aftertouch(uint8_t channel, uint8_t velocity) {
    engine.Aftertouch(channel, velocity);
  }
  static void AllSoundOff(uint8_t channel) {
    engine.AllSoundOff(channel);
  }
  static void ResetAllControllers(uint8_t channel) {
    engine.ResetAllControllers(channel);
  }
  static void AllNotesOff(uint8_t channel) {
    engine.AllNotesOff(channel);
  }
  static void OmniModeOff(uint8_t channel) {
    engine.OmniModeOff(channel);
  }
  static void OmniModeOn(uint8_t channel) {
    engine.OmniModeOn(channel);
  }
  
  static void ProgramChange(uint8_t channel, uint8_t program) {
    uint16_t n = program + (current_bank_ << 7);
    if (n < Storage::size<Patch>()) {
      Storage::Load(engine.mutable_patch(), n);
      // Do not force a SysEx sync because the slave in the polychain will also
      // receive the program change anyway!
      engine.TouchPatch(0);
    }
  }
  
  static void Reset() { engine.Reset(); }
  static void Clock() { engine.Clock(); }
  static void Start() { engine.Start(); }
  static void Stop() { engine.Stop(); }
  
  static void SysExStart() {
    ProcessSysEx(0xf0);
  }
  static void SysExByte(uint8_t sysex_byte) {
    ProcessSysEx(sysex_byte);
  }
  static void SysExEnd() {
    ProcessSysEx(0xf7);
    if (Storage::sysex_rx_state() == RECEPTION_OK) {
      display.set_status('+');
      engine.TouchPatch(0);
    } else {
      display.set_status('#');
    }
  }
  
  static uint8_t CheckChannel(uint8_t channel) {
    const SystemSettings& settings = engine.system_settings();
    return settings.midi_channel == 0 ||
           settings.midi_channel == (channel + 1);
  }
  
  static void RawMidiData(
      uint8_t status,
      uint8_t* data,
      uint8_t data_size,
      uint8_t accepted_channel) {
    uint8_t hi = status & 0xf0;
    // When is parsed midi data forwarded to the MIDI out?
    // - When the data is a channel different from the RX channel.
    // - When we are in "Full" mode.
    // - When the midi message is not a note on/note off.
    if (mode() >= MIDI_OUT_SPLIT) {
      if (status != 0xf0 && status != 0xf7) {
        if ((hi != 0x80 && hi != 0x90) ||
            mode() == MIDI_OUT_FULL ||
            !accepted_channel) {
          Send(status, data, data_size);
        }
      }
    }
  }
  
  static void RawByte(uint8_t byte) {
    // Report that some data has been received. The MIDI Out filter might
    // propagate it directly to the output if "Soft Thru" is enabled.
    if (mode() == MIDI_OUT_SOFT_THRU) {
      OutputBuffer::Overwrite(byte);
    }
  }
  
  // ------ MIDI out handling --------------------------------------------------
  static void NoteKilled(uint8_t note);
  static void NoteTriggered(uint8_t note, uint8_t velocity);
  static void Send(uint8_t status, uint8_t* data, uint8_t size);
  static void Send2(uint8_t status, uint8_t a);
  static void Send3(uint8_t status, uint8_t a, uint8_t b);
  static void SendParameter(uint8_t index, uint8_t value);

  static uint8_t readable() {
    return OutputBuffer::readable();
  }

  static uint8_t ImmediateRead() {
    return OutputBuffer::ImmediateRead();
  }
  
 private:
  static uint8_t data_entry_counter_;
  static uint8_t current_parameter_index_;
  static uint8_t current_bank_;
  
  static void ProcessSysEx(uint8_t byte) {
    if (mode() >= MIDI_OUT_SPLIT) {
      Send(byte, NULL, 0);
    }
    Storage::SysExReceive(byte);
  }
  
  static uint8_t mode() { return engine.system_settings().midi_out_mode; }
  static uint8_t channel() {
    return engine.system_settings().midi_channel == 0
        ? 0
        : engine.system_settings().midi_channel - 1;
  }
  DISALLOW_COPY_AND_ASSIGN(MidiDispatcher);
};

extern MidiDispatcher midi_dispatcher;

}  // namespace hardware_shruthi

#endif  // HARDWARE_SHRUTHI_MIDI_DISPATCHER_H_
