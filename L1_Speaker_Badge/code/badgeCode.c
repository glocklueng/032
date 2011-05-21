/*

Title:			peggy2.c 
Date Created:   3/23/08
Last Modified:  4/28/08
Target:			Atmel ATmega168 
Environment:	AVR-GCC
Purpose:		Drive 25x25 LED array
Application:	Conway's Life

Copyright 2008 Windell H. Oskay
Distributed under the terms of the GNU General Public License, please see below. 
Adapted from a program by David Gustafik, and re-released under the GPL. :)

We'd really like to see improved versions of this software!

 
Portions Copyright David Gustafik, 2007

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/



#include <avr/io.h> 
#include <stdlib.h> 
#include <avr/eeprom.h>
#include <stdint.h>

//#include <stdio.h>

#define CELLS_X 25		
#define CELLS_Y 25 


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
		 
//		longtemp |=  (unsigned long) 1 << 8;   // Optional grid lines
//		longtemp |=  (unsigned long) 1 << 16;  // Optional grid lines
//		longtemp |=  (unsigned long) 1 << 24;  // Optional grid lines
		 
		d[x] = longtemp;
						
	}
		
	return;
}


/////////////////////////////////////////////////////////////////////////

void SPI_TX (char cData)
{
//Start Transmission
SPDR = cData;
//Wait for transmission complete:
while (!(SPSR & _BV(SPIF))) ;
	
}


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





int main(void)
{
	unsigned char generations=0;

	unsigned char temp=0;
	signed char x=0,y=0;
	
	unsigned char out1,out2,out3,out4;
	unsigned long dtemp;  
	unsigned int brightness = 0;
 
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


	while(1)    //Main Loop
	{
	
	
	
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
			current_generation[0] = rand() & 0xFF;
			current_generation[23] = rand() & 0xFF;
			current_generation[24] = rand() & 0xFF; 
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
}

