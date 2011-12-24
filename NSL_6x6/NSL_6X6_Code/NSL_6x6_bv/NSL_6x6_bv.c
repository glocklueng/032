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


static void SetLED( unsigned char row, unsigned char col ) 
{	
	PORTD &= ( _BV( PD0 ) | _BV( PD1 ) );
	PORTD |= ( _BV( PD5 ) | _BV( PD4 ) | _BV( PD3 ) | _BV( PD2 ) );
	PORTB &= ( _BV( PB5 ) | _BV( PB6 ) | _BV ( PB7) ) ;
	PORTA |= ( _BV(PA0) | _BV(PA1) );

	switch( col ) {
		case 0:
			PORTD  |= _BV(PD6);
			break;
		case 1:
			PORTB  |= _BV(PB0);
			break;
		case 2:
			PORTB  |= _BV(PB1);
			break;
		case 3:
			PORTB  |= _BV(PB2);
			break;
		case 4:
			PORTB  |= _BV(PB3);
			break;
		case 5:
			PORTB  |= _BV(PB4);
			break;
	}
	
	switch( row ) {
		case 0:
			PORTD  &= ~(_BV(PD5));
			break;
		case 1:
			PORTD  &= ~(_BV(PD4));
			break;
		case 2:
			PORTD  &= ~(_BV(PD3));
			break;
		case 3:
			PORTD  &= ~(_BV(PD2));
			break;
		case 4:
			PORTA  &= ~(_BV(PA0));
			break;
		case 5:
			PORTA  &= ~(_BV(PA1));
			break;
	}
	
	// these are the rows
	//PORTD |= _BV( PD5 ) | _BV( PD4 ) |_BV( PD3 ) | _BV( PD2 ) ;
	
	//PORTA |= _BV( PA0 ) | _BV( PA1 );

}

unsigned int X = 0;


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
	unsigned char R,C;
	unsigned int i;

	DDRA   = _BV( 0 ) | _BV( 1 );
	DDRB   =  (_BV(0) | _BV(1) |_BV(2) |_BV(3) |_BV(4) |_BV(5) );
	DDRD   = _BV( 6) |_BV( 5 ) | _BV( 4 ) |_BV( 3 ) |_BV( 2 );

	R = 0 ;
	C = 0 ;	
	i =0;	

	while( 1 ) 
	{
		//PORTA=0;PORTD=0;PORTB=0;

		//if( i < 6 ) 
		

		//if(C==5)
		//	i++;
	
		//if( i== 900 ) 
		//	i=0;

	
		delay_ten_us(4500);
		SetLED( R,C);
		C++ ;
		R++ ; 
		if ( R==6) C++ ;
		if( R >= 6 ) R -= 6;
		if( C >= 6 ) C -= 6;

	}

}


