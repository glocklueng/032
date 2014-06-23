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
		PB2 = PULSE PWM Output (depends on mode, doesn't have to use PWM)
		PB3 = DIR PIN11
	
	Inputs
		PD5 = Input UP
		PD4 = Input Down
		PD3 = Input UP Limit
		PD2 = Input Down Limit
*/

// 10 is 49khz
// 20 is 24khz
// 40 is 12khz
// 74 is 6.8khz
// 80 is 6.3khz
// 82 is 6.1khz
// 83 is 6.02khz

// define this to use the toggle mode
#define TOGGLE_SPEED		( 83 )

#define SW_LIMIT_UP( ) (!( PIND & (1 << PD3) ))
#define SW_LIMIT_DOWN( ) (!( PIND & (1 << PD2) ))

// Enable PWM output
#define ENABLE_PWM( )			TCCR0B = _BV(CS01)
// Disable PWM output
#define DISABLE_PWM( )          TCCR0B = 0

// Set DIR direction
#define DIR( x )				{ if (x)  PORTB |= (1 << PB3); else PORTB &= (0 << PB3);     }

/*
	// 32khz
	TCCR0A =  _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00); 

	// 15khz clk/1
	TCCR0A =  _BV(COM0A1) | _BV(COM0B1)  | _BV(WGM00); 

	// 3khz, clk/8
	TCCR0A =  _BV(COM0A1) | _BV(COM0B1)  | _BV(WGM01)  | _BV(WGM00); 

	// 1.9khz, clk/8
	TCCR0A =  _BV(COM0A1) | _BV(COM0B1)  | _BV(WGM00); 
*/
	
int main(void)
{

	PORTB = 0 ;
	PORTD = 0 ;
	DDRB  = 0 ;
	DDRD  = 0 ;

	DDRB   |= (1 << PB2);                   // PWM output on PB2

#ifndef TOGGLE_SPEED
	TCCR0A = (1 << COM0A1) | (1 << WGM00);  // Fast PWM,   was phase correct PWM mode
#endif

#ifndef TOGGLE_SPEED
	OCR0A  = 127;                          // initial PWM pulse width 50%
	DISABLE_PWM();
#endif
	   
	// output pins
	DDRB |= (1 << PB3);     // DIR on pin 13 (PB3)
	
	// input pins (wasnt working on proto board)
	//DDRD &= ~(1 << PD5);    // switch on pin 7 (PD5)
	//PORTD |= (1 << PD5);    // enable pull-up resistor

	DDRD &= ~(1 << PD4);    // switch on pin 6 (PD4)
	PORTD |= (1 << PD4);    // enable pull-up resistor
	
	DDRD &= ~(1 << PD3);    // switch on pin 5 (PD3)
	PORTD |= (1 << PD3);    // enable pull-up resistor
	
	DDRD &= ~(1 << PD2);    // switch on pin 4 (PD2)
	PORTD |= (1 << PD2);    // enable pull-up resistor
	  
// PWM test mode
	if( 0 ) {

		ENABLE_PWM();

		while(1){
				DIR( 1 ) ;	
				_delay_ms(1000);
				DIR(0);
				_delay_ms(1000);
		}
	}

    while(1)
    {
		if( bit_is_clear(PIND, PD3)  ) {    // Input UP
			
			DIR( 1 ) ;
				
#ifndef TOGGLE_SPEED

			ENABLE_PWM();

			while (bit_is_clear(PIND, PD3));
#else		
	
			while (bit_is_clear(PIND, PD3)) {

				// if the limit switch is checked in here, toggle speed would have to change

				PORTB |=  (1 << PB2);
				_delay_us(TOGGLE_SPEED);

				PORTB &= ~(1 << PB2);
				_delay_us(TOGGLE_SPEED);
			}		
#endif
					
#ifndef TOGGLE_SPEED

			DISABLE_PWM();

#endif			
		} else if (bit_is_clear(PIND, PD4)   ) {    // Input Down
			
			DIR( 0 ) ;
				
#ifndef TOGGLE_SPEED

			ENABLE_PWM();
			
			while (bit_is_clear(PIND, PD4) );
#else

			while (bit_is_clear(PIND, PD4) ){
						
				// if the limit switch is checked in here, toggle speed would have to change

				PORTB |=  (1 << PB2);
				_delay_us(TOGGLE_SPEED);

				PORTB &= ~(1 << PB2);
				_delay_us(TOGGLE_SPEED);
			}
#endif

#ifndef TOGGLE_SPEED
			DISABLE_PWM();
#endif			
		}
    }
}
