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
#include "atc_control.h"
#include "head_control.h"
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
	if( gHomed == FALSE ) return;


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
		
		printPgmString(PSTR("yes\r\n"));
	} else {
		printPgmString(PSTR("no\r\n"));
	}
}

/* This runs the whole test , vaccum on, goes to pad, head down, test vacuum, head up, returns state */
unsigned char run_vacuum_test( void )
{
	if( gHomed == FALSE  ){ 
		return GCSTATUS_NOT_HOMED;
	}

// switch vacuum on, faster, wastes air
	vacuum( 1 );

	// move to pad
	gotoxy( VACUUM_PAD_X, VACUUM_PAD_Y ,-1,-1) ;

	// this only checks if the interrupt is enable right now, not really if the machine is moving, which should be the same thing.
	while( head_moving() ) ;

	// settle head
	_delay_us( HEAD_MOVE_SETTLE_TIME );

	return GCSTATUS_OK;

}

unsigned char goto_vacpad( void ) 
{
	unsigned char hasTool = FALSE;

	if( gHomed == FALSE ) {
		return GCSTATUS_NOT_HOMED;
	}
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad\r\n"));
#endif

	if( is_head_down() ) {
		head_down( FALSE )  ;
	}
	
	// what pickobear sends
	// G1X12.1142Y4.92323F100

	// move head to vacuum pad
	gotoxy( VACUUM_PAD_X, VACUUM_PAD_Y ,-1,-1) ;

	while( head_moving() );	

	_delay_us( HEAD_MOVE_SETTLE_TIME );

// vacuum down, wait, test vacuum

	vacuum( 1 );

   _delay_ms( AIR_SETTLE_TIME );

	if( head_down( 1 ) == 0 ) {

#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad:  head failed to go down\r\n"));
#endif

		vacuum ( 0 );
		return GCSTATUS_FAILED_COMMAND;	

	}

   _delay_ms( 1000 );

	if( vacuum_state() ) {

#ifdef VERBOSE_DEBUG
		printPgmString(PSTR("vacuum_test:1, dropping tool\r\n"));
#endif
		hasTool = TRUE; 

	} else { 

		hasTool = FALSE ;

#ifdef VERBOSE_DEBUG
		printPgmString(PSTR("vacuum_test:0 picking up tool\r\n"));
#endif
	}

// vacuum off
	vacuum( 0 );

// head up
	if( head_down( 0 ) == 0 ) {

#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad:  head failed to go back up\r\n"));
#endif

		return GCSTATUS_FAILED_COMMAND;
	}

	while( is_head_down() ) ;


// move to ATC tool 1
//	G1X12.8504Y4.6378F100

	gotoxy(ATC_1_X,ATC_1_Y,-1,-1);

	while( head_moving() );	

	_delay_us( HEAD_MOVE_SETTLE_TIME );

	_delay_ms( 400 );

//head down
	if( head_down( 1 ) == 0 ) {

#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad:  head failed to go down\r\n"));
#endif

		return GCSTATUS_FAILED_COMMAND;	

	}

	// wait til head is down
	while( is_head_down() == 0 ) ;
	
	_delay_ms( 1000 );


	// atc up
	atc_fire( 1 ) ;

	//wait 
	_delay_ms( 800 );

	if( hasTool == FALSE ) {

		// tool changer off
		atc_fire ( 0 );
		
		// head up
		if( head_down( 0 ) == 0 ) {

#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad:  head failed to go \r\n"));
#endif

			return GCSTATUS_FAILED_COMMAND;	
		}

	} else {
	
		_delay_ms( 500 );

	// head up
		if( head_down( 0 ) == 0 ) {

#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad:  head failed to up \r\n"));
#endif


			return GCSTATUS_FAILED_COMMAND;	
		}

	}

// wait til head is up
	while( is_head_down() == 1 ) ;

	_delay_ms( 1500 );

	// move head to vacuum pad
	gotoxy( VACUUM_PAD_X, VACUUM_PAD_Y ,-1,-1) ;

	while( head_moving() );	

	_delay_us( HEAD_MOVE_SETTLE_TIME );

// vacuum down, wait, test vacuum

	vacuum( 1 );

   _delay_ms( AIR_SETTLE_TIME );

	if( head_down( 1 ) == 0 ) {

#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad:  head failed to down\r\n"));
#endif

		vacuum ( 0 );
		return GCSTATUS_FAILED_COMMAND;	

	}

   _delay_ms( 1000 );


	// check vacuum
	if( vacuum_state() ) {

		// vacuum is blocked
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad:  detected blockage\r\n"));
#endif

		
		// were we picking up?
		if( hasTool == FALSE  ) { 
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("tool pickup ok\r\n"));
#endif
		} else {
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("tool pickup failed\r\n"));
#endif

// oops
		head_down(0);
		vacuum( 0 );

		return GCSTATUS_FAILED_COMMAND;
		}

	} else {

		// vacuum is free (no block on test pad)
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad:  no blockage\r\n"));
#endif

			// vacuuum =  0
			// drop or pick

		// were we dropping off?
		if( hasTool == TRUE ) { 
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("tool drop ok\r\n"));
#endif
		} else {
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("tool drop failed\r\n"));
#endif

			head_down(0);
			vacuum( 0 );

			return GCSTATUS_FAILED_COMMAND;
		}

	}	

// vacuum off
	vacuum( 0 );

// head up
	if( head_down( 0 ) == 0 ) {
#ifdef VERBOSE_DEBUG
			printPgmString(PSTR("goto_vacpad:  head failed to go up\r\n"));
#endif

		return GCSTATUS_FAILED_COMMAND;
	}

	while( is_head_down() ) ;
	
	return GCSTATUS_OK;

}


