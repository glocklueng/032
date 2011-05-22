// common

extern "C" {

	#include <avr/io.h> 
	#include <avr/interrupt.h> 
	#include <stdlib.h> 
	#include <math.h> 
	#include <avr/eeprom.h>
	#include <avr/sleep.h>
	#include <avr/power.h>


	#define PI		3.14159265

	#define TWO_PI 6.283185307
}



void SPI_TX (char cData)
{
	//Start Transmission
	SPDR = cData;
	
	//Wait for transmission complete:
	while (!(SPSR & _BV(SPIF))) ;
}



#define CELLS_X 18		
#define CELLS_Y 18 


/////////////////////////////////////////////////////////////////////////

unsigned long current_generation[CELLS_X];
unsigned long old_generation[CELLS_X];

volatile unsigned long d[25];

unsigned char InputReg,InputRegOld;

unsigned char EditMode = 0;

unsigned int refreshNum = 10;

signed char xCursor = 12;
signed char yCursor = 12;
  
 
/////////////////////////////////////////////////////////////////////////

unsigned char get_cell(unsigned long from[],  signed char x, signed char y)
{
	if(x < 0) 
	   x = 24;
	   
	if(x > 24) 
	   x = 0;
	
	if(y < 0) 
	   y = 24;
	   
	if(y > 24) 
	   y = 0;
	 
	return ((from[x] & ( (unsigned long) 1 << y)) > 0);
}

/////////////////////////////////////////////////////////////////////////

unsigned int get_total(unsigned long from[])
{
	unsigned int total = 0;
	unsigned char x,y;
	for(x=0; x < CELLS_X; x++)
	{
		for(y=0; y < CELLS_Y; y++)
		{
			if(get_cell(from,x,y)) total++;
		}
	}
	return total;
}
 
 /////////////////////////////////////////////////////////////////////////

 
static inline void set_cell(unsigned long to[], signed char x, signed char y, unsigned char value)
{
	if(value) 
		to[x] |= (unsigned long) 1 <<  y;
	else 
		to[x] &= ~( (unsigned long) 1 << y);
	return;
}

/////////////////////////////////////////////////////////////////////////


static inline void fill_cell(unsigned long to[], signed char x, signed char y)
{ 
		to[x] |= (unsigned long) 1 <<  y;
		return;
}

/////////////////////////////////////////////////////////////////////////


static inline void clear_cell(unsigned long to[], signed char x, signed char y)
{
		to[x] &= ~( (unsigned long) 1 << y);
	return;
}

/////////////////////////////////////////////////////////////////////////


static inline void fill_random_data(unsigned long to[])
{
	unsigned char temp;
	for (temp = 0; temp < CELLS_X; temp++) 
{//	   to[temp] = rand() & 0xFF;

	   to[temp] = rand();
	   to[temp] <<= 15;
	   to[temp] |= rand();
}
	return;
}

/////////////////////////////////////////////////////////////////////////


static inline void clear_data(unsigned long to[])
{
	unsigned char temp;
	for (temp = 0; temp < CELLS_X; temp++) 
	   to[temp] = 0; 
	return;
}




/////////////////////////////////////////////////////////////////////////



static inline void copy_old_new(unsigned long old_gen[], unsigned long new_gen[])
{
	unsigned char temp;
	for(temp = 0; temp < CELLS_X; temp++) old_generation[temp] = current_generation[temp];
	return;
}

/////////////////////////////////////////////////////////////////////////



unsigned char get_neighbours(unsigned long from[], signed char x, signed char y)
{
	unsigned char out = 0;
	
	if(get_cell(from,x-1,y-1))
		out++;
	
	if(get_cell(from,x-1,y))
		out++;
	
	if(get_cell(from,x-1,y+1))
		out++;

	if(get_cell(from,x,y-1))
		out++;
	
	if(get_cell(from,x,y+1))
		out++;

	if(get_cell(from,x+1,y-1))
		out++;
	
	if(get_cell(from,x+1,y))
		out++;
	
	if(get_cell(from,x+1,y+1))
		out++;

	return out;
}

/////////////////////////////////////////////////////////////////////////



unsigned char get_difference(unsigned long a[],unsigned long b[])
{
	unsigned char x,y,diff=0;

	for(x=0; x < CELLS_X; x++)
	{
		for(y=0; y < CELLS_Y; y++)
		{
			if((get_cell(a,x,y) && !get_cell(b,x,y)) || (!get_cell(a,x,y) && get_cell(b,x,y)))
			    diff++;
		}
	}
	return diff;
}

/////////////////////////////////////////////////////////////////////////



static inline void display(unsigned long from[])
{
	unsigned char x,y;

	unsigned long longtemp;

	
	for(x=0; x < CELLS_X; x++)
	{ 
	
		longtemp = 0;
		
		
		for(y=0; y < CELLS_Y; y++)  
		{  
			
			if(get_cell(from,x,y))   
				longtemp |= (unsigned long) 1 << y;
		}
		 
		//longtemp |=  (unsigned long) 1 << 8;   // Optional grid lines
		//longtemp |=  (unsigned long) 1 << 16;  // Optional grid lines
		//longtemp |=  (unsigned long) 1 << 24;  // Optional grid lines
		 
		d[x] = longtemp;
						
	}
		
	return;
}


/////////////////////////////////////////////////////////////////////////


void DisplayLEDs()
{

	unsigned int j,k;
	unsigned char out1,out2,out3,out4;
	unsigned long dtemp;  
	k = 0;
	while (k < refreshNum)		// k must be at least 1

	{

		k++;

		j = 0;

		while (j < 25) 
		{
	
			if (j == 0)
		      PORTD = 160;
			else if (j < 16)
			  PORTD = j;
			else
			  PORTD = (j - 15) << 4;  
	  
			dtemp = d[j]; 
	
			out4 = dtemp & 255U;
			dtemp >>= 8;
			out3 = dtemp & 255U;
			dtemp >>= 8;
			out2 = dtemp & 255U;	 
			dtemp >>= 8;
			out1 = dtemp & 255U; 	
		
			SPI_TX(out1);
			SPI_TX(out2);
			SPI_TX(out3);
			PORTD = 0;  				// Turn displays off
			SPI_TX(out4);


			PORTB |= _BV(1);			// Latch Pulse 
			PORTB &= ~( _BV(1));
 
			j++;
		 
		}
	}
}



void delayLong(unsigned int delayLocal)
{
return;
	unsigned int delayvar;
	delayvar = 0; 

	while (delayvar <=  delayLocal)		
		{ 
			//asm("nop");  
			delayvar++;
		} 
}





void setup_life(void)
{
	int temp;



	signed char x=0,y=0;
	unsigned long dtemp;  
 
	srand(eeprom_read_word((uint16_t *) 2));

	for(temp = 0; temp != 255; temp++)
	{
		TCNT0 = rand();
	}

	eeprom_write_word((uint16_t *) 2,rand());
	  
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

	SPI_TX(0);
	SPI_TX(0);
	SPI_TX(0);

	PORTB |= _BV(1);		//Latch Pulse 
	PORTB &= ~( _BV(1));
 
	fill_random_data(old_generation);
 	
	InputRegOld = (PINC & 31) | ((PINB & 1)<<5);  
}


void loop_life(void)

	{
	signed char x,y;

	static unsigned long dtemp;  
static		unsigned int brightness = 0;

static	unsigned char out1,out2,out3,out4;

		static unsigned char generations=0;

static 	unsigned char temp=0;
	
	
	// Some routines follow to do things if the optional buttons are installed-- a simple editor is implemented.
	 
	InputReg = (PINC & 31) | ((PINB & 1)<<5);    // Input reg measures OFF ( bit 5) and b1-b5 (bits 0-4). 
	InputRegOld ^= InputReg;    // InputRegOld is now nonzero if there has been a change.
	
	if (InputRegOld) {
	
	InputRegOld &= InputReg;  // InputRegOld is now nonzero if the change was to the button-released (not-pulled-down) state.
								// i.e., the bit that was high after the XOR corresponds to a bit that is presently high.
								// The checks that follow will handle MULTIPLE buttons being pressed and unpressed at the same time.
	if (InputRegOld & 1)   //b1 "ANY" button is pressed
		{
		
		if (EditMode){
				
				temp = get_cell(old_generation,yCursor,xCursor);
				
				
			set_cell(current_generation, yCursor, xCursor, !temp ); // Invert Cell
		
		}
		}  
	if (InputRegOld & 2)   //b2 "left" button is pressed
		{
		
			if (EditMode)	{
				xCursor--;
				if(xCursor < 0) 
					xCursor = 24;
			}
		}  
	if (InputRegOld & 4)   //b3 "down" button is pressed	
		{
		
			if (EditMode)
				{ 					
					yCursor++;	// move cursor down
					if(yCursor > 24) 
						yCursor = 0;
				}
			else if (refreshNum < 25)		// if NOT in edit mode
				refreshNum++;

		}  			
	if (InputRegOld & 8)   //b4 "up" button is pressed		
		{

		if (EditMode)
		{
			yCursor--;	// move cursor up
			if(yCursor < 0) 
				yCursor = 24;
		}
		else if (refreshNum > 1)
			refreshNum--;

		}  
	if (InputRegOld & 16)	 //b5 "right" button is pressed
		{
		
			if (EditMode)	
			{
				xCursor++;
				if(xCursor > 24) 
					xCursor = 0;
			} 
		
		}
	if (InputRegOld & 32)	// s2 "Off/Select" button is pressed... 
		{
		EditMode = !EditMode;		//Toggle in/out of edit mode.
		if (EditMode)
			{
			generations = 0;								// Postpone mutations for a while.
			
			//	clear_data(current_generation);				//Clear screen when entering edit mode
															// Quite optional to enable this!
			}	
							
		}  
		
	}

	InputRegOld = InputReg;
		
	if ((InputReg & 1U) == 0) 
		if(EditMode == 0)
			fill_random_data(old_generation);	// Randomize on "Any" key only if we are NOT in edit mode.

	display(old_generation);
	DisplayLEDs();
		
if (EditMode)
{		// Draw an extra dot as the cursor -- Mini display mode.
		// By doing it this way, we're independent of the display matrices.

	dtemp = (unsigned long) 1 << xCursor; 
	
	out4 = dtemp & 255U;
	dtemp >>= 8;
	out3 = dtemp & 255U;
	dtemp >>= 8;
	out2 = dtemp & 255U;	 
	dtemp >>= 8;
	out1 = dtemp & 255U; 	
		
	SPI_TX(out1);
	SPI_TX(out2);
	SPI_TX(out3);
	SPI_TX(out4);


	PORTB |= _BV(1);		//Latch Pulse 
	PORTB &= ~( _BV(1));
 
    if (yCursor < 15)
      PORTD =  yCursor+1;
    else
      PORTD = (yCursor - 14) << 4; 
	  
	  delayLong(brightness);

	PORTD = 0;  // Turn displays off

	SPI_TX(0);
	SPI_TX(0);
	SPI_TX(0);
	SPI_TX(0);

	PORTB |= _BV(1);		//Latch Pulse 
	PORTB &= ~( _BV(1));

	brightness += 50;
	if (brightness > 500)
	brightness = 0;

	DisplayLEDs();

	}
	else
	{		
		
			for(x=0; x < CELLS_X; x++)
			{ 
	
				for(y=0; y < CELLS_Y; y++)		// Inner loop; should be made quick as possible.
				{  
					temp = get_neighbours(old_generation, x, y);
				
					if(temp < 2) 
						clear_cell(current_generation, x, y);
					
					if(temp == 3) 
						fill_cell(current_generation, x, y);
					
					if(temp > 3) 
						clear_cell(current_generation, x, y);	  
	
				}
	
	
				DisplayLEDs();
			
			}

			DisplayLEDs();
		
			if( generations++ == 200 )
			{


				fill_random_data(old_generation);
 
				generations = 0;
			}
		
		
	/*		
			//Alternative boringness detector:
			if(button1() || generations++ == 200 || get_difference(current_generation,old_generation) < 2 || get_total(current_generation) < 6)
			{
				current_generation[7] = rand() & 0xFF;
				current_generation[8] = rand() & 0xFF;
				current_generation[9] = rand() & 0xFF; 
				generations = 0;
			}
	*/

		
		
	}

	copy_old_new(old_generation, current_generation); 
	DisplayLEDs ();
		 
	
}



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


#include "Peggy2.h"


/////////////////////////////////////////////////////////////////////////////////////

// Member constants
// Set this to the number of stars you would like in the starfield
#define NUM_STARS (20)

// Speed of the stars ranges from 0.1 to this.
#define VELOCITY_MAX 1.2

// NOTE: BRIGHT_MAX is related to FRAME_MAX.
// BRIGHT_MAX is the number of greyscale levels and FRAME_MAX is the number
// of frames (or planes) required to represent it.
// Safe numbers are:
/*
   Greyscale levels       Frames
         2                  1
         4                  2
         8                  3
         16                 4
         32                 5
       higher            too slow!
*/
// Set as per chart above.
#define FRAMES_MAX 5

// Set as per chart.  This could be automatically calculated with some macros
// here (it is pow(2,FRAMES_MAX)) but it... isn't.  C'mon now. Really? Use the chart.
#define BRIGHT_MAX 32

// Peggy2 dimensions
#define PEGGY2_HEIGHT (18)
#define PEGGY2_WIDTH (18)


Peggy2 buffer[FRAMES_MAX];   // The drawing frames supporting greyscale
int intensity[FRAMES_MAX];   // Stores LED intensity for greyscale


void setup_fade()                    // run once, when the sketch starts
{
     buffer[0].HardwareInit();   // Call this once to init the hardware. 
                              // (Only needed once, even if you've got lots of frames.)
    
   buffer[0].Clear();
   buffer[1].Clear();
   buffer[2].Clear();
   buffer[3].Clear();
   
// Manually write sample patterns to buffers:

	unsigned short x = 0; 
	unsigned short y = 0;    
	
	while (y < 18 ) {
  
	  x = 0;
	  while (x < 18) {
  
    
	if (x < 5)    
	  buffer[0].SetPoint(x, y);
	else if (x < 10)  
	    buffer[1].SetPoint(x, y);
	else if (x < 15)  
	    buffer[2].SetPoint(x, y);
	 else  
	    buffer[3].SetPoint(x, y);   
	  x++;
	  }
	 y++;
	}

  
}  // End void setup()  



void loop_fade()                     // run over and over again
{ 
	  // What we're doing here is just switching between frames-- can be used for real animation.

	buffer[0].RefreshAll(1); //Draw frame buffer 1 time
	buffer[1].RefreshAll(2); //Draw frame buffer 2 times
	buffer[2].RefreshAll(4); //Draw frame buffer 4 times
	buffer[3].RefreshAll(8); //Draw frame buffer 8 times
}

void setup1()                    // run once, when the sketch starts
{
     buffer[0].HardwareInit();   // Call this once to init the hardware. 
                                        // (Only needed once, even if you've got lots of frames.)
     
   
   
   buffer[0].Clear();

// Manually write sample patterns to buffers:

	unsigned short x = 0; 
	unsigned short y = 0;    

	while (y < 18 ) {
  
		x = 0;
		while (x < 18 ) {

			if ((17-x) == y)
				buffer[0].SetPoint(x, y);   
			x+=1;
		}

		y+=1;
	}

}  // End void setup()  



void loop1()                     // run over and over again
{ 
	  // What we're doing here is just switching between frames-- can be used for real animation.

	buffer[0].RefreshAll(1); //Draw frame buffer 1 time
}

// use this main to run the peggy arduino examples.
   
void setup_block()                    // run once, when the sketch starts
{
	buffer[0].HardwareInit();   // Call this once to init the hardware. 
                              // (Only needed once, even if you've got lots of frames.)
   buffer[0].Clear();
   buffer[1].Clear();
   buffer[2].Clear();
   buffer[3].Clear();
   
// Manually write sample patterns to buffers:

	unsigned short x = 0; 
	unsigned short y = 0;    

	while (y < 18) {
  
	  x = 0;
	  while (x < 18) {
  
    
	if (x < 4)    
	  buffer[0].SetPoint(x, y);
	else if (x < 9)  
	    buffer[1].SetPoint(x, y);
	else if (x < 14)  
	    buffer[2].SetPoint(x, y);
	 else  
	    buffer[3].SetPoint(x, y);   
	  x++;
	  }
	 y++;
	}

  
}  // End void setup()  


void loop_block()                     // run over and over again
{ 
  // What we're doing here is just switching between frames-- can be used for real animation.
	buffer[0].RefreshAll(150); //Draw frame buffer 1
	buffer[1].RefreshAll(150); //Draw frame buffer 2
	buffer[2].RefreshAll(150); //Draw frame buffer 3
	buffer[3].RefreshAll(150); //Draw frame buffer 4
}

void sleep()
{
	// Everything as an output, floating or inputs will consume more mA
   DDRD = 255;
   DDRB = 0;
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

	sleep_bod_disable();

    // Enter sleep mode
    sleep_mode();


}



// The star class represents a star (drawn as one LED on PEGGY2).
// Each star originates at the middle of the board.  Each star
// is randomly assigned an angle, defining their direction from the
// center; a velocity, and a brightness.  Since the standard PEGGY2 has
// width=height I have omitted a scaling factor for the velocity
// of the height versus the width.  If you have stacked PEGGY2s,
// then you will need to add this consideration.

// How brightness is handled:
// Each star is randomly assigned a brightness of 1 to BRIGHT_MAX, representing
// the available greyscale levels on Peggy2.  A brightness of 0 would
// be valid, but isn't drawn (black/off).


// The Star class represents the "star" in the starfield.
class Star 
{
  private:
  
  float angle;       // Angle in radians
  float velocity;    // Speed of star
  float step_x;      // Per frame increment to x pos
  float step_y;      // Per frame increment to y pos

  // Quick utility method to generate random floats  
  float CreateRandomFloat (float maxValue)
  {
    return ((float)rand() / ((float)RAND_MAX +1.) * maxValue);
  }
  
  // Overloaded for ranges
  float CreateRandomFloat (float minValue, float maxValue)
  {
    float swap;
    
    // Validate range
    if (minValue > maxValue)
    {
      swap = maxValue;
      maxValue = minValue;
      minValue = swap;
    }
    
    return ((maxValue- minValue)*((float)rand()/RAND_MAX)) + minValue;
  }
  
  
  public:
  
  float pos_x;      // Raw x position
  float pos_y;      // Raw y position
  int   brightness; // Greyscale level
  
  //Constructor
  Star()
  {
    GenerateStar();
  }

  // Destructor  
  ~Star()
  {
    //Do nothing
  }  
  
  
  // Creates the "star" by defining initial values.
  void GenerateStar()
  {
    // Set the position to be near the centre
    pos_x = PEGGY2_WIDTH / 2;
    pos_y = PEGGY2_HEIGHT / 2;

    // What's going on?  Imagine a circle on a plane bound by a width of 2 and a height
    // of 2 units.  If it is at origin (0,0) it extends up and down from (0,1) to (0,-1)
    // and left to right from (-1,0) to (0,-1).  The circumference of this circle is
    // diameter * PI.  Since d=2, this gives 2PI.  2PI is the number of radians in a 
    // circle; 360o is equivalent to 2PI.
    
    // By using an origin near the centre of the Peggy2 board, we can give each "star"
    // a direction, by giving it an angle from the origin.  This will cause all the
    // stars to look like they are spreading outwards in 3D.  We move the star along the
    // x and y axis depending on its angle and velocity.  The increment amount is calculated
    // via x = cosine(a) and y = sin(a). Next, we multiply the increment by the
    // velocity (v<1 means slower stars, while, you guessed it, v>1 means faster stars).
    
    // All these numbers are floats though, and to display them we have to convert them
    // to real world coordinates that represent LEDs on the Peggy2 board.  To do this,
    // when we plot the pixel as a point on the board, we convert the floats to integer.
    // We use a roundoff function instead of truncating in order to create smoothness.
    
    // By using multiple frames, and drawing the stars into one or more of these frames,
    // and then displaying each frame for a different amount of time, we create greyscale
    // (technically, this is called pulse width modulation).  The program defaults to
    // 5 frames, meaning 2^5 or 32 levels of greyscale are available for the starfield.
    // You can reduce this as per the chart above, but I have found it too slow going
    // any higher.  The brightness variable of the start represents its greyscale level.

    
    // Velocity must be v > 0, and v < VELOCITY_MAX.
    do
    {
      velocity = CreateRandomFloat(0.1f, VELOCITY_MAX);
    }
    while (velocity == 0);
    
    // Create in degrees
    angle = CreateRandomFloat(360.f);
    
    // Convert to radians
    angle = (float)((angle * PI) / 180);
    
    // Pre calculate the incremental steps; this 
    // reduces calc time during execution
    // x increment = cosine(angle) * velocity; y increment = sine(angle) * velocity)
    step_x = (float)(cos(angle) * velocity);
    step_y = (float)(sin(angle) * velocity);
    
    // Integer brightness from 1 to BRIGHT_MAX-1
    brightness = (int)random()%BRIGHT_MAX; 
  }
  
  
  // Move star; if star has moved off screen, regenerate it.
  void MoveStar()
  {
    // Move x,y by step_x, step_y
    pos_x += step_x;
    pos_y += step_y;
    
    if ((pos_x < 0) || (pos_x > PEGGY2_WIDTH) ||
        (pos_y < 0) || (pos_y > PEGGY2_HEIGHT))
      GenerateStar();
  }

};


// Variables

Star starfield[NUM_STARS];   // Starfield of n stars

// Another quick utility method to round - this makes the stars
// display smoother than just truncating... if anything can be "smooth"
// on a 25x25 display. ;)
int Round(float x)
{
  return ((int)(x > 0.0 ? x + 0.5 : x- 0.5));
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
  for (int i=0; i<FRAMES_MAX; i++)
  {
    if ((grey & intensity[i]) == intensity[i])
     buffer[i].SetPoint(x,y); 
  }
}
  

// Redraws frame buffers to create greyscale 
void Redraw()
{
   for (int i=0; i<FRAMES_MAX; i++)
    buffer[i].RefreshAll(intensity[i]);
}
  
  
void setup_star()
{ 
  // Init frame buffer.  Only needs to be done once, no matter how many frames.
  buffer[0].HardwareInit();

  // Randomize
  //randomSeed(millis());
 
  // Pre-calc tbe LED brightness levels to save clocks
  // The madness of the casting and rounding is necessary with pow() otherwise
  // it produces unpredictable integer values.
  for (int i=0; i<FRAMES_MAX; i++)
    intensity[i] = Round(pow (2.f, (float)i));
    
  // Create each star in the starfield.
  for (int i=0; i<NUM_STARS; i++)
    starfield[i].GenerateStar();
} 

 
void loop_star()
{ 
  // Reset all frames.
  for (int i=0; i<FRAMES_MAX; i++)
    buffer[i].Clear();
  
  // Translate each star coords to integer (x,y) locations and draw
  for (int i=0; i<NUM_STARS; i++)
  {
    starfield[i].MoveStar();
    SetPointGrey(Round(starfield[i].pos_x), Round(starfield[i].pos_y), starfield[i].brightness); 
  }
  
  // Draw each frame for its intensity level
  Redraw();
    
  // Pause display if Off/Select button pressed
  while ((PINB & 1) == 0) 
  {
    Redraw();
  }
} 
 
//////////////////////////////////////////////////////////

/// cube



// ------------------------------------------------------------
// Globals
// ------------------------------------------------------------
float focal = 30; // Focal of the camera
int cubeWidth = 18; // Cube size
float Angx = 0, AngxSpeed = 0.017; // rotation (angle+speed) around X-axis
float Angy = 0, AngySpeed = 0.022; // rotation (angle+speed) around Y-axis
float Ox=9,Oy=9; // position (x,y) of the frame center
int zCamera = 110; // distance from cube to the eye of the camera

unsigned short reps = 0; // used for grey scaling
unsigned short repNumber = 1;  //Change scrolling rate-- number of reps at each position.
 

// ------------------------------------------------------------
// Objects
// ------------------------------------------------------------
// The cube is defined before the setup method

// ------------------------------------------------------------
// struct Vertex
// ------------------------------------------------------------
struct Vertex
{
  float x,y,z;	
  Vertex()
  {
    this->set(0,0,0);
  }

  Vertex(float x,float y, float z)
  {
    this->set(x,y,z);
  }

  void set(float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

// ------------------------------------------------------------
// struct EdgePoint
// ------------------------------------------------------------
struct EdgePoint
{
  int x,y;
  char visible;

  EdgePoint()
  {
    this->set(0,0);
    this->visible = false;
  }

  void set(int a,int b)
  {
    this->x = a;
    this->y = b;
  }
};

// ------------------------------------------------------------
// struct Point
// ------------------------------------------------------------
struct Point
{	
  float x,y;

  Point()
  {
    set(0,0);
  }

  Point(float x,float y)
  {
    set(x,y);
  }

  void set(float x, float y)
  {
    this->x = x;
    this->y = y;    
  }

};	

// ------------------------------------------------------------
// struct squareFace
// ------------------------------------------------------------
struct squareFace
{	
  int length;
  int sommets[4];
  int ed[4];

  squareFace()
  {
    set(-1,-1,-1,-1);
  }

  squareFace(int a,int b,int c,int d)
  {
    this->length = 4;
    this->sommets[0]=a;
    this->sommets[1]=b;
    this->sommets[2]=c;
    this->sommets[3]=d;
  }

  void set(int a,int b,int c,int d)
  {
    this->length = 4;
    this->sommets[0]=a;
    this->sommets[1]=b;
    this->sommets[2]=c;
    this->sommets[3]=d;
  }

};

// ------------------------------------------------------------
// struct Cube
// ------------------------------------------------------------
struct Cube
{
  // Local vertices
  Vertex  local[8];
  // Camera aligned vertices
  Vertex  aligned[8];
  // On-screen projected vertices
  Point   screen[8];
  // Faces
  squareFace face[6];
  // Edges
  EdgePoint edge[12];
  int nbEdges;
  // ModelView matrix
  float m00,m01,m02,m10,m11,m12,m20,m21,m22;   

  // constructor
  Cube(){}

  // constructs the cube
  void make(int w)
  {
    nbEdges = 0;

    local[0].set(-w,w,w);
    local[1].set(w,w,w);
    local[2].set(w,-w,w);
    local[3].set(-w,-w,w);
    local[4].set(-w,w,-w);
    local[5].set(w,w,-w);
    local[6].set(w,-w,-w);
    local[7].set(-w,-w,-w);  

    face[0].set(1,0,3,2);
    face[1].set(0,4,7,3);
    face[2].set(4,0,1,5);
    face[3].set(4,5,6,7);
    face[4].set(1,2,6,5);
    face[5].set(2,3,7,6);

    int f,i;
    for (f=0;f<6;f++)
    {	
      for (i=0;i<face[f].length;i++)
      {	
        face[f].ed[i]= this->findEdge(face[f].sommets[i],face[f].sommets[i?i-1:face[f].length-1]);
      }
    }

  }

  // finds edges from faces
  int findEdge(int a,int b)
  {	
    int i;
    for (i=0;i<nbEdges;i++)
      if ( (edge[i].x==a && edge[i].y==b) || (edge[i].x==b && edge[i].y==a))
        return i;
    edge[nbEdges++].set(a,b);
    return i;
  }

  // rotates according to angle x&y
  void rotate(float angx, float angy)
  {
    int i,j;
    int a,b,c;
    float cx=cos(angx);
    float sx=sin(angx);
    float cy=cos(angy);
    float sy=sin(angy);

    m00=cy;   
    m01=0;  
    m02=-sy;
    m10=sx*sy;
    m11=cx; 
    m12=sx*cy;
    m20=cx*sy;
    m21=-sx;
    m22=cx*cy;

    for (i=0;i<8;i++)
    {	
      aligned[i].x=m00*local[i].x+m01*local[i].y+m02*local[i].z;
      aligned[i].y=m10*local[i].x+m11*local[i].y+m12*local[i].z;
      aligned[i].z=m20*local[i].x+m21*local[i].y+m22*local[i].z+zCamera;

      screen[i].x = floor((Ox+focal*aligned[i].x/aligned[i].z));
      screen[i].y = floor((Oy-focal*aligned[i].y/aligned[i].z));	
    }		

    for (i=0;i<12;i++) 
      edge[i].visible=false;

    Point *pa,*pb,*pc;
    for (i=0;i<6;i++)
    {	
      pa=screen + face[i].sommets[0];	
      pb=screen + face[i].sommets[1];	
      pc=screen + face[i].sommets[2];	

      char back=((pb->x-pa->x)*(pc->y-pa->y)-(pb->y-pa->y)*(pc->x-pa->x))<0;
      if (!back)
      {
        int j;
        for (j=0;j<4;j++)
        {	
          edge[face[i].ed[j]].visible=true;
        }
      }      
    }
  }

  // Draw the cube using the line method !
  void draw()
  {
     int i;

    // Backface
    EdgePoint *e;
    for (i=0;i<12;i++)
    {	
      e = edge+i;
      if (!e->visible)
        buffer[0].Line(screen[e->x].x,screen[e->x].y,screen[e->y].x,screen[e->y].y);  
    }

    // Frontface
    for (i=0;i<12;i++)
    {	
      e = edge+i;
      if (e->visible)
      {
        buffer[1].Line(screen[e->x].x,screen[e->x].y,screen[e->y].x,screen[e->y].y);
      }
    }

  }

};





// ------------------------------------------------------------
// setup
// ------------------------------------------------------------
Cube cube;

void setup_cube()
{
  buffer[0].HardwareInit();   // Call this once to init the hardware. 
  cube.make(cubeWidth);    // Create vertices, edges, faces
}

// ------------------------------------------------------------
// loop
// ------------------------------------------------------------
void loop_cube()
{
  // Update values
  Angx+=AngxSpeed;
  Angy+=AngySpeed;
  if (Angx>=TWO_PI)
    Angx-=TWO_PI;
  if (Angy>=TWO_PI)
    Angy-=TWO_PI;

  cube.rotate(Angx, Angy);

  // Clear frames
  buffer[0].Clear();
  buffer[1].Clear();

  // Draw cube
  cube.draw();

  // Flicker !
  for (reps=0; reps< repNumber;reps++)
  {
    buffer[0].RefreshAll(1); //Draw frame buffer 1 time
    buffer[1].RefreshAll(10); //Draw frame buffer 10 times
  }

}

//////////////////////////////////////////////////////////


// PORTB doesn't affect power 
// PORTC 30-40mA
// PORTD +10mA if C is 0xff

int main(void)
{
	unsigned int count ;
	unsigned char temp=0;
	unsigned char mode = 0; 
 
	srand(eeprom_read_word((uint16_t *) 2));

	for(temp = 0; temp != 255; temp++)
	{
		TCNT0 = rand();
	}

	eeprom_write_word((uint16_t *) 2,rand());
	  
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

	SPI_TX(0);
	SPI_TX(0);
	SPI_TX(0);

	PORTB |= _BV(1);		//Latch Pulse 
	PORTB &= ~( _BV(1));
 

loop:

	count = 250;

	switch( mode ) {
		case 0:
			setup_star();
			break;
		case 1:
			setup_cube();
			break;
		case 2:
			setup_fade();
			break;
		case 3:
			setup1();
			break;
		case 4:
			setup_block();
			count = 10;
			break;
		case 5:
			setup_life();
			break;
	}
	
	//sleep();

	while(1) {

		count -- ;

		switch( mode ) {
			case 0:
		 		loop_star();
				break;

			case 1:
				loop_cube();
				break;
			case 2:
				loop_fade();
				break;
			case 3:
				loop1();
				break;
			case 4:
				loop_block();
				break;
			case 5:
				loop_life();
				break;
		}

		if ( count == 0 ){
			mode ++ ;

			if ( mode == 6 ) 
				mode = 0;

			goto loop;

			sleep();
		}
	}

 }

// Timer2 overflow interrupt vector handler
ISR(TIMER2_OVF_vect, ISR_NOBLOCK) 
{
	uint8_t latency = TCNT2;
	TCNT2 = latency + DEFAULT_REFRESHRATE;
}

