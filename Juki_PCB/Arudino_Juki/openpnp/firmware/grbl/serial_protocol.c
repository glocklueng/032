/*
  serial_protocol.c - the serial protocol master control unit
  Part of Grbl

  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include <math.h>
#include <avr/pgmspace.h>

#include "motion_control.h"
#include "serial_protocol.h"
#include "gcode.h"
#include "wiring_serial.h"
#include "settings.h"
#include "config.h"
#include "nuts_bolts.h"
#include "vacuum_control.h"
#include "head_control.h"
#include "atc_control.h"
#include "stepper.h"




#define LINE_BUFFER_SIZE ( 50 )

static char line[LINE_BUFFER_SIZE];
static uint8_t char_counter;

// this needs to be fixed so it can reply correctly
static void status_message(int status_code)
{
	// Always override
	if( gHomed == FALSE ) {
		status_code = GCSTATUS_NOT_HOMED;
	}

	switch(status_code) {          
		case GCSTATUS_OK:
			printPgmString(PSTR("ok\n")); 
			break;
		case GCSTATUS_BAD_NUMBER_FORMAT:
			printPgmString(PSTR("error: Bad number format\n")); 
			// typos are ok
			return;
		case GCSTATUS_EXPECTED_COMMAND_LETTER:
			printPgmString(PSTR("error: Expected command letter\n")); 
			// typos are ok
			return;
		case GCSTATUS_UNSUPPORTED_STATEMENT:
			printPgmString(PSTR("error: Unsupported statement\n")); 
			// typos are ok
			return;

		case GCSTATUS_FLOATING_POINT_ERROR:
			printPgmString(PSTR("error: Floating point error\n")); 
			break;
		case GCSTATUS_FAILED_COMMAND:
			printPgmString(PSTR("error: Command Failed\n")); 
			break;
		case GCSTATUS_NOT_HOMED:
			printPgmString(PSTR("error: Not homed\n")); 
			break;
		case GCSTATUS_NO_ACK:
			return;
			break;
		default:
			printPgmString(PSTR("error: "));
			printInteger(status_code);
			printPgmString(PSTR("\n"));
			break;
	}


	// if didn't pass, stop everything , can only do home after this
	if( status_code != GCSTATUS_OK ) {

		vacuum(0);
		head_down(0);
		atc_fire(0);


		// do these two last, or the previous ones won't work
		gHomed = FALSE ;

		// stops all but home
		set_busy(TRUE);
	}
}

void sp_init() 
{
  beginSerial(BAUD_RATE);    
}

void sp_process(void)
{
  // force signed
  signed char c;
  while((c = serialRead()) != -1) 
  {
      if((char_counter > 0) && ((c == '\n') || (c == '\r'))) {  // Line is complete. Then execute!
      line[char_counter] = 0; // terminate string
      status_message(gc_execute_line(line));
      char_counter = 0; // reset line buffer index
    } else if (c <= ' ') { // Throw away whitespace and control characters
    } else if (c >= 'a' && c <= 'z') { // Uppercase lowercase
      line[char_counter++] = c-'a'+'A';
    } else {
      line[char_counter++] = c;
    }
  }
}
