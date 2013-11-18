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

#include "cc3000\cc3000.h"

#include "cc3000\cc3000_hci.h"
#include "cc3000\cc3000_general.h"
#include "cc3000\cc3000_spi.h"
#include "cc3000_wlan.h"


uint8_t wlan_rx_buffer[CC3000_RX_BUFFER_SIZE];
uint16_t spi_length;

uint8_t free_buffers;
uint16_t buffer_length;

uint8_t cc3000_smart_config_complete = 0;
uint8_t cc3000_connected = 0;
uint8_t cc3000_dhcp_complete = 0;
uint8_t cc3000_interrupts_enabled = 0;

void cc3000_setup_io ( void )
{
/*
	set_pin ( cc3000_cs_port, cc3000_cs_pin );
	make_output ( cc3000_cs_port, cc3000_cs_pin );

	make_input ( cc3000_int_port, cc3000_int_pin );

	clear_pin ( cc3000_en_port, cc3000_en_pin );
	make_output ( cc3000_en_port, cc3000_en_pin );

	PORTC.DIRSET = VIO_bm;
	PORTC.DIRSET = VBAT_SW_EN_bm;

	PORTC.OUTSET = VBAT_SW_EN_bm;
	_delay_ms(10 );
	PORTC.OUTSET = VIO_bm;
*/	

//	cc3000_spi_setup_io();
}

void cc3000_init ( void )
{
	__PROLOG("cc3000_init\n");
		
	PORTC.OUTSET = VIO_bm;
		
	spi_hw_init();
	
	PORTC.OUTSET = VBAT_SW_EN_bm;
}

void wlan_Setup(void )
{
	while(circular_empty()==0);

	//circular_buffer_puts("cc3000_setup_io\n");
	cc3000_setup_io();

	//circular_buffer_puts("cc3000_init\n");
	cc3000_init();

	//circular_buffer_puts("cc3000_wlan_set_connection_policy\n");
	cc3000_wlan_set_connection_policy ( fast_connect_disabled, open_ap_auto_connect_disabled, use_profiles_disabled );


	//circular_buffer_puts("cc3000_wlan_del_profile\n");
	cc3000_wlan_del_profile ( 255 ); // delete them all!

	//circular_buffer_puts("cc3000_wlan_del_profile\n");
	cc3000_wlan_disconnect ();  // disconnect

	//circular_buffer_puts("cc3000_wlan_smart_config_set_prefix\n");
	cc3000_wlan_smart_config_set_prefix ();

	//circular_buffer_puts("cc3000_wlan_smart_config_start\n");
	cc3000_wlan_smart_config_start ( 0 ); // start! no encryption
	
	//circular_buffer_puts("cc3000_hci_wait_for\n");
	cc3000_hci_wait_for ( HCI_EVENT_WLAN_ASYNC_SMART_CONFIG_DONE );
	
	//circular_buffer_puts("cc3000_wlan_set_connection_policy\n");
	cc3000_wlan_set_connection_policy ( fast_connect_disabled, open_ap_auto_connect_disabled, use_profiles_enabled );
		
	circular_buffer_puts("cc3000_general_shutdown\n");
	while(circular_empty()==0);

	cc3000_general_shutdown ();

	
	_delay_ms ( 250 );
	_delay_ms ( 250 );
	_delay_ms ( 250 );
	_delay_ms ( 250 );
		
	cc3000_general_startup ( 0 );

	//process_smart_config = 1;   



	__PROLOG("wlan setup done\n");
}

void cc3000_interrupt_enable ( void )
{
	debug_str ( "Interrupt ENable\n" );
	//clear_bit ( intcon, INT0IF ); // in case it was triggered already
	//set_bit ( intcon, INT0IE ); // turn on rb0/int0 interrupts
		
	// PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
		
	PORTC.INT0MASK = WLAN_SPI_IRQ_bm;

	// External interrupt SPI_IRQ from CC3000 on PC2
	PORTC.PIN2CTRL =  PORT_ISC_FALLING_gc;
	PORTC.INTCTRL = PORT_INT0LVL_MED_gc;

	cc3000_interrupts_enabled = 1;

}

void cc3000_interrupt_disable ( void )
{
	debug_str ( "Interrupt DISable\n" );
	//clear_bit ( intcon, INT0IF ); // in case it was triggered already
	//clear_bit ( intcon, INT0IE ); // turn off rb0/int0 interrupts
	cc3000_interrupts_enabled = 0;
	
	// External interrupt SPI_IRQ from CC3000 on PC2
	PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_LEVEL_gc;
	// PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
		
	bit_clr( PORTC.INT0MASK , WLAN_SPI_IRQ_bp );

	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;
		
}

void WriteWlanPin( unsigned char val )
{
	if ( val ) {
		PORTC.OUTSET = VBAT_SW_EN_bm;
	} else {
		PORTC.OUTCLR = VBAT_SW_EN_bm;
	}
}


void cc3000_module_enable ( void )
{
	debug_str ( "Module ENable\n" );
	WriteWlanPin(1);
}

void cc3000_module_disable ( void )
{
	debug_str ( "Module DISable\n" );
	cc3000_interrupt_disable();
	WriteWlanPin(0);
}

void cc3000_handle_isr ( void )
{
	cc3000_hci_receive();
}


uint8_t cc3000_read_irq_pin ( void )
{
	//dump_register("PORTC.IN",PORTC.IN);
	
	return ((PORTC.IN & WLAN_SPI_IRQ_bm ) == 0 ) ? 1: 0;
}

void cc3000_cs_enable ( void )
{
//	__PROLOG ( "CS ENable\n" );
	
	clear_pin ( cc3000_cs_port, cc3000_cs_pin );
	PORTC.OUTCLR = WLAN_SS_bm;

}

void cc3000_cs_disable ( void )
{
//	__PROLOG ( "CS Disable\n" );

	set_pin ( cc3000_cs_port, cc3000_cs_pin );
	
	PORTC.OUTSET = WLAN_SS_bm;
}




uint8_t * uint32_t_to_stream ( uint8_t * i, uint32_t n )
{

	* ( i )++ = ( uint8_t ) ( n & 0xff );   // lsb
	* ( i )++ = ( uint8_t ) ( ( n >> 8 ) & 0xff );
	* ( i )++ = ( uint8_t ) ( ( n >> 16 ) & 0xff );
	* ( i )++ = ( uint8_t ) ( ( n >> 24 ) & 0xff ); // msb
	return i;
}

uint8_t * uint16_t_to_stream ( uint8_t * i, uint16_t n )
{

	* ( i )++ = ( uint8_t ) ( n & 0xff );       // lsb first
	* ( i )++ = ( uint8_t ) ( ( n >> 8 ) & 0xff ); // msb second

	return i;
}

uint16_t stream_to_uint16_t ( uint8_t * i )
{
	return  ( uint16_t ) ( *i );
}

// we ARE little endian!

uint32_t stream_to_uint32_t ( uint8_t * i )
{
	return  ( uint32_t ) *i;
}

void cc3000_callback_connect_status ( uint8_t connected, uint8_t dhcp )
{
	__PROLOG ( "Status change! " );

	if ( connected ) {
		__PROLOG ( " <Connected> " );
	}

	else {
		__PROLOG ( " <Disconnected> " );
	}

	if ( dhcp ) {
		__PROLOG ( " <IP address good> " );
	}

	else {
		__PROLOG ( " <No IP address yet> " );
	}

	__PROLOG ("\n");
}