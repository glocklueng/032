#include "cc3000\cc3000_patch.h"
#include "cc3000\cc3000.h"
#include "cc3000\cc3000_hci.h"
#include "cc3000\cc3000_nvmem.h"
#include "user_board.h"

#include <string.h>


const unsigned char cRMdefaultParams[128] = { 0x03, 0x00, 0x01, 0x01, 0x14, 0x14, 0x00, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x23, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x23, 0x23, 0x23, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x01, 0x77, 0x80, 0x1D, 0x1F, 0x22, 0x26, 0x28, 0x29, 0x1A, 0x1F, 0x22, 0x24, 0x26, 0x28, 0x16, 0x1D, 0x1E, 0x20, 0x24, 0x25, 0x1E, 0x2D, 0x01, 0x02, 0x02, 0x02, 0x02, 0x00, 0x15, 0x15, 0x15, 0x11, 0x15, 0x15, 0x0E, 0x00};

// array to store RM parameters from EEPROM
unsigned char cRMParamsFromEeprom[128];

// array to store MAC address from EEPROM
unsigned char cMacFromEeprom[MAC_ADDR_LEN];

// 2 dim array to store address and length of new FAT
const unsigned short aFATEntries[2][NVMEM_RM_FILEID + 1] =
/*  address     */  {{0x50,     0x1f0,  0x390,  0x1390,     0x2390,     0x4390,     0x6390,     0x63a0,     0x63b0,     0x63f0,     0x6430,     0x6830},
	/*  length  */  {0x1a0,     0x1a0,  0x1000,     0x1000,     0x2000,     0x2000,     0x10,   0x10,   0x40,   0x40,   0x400,  0x200}
};
/* 0. NVS */
/* 1. NVS Shadow */
/* 2. Wireless Conf */
/* 3. Wireless Conf Shadow */
/* 4. BT (WLAN driver) Patches */
/* 5. WiLink (Firmware) Patches */
/* 6. MAC addr */
/* 7. Frontend Vars */
/* 8. IP config */
/* 9. IP config Shadow */
/* 10. Bootloader Patches */
/* 11. Radio Module params */
/* 12. AES128 for smart config */
/* 13. user file */
/* 14. user file */
/* 15. user file */

unsigned char   *pRMParams;
//.unsigned short  index;

//*****************************************************************************
//
//! fat_write_content
//!
//! \param[in] file_address  array of file address in FAT table:\n
//!                      this is the absolute address of the file in the EEPROM.
//! \param[in] file_length  array of file length in FAT table:\n
//!                      this is the upper limit of the file size in the EEPROM.
//!
//! \return on succes 0, error otherwise
//!
//! \brief  parse the FAT table from eeprom
//
//*****************************************************************************
uint8_t cc3000_patch_fat_write_content ( uint16_t *file_address, uint16_t *file_length )
{
	uint16_t  iindex = 0;
	uint8_t   ucStatus;
	uint8_t   fatTable[48];
	uint8_t*  fatTablePtr = fatTable;

	// first, write the magic number
	ucStatus = cc3000_nvmem_write ( 16, 2, 0, "LS" );

	for ( ; iindex <= NVMEM_RM_FILEID; iindex++ ) {
		// write address low char and mark as allocated
		*fatTablePtr++ = ( unsigned char ) ( file_address[iindex] & 0xff ) | 0b00000001; // BIT0;

		// write address high char
		*fatTablePtr++ = ( unsigned char ) ( ( file_address[iindex]>>8 ) & 0xff );

		// write length low char
		*fatTablePtr++ = ( unsigned char ) ( file_length[iindex] & 0xff );

		// write length high char
		*fatTablePtr++ = ( unsigned char ) ( ( file_length[iindex]>>8 ) & 0xff );
	}

	// second, write the FAT
	// write in two parts to work with tiny driver
	ucStatus = cc3000_nvmem_write ( 16, 24, 4, fatTable );
	ucStatus = cc3000_nvmem_write ( 16, 24, 24+4, &fatTable[24] );

	// third, we want to erase any user files
	memset ( fatTable, 0, sizeof ( fatTable ) );
	ucStatus = cc3000_nvmem_write ( 16, 16, 52, fatTable );

	return ucStatus;
}


hci_status_t mac_status;

unsigned char ucStatus_Dr, ucStatus_FW, return_status = 0xFF;
//signed char mac_status = -1;
unsigned char counter = 0;


void cc3000_patch_store_data()
{

}

void cc3000_patch_fix_fat()
{

}


void cc3000_patch_restore_data ( void )
{


}

// convert to internal flash format

hci_status_t cc3000_nvmem_write_patch ( uint8_t file_id, uint16_t length, uint16_t mem_location )
{
	uint8_t status = 0;
	return status;
}

void cc3000_patch_update_dr ( void )
{


}

void cc3000_patch_update_fw ( void )
{
}

void wait_enter_key()
{
}

void cc3000_patch_execute()
{

}


