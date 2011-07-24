
#if 1

#include <avr/io.h> 
#include <avr/eeprom.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

#include <stdlib.h>

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



//  state definitions for each individual LED
// A5  A2  A1  A0  A4  A3
// PD6 PB0 PB1 PB2 PB3 PB4  

// 00 = LOW
// 01 = HIGH
// 11 = X (input)

static const unsigned char PROGMEM led30[] = {

	B8( 1111),  B8(11110001 ),		// LED1
	B8( 1111) , B8(11110100 ),		// LED2
	B8( 1111) , B8(11001101 ),		// LED3
	B8( 1111) , B8(11011100 ),		// LED4
	B8( 1111) , B8(00111101 ),		// LED5
	B8( 1111) , B8(01111100 ),		// LED6

	B8( 1111) , B8(11000111 ),		// LED11
	B8( 1111) , B8(11010011 ),		// LED12
	B8( 1111) , B8(00110111 ),		// LED13
	B8( 1111) , B8(01110011 ),		// LED14
	B8( 1100) , B8(11110111 ),		// LED15
	B8( 1101) , B8(11110011 ),		// LED16

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
	B8( 1100),  B8(11111101 ),		// LED7
	B8( 1101) , B8(11111100 ),		// LED8


	B8( 0001) , B8(11111111 ),		// LED29
	B8( 0100) , B8(11111111 ),		// LED30
	B8( 0011) , B8(11110111 ),		// LED17
	B8( 0111) , B8(11110011 ),		// LED18
	B8( 0011) , B8(11111101 ),		// LED9
	B8( 0111) , B8(11111100 ),		// LED10
};


unsigned int X = 0;
signed char XDIR =1;

// this routine is written to help understand how the process works, not be fast.
void SetRC( unsigned char R,unsigned char C ) 
{	
	unsigned char b1,b2;
	unsigned char portb, ddrb,portd6=2;

	// get current values, Can't cache PORTD/DDRD since the sound interrupt can change, should cache DDRD6/PORTD6 instead
	portb = PORTB;
	ddrb = DDRB;

//	read two values from array, we use pgm_read_byte because the data is in program memory space
// its 6*2 because there are 6 leds in each column, and two bytes per led
	b1 = pgm_read_byte(&led30[ (R*(6*2))+(C*2)   ] ) ;
	b2 = pgm_read_byte(&led30[((R*(6*2))+(C*2))+1] ) ;

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

// if 32 == 01 then set to high for PB4
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

static const unsigned char pattern[] PROGMEM =
{ 
//	5,0,0,0,0,0,

//6x5

	0,0,0,0,0,0,

	1,1,1,1,1,1,
	1,0,0,0,0,1,
	1,0,0,0,0,1,

	0,0,0,0,0,0,

	1,1,1,1,1,1,
    0,0,0,0,1,0,
    0,0,0,1,0,0,
    0,0,1,0,0,0,
	1,1,1,1,1,1,
  
	0,0,0,0,0,0,

	1,0,0,1,1,1,
	1,0,1,1,0,1,
	1,0,1,1,0,1,
	1,0,1,1,0,1,
	1,1,1,0,0,1,

	0,0,0,0,0,0,

	1,1,1,1,1,1,
	1,0,0,0,0,0,
	1,0,0,0,0,0,
	1,0,0,0,0,0,

	0,0,0,0,0,0,

	1,0,0,0,0,1,
	1,0,0,0,0,1,
	1,1,1,1,1,1,

	0,0,0,0,0,0,
	0,0,0,0,0,0,
	0,0,0,0,0,0,
	0,0,0,0,0,0,


	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
};


// Speaker drive interrupt
SIGNAL( SIG_TIMER1_COMPA ) 
{
	// toggle speaker on/off
	//SWITCH_BIT( PORTD , 4 );

	X += XDIR;

	if (X > 10000 ) { 
		XDIR = -((rand()%9)+1);
	} else if ( X < 20) {
		XDIR = ((rand()%9)+1);
	}

	OCR1A = X+(rand()%100);
}


int main(void)
{
	unsigned int x,speed=4;;
	unsigned int R,C;
	unsigned long i;

	// setup timer
	TCCR1B = (1 << WGM12) | TIMER1_PRESCALE_1;
	OCR1A = (uint16_t)1000;
	TIMSK |= 1 << OCIE1A;   // Output Compare Interrupt Enable (timer 1, OCR1A) 

//  speaker off (eats power otherwise)
	DDRD = 0xff;
	PIND = 0x0;
	PORTD = 0x10; 
	DDRB = 0;

// switch on interrupts ( start speaker )

	//sei();

// index to start
	i = 0;

	X = 0;
	
	while(1) {
		for ( R= 0 ; R < 5 ; R ++ ) {
			for ( C= 0 ; C < 6 ; C ++ ) {
				SetRC(R,C);
				_delay_ms(500);				_delay_ms(500);
				_delay_ms(500);
			}
		}
	}

	for(;;) {	

		for ( x = 0 ; x < speed*100; x ++) {

		//if( x == 9 ) break;
;
			for ( R= 0 ; R < 5 ; R ++ ) {

				for ( C= 0 ; C < 6 ; C ++ ) {

					unsigned char code;

					if ( i < sizeof(pattern) ) {
						
						// fetch value
						code = pgm_read_byte ( &pattern[i]  );

					} else {

						break;
					}
	
					// if first column
					if (C == 0 && code > 1 ) {

						// skip rest of row ( i++ later makes it 6)
						i += 6;

						// set speed
						speed = code - 2;
					}
					else
						if( code  ) {
							SetRC(R,C);
						} 


					i++;

				}
			}

			// go back to beginning of pattern, repeat for speed%10
			i -= (5*6);
	
		}	

		if( speed < 20 ) {

			// skip a row
			i += 6;

		} else {

			speed -- ;

			if(speed == 21) {
				i+=(5*6);
			}
		}
		if( i >=  sizeof(pattern) ) 
			i = 0;
	}
}


#else
/*
TV-B-Gone
Firmware
for use with ATtiny2313
Chaos Communications Camp 2007
Mitch Altman
31-Jul-07

Distributed under Creative Commons 2.5 -- Attib & Share Alike
*/


/*
Ladyada has a great website about the MiniPOV kit, from which this project was hacked:
http://ladyada.net/make/minipov3/index.html
Her website also has a user forum, where people can ask and answer questions by people building
various projects, including this one:
http://www.ladyada.net/forums/
(Click on the link for MiniPOV).
*/


#include <avr/io.h>             // this contains all the IO port definitions
#include <avr/interrupt.h>      // definitions for interrupts
#include <avr/sleep.h>          // definitions for power-down modes
#include <avr/pgmspace.h>       // definitions or keeping constants in program memory
#include <util/delay.h>



/*
This project transmits a bunch of TV POWER codes, one right after the other, 
with a pause in between each.  (To have a visible indication that it is 
transmitting, it also pulses a Red  LED once each time a POWER code is 
transmitted.)  That is all TV-B-Gone does.  The tricky part of TV-B-Gone 
was collecting all of the POWER codes, and getting rid of the duplicates and 
near-duplicates (because if there is a duplicate, then one POEWR code will 
turn a TV off, and the duplicate will turn it on again (which we certainly 
do not want).  I have compiled the top-40 most popular codes with the 
duplicates eliminated, both for North America (which is the same as Asia, as 
far as POWER codes are concerned -- even though much of Asia USES PAL video) 
and for Europe (which works for Australia, New Zealand, the Middle East, and 
other parts of the world that use PAL video).

Using the MiniPOV for this project presents a limitation, based on the size of
the Atmel ATtiny2313 internal flash memory, which is 2KB.  With 2KB we can only 
fit about 7 POWER codes into the firmware's database of POWER codes.  If you 
would like to make a TV-B-Gone with all 40 codes, then they should all fit into 
another Atmel chip with 8KB of flash, such as ATtiny85.

This version of the firmware has the most popular 7 POWER codes for North America
(which is how many codes fit into the ATtiny2313 2KB flash memory).
*/


/*
This project is a good example of how to use the AVR chip timers.
*/


/*
The hardware for this project is very simple:
     ATtiny2313 has 20 pins:
       pin 1   connects to serial port programming circuitry
       pin 4   one pin of ceramic resonator
       pin 5   one pin of ceramic resonator
       pin 10  ground
       pin 12  PB0 - visible LED
       pin 14  OC0A - IR emitter, through a 2N3904 driver (with 120 ohm base resistor)
       pin 17  connects to serial port programming circuitry
       pin 18  connects to serial port programming circuitry
       pin 19  connects to serial port programming circuitry
       pin 20  +3v
    All other pins are unused

    This firmware requires that the clock frequency of the ATtiny 
      is the default that it is shipped with:  8.0MHz

    IMPORTANT:  to use the ceramic resonator, you must perform the following:
                    make burn-fuse_cr
*/


/*
The C compiler creates code that will transfer all constants into RAM when the microcontroller
resets.  Since this firmware has a table (powerTab) that is too large to transfer into RAM,
the C compiler needs to be told to keep it in program memory space.  This is accomplished by
the macro PROGMEM (this is used, below, in the definition for the powerTab).  Since the
C compiler assumes that constants are in RAM, rather than in program memory, when accessing
the powerTab, we need to use the pgm_read_word() macro, and we need
to use the powerTab as an address, i.e., precede it with "&".  For example, to access 
powerTab[3].onTime, which is a word, this is how to do it:
     pgm_read_word( &powerTab[3].onTime );
And to access powerTab[3].offTime, which is also a word, this is how to do it:
     pgm_read_word( &powerTab[3].offTime );
*/



/*
The following POWER code table consists of any number of POWER codes that will fit into the 2k flash ROM.
Each POWER code consists of any number of codeElements, each of which consists of an onTime
and an offTime (both onTime and offTime are expressed as the number of 10 microseconds -- for 
example, 4110 microseconds would be entered as 411).  The onTime is a length of time that the 
IR emitter is being pulsed with the carrier frequency (usually about 40KHz).  The offTime is 
a length of time that the IR emitter is not emitting any IR.  The first codeElement of each 
POWER code is actually a fake one:  the onTime contains the value to store in the compare 
register for Timer 0 (called OCR0A), and the offTime is ignored -- see more info on this in 
the next paragraph.  The last codeElement of each POWER code in the table is signified by 
making its offTime = 0.  To signify the last POWER code in the table, the last POWER code in 
the table must be followed by a codeElement with onTime = 0 (the offTime is ignored).

As stated above, the carrier frequency for each POWER code is determined by the value for 
onTime in the POWER code's first codeElement.  This value is derived from the following 
equation, given in the datasheet for the ATtiny2313:
     Output frequency = Fclk / (2 * Prescale * (1 + OCR1A) ) = 8,000,000 / (2 * (1 + OCR0A) )
So, given a desired carrier frequency, with the ATtiny2313 Fclk at 8.0Mhz, we determine OCR0A 
by the following equation:
     OCR0A = ( ( (8,000,000.0 / carrierFrequency) / 2.0 ) - 1.0)
Where the carrierFrequency is stated in Hz (with at least one decimal place at the end).
Since all of these values are constants, we will let the C compiler calculate this value in 
powerTab (using floating point by using a decimal point with all of the numbers).


There is a delay of 205 milliseconds after each POWER code is transmitted (whenever offTime = 0).
This delay is necessary so that a TV receiving the wrong POWER code will have time to reset itself 
before receiving the next POWER code (which might be the correct one for it).
*/


// table of POWER codes
struct codeElement {
  unsigned int onTime;   // duration of "On" time
  unsigned int offTime;  // duration of "Off" time
} const powerTab[] PROGMEM = {
  { ( ( (8000000.0 / 37520.0) / 2.0 ) - 1.0),   0 },   // Code 000 -- Sony, Baur and a few others
  { 245,    60 },
  { 123,    60 },
  {  61,    60 },
  { 123,    60 },
  {  61,    60 },
  { 123,    60 },
  {  61,    60 },
  {  61,    60 },
  { 123,    60 },
  {  61,    60 },
  {  61,    60 },
  {  61,    60 },
  {  61,  2758 },
  { 246,    60 },
  { 123,    60 },
  {  61,    60 },
  { 123,    60 },
  {  61,    60 },
  { 123,    60 },
  {  61,    60 },
  {  61,    60 },
  { 123,    60 },
  {  61,    60 },
  {  61,    60 },
  {  61,    60 },
  {  61,     0 },
  { ( ( (8000000.0 / 55600.0) / 2.0 ) - 1.0),   0 },   // Code 001 -- Proscan, RCA and a few others
  { 411,   410 },
  {  52,   205 },
  {  52,   205 },
  {  52,   205 },
  {  52,   205 },
  {  52,   102 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   102 },
  {  52,   102 },
  {  52,   102 },
  {  52,   102 },
  {  52,   205 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,   821 },
  { 411,   410 },
  {  52,   205 },
  {  52,   205 },
  {  52,   205 },
  {  52,   205 },
  {  52,   102 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   102 },
  {  52,   102 },
  {  52,   102 },
  {  52,   102 },
  {  52,   205 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,   102 },
  {  52,   205 },
  {  52,     0 },
  { ( ( (8000000.0 / 36130.0) / 2.0 ) - 1.0),   0 },   // Code 002 -- Panasonic
  { 358,   179 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,   135 },
  {  44,   135 },
  {  44,   135 },
  {  44,   135 },
  {  44,    45 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,   135 },
  {  44,   135 },
  {  44,   135 },
  {  44,   135 },
  {  44,    45 },
  {  44,   135 },
  {  44,  7720 },
  { 358,   180 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,   135 },
  {  44,   135 },
  {  44,   135 },
  {  44,   135 },
  {  44,    45 },
  {  44,    45 },
  {  44,   135 },
  {  44,    45 },
  {  44,   135 },
  {  44,   135 },
  {  44,   135 },
  {  44,   135 },
  {  44,    45 },
  {  44,   135 },
  {  44,     0 },
  { ( ( (8000000.0 / 37470.0) / 2.0 ) - 1.0),   0 },   // Code 003 -- Sharp
  {  28,   189 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,   189 },
  {  29,   189 },
  {  29,    81 },
  {  29,   189 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,   189 },
  {  29,    81 },
  {  29,  4670 },
  {  29,   189 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,   189 },
  {  29,    81 },
  {  29,    81 },
  {  29,   189 },
  {  29,    81 },
  {  29,   189 },
  {  29,   189 },
  {  29,   189 },
  {  29,    81 },
  {  29,   189 },
  {  29,  4670 },
  {  29,   189 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,   189 },
  {  29,   189 },
  {  29,    81 },
  {  29,   189 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,   189 },
  {  29,    81 },
  {  29,  4670 },
  {  29,   189 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,    81 },
  {  29,   189 },
  {  29,    81 },
  {  29,    81 },
  {  29,   189 },
  {  29,    81 },
  {  29,   189 },
  {  29,   189 },
  {  29,   189 },
  {  29,    81 },
  {  29,   189 },
  {  29,     0 }, 
  { ( ( (8000000.0 / 37470.0) / 2.0 ) - 1.0),   0 },   // Code 004 -- Toshiba, Apex
  { 924,   464 },
  {  58,    57 },
  {  58,    57 },
  {  58,    57 },
  {  58,    57 },
  {  58,    57 },
  {  58,    57 },
  {  58,   173 },
  {  58,    57 },
  {  58,   173 },
  {  58,   173 },
  {  58,   173 },
  {  58,   173 },
  {  58,   173 },
  {  58,   173 },
  {  58,    57 },
  {  58,   173 },
  {  58,    57 },
  {  58,   173 },
  {  58,    57 },
  {  58,    57 },
  {  58,   173 },
  {  58,    57 },
  {  58,    57 },
  {  58,    57 },
  {  58,   173 },
  {  58,    57 },
  {  58,   173 },
  {  58,   173 },
  {  58,    57 },
  {  58,   173 },
  {  58,   173 },
  {  58,   173 },
  {  58,  4054 },
  { 926,   230 },
  {  58,  9880 },
  { 926,   230 },
  {  58,     0 },
  { ( ( (8000000.0 / 34800.0) / 2.0 ) - 1.0),   0 },   // Code 005 -- Philips, Grundig, Pye
  {  92,    92 },
  { 184,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,   184 },
  {  92,    92 },
  { 184,    92 },
  {  92,  9216 },
  {  92,    92 },
  { 184,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,    92 },
  {  92,   184 },
  {  92,    92 },
  { 184,    92 },
  {  92,     0 },
  { ( ( (8000000.0 / 37470.0) / 2.0 ) - 1.0),   0 },   // Code 006 -- Samsung
  { 462,   476 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,   175 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,   175 },
  {  53,    62 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,  4661 },
  { 464,   476 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,   175 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,    62 },
  {  53,   175 },
  {  53,    62 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,   175 },
  {  53,     0 },
//  { ( ( (8000000.0 / 38200.0) / 2.0 ) - 1.0),   0 },   // Code 007 -- Zenith
//  {  52,    49 },
//  {  52,   419 },
//  {  52,   522 },
//  {  52,    49 },
//  {  52,   419 },
//  {  52,    49 },
//  {  52,   419 },
//  {  52,   522 },
//  {  52,    49 },
//  {  52,   419 },
//  {  52,   522 },
//  {  52,    49 },
//  {  52,   419 },
//  {  52,   522 },
//  {  52,   522 },
//  {  52,    49 },
//  {  52, 12433 },
//  {  52,    49 },
//  {  52,   419 },
//  {  52,   522 },
//  {  52,    49 },
//  {  52,   419 },
//  {  52,    49 },
//  {  52,   419 },
//  {  52,   522 },
//  {  52,    49 },
//  {  52,   419 },
//  {  52,   522 },
//  {  52,    49 },
//  {  52,   419 },
//  {  52,   522 },
//  {  52,   522 },
//  {  52,    49 },
//  {  52,     0 },
  {   0,     0 }
};

// This function delays the specified number of 10 microseconds
void delay_ten_us(unsigned long int us) 
{
  while (us != 0) {
	_delay_us(10);
    us--;
  }
}


// This function quickly pulses the visible LED (connected to PB0, pin 12)
void quickflashLED( void ) {
  // pulse LED at PB0 on for 30ms
  PORTB |= 0b00000001;          // turn on LED at PB0
  delay_ten_us(3000);           // 30 millisec delay
  PORTB &= 0b11111110;          // turn off LED at PB0
}


// This function quickly pulses the visible LED (connected to PB0, pin 12) 4 times
void quickflashLED4x( void ) {
  quickflashLED();
  delay_ten_us(15000);        // 150 millisec delay
  quickflashLED();
  delay_ten_us(15000);        // 150 millisec delay
  quickflashLED();
  delay_ten_us(15000);        // 150 millisec delay
  quickflashLED();
}


// This function transmits one Code Element of a POWER code to the IR emitter, 
//   given index to the codeElement in powerTab
//     If offTime = 0 that signifies the last Code Element of the POWER code
//     and the delay_ten_us function will have no delay for offTime 
//     (but we'll delay for 205 milliseconds in the main function)
void xmitCodeElement( int index ) {
  // start Timer0 outputting the carrier frequency to IR emitter on OC0A (PB2, pin 14)
  TCCR0A = 0b01000010;   // COM0A1:0=01 to toggle OC0A on Compare Match
                         // COM0B1:0=00 to disconnect OC0B from PB2 (pin 14)
                         // bits 3:2 are unused
                         // WGM01:00=10 for CTC Mode (WGM02=0 in TCCR0B)
  TCCR0B = 0b00000001;   // FOC0A=0 (no force compare)
                         // F0C0B=0 (no force compare)
                         // bits 5:4 are unused
                         // WGM2=0 for CTC Mode (WGM01:00=10 in TCCR0A)
                         // CS02:00=100 for divide by 1 prescaler

PORTB = 0xff;

  // keep transmitting carrier for onTime
  delay_ten_us(pgm_read_word(&powerTab[index].onTime));

  // turn off output to IR emitter on 0C0A (PB2, pin 14) for offTime
  TCCR0B &= 0b11111000;  // CS02:CS00=000 to stop Timer0
  TCCR0A &= 0b00111111;  // COM0A1:0=00 to disconnect 0OC0A from PB2 (pin 14)
  PORTB &= 0b11111011;   // turn off IR emitter on 0C0A (PB2, pin 14) in case it was High
	PORTB = 0;
  delay_ten_us(pgm_read_word(&powerTab[index].offTime));
}


int main(void) {

  TIMSK = 0x00;  // no Timer interrupts enabled
  DDRB = 0xFF;   // set all PortB pins as outputs
  PORTB = 0x00;  // all PORTB output pins Off

  // loop through entire POWER Table of Code Elements
  //   each Code Element consists of an On Time and an Off Time
  //     but the first Code Element of each POWER code is a fake one, 
  //       with the onTime = value for OCR0A to create the POWER code's carrier frequency (offTime is ignored)
  //     and the last Code Element of each POWER code has an offTime = 0
  //       (to indicate it is the last Code Element)
  //     and the last Code Element in the POWER Table (after the last POWER Code in the table) 
  //       has onTime = 0 (offTime is ignored) (to indicate that there are no more POWER Codes in the table)

while(1 ) {
  int i = 0;

  while (pgm_read_word(&powerTab[i].onTime) != 0) {  // onTime = 0 signifies end of powerTab
    // indicate transmission of next POWER code with a blink of the visible LED on PB0 (pin 12)
    quickflashLED();

    // set OCR0A for Timer0 to output this POWER code's carrier frequency
    OCR0A = pgm_read_word(&powerTab[i].onTime);  // the 1st codeElement of a POWER code contains value for OCR0A in its onTime

    // transmit all codeElements for this POWER code
    do {
      i++;                 // increment to point to next codeElement in powerTab
      xmitCodeElement(i);  // transmit one codeElement
    } while ( (pgm_read_word(&powerTab[i].offTime)) != 0 );  // offTime = 0 signifies last codeElement for a POWER code

    // delay 250 milliseconds before transmitting next POWER code
    delay_ten_us(25000);

    // point to beginning of next POWER code in powerTab (or possibly the end of powerTab)
    i++;                 // increment to point to next codeElement in powerTab
  }

  // flash the visible LED on PB0 (pin 12) 4 times to indicate that we're done
  quickflashLED4x();

}

  // Shut down everything and put the CPU to sleep
  TCCR0B &= 0b11111000;  // CS02:CS00=000 to stop Timer0 (turn off carrier frequency generation) -- although it should already be off
  TCCR0A &= 0b00111111;  // COM0A1:0=00 to disconnect 0OC0A from PB2 (pin 14)
  MCUCR |= 0b00100000;   // SE=1 (bit 5) -- enables Sleep Modes
  MCUCR |= 0b00010000;   // SM1:0=01 -- selects Power Down Sleep Mode (bits 6, 4)
  delay_ten_us(1000);    // wait 10 millisec second
  
  while(1)quickflashLED4x();

  PORTB = 0x00;          // turn off all PORTB outputs
  DDRB = 0x00;           // make PORTB all inputs
  sleep_cpu();           // put CPU into Power Down Sleep Mode
}
#endif
