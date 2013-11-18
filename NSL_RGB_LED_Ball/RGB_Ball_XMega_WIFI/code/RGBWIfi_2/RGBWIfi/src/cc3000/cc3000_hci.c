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

#include "cc3000\cc3000_hci.h"
#include "cc3000\cc3000.h"
#include "cc3000\cc3000_spi.h"

#include "user_board.h"

#define HCI_COMMAND_HEADER_LENGTH 4
#define HCI_DATA_HEADER_LENGTH 5


uint8_t padding_byte_required;

/*

HCI Command

uint8_t 	HCI message type
uint16_t 	HCI opcode (LSB first)
uint8_t	HCI arguments length
uint8_t    HCI arguments (HCI arguments length bytes long)

*/

void cc3000_hci_send_command(uint16_t opcode, uint8_t args_length, uint8_t *args) {
	
uint8_t padding_byte;
	debug_str("Send cop\n");
	debug_int_hex_16bit(opcode);
	debug_nl();
//	debug_str("\nSPI header:\n");
	padding_byte = cc3000_spi_start_send(HCI_COMMAND_HEADER_LENGTH + args_length);
//	debug_str("\nHCI header:\n");
	cc3000_spi_send(HCI_TYPE_COMMAND);
	cc3000_spi_send(opcode & 0xff);
	cc3000_spi_send(opcode >> 8);
	cc3000_spi_send(args_length);	
	if (args_length > 0) {
		debug_str("\nHCI args:\n");	
		for (uint16_t count = 0; count < args_length; count++) {
			cc3000_spi_send(args[count]);
			debug_int_hex(args[count]);
			debug_putc(' ');
		}
	}	
	debug_str("SPI padding:\n");
	cc3000_spi_finish_send(padding_byte);
	debug_str("Done.\n\n");
}


uint8_t cc3000_hci_start_command(uint16_t opcode, uint8_t args_length) {
	
	debug_str("sco");
	debug_int_hex_16bit(opcode);
	debug_nl();
	debug_str("\n");

	padding_byte_required = cc3000_spi_start_send(HCI_COMMAND_HEADER_LENGTH + args_length);
	
	debug_str("\nHCI header:\n");
	cc3000_spi_send(HCI_TYPE_COMMAND);
	cc3000_spi_send(opcode & 0xff);
	cc3000_spi_send(opcode >> 8);
	cc3000_spi_send(args_length);
	
	return padding_byte_required;
}

void cc3000_hci_end_command() {
	debug_str("SPI padding:\n");
	cc3000_spi_finish_send(padding_byte_required);
	debug_str("Done.\n\n");
}

uint8_t cc3000_hci_start_data(uint8_t opcode, uint8_t args_length, uint16_t data_length) {

uint16_t payload_length;

	payload_length = args_length + data_length;
		
	debug_str("\n---\nSending data opcode = ");
	debug_int_hex(opcode);
	debug_nl();
	
	debug_str("\nSPI header:\n");
	padding_byte_required = cc3000_spi_start_send(HCI_DATA_HEADER_LENGTH + payload_length);

	debug_str("\nHCI header:\n");
	cc3000_spi_send(HCI_TYPE_DATA);
	cc3000_spi_send(opcode);
	cc3000_spi_send(args_length);
	cc3000_spi_send(payload_length & 0xff);
	cc3000_spi_send(payload_length >> 8);
	
	return padding_byte_required;
}

		
void cc3000_hci_end_data() {
	debug_str("SPI padding:\n");
	cc3000_spi_finish_send(padding_byte_required);
	debug_str("Done.\n\n");
}

void cc3000_hci_send_uint32_t(uint32_t u) {

	
	cc3000_spi_send((uint8_t) (u & 0xff));
	debug_int_hex((uint8_t) (u & 0xff));
	debug_putc(' ');
	
	cc3000_spi_send((uint8_t) ((u >> 8 ) & 0xff));
	debug_int_hex((uint8_t) ((u >> 8 ) & 0xff));
	debug_putc(' ');
	
	cc3000_spi_send((uint8_t) ((u >> 16 ) & 0xff));
	debug_int_hex((uint8_t) ((u >> 16 ) & 0xff));
	debug_putc(' ');
	
	cc3000_spi_send((uint8_t) ((u >> 24 ) & 0xff));
	debug_int_hex((uint8_t) ((u >> 24 ) & 0xff));
	debug_putc(' ');

}

void cc3000_hci_send_uint16_t(uint16_t u) {

	
	cc3000_spi_send((uint8_t) (u & 0xff));
	debug_int_hex((uint8_t) (u & 0xff));
	debug_putc(' ');
	
	cc3000_spi_send((uint8_t) ((u >> 8 ) & 0xff));
	debug_int_hex((uint8_t) ((u >> 8 ) & 0xff));
	debug_putc(' ');

}

void cc3000_hci_send_uint8_t(uint8_t u) {

	cc3000_spi_send(u);
	debug_int_hex(u);
	debug_putc(' ');
	
}

/*

HCI Data

uint8_t 	HCI message type
uint8_t 	HCI opcode (LSB first) [Yes, inconcistanct with HCI command, which takes a 16bit opcode]
uint8_t	HCI arguments length
uint16_t   HCI data length + arguments length (payload length)
uint8_t    HCI arguments (HCI arguments length bytes long)
uint8_t	HCI data (data length bytes long)

*/
void cc3000_hci_send_data(uint8_t opcode, uint8_t args_length, uint8_t *args, uint16_t data_length, uint8_t *data) {

uint8_t padding_byte;
uint16_t payload_length;

	payload_length = args_length + data_length;

	debug_str("\n---\nSending data opcode = ");
	debug_int_hex(opcode);
	debug_nl();

	debug_str("\nSPI header:\n");
	padding_byte = cc3000_spi_start_send(HCI_DATA_HEADER_LENGTH + payload_length);

	debug_str("\nHCI header:\n");
	cc3000_spi_send(HCI_TYPE_DATA);
	cc3000_spi_send(opcode);
	cc3000_spi_send(args_length);
	cc3000_spi_send(payload_length & 0xff);
	cc3000_spi_send(payload_length >> 8);
	if (args_length > 0) {
		debug_str("\nHCI args:\n");	
		for (uint8_t count = 0; count < args_length; count++) {
			cc3000_spi_send(args[count]);
			debug_int_hex(args[count]);
			debug_putc(' ');
		}
	}
	
	if (data_length > 0) {
		debug_str("\nData:\n");	
		for (uint16_t count = 0; count < data_length; count++) {
			cc3000_spi_send(data[count]);
			debug_int_hex(data[count]);
			debug_putc(' ');
		}
	}
		
		
	debug_str("SPI padding:\n");
	cc3000_spi_finish_send(padding_byte);
	debug_str("Done.\n\n");
}

uint16_t cc3000_hci_handle_received_transaction() {

uint8_t hci_type;
uint16_t hci_opcode, hci_args_length;
		
	debug_str("\n---hci receive\n");

	hci_type = wlan_rx_buffer[0];
	
	debug_str("TYPE = ");
	debug_int_hex(hci_type);
	
	switch (hci_type) {
		case HCI_TYPE_EVENT:
			debug_str("\nEVENT: ");
			hci_opcode = wlan_rx_buffer[2];
			hci_opcode = hci_opcode << 8;
			hci_opcode = hci_opcode + wlan_rx_buffer[1];
			
			hci_args_length = wlan_rx_buffer[4];
			hci_args_length = hci_args_length << 8;
			hci_args_length = hci_args_length + wlan_rx_buffer[3];
			
			debug_str(" Command = ");
			debug_int_hex(hci_type);
			debug_str(" opcode = ");
			debug_int_hex_16bit(hci_opcode);
			debug_str(" args_length = ");
			debug_int_hex_16bit(hci_args_length);
			debug_nl();
			
			switch (hci_opcode) {
				case HCI_EVENT_WLAN_ASYNC_SMART_CONFIG_DONE: 
					cc3000_smart_config_complete = 1;
					debug_str("***Async smart config complete\n");
					break;
				case HCI_EVENT_WLAN_KEEPALIVE:  
					debug_str("***Keep alive\n"); 
					break;	
				case HCI_EVENT_WLAN_UNSOL_CONNECT:
					cc3000_callback_connect_status(1, 0);
					cc3000_connected = 1;
					debug_str("***Connect!!!!\n");
					break;
				case HCI_EVENT_WLAN_UNSOL_DISCONNECT:
					cc3000_callback_connect_status(0, 0);
					cc3000_connected = 0;
					cc3000_dhcp_complete = 0;
					debug_str("***Disconnect\n");	
					break;
				case HCI_EVENT_WLAN_UNSOL_DHCP:
					cc3000_callback_connect_status(1, 1);
					cc3000_dhcp_complete = 1;
					debug_str("***DHCP\nIP: ");	

					debug_int(wlan_rx_buffer[8]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[7]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[6]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[5]);
					debug_str("GW: ");

					debug_int(wlan_rx_buffer[16]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[15]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[14]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[13]);
					debug_nl();

					break;
				case HCI_EVENT_WLAN_UNSOL_INIT:
					debug_str("***Init complete\n");	//– Notification that the CC3000 device 
					break;
				case HCI_EVENT_WLAN_ASYNC_PING_REPORT:
				
					debug_str("***Ping complete\nPkts SENT:");	
					debug_int(wlan_rx_buffer[5]); // - 8 
					debug_str("\nPkts RECV:");
					debug_int(wlan_rx_buffer[9]); // - 12
					debug_str("\nMin time:");
					debug_int(wlan_rx_buffer[13] + (uint16_t)(wlan_rx_buffer[14] << 8));
					debug_str("\nMax time:");
					debug_int(wlan_rx_buffer[17] + (uint16_t)(wlan_rx_buffer[18] << 8));
					debug_str("\nAvg time:");
					debug_int(wlan_rx_buffer[21] + (uint16_t)(wlan_rx_buffer[22] << 8));
					debug_nl();
				
					break;
				case HCI_EVENT_DATA_UNSOL_FREE_BUFF:
					debug_str("Free buffer!\n");
					uint8_t buffer_count = wlan_rx_buffer[5]; // 16 bits but who cares
					for (uint8_t count = 0; count < buffer_count; count++) {
						uint16_t my_buffers_free = wlan_rx_buffer[5+2+2+(count*2)];
						debug_var(" Buffers_free+ = ", my_buffers_free);
						free_buffers += my_buffers_free;
					}
					debug_nl();
					break;	
			}			
			break;
		case HCI_TYPE_PATCH:
			debug_str("PATCH: ");
			break;
		case HCI_TYPE_DATA:
			debug_str("\nDATA: ");
			hci_opcode = wlan_rx_buffer[1];	// 8 uint8_t opcode
			
			hci_args_length = wlan_rx_buffer[2];	// 8 uint8_t args
			debug_str(" Data opcode = ");
			debug_int_hex_16bit(hci_opcode);
		
			break;
		case HCI_TYPE_COMMAND:
			debug_str("COMMAND: ");
			break;
	}
	return hci_opcode;	
}

uint16_t cc3000_hci_receive() {
	
	debug_str("\nSPI Receive\n");
	cc3000_spi_receive();
	// now process at higher layer (HCI)
	debug_str("\nNow process in HCI\n");
	uint16_t hci_opcode = cc3000_hci_handle_received_transaction();
	debug_var("hci_opcode=", hci_opcode);
	cc3000_cs_disable();
	return hci_opcode;
}

hci_status_t cc3000_hci_wait_for(uint16_t wait_for_command) 
{
	uint16_t command;
	hci_status_t status;
	uint8_t interrupts_enabled;

	debug_str("\nhci_wait_for: Waiting for irq...\n");

	interrupts_enabled = cc3000_interrupts_enabled;
	if (interrupts_enabled) {
		cc3000_interrupt_disable();
	}

	do {
		while (cc3000_read_irq_pin() != 0);
		
		command = cc3000_hci_receive();

	} while (wait_for_command && command != wait_for_command);

	debug_str("-----Finally got ");
	debug_int_hex_16bit(wait_for_command);
	debug_nl();

	// grab status before we enable interrupts
	status = wlan_rx_buffer[POS_HCI_STATUS];
	
	if (interrupts_enabled) {	// if we had them on, then turn them back on

		cc3000_interrupt_enable();
	}	
	return 	status; 
}

