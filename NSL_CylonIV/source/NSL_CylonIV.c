/*
 #     #                        #
 #     #                        #
 #     #                        #
 #######   ####    #####    #####   ####   #####    #####
 #     #  #    #  #    #   #    #  #    #  #    #  #
 #     #  ######  #    #   #    #  ######  #        ####
 #     #  #       #    #   #    #  #       #            #
 #     #   #####   ######   #####   #####  #       #####
*/

#include <avr/io.h> 
#include <avr/eeprom.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "mma7455l.h"

#define ABS(a) (((a) < 0) ? -(a) : (a))

#define shortdelay(); 			asm("nop\n\tnop\n\t");

#define TIMER1_PRESCALE_1 		(1)
#define TIMER1_PRESCALE_8 		(2)
#define TIMER1_PRESCALE_64 		(3)
#define TIMER1_PRESCALE_256		(4)
#define TIMER1_PRESCALE_1024	(5)

#define MIN_OF(x,y) ((x) < (y) ? (x) : (y))
#define MAX_OF(x,y) ((x) > (y) ? (x) : (y))

#define GET_BIT(a,n)    ((a >> n) & 1)
#define SET_BIT(a,n)    (a|=(1<<n))
#define CLR_BIT(a,n)    (a&=~(1<<n))
#define SWITCH_BIT(a,n) (a^=(1<<n))
#define IS_BIT_SET(a,n) (a & (1<<n))
#define IS_BIT_CLR(a,n) (~(a & (1<<n)))

//  state definitions for each individual LED
// A5  A2  A1  A0  A4  A3
// PD6 PB0 PB1 PB2 PB3 PB4  

// 00 = LOW
// 01 = HIGH
// 11 = X (input)


#define D6_ON ( 1 << 0 )
#define DD_ON ( 1 << 1 )
#define BO_ON ( D6_ON | DD_ON )

static const unsigned char PROGMEM _state[30] = {

	0,0,0,    0,    0,    0,
	0,0,DD_ON,BO_ON,0,    0,
	0,0,0,    0,    DD_ON,BO_ON,
	0,0,0,    0,    DD_ON,BO_ON,
	0,0,DD_ON,BO_ON,DD_ON,BO_ON
};

static const unsigned char PROGMEM _portb[30] = {
	0x10,0x08,0x10,0x04,0x10,0x02,
	0x10,0x01,0x10,0x00,0x08,0x04,
	0x08,0x02,0x08,0x01,0x08,0x00,
	0x04,0x02,0x04,0x01,0x04,0x00,
	0x02,0x01,0x02,0x00,0x01,0x00
};

static const unsigned char PROGMEM _ddrb[15] = {

	24,20,18,
	17,16,12,
	10, 9, 8,
	 6, 5, 4,
	 3, 2, 1	
};

static void SetLED( unsigned char index ) 
{	
	PORTB  = pgm_read_byte(&_portb[index]);
	DDRB   = pgm_read_byte(&_ddrb[index>>1]);

	switch( pgm_read_byte(&_state[index] ))
	{
		case 0:
			CLR_BIT(DDRD,6 );
			CLR_BIT(PORTD,6 ); 
			break;

		case DD_ON:			
			SET_BIT(DDRD,6 );
			CLR_BIT(PORTD,6 ); 
			break;

		case D6_ON:			
			CLR_BIT(DDRD,6 );
			SET_BIT(PORTD,6 ); 
			break;

		case BO_ON:
			SET_BIT(DDRD,6 );
			SET_BIT(PORTD,6 ); 
			break;
	}
}

int main(void)
{
	int8_t ax;

// setup startup mode
	DDRD = 0xff;
	PIND = 0x0;
	PORTD = 0x10; 
	DDRB = 0;

	mma_init();
	_delay_ms(25);

	uint8_t i2cad = mma_read( MMA_I2CAD );
	mma_write( MMA_I2CAD, i2cad |= MMA_I2CDIS_bit ); // disable I2C

    uint8_t ctl1 = mma_read( MMA_CTL1 );

    mma_write( MMA_CTL1, ctl1 |= MMA_DFBW_bit ); 	// high output data rate

    mma_write( MMA_MCTL, 0b00000101 ); 				// 2g range; measurement mode

    mma_calibrate_offset( 0, 0, 0 );
		
	while( 1 ) 
	{
		// varies on whether we need this or not by how long we hold the led
		mma_wait_until_ready();

		// read accelerometer
	    ax = mma_read( MMA_XOUT8 );

		// calc a  range so all the leds light
		ax += 60;
		ax /= 4;
		ax = ABS( ax  );
		
		// clamp to number of leds
		if ( ax > 29 ) {
			ax = 29;
		}

		do {
	
			SetLED(ax);	
			_delay_ms( 2 );

		} while( ax-- );
	}
}


