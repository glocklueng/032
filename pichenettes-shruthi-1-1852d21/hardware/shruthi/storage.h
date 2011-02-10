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
// Storage for patches, sequences and system settings.

#ifndef HARDWARE_SHRUTHI_STORAGE_H_
#define HARDWARE_SHRUTHI_STORAGE_H_

#include <avr/eeprom.h>
#include <string.h>

#include "hardware/base/base.h"
#include "hardware/hal/devices/external_eeprom.h"
#include "hardware/shruthi/display.h"
#include "hardware/shruthi/patch.h"
#include "hardware/shruthi/sequencer_settings.h"
#include "hardware/shruthi/shruthi.h"
#include "hardware/shruthi/system_settings.h"

namespace hardware_shruthi {
  
enum SysExReceptionState {
  RECEIVING_HEADER = 0,
  RECEIVING_COMMAND = 1,
  RECEIVING_DATA = 2,
  RECEIVING_FOOTER = 3,
  RECEPTION_OK = 4,
  RECEPTION_ERROR = 5,
};

template<typename T>
class StorageConfiguration { };

template<> class StorageConfiguration<Patch> {
 public:
  enum {
    num_internal = 16,
    num_external = 64,
    offset_internal = 16,
    offset_external = 0,
    size = sizeof(Patch),
    sysex_object_id = 0x01,
  };
};

template<> class StorageConfiguration<SequencerSettings> {
 public:
  enum {
    num_internal = 16,
    num_external = 64,
    offset_internal = StorageConfiguration<Patch>::offset_internal + \
        StorageConfiguration<Patch>::num_internal * sizeof(Patch),
    offset_external = StorageConfiguration<Patch>::offset_external + \
        StorageConfiguration<Patch>::num_external * sizeof(Patch),
    size = sizeof(SequenceStep) * kNumSteps,
    sysex_object_id = 0x02,  
  };
};

class Storage {
 public:
  static void Init();
   
  static inline uint8_t sysex_rx_state() {
    return sysex_rx_state_;
  }
  
  template<typename T>
  static uint16_t size() {
    return StorageConfiguration<T>::num_internal +
           StorageConfiguration<T>::num_external * num_accessible_banks_;
  }
  
  static uint16_t addressable_space_size() {
    return hardware_hal::kInternalEepromSize + num_accessible_banks_ * kBankSize;
  }

  template<typename T>
  static void SysExDump(T* ptr) {
    SysExDumpBuffer(
        ptr->saved_data(),
        StorageConfiguration<T>::sysex_object_id,
        0,
        StorageConfiguration<T>::size);
  }
  
  static void SysExBulkDump();
  
  static void SysExReceive(uint8_t sysex_rx_byte);

  template<typename T>
  static void Backup(T* ptr) {
    memcpy(undo_buffer_, ptr->saved_data(), StorageConfiguration<T>::size);
  }
  template<typename T>
  static void Restore(T* ptr) {
    memcpy(ptr->saved_data(), undo_buffer_, StorageConfiguration<T>::size);
  }
  
  template<typename T>
  static void Write(T* ptr, uint16_t slot) {
    ptr->PrepareForWrite();
    if (slot < StorageConfiguration<T>::num_internal) {
      eeprom_write_block(
          (uint8_t *)(ptr->saved_data()),
          address<T>(slot),
          StorageConfiguration<T>::size);
      
    } else {
      WriteExternal(
          (uint8_t *)(ptr->saved_data()),
          (uint16_t)(address<T>(slot)),
          StorageConfiguration<T>::size);
    }
  }
  template<typename T>
  static void Load(T* ptr, uint16_t slot) {
    if (slot < StorageConfiguration<T>::num_internal) {
      eeprom_read_block(
          load_buffer_,
          address<T>(slot),
          StorageConfiguration<T>::size);
    } else {
      ReadExternal(
          load_buffer_,
          (uint16_t)(address<T>(slot)),
          StorageConfiguration<T>::size);
    }
    AcceptData(ptr, load_buffer_);
  }
  
  template<typename T>
  static uint8_t AcceptData(T* ptr, uint8_t* data) {
    uint8_t success = ptr->CheckBuffer(data);
    if (success) {
      memcpy(ptr->saved_data(), data, StorageConfiguration<T>::size);
      ptr->Update();
    }
    return success;
  }
   
 private:
  template<typename T>
  static uint8_t* address(uint16_t slot) {
    if (slot < StorageConfiguration<T>::num_internal) {
      return (uint8_t*)(
          StorageConfiguration<T>::offset_internal + \
          StorageConfiguration<T>::size * slot);
    } else {
      slot -= StorageConfiguration<T>::num_internal;
      uint16_t base = StorageConfiguration<T>::offset_external;
      while (slot >= StorageConfiguration<T>::num_external) {
        slot -= StorageConfiguration<T>::num_external;
        base += kBankSize;
      }
      return (uint8_t*)(base + StorageConfiguration<T>::size * slot);
    }
  }

 private:
  static void WriteExternal(const uint8_t* data, uint16_t address, uint8_t size);
  static void ReadExternal(uint8_t* data, uint16_t address, uint8_t size);
  static void SysExParseCommand();
  static void SysExAcceptBuffer();
  static void SysExDumpBuffer(
      uint8_t* data,
      uint8_t command,
      uint8_t argument,
      uint8_t size);
  static uint8_t undo_buffer_[sizeof(Patch)];
  static uint8_t load_buffer_[sizeof(Patch)];
  static uint8_t sysex_rx_buffer_[129];
  static uint8_t* sysex_rx_destination_;
  static uint16_t sysex_rx_bytes_received_;
  static uint16_t sysex_rx_expected_size_;
  static uint8_t sysex_rx_state_;
  static uint8_t sysex_rx_checksum_;
  static uint8_t sysex_rx_command_[2];
  static uint8_t num_accessible_banks_;
};

extern hardware_hal::ExternalEeprom<kBankSize * kMaxNumBanks> external_eeprom;

extern Lcd lcd;


}  // namespace hardware_shruthi

#endif  // HARDWARE_SHRUTHI_PATCH_H_
