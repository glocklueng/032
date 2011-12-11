/*
  main.c - An embedded CNC Controller with rs274/ngc (g-code) support
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
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "planner.h"
#include "stepper.h"
#include "spindle_control.h"
#include "coolant_control.h"
#include "motion_control.h"
#include "gcode.h"
#include "serial_protocol.h"
#include "head_control.h"
#include "atc_control.h"
#include "vacuum_control.h"
#include "panel_control.h"
#include "settings.h"
#include "wiring_serial.h"

void limits_init(void);

// #ifndef __AVR_ATmega328P__
// #  error "As of version 0.6 Grbl only supports atmega328p. If you want to run Grbl on an 168 check out 0.51 ('git co v0_51')"
// #endif


// stepper interrupt ack to host
extern char ackHost;

int main(void)
{
  sp_init();        
  settings_init();  
  plan_init();      
  st_init();        
  limits_init();
  spindle_init();   
  coolant_init();
  atc_init();
  head_init();
  vacuum_init();
  panel_init();
  gc_init();
 
  
  // Once everything is initialized, send the standard "ok" to let clients
  // know it's okay to go ahead
  printPgmString(PSTR("ok\n\r"));

// test inputs	
	while(0){
		unsigned char ch = 0;

	      	printPgmString(PSTR("head down = "));
			printInteger( is_head_down() );
	      	printPgmString(PSTR(" "));

	      	printPgmString(PSTR("rotated = "));
			printInteger( is_rotated() );
	      	printPgmString(PSTR(" "));

ch = is_pleft(  );
	      	printPgmString(PSTR("is_pleft = "));
			printInteger( ch );
	      	printPgmString(PSTR("\t"));
ch = is_pright(  );
	      	printPgmString(PSTR("is_pright = "));
			printInteger( ch );
	      	printPgmString(PSTR("\t"));
ch = is_pback(  );
	      	printPgmString(PSTR("is_pback = "));
			printInteger( ch );
	      	printPgmString(PSTR("\t"));
ch = is_pforward(  );
	      	printPgmString(PSTR("is_pforward = "));
			printInteger( ch );
	      	printPgmString(PSTR("\t"));
ch = is_phead(  );
	      	printPgmString(PSTR("is_phead = "));
			printInteger( ch );
	      	printPgmString(PSTR("\t"));
ch = is_pvac(  );
	      	printPgmString(PSTR("is_pvac = "));
			printInteger( ch );
	      	printPgmString(PSTR("\t"));
ch = is_pfast(  );
	      	printPgmString(PSTR("is_pfast = "));
			printInteger( ch );
	      	printPgmString(PSTR("\t"));
ch = is_pteach(  );
	      	printPgmString(PSTR("is_pteach = "));
			printInteger( ch );
	      	printPgmString(PSTR("\t"));
ch = is_phome(  );
	      	printPgmString(PSTR("is_phome = "));
			printInteger( ch );
	      	printPgmString(PSTR("\r\n"));


	}

  for(;;){
    sleep_mode(); // Wait for it ...
	
	if( ackHost != 0 ) {
		serialWrite( ackHost );
		ackHost = 0;
	}

    sp_process(); // ... process the serial protocol

	process_panel(); // check the panel
  }
  return 0;   /* never reached */
}
