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
}

// changes tool , 0 put back, 1-6 picks up
void atc_change(int tool)
{
  printPgmString(PSTR("atc change\n\r"));

}

unsigned char has_tool( void )
{
	return 0;
}
