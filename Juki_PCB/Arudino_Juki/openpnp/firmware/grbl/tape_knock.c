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
#include "tape_knock.h"
#include "config.h"
#include "settings.h"


void tape_init(void)
{
// set the tape knock as an output
	TAPE_DDR |= _BV( TAPE_KNOCK );
}

// tape 
void tape_knock( void )
{
#ifdef VERBOSE_DEBUG
	printPgmString(PSTR("tape knock\n\r"));
#endif

	if( gHomed == FALSE ) {
   		return ;
   	}


	while( head_moving() );

	 set_busy( TRUE );

	TAPE_PORT &= ~(_BV( TAPE_KNOCK ));

	_delay_ms( TAPE_KNOCK_SETTLE );

	TAPE_PORT |= _BV( TAPE_KNOCK );

	_delay_ms( TAPE_KNOCK_SETTLE );

	 set_busy( FALSE );
}
