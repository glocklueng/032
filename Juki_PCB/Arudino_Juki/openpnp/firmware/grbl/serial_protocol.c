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

#define LINE_BUFFER_SIZE ( 50 )

static char line[LINE_BUFFER_SIZE];
static uint8_t char_counter;

static void status_message(int status_code)
{
	if( gHomed == FALSE ) {
    	printPgmString(PSTR("\r\nNot homed\n\r"));status_code = 99;
	}

  switch(status_code) {          
    case GCSTATUS_OK:
    	printPgmString(PSTR("ok\n\r")); 
		break;
    case GCSTATUS_BAD_NUMBER_FORMAT:
    	printPgmString(PSTR("error: Bad number format\n\r")); 
		break;
    case GCSTATUS_EXPECTED_COMMAND_LETTER:
    	printPgmString(PSTR("error: Expected command letter\n\r")); 
		break;
    case GCSTATUS_UNSUPPORTED_STATEMENT:
    	printPgmString(PSTR("error: Unsupported statement\n\r")); 
		break;
    case GCSTATUS_FLOATING_POINT_ERROR:
   		printPgmString(PSTR("error: Floating point error\n\r")); 
		break;
    default:
    	printPgmString(PSTR("error: "));
    	printInteger(status_code);
    	printPgmString(PSTR("\n\r"));
		break;
  }
}

void sp_init() 
{
  beginSerial(BAUD_RATE);  
  printPgmString(PSTR("\r\nOpenPnP/Grbl " GRBL_VERSION));
  printPgmString(PSTR("\r\n"));  
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
