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
#include "head_control.h"
#include "config.h"
#include "settings.h"


void head_init()
{
// set the head as an output
	HEAD_DDR |= _BV( HEAD );

// setup head sensor
	HEADDT_DDR  &= ~(_BV( HEAD_DOWN_TEST ));
	HEADDT_PORT |= (_BV( HEAD_DOWN_TEST ));

// setup 90o sensor
	HEADDT_DDR  &= ~(_BV( D90 ));
	HEADDT_PORT |= (_BV( D90 ));
}

void head_down(int state)
{
  printPgmString(PSTR("Head down\n\r"));

// head down mode
  if( state == 1 ) {

	HEAD_PORT |= _BV( HEAD );
  } else {

	HEAD_PORT &= ~(_BV( HEAD ));
  }
}

unsigned char is_head_down( void )
{
	unsigned char state ;

	state  = bit_is_set( HEADDT_PIN, HEAD_DOWN_TEST );

	return state;
}

unsigned char is_rotated( void )
{
	unsigned char state ;

	state  = bit_is_set( HEADDT_PIN, D90 );

	return state;
}
