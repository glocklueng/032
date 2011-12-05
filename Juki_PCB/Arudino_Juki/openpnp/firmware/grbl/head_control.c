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
	HEAD_PORT |= _BV( HEAD );

// setup head sensor
	HEADDT_DDR  &= ~(_BV( HEAD_DOWN_TEST ));
	HEADDT_PORT |= (_BV( HEAD_DOWN_TEST ));

// setup 90o sensor
	HEADDT_DDR  &= ~(_BV( D90 ));
	HEADDT_PORT |= (_BV( D90 ));
}

// todo: don't allow head down while not homed?
void head_down(int state)
{

 set_busy( TRUE );

  printPgmString(PSTR("dbg: head_down\n\r"));

// head down mode
  if( state == 0 ) {

	HEAD_PORT |= _BV( HEAD );
	
	// wait til head up.

  } else {

	HEAD_PORT &= ~(_BV( HEAD ));
  }

	//	 length of a delay for the head to move up and down
  _delay_ms( HEAD_SETTLE_TIME ) ;

  if(is_head_down() != state ) {

	  printPgmString(PSTR("Head down error\n\r"));
  }

 set_busy( FALSE );

}

unsigned char is_head_down( void )
{
	unsigned char state ;

	state  = bit_is_set( HEADDT_PIN, HEAD_DOWN_TEST )?1:0;

	return state;
}

unsigned char is_rotated( void )
{
	unsigned char state ;

	state  = bit_is_set( HEADDT_PIN, D90 )?0:1;

	return state;
}
