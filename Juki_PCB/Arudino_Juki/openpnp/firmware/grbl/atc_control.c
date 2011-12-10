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
#include "atc_control.h"
#include "head_control.h"
#include "vacuum_control.h"
#include "config.h"
#include "stepper.h"

void atc_init()
{
	// set the toolchangers as outputs
	STEPPING_DDR |= _BV( SPARE1 ); // spare 1 is on a different port

	SPARE_DDR |= _BV( SPARE2 );
	SPARE_DDR |= _BV( SPARE3 );
	SPARE_DDR |= _BV( SPARE4 );
	SPARE_DDR |= _BV( SPARE5 );
	SPARE_DDR |= _BV( SPARE6 );

// initialise PORT as well

}


// check to see if the head has a tool in it
unsigned char check_for_tool( void ) 
{	
	//move head up
	if( is_head_down() ) {
		head_down( FALSE) ;
	}

	goto_vacpad();
	
	head_down(TRUE);
	
	vacuum(TRUE);

	if( vacuum_state() == TRUE )  {
		head_down( FALSE) ;
		return TRUE;
	} else {
		head_down( FALSE) ;
		return FALSE; 
	}
}

// has no tool to start of with
static unsigned char toolId = 255;

// this should to the whole procedure

// these two are similatr enough they could be one function
char PickupTool( unsigned char tool )
{
	// head down, won't return til its done
	if( head_down( 1 ) == 0 ) {
		// Head failed to go down.
		return GCSTATUS_FAILED_COMMAND;
	}

	// bring up atc, inbuilt delay, marked as busy so gantry can't move, atc doesn't know if its up or down
	atc_change( tool ) ;

	// head back up
	head_down( 0 );

	_delay_us(100);

	// tool changer down, should no longer be marked as busy, head can move.
	atc_change( 0 );
	
	return GCSTATUS_OK;

}

char DropTool( unsigned char tool )
{
	// head down, won't return til its done
	if( head_down( 1 ) == 0 ) {
		// Head failed to go down.
		return GCSTATUS_FAILED_COMMAND;
	}

	// bring up atc, inbuilt delay, marked as busy so gantry can't move, atc doesn't know if its up or down
	atc_fire( tool ) ;

	// tool changer down, should no longer be marked as busy, head can move.
	atc_fire( 0 );

	// head back up
	head_down( 0 );
	
	return GCSTATUS_OK;

}

/* 
If current tool  = requested tool , return 

  Move to vacuum pad
  Switch on vacuum
  Head down
  Wait
  Test vacuum 
  Head up
  If vacuum on, put tool away first into toolId hole

If Putting away
	Move to toolId tool holder
	pause
	Head down
	ATC up
	Head Up
	Atc Down

If Picking up
	Move to requested tool holder
	pause
	Head down
	ATC up
	Atc Down
	Head Up

Test to seee if has tool
	Move to vacuum table

	Failed ?

*/

unsigned char atc_change(int tool)
{
	// do we have that tool already ?
	if( tool == toolId ) {
		return GCSTATUS_OK;
	}
	
	if( run_vacuum_test() == 0 ) {
		PickupTool(tool);
	} else { 
		DropTool(tool);
	}

	return GCSTATUS_OK;
}



// tool changer
void atc_fire(int tool)
{
#ifdef VERBOSE_DEBUG
	printPgmString(PSTR("atc change\n\r"));
#endif

	set_busy( TRUE ) ;

	// turn off toolchanger
	if( tool == 0 ) {

#ifdef VERBOSE_DEBUG
		printPgmString(PSTR("ATCs off\n\r"));
#endif
		// set air off
		SET_BIT(SPARE_PORT, _BV( SPARE2 ) );

		//	 clear state
		SET_BIT(SPARE_PORT, _BV( SPARE3 ) );
		SET_BIT(SPARE_PORT, _BV( SPARE4 ) );
		SET_BIT(SPARE_PORT, _BV( SPARE5 ) );

		// head can move again
		set_busy( FALSE ) ;

		return;
	}
  
	// air off and down
	SET_BIT(SPARE_PORT, _BV( SPARE2 ) );

/*
 spare2 = 1 air on/off
 
 spare 3,4,5 
 345
 000 = tool 1
 001 = tool 2
 010 = tool 3
 011 = tool 4 
 100 = tool 5
 101 = tool 6
 */
 	switch( tool ) {
  	
		// select tool M13
		case 1:
			CLEAR_BIT(SPARE_PORT, _BV( SPARE3 ) );
			CLEAR_BIT(SPARE_PORT, _BV( SPARE4 ) );
			CLEAR_BIT(SPARE_PORT, _BV( SPARE5 ) );

			//on
			CLEAR_BIT(SPARE_PORT, _BV( SPARE2 ) );

			break;
		case 2: // M14
		
			CLEAR_BIT(SPARE_PORT, _BV( SPARE3 ) );
			CLEAR_BIT(SPARE_PORT, _BV( SPARE4 ) );
			SET_BIT  (SPARE_PORT, _BV( SPARE5 ) );
		
			//on
			CLEAR_BIT(SPARE_PORT, _BV( SPARE2 ) );


			break;
		case 3: // M15

			CLEAR_BIT(SPARE_PORT, _BV( SPARE3 ) );
			SET_BIT(SPARE_PORT, _BV( SPARE4 ) );
			CLEAR_BIT  (SPARE_PORT, _BV( SPARE5 ) );
		
			//on
			CLEAR_BIT(SPARE_PORT, _BV( SPARE2 ) );

			break;
		case 4: // M16

		
			CLEAR_BIT(SPARE_PORT, _BV( SPARE3 ) );
			SET_BIT(SPARE_PORT, _BV( SPARE4 ) );
			SET_BIT  (SPARE_PORT, _BV( SPARE5 ) );
		
			//on
			CLEAR_BIT(SPARE_PORT, _BV( SPARE2 ) );
			break;
		case 5: // M17
		
			SET_BIT   (SPARE_PORT, _BV( SPARE3 ) );
			CLEAR_BIT (SPARE_PORT, _BV( SPARE4 ) );
			CLEAR_BIT (SPARE_PORT, _BV( SPARE5 ) );
		
			break;
		case 6: // M18
		
			SET_BIT  (SPARE_PORT, _BV( SPARE3 ) );
			CLEAR_BIT(SPARE_PORT, _BV( SPARE4 ) );
			SET_BIT  (SPARE_PORT, _BV( SPARE5 ) );
		
			//on
			CLEAR_BIT(SPARE_PORT, _BV( SPARE2 ) );

			break;
		}


	// switch ATC on

	CLEAR_BIT(SPARE_PORT, _BV( SPARE2 ) );

	_delay_ms( 100 );


// don't turn off busy flag

}

unsigned char has_tool( void )
{
	return 0;
}
