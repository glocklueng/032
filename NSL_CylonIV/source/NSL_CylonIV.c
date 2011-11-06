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

// 		PORTB,DDRB,PINB

#define ddrb(r,c)  charlieplex[ (r*(3*4)) + ((c*3)+1) ]
#define pinb(r,c)  charlieplex[ (r*(3*4)) + ((c*3)+2) ]
#define portb(r,c) charlieplex[ (r*(3*4)) + ((c*3)+0) ]

#define HEX__(n) 0x##n##LU

/* 8-bit conversion function */
#define B8__(x) ((x&0x0000000FLU)?1:0)	\
	+((x&0x000000F0LU)?2:0)		\
	+((x&0x00000F00LU)?4:0)		\
	+((x&0x0000F000LU)?8:0)		\
	+((x&0x000F0000LU)?16:0)	\
	+((x&0x00F00000LU)?32:0)	\
	+((x&0x0F000000LU)?64:0)	\
	+((x&0xF0000000LU)?128:0)

/* *** user macros ***/

/* for upto 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* for upto 16-bit binary constants, MSB first */
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<	+ B8(dlsb)))

/* for upto 32-bit binary constants, MSB first */
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24)	 \
	+ ((unsigned long)B8(db2)<<16) \
	+ ((unsigned long)B8(db3)<<	 \
	+ B8(dlsb))




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
#if 1

static const unsigned char PROGMEM led30[] = {
  //               76543210
	B8( 1111),  B8(11110001 ),		// LED1
	B8( 1111) , B8(11110100 ),		// LED2
	B8( 1111) , B8(11001101 ),		// LED3

  //               76543210
	B8( 1111) , B8(11011100 ),		// LED4

	B8( 1111) , B8(00111101 ),		// LED5
	B8( 1111) , B8(01111100 ),		// LED6
	B8( 1100),  B8(11111101 ),		// LED7
	B8( 1101) , B8(11111100 ),		// LED8
	B8( 0011) , B8(11111101 ),		// LED9

	B8( 0111) , B8(11111100 ),		// LED10

	B8( 1111) , B8(11000111 ),		// LED11

	B8( 1111) , B8(11010011 ),		// LED12
	B8( 1111) , B8(00110111 ),		// LED13
	B8( 1111) , B8(01110011 ),		// LED14
	B8( 1100) , B8(11110111 ),		// LED15
	B8( 1101) , B8(11110011 ),		// LED16
	B8( 0011) , B8(11110111 ),		// LED17
	B8( 0111) , B8(11110011 ),		// LED18

	B8( 1111) , B8(00011111 ),		// LED19
	B8( 1111) , B8(01001111 ),		// LED20
	B8( 1100),  B8(11011111 ),		// LED21
	B8( 1101) , B8(11001111 ),		// LED22
	B8( 0011) , B8(11011111 ),		// LED23
	B8( 0111) , B8(11001111 ),		// LED24

	B8( 1100) , B8(01111111 ),		// LED25
	B8( 1101) , B8(00111111 ),		// LED26
	B8( 0011) , B8(01111111 ),		// LED27
	B8( 0111) , B8(00111111 ),		// LED28
	B8( 0001) , B8(11111111 ),		// LED29
	B8( 0100) , B8(11111111 ),		// LED30
};

#else

static const unsigned char PROGMEM led30[] = {

	B8( 1111) , B8(11110100 ),		// LED2
	B8( 1111),  B8(11110001 ),		// LED1
	B8( 1111) , B8(11011100 ),		// LED4
	B8( 1111) , B8(11001101 ),		// LED3
	B8( 1111) , B8(01111100 ),		// LED6
	B8( 1111) , B8(00111101 ),		// LED5
	
	B8( 1111) , B8(11010011 ),		// LED12
	B8( 1111) , B8(11000111 ),		// LED11
	B8( 1111) , B8(01110011 ),		// LED14
	B8( 1111) , B8(00110111 ),		// LED13
	B8( 1101) , B8(11110011 ),		// LED16
	B8( 1100) , B8(11110111 ),		// LED15
	
	B8( 1111) , B8(01001111 ),		// LED20
	B8( 1111) , B8(00011111 ),		// LED19
	B8( 1101) , B8(11001111 ),		// LED22
	B8( 1100),  B8(11011111 ),		// LED21
	B8( 0111) , B8(11001111 ),		// LED24
	B8( 0011) , B8(11011111 ),		// LED23

	B8( 1101) , B8(00111111 ),		// LED26
	B8( 1100) , B8(01111111 ),		// LED25
	B8( 0111) , B8(00111111 ),		// LED28
	B8( 0011) , B8(01111111 ),		// LED27
	B8( 1101) , B8(11111100 ),		// LED8
	B8( 1100),  B8(11111101 ),		// LED7

	B8( 0100) , B8(11111111 ),		// LED30
	B8( 0001) , B8(11111111 ),		// LED29
	B8( 0111) , B8(11110011 ),		// LED18
	B8( 0011) , B8(11110111 ),		// LED17
	B8( 0111) , B8(11111100 ),		// LED10
	B8( 0011) , B8(11111101 ),		// LED9
};
#endif


unsigned int X = 0;

// this routine is written to help understand how the process works, not be fast.
static void SetRC( unsigned char index ) 
{	
	unsigned char b1,b2;
	unsigned char portb, ddrb,portd6=2;
	
	DDRB = 0x0;
	PORTB =0x0;
	PORTD &=3;

	// get current values, Can't cache PORTD/DDRD since the sound interrupt can change, should cache DDRD6/PORTD6 instead
	portb = PORTB;
	ddrb = DDRB;
	
	index = index - 30 * (index / 30);

//	read two values from array, we use pgm_read_byte because the data is in program memory space
// its 6*2 because there are 6 leds in each column, and two bytes per led
	b1 = pgm_read_byte(&led30[ index*2   ] ) ;
	b2 = pgm_read_byte(&led30[(index*2)+1] ) ;

// if  bit positions 32 in b1 == 11 then set to don't care (input ) tristate for PD6
	if( IS_BIT_SET( b1,3 ) && IS_BIT_SET( b1,2 ) ) {

		CLR_BIT(DDRD,6 );

	} else { 

		// otherwise it is set to an output
		SET_BIT(DDRD ,6 );
	}

// if 10 == 11 then set to don't care (input ) tristate for PB0
	if( GET_BIT( b1,1 )==1 && GET_BIT( b1,0 )==1 ) {

		CLR_BIT(ddrb,0 );

	} else { 

		// otherwise its an output
		SET_BIT(ddrb ,0 );
	}

// if 76 == 11 then set to don't care (input ) tristate  for PB1
	if( GET_BIT( b2,7 )==1 && GET_BIT( b2,6 )==1 ) {

		CLR_BIT(ddrb,1 );

	} else { 

		// otherwise its an output
		SET_BIT(ddrb ,1 );
	}

// if 54 == 11 then set to don't care (input ) tristate for PB2
	if( GET_BIT( b2,5 )==1 && GET_BIT( b2,4 )==1 ) {

		CLR_BIT(ddrb,2 );

	} else { 

		// otherwise its an output
		SET_BIT(ddrb ,2 );
	}

// if 32 == 11 then set to don't care (input ) tristate for PB3
	if( GET_BIT( b2,3 )==1 && GET_BIT( b2,2 )==1 ) {

		CLR_BIT(ddrb,3 );

	} else { 

		// otherwise its an output
		SET_BIT(ddrb ,3 );
	}

// if 10 == 11 then set to don't care (input ) tristate for PB4
	if( GET_BIT( b2,1 )==1 && GET_BIT( b2,0 )==1 ) {

		CLR_BIT(ddrb,4 );

	} else { 

		// otherwise its an output
		SET_BIT(ddrb ,4 );
	}

/// all of the DDR's are set

// if 32 == 00 then set to low for PD6
	if( GET_BIT( b1,3 )==0 && GET_BIT( b1,2 )==0 ) {

//		CLR_BIT(PORTD,6 ); //set to off

		portd6 = 0;

	// if 32 == 01 then set to high for PD6
	} else if( GET_BIT( b1,3 )==0 && GET_BIT( b1,2 )== 1) { 

		// set to on
		//SET_BIT(PORTD ,6 );
		portd6 = 1;
	}

// if 10 == 00 then set to low for PB0
	if( GET_BIT( b1,1 )==0 && GET_BIT( b1,0 )==0 ) {

		CLR_BIT(portb,0); //set to off

	// if 32 == 01 then set to high for PB0
	} else if( GET_BIT( b1,1 )==0 && GET_BIT( b1,0 )==1 ) { 

		// set to on
		SET_BIT(portb ,0 );
	}

// if 76 == 00 then set to low for PB1
	if( GET_BIT( b2,7 )==0 && GET_BIT( b2,6 )==0 ) {

		CLR_BIT(portb,1); //set to off

	// if 32 == 01 then set to high for PB1
	} else if( GET_BIT( b2,7 )==0 && GET_BIT( b2,6 ) ==1 ) { 

		// set to on
		SET_BIT(portb ,1 );
	}


// if 54 == 00 then set to low for PB2
	if( GET_BIT( b2,5 )==0 && GET_BIT( b2,4 )==0 ) {

		CLR_BIT(portb,2); //set to off

	// if 32 == 01 then set to high for PB2
	} else if( GET_BIT( b2,5 )==0 && GET_BIT( b2,4 )==1 ) { 

		// set to on
		SET_BIT(portb ,2 );
	}

// if 32 == 00 then set to low for PB3
	if( GET_BIT( b2,3 )==0 && GET_BIT( b2,2 )==0 ) {

		CLR_BIT(portb,3); //set to off

	// if 32 == 01 then set to high for PB3
	} else if( GET_BIT( b2,3 ) ==0 && GET_BIT( b2,2 )==1 ) { 

		// set to on
		SET_BIT(portb ,3 );
	}

// if 10 == 00 then set to low for PB4
	if( GET_BIT( b2,1 )==0 && GET_BIT( b2,0 )==0 ) {

		CLR_BIT(portb,4); //set to off

// if 10 == 01 then set to high for PB4
	} else if( GET_BIT( b2,1)==0 && GET_BIT( b2,0 )==1 ) { 

		// set to on
		SET_BIT(portb ,4 );
	}

	//set all at once, stops it from flickering 
	PORTB = portb;
	DDRB = ddrb;

	if(portd6 == 1 ) { 
	
		SET_BIT(PORTD ,6 );

	} else	if(portd6 == 0 ) {

		CLR_BIT(PORTD,6 ); 
	}
}



// This function delays the specified number of 10 microseconds
static void delay_ten_us(unsigned long int us) 
{
	us /= 8;

  while (us != 0) {
	_delay_us(10);
    us--;
  }
}

int main(void)
{
	unsigned int x,c;
	unsigned char R,C;
	unsigned short i;
	int16_t ax,ay,az;

	uint8_t eeprom;
	uint16_t counter2;

//  speaker off (eats power otherwise)
	DDRD = 0xff;
	PIND = 0x0;
	PORTD = 0x10; 
	DDRB = 0;

	mma_init();
	_delay_ms(20);

	uint8_t i2cad = mma_read( MMA_I2CAD );
	mma_write( MMA_I2CAD, i2cad |= MMA_I2CDIS_bit ); // disable I2C

    uint8_t ctl1 = mma_read( MMA_CTL1 );
    mma_write( MMA_CTL1, ctl1 |= MMA_DFBW_bit ); // high output data rate

    mma_write( MMA_MCTL, 0b00000101 ); // 2g range; measurement mode

    mma_calibrate_offset( -15, -23, 65 );

// switch on interrupts ( start speaker )

//	sei();


// index to start
	i = 0;

	X = 0;
	
	while( 1 ) 
	{
		mma_get_average(2, &ax, &ay, &az);

		ax += 64;

		ax = abs( ax / 4 );

		for(X = 0 ; X < ax ; X ++ ) {
			for ( i = 0 ; i < 30 ; i++ ) {
				SetRC(X);
			}
		}
	
#if 0
		R = 0;	
		C = 29;			
		for ( X = 0 ; X < 200 ; X++ ) {
			for ( i = 0 ; i < 10 ; i++ ) {
				SetRC(R);	
				SetRC(R+1);	
				delay_ten_us(10);	
				SetRC(R+2);	
				delay_ten_us(25);	
				SetRC(R+3);	
				delay_ten_us(10);	
				SetRC(R+4);	

				SetRC(C+15);	
				SetRC(C+1+15);	
				delay_ten_us(10);	
				SetRC(C+2+15);	
				delay_ten_us(25);	
				SetRC(C+3+15);	
				delay_ten_us(10);	
				SetRC(C+4+15);	
			}

			R++;
			if ( R == 30 ) R = 0;
			C--;
			if( c == 0 ) c = 29;
		}
		R= 0 ;

		for ( X = 0 ; X < 200 ; X++ ) {
			for ( i = 0 ; i < 20 ; i++ ) {
				SetRC(R);	
				SetRC(R+1);	
				delay_ten_us(20);	
				SetRC(R+2);	
				delay_ten_us(100);	
				SetRC(R+3);	
				delay_ten_us(20);	
				SetRC(R+4);	
				delay_ten_us(10);	
				SetRC(R+5);	
			}

			R++;
			if ( R == 30 )  R = 0;

		}

		x = 30;

		c = 0;

		for ( X = 0 ; X < 200 ; X++ )  {

			x--;

			if ( x == 0 ) 
				x = 29;

			for ( i = 0 ; i < 20 ; i++ ) {
				for ( R= 0 ; R < x ; R ++ ) 
				{
					SetRC(c+R);
				}
				delay_ten_us(30-R);
				if( i&1 == 1 ) 
					c++;

			}
		}
		for ( X = 0 ; X < 400 ; X++ ) 
			for ( R= 0 ; R < 30 ; R ++ ) 
			{


				SetRC(R);	
				
			}

		for ( X = 0 ; X < 5 ; X++ ) 
			for ( R= 0 ; R < 30 ; R ++ ) 
			{

				SetRC(R);
				i = 150*(((R+1)*6));
				delay_ten_us(i);

			}

		for ( X = 0 ; X < 5 ; X++ ) 
			for ( R= 0 ; R < 30 ; R ++ ) 
			{
				SetRC(R);
				i = 150*(((R+1)*6));
				delay_ten_us(i);
			}
		
		for ( X = 0 ; X < 200 ; X++ ) 
			for ( R= 0 ; R < 30 ; R ++ ) 
			{


				SetRC(R);	
				delay_ten_us(X*R);
				
			}
#endif

/// end
	}

}


