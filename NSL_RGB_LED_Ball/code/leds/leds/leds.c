/*
* leds.c
*
* Created: 5/23/2013 8:13:35 PM
*  Author: charliex
*/

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"

// current mode ball is in
static int gMode = 0;

// CC3000 SPI
static SPI_Master_t spiMasterC;

// Variables for sample function
static float offset = 0;

unsigned char mode_switch ( void )
{
	return 0;
}


ISR ( TCC0_OVF_vect )
{
	static unsigned int counter = 0;

	counter ++ ;

	if ( counter == 1000 )	{

		counter = 0;

		gMode++;

		gMode %= 13;
	}
}

// Configure XMEGA oscillator and clock source.

void xmega_set_cpu_clock_to_32MHz()
{
	CCP = CCP_IOREG_gc;              // disable register security for oscillator update
	OSC.CTRL = OSC_RC32MEN_bm;       // enable 32MHz oscillator

	while ( ! ( OSC.STATUS & OSC_RC32MRDY_bm ) );

	// wait for oscillator to be ready

	CCP = CCP_IOREG_gc;              // disable register security for clock update
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
}


void xmega_set_cpu_clock_to_8MHz ( void )
{
	uint8_t u8PrescalerConfig;
	uint8_t u8ClockControl;

	/*  Enable internal 32MHz ring oscillator. */
	OSC.CTRL |= OSC_RC32MEN_bm;

	/*  Wait until oscillator is ready. */
	while ( ( OSC.STATUS & OSC_RC32MRDY_bm ) == 0 );

	/*  Select Prescaler A divider as 4 and Prescaler B & C divider as (1,1) respectively.  */
	/*  Overall divide by 4 i.e. A*B*C  */
	u8PrescalerConfig = ( uint8_t ) ( CLK_PSADIV_4_gc | CLK_PSBCDIV_1_1_gc );

	/* Disable register security for clock update */
	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = u8PrescalerConfig;

	/*  Set the 32 MHz ring oscillator as the main clock source */
	u8ClockControl = ( CLK.CTRL & ~CLK_SCLKSEL_gm ) | CLK_SCLKSEL_RC32M_gc;

	/* Disable register security for clock update */
	CCP = CCP_IOREG_gc;
	CLK.CTRL = u8ClockControl;
}



ISR( SPIC_INT_vect) {
	SPI_MasterInterruptHandler( &spiMasterC );
}


int main ( void )
{

	//xmega_set_cpu_clock_to_8MHz();
	xmega_set_cpu_clock_to_32MHz();

	// Power reduction: Stop unused peripherals
	//PR.PRGEN = 0x18;        // Stop: AES, EBI
	//PR.PRPC  = 0x74;        // Stop: TWI, USART0, USART1, HIRES
	//PR.PRPD  = 0x7C;        // Stop: TWI, USART0, USART1, SPI, HIRES
	//PR.PRPE  = 0x60;        // Stop: TWI, USART1, (XMEGA bug - HIRES cannot be turned off)
	//PR.PRPF  = 0x78;        // Stop: TWI, USART0, USART1, SPI, (XMEGA bug - HIRES cannot be turned off)

	PORTD.DIR      = 0xFF;  // LEDs
	PORTCFG.MPCMASK = 0xFF; // Configure all pins on PORTD the same way
	PORTD.PIN0CTRL = 0x00;  // Pull up on pin PortD (Keypad and K2-K5 buttons)
	PORTD.INTCTRL  = 0x00;  // PORTD will generate no low level interrupts
	PORTD.INT0MASK = 0x00;
	PORT_OUT      = 0x00;

	PORTC.DIR      = 0xFF;  // output
	PORTC.PIN0CTRL = 0x00;  //
	PORTC.INTCTRL  = 0x00;  // PORTC will generate no low level interrupts
	PORTC.INT0MASK = 0x00;
	PORTC.OUT      = 0x00;

	bit_clr ( PORTC.PIN0CTRL, VIO );
	bit_set ( PORTC.OUT, VIO );

	PORTC.OUT |= ( 1 << VBAT_SW_EN );
	PORTC.OUT |= ( 1 << VIO );

	/* Enable HIGH level interrupts in the PMIC. */
	PMIC.CTRL		= PMIC_HILVLEN_bm;

	TCC0.CTRLA		= TC_CLKSEL_DIV8_gc;
	TCC0.PER		= 185;
	TCC0.CNT		= 0;
	TCC0.CTRLFSET	= 0x0; /* count upwards, but don't start to interrupt yet */
	TCC0.INTCTRLA	= TC_OVFINTLVL_HI_gc; /* enable interrupts */

	// Init the SPI for the CC3000

	SPI_MasterInit (
	    &spiMasterC,
	    &SPIC,
	    &PORTC,
	    false,
	    SPI_MODE_0_gc,
	    SPI_INTLVL_OFF_gc,
	    false,
	    SPI_PRESCALER_DIV4_gc
	);

	LED_Init();

	_delay_ms ( 500 );

	bit_set ( PORTC.OUT, VBAT_SW_EN );

	gMode = 0;

	volatile unsigned short tempOne = 0;

	while ( 1 ) {

		for ( offset = 0; offset < 360; offset += 0.5 ) {
			RGBscan ( 4095, 0, 0, offset );
			//_delay_us(1);
		}

		for ( offset = 0; offset < 360; offset += 0.5 ) {
			RGBscan ( 0, 4095, 0, offset );
			//_delay_us(1);
		}

		for ( offset = 0; offset < 360; offset += 0.5 ) {
			RGBscan ( 0, 0, 4095, offset );
			//_delay_us(1);
		}

		for ( offset = 0; offset < 360; offset += 0.5 ) {
			RGBscan ( 4095, 4095, 4095, offset );
			//_delay_us(1);
		}

		SetRGB ( 4095, 0, 0 );

		_delay_ms ( 5000 );
		_delay_ms ( 5000 );

		SetRGB ( 0, 4095, 0 );

		_delay_ms ( 5000 );
		_delay_ms ( 5000 );

		SetRGB ( 0, 0, 4095 );

		_delay_ms ( 5000 );
		_delay_ms ( 5000 );

		SetRGB ( 4095, 4095, 4095 );

		_delay_ms ( 5000 );
		_delay_ms ( 5000 );
	}



	/// bridged pins!
	while ( 0 ) {
		{


			for ( int y = 0 ; y < ( 24 * 3 ) ; y++ ) {


				tempOne ++;

				// Number  of BITS in each shift reg
				for ( int j = 0; j < 12; j++ ) {

					if ( ( tempOne >> ( 11 - j ) ) & 1 ) {

						PORT_OUT |= ( 1 << DATPIN );
					}

					else {
						PORT_OUT &= ~ ( 1 << DATPIN );
					}
				}

				asm ( "nop" );
				PORT_OUT |= ( 1 << CLKPIN );
				asm ( "nop" );
				PORT_OUT &= ~ ( 1 << CLKPIN );

			}

			asm ( "nop" );
			PORT_OUT |= ( 1 << LATPIN | 1 << BLANKPIN );
			asm ( "nop" );
			PORT_OUT &= ~ ( 1 << LATPIN | 1 << BLANKPIN );
			asm ( "nop" );

		}
	}

	/* enable global interrupts */
	sei();

	while ( 1 ) {				// Loop Here

		if ( mode_switch( ) ) {
			brightnessShift++;
			brightnessShift %= 6;
			_delay_ms ( 1000 );
		}


		switch ( gMode ) {

			case 0:

				break;

			default:

				gMode = 0;
				break;
		}
	}

}