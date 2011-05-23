

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


Peggy2 buffer[FRAMES_MAX];   // The drawing frames supporting greyscale



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

void SPI_TX(char cData)
{
	//Start Transmission
	SPDR = cData;
	
	//Wait for transmission complete:
	while (!(SPSR & _BV(SPIF))) ;
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

static unsigned char routineIndex = 0;


/* 

 Add a #define to common.h, create a .c/cpp file , include common.h , add a setup_ and a loop_ function, insert here.
 change gCount in the setup function if you want to run longer/shorter

*/

const pFunction demoRoutines[]  = {

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

	NULL,
	NULL,
};

unsigned int gCount = 250;

int main(void)
{
	unsigned char temp=0;
	unsigned char gameState = 0; 
 
	PORTD = 0U;			//All B Input
	DDRD = 255U;		// All D Output
	
	PORTB = 1;		// Pull up on ("OFF/SELECT" button)
	PORTC = 255U;	// Pull-ups on C

	DDRB = 254U;  // B0 is an input ("OFF/SELECT" button)
	DDRC = 0;		//All inputs

	////SET MOSI, SCK Output, all other SPI as input:
	DDRB = ( 1 << 3 ) | (1 << 5) | (1 << 2) | (1 << 1);

	//ENABLE SPI, MASTER, CLOCK RATE fck/4:		//TEMPORARY:: 1/128
	SPCR = (1 << SPE) | ( 1 << MSTR );//| ( 1 << SPR0 );//| ( 1 << SPR1 ) | ( 1 << CPOL ); 

	// Cleard LED drivers
	SPI_TX(0);
	SPI_TX(0);
	SPI_TX(0);

	PORTB |= _BV(1);		//Latch Pulse 
	PORTB &= ~( _BV(1));


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

			// number of frames, we should do this in a timer int, since some routines take more time per frame than others.
			gCount -- ;

		
			// call the drawing routine
			(*demoRoutines[ (gameState << 1) + 1 ])();

			if ( gCount == 0 ){

				// next state
				gameState ++ ;

				// wrap
				if ( demoRoutines[gameState <<1 ] == NULL  ) {
					gameState = 0;
				}
			}
		}
	}
}

