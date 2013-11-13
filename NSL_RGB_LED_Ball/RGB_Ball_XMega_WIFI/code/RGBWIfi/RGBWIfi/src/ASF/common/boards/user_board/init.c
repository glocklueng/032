/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#include "user_board.h"
#include "spi_driver.h"


ISR(PORTC_INT0_vect)
{
	static int flag =0;
	
	circular_buffer_put('C');
	
	if ( flag == 0 ){
		flag = 1;
		PORT_OUT &= ~ ( 1 << BLANKPIN );
	} else {
		flag = 0 ;
		PORT_OUT |= ( 1 << BLANKPIN )  ;
	}
	
	PORTC.INTFLAGS |= PORT_INT0IF_bm; 
}


void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	PORTD.DIR      = 0xFF;  // LEDs
	PORTCFG.MPCMASK = 0xFF; // Configure all pins on PORTD the same way
	PORTD.PIN0CTRL = 0x00;  // Pull up on pin PortD (Keypad and K2-K5 buttons)
	PORTD.INTCTRL  = 0x00;  // PORTD will generate no low level interrupts
	PORTD.INT0MASK = 0x00;
	PORTD.OUT      = 0x00;
	
	PORTC.DIR      = 0xFF;  // output
	PORTC.PIN0CTRL = 0x00;  // pull ups?
	PORTC.INTCTRL  = 0x00;  // PORTC will generate no low level interrupts
	PORTC.INT0MASK = 0x00;
	PORTC.OUT      = 0x00;
	
	// Set Pullup on SPI_IRQ
	PORTC.PIN6CTRL = WLAN_SPI_IRQ_bm;

	//bit_clr ( PORTC.PIN0CTRL, VIO );
	// off
	PORTC.OUTCLR = VBAT_SW_EN_bm ;
	PORTC.OUTCLR = VIO_bm;

/*

	// Enable HIGH level interrupts in the PMIC. 
	PMIC.CTRL		= PMIC_HILVLEN_bm;

	TCC0.CTRLA		= TC_CLKSEL_DIV8_gc;
	TCC0.PER		= 185;
	TCC0.CNT		= 0;
	TCC0.CTRLFSET	= 0x0; // count upwards, but don't start to interrupt yet 
	TCC0.INTCTRLA	= TC_OVFINTLVL_HI_gc; // enable interrupts 
*/

	TCC0.PER = 125;
	TCC0.CTRLA = ( TCC0.CTRLA & ~TC0_CLKSEL_gm ) | TC_CLKSEL_DIV256_gc;
	TCC0.INTCTRLA = ( TCC0.INTCTRLA & ~TC0_OVFINTLVL_gm ) | TC_OVFINTLVL_LO_gc;

	LED_Init();
	
	_delay_ms ( 500 );

}

unsigned char gMode = 0;
void timer(void );

ISR ( TCC0_OVF_vect )
{
	static unsigned int counter = 0;


	timer();

	counter ++ ;


	if ( counter == 1000 )	{

		counter = 0;

		gMode++;

		gMode %= 13;
	}
	
	
}
