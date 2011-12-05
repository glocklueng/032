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
#include "config.h"
#include "settings.h"

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

void goto_vacpad( void ) 
{
	if( is_head_down() ) {
		head_down( FALSE) ;
	}

	// move head to vacuum pad
	//gotoxy(12308,5002);
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



// changes tool , 0 put back, 1-6 picks up
void atc_change(int tool)
{
#ifdef VERBOSE_DEBUG
	printPgmString(PSTR("atc change\n\r"));
#endif

	set_busy( TRUE ) ;

	// make sure head is up before moving
	if( is_head_down() ) {
#ifdef VERBOSE_DEBUG
		printPgmString(PSTR("TOOL headup\n\r"));
#endif
		head_down( FALSE ) ;
	}

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
