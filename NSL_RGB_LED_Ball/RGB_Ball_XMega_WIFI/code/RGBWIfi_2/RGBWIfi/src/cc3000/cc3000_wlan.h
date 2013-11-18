#ifndef __cc3000_wlan_h
#define __cc3000_wlan_h

#include "user_board.h"
#include "cc3000\cc3000.h"
#include "cc3000\cc3000_hci.h"

#define      WLAN_SEC_UNSEC 0
#define      WLAN_SEC_WEP	1
#define      WLAN_SEC_WPA	2
#define      WLAN_SEC_WPA2	3

enum open_ap_connect_options_enum {
	open_ap_auto_connect_disabled = 0,
	open_ap_auto_connect_enabled = 1
};

enum fast_connect_options_enum {
	fast_connect_disabled = 0,
	fast_connect_enabled = 1
};

	

enum use_profiles_options_enum {
	use_profiles_disabled = 0,
	use_profiles_enabled = 1
};

enum con_status_enum {
	 con_disconnected = 0,
	 con_scanning = 1,
	 con_connecting = 2,
	 con_connected = 3
};

/**
	\struct ap_entry_t AP access point
*/	
	
typedef struct _ap_entry_t {
		uint8_t entry_count;	//< Number of this entry point (zero for last)
		uint8_t scan_status;	//< Status of the scan 0 = aged results, 1 = results valid, 2 = no result
		uint8_t entry_valid;	//< Validity of the entry, 0 = invalid, 1 = valid
		uint8_t rssi;			//< RSSI entry
		uint8_t security;		//< Secutity type, 0 = Open, 1 = WEP, 2 = WPA, 3 = WPA2
		uint8_t ssid[32 + 1];	//< SSID name
} ap_entry_t;

/**

	\brief Get connection status

	\param con_status Status of the connection
	
*/	
	

hci_status_t cc3000_wlan_get_status(enum con_status_enum *con_status);


/**

	\brief Set connection policy for connections on startup
	
	\param fast_connect Turn fast connect on or off.  Fast connect enables quick connection to the last access point, 
	or one of the stored profiles if this is not available.
	\param open_ap_connect_options If enabled, CC3000 will try to connect an open access point if none of the profiles work on startup.
	\param use_profiles_options If enabled, CC3000 will try and use one of the stored profiles on startup.
	
*/	
hci_status_t cc3000_wlan_set_connection_policy(enum fast_connect_options_enum fast_connect,
											   enum open_ap_connect_options_enum open_ap_connect,
 											   enum use_profiles_options_enum use_profiles);
/**

	\brief Set the parameters for scanning for hotspots
	
	\note Scanning for hotspots only occurs on startup (trigger to start up, disable the module and re-enable it)
	
	\param scan_frequency Seconds between scans
	\param channel_mask Bitwise channel mask of channels to scan up to 13 channels (default 0x7fff), up to 0x1fff
*/	
 											   
hci_status_t cc3000_wlan_set_scan_params(uint16_t scan_frequency, uint16_t channel_mask);

/**
	\brief Connect to a wifi router
	
	\param ssid Name of the network to connect to
	\param sec_type Network type (WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA, or WLAN_SEC_WPA2)
	\param key Password to the network (if using WEP, this must be in ASCII)
*/

hci_status_t cc3000_wlan_connect(uint8_t *ssid, uint8_t sec_type, uint8_t *key);

/**
	\brief Get a single access point scan result
	
	\param ap_entry Pointer to ap_entry_t
	
	\sa ap_entry_t
	
*/

hci_status_t cc3000_wlan_get_scan_result(ap_entry_t *ap_entry);

/**
	\brief Set the event mask to prevent asyncronous events
	
	\param Event Mask. A uint8_t that is 1 = mask event
	

*/

hci_status_t cc3000_wlan_set_event_mask(uint16_t mask);

/**
	\brief Disconnect from the current wifi access point
	
*/

hci_status_t cc3000_wlan_disconnect(void);

/**
	\brief Disconnect from the current wifi access point
	
*/

hci_status_t cc3000_wlan_disconnect(void);

/**

	\brief Listen for smart config messages and store profile. 

	\param encrypted 0 = not encrypted, 1 = encrypted
	
*/

hci_status_t cc3000_wlan_smart_config_start(uint8_t encrypted);

/**

	\brief Stop doing all that smart config stuff


*/

hci_status_t cc3000_wlan_smart_config_stop(void);

/**

	\brief Delete a stored connection profile

	\param profile_index Profile index to delete. Range 0-6, or 255 to delete all profiles.
	
*/
hci_status_t cc3000_wlan_del_profile(uint8_t profile_index);

/**

	\brief Set smart config prefix (to "TTT")

	
*/

hci_status_t cc3000_wlan_smart_config_set_prefix(void);

#endif
