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

	// make sure head is up before moving
	if( is_head_down() ) {
		head_down( FALSE ) ;
	}

	// turn off toolchanger
	if( tool == 0 ) {
		SPARE_DDR |= _BV( SPARE2 );
		return;
	}
  
  	switch( tool ) {
  	
		// select tool A
		case 1:
			SPARE_DDR |= _BV( SPARE3 );
			SPARE_DDR |= _BV( SPARE4 );
			SPARE_DDR |= _BV( SPARE5 );

			SPARE_DDR |= _BV( SPARE2 );

			// sets low 
	//		SPARE_DDR &= ~(_BV( SPARE3 ));

			break;
  }
}

unsigned char has_tool( void )
{
	return 0;
}
