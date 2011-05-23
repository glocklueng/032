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

#if USE_FADER


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

#endif

#if USE_LINE

void setup_line()                    // run once, when the sketch starts
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



void loop_line()                     // run over and over again
{ 
	  // What we're doing here is just switching between frames-- can be used for real animation.

	buffer[0].RefreshAll(1); //Draw frame buffer 1 time
}

#endif


#if USE_BLOCK
   
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

  gCount = 10;

}  // End void setup()  


void loop_block()                     // run over and over again
{ 
  // What we're doing here is just switching between frames-- can be used for real animation.
	buffer[0].RefreshAll(150); //Draw frame buffer 1
	buffer[1].RefreshAll(150); //Draw frame buffer 2
	buffer[2].RefreshAll(150); //Draw frame buffer 3
	buffer[3].RefreshAll(150); //Draw frame buffer 4
}

#endif
