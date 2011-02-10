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
// Polyphonic voice allocator. This is used when the Shruthi-1 is configured
// for poly-chaining.

#ifndef HARDWARE_SHRUTHI_VOICE_ALLOCATOR_H_
#define HARDWARE_SHRUTHI_VOICE_ALLOCATOR_H_

#include "hardware/base/base.h"

static const uint8_t kMaxPolyphony = 8;

namespace hardware_shruthi {

struct VoiceEntry {
  uint8_t note;
  uint8_t active;
};

class VoiceAllocator {
 public: 
  VoiceAllocator() { }
  static void Init() { size_ = 0; Clear(); }
  static void set_size(uint8_t size) {
    size_ = size;
  }
  static uint8_t NoteOn(uint8_t note);
  static uint8_t NoteOff(uint8_t note);

 private:
  static void Clear();
  static void Touch(uint8_t voice);
   
  static VoiceEntry pool_[kMaxPolyphony];
  // Holds the indices of the voices sorted by most recent usage.
  static uint8_t lru_[kMaxPolyphony];
  static uint8_t size_;

  DISALLOW_COPY_AND_ASSIGN(VoiceAllocator);
};

}  // namespace hardware_shruthi

#endif  // HARDWARE_SHRUTHI_VOICE_ALLOCATOR_H_
