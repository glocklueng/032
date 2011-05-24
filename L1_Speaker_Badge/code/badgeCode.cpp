/*
 * Keep Data section usage below 74% or so, otherwise it won't boot. 
 */

#include "common.h"

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


Peggy2 buffer[FRAMES_MAX]; 		 // The drawing frames, 0 is always display  1..FRAMES_MAX can be used as scratchpad
unsigned short gCount = 250;	// Number of frames to run
static char intensity[FRAMES_MAX];   // Stores LED intensity for greyscale

const PROGMEM unsigned char text1[] = "NULLSPACE LABS";

/*
                                                                                                                                    
	 ad88888ba   88                                                        88        ,ad8888ba,                         88              
	d8"     "8b  88                                                        88       d8"'    `"8b                        88              
	Y8,          88                                                        88      d8'                                  88              
	`Y8aaaaa,    88,dPPYba,   ,adPPYYba,  8b,dPPYba,   ,adPPYba,   ,adPPYb,88      88              ,adPPYba,    ,adPPYb,88   ,adPPYba,  
	  `"""""8b,  88P'    "8a  ""     `Y8  88P'   "Y8  a8P_____88  a8"    `Y88      88             a8"     "8a  a8"    `Y88  a8P_____88  
	        `8b  88       88  ,adPPPPP88  88          8PP"""""""  8b       88      Y8,            8b       d8  8b       88  8PP"""""""  
	Y8a     a8P  88       88  88,    ,88  88          "8b,   ,aa  "8a,   ,d88       Y8a.    .a8P  "8a,   ,a8"  "8a,   ,d88  "8b,   ,aa  
	 "Y88888P"   88       88  `"8bbdP"Y8  88           `"Ybbd8"'   `"8bbdP"Y8        `"Y8888Y"'    `"YbbdP"'    `"8bbdP"Y8   `"Ybbd8"'  
                                                                                                                                  
*/


unsigned int qrand (void)
{
   static unsigned long z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
   unsigned long b;
   b  = ((z1 << 6) ^ z1) >> 13;
   z1 = ((z1 & 4294967294U) << 18) ^ b;
   b  = ((z2 << 2) ^ z2) >> 27; 
   z2 = ((z2 & 4294967288U) << 2) ^ b;
   b  = ((z3 << 13) ^ z3) >> 21;
   z3 = ((z3 & 4294967280U) << 7) ^ b;
   b  = ((z4 << 3) ^ z4) >> 12;
   z4 = ((z4 & 4294967168U) << 13) ^ b;
   return (z1 ^ z2 ^ z3 ^ z4);
}

// random number generation
// http://www.dragonsgate.net/pipermail/icc-avr/2005-January/004853.html

static long seed = 1234;

#define M 	0x7FFFFFFF  // 2^31-1, the modulus used by the psuedo-random
            	          // number generator prng().
// a good random number generator; call with 1 <= x <= M-1

unsigned char prng(void)
{
  seed = (seed >> 16) + ((seed << 15) & M) - (seed >> 21) - ((seed << 10) & M);
  if (seed < 0) seed += M;
  return (unsigned char) seed;
}

// Timer2 overflow interrupt vector handler
ISR(TIMER2_OVF_vect, ISR_NOBLOCK) 
{
	uint8_t latency = TCNT2;
	TCNT2 = latency + DEFAULT_REFRESHRATE;
}

void sleep(void)
{
	// Everything as an output, floating or inputs will consume more mA
   DDRD = 255;
   DDRB = 255;
   DDRC = 255;

    
    // Set sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    // Enable sleep mode
    sleep_enable();

    // Disable ADC, must be done before calling 
    // power_all_disable() as to disable ADC, 
    // clock is required
    // Refer to datasheet page 45
    ADCSRA &= ~(1 << ADEN);

    // Disable all peripheral power
    power_all_disable();

	//disable brown out detection
	sleep_bod_disable();

    // Enter sleep mode
    sleep_mode();
}


// PORTB doesn't affect power 
// PORTC 30-40mA
// PORTD +10mA if C is 0xff

typedef void (*pFunction)(void);


/* 

 Add a #define to common.h, create a .c/cpp file , include common.h , add a setup_ and a loop_ function, insert here.
 change gCount in the setup function if you want to run longer/shorter

*/

const pFunction demoRoutines[]  = {

//	setup_keys,
//	loop_keys,


#if USE_SCROLLER
	setup_scroll,
	loop_scroll,
#endif

#if USE_SNAKE
	init_snake,
	snake_loop,
#endif

#if USE_MAZE
	loop_maze,
	setup_maze,
#endif

#if USE_LIFE2
	&setup_life2,
	&loop_life2,
#endif


#if USE_MANDEL
	setup_mandel,
	loop_mandel,
#endif


#if USE_STARFIELD1
	&setup_star,
	&loop_star,
#endif

#if USE_3DCUBE
	&setup_cube,
	&loop_cube,
#endif

#if USE_FADER
	&setup_fade,
	&loop_fade,
#endif

#if USE_LINE
	&setup_line,
	&loop_line,
#endif

#if USE_BLOCK
	&setup_block,
	&loop_block,
#endif


#if USE_STARFIELD2
	&setup_star2,
	&loop_star2,
#endif

#if USE_LIFE
	&setup_life,
	&loop_life,
#endif


#if USE_BOUNCER
	&setup_bounce,
	&loop_bounce,
#endif

#if USE_STARFIELD3
	&init_star3,
	&loop_star3,
#endif



//Terminate list
	NULL,
	NULL,
};

int main(void)
{
	unsigned char gameState = 0; 

 	buffer[0].HardwareInit();   // Call this once to init the hardware. 

  // Pre-calc tbe LED brightness levels to save clocks
  // The madness of the casting and rounding is necessary with pow() otherwise
  // it produces unpredictable integer values.
	for (int i=0; i<FRAMES_MAX; i++)
		intensity[i] = Round(pow (2.f, (float)i));


#if USE_BLOCK

/// Do a sweep forward and back on boot
	setup_block();

	buffer[0].RefreshAll(80); //Draw frame buffer 0
		buffer[1].RefreshAll(80); //Draw frame buffer 1
			buffer[2].RefreshAll(80); //Draw frame buffer 2
				buffer[3].RefreshAll(80); //Draw frame buffer 3
			buffer[2].RefreshAll(80); //Draw frame buffer 2
		buffer[1].RefreshAll(80); //Draw frame buffer 1
	buffer[0].RefreshAll(80); //Draw frame buffer 0
#endif

	gameState = 0;

	// for ever!
	while( 1 ) {


		// default run length
		gCount = 250;

		//call the setup routine
		// can change the number of times to run, via gCount
		(*demoRoutines[ gameState << 1 ])() ;


		// loop drawing routines

		while( gCount) {


			buffer[0].keyB =  (!(PINB & 0x01));
			
			// reset
			//buffer[0].keyA =  (!(PINB & 0x01));

			buffer[0].keyUp =  (!(PINC & 0x10));
			buffer[0].keyDown =  (!(PINC & 0x04)) ;
			buffer[0].keyLeft =  (!(PINC & 0x08));
			buffer[0].keyRight =  (!(PINC & 0x02)) ;


//			loop_keys();


			// number of frames, we should do this in a timer int, since some routines take more time per frame than others.
			gCount -- ;

			// call the drawing routine
			(*demoRoutines[ (gameState << 1) + 1 ])();

			// frame counter ran out
			if ( gCount == 0 ){

				// next state
				gameState ++ ;

				// wrap if end of list
				if ( demoRoutines[gameState <<1 ] == NULL  ) {
					gameState = 0;
				}
			}
		}
	}
}


void memset( unsigned char *dst,int data, int length) 
{
	int i;

	for( i = 0 ; i < length ; i++ ) {

		dst[i] = data;
	}
}


void ClearFrames(void)
{
	int i;

	for( i = 0 ; i < FRAMES_MAX ;i++ )  {
		buffer[i].Clear();
	}


}

#if 1


void setup_keys(void)

{
                                        // (Only needed once, even if you've got lots of frames.)
    buffer[0].Clear();

    DDRB = 0xFE;    // PORTB pin 0 is button 0, set it up as an input
    PORTB |= 0x01;  // PORTB pin 0 pull up
    DDRC = 0xE0;    // PORTC pins 0 - 4 set up as inputs corresponding to buttons 1 - 5
    PORTC |= 0x1F;  // PORTC pins 0 - 4 pull ups


	gCount= 9000;

}  // End void setup()  

void loop_keys(void )
{
  if (!(PINB & 0x01)) buffer[0].SetPoint(0,0);  // PORT B pin 0 is attached to button 0
  else buffer[0].ClearPoint(0,0);
  
  if (!(PINC & 0x01)) buffer[0].SetPoint(0,1);  // PORT C pin 0 is attached to button 1
  else buffer[0].ClearPoint(0,1);
  
  if (!(PINC & 0x02)) buffer[0].SetPoint(0,2);  // PORT C pin 1 is attached to button 2
  else buffer[0].ClearPoint(0,2);
  
  if (!(PINC & 0x04)) buffer[0].SetPoint(0,3);  // PORT C pin 2 is attached to button 3
  else buffer[0].ClearPoint(0,3);
  
  if (!(PINC & 0x08)) buffer[0].SetPoint(0,4);  // PORT C pin 3 is attached to button 4
  else buffer[0].ClearPoint(0,4);
  
  if (!(PINC & 0x10)) buffer[0].SetPoint(0,5);  // PORT C pin 4 is attached to button 5
  else buffer[0].ClearPoint(0,5);
  
 
}

#endif



// Redraws frame buffers to create greyscale 
void Redraw(void )
{
   for (int i=0; i<FRAMES_MAX; i++)
    buffer[i].RefreshAll(intensity[i]);
}
 
// This procedure implements the levels of greyscale by
// drawing the stars onto each frame accordingly, as per this
// chart
/*
        Greyscale
Frame   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
    0   -  x  -  x  -  x  -  x  -  x  -  x  -  x  -  x
    1   -  -  x  x  -  -  x  x  -  -  x  x  -  -  x  x
    2   -  -  -  -  x  x  x  x  -  -  -  -  x  x  x  x
    3   -  -  -  -  -  -  -  -  x  x  x  x  x  x  x  x
    
    and so on.  32 levels of greyscale uses 5 frames (0-4).
*/
void SetPointGrey (int x, int y, int grey)
{
	unsigned char s;

	s =   ( (grey) / 32.0f);
	
	if ( s== 0 ) s++;

	if (  s > 8 ) {
		s = 8;
	}

  for (int i=0; i<FRAMES_MAX; i++)
  {
    if ((grey & intensity[i]) == intensity[i]) {
     	buffer[i].SetPoint(x,y); 
	 }
  }
}
  


// Another quick utility method to round - this makes the stars
// display smoother than just truncating... if anything can be "smooth"
// on a 25x25 display. ;)
int Round(float x)
{
  return ((int)(x > 0.0 ? x + 0.5f : x- 0.5f));
}

