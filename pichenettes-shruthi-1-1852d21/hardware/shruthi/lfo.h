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
// LFO (cheap oscillator).
//
// Contrary to oscillators which are templatized "static singletons", to
// generate the fastest, most specialized code, LFOs are less
// performance-sensitive and are thus implemented as a traditional class.

#ifndef HARDWARE_SHRUTHI_LFO_H_
#define HARDWARE_SHRUTHI_LFO_H_

#include "hardware/base/base.h"
#include "hardware/shruthi/envelope.h"
#include "hardware/shruthi/patch.h"
#include "hardware/shruthi/resources.h"
#include "hardware/shruthi/sequencer_settings.h"
#include "hardware/shruthi/shruthi.h"
#include "hardware/utils/random.h"
#include "hardware/utils/op.h"

using hardware_utils::Random;

namespace hardware_shruthi {

class Lfo {
 public:
  Lfo() { }

  uint8_t Render(const SequencerSettings& sequence) {
    uint8_t value;

    // Ramp up the intensity envelope.
    if (!intensity_envelope_stage_) {
      intensity_ += intensity_increment_;
      if (intensity_ >= 16384) {
        intensity_ = 16383;
        intensity_envelope_stage_ = 1;
      }
    }
    
    triggered_ = phase_ < previous_phase_;
    
    // Compute the LFO value.
    switch (shape_) {
      case LFO_WAVEFORM_RAMP:
        value = phase_ >> 8;
        break;
        
      case LFO_WAVEFORM_S_H:
        if (triggered_) {
          value_ = Random::GetByte();
        }
        value = value_;
        break;

      case LFO_WAVEFORM_TRIANGLE:
        value = (phase_ & 0x8000) ?
            phase_ >> 7 :
            ~static_cast<uint8_t>(phase_ >> 7);
        break;

      case LFO_WAVEFORM_SQUARE:
        value = (phase_ & 0x8000) ? 255 : 0;
        break;
        
      case LFO_WAVEFORM_STEP_SEQUENCER:
        {
          uint8_t phase_to_step = phase_ >> 8;
          phase_to_step = MulScale8(phase_to_step, sequence.pattern_size);
          value = sequence.steps[phase_to_step].controller() << 4;
        }
        break;
        
      default:
        {
          uint8_t shape_offset = shape_ - LFO_WAVEFORM_WAVE_1;
          value = ResourcesManager::Lookup<uint8_t, uint8_t>(
              wav_res_wavetable_8 + (
                  static_cast<uint16_t>(shape_offset) << 7) + shape_offset,
              phase_ >> 9);
        }
        break;
    }
    previous_phase_ = phase_;
    phase_ += phase_increment_;

    // Apply the intensity envelope.
    return SignedMulScale8(
        static_cast<int8_t>(value) - 128,
        hardware_utils_op::ShiftRight6(intensity_)
    ) + 128;
  }

  void Reset() {
    ResetPhase();
    Trigger();
  }

  void ResetPhase() {
    phase_ = 0;
    triggered_ = 1;
  }

  void Trigger() {
    if (retrigger_) {
      ResetPhase();
    }
    intensity_ = 0;
    intensity_envelope_stage_ = 0;
  }

  void Update(uint8_t shape, uint16_t phase_increment,
              uint8_t attack, uint8_t retrigger) {
    shape_ = shape;
    phase_increment_ = phase_increment;
    intensity_increment_ = Envelope::ScaleEnvelopeIncrement(attack, 127);
    retrigger_ = retrigger;
  }
  
  uint8_t triggered() const { return triggered_; }

 private:
  // Phase increment.
  uint16_t phase_increment_;
  uint8_t intensity_envelope_stage_;
  uint16_t intensity_increment_;
  uint16_t intensity_;

  // Current phase of the lfo.
  uint16_t phase_;
  uint16_t previous_phase_;

  // Copy of the shape used by this lfo.
  uint8_t shape_;
  uint8_t retrigger_;
  uint8_t triggered_;

  // Current value of S&H.
  uint8_t value_;
  uint8_t step_;
  
  DISALLOW_COPY_AND_ASSIGN(Lfo);
};

}  // namespace hardware_shruthi

#endif  // HARDWARE_SHRUTHI_OSCILLATOR_H_
