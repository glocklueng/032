
/*
  head_control.h - head control methods
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

#ifndef head_control_h
#define head_control_h 

#include <avr/io.h>

// length of time for ehad to transition state
#define HEAD_SETTLE_TIME		( 200 )

void head_init();
char head_down(int state);

unsigned char is_head_down( void );
unsigned char is_rotated( void );

#endif
