#ifndef __cc3000_patch_h
#define __cc3000_patch_h

#include "user_board.h"
void cc3000_patch_update_driver();
void cc3000_patch_restore_data();
void cc3000_patch_fix_fat();
void cc3000_patch_store_data();
uint8_t cc3000_patch_fat_write_content(uint16_t *file_address, uint16_t *file_length);

#endif
