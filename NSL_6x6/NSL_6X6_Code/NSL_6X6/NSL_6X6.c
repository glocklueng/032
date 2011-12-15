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

Row:
1		2		3		4		5		6
PD5		PD4		PD3		PD2		PA0		PA1

Column:
1		2		3		4		5		6
PD6		PB0		PB1		PB2		PB3		PB4


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
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00
};

static const unsigned char PROGMEM _state_a[30] = {
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00
};

static const unsigned char PROGMEM _porta[30] = {
	0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,
	0x01,0x01,0x01,0x01,0x01,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00
};


static const unsigned char PROGMEM _state_b[30] = {
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00
};

static const unsigned char PROGMEM _portb[30] = {
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00
};


static const unsigned char PROGMEM _state_d[30] = {
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00
};

static const unsigned char PROGMEM _portd[30] = {
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00
};


/*
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
*/
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

// switch on interrupts ( start speaker )

	sei();


// index to start
	i = 0;

	X = 0;

	while( 1 ) 
	{
		/* SOOON
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
		*/

		PORTA  = (char)(rand() % 128);
		DDRA   = (char)(rand() % 128);
		
		
		PORTB  = (char)(rand() % 128);
		DDRB   = (char)(rand() % 128);

		PORTD  = (char)(rand() % 128);
		DDRD   = (char)(rand() % 128);
	}

}


