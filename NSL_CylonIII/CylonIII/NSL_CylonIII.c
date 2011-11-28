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

// 		PORTB,DDRB,PIN
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
	DDRD  = 0;
	PORTD = 0;

	index = index - 30 * (index / 30);

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

unsigned int X = 0;


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

int main(void)
{
	unsigned int x,c;
	unsigned char R,C;
	unsigned short i;

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

	sei();


// index to start
	i = 0;

	X = 0;

	while( 1 ) 
	{
		for ( X = 0 ; X < 5 ; X++ ) {
			for ( R= 0 ; R < 30 ; R +=2) 
			{
				for ( c = 0 ; c < 100 ; c ++) {
					SetLED(29-R);	
					//delay_ten_us(1);	
					SetLED(R);
//					delay_ten_us(2000);	
				}
				
			}
		}

		R = 0;	
		C = 29;	
				
		for ( X = 0 ; X < 200 ; X++ ) {
			for ( i = 0 ; i < 10 ; i++ ) {
				SetLED(R);	
				SetLED(R+1);	
				delay_ten_us(10);	
				SetLED(R+2);	
				delay_ten_us(25);	
				SetLED(R+3);	
				delay_ten_us(10);	
				SetLED(R+4);	

				SetLED(C+15);	
				SetLED(C+1+15);	
				delay_ten_us(10);	
				SetLED(C+2+15);	
				delay_ten_us(25);	
				SetLED(C+3+15);	
				delay_ten_us(10);	
				SetLED(C+4+15);	
			}

			R++;
			if ( R == 30 ) R = 0;
			C--;
			if( c == 0 ) c = 29;
		}

		R= 0 ;

		for ( X = 0 ; X < 200 ; X++ ) {
			for ( i = 0 ; i < 20 ; i++ ) {
				SetLED(R);	
				SetLED(R+1);	
				delay_ten_us(20);	
				SetLED(R+2);	
				delay_ten_us(100);	
				SetLED(R+3);	
				delay_ten_us(20);	
				SetLED(R+4);	
				delay_ten_us(10);	
				SetLED(R+5);	
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
					SetLED(c+R)
					;
				}
				delay_ten_us(30-R);
				if( (i&1) == 1 ) 
					c++;

			}
		}
		for ( X = 0 ; X < 400 ; X++ ) 
			for ( R= 0 ; R < 30 ; R ++ ) 
			{


				SetLED(R);	
				delay_ten_us(80);

				
			}

		for ( X = 0 ; X < 5 ; X++ ) 
			for ( R= 0 ; R < 30 ; R ++ ) 
			{

				SetLED(R);
				i = 150*(((R+1)*6));
				delay_ten_us(i);
				delay_ten_us(80);
;

			}

		for ( X = 0 ; X < 5 ; X++ ) 
			for ( R= 0 ; R < 30 ; R ++ ) 
			{
				SetLED(R);
				i = 150*(((R+1)*6));
				delay_ten_us(i);
				delay_ten_us(80);
			}
		
		for ( X = 0 ; X < 200 ; X++ ) 
			for ( R= 0 ; R < 30 ; R ++ ) 
			{
				SetLED(R);	
				delay_ten_us(X*R);
			}
	}

}


