/*
  config.h - compile time configuration
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

#ifndef config_h
#define config_h

#define SET_BIT(p,m) ((p) |= (m)) 
#define CLEAR_BIT(p,m) ((p) &= ~(m)) 

#define BAUD_RATE (38400)

// Updated default pin-assignments from 0.6 onwards 
// (see bottom of file for a copy of the old config)

/*
  Mega328P Arduino Pin Mapping
  Digital 0     PD0 (RX)
  Digital 1     PD1 (TX)
  Digital 2     PD2
  Digital 3     PD3
  Digital 4     PD4
  Digital 5     PD5
  Digital 6     PD6
  Digital 7     PD7
  Digital 8     PB0
  Digital 9     PB1
  Digital 10    PB2
  Digital 11    PB3 (MOSI)
  Digital 12    PB4 (MISO)
  Digital 13    PB5 (SCK)
  
  Analog 0      PC0
  Analog 1      PC1
  Analog 2      PC2
  Analog 3      PC3
  Analog 4      PC4
*/

// TODO INVERT is supported for stepping and direction currently. Still need
// to implement the rest.

#define STEPPERS_ENABLE_DDR     DDRC
#define STEPPERS_ENABLE_PORT    PORTC
#define STEPPERS_ENABLE_BIT     4
// Set INVERT to 0 for high enable or 1 for low enable
#define STEPPERS_ENABLE_INVERT  1

#define HEAD_ROT				(6)


// For performance reasons it is neccesary to have all of the
// step outputs on one port. They cannot be split across
// multiple ports.
#define STEPPING_DDR        DDRF
#define STEPPING_PORT       PORTF
#define X_STEP_BIT          0
#define Y_STEP_BIT          2
#define Z_STEP_BIT          4
#define C_STEP_BIT          5

// Set INVERT to 0 for pulse high step or 1 for pulse low step
#define X_STEP_INVERT       0
#define Y_STEP_INVERT       0
#define Z_STEP_INVERT       0
#define C_STEP_INVERT       0

//DIRECTION_DDR   = 10001111
//DIRECTION_PORT  = 10001010


// For performance reasons it is neccesary to have all of the
// direction outputs on one port. They cannot be split across
// multiple ports.
#define DIRECTION_DDR       DDRF
#define DIRECTION_PORT      PORTF
#define X_DIRECTION_BIT     1
#define Y_DIRECTION_BIT     3
#define Z_DIRECTION_BIT     6
#define C_DIRECTION_BIT     7

// Set INVERT to 0 for pulse high direction or 1 for pulse low direction
#define X_DIRECTION_INVERT  1
#define Y_DIRECTION_INVERT  1
#define Z_DIRECTION_INVERT  0
#define C_DIRECTION_INVERT  0

// For performance reasons it is neccesary to have all of the
// limit inputs on one port. They cannot be split across
// multiple ports.
#define LIMIT_DDR           DDRD
#define LIMIT_PORT          PORTD
#define LIMIT_PIN	        PIND
#define X1_LIMIT_BIT         3
#define X2_LIMIT_BIT         2
#define Y1_LIMIT_BIT         0
#define Y2_LIMIT_BIT         1

#define TACSENSE			 7   // Taccuum sensor PD7

#define Z_LIMIT_BIT         2
#define C_LIMIT_BIT         3

// home switches
#define XHM_DDR		       DDRA
#define XHM_PIN      	   PINA

#define YHM_DDR		       DDRG
#define YHM_PIN      	   PING

#define X_HOME				4 //PA4
#define Y_HOME				0 //PG0

#define X_LIMIT_INVERT      0
#define Y_LIMIT_INVERT      0
#define Z_LIMIT_INVERT      0
#define C_LIMIT_INVERT      0

#define SPINDLE_ENABLE_DDR    DDRH
#define SPINDLE_ENABLE_PORT   PORTH
#define SPINDLE_ENABLE_BIT    6
#define SPINDLE_ENABLE_INVERT 0

#define SPINDLE_DIRECTION_DDR     DDRB
#define SPINDLE_DIRECTION_PORT    PORTB
#define SPINDLE_DIRECTION_PIN     PINB
#define SPINDLE_DIRECTION_BIT     7
#define SPINDLE_DIRECTION_INVERT  0

#define FLOOD_COOLANT_DDR       DDRB
#define FLOOD_COOLANT_PORT      PORTB
#define FLOOD_COOLANT_BIT       4
#define FLOOD_COOLAND_INVERT    0


// head down sensor
#define HEADDT_DDR				DDRC
#define HEADDT_PORT				PORTC
#define HEADDT_PIN				PINC

// these two are swapped in the schematic
#define	HEAD_DOWN_TEST			( 0 ) 			// PC1
#define	D90						( 1 ) 			// PC0

// control the head up/down
#define HEAD_DDR				DDRL
#define HEAD_PORT				PORTL
#define HEAD_PIN				PINL
#define HEAD					(5)			// PL5
#define VACUUM					(4)			// PL4

#define TAPE_DDR				DDRK
#define TAPE_PORT				PORTK
#define TAPE_PIN				PINK
#define TAPE_KNOCK				( 7 ) // PK7

// switch panel
#define PANEL_HOME				( 5 ) // PB5 (M.ORG)
#define PANEL_HEAD				( 4 ) // PC4 HEAD
#define PANEL_LEFT				( 5 ) // PE5 -X
#define PANEL_FORWARD			( 1 ) // PJ1 +Y
#define PANEL_BACK				( 4 ) // PE4 -Y
#define PANEL_RIGHT				( 5 ) // PG5 +X
#define PANEL_FAST				( 0 ) // PJ0 FAST
#define PANEL_VAC				( 0 ) // PH0 VAC
#define PANEL_TEACH				( 1 ) // PH1 TEACH

// atc
#define SPARE_DDR				DDRK
#define SPARE_PORT				PORTK
#define SPARE_PIN				PINK
			
#define SPARE1					( 7 ) // PF7
#define SPARE2					( 0 ) // PK0
#define SPARE3					( 1 ) // PK1
#define SPARE4					( 2 ) // PK2
#define SPARE5					( 3 ) // PK3
#define SPARE6					( 6 ) // PK6
#define SPARE8					( 5 ) // PK5

// The temporal resolution of the acceleration management subsystem. Higher number
// give smoother acceleration but may impact performance
#define ACCELERATION_TICKS_PER_SECOND 40L

#endif

// Pin-assignments from Grbl 0.5

// #define STEPPERS_ENABLE_DDR     DDRD
// #define STEPPERS_ENABLE_PORT    PORTD
// #define STEPPERS_ENABLE_BIT         2
// 
// #define STEPPING_DDR       DDRC
// #define STEPPING_PORT      PORTC 
// #define X_STEP_BIT           0
// #define Y_STEP_BIT           1
// #define Z_STEP_BIT           2
// #define X_DIRECTION_BIT            3
// #define Y_DIRECTION_BIT            4
// #define Z_DIRECTION_BIT            5
// 
// #define LIMIT_DDR      DDRD
// #define LIMIT_PORT     PORTD
// #define X_LIMIT_BIT          3
// #define Y_LIMIT_BIT          4
// #define Z_LIMIT_BIT          5
// 
// #define SPINDLE_ENABLE_DDR DDRD
// #define SPINDLE_ENABLE_PORT PORTD
// #define SPINDLE_ENABLE_BIT 6
// 
// #define SPINDLE_DIRECTION_DDR DDRD
// #define SPINDLE_DIRECTION_PORT PORTD
// #define SPINDLE_DIRECTION_BIT 7

