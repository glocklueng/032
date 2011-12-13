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
#include "head_control.h"
#include "stepper.h"

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


char pickup_part ( void ) 
{
	if( is_head_down() ){
		return GCSTATUS_FAILED_COMMAND;
	}

#if 0
	 Sleep( 100 );

	 // This will stall the machine till it can knock
	 WriteSerial("M21\r\n");
	 Sleep( 100 );

	 _RPT0(_CRT_WARN,"Picking up part\n");
	 // head down
	 WriteSerial("M10\r\n");
	 //wait
	 Sleep( 500 );
	 // vacuum on
	 WriteSerial("M19\r\n");
	 //wait
	 Sleep( 500 );
	 // head up
	 WriteSerial("M11\r\n");
	 //wait
	 Sleep( 500 );

#endif

	// advance part, waits til head has done
	tape_knock();

	// vacuum on
	vacuum ( 1 );

	// push head down
	head_down( 1 );

	_delay_ms( 500 );

	// head up
	head_down ( 0 ) ;

	return GCSTATUS_OK;

}

char putdown_part ( void )
{
	if( is_head_down() ){
		return GCSTATUS_FAILED_COMMAND;
	}

#if 0

	 // head down/air off/up
	 _RPT0(_CRT_WARN,"dropping off part\n");
	 // head down
	 WriteSerial("M10\r\n");
	 //wait
	 Sleep( 800 );
	 // vacuum off
	 WriteSerial("M20\r\n");
	 //wait
	 Sleep( 500 );
	 // head up
	 WriteSerial("M11\r\n");


#endif

	// wait til head stops
	while( head_moving() );

	_delay_ms( 100 );

	// push head down
	head_down( 1 );

	_delay_ms( 250 );

	// vacuum off
	vacuum ( 0 );

	// head up
	head_down ( 0 ) ;

	return GCSTATUS_OK;

}

