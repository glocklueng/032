#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "gcode.h"
#include "nuts_bolts.h"
#include "settings.h"
#include "motion_control.h"
#include "spindle_control.h"
#include "coolant_control.h"
#include "errno.h"
#include "serial_protocol.h"
#include "wiring_serial.h"
#include "panel_control.h"
#include "config.h"
#include "settings.h"

// Setup pin interrupts
void panel_init()
{
// set the panel as inputs
// home, morg switch
	DDRB  &= ~(_BV( PANEL_HOME ));
	PORTB |=   _BV( PANEL_HOME );

// head down switch
	DDRC  &= ~(_BV( PANEL_HEAD ));
	PORTC |=   _BV( PANEL_HEAD );

// go forward switch
	DDRE  &= ~(_BV( PANEL_FORWARD ));
	PORTE |=   _BV( PANEL_FORWARD );

	DDRG  &= ~(_BV( PANEL_RIGHT ));
	PORTG |=   _BV( PANEL_RIGHT );

// fast move switch
	DDRJ  &= ~(_BV( PANEL_FAST ));
	PORTJ |=   _BV( PANEL_FAST );

	DDRJ  &= ~(_BV( PANEL_BACK ));
	PORTJ |=   _BV( PANEL_BACK );

// switch on vacuum
	DDRH  &= ~(_BV( PANEL_VAC ));
	PORTH |=   _BV( PANEL_VAC );

// teach mode
	DDRH  &= ~(_BV( PANEL_TEACH ));
	PORTH |=   _BV( PANEL_TEACH );

	TCCR3A |= (1 << WGM21); //ctc mode
	OCR3A   = 124; //set timer maximum to 124
	TCNT3   = 0;//reset timer
	TCCR3B  = 0b00000100; //((1 << CS22)|(1 << CS20));  //prescaler 64
	TIMSK3 |= (1 << OCIE2A);  //enable timer interrupt

}

unsigned char is_pleft( void )
{
	unsigned char state ;

	state  = bit_is_set( PINE, PANEL_LEFT )?0:1;

	return state;
}

unsigned char is_pright( void )
{
	unsigned char state ;

	state  = bit_is_set( PING, PANEL_RIGHT )?0:1;

	return state;
}

unsigned char is_pback( void )
{
	unsigned char state ;

	state  = bit_is_set( PINE, PANEL_BACK )?0:1;

	return state;
}

unsigned char is_pforward( void )
{
	unsigned char state ;

	state  = bit_is_set( PINJ, PANEL_FORWARD )?0:1;

	return state;
}

unsigned char is_phead( void )
{
	unsigned char state ;

	state  = bit_is_set( PINC, PANEL_HEAD )?0:1;

	return state;
}

unsigned char is_pvac( void )
{
	unsigned char state ;

	state  = bit_is_set( PINH, PANEL_VAC )?0:1;

	return state;
}

unsigned char is_pfast( void )
{
	unsigned char state ;

	state  = bit_is_set( PINJ, PANEL_FAST )?0:1;

	return state;
}

unsigned char is_phome( void )
{
	unsigned char state ;

	state  = bit_is_set( PINB, PANEL_HOME )?0:1;

	return state;
}


unsigned char is_pteach( void )
{
	unsigned char state ;

	state  = bit_is_set( PINH, PANEL_TEACH )?0:1;

	return state;
}

void process_panel(void ) 
{
	static	char bvac = 0;
	static	char bhead = 0;

	if( is_phome() ) mc_go_home();

	if( is_pvac() ) {
		bvac = 1;
		vacuum(1);
	} else if(bvac == 1 ) {
		bvac = 0;
		vacuum(0);
	}	

	if( is_phead() ) {
		bhead = 1;
		head_down(1);
	} else if(bhead == 1 ) {
		bhead = 0;
		head_down(0);
	}	


}


// Put the panel on an interrupt
SIGNAL(TIMER3_COMPA_vect)
{
	return;
}


