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

void SPI_IRQ ( void );

ISR(PORTC_INT0_vect)
{
	static int flag =0;
	
	//circular_buffer_puts("\nC\n");
	
	SPI_IRQ();
	
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
	
	PORTC.DIR      = 0x00;  // output
	PORTC.PIN0CTRL = 0x00;  // pull ups?
	PORTC.INTCTRL  = 0x00;  // PORTC will generate no low level interrupts
	PORTC.INT0MASK = 0x00;
	PORTC.OUT      = 0x00;
	
	//outputs
	PORTC.DIRSET = WLAN_SS_bm;
	PORTC.DIRSET = WLAN_MOSI_bm;
	PORTC.DIRSET = WLAN_SCK_bm;
	PORTC.DIRSET = VIO_bm;
	PORTC.DIRSET = VBAT_SW_EN_bm;

	//Inputs
	PORTC.DIRCLR = WLAN_MISO_bm;
	PORTC.DIRCLR = WLAN_SPI_IRQ_bm;
	
	
	// Set Pullup on SPI_IRQ
	//PORT_OPC_WIREDANDPULL_gc is totem pole with pull up. Page 141  http://www.atmel.com/Images/Atmel-8331-8-and-16-bit-AVR-Microcontroller-XMEGA-AU_Manual.pdf
	//
	PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;

	//bit_clr ( PORTC.PIN0CTRL, VIO );
	// off
	PORTC.OUTCLR = VBAT_SW_EN_bm ;
	PORTC.OUTCLR = VIO_bm;
	PORTC.OUTCLR = WLAN_SS_bm;// pin4
	PORTC.PIN4CTRL = PORT_OPC_PULLUP_gc;
	
	// Set SS output to high. (No slave addressed).
	PORTC.OUTSET = WLAN_SS_bm;

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
void cc3k_int_poll(void);

ISR ( TCC0_OVF_vect )
{
	static unsigned int counter = 0;

	cc3k_int_poll();

	counter ++ ;


	if ( counter == 50 )	{

		timer();

		counter = 0;

		gMode++;

		gMode %= 13;
	}
	
	
}
