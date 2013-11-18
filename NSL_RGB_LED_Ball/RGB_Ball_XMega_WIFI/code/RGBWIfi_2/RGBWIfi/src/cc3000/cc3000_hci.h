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

#ifndef __cc3000_hci_h
#define __cc3000_hci_h

#include "user_board.h"
// locations in the buffer

#define POS_HCI_STATUS 4
	
// HCI packet types

#define  HCI_TYPE_COMMAND   0x01
#define  HCI_TYPE_DATA		0x02
#define  HCI_TYPE_PATCH     0x03
#define  HCI_TYPE_EVENT     0x04

#define HCI_EVENT_PATCHES_DRV			1
#define HCI_EVENT_PATCHES_FW			2
#define HCI_EVENT_PATCHES_BOOTLOAD		3

// GENERAL

#define HCI_COMMAND_SIMPLE_LINK_START 	0x4000
#define HCI_COMMAND_READ_BUFFER_SIZE  	0x400B

#define HCI_EVENT_READ_BUFFER_SIZE    HCI_COMMAND_READ_BUFFER_SIZE

// WLAN

#define  HCI_COMMAND_WLAN_CONNECT  							0x0001
#define  HCI_COMMAND_WLAN_DISCONNECT  						0x0002
#define  HCI_COMMAND_WLAN_SET_SCANPARAM						0x0003
#define  HCI_COMMAND_WLAN_SET_CONNECTION_POLICY  			0x0004
#define  HCI_COMMAND_WLAN_ADD_PROFILE						0x0005
#define  HCI_COMMAND_WLAN_DEL_PROFILE						0x0006
#define  HCI_COMMAND_WLAN_GET_SCAN_RESULTS					0x0007
#define  HCI_COMMAND_WLAN_SET_EVENT_MASK					0x0008
#define  HCI_COMMAND_WLAN_GET_STATUS						0x0009
#define  HCI_COMMAND_WLAN_SMART_CONFIG_START				0x000A
#define  HCI_COMMAND_WLAN_SMART_CONFIG_STOP					0x000B
#define  HCI_COMMAND_WLAN_SMART_CONFIG_SET_PREFIX			0x000C
#define  HCI_COMMAND_WLAN_CONFIGURE_PATCH					0x000D

// responses to commands:

#define HCI_EVENT_WLAN_SET_SCANPARAM      					HCI_COMMAND_WLAN_SET_SCANPARAM
#define HCI_EVENT_WLAN_SET_CONNECTION_POLICY 				HCI_COMMAND_WLAN_SET_CONNECTION_POLICY
#define HCI_EVENT_WLAN_GET_SCAN_RESULTS						HCI_COMMAND_WLAN_GET_SCAN_RESULTS
#define HCI_EVENT_WLAN_SET_EVENT_MASK						HCI_COMMAND_WLAN_SET_EVENT_MASK
#define HCI_EVENT_WLAN_GET_STATUS							HCI_COMMAND_WLAN_GET_STATUS
#define HCI_EVENT_WLAN_SMART_CONFIG_START					HCI_COMMAND_WLAN_SMART_CONFIG_START
#define HCI_EVENT_WLAN_SMART_CONFIG_STOP					HCI_COMMAND_WLAN_SMART_CONFIG_STOP
#define HCI_EVENT_WLAN_SMART_CONFIG_SET_PREFIX				HCI_COMMAND_WLAN_SMART_CONFIG_SET_PREFIX
#define HCI_EVENT_WLAN_CONNECT  							HCI_COMMAND_WLAN_CONNECT
#define HCI_EVENT_WLAN_DISCONNECT 							HCI_COMMAND_WLAN_DISCONNECT
#define HCI_EVENT_WLAN_ADD_PROFILE  						HCI_COMMAND_WLAN_ADD_PROFILE
#define HCI_EVENT_WLAN_DEL_PROFILE							HCI_COMMAND_WLAN_DEL_PROFILE

#define HCI_EVENT_WLAN_UNSOL_BASE     			0x8000

#define HCI_EVENT_WLAN_UNSOL_CONNECT  	 		0x0001 + HCI_EVENT_WLAN_UNSOL_BASE
#define HCI_EVENT_WLAN_UNSOL_DISCONNECT   		0x0002 + HCI_EVENT_WLAN_UNSOL_BASE
#define HCI_EVENT_WLAN_UNSOL_INIT         		0x0004 + HCI_EVENT_WLAN_UNSOL_BASE

#define HCI_EVENT_WLAN_TX_COMPLETE         		0x0008 + HCI_EVENT_WLAN_UNSOL_BASE
#define HCI_EVENT_WLAN_UNSOL_DHCP         		0x0010 + HCI_EVENT_WLAN_UNSOL_BASE

#define HCI_EVENT_WLAN_ASYNC_PING_REPORT  		0x0040 + HCI_EVENT_WLAN_UNSOL_BASE
#define HCI_EVENT_WLAN_ASYNC_SMART_CONFIG_DONE	0x0080 + HCI_EVENT_WLAN_UNSOL_BASE
#define HCI_EVENT_WLAN_KEEPALIVE			 	0x0200 + HCI_EVENT_WLAN_UNSOL_BASE


// socket

#define HCI_COMMAND_SOCKET_BASE		0x1000

#define HCI_COMMAND_SOCKET			0x1001
#define HCI_COMMAND_BIND			0x1002
#define HCI_COMMAND_RECV			0x1004
#define HCI_COMMAND_ACCEPT			0x1005
#define HCI_COMMAND_LISTEN			0x1006
#define HCI_COMMAND_CONNECT			0x1007
#define HCI_COMMAND_BSD_SELECT		0x1008
#define HCI_COMMAND_SETSOCKOPT		0x1009
#define HCI_COMMAND_GETSOCKOPT		0x100A
#define HCI_COMMAND_CLOSE_SOCKET	0x100B
#define HCI_COMMAND_RECVFROM		0x100D
#define HCI_COMMAND_GETHOSTNAME		0x1010
#define HCI_COMMAND_MDNS_ADVERTISE	0x1011

#define  HCI_EVENT_SOCKET              HCI_COMMAND_SOCKET
#define  HCI_EVENT_BIND                HCI_COMMAND_BIND
#define  HCI_EVENT_RECV                HCI_COMMAND_RECV
#define  HCI_EVENT_ACCEPT              HCI_COMMAND_ACCEPT
#define  HCI_EVENT_LISTEN              HCI_COMMAND_LISTEN
#define  HCI_EVENT_CONNECT             HCI_COMMAND_CONNECT
#define  HCI_EVENT_SELECT              HCI_COMMAND_BSD_SELECT
#define  HCI_EVENT_CLOSE_SOCKET        HCI_COMMAND_CLOSE_SOCKET
#define  HCI_EVENT_RECVFROM            HCI_COMMAND_RECVFROM
#define  HCI_EVENT_SETSOCKOPT          HCI_COMMAND_SETSOCKOPT
#define  HCI_EVENT_GETSOCKOPT          HCI_COMMAND_GETSOCKOPT
#define  HCI_EVENT_GETHOSTBYNAME   	   HCI_COMMAND_GETHOSTNAME
#define  HCI_EVENT_MDNS_ADVERTISE	   HCI_COMMAND_MDNS_ADVERTISE

// data

#define HCI_DATA_BASE				0x80

#define HCI_COMMAND_SEND			0x01 + HCI_DATA_BASE
#define HCI_COMMAND_SENDTO			0x03 + HCI_DATA_BASE
#define HCI_DATA_BSD_RECVFROM		0x04 + HCI_DATA_BASE
#define HCI_DATA_BSD_RECV			0x05 + HCI_DATA_BASE

// NVMEM

#define HCI_COMMAND_NVMEM_BASE			0x0200


#define HCI_COMMAND_NVMEM_READ    		0x0001 + HCI_COMMAND_NVMEM_BASE
#define HCI_COMMAND_NVMEM_CREATE_ENTRY 	0x0003 + HCI_COMMAND_NVMEM_BASE
#define HCI_COMMAND_NVMEM_WRITE_PATCH	0x0004 + HCI_COMMAND_NVMEM_BASE
#define HCI_COMMAND_NVMEM_SWAP_ENTRY  	0x0005 + HCI_COMMAND_NVMEM_BASE
#define HCI_COMMAND_READ_SP_VERSION  	0x0007 + HCI_COMMAND_NVMEM_BASE 
#define HCI_COMMAND_NVMEM_WRITE   		0x0090

#define HCI_EVENT_NVMEM_CREATE_ENTRY    	HCI_COMMAND_NVMEM_CREATE_ENTRY
#define HCI_EVENT_NVMEM_SWAP_ENTRY			HCI_COMMAND_NVMEM_SWAP_ENTRY

#define HCI_EVENT_NVMEM_READ     			HCI_COMMAND_NVMEM_READ
#define HCI_EVENT_NVMEM_WRITE    			0x0202


// network

#define HCI_COMMAND_NETWORK_BASE		0x2000

#define HCI_NETWORK_DHCP				0x0001 + HCI_COMMAND_NETWORK_BASE
#define HCI_NETWORK_PING_SEND           0x0002 + HCI_COMMAND_NETWORK_BASE
#define HCI_NETWORK_PING_REPORT         0x0003 + HCI_COMMAND_NETWORK_BASE
#define HCI_NETWORK_PING_STOP           0x0004 + HCI_COMMAND_NETWORK_BASE
#define HCI_NETWORK_IPCONFIG            0x0005 + HCI_COMMAND_NETWORK_BASE
#define HCI_NETWORK_ARP_FLUSH			0x0006 + HCI_COMMAND_NETWORK_BASE
#define HCI_NETWORK_SET_DEBUG_LEVEL		0x0008 + HCI_COMMAND_NETWORK_BASE
#define HCI_NETWORK_SET_TIMERS			0x0009 + HCI_COMMAND_NETWORK_BASE

// evnts

#define HCI_EVENT_PATCHES_REQ   0x1000
#define HCI_EVENT_SEND          0x1003
#define HCI_EVENT_WRITE         0x100E
#define HCI_EVENT_SENDTO        0x100F

#define HCI_EVENT_UNSOL_BASE     0x4000

#define HCI_EVENT_DATA_UNSOL_FREE_BUFF    		0x0100 + HCI_EVENT_UNSOL_BASE


// NVMEM


#define HCI_EVENT_READ_SP_VERSION 			HCI_COMMAND_READ_SP_VERSION

#define HCI_EVENT_INPROGRESS    			0xFFFF

// Fake event

#define HCI_EVENT_CC3000_CAN_SHUT_DOWN 0x99

#define HCI_DATA_RECVFROM       0x84
#define HCI_DATA_RECV           0x85
#define HCI_DATA_NVMEM          0x91

typedef int8_t hci_status_t;

/**

	\brief Send an HCI command
	
	Send the HCI command specified
	
	\param opcode HCI opcode to send
	\param args_length length of arguments to send
	\param args pointer to arguments to send
*/	
	
void cc3000_hci_send_command(uint16_t opcode, uint8_t args_length, uint8_t *args);

/**

	\brief Send HCI data
	
	Send the HCI data specified
	
	\param opcode HCI opcode to send
	\param args_length length of arguments to send
	\param args pointer to arguments to send
	\param data_length length of data to send
	\param pointer to data to send 
	
*/	
void cc3000_hci_send_data(uint8_t opcode, uint8_t args_length, uint8_t *args, uint16_t data_length, uint8_t *data);


/**

	\brief Start an HCI command
	
	Use this function to start an HCI command transmission
	
	\param opcode to send
	\param args_length argument length that will be transmitted
*/

uint8_t cc3000_hci_start_command(uint16_t opcode, uint8_t args_length);

/**

	\brief End an HCI command
	
	Use this fucntion to complete an HCI command
	
*/	

void cc3000_hci_end_command(void);

/**

	\brief Get the SPI layer to do its thing, then handle the HCI transaction

	Calls SPI receive, and then processes transaction at the HCI layer
*/	

uint16_t cc3000_hci_receive(void);

/**

	\brief Process the received HCI event
	
	Handle the received event, especially for asynchronous transactions
	
	
*/	

uint16_t cc3000_hci_handle_received_transaction(void);

/**

	\brief Transmit uint32_t to the HCI connection

*/

void cc3000_hci_send_uint32_t(uint32_t u);

/**

	\brief Transmit uint16_t to the HCI connection

*/


void cc3000_hci_send_uint16_t(uint16_t u);

/**

	\brief Transmit uint16_t to the HCI connection

*/

void cc3000_hci_send_uint8_t(uint8_t u);

/**

	\brief Wait for specified HCI command
	
	Keep receiving packets until specified HCI command or event is received

*/

hci_status_t cc3000_hci_wait_for(uint16_t wait_for_command);

	
/** 
	\brief Callback for connection status change
*/
	
void cc3000_callback_connect_status(uint8_t connected, uint8_t dhcp);


uint8_t cc3000_hci_start_data(uint8_t opcode, uint8_t args_length, uint16_t data_length);
void cc3000_hci_end_data(void);

extern hci_status_t status;

#endif
