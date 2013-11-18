/*

Copyright (c) 2013, Embedded Adventures, www.embeddedadventures.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
 
- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.

Contact us at admin@embeddedadventures.com

*/

#include "cc3000\cc3000_network.h"
#include "cc3000\cc3000.h"
#include "cc3000\cc3000_hci.h"

#include <string.h>

#define makelong(ch0, ch1, ch2, ch3) (uint(uint8(ch0)) | (uint(uint8(ch1)) << 8) | (uint(uint8(ch2)) << 16) | (uint(uint8(ch3)) << 24 ))

hci_status_t cc3000_network_ping(ip_addr_t ip_addr, uint8_t attempts, uint16_t size, uint32_t timeout) {

uint8_t status;

	cc3000_hci_start_command(HCI_NETWORK_PING_SEND, 4*sizeof(uint32_t));
	cc3000_hci_send_uint32_t((uint32_t)ip_addr.ip[3] );
	cc3000_hci_send_uint32_t(attempts);
	cc3000_hci_send_uint32_t(size);
	cc3000_hci_send_uint32_t(timeout);
	cc3000_hci_end_command();
	
	status = cc3000_hci_wait_for(HCI_NETWORK_PING_SEND);

	return status;
	
}

hci_status_t cc3000_network_ping_report() {

	cc3000_hci_send_command(HCI_NETWORK_PING_REPORT, 0, NULL);
	return cc3000_hci_wait_for(HCI_NETWORK_PING_REPORT);
	
}



int8_t cc3000_network_gethostbyname(char *host_name, ip_addr_t *out_ip_addr) {

int8_t ret_val;
uint8_t name_len;
hci_status_t status;
	name_len = strlen(host_name);
	
	if (name_len > HOSTNAME_MAX_LENGTH) {
		return -1;
	}

	cc3000_hci_start_command(HCI_COMMAND_GETHOSTNAME, 8 + name_len);
	cc3000_hci_send_uint32_t(8);	// magic number
	cc3000_hci_send_uint32_t(name_len);
	for (uint8_t count = 0; count < name_len; count++) {
		cc3000_hci_send_uint8_t(host_name[count]);
	}	
	cc3000_hci_end_command();
	
	status = cc3000_hci_wait_for(HCI_EVENT_GETHOSTBYNAME);
	
	ret_val = (int8_t)wlan_rx_buffer[5];
	
	out_ip_addr->ip[0] = wlan_rx_buffer[12];
	out_ip_addr->ip[1] = wlan_rx_buffer[11];
	out_ip_addr->ip[2] = wlan_rx_buffer[10];
	out_ip_addr->ip[3] = wlan_rx_buffer[9];
	
	return ret_val;
	
}

