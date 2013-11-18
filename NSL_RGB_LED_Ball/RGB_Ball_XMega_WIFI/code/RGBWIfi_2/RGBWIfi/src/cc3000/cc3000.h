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


#ifndef __cc3000_h
#define __cc3000_h

#include "user_board.h"

#define	MAC_ADDR_LEN	6

#define EFAIL -1

#define TINY_CC3000_MAXIMAL_RX_SIZE 44
#define TINY_CC3000_MAXIMAL_TX_SIZE 59
#define HCI_DATA_HEADER_SIZE                    (5)
#define HCI_EVNT_WLAN_UNSOL_BASE                (0x8000)
#define HCI_EVNT_WLAN_UNSOL_CONNECT  	          (0x0001 + HCI_EVNT_WLAN_UNSOL_BASE)
#define HCI_EVNT_WLAN_UNSOL_DISCONNECT          (0x0002 + HCI_EVNT_WLAN_UNSOL_BASE)
#define HCI_EVNT_WLAN_UNSOL_DHCP                (0x0010 + HCI_EVNT_WLAN_UNSOL_BASE)
#define HCI_EVNT_WLAN_ASYNC_PING_REPORT         (0x0040 + HCI_EVNT_WLAN_UNSOL_BASE)
#define HCI_EVNT_WLAN_ASYNC_SIMPLE_CONFIG_DONE  (0x0080 + HCI_EVNT_WLAN_UNSOL_BASE)
#define HCI_EVNT_WLAN_KEEPALIVE			            (0x0200  + HCI_EVNT_WLAN_UNSOL_BASE)
#define HCI_EVNT_BSD_TCP_CLOSE_WAIT      (0x0800 + HCI_EVNT_WLAN_UNSOL_BASE)

/*
Buffer size increased to 130 following the add_profile() with WEP security
  which requires TX buffer size of 130 bytes: 
  HEADERS_SIZE_EVNT + WLAN_ADD_PROFILE_WEP_PARAM_LEN + MAX SSID LEN + 4 * MAX KEY LEN = 130
  MAX SSID LEN = 32 
  MAX SSID LEN = 13 (with add_profile only ascii key setting is supported, 
  therfore maximum key size is 13)*/

// Given the maximal data size MAX_DATA that is expected to be received by application, the required buffer is:
// Using recv() or recvfrom():
// max(CC3000_MINIMAL_RX_SIZE, MAX_DATA + HEADERS_SIZE_DATA + fromlen + ucArgsize + 1)
// Using gethostbyname() with minimal buffer size will limit the host name returned to 99 bytes only.  
// The 1 is used for the overrun detection
#define CC3000_MINIMAL_RX_SIZE      (130+1)
#define CC3000_MAXIMAL_RX_SIZE      (1519 + 1)

// Given the maximal data size MAX_DATA, the required buffer is:
// Using Sendto:
// max(CC3000_MINIMAL_TX_SIZE, MAX_DATA + SPI_HEADER_SIZE + SOCKET_SENDTO_PARAMS_LEN + SIMPLE_LINK_HCI_DATA_HEADER_SIZE + 1)
// Using Send():
// max(CC3000_MINIMAL_TX_SIZE, MAX_DATA + SPI_HEADER_SIZE + HCI_CMND_SEND_ARG_LENGTH + SIMPLE_LINK_HCI_DATA_HEADER_SIZE + 1)
// The 1 is used for the overrun detection

#define	CC3000_MINIMAL_TX_SIZE      (130 + 1)  
#define	CC3000_MAXIMAL_TX_SIZE      (1519 + 1)

//#define CC3000_TX_BUFFER_SIZE 250 Not required for this implementation
#define CC3000_RX_BUFFER_SIZE 250

#define CC3000_IRQ_ACTIVE 0
#define CC3000_IRQ_IDLE   1

typedef struct _ip_addr_t {
	union {
		uint8_t ip[4];
		unsigned long ipl;	
	};
} ip_addr_t;


//extern uint8_t wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];
extern uint8_t wlan_rx_buffer[CC3000_RX_BUFFER_SIZE];
extern uint16_t spi_length;
extern uint8_t free_buffers;
extern uint16_t buffer_length;
extern uint8_t cc3000_smart_config_complete;
extern uint8_t cc3000_connected;
extern uint8_t cc3000_dhcp_complete;
extern uint8_t cc3000_interrupts_enabled;

/** 
 
    \brief Set up the cc3000 pins ready for I/O
 
    Configure pins for cc3000 I/O
 
*/
void cc3000_setup_io(void);

/** 
 
    \brief Initialise any hardware required to communicate with the cc3000
    
    Get any hardware ready for talking to the cc3000. In our case, we simply call the SPI hardware initialisation.
 
 
*/
void cc3000_init(void);

/** 
 
    \brief Enable interrupts from the IRQ line
 
	Enable interrupts from the IRQ line.

*/

void cc3000_interrupt_enable(void);

/** 
 
    \brief Disable interrupts from the IRQ line
 
*/
void cc3000_interrupt_disable(void);

/** 
 
    \brief Disable the module by pulling EN line low

*/

void cc3000_module_disable(void);

/** 
 
    \brief Enable the module by pulling EN line high
 


*/

void cc3000_module_enable(void);

/** 
 
    \brief Find out the value of the IRQ pin
 
	Query the value of the IRQ pin

*/

uint8_t cc3000_read_irq_pin(void);

/** 
 
    \brief Enable CS pin
 
	Enable CS pin (set low)

*/

void cc3000_cs_enable(void);

/** 
 
    \brief Disable CS pin
 
	Disable CS pin (set high)

*/

void cc3000_cs_disable(void);


/** 
 
    \brief Interrupt service routine
 
	To be called when IRQ line goes high (and not waiting for it to happen), 
	this routine should call HCI/SPI receive
	
*/

void cc3000_handle_isr(void);

// convert between stream / internal data formats
uint16_t stream_to_uint16_t(uint8_t *i);
uint32_t stream_to_uint32_t(uint8_t *i);
uint8_t * uint32_t_to_stream( uint8_t * i, uint32_t n);
uint8_t * uint16_t_to_stream( uint8_t * i, uint16_t n);


#endif
