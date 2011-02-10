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
//
// -----------------------------------------------------------------------------
//
// Voice manager / arpeggiator.
//
// Routes note messages to a pool of voices, and handles arpeggiation.
// For now, used in monophonic mode only, but it should be fairly easy to use
// the NoteStack instance contained in this class to handle voice stealing for
// a polyphonic synth.
//
// Two instances of this guy will be needed for multitimbrality. Since there is
// no plan to support multitimbrality, this class is implemented as a "static
// singleton". This does not yield a code size gain, but this is coherent with
// what was done with NoteStack.

#ifndef HARDWARE_SHRUTHI_VOICE_CONTROLLER_
#define HARDWARE_SHRUTHI_VOICE_CONTROLLER_

#include "hardware/shruthi/shruthi.h"

#include "hardware/shruthi/note_stack.h"
#include "hardware/shruthi/sequencer_settings.h"

namespace hardware_shruthi {

class SequencerSettings;
class Voice;

struct EventHandler {
  void (*handle_note_on)(uint8_t note, uint8_t velocity);
  void (*handle_note_off)(uint8_t note);
  void (*handle_step)(int8_t delta);
  uint8_t kind;
  uint8_t padding;
};

class VoiceController {
 public:
  VoiceController() { }
  static void Init(SequencerSettings* sequencer_settings,
                   Voice* voices, uint8_t num_voices_);
  static void AllNotesOff();
  static void AllSoundOff();
  static void Reset();
  static void Step(int8_t delta);
  static void NoteOn(uint8_t note, uint8_t velocity);
  static void ForceNoteOn(uint8_t note, uint8_t velocity);
  static void NoteOff(uint8_t note);
  static void TouchSequence();
  static inline void ExternalSync() { --midi_clock_counter_; }
  static inline uint8_t step() { return pattern_step_; }
  static inline uint8_t active() { return active_; }
  static uint16_t has_arpeggiator_note();
  // Returns 1 when the clock has advanced.
  static uint8_t Control();
  static uint16_t estimated_beat_duration() {
    return estimated_beat_duration_;
  }
  static uint8_t recording() { return recording_; }
  
  // (for external sync).
  static void Stop() {
    active_ = 0;
  }
  static void StopAndKillNotes();
  static void Start() {
    Reset();
    active_ = 1;
  }

 private:
  static void ArpeggioStep(int8_t delta);
  static void ArpeggioStart(int8_t delta);
  static int8_t FoldPattern();
  static void ComputeExpandedPatternSize();
  
  // The following functions handle note on/off/step updates messages for each
  // different sequencer mode.
  static void NoteOnHandlerArpStep(uint8_t note, uint8_t velocity);
  static void NoteOnHandlerArpLatch(uint8_t note, uint8_t velocity);
  static void NoteOnHandlerRps(uint8_t note, uint8_t velocity);
  static void NoteOnHandlerRpsLatch(uint8_t note, uint8_t velocity);
  static void NoteOnHandlerRpsRecording(uint8_t note, uint8_t velocity);

  static void NoteOffHandlerLatch(uint8_t note);
  static void NoteOffHandlerDefault(uint8_t note);
  static void NoteOffHandlerRpsRecording(uint8_t note);
  
  static void StepHandlerArp(int8_t delta);
  static void StepHandlerSequencer(int8_t delta);
  static void StepHandlerImprovisation(int8_t delta);

  static int16_t internal_clock_counter_;
  static int8_t midi_clock_counter_;
  static uint8_t midi_clock_prescaler_;
  static int16_t average_step_duration_;
  static int16_t step_duration_[kNumSteps];

  // 16 steps x-o-x pattern storing the steps at which a new note is triggered.
  static uint16_t pattern_;
  // Shift by 1 every 1/16th note, with swing.
  static uint16_t pattern_mask_;
  // Increment by 1 every 1/16th note, with swing.
  static uint8_t pattern_step_;
  static uint8_t expanded_pattern_step_;
  static uint8_t expanded_pattern_size_;
  static uint8_t expanded_pattern_[256];

  // Incremented/decremented by 1 for up/down pattern.
  static int8_t arp_step_;
  // Direction increment.
  static int8_t arp_current_direction_;
  static int8_t arp_octave_step_;

  static NoteStack notes_;
  static Voice* voices_;
  static uint8_t num_voices_;

  // After 4 beats without event, the sequencer is not active. The LED stops
  // blinking and the sequencer will restart from the first note in the pattern. 
  static uint8_t active_;
  static uint8_t inactive_steps_;
  static uint8_t previous_mode_;
  
  // We are in "recording mode" when:
  // - The arpeggiator is set to "latch" and a first key has been pressed.
  // - The "rec" sequencer mode has been activated.
  static uint8_t recording_;
  static uint8_t recording_note_;
  static uint8_t recording_gate_;
  // This indicates at which step we must stop recording (1 pattern cycle). Set
  // to 0xff when we're still waiting for a note to be played to initiate
  // recording.
  static uint8_t will_stop_at_step_;  

  // In order to sync the LFOs to an external MIDI clock, we need to estimate at
  // which BPM the master MIDI clock is running. This attemps to track this by
  // counting the number of control rate cycles in a beat.
  static uint16_t step_duration_estimator_num_;
  static uint8_t step_duration_estimator_den_;
  static uint16_t estimated_beat_duration_;

  static EventHandler handler_;
  static EventHandler handler_table_[];
  
  static SequencerSettings* sequencer_settings_;

  DISALLOW_COPY_AND_ASSIGN(VoiceController);
};

}  // namespace hardware_shruthi

#endif  // HARDWARE_SHRUTHI_VOICE_CONTROLLER_
