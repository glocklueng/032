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
#include "stepper.h"


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


char head_down(int state)
{

	// this one is interesting, it can't rely on the busy flag so lets use gHomed instead
	if( gHomed == FALSE  ){ 
		return 0;
	}

	// wait til head stops
	while( head_moving() );

#ifdef VERBOSE_DEBUG
	printPgmString(PSTR("dbg: head_down\n\r"));
#endif

	set_busy( TRUE );

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
	  
	  return 0;
  }

 	set_busy( FALSE );

	return 1;
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

	state  = bit_is_set( HEADDT_PIN, D90 )?1:0;

	return state;
}
