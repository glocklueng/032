/*
 * spi_hw.c
 *
 * Created: 11/16/2013 8:31:28 AM
 *  Author: charlie
 */ 

#include "user_board.h"

#include "cc3000.h"
#include "spi_hw.h"


void spi_hw_setup_io(void)
{
	__PROLOG("spi_hw_setup_io\n");
	
}

void spi_hw_init(void)
{
	__PROLOG("spi_hw_init\n");

	SPIC.CTRL = 0x55; // 0x51;//b01010101;
	SPIC.INTCTRL = SPI_INTLVL_LO_gc;
				
	PORTC.DIRSET = PIN0_bm;
	PORTC.DIRSET = PIN1_bm;


	PORTC.DIRSET  = WLAN_MOSI_bm ;
	PORTC.DIRSET  = WLAN_SCK_bm;
	PORTC.DIRSET = WLAN_SS_bm;

	PORTC.DIRCLR = WLAN_MISO_bm;
	PORTC.DIRCLR = PIN2_bm;
			
	circular_buffer_put('\n');
	if( cc3000_read_irq_pin() == 0 	) {
		circular_buffer_put('0');
	} else {
		circular_buffer_put('1');
		
	}
	circular_buffer_put('\n');
}

void spi_hw_master_transmit(uint8_t data)
{
	SPIC.DATA = data;
	//while(!(SPIC.STATUS & SPI_IF_bm)) {}
}

uint8_t spi_hw_master_receive(void)
{
	
	//while(!(SPIC.STATUS & SPI_IF_bm)) {}
	return SPIC.DATA;	
}