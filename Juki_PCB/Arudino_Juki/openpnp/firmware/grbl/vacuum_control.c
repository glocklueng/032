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
	LIMIT_PORT |= (_BV( TACSENSE ) );

	// Vacuum on/off control
	HEAD_DDR  |= _BV( VACUUM );
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

	state  = bit_is_set( LIMIT_PIN, TACSENSE )?0:1;

	return state;
}


void vacuum_test( void )
{
	if( vacuum_state()  == 1 ) { 
		
		printPgmString(PSTR("y\r\n"));

	} else {

		printPgmString(PSTR("n\r\n"));

	}
}

/* This runs the whole test , vaccum on, goes to pad, head down, test vacuum, head up, returns state */
unsigned char run_vacuum_test( void )
{
// switch vacuum on, faster, wastes air
	vacuum( 1 );

	// move to pad
	gotoxy( VACUUM_PAD_X, VACUUM_PAD_Y ,-1,-1) ;

	// this only checks if the interrupt is enable right now, not really if the machine is moving, which should be the same thing.
	while( head_moving() ) ;

	// settle head
	_delay_us( HEAD_MOVE_SETTLE_TIME );

}

