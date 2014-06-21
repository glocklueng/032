/*
 * ztable.c
 *
 * Created: 6/21/2014 1:31:49 PM
 *  Author: charlie
 */ 


#include <avr/io.h>
#include <util/delay.h>

/*
	Outputs
		PB2  = PULSE PWM Output
		PD6 = DIR PIN11
	
	Inputs
		PD5 = Input UP
		PD4 = Input Down
		PD3 = Input UP Limit
		PD2 = Input Down Limit
*/

#define SW_UP( ) (!( PIND & (1 << PD5) ))
#define SW_DOWN( ) (!( PIND & (1 << PD4) ))

#define SW_LIMIT_UP( ) (!( PIND & (1 << PD3) ))
#define SW_LIMIT_DOWN( ) (!( PIND & (1 << PD2) ))

// Enable PWM output
#define ENABLE_PWM( )			DDRB |= (1 << PB2)

// Disable PWM output
#define DISABLE_PWM( )          DDRB &= ~(1 << PB2)

// Set DIR direction
#define DIR( x )				{ if (x)  PORTB |= (1 << PB3); else PORTB &= (0 << PB3);     }
	
int main(void)
{
	//DDRB   |= (1 << PB2);                   // PWM output on PB2
	DISABLE_PWM();
		
	TCCR0A = (1 << COM0A1) | (1 << WGM00);  // phase correct PWM mode
	OCR0A  = 0x10;                          // initial PWM pulse width
	    
	TCCR0B = (1 << CS01);					// clock source = CLK/8, start PWM
		
	// output pins
	DDRB |= (1 << PB3);     // DIR on pin 13 (PB3)
	
	// input pins
	DDRD &= ~(1 << PD5);    // switch on pin 7 (PD5)
	PORTD |= (1 << PD5);    // enable pull-up resistor

	DDRD &= ~(1 << PD4);    // switch on pin 6 (PD4)
	PORTD |= (1 << PD4);    // enable pull-up resistor
	
	DDRD &= ~(1 << PD3);    // switch on pin 5 (PD3)
	PORTD |= (1 << PD3);    // enable pull-up resistor
	
	DDRD &= ~(1 << PD2);    // switch on pin 4 (PD2)
	PORTD |= (1 << PD2);    // enable pull-up resistor
			
    while(1)
    {
		if( SW_UP() ) {    // Input UP
			
			DIR( 1 ) ;	
			ENABLE_PWM();

			while (SW_UP());						
			DISABLE_PWM();
			
		} else if (SW_DOWN() ) {    // Input Down
			
			DIR( 0 ) ;	
			ENABLE_PWM();
			
			while (SW_DOWN());
			DISABLE_PWM();
			
		}
    }
}