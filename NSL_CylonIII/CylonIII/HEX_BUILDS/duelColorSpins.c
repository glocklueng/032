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

#define RED_MIN 1
#define RED_MAX 100
#define RED_INIT 50
#define GREEN_MIN 40
#define GREEN_MAX 60
#define GREEN_INIT 50


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
void SetRC( unsigned char index ) 
{	
	unsigned char b1,b2;
	unsigned char portb, ddrb,portd6=2;
	DDRB = 0x0;
	DDRD = 0x0;
	PORTB =0x0;
	PORTD = 0;

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



// Speaker drive interrupt
SIGNAL( SIG_TIMER1_COMPA ) 
{
	// toggle speaker on/off
	//SWITCH_BIT( PORTD , 4 );
}


// This function delays the specified number of 10 microseconds
void delay_ten_us(unsigned long int us) 
{
	us /= 8;

  while (us != 0) {
	_delay_us(10);
    us--;
  }
}

void spinAll(int speed) {
	
	
	int R;
	int speedMultipler = 2000 * speed;
	
	
			for ( R= 0 ; R < 30 ; R ++) 
			{
					SetRC(R);	
				
					delay_ten_us(speedMultipler);				
					delay_ten_us(speedMultipler);				
					delay_ten_us(speedMultipler);				
					delay_ten_us(speedMultipler);				
					delay_ten_us(speedMultipler);				
					delay_ten_us(speedMultipler);				
					delay_ten_us(speedMultipler);				
					delay_ten_us(speedMultipler);
			}
}	


void spinUp(int speed) {
	int R;
	int speedMultipler = 2000 * speed;
	
	
			for ( R= 0 ; R < 30 ; R +=2) 
			{
					SetRC(29-R);	
					delay_ten_us(speedMultipler);
			}
			
			for ( R= 30 ; R > 0 ; R -=2) 
			{
					SetRC(R);	
					delay_ten_us(speedMultipler);
			}
	
}

void spinDown(int speed) {
	int R;
	int speedMultipler = 2000 * speed;
	
				for ( R= 28 ; R >= 0 ; R -=2) 
			{
					SetRC(29-R);	
					delay_ten_us(speedMultipler);
			}
			
			for ( R= 0 ; R < 30 ; R +=2) 
			{
					SetRC(R);	
					delay_ten_us(speedMultipler);
			}
	
}


showGreen (int value) {
	value = 14 - value;
	SetRC((value * 2) + 1);
}

showRed (int value) {
	SetRC(value * 2);
}

int increment(int value) {
	
	value++;
	if (value > 14)
	 value = 0;
	
	return value;
}

int main(void)
{
	unsigned int x,c;
	unsigned char R,C;
	unsigned short i, j;

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

//	sei();


// index to start

int greenSpeed = GREEN_INIT;
int redSpeed = RED_INIT;


int redAcc = 1;
int greenAcc = 1;

int redValue = 0;
int greenValue = 0;
	
	while( 1 ) 
	{
				
					
					
		for (i = 0; i < 1024000; i++) {
			if ((i % redSpeed) == 0) {
			  redValue = increment(redValue);
			  if (1 == redAcc) {
				redSpeed--;
			  }	else {
				redSpeed++;
			  }	
			  
	  
			  if(redSpeed == RED_MIN) {
				  redAcc = 0;
			  }
			  if(redSpeed == RED_MAX) {
				  redAcc = 1;
			  }
			}
		  
			showRed(redValue);

			if ((i % greenSpeed) == 0) {
			  greenValue = increment(greenValue);
			  if (1 == greenAcc) {
				greenSpeed--;
			  }	else {
				greenSpeed++;
			  }	
			  
	  
			  if(greenSpeed == GREEN_MIN) {
				  greenAcc = 0;
			  }
			  if(greenSpeed == GREEN_MAX) {
				  greenAcc = 1;
			  }
			}
		  
			showGreen(greenValue);

		}
/// end
		
	}

}


