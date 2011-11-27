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



/*
   PPPPPP
   BBBBBD
   432106
 1 10xxxx
 2 01xxxx
 3 1x0xxx
 4 0x1xxx
 5 1xx0xx
 6 0xx1xx
 7 1xxx0x
 8 0xxx1x
 9 1xxxx0
10 0xxxx1

   PPPPPP
   BBBBBD
   432106
11 x10xxx
12 x01xxx
13 x1x0xx
14 x0x1xx
15 x1xx0x
16 x0xx1x
17 x1xxx0
18 x0xxx1

   PPPPPP
   BBBBBD
   432106
19 xx10xx
20 xx01xx
21 xx1x0x
22 xx0x1x
23 xx1xx0
24 xx0xx1

   PPPPPP
   BBBBBD
   432106
25 xxx10x
26 xxx01x
27 xxx1x0
28 xxx0x1

   PPPPPP
   BBBBBD
   432106
29 xxxx10
30 xxxx01

*/

//  state definitions for each individual LED
// A5  A2  A1  A0  A4  A3
// PD6 PB0 PB1 PB2 PB3 PB4  

// 00 = LOW
// 01 = HIGH
// 11 = X (input)

// If leds are reversed, 0 this




#if 0
#define CONST
#define progmem 
#else
#define CONST const
#define progmem PROGMEM
#endif

// combine these two
static  CONST unsigned char progmem _portd6[30] = {
	
	2,2,2,2,2,2,
	2,2,0,1,2,2,
	2,2,2,2,0,1,
	2,2,2,2,0,1,
	2,2,0,1,0,1
};

static  CONST unsigned char progmem _ddrd[30] = {
	0,0,0,0,0,0,
	0,0,1,1,0,0,
	0,0,0,0,1,1,
	0,0,0,0,1,1,
	0,0,1,1,1,1
};

static CONST unsigned char progmem _portb[30] = {
	0x10,0x08,0x10,0x04,0x10,0x02,
	0x10,0x01,0x10,0x00,0x08,0x04,
	0x08,0x02,0x08,0x01,0x08,0x00,
	0x04,0x02,0x04,0x01,0x04,0x00,
	0x02,0x01,0x02,0x00,0x01,0x00
};

static CONST  unsigned char progmem  _ddrb[15] = {
// 0..15
	24,20,18,
	17,16,12,
	10,9,8,
	6,5,4,
	3,2,1	
};

// this routine is written to help understand how the process works, not be fast.
static void SetRC( unsigned char index ) 
{	
	PORTD &=3;

	PORTB  = pgm_read_byte(&_portb[index]);
	DDRB   = pgm_read_byte(&_ddrb[index>>1]);
	if( pgm_read_byte(&_ddrd[index]) == 0 ) {
			CLR_BIT(DDRD,6 );
	} else {
			SET_BIT(DDRD,6 );
	}

	if(pgm_read_byte(&_portd6[index]) == 1 ) { 
	
		SET_BIT(PORTD ,6 );

	} else	if(pgm_read_byte(&_portd6[index]) == 0 ) {
		CLR_BIT(PORTD,6 ); 
	}
}

int main(void)
{
	unsigned short i;
	int16_t ax;

//  speaker off (eats power otherwise)
	DDRD = 0xff;
	PIND = 0x0;
	PORTD = 0x10; 
	DDRB = 0;

	mma_init();
	_delay_ms(25);

	uint8_t i2cad = mma_read( MMA_I2CAD );
	mma_write( MMA_I2CAD, i2cad |= MMA_I2CDIS_bit ); // disable I2C

    uint8_t ctl1 = mma_read( MMA_CTL1 );
	// 125 h	z
    mma_write( MMA_CTL1, ctl1 |= MMA_DFBW_bit ); // high output data rate

    mma_write( MMA_MCTL, 0b00000101 ); 				// 2g range; measurement mode

    mma_calibrate_offset( 0, 0, 0 );
		
	while( 1 ) 
	{

		mma_wait_until_ready();

		// read accelerometer
	    ax = mma_read10( MMA_XOUT10 );

		// calc a range
		ax += 50;
		ax /= 4;
		ax = ABS( ax  );
		
		// clamp
		if ( ax > 29)
			ax = 29;

		while( ax-- )
		{
			for ( i = 0 ; i < 20 ; i++ ) 
			{
				SetRC(ax);
			}
		}
	}
}


