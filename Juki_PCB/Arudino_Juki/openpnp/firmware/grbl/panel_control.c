#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

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

}

unsigned char is_pleft( void )
{
	unsigned char state ;

	state  = bit_is_set( PINB, PANEL_LEFT );

	return state;
}

unsigned char is_pright( void )
{
	unsigned char state ;

	state  = bit_is_set( PING, PANEL_RIGHT );

	return state;
}

unsigned char is_pback( void )
{
	unsigned char state ;

	state  = bit_is_set( PINJ, PANEL_BACK );

	return state;
}

unsigned char is_pforward( void )
{
	unsigned char state ;

	state  = bit_is_set( PINE, PANEL_FORWARD );

	return state;
}

unsigned char is_phead( void )
{
	unsigned char state ;

	state  = bit_is_set( PINC, PANEL_HEAD );

	return state;
}


unsigned char is_pvac( void )
{
	unsigned char state ;

	state  = bit_is_set( PINH, PANEL_VAC );

	return state;
}

unsigned char is_pfast( void )
{
	unsigned char state ;

	state  = bit_is_set( PINJ, PANEL_FAST );

	return state;
}

unsigned char is_phome( void )
{
	unsigned char state ;

	state  = bit_is_set( PINB, PANEL_HOME );

	return state;
}
