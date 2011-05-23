#ifndef _COMMON_H_
#define _COMMON_H_		(1)

#ifdef __cplusplus
extern "C" {
#endif
                                                                                    
/*
	88        88                                   88                                     
	88        88                                   88                                     
	88        88                                   88                                     
	88aaaaaaaa88   ,adPPYba,  ,adPPYYba,   ,adPPYb,88   ,adPPYba,  8b,dPPYba,  ,adPPYba,  
	88""""""""88  a8P_____88  ""     `Y8  a8"    `Y88  a8P_____88  88P'   "Y8  I8[    ""  
	88        88  8PP"""""""  ,adPPPPP88  8b       88  8PP"""""""  88           `"Y8ba,   
	88        88  "8b,   ,aa  88,    ,88  "8a,   ,d88  "8b,   ,aa  88          aa    ]8I  
	88        88   `"Ybbd8"'  `"8bbdP"Y8   `"8bbdP"Y8   `"Ybbd8"'  88          `"YbbdP"'   
*/                                                                                      

#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdlib.h> 
#include <math.h> 
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include "Peggy2.h"

/*
                                                                            
	88888888ba,                   ad88  88                                      
	88      `"8b                 d8"    ""                                      
	88        `8b                88                                             
	88         88   ,adPPYba,  MM88MMM  88  8b,dPPYba,    ,adPPYba,  ,adPPYba,  
	88         88  a8P_____88    88     88  88P'   `"8a  a8P_____88  I8[    ""  
	88         8P  8PP"""""""    88     88  88       88  8PP"""""""   `"Y8ba,   
	88      .a8P   "8b,   ,aa    88     88  88       88  "8b,   ,aa  aa    ]8I  
	88888888Y"'     `"Ybbd8"'    88     88  88       88   `"Ybbd8"'  `"YbbdP"'  
                                                                            
                                                                           
*/

#define PI					( 3.14159265f  ) 
#define TWO_PI 				( 6.283185307f )

#define FRAMES_MAX 			( 5 )


// Badge dimensions ( Peggy2 is 25x25 )
#define PEGGY2_HEIGHT 		( 18 )
#define PEGGY2_WIDTH 		( 18 )

#define CENTERX 			( PEGGY2_WIDTH  >> 1 )
#define CENTERY  			( PEGGY2_HEIGHT >> 1 )


// Set these 0/1 for each of the possible effects routines, memory can be a problem for all at once.

#define USE_STARFIELD1		( 1 )
#define USE_3DCUBE			( 1 )
#define USE_FADER			( 1 )
#define USE_LINE			( 1 )
#define USE_BLOCK			( 1 )
#define USE_STARFIELD2		( 0 )
#define USE_LIFE			( 0 ) // big
#define USE_BOUNCER			( 1 )
#define USE_STARFIELD3		( 0 ) // not really finished
#define	USE_SCROLLER		( 1 )

#define USE_FONT			( 1 ) // include font routines

/*                                                                        
	  ,ad8888ba,   88               88                       88             
	 d8"'    `"8b  88               88                       88             
	d8'            88               88                       88             
	88             88   ,adPPYba,   88,dPPYba,   ,adPPYYba,  88  ,adPPYba,  
	88      88888  88  a8"     "8a  88P'    "8a  ""     `Y8  88  I8[    ""  
	Y8,        88  88  8b       d8  88       d8  ,adPPPPP88  88   `"Y8ba,   
	 Y8a.    .a88  88  "8a,   ,a8"  88b,   ,a8"  88,    ,88  88  aa    ]8I  
	  `"Y88888P"   88   `"YbbdP"'   8Y"Ybbd8"'   `"8bbdP"Y8  88  `"YbbdP"'  
                                                                
*/



/*
                                                                                                                               
	 ad88888ba   88                                                        88      88888888ba,                                     
	d8"     "8b  88                                                        88      88      `"8b                 ,d                 
	Y8,          88                                                        88      88        `8b                88                 
	`Y8aaaaa,    88,dPPYba,   ,adPPYYba,  8b,dPPYba,   ,adPPYba,   ,adPPYb,88      88         88  ,adPPYYba,  MM88MMM  ,adPPYYba,  
	  `"""""8b,  88P'    "8a  ""     `Y8  88P'   "Y8  a8P_____88  a8"    `Y88      88         88  ""     `Y8    88     ""     `Y8  
	        `8b  88       88  ,adPPPPP88  88          8PP"""""""  8b       88      88         8P  ,adPPPPP88    88     ,adPPPPP88  
	Y8a     a8P  88       88  88,    ,88  88          "8b,   ,aa  "8a,   ,d88      88      .a8P   88,    ,88    88,    88,    ,88  
	 "Y88888P"   88       88  `"8bbdP"Y8  88           `"Ybbd8"'   `"8bbdP"Y8      88888888Y"'    `"8bbdP"Y8    "Y888  `"8bbdP"Y8  
                                                                                                                               
                                                                                                                               
*/

extern Peggy2 buffer[FRAMES_MAX];
extern unsigned int gCount;


/*                                                                                                     
		88888888888                                                 88                                       
		88                                                   ,d     ""                                       
		88                                                   88                                              
		88aaaaa      88       88  8b,dPPYba,    ,adPPYba,  MM88MMM  88   ,adPPYba,   8b,dPPYba,   ,adPPYba,  
		88"""""      88       88  88P'   `"8a  a8"     ""    88     88  a8"     "8a  88P'   `"8a  I8[    ""  
		88           88       88  88       88  8b            88     88  8b       d8  88       88   `"Y8ba,   
		88           "8a,   ,a88  88       88  "8a,   ,aa    88,    88  "8a,   ,a8"  88       88  aa    ]8I  
		88            `"YbbdP'Y8  88       88   `"Ybbd8"'    "Y888  88   `"YbbdP"'   88       88  `"YbbdP"'  
                                                                                                 
*/
                                                                                                     

unsigned int qrand (void);

/* Ball bouncing example */
void setup_bounce( void );
void loop_bounce( void );

/* Conway's game of life */
void setup_life( void ) ;
void loop_life( void);

/* Star field */
void setup_star(void);
void loop_star(void);
void init_star3(void);
void loop_star3(void);

/* 3d cubes */
void setup_cube(void);
void loop_cube(void);

/* 16 greys */
void setup_fade(void);
void loop_fade(void);

void setup_block(void);
void loop_block(void);

void setup_line(void);
void loop_line(void);

void setup_scroll(void);
void loop_scroll(void);

/* general purpose */
void SPI_TX (char cData);
void Text8x6(short x,short y,const char *string);


#ifdef __cplusplus
}
#endif

#endif
