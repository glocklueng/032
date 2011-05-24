/*
  Peggy2.cpp - Peggy 2.0 LED Matrix library for Arduino
  LIBRARY VERSION: 0.30b, DATED 7/08/2008

  
  
  Copyright (c) 2008 Windell H Oskay.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modifications by Michael Yin, Copyright (c) 2008. All rights reserved.
  
  Line code contributed by Michaël Zancan & Julien 'v3ga' Gachadoat
  websites: http://www.2roqs.com    http://www.zancan.fr    http://www.v3ga.net
  
*/


#include "common.h"



// Constructors ////////////////////////////////////////////////////////////////

Peggy2::Peggy2()
{
	// 100 bytes
	pbuffer = (uint32_t*)calloc(25, sizeof(uint32_t));
}

 
void Peggy2::SPI_TX(char cData)
{
	SPDR = cData;
	//Wait for transmission complete:
	while (!(SPSR & _BV(SPIF))) ;
} 


void Peggy2::HardwareInit()
{
	//  Hardware Initialization:

	PORTD = 0U;
	DDRD = 255U;

	////SET MOSI, SCK Output, all other SPI as input: 
	DDRB |= _BV(5) | _BV(3) | _BV(2) | _BV(1);

	DDRC = 0;		//All inputs

	//ENABLE SPI, MASTER, CLOCK RATE fck/4:  
	SPCR =  _BV(SPE) |  _BV(MSTR) ;

	//  Flush SPI LED drivers::
	SPI_TX(0);
	SPI_TX(0);
	SPI_TX(0);
	SPI_TX(0);

	PORTB |= 2;    //Latch Pulse 
	PORTB &= 253;  
}


void delay()
{
	unsigned int delayvar;
	
	delayvar = 0; 

	while (delayvar <=  6000U) { 
		asm("nop");  
		delayvar++;
	}
}


void Peggy2::RefreshAll(unsigned int refreshNum)
{
  unsigned int i,k;
  unsigned char j;
  
  union mix_t {
    unsigned long atemp; 
    unsigned char c[4];
  } mix;
  
  
  k = 0;
  
  while (k != refreshNum)   
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
      
	  i = 0;
	  while (i < 50)
	  {
	  asm("nop"); 
	  i++;
	  }
	  
  	  mix.atemp = pbuffer[j];
	  
	  if( j <= 8 || j >= 15 ) {
	      SPI_TX(mix.c[3]);
    	  SPI_TX(mix.c[2]);
      		SPI_TX(mix.c[1]);
		} else { 
	     // SPI_TX(0);
    	 // SPI_TX(0);
      	 // SPI_TX(0);
		}

	  
      PORTD = 0;  // Turn displays off
 
      SPI_TX(mix.c[0]); 
      PORTB |= 2U;    //Latch Pulse 
      PORTB &= 253U;
      
      j++;
    }
  }
}

// Set up Timer2
//
// Configures the ATMega168 8-Bit Timer2 to generate an interrupt
// at the specified frequency.
//
// Returns the timer load value which must be loaded into TCNT2
// inside the ISR routine.
uint8_t Peggy2::SetupTimer2(void)
{
  uint8_t result; //The timer load value.

  TCCR2A = 0;
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);

  // Timer2 Overflow Interrupt Enable
  TIMSK2 = _BV(TOIE2);

  // Load the timer for its first cycle
	result = (uint8_t)((256 - (TIMER_CLOCK_FREQ / 25 / DEFAULT_REFRESHRATE)) + 0.5);
  TCNT2 = result;

  return(result);
}



void Peggy2::RefreshAllFast(unsigned int refreshNum)
{
  unsigned int k;
  unsigned char j;
  
  union mix_t {
    unsigned long atemp; 
    unsigned char c[4];
  } mix;
  
  
  k = 0;
  
  while (k != refreshNum)   
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
      
	  
  	  mix.atemp = pbuffer[j];
	  
      SPI_TX(mix.c[3]);
      SPI_TX(mix.c[2]);
      SPI_TX(mix.c[1]);
	  
      PORTD = 0;  // Turn displays off
 
      SPI_TX(mix.c[0]); 
      PORTB |= 2U;    //Latch Pulse 
      PORTB &= 253U;
      
      j++;
    }
  }
}

void Peggy2::Clear() 
{
  memset(pbuffer, 0, 25*sizeof(uint32_t));
}


void Peggy2::WritePoint(uint8_t xPos, uint8_t yPos, uint8_t Value)
{
	if( xPos < 0 || xPos > PEGGY2_WIDTH -1 ) return; 
	if( yPos < 0 || yPos > PEGGY2_HEIGHT-1) return; 
	
	
	// this is for the badge, it  skips the missing LED's. Should really be in the refresh code.
	if (xPos > 8 ) xPos += 7;
	if (yPos > 8 ) yPos += 6;

  if (Value)
    pbuffer[yPos] |= (uint32_t) 1 << xPos;
  else
    pbuffer[yPos] &= ~((uint32_t) 1 << xPos);
}

void Peggy2::WriteRow(uint8_t yPos, uint32_t row)
{
  if (yPos > 8 ) 
  	yPos += 6;

  pbuffer[yPos] = row;
}

void Peggy2::SetRow(uint8_t yPos, uint32_t row = PEGGY_ROW_ON)
{
  if (yPos > 8 ) yPos += 6;
  pbuffer[yPos] |= row;
}

// should ClearRow be implemented? if you only want to clear portions of the row, 
// you'd need to send masks. Might be overly complex to document.

// Turn a pixel on
void Peggy2::SetPoint(uint8_t xPos, uint8_t yPos)
{
	if (xPos > 8 ) xPos += 7;
	if (yPos > 8 ) yPos += 6;

	pbuffer[yPos] |= (uint32_t) 1 << xPos; 
}

// Turn a pixel on (clips)
void Peggy2::SetPointClip(uint8_t xPos, uint8_t yPos)
{
	if( xPos < 0 || xPos > PEGGY2_WIDTH -1 ) return; 
	if( yPos < 0 || yPos > PEGGY2_HEIGHT-1) return; 

	if (xPos > 8 ) xPos += 7;
	if (yPos > 8 ) yPos += 6;


	pbuffer[yPos] |= (uint32_t) 1 << xPos; 
}


// Determine if a pixel is on or off
uint8_t Peggy2::GetPoint(uint8_t xPos, uint8_t yPos)
{
	if( xPos < 0 || xPos > PEGGY2_WIDTH -1 ) return 0; 
	if( yPos < 0 || yPos > PEGGY2_HEIGHT-1) return 0;  


	if (xPos > 8 ) xPos += 7;
	if (yPos > 8 ) yPos += 6;

	return ((pbuffer[yPos] & (uint32_t) 1 << xPos) > 0); 
}


// Turn a pixel off
void Peggy2::ClearPoint(uint8_t xPos, uint8_t yPos)
{
  if (xPos > 8 ) xPos += 7;
  if (yPos > 8 ) yPos += 6;

  pbuffer[yPos] &= ~((uint32_t) 1 << xPos);
}
 
void Peggy2::Line(int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
  if ( (x1>=25 && x2>=25) || (y1>=25 && y2>=25) ) return;
  int8_t dx = ABS(x2 -x1);
  int8_t dy = ABS(y2 -y1);

  int8_t p1x,p1y,p2x,p2y,i;

  if (dx > dy)
  {
    if (x2>x1) {
      p1x=x1;
      p1y=y1;
      p2x=x2;
      p2y=y2;
    } 
    else {
      p1x=x2;
      p1y=y2;
      p2x=x1;
      p2y=y1;
    }
	
    int8_t y = p1y;
    int8_t x = p1x;
    int8_t count = 0;
    int8_t increment = p2y > p1y ? 1 : -1;
    for (i=0; i<=dx; i++)
    {	
      count += dy;
      if (count > dx)
      {
        count -= dx; 
        y+= increment;
      }				
      if (y>=0 && y<25 && x>=0 && x<25) 
		SetPoint(x,y);
      
		x++; 
      if (x>=25) 
		break;
    }

  }
  else
  {
    if (y2>y1) {
      p1x=x1;
      p1y=y1;
      p2x=x2;
      p2y=y2;
    } 
    else {
      p1x=x2;
      p1y=y2;
      p2x=x1;
      p2y=y1;
    }
    int8_t y = p1y;
    int8_t x = p1x;
    int8_t count = 0;
    int8_t increment = p2x > p1x ? 1 : -1;
    for (i=0; i<=dy; i++)
    {	
      count += dx;
      if (count > dy)
      {
        count -= dy; 
        x+= increment;
      }				
      if (y>=0 && y<25 && x>=0 && x<25) SetPoint(x,y);
      y+=1; 
      if (y>=25) break;
    }

  }
 
}
	
	 
//Set current cursor position to (xPos,yPos)
void Peggy2::MoveTo(int8_t xPos, int8_t yPos)
{
	_Xcursor = xPos;
	_Ycursor = yPos;
}

//Draw a line from current cursor position to (xPos,yPos)
void Peggy2::LineTo(int8_t xPos, int8_t yPos)
{
	Line(_Xcursor, _Ycursor, xPos, yPos);
	_Xcursor = xPos;
	_Ycursor = yPos;
}
    


/**
 *	Draw an outlined single colour circle 
 *
 * @param colour    - colour
 * @param xc	    - x centre position
 * @param yc		- y centre position
 * @param radius	- radius of circle
 *
 */
void Peggy2::DrawOutlineCircle( char xc,char yc, unsigned char radius  )
{
	short x, y, p;
	
	if ( !radius ) {
		return;	
	}

	x = 0;
	y = radius;
	p = 3 - 2 * radius;

	while ( x <= y ) {

		SetPointClip(xc+x,yc+y);
		SetPointClip(xc-x,yc+y);
		SetPointClip(xc+x,yc-y);
		SetPointClip(xc-x,yc-y);
		SetPointClip(xc+y,yc+x);
		SetPointClip(xc-y,yc+x);
		SetPointClip(xc+y,yc-x);
		SetPointClip(xc-y,yc-x);
		
		if (p < 0) {
			p = p + 4 * x + 6;
		} else {
			p = p + 4 * ( x - y ) + 10;
			y --;
		}
		
		x++;
	}
}

