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
#include "vacuum_control.h"
#include "config.h"
#include "settings.h"

void vacuum_init(void)
{
	// setup vacuum sensor TAC
	LIMIT_DDR &= ~(_BV( TACSENSE ) );
	LIMIT_DDR |= (_BV( TACSENSE ));

	// Vacuum on/off control
	HEAD_DDR |= _BV( VACUUM );
	HEAD_PORT |= _BV( VACUUM );

}

// switches vacuum on and off
void vacuum(int state)
{
#ifdef VERBOSE_DEBUG
	printPgmString(PSTR("vacuum change\n\r"));
#endif

// vacuum  on / off
  if( state == 0 ) {

	HEAD_PORT |= _BV( VACUUM );

  } else {

	HEAD_PORT &= ~(_BV( VACUUM ));

  }

   // wait for air to get settled
   _delay_ms( AIR_SETTLE_TIME );

}

// is vacuum working ?
unsigned char vacuum_state( void )
{
	unsigned char state ;

	state  = bit_is_set( LIMIT_DDR, TACSENSE )?0:1;

	return state;
}


void do_vacuum_test( void )
{
	if( vacuum_state() ) { 
		
		printPgmString(PSTR("y\r\n"));

	} else {

		printPgmString(PSTR("n\r\n"));

	}

}

