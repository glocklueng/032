
/*
  panel_control.h - panel control methods
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

#ifndef panel_control_h
#define panel_control_h 

#include <avr/io.h>

void panel_init();

unsigned char is_rotated( void );
unsigned char is_pleft( void );
unsigned char is_pright( void );
unsigned char is_pback( void );
unsigned char is_pforward( void );
unsigned char is_phead( void );
unsigned char is_pvac( void );
unsigned char is_pfast( void );
unsigned char is_phome( void );
unsigned char is_pteach( void );

#endif
