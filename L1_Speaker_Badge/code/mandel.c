#include "common.h"


#if USE_MANDEL

/*
			88b           d88                                    88              88  88                                            
			888b         d888                                    88              88  88                                     ,d     
			88`8b       d8'88                                    88              88  88                                     88     
			88 `8b     d8' 88  ,adPPYYba,  8b,dPPYba,    ,adPPYb,88   ,adPPYba,  88  88,dPPYba,   8b,dPPYba,   ,adPPYba,  MM88MMM  
			88  `8b   d8'  88  ""     `Y8  88P'   `"8a  a8"    `Y88  a8P_____88  88  88P'    "8a  88P'   "Y8  a8"     "8a   88     
			88   `8b d8'   88  ,adPPPPP88  88       88  8b       88  8PP"""""""  88  88       d8  88          8b       d8   88     
			88    `888'    88  88,    ,88  88       88  "8a,   ,d88  "8b,   ,aa  88  88b,   ,a8"  88          "8a,   ,a8"   88,    
			88     `8'     88  `"8bbdP"Y8  88       88   `"8bbdP"Y8   `"Ybbd8"'  88  8Y"Ybbd8"'   88           `"YbbdP"'    "Y888  
                                                                                                                       
*/                                                                                                                  

#define BAILOUT 			( 16 )
#define MAX_ITERATIONS		( 1000 ) 

static short mandelbrot(float x, float y) 
{
	float cr = y - 0.5f;
	float ci = x;
	float zi = 0.0f;
	float zr = 0.0f;

	int iters = 0;
	
	while( 1 ) {

		iters ++;

		float temp = zr * zi;
		float zr2  = zr * zr;
		float zi2  = zi * zi;

		zr = zr2 - zi2 + cr;
		zi = temp + temp + ci;

		if ( ( zi2 + zr2 ) > BAILOUT ) {
			return iters; 
		}

		if ( iters > MAX_ITERATIONS ) {
			return 0; 
		}	
	} 
}

void setup_mandel(void)
{
	ClearFrames();
}

void loop_mandel(void) 
{ 
	char x,y; 
	
	for (y = 0; y < PEGGY2_HEIGHT; y++) { 

		for (x = 0; x < PEGGY2_WIDTH; x++) {

			int i = mandelbrot(x/40.0f, y/40.0f);

			if ( i == 0 ) {
				buffer[0].SetPoint(x,y);
			}
		} 

		// Display the frame buffer as we draw each row
		buffer[0].RefreshAll(1);  
	}

	// Hold last frame
	buffer[0].RefreshAll(1500); 

	// done
	gCount = 0;

}

#endif

