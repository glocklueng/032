
/*
  vacuum_control.h - vacuum control methods
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

#ifndef vacuum_control_h
#define vacuum_control_h 

#include <avr/io.h>

// ms needed for air to transistion
#define AIR_SETTLE_TIME		( 50 )

#define	VACUUM_PAD_X		( 12.1142 )
#define	VACUUM_PAD_Y		( 4.92323 )

void vacuum_init();
void vacuum(int state);
unsigned char vacuum_state( void );
void vacuum_test( void );
unsigned char run_vacuum_test( void );
unsigned char goto_vacpad( void ) ;
unsigned char test_mode( void ) ;

#endif
