
#include "user_board.h"
#include "cc3000\cc3000_nvmem.h"
#include "cc3000\cc3000.h"
#include "cc3000\cc3000_hci.h"


hci_status_t cc3000_nvmem_read_sp_version(uint8_t *sp_maj, uint8_t *sp_min) {

uint8_t status;

	cc3000_hci_send_command(HCI_COMMAND_READ_SP_VERSION, NULL, 0);
	cc3000_hci_wait_for(HCI_COMMAND_READ_SP_VERSION);
	
	status = wlan_rx_buffer[4];
	*sp_maj = wlan_rx_buffer[7];
	*sp_min = wlan_rx_buffer[8];

	return status;	
}
/*

NVMEM_NVS_FILEID, NVMEM_NVS_SHADOW_FILEID,
NVMEM_WLAN_CONFIG_FILEID, NVMEM_WLAN_CONFIG_SHADOW_FILEID,
NVMEM_WLAN_DRIVER_SP_FILEID, NVMEM_WLAN_FW_SP_FILEID,
NVMEM_MAC_FILEID, NVMEM_FRONTEND_VARS_FILEID,
NVMEM_IP_CONFIG_FILEID, NVMEM_IP_CONFIG_SHADOW_FILEID,
NVMEM_BOOTLOADER_SP_FILEID, NVMEM_RM_FILEID,
and user files 12-15.

*/


hci_status_t cc3000_nvmem_read(uint8_t file_id, uint8_t length, uint16_t offset, uint8_t *buffer) {

	uint8_t status;
	
	cc3000_hci_start_command(HCI_COMMAND_NVMEM_READ, 3*4);
	cc3000_hci_send_uint32_t(file_id);
	cc3000_hci_send_uint32_t(length);
	cc3000_hci_send_uint32_t(offset);
	cc3000_hci_end_command();
	
	status = cc3000_hci_wait_for(HCI_COMMAND_NVMEM_READ);

	if (cc3000_hci_wait_for(HCI_DATA_NVMEM)==0) {	// status ok
		
		//serial_print_str(" STATUS OK ");
		
		uint8_t hci_args_length = wlan_rx_buffer[2];	// 8 uint8_t args length
		uint16_t payload_length = wlan_rx_buffer[3] + (uint16_t)(wlan_rx_buffer[4] << 8);
		uint16_t data_length = payload_length - hci_args_length;

		
		for (uint8_t count = 0; count < data_length; count++) {
			*(buffer++) = wlan_rx_buffer[5 + hci_args_length + count];
		}
	}
	return status;
}

/*
NVMEM_WLAN_DRIVER_SP_FILEID, NVMEM_WLAN_FW_SP_FILEID,
NVMEM_MAC_FILEID, NVMEM_BOOTLOADER_SP_FILEID,
and user files 12-15.
*/

hci_status_t cc3000_nvmem_write(uint8_t file_id, uint8_t length, uint16_t offset, uint8_t *buffer) {

uint8_t status;

	cc3000_hci_start_command(HCI_COMMAND_NVMEM_WRITE,4*4);
	cc3000_hci_send_uint32_t(file_id);
	cc3000_hci_send_uint32_t(12);	// random 12 number
	cc3000_hci_send_uint32_t(length);
	cc3000_hci_send_uint32_t(offset);
	
	for (uint8_t count = 0; count < length; count++) {
		cc3000_hci_send_uint8_t(buffer[count]);
	}	
	
	cc3000_hci_end_command();
	
	status = cc3000_hci_wait_for(HCI_COMMAND_NVMEM_WRITE);
	
	return status;
}


hci_status_t cc3000_nvmem_set_mac_address(uint8_t *mac) {
	return  cc3000_nvmem_write(NVMEM_MAC_FILEID, MAC_ADDR_LEN, 0, mac);
}


hci_status_t cc3000_nvmem_get_mac_address(uint8_t *mac) {
	return  cc3000_nvmem_read(NVMEM_MAC_FILEID, MAC_ADDR_LEN, 0, mac);
}

