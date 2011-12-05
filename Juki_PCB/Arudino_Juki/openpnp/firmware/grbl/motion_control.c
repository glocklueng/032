/*
  motion_control.c - high level interface for issuing motion commands
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


#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include <avr/io.h>

#include "settings.h"
#include "motion_control.h"
#include "nuts_bolts.h"
#include "stepper.h"
#include "planner.h"
#include "wiring_serial.h"


void mc_dwell(uint32_t milliseconds) 
{
  st_synchronize();
  _delay_ms(milliseconds);
}

void mc_go_home()
{
  st_go_home();
}
