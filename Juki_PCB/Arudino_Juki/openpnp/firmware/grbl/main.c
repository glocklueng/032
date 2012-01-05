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
#include "usb.h"

#include "timers.h"

#include "config.h"

void limits_init(void);

// #ifndef __AVR_ATmega328P__
// #  error "As of version 0.6 Grbl only supports atmega328p. If you want to run Grbl on an 168 check out 0.51 ('git co v0_51')"
// #endif

// stepper interrupt ack to host
extern volatile char ackHost;

/**
 * Handles events from the USB layer.
 * @param device USB device that generated the event.
 * @param event USB event.
 */
static void pb_usbEventHandler(usb_device * device, usb_eventType event)
{
        //adb_usbConfiguration handle;

        switch (event)
        {
        case USB_CONNECT:

				serialWrite('U');
				serialWrite('C');

                // Check if the newly connected device is an ADB device, and initialise it if so.
                //if (adb_isAdbDevice(device, 0, &handle))
                //        adb_initUsb(device, &handle);

                break;

        case USB_DISCONNECT:

				serialWrite('U');
				serialWrite('D');

                // Check if the device that was disconnected is the ADB device we've been using.
                //if (device == adbDevice)
                {
                        // Close all open ADB connections.
                        //adb_closeAll();

                        // Signal that we're no longer connected by setting the global device handler to NULL;
//                        adbDevice = NULL;
  //                      connected = false;
                }

                break;

        default:
                // ignore
                break;
        }
}

int main(void)
{
	setup_timer();

	sp_init();        

#ifdef VERBOSE_DEBUG
  serialWrite('1');
#endif
  settings_init();  

#ifdef VERBOSE_DEBUG
  serialWrite('2');
#endif
  plan_init();      

#ifdef VERBOSE_DEBUG
  serialWrite('3');
#endif
  st_init();       


#ifdef VERBOSE_DEBUG
  serialWrite('4');
#endif
  // do limits init first because its dumb at the moment 
  limits_init();

#ifdef VERBOSE_DEBUG
  serialWrite('5');
#endif
	usb_init();

	usb_setEventHandler(pb_usbEventHandler);

#ifdef VERBOSE_DEBUG
  serialWrite('6');
#endif

  spindle_init();   
  coolant_init();
  atc_init();
  head_init();
  vacuum_init();
  panel_init();
  gc_init();
 
  // Once everything is initialized, send the standard "ok" to let clients
  // know it's okay to go ahead
  printPgmString(PSTR("ok\n"));

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


#ifdef VERBOSE_DEBUG
	if(ackHost){
		serialWrite(ackHost);
	}
#endif
	// sends the move ACK back to the host ( sleep mode isn't working on the mega )
	switch( ackHost ) {
		// X = move finished ok
		// L = hit a limit
		// H = not homed
		case 'L':
		case 'H':
			serialWrite( ackHost );
			break;
		case 'O':
		case 'X':
			if( head_moving() == 0 ) 
				printPgmString( PSTR("ok\n"));
			break;
		case 'E':
			printPgmString( PSTR("err\n"));
			break;
	}

	ackHost = 0;

    sp_process(); // ... process the serial protocol

	process_panel(); // check the panel

	usb_poll();

  }

  return 0;   /* never reached */
}
