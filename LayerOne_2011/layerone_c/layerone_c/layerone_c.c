/*
 *
 * LAYERONE 2011
 * layerone_c.c
 * 
 */ 

// This line is a heat sink
 
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/pgmspace.h>

#include <stdlib.h>
#include <util/delay.h>
#include "init.h"
#include "triangle.h"

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

// 	PORTA,DDRA,PINA
#define ddr_a(r,c)  charlieplex[ (r*(3*4)) + ((c*3)+1) ]
#define pin_a(r,c)  charlieplex[ (r*(3*4)) + ((c*3)+2) ]
#define port_a(r,c) charlieplex[ (r*(3*4)) + ((c*3)+0) ]

#define HEX__(n) 0x##n##LU

/* 8-bit conversion function */
#define B8__(x)	((x&0x0000000FLU)?1:0)	\
			   +((x&0x000000F0LU)?2:0)	\
			   +((x&0x00000F00LU)?4:0)	\
			   +((x&0x0000F000LU)?8:0)	\
			   +((x&0x000F0000LU)?16:0)	\
			   +((x&0x00F00000LU)?32:0)	\
			   +((x&0x0F000000LU)?64:0)	\
			   +((x&0xF0000000LU)?128:0)

/* *** user macros ***/

/* for up to 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* for up to 16-bit binary constants, MSB first */
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<	+ B8(dlsb)))

/* for up to 32-bit binary constants, MSB first */
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24)	 \
							 + ((unsigned long)B8(db2)<<16)		 \
							 + ((unsigned long)B8(db3)<<		 \
							 + B8(dlsb))

//  State Definitions for each individual LED
//  A2  A1  A0  A4  A3
//  PB0 PB1 PB2 PB3 PB4  
//  PA2 PA1 PA0 PA4 PA3
//
//	00 = LOW
//	01 = HIGH
//	11 = X (input)
static const unsigned char PROGMEM led30[] = 
{
	B8( 00) , B8(01111111 ),		// LED25
	B8( 11) , B8(00011111 ),		// LED19
	B8( 11) , B8(11000111 ),		// LED11
	B8( 11) , B8(11110001 ),		// LED1
	
	B8( 01) , B8(00111111 ),		// LED26
	B8( 11) , B8(01001111 ),		// LED20
	B8( 11) , B8(11010011 ),		// LED12
	B8( 11) , B8(11110100 ),		// LED2
	
	B8( 00) , B8(11110111 ),		// LED15
	B8( 00),  B8(11011111 ),		// LED21
	B8( 11) , B8(00110111 ),		// LED13
	B8( 11) , B8(11001101 ),		// LED3
	
	B8( 01) , B8(11110011 ),		// LED16
	B8( 01) , B8(11001111 ),		// LED22
	B8( 11) , B8(01110011 ),		// LED14
	B8( 11) , B8(11011100 ),		// LED4

	B8( 01) , B8(11111100 ),		// LED8
	B8( 00),  B8(11111101 ),		// LED7
	B8( 11) , B8(01111100 ),		// LED6
	B8( 11) , B8(00111101 ),		// LED5
};


// Hardware Variables
unsigned long adc_channel[7];		// Holds the 10-bit ADC values
unsigned char button_state[16];		// Holds the state of the 16 buttons
unsigned char led_state[20] =		// Holds the state of the 20 LED's
{	
	1,0,1,0,						// Initialize the LED Matrix with a fun pattern for the whole family!
	0,1,0,1,
	1,0,1,0,
	0,1,0,1,
	1,0,1,0,
};		
unsigned char dac_value = 0;		// Holds the DAC value

//-------------------------------------------------
//			M A I N   P R O G R A M
//-------------------------------------------------
int main(void)
{
	cli();					// Disable Global Interrupts - Disable because we have srs bizness to do 

	init();					// Initialize the AtMega64 registers based on hardware
	init_timers();			// Initialize Timers
	init_interrupts();		// Initialize Interrupts
	
	sei();					// Enable Global Interrupts - Let's get this show on the road
		
	// MAIN LOOP   
	while(1)
	{	
		// DO STUFF!
		scrollLEDS();
	}
	return 0;
}


//-------------------------------------------------
//		H A R D W A R E  F U N C T I O N S
//-------------------------------------------------

// Read Button States
void readButtonStates(unsigned char *button_state)
{
	// PD7	PD6  PD5  PD4		// Outputs		DDRD = 0xFn (Buttons)
	// M1   M2   M3   M4		// Select Row	
	
	// PB3  PB2  PB1  PB0		// Input		DDRB = 0x0n (Buttons)
								// Read Column	
	
	// button_state[i] = Switch #i+1
	// i.e. - button_state[0] = Switch #1
	//		  button_state[5] = Switch #6
	//		  ...
	signed char i = 3;
	while(i >= 0)
	{	
		PORTD = ((~((1<<i)<<4)) & 0xF0) | (PORTD & 0x0F);	// Select Row
		//shortdelay();					
		// Note: You are reading for a low 
		//       Port B is held high with pull-up resistors
		//       Port D pins are cycled through and brought low to read each Port B column
		button_state[3-i] = ((~PINB & 0x01) >> 0);			// Read PB0 
		button_state[7-i] = ((~PINB & 0x02) >> 1);			// Read PB1
		button_state[11-i] = ((~PINB & 0x04) >> 2);			// Read PB2
		button_state[15-i] = ((~PINB & 0x08) >> 3);			// Read PB3
		
		i--;
	}					
}

// Read ADC CHANNEL 0 - 7
void readADC(unsigned long *adc_channel) 
{ 
	unsigned char i = 0;		// Start reading at channel 0
	while(i <= 7)
	{
		ADMUX = i;				// Set channel to read
		ADCSRA |= (1 << ADSC);  // ADC Start Conversion
		while(!ADIF);			// Wait for ADC to clear	
		adc_channel[i] = (ADCH << 8) | ADCL; // Combine MSB and LSB of the ADC
		
		i++;					// Increment through channel
	}
}

// Set LED Array
void setLEDS(unsigned char *led)
{
	// Note: This LED array uses charlieplexing
	// PA0 PA1 PA2 PA3 PA4
	// A0  A1  A2  A3  A4
	
	int c = 0;
	int r = 0;
		
	for(r = 0; r <= 4; r++)
	{
		for(c = 0; c <= 3; c++)
		{
			if(led[(r*4)+c] > 0)
			{
				SetRC(r,c);
			}															
		}
	}
}	
	
// Set Digital to Analog Converter	
void setDAC(unsigned char *signal)
{
	// signal = 0	 : 0   volts
	// ...
	// signal = 1024 : VCC volts
	
	PORTC = signal;
}


// Write to LED Array - Takes in a Row and Column
// this routine is written to help understand how the process works, not fast.
void SetRC( unsigned char R, unsigned char C ) 
{	
	unsigned char b1,b2;
	unsigned char port_a, ddr_a;

	if ( C  > 4  ) C = 0;
	if ( R  > 5  ) R = 0;

	// get current values, Can't cache PORTA/DDRA since the sound interrupt can change, 
	// should cache DDRD6/PORTD6 instead
	port_a = PORTA;
	ddr_a = DDRA;

	//	read two values from array, we use pgm_read_byte because the data is in program memory space
	// its 4*2 because there are 4 leds in each column, and two bytes per led
	b1 = pgm_read_byte(&led30[ (R*(4*2))+(C*2)   ] ) ;
	b2 = pgm_read_byte(&led30[((R*(4*2))+(C*2))+1] ) ;

	//	b1 = led30[ (R*(4*2))+(C*2)   ]  ;
	//	b2 = led30[((R*(4*2))+(C*2))+1];
	
	// if 10 == 11 then set to don't care (input ) tristate for PB0
	if( GET_BIT( b1,1 )==1 && GET_BIT( b1,0 )==1 ) {

		CLR_BIT(ddr_a,2 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,2 );
	}

	// if 76 == 11 then set to don't care (input ) tristate  for PB1
	if( GET_BIT( b2,7 )==1 && GET_BIT( b2,6 )==1 ) {

		CLR_BIT(ddr_a,1 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,1 );
	}

	// if 54 == 11 then set to don't care (input ) tristate for PB2
	if( GET_BIT( b2,5 )==1 && GET_BIT( b2,4 )==1 ) {

		CLR_BIT(ddr_a,0 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,0 );
	}

	// if 32 == 11 then set to don't care (input ) tristate for PB3
	if( GET_BIT( b2,3 )==1 && GET_BIT( b2,2 )==1 ) {

		CLR_BIT(ddr_a,4 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,4 );
	}

	// if 10 == 11 then set to don't care (input ) tristate for PB4
	if( GET_BIT( b2,1 )==1 && GET_BIT( b2,0 )==1 ) {

		CLR_BIT(ddr_a,3 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,3 );
	}

	/// all of the DDR's are set


	// if 10 == 00 then set to low for PB0
	if( GET_BIT( b1,1 )==0 && GET_BIT( b1,0 )==0 ) {

		CLR_BIT(port_a,2); //set to off

	// if 32 == 01 then set to high for PB0
	} else if( GET_BIT( b1,1 )==0 && GET_BIT( b1,0 )==1 ) { 

		// set to on
		SET_BIT(port_a ,2 );
	}

	// if 76 == 00 then set to low for PB1
	if( GET_BIT( b2,7 )==0 && GET_BIT( b2,6 )==0 ) {

		CLR_BIT(port_a,1); //set to off

	// if 32 == 01 then set to high for PB1
	} else if( GET_BIT( b2,7 )==0 && GET_BIT( b2,6 ) ==1 ) { 

		// set to on
		SET_BIT(port_a ,1 );
	}


	// if 54 == 00 then set to low for PB2
	if( GET_BIT( b2,5 )==0 && GET_BIT( b2,4 )==0 ) {

		CLR_BIT(port_a,0); //set to off

	// if 32 == 01 then set to high for PB2
	} else if( GET_BIT( b2,5 )==0 && GET_BIT( b2,4 )==1 ) { 

		// set to on
		SET_BIT(port_a ,0 );
	}

	// if 32 == 00 then set to low for PB3
	if( GET_BIT( b2,3 )==0 && GET_BIT( b2,2 )==0 ) {

		CLR_BIT(port_a,4); //set to off

	// if 32 == 01 then set to high for PB3
	} else if( GET_BIT( b2,3 ) ==0 && GET_BIT( b2,2 )==1 ) { 

		// set to on
		SET_BIT(port_a ,4 );
	}

	// if 10 == 00 then set to low for PB4
	if( GET_BIT( b2,1 )==0 && GET_BIT( b2,0 )==0 ) {

		CLR_BIT(port_a,3); //set to off

	// if 32 == 01 then set to high for PB4
	} else if( GET_BIT( b2,1)==0 && GET_BIT( b2,0 )==1 ) { 

		// set to on
		SET_BIT(port_a ,3 );
	}

	//set all at once.
	PORTA = port_a;
	DDRA = ddr_a;
}






//-------------------------------------------------
//			   I N T E R R U P T S
//-------------------------------------------------

/* USART Received byte interrupt (get MIDI byte)*/
ISR(USART_RX_vect) 
{
	byte_received = UDR0;
	byte_ready = 1;
}

ISR(TIMER1_COMPA_vect)
{
	// Code to update all states
	// ***************************************
	readADC(&adc_channel[0]);				// Read ADC
	shortdelay();
	readButtonStates(&button_state[0]);		// Read Button States
	shortdelay();		
	setLEDS(&button_state[0]);				// Drive LEDs
	shortdelay();
	setDAC(&dac_value);						// Drive DAC
	shortdelay();
	// ***************************************
}

ISR(TIMER0_OVF_vect) 
{
	// TESTING TIMER0 - REMOVE THIS CODE AFTER TESTING

}



//-------------------------------------------------
//		P R O G R A M  F U N C T I O N S
//-------------------------------------------------

void stepDisplayGlow(void)
{					
	// GLOWLY MODE - This is just an example to make the screen glow
	// Note: There are good odds that PA0 - PA4 won't be given 
	PORTA = (unsigned char)(1 + rand()%31);		// Random number between 1 - 31 											
	DDRA = (unsigned char)(1 + rand()%31);		// Then write it to ports
}


void scrollLEDS(void)
{
	int c = 0;
	int r = 0;
		
	for(r = 0; r <= 4; r++)
	{
		for(c = 0; c <= 3; c++)
		{
				SetRC(r,c);
				int i = 0;
				for(i=0; i < 29000; i++)
				{
					shortdelay();
					shortdelay();
				}														
		}
	}
}