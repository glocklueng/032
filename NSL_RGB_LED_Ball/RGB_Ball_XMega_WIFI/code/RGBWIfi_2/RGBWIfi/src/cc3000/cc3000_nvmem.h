#ifndef __cc3000_nvmem_h
#define __cc3000_nvmem_h

#include "cc3000\cc3000.h"
#include "cc3000\cc3000_hci.h"

/* NVMEM file ID - system files*/

#define NVMEM_NVS_FILEID 							(0)
#define NVMEM_NVS_SHADOW_FILEID 					(1)
#define NVMEM_WLAN_CONFIG_FILEID 					(2)
#define NVMEM_WLAN_CONFIG_SHADOW_FILEID 			(3)
#define NVMEM_WLAN_DRIVER_SP_FILEID					(4)
#define NVMEM_WLAN_FW_SP_FILEID						(5)
#define NVMEM_MAC_FILEID 							(6)
#define NVMEM_FRONTEND_VARS_FILEID 					(7)
#define NVMEM_IP_CONFIG_FILEID 						(8)
#define NVMEM_IP_CONFIG_SHADOW_FILEID 				(9)
#define NVMEM_BOOTLOADER_SP_FILEID 					(10)
#define NVMEM_RM_FILEID			 					(11)

/* NVMEM file ID - user files*/

#define NVMEM_AES128_KEY_FILEID	 					(12)
#define NVMEM_SHARED_MEM_FILEID	 					(13)

/*  max entry in order to invalid nvmem              */

#define NVMEM_MAX_ENTRY                              (16)



hci_status_t cc3000_nvmem_read_sp_version(uint8_t *sp_maj, uint8_t *sp_min);
hci_status_t cc3000_nvmem_read(uint8_t file_id, uint8_t length, uint16_t offset, uint8_t *buffer);
hci_status_t cc3000_nvmem_set_mac_address(uint8_t *mac);
hci_status_t cc3000_nvmem_get_mac_address(uint8_t *mac);
hci_status_t cc3000_nvmem_write(uint8_t file_id, uint8_t length, uint16_t offset, uint8_t *buffer);

#endif
