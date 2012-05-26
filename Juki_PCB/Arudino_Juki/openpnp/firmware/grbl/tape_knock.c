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
#include "vacuum_control.h"
#include "stepper.h"

#include "config.h"
#include "settings.h"


void tape_init(void)
{
// set the tape knock as an output
	TAPE_DDR |= _BV( TAPE_KNOCK );
}

// tape 
// some of the feeders are really slow
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

 /*
  * Picks up a part
  *
  * checks if head is down, 
  *    if so fails out
  * tape knock
  * vacuum on
  * head down
  * head up
  * check the vacuum is blocked
  *    if not, try 4 times, delay more each time , after 4 fail out, vacuum off, return error code
  *
  */


char pickup_part ( void ) 
{
	unsigned char failedCounter;


	// wait til head stops
	while( head_moving() );

	// maybe?
	while( get_busy() );


	// count number of pickup fails
	failedCounter = 0 ;


	if( is_head_down() ){
		return GCSTATUS_FAILED_COMMAND;
	}

	set_busy( TRUE ) ;

	//for(;;)  (oops!)
	{
// advance part, waits til head has done
// trying tape_knock after pickup		tape_knock();

		// vacuum on
		vacuum ( 1 );

		// push head down
		head_down( 1 );

		//settle (probably doesn't need this)
		_delay_ms( 150 );

		// head up
		head_down ( 0 ) ;

// advance part, waits til head has done
 		tape_knock();

// disable
#if 0
		// check vacuum line...
		if( vacuum_state() == 0 ) {

			// vacuum off
			vacuum ( 0 );
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("pickup_part:  part wasn't picked up!\r\n"));
#endif
		
			// failed to pickup a part
			failedCounter ++ ;

			if( failedCounter == 4 ) {


				set_busy( FALSE ) ;

				return GCSTATUS_FAILED_COMMAND;

			}

			// delay 500ms ( based on counter ? )  let the air compressor catch up, as that may be the problem
			_delay_ms( 500 * failedCounter );

		}else
			break;

#endif

	}

	_delay_ms( 10 );

	set_busy( FALSE ) ;

	return GCSTATUS_OK;

}
 /*
  * Places a part
  *
  * wait for head to stop
  * check the vacuum is blocked
  *    if not, fail out, vacuum off return error code
  *
  * head goes down
  * vacuum goes off
  * head goes up
  *
  *
  */

char putdown_part ( void )
{
	if( is_head_down() ){
		return GCSTATUS_FAILED_COMMAND;
	}

	// wait til head stops
	while( head_moving() );

	// maybe?
	while( get_busy() );

	set_busy( TRUE ) ;

	// settle time
	_delay_ms( 80 );

// disable while debugging air pressure
#if 0

	// check vacuum here
	if( vacuum_state() == 0 ) {
		// dropped the part !
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("putdown_part:  part was lost in transit!\r\n"));
#endif
		
		// vacuum off
		vacuum ( 0 );

		// now we need the GCODE function to set the location of the feeder, so we can retry the pickup

		set_busy( FALSE) ;

		return GCSTATUS_FAILED_COMMAND;	
	}

#endif

	// push head down
	head_down( 1 );

	// settle after head down ( probably doesn't need it  )
	_delay_ms( 150 );

	// vacuum off
	vacuum ( 0 );

	// head up
	head_down ( 0 ) ;

	_delay_ms( 80 );

	set_busy( FALSE) ;

	return GCSTATUS_OK;

}

