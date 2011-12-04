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
	PANEL_DDR  &= ~(_BV( PANEL_LEFT ));
	PANEL_DDR |= _BV( PANEL_LEFT );

// setup panel sensor
	panelDT_DDR  &= ~(_BV( PANEL_LEFT ));
	panelDT_PORT |= (_BV( PANEL_LEFT ));

}


unsigned char is_pleft( void )
{
	unsigned char state ;

	state  = bit_is_set( panelDT_PIN, PANEL_LEFT );

	return state;
}

unsigned char is_pright( void )
{
	unsigned char state ;

	state  = bit_is_set( panelDT_PIN, PANEL_RIGHT );

	return state;
}

unsigned char is_pback( void )
{
	unsigned char state ;

	state  = bit_is_set( panelDT_PIN, PANEL_BACK );

	return state;
}

unsigned char is_pforward( void )
{
	unsigned char state ;

	state  = bit_is_set( panelDT_PIN, PANEL_FORWARD );

	return state;
}

unsigned char is_phead( void )
{
	unsigned char state ;

	state  = bit_is_set( panelDT_PIN, PANEL_HEAD );

	return state;
}


unsigned char is_pvac( void )
{
	unsigned char state ;

	state  = bit_is_set( panelDT_PIN, PANEL_VACUUM );

	return state;
}

unsigned char is_pfast( void )
{
	unsigned char state ;

	state  = bit_is_set( panelDT_PIN, PANEL_FAST );

	return state;
}

unsigned char is_phome( void )
{
	unsigned char state ;

	state  = bit_is_set( panelDT_PIN, PANEL_HOME );

	return state;
}
