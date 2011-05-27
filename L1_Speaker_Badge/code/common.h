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
#include <avr/pgmspace.h>

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

#ifndef RAND_MAX
#define	RAND_MAX			( 32767 )
#endif

#ifndef PI
#define PI					( 3.14159265f  ) 
#endif

#ifndef TWO_PI
#define TWO_PI 				( 6.283185307f )
#endif

#ifndef ABS
#define ABS(a)	   			(((a) < 0) ? -(a) : (a))
#endif


// number of framebuffers to define (min 4)
#define FRAMES_MAX 			( 4 )


// Badge dimensions ( Peggy2 is 25x25 ), using them as unsigend adds +100 bytes to the code size
#define PEGGY2_HEIGHT 		( 18 )
#define PEGGY2_WIDTH 		( 18 )

#define CENTERX 			( PEGGY2_WIDTH  >> 1 )
#define CENTERY  			( PEGGY2_HEIGHT >> 1 )

// get high/low nibble of a byte
#define	HIGH(x)				(( x&0xf0)>>4)
#define	LOW(x)				( x&0x0f)

// Set these 0/1 for each of the possible effects routines, memory can be a problem for all at once.

#define	USE_SCROLLER		( 1 ) // scrolling text
#define USE_STARFIELD1		( 1 ) // 3d starfield with intensity (very data hungry)
#define USE_3DCUBE			( 1 ) // peggy rotating cube
#define USE_FADER			( 1 ) // shows 4 fade leves
#define USE_LINE			( 0 ) // simple line test
#define USE_BLOCK			( 1 ) // shows 4 frame buffers
#define USE_STARFIELD2		( 0 ) // not done (2d)

#define USE_LIFE			( 0 ) // conways life. Big footprint, uses 3402 Program, and 243 Data
#define USE_LIFE2			( 1 ) // different implementation, much smaller than USE_LIFE, but still needs two (18*18) sram buffers. 678 Program, 6 Data


#define USE_BOUNCER			( 1 ) // bouncing ball
#define USE_STARFIELD3		( 0 ) // not really finished
#define	USE_SNAKE			( 1 ) // snake!

#define	USE_MANDEL			( 1 ) // Simple mandelbrot

#define USE_MAZE			( 0 ) // Maze Game

#define USE_FONT			( 1 ) // include font routines, needed for scroller etc

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

extern Peggy2 buffer[FRAMES_MAX];					// Contains framebuffer and associated variables
extern unsigned short gCount;						// Counter that when 0 is reached, moves to the next effect
extern unsigned char  scratchpad[100+(324*2)];		// Scratchpad memory ( not implemented )


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
                                                                                                     

unsigned int qrand (void);							// rand() replacement
unsigned char prng(void);

/* Ball bouncing example */
void setup_bounce( void );
void loop_bounce( void );

/* Conway's game of life */
void setup_life( void ) ;
void loop_life( void);

/* Star field */
// large c++ with intensity
void setup_star(void);
void loop_star(void);
//simple 3d
void init_star3(void);
void loop_star3(void);
//2d 
void setup_star2(void);
void loop_star2(void);

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

void setup_life2(void );
void loop_life2(void );

void init_snake(void);
void snake_loop(void);

void setup_keys(void );
void loop_keys(void );
void setup_mandel(void);
void loop_mandel(void);
void setup_maze(void);
void loop_maze(void);


/* general purpose */

/* uses progmem */
void Text8x6(short x,short y,const unsigned char *string);
void Text6x6(short x,short y,const unsigned char *string);

/* uses progmem */
unsigned short pstrlen(const unsigned char * str);

void DrawOutlineCircle( int xc,int yc, unsigned int radius  );
void memset( void *dst,int  data,int length) ;
void memcpy( unsigned char *dst,unsigned char*src, int length) ;
void ClearFrames(void);
void SetPointGrey (int x, int y, int grey);
void Redraw(void );
int Round(float x);
void banner( const unsigned char *text );

#ifdef __cplusplus
}
#endif

#endif
