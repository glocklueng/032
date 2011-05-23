/* Simple example code for Peggy 2.0, using the Peggy2 library.

Generate four (very simple) frame buffers and switch between them *fast* to make gray scale.


Copyright (c) 2008 Windell H Oskay.  All right reserved.

  This example is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this software; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "common.h"


#if USE_BOUNCER

//////////////////////////////////////////////////////////
/// bounce

#define g 0.1f   //gravitational acceleration (should be positive.)
#define ts 0.01f // TimeStep

void setup_bounce(void )                    // run once, when the sketch starts
{
	buffer[0].HardwareInit();  			 	// Call this once to init the hardware. 
                                        	// (Only needed once, even if you've got lots of frames.
	DDRB = 254;		//All outputs except B0, the "OFF/SELECT" button.
	PORTB |= 1;            // Turn on PortB Pin 0 pull up!

	// bounce runs quickly
	gCount = 2500;

}  // End void setup()  

void loop_bounce(void)                     // run over and over again
{    
	float Xnew, Ynew, VxNew, VyNew;
	static float xOld; // a few x & y position values
	static float yOld; // a few x & y position values
	
	static float VxOld; //  x & y velocity values 
	static float VyOld; //  x & y velocity values 
	static uint8_t NewBall = 101;
	static uint8_t  debounce = 0;


	uint8_t xp, yp;      
  
	if ( NewBall > 10 ) // IF ball has run out of energy, make a new ball!
  	{ 
    	buffer[0].Clear();
   		NewBall = 0;
    
		//Clear history: 
		xOld = -2;
		yOld = -2; 

		xOld = ((float)rand() / ((float)RAND_MAX + 1) * 25) ;   // Initial position: up to 24.
		yOld = (float) 25;

		//Random initial x-velocity:

		VxOld = ((float)rand() / ((float)RAND_MAX + 1) * 2*g) - g;   // Initial velocity: up to +/- g.

		//Zero initial y velocity:
		VyOld = 0; 
	} 

	/* Physics time!
	x' = x + v*t + at*t/2
	v' = v + a*t

	Horizontal (X) axis: No acceleration; a = 0.
	Vertical (Y) axis: a = -g
	*/

	// float Xnew,Ynew,VxNew,VyNew;

	Xnew = xOld + VxOld;
	Ynew = yOld + VyOld - 0.5*g*ts*ts;

	VyNew = VyOld - g*ts;
	VxNew = VxOld;

	// Bounce at walls

	if (Xnew < 0)
	{
		VxNew *= -1;
		Xnew = 0;
	}

	if (Xnew >= 17)
	{
		VxNew *= -1;
		Xnew = 17;
	}

	if (Ynew <= 0) {
		Ynew = 0;

	if (VyNew*VyNew < 0.1)  
		NewBall++;   

	if (VyNew < 0)
		VyNew *= -0.85;  

	}

	if (Ynew >= 18) {
		Ynew = 18; 

	if (VyNew > 0)  
		VyNew *= -0.85;  

	}

	if ( debounce && ((PINB & 1) == 0))
	{   
		if  (VyNew < 0)
			VyNew = -0.85*VyNew +  g*ts;
		else
			VyNew +=  g*ts;

		buffer[0].SetPoint(0, 0);
	}

	if ((PINB & 1) == 0)
	{ // "Off/select" button pressed
		debounce = 1;
	}
	else
		debounce = 0;


	//   *** Draw buffer matrix ***

	//First, clear old frame buffer
	//Note: Comment out the following line to leave trails after the ball!
	buffer[0].Clear();

	//Next, figure out which point we're going to draw. 

	xp =      (uint8_t) round(Xnew);
	yp = 18 - (uint8_t) round(Ynew);

	// Write the point to the buffer
	buffer[0].SetPoint(xp, yp);

	// Display the frame buffer
	buffer[0].RefreshAll(4); //Draw frame buffer some number of times

	//Age variables for the next iteration
	VxOld = VxNew;
	VyOld = VyNew;

	xOld = Xnew;
	yOld = Ynew; 

}

// end bounce

// start star2 


#define NUMBER_OF_STARS 	( 10 )
#define SCREEN_WIDTH 		( 18 )
#define SCREEN_HEIGHT 		( 18 ) 

/*star struct*/
typedef struct 
{
  float xpos, ypos;
  short zpos, speed;
  unsigned char  color;
} STAR;

static STAR stars[NUMBER_OF_STARS];

void init_star(STAR* star, int i)
{
  /* randomly init stars, generate them around the center of the screen */
  
  star->xpos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  star->ypos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  
  star->xpos *= 3072.0; /*change viewpoint */
  star->ypos *= 3072.0;

  star->zpos =  i;
  star->speed =  2 + (int)(2.0 * (rand()/(RAND_MAX+1.0)));

  star->color = i >> 2; /*the closer to the viewer the brighter*/
}

static   unsigned short centerx, centery;

void star2_start( void ) 
{


 	centerx = SCREEN_WIDTH >> 1;
 	centery = SCREEN_HEIGHT >> 1;
}

void star2_loop(void)
{
	static int i, tempx, tempy;  
  /* time based demo loop */
    {
   

      /* clear screen */
      
		buffer[0].Clear();
      /* move and draw stars */
      
      for (i = 0; i < NUMBER_OF_STARS; i++)
	{
	  stars[i].zpos -= stars[i].speed;
	  
	  if (stars[i].zpos <= 0)
	    {
	      init_star(stars + i, i + 1);
	    }

	  /*compute 3D position*/
	  tempx = (stars[i].xpos / stars[i].zpos) + centerx;
	  tempy = (stars[i].ypos / stars[i].zpos) + centery;

	  if (tempx < 0 || tempx > SCREEN_WIDTH - 1 || tempy < 0 || tempy > SCREEN_HEIGHT - 1) /*check if a star leaves the screen*/
	    {
	      init_star(stars + i, i + 1);
	      continue;
	    }
	  
		// Write the point to the buffer
		buffer[0].SetPoint(tempx, tempy);
	}



	// Display the frame buffer
	buffer[0].RefreshAll(4); //Draw frame buffer some number of times
   
   }
  
}

// end

#endif
