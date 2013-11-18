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

#include "user_board.h"

#include "cc3000\cc3000_socket.h"
#include "cc3000\cc3000.h"
#include "cc3000\cc3000_hci.h"
#include <string.h>

extern hci_status_t status;

int8_t cc3000_socket(uint8_t domain, uint8_t type, uint8_t protocol)
{

hci_status_t status;


 
	cc3000_hci_start_command(HCI_COMMAND_SOCKET, 3*4);

	cc3000_hci_send_uint32_t(domain);
	cc3000_hci_send_uint32_t(type);
	cc3000_hci_send_uint32_t(protocol);
	
	cc3000_hci_end_command();

	status = cc3000_hci_wait_for(HCI_EVENT_SOCKET);
    
    return (int8_t)status;
}


hci_status_t cc3000_connect(uint8_t sd, uint16_t port, ip_addr_t addr)
{

	cc3000_hci_start_command(HCI_COMMAND_CONNECT, 20);
    cc3000_hci_send_uint32_t(sd);
	cc3000_hci_send_uint32_t(0x00000008);	// magic
	cc3000_hci_send_uint32_t(8);	// addrlen
	cc3000_hci_send_uint16_t(AF_INET /*stSockAddr.sin_framily */);	
	cc3000_hci_send_uint16_t(htons(port) /*stSockAddr.sin_port */);
	cc3000_hci_send_uint32_t((uint32_t) addr.ip /*stSockAddr.sin_addr*/);
	cc3000_hci_end_command();

	return cc3000_hci_wait_for(HCI_EVENT_CONNECT);
    
}


#define POS_RECV_PARAMS_SD 			POS_HCI_STATUS 				+ 1
#define POS_RECV_PARAMS_NUM_BYTES 	POS_RECV_PARAMS_SD			+ 4
#define POS_RECV_PARAMS_FLAGS 		POS_RECV_PARAMS_NUM_BYTES	+ 4


int16_t cc3000_recv(uint8_t sd, uint8_t *buf, uint16_t len, uint8_t flags) {

uint8_t interrupts_enabled;
	
	interrupts_enabled = cc3000_interrupts_enabled;
	if (interrupts_enabled) {
		cc3000_interrupt_disable();
	}
	
	cc3000_hci_start_command(HCI_COMMAND_RECV, 3*4);

	cc3000_hci_send_uint32_t(sd);
	cc3000_hci_send_uint32_t(len);
	cc3000_hci_send_uint32_t(flags);
	
	cc3000_hci_end_command();
	debug_str("\nWaiting for response...");	

	status = cc3000_hci_wait_for(HCI_EVENT_RECV);

	// In return, we get 
	// int32 SocketDescriptor (8 bits will do us) - 5
	// int32 NumberOfBytes	(16 bits will do us)
	// int32 Flags
	
	// We will just assume that we are dealing with the socket we first thought of
	// In a more complex case of multiple sockets, we would need to separate this out
	// and probably have a more complex model (ie, a state machine)
	
	// uint8_t  sd2 = wlan_rx_buffer[POS_RECV_PARAMS_SD];
	uint16_t num_bytes = stream_to_uint16_t(&wlan_rx_buffer[POS_RECV_PARAMS_NUM_BYTES]);
	//uint8_t  flags2 = wlan_rx_buffer[POS_RECV_PARAMS_FLAGS];
	
	// debug_var("\nsd2=", sd2);
	debug_var("\nnum_bytes=", num_bytes);
	// debug_var("\nflags2=", flags2);
	debug_str("\ngot bytes = ");
	debug_int(num_bytes);
	
	// In case the number of bytes is more then zero - read data
	
	if (num_bytes > 0) {

		debug_str("*^*^*^*^*^*^*^*\n");
		debug_str("waiting for data\n");
		status = cc3000_hci_wait_for(HCI_DATA_RECV);
		uint8_t hci_opcode = wlan_rx_buffer[1]; 		// 8 uint8_t opcode for data
		uint8_t hci_args_length = wlan_rx_buffer[2];	// 8 uint8_t args length
		
		debug_str(" opcode = ");
		debug_int_hex(hci_opcode);
		debug_str(" args_length = ");
		debug_int_hex(hci_args_length);
		debug_nl();			
			
		uint16_t payload_length = wlan_rx_buffer[3] + (uint16_t)(wlan_rx_buffer[4] << 8);
		uint16_t data_length = payload_length - hci_args_length;
		
		debug_var(" payload_len = ", payload_length);
		debug_var("data_len = ", data_length);
		debug_str("\nData=<");
		
		for (uint8_t count = 0; count < data_length; count++) {
				debug_putc(wlan_rx_buffer[5 + hci_args_length + count]);
				*buf = wlan_rx_buffer[5 + hci_args_length + count];
				buf++;
			}
		
		debug_str(">\n\n");	
	}
	debug_str("returning\n");
	
	if (interrupts_enabled) {	// if we had them on, then turn them back on
		cc3000_interrupt_enable();
	}	
	return num_bytes;
}



int16_t cc3000_send(uint8_t sd, uint8_t *buf, uint16_t len, uint8_t flags) {

uint8_t args[16];
uint8_t *ptr;

	if (free_buffers == 0) {
		debug_str("Send: No free buffers!\n");
		return -2; // no free buffers;
	}
	
	free_buffers--;	// strangely, we the host must keep track of free buffers
	
	cc3000_hci_start_data(HCI_COMMAND_SEND, 16, len);
	cc3000_hci_send_uint32_t(sd);
	cc3000_hci_send_uint32_t(16-4);	
	cc3000_hci_send_uint32_t(len);
	cc3000_hci_send_uint32_t(flags);
	
	if (len > 0) {
		debug_str("\nData:\n");	
		for (uint16_t count = 0; count < len; count++) {
			cc3000_hci_send_uint8_t(buf[count]);
			debug_int_hex(buf[count]);
			debug_putc(' ');
		}
	}
	cc3000_hci_end_data();
	
	return len;
}

hci_status_t cc3000_socket_close(uint8_t sd) {


	cc3000_hci_start_command(HCI_COMMAND_CLOSE_SOCKET, 4);
	cc3000_hci_send_uint32_t(sd);
	cc3000_hci_end_command();
	
	status = cc3000_hci_wait_for(HCI_EVENT_CLOSE_SOCKET);
	
	return status;

}

hci_status_t cc3000_socket_mdns_advertise(uint8_t mdns_enabled, uint8_t *service_name) {

uint8_t service_name_length;

	service_name_length = strlen(service_name);
	if (service_name_length > MDNS_DEVICE_SERVICE_MAX_LENGTH) {
		return -1;
	}
	cc3000_hci_start_command(HCI_COMMAND_MDNS_ADVERTISE, 12 + service_name_length);
	cc3000_hci_send_uint32_t(mdns_enabled);
	cc3000_hci_send_uint32_t(8);	// magic
	cc3000_hci_send_uint32_t(service_name_length);
	for (uint8_t count = 0; count < service_name_length; count++) {
		cc3000_hci_send_uint8_t(service_name[count]);
	}	
	cc3000_hci_end_command();
	
	return cc3000_hci_wait_for(HCI_EVENT_MDNS_ADVERTISE);
	
}