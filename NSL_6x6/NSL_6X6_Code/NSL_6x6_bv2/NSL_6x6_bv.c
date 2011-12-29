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

char fuckyou[35] = {
	0b00000000,	
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111111,
	0b00001001,
	0b00001001,
	0b00000000,
	0b00111111,
	0b00100000,
	0b00111111,
	0b00000000,
	0b00111111,
	0b00100001,
	0b00100001,
	0b00000000,
	0b00111111,
	0b00001000,
	0b00010100,
	0b00100010,
	0b00000000,
	0b00000000,
	0b00000111,
	0b00111000,
	0b00000111,
	0b00000000,
	0b00111111,
	0b00100001,
	0b00111111,
	0b00000000,
	0b00111111,
	0b00100000,
	0b00111111
	};

void delay_ten_us(unsigned long int us) 
{
	us /= 8;

  while (us != 0) {
	_delay_us(10);
    us--;
  }
}



static void InitRC()
{
	PORTD = 0b00111111;
	//PORTD &= ( _BV( PD0 ) | _BV( PD1 ) ); //PD0 and PD1 should be 1
	PORTD |= ( _BV( PD5 ) | _BV( PD4 ) | _BV( PD3 ) | _BV( PD2 ) ); // PD5, PD4, PD3, PD2 = 1 turns all rows off
	PORTB &= ( _BV( PB5 ) | _BV( PB6 ) | _BV ( PB7) ) ;	
	PORTA |= ( _BV(PA0) | _BV(PA1) );
	}

static void RowByte( unsigned char row )
{
	if (GET_BIT(row,5)==GET_BIT(PORTD,PD5)) SWITCH_BIT(PORTD,PD5);
	if (GET_BIT(row,4)==GET_BIT(PORTD,PD4)) SWITCH_BIT(PORTD,PD4);
	if (GET_BIT(row,3)==GET_BIT(PORTD,PD3)) SWITCH_BIT(PORTD,PD3);
	if (GET_BIT(row,2)==GET_BIT(PORTD,PD2)) SWITCH_BIT(PORTD,PD2);
	if (GET_BIT(row,1)==GET_BIT(PORTA,PA0)) SWITCH_BIT(PORTA,PA0);
	if (GET_BIT(row,0)==GET_BIT(PORTA,PA1)) SWITCH_BIT(PORTA,PA1);
}

static void RowON( unsigned char row )
{

	switch( row ) {
		case 0:
			CLR_BIT(PORTD,PD5);
			break;
		case 1:
			CLR_BIT(PORTD,PD4);
			break;
		case 2:
			CLR_BIT(PORTD,PD3);
			break;
		case 3:
			CLR_BIT(PORTD,PD2);
			break;
		case 4:
			CLR_BIT(PORTA,PA0);
			break;
		case 5:
			CLR_BIT(PORTA,PA1);
			break;
		}
}

static void ColON( unsigned char column )
{
	switch( column ) {
		case 0:
			SET_BIT(PORTD, PD6);
			break;
		case 1:
			SET_BIT(PORTB, PB0);
			break;
		case 2:
			SET_BIT(PORTB, PB1);
			break;
		case 3:
			SET_BIT(PORTB, PB2);
			break;
		case 4:
			SET_BIT(PORTB, PB3);
			break;
		case 5:
			SET_BIT(PORTB, PB4);
			break;
	}
}

static void SetLED( unsigned char row, unsigned char col ) 
{	
	PORTD &= ( _BV( PD0 ) | _BV( PD1 ) ); //PD0 and PD1 should be 1
	PORTD |= ( _BV( PD5 ) | _BV( PD4 ) | _BV( PD3 ) | _BV( PD2 ) ); // PD5, PD4, PD3, PD2 = 1 turns all rows off
	PORTB &= ( _BV( PB5 ) | _BV( PB6 ) | _BV ( PB7) ) ;	//
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

// This function delays the specified number of 10 microseconds

static void fillrow(unsigned int from, unsigned int to, unsigned int delay)
{
	int dir=1,X;
	if (to<from) dir=-1;
	for (X=0;X<6;X++){
		ColON(X);
		}
	for (X=from;(MIN_OF(from,to)<X<MAX_OF(from,to));X+=dir){
		if (X>(MAX_OF(from,to))) break;
		if (X<(MIN_OF(from,to))) break;
		RowON(X);
		delay_ten_us(delay);
		}
}

static void drawframe( unsigned int frame , unsigned int delayon, unsigned int delayoff )
{
	InitRC();
	delay_ten_us(delayoff);
	ColON(5);
	RowByte(fuckyou[frame]);
	delay_ten_us(delayon);
	InitRC();
	delay_ten_us(delayoff);
	ColON(4);
	RowByte(fuckyou[frame+1]);
	delay_ten_us(delayon);
	InitRC();
	delay_ten_us(delayoff);
	ColON(3);
	RowByte(fuckyou[frame+2]);
	delay_ten_us(delayon);
	InitRC();
	delay_ten_us(delayoff);
	ColON(2);
	RowByte(fuckyou[frame+3]);
	delay_ten_us(delayon);
	InitRC();
	delay_ten_us(delayoff);
	ColON(1);
	RowByte(fuckyou[frame+4]);
	delay_ten_us(delayon);
	InitRC();
	delay_ten_us(delayoff);
	ColON(0);
	RowByte(fuckyou[frame+5]);
	delay_ten_us(delayon);
}

int main(void)
{
	unsigned char R,C;
	unsigned int X, frame;
	R=0b11111100;
	X=0;
	C=0;
	frame=0;
	DDRA   = _BV( 0 ) | _BV( 1 );
	DDRB   =  (_BV(0) | _BV(1) |_BV(2) |_BV(3) |_BV(4) |_BV(5) );
	DDRD   = _BV( 6) |_BV( 5 ) | _BV( 4 ) |_BV( 3 ) |_BV( 2 );

	while( 1 ) 
	{
	X++;
	if (X>75) {frame++; X=0;}
	if (frame>35) frame=0;
	
	drawframe(frame, 10, 0);
	}
}


