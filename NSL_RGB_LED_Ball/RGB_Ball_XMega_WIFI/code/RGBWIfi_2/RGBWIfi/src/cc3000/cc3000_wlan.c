
#include "cc3000\cc3000_wlan.h"
#include "cc3000\cc3000.h"
#include "cc3000\cc3000_hci.h"

#include "string.h"

hci_status_t cc3000_wlan_disconnect() {

hci_status_t status;
	
	cc3000_hci_start_command(HCI_COMMAND_WLAN_DISCONNECT, 0);
	cc3000_hci_end_command();

	// Wait for command complete event
	status = cc3000_hci_wait_for(HCI_EVENT_WLAN_DISCONNECT);
	
	return status; 
}

hci_status_t cc3000_wlan_get_status(enum con_status_enum *con_status) 
{

hci_status_t status;
	
	cc3000_hci_start_command(HCI_COMMAND_WLAN_GET_STATUS, 0);
	cc3000_hci_end_command();

	// Wait for command complete event
	status = cc3000_hci_wait_for(HCI_EVENT_WLAN_GET_STATUS);
	
	*con_status = (enum con_status_enum) wlan_rx_buffer[5];
	return status; 
}


hci_status_t cc3000_wlan_get_scan_result(ap_entry_t *ap_entry) {

hci_status_t status;
uint8_t ssid_length;
uint8_t count;

	cc3000_hci_start_command(HCI_COMMAND_WLAN_GET_SCAN_RESULTS, 4);
	cc3000_hci_send_uint32_t(0x0000);	// magic
	cc3000_hci_end_command();
	
	status = cc3000_hci_wait_for(HCI_EVENT_WLAN_GET_SCAN_RESULTS);
	
	ap_entry->entry_count = wlan_rx_buffer[5];
	ap_entry->scan_status = wlan_rx_buffer[9];

	
	if (wlan_rx_buffer[13] & 0b10000000) {
		ap_entry->entry_valid = 1;
	} else {
		ap_entry->entry_valid = 0;
	}

	ap_entry->entry_valid = wlan_rx_buffer[13] >> 6;
	ap_entry->rssi = wlan_rx_buffer[13] & 0b01111111;
	ap_entry->security = wlan_rx_buffer[14] >> 6;
	ssid_length = wlan_rx_buffer[14] & 0b00111111;

	for (count = 0; count < ssid_length; count++) {
		ap_entry->ssid[count] = wlan_rx_buffer[17 + count];
	}
	ap_entry->ssid[count + 1] = '\0';
		
}


hci_status_t cc3000_wlan_set_scan_params(uint16_t scan_frequency /* seconds */,
								uint16_t channel_mask /* bitwise up to 13 channels (0x1fff), default 0x7ff */) {

	if (scan_frequency != 1) {	// 1 = magic, means 10 minute default
		scan_frequency = scan_frequency * 1000;
	}	
	cc3000_hci_start_command(HCI_COMMAND_WLAN_SET_SCANPARAM, 25*4);
	cc3000_hci_send_uint32_t(36);	// This 36 appears to be undocumented and magic
	cc3000_hci_send_uint32_t(scan_frequency);
	cc3000_hci_send_uint32_t(100);	// min dwell time
	cc3000_hci_send_uint32_t(100);	// max dwell time
	cc3000_hci_send_uint32_t(5);	// max probe request between dwell time
	cc3000_hci_send_uint32_t(channel_mask);	
	cc3000_hci_send_uint32_t((uint32_t)-80);	// rssi threshold
	cc3000_hci_send_uint32_t(0);	// SNR threshold
	cc3000_hci_send_uint32_t(205);	// probe tx power
	for (uint8_t count = 0; count < 16; count++) {
		cc3000_hci_send_uint32_t(2000);	// each channel entry periodic scan
	}
	cc3000_hci_end_command();
	
	return cc3000_hci_wait_for(HCI_EVENT_WLAN_SET_SCANPARAM);
}

hci_status_t cc3000_wlan_connect(uint8_t *ssid, uint8_t sec_type, uint8_t *key) {

	uint8_t status;
	uint8_t key_len;
	uint8_t ssid_len;

	ssid_len = strlen(ssid);
	key_len  = strlen(key);

	cc3000_hci_start_command(HCI_COMMAND_WLAN_CONNECT, 28 + ssid_len + key_len);
	cc3000_hci_send_uint32_t(0x0000001c);	// magic
	cc3000_hci_send_uint32_t(ssid_len);
	cc3000_hci_send_uint32_t(sec_type);
	cc3000_hci_send_uint32_t(0x10 + ssid_len);
	cc3000_hci_send_uint32_t(key_len);
	cc3000_hci_send_uint16_t(0);	// magic 0x0000
	
	// bssid
	
	for (uint8_t count = 0; count < 6; count++) {
		//*(ptr)++ = 0;
		cc3000_hci_send_uint8_t(0);
	}
	
	// ssid
	
	for (uint8_t count = 0; count < ssid_len; count++) {
		//*(ptr)++ = ssid[count];
		cc3000_hci_send_uint8_t(ssid[count]);
	}
	
	// key
	
	if ((key) && (key_len)) {
		for (uint8_t count = 0; count < key_len; count++) {
			//*(ptr)++ = key[count];
			cc3000_hci_send_uint8_t(key[count]);
		}
	}


	cc3000_hci_end_command();
	
	return cc3000_hci_wait_for(HCI_EVENT_WLAN_CONNECT);

}


hci_status_t cc3000_wlan_set_event_mask(uint16_t mask) {

	cc3000_hci_start_command(HCI_COMMAND_WLAN_SET_EVENT_MASK, 4);
	cc3000_hci_send_uint32_t(mask);	
	cc3000_hci_end_command();
	return cc3000_hci_wait_for(HCI_EVENT_WLAN_SET_EVENT_MASK);
	
}


hci_status_t cc3000_wlan_set_connection_policy(enum fast_connect_options_enum fast_connect,
											   enum open_ap_connect_options_enum open_ap_connect,
 											   enum use_profiles_options_enum use_profiles) {
 											   	
	cc3000_hci_start_command(HCI_COMMAND_WLAN_SET_CONNECTION_POLICY, 3*4);


	cc3000_hci_send_uint32_t(open_ap_connect);
	cc3000_hci_send_uint32_t(fast_connect);
	cc3000_hci_send_uint32_t(use_profiles);
	cc3000_hci_end_command();
	
	return cc3000_hci_wait_for(HCI_EVENT_WLAN_SET_CONNECTION_POLICY);	

}

hci_status_t cc3000_wlan_smart_config_start(uint8_t encrypted) {

	cc3000_hci_start_command(HCI_COMMAND_WLAN_SMART_CONFIG_START, 4);
	cc3000_hci_send_uint32_t(encrypted);
	cc3000_hci_end_command();
	
	return cc3000_hci_wait_for(HCI_EVENT_WLAN_SMART_CONFIG_START);	
}

hci_status_t cc3000_wlan_smart_config_stop(void) {

	cc3000_hci_start_command(HCI_COMMAND_WLAN_SMART_CONFIG_STOP, 0);
	cc3000_hci_end_command();
	
	return cc3000_hci_wait_for(HCI_EVENT_WLAN_SMART_CONFIG_STOP);	
}

hci_status_t cc3000_wlan_smart_config_set_prefix(void) {
	
	cc3000_hci_start_command(HCI_COMMAND_WLAN_SMART_CONFIG_SET_PREFIX, 3);
	cc3000_hci_send_uint8_t('T');
	cc3000_hci_send_uint8_t('T');
	cc3000_hci_send_uint8_t('T');
	cc3000_hci_end_command();
	
	return cc3000_hci_wait_for(HCI_EVENT_WLAN_SMART_CONFIG_SET_PREFIX);	
	
}

hci_status_t cc3000_wlan_del_profile(uint8_t profile_index) {
	
	cc3000_hci_start_command(HCI_COMMAND_WLAN_DEL_PROFILE, 4);
	cc3000_hci_send_uint32_t(profile_index);
	cc3000_hci_end_command();
	
	return cc3000_hci_wait_for(HCI_EVENT_WLAN_DEL_PROFILE);	
}
