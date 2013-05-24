#include "leds.h"

uint16_t LEDChannels[(NUM_TLC5947)];

void SetPoint( unsigned short x, unsigned short y,unsigned short val)
{
	LEDChannels[ (  x * LEDS_WIDTH ) + ( LEDS_HEIGHT - y ) ] = val;
}

// 0.4095
void Clear(unsigned short val)
{

    for(unsigned short LEDindex = 0; LEDindex < (NUM_TLC5947 ); LEDindex++)
	{
      LEDChannels[LEDindex] = val;
    }
}


//------------------------------------------------------------------------------------------
// Read all bits in the LEDChannels array and send them on the selected pins
//------------------------------------------------------------------------------------------
void WriteLEDArray(unsigned int count) {

  unsigned short tempOne = 0;

  for (unsigned int i = 0; i < (count); i++) {

    tempOne = LEDChannels[i];

    for (int j = 0; j < 12; j++) {
      if ((tempOne >> (11 - j)) & 1) {
        PORTD.OUT |= (1 << DATPIN);
      } 
      else {
        PORTD.OUT &= ~(1 << DATPIN);
      }

      PORTD.OUT |= (1 << CLKPIN);
      PORTD.OUT &= ~(1 << CLKPIN); 

    } 
  }

  PORTD.OUT |= (1 << LATPIN | 1 << BLANKPIN);
  PORTD.OUT &= ~(1 << LATPIN | 1 << BLANKPIN);
}

//------------------------------------------------------------------------------------------
// Sample function to draw a scanning pattern with fading
//------------------------------------------------------------------------------------------
void LEDscan(int red, float degreeoffset,unsigned int count) 
{

  float brightnessfactor = 0.0f;
  
  float scanindex = (1.0f + sinf(degreeoffset*3.14159f/180.0f)) * ((float)(NUM_TLC5947 * 24 ) / 2.0);
  
    for(unsigned int LEDindex = 0; LEDindex < (NUM_TLC5947); LEDindex++) {
      

      brightnessfactor = expf(0.0f - fabs(scanindex - ((float)LEDindex + 0.5f)) * 1.3f);

      LEDChannels[LEDindex] = red * brightnessfactor;
    }
    
    WriteLEDArray(count);  
}

void LEDscan2(int red, float degreeoffset,unsigned int count) 
{

  float brightnessfactor = 0.0f;
  
  float scanindex = (1.0f + sinf(degreeoffset*3.14159f/180.0f)) * ((float)(count * 24 ) / 2.0);
  
    for(unsigned int LEDindex = 0; LEDindex < (count  ); LEDindex++) {
      

      brightnessfactor = expf(0.0f - fabs(scanindex - ((float)LEDindex + 0.5f)) * 1.3f);

      LEDChannels[LEDindex] = red * brightnessfactor;
    }
}

void LEDscan2Add(int red, float degreeoffset,unsigned int count) 
{

  float brightnessfactor = 0.0f;
  
  float scanindex = (1.0f + sinf(degreeoffset*3.14159f/180.0f)) * ((float)(count * 24 ) / 2.0);
  
    for(unsigned int LEDindex = 0; LEDindex < (count  ); LEDindex++) {
      

      brightnessfactor = expf(0.0f - fabs(scanindex - ((float)LEDindex + 0.5f)) * 1.3f);

      LEDChannels[LEDindex] += red * brightnessfactor;

	  if( LEDChannels[LEDindex] > 4095 )  LEDChannels[LEDindex] = 4095;
    }
}


void LEDscan3(int red, float degreeoffset,unsigned int count) 
{

  float brightnessfactor = 0.0f;
  
  float scanindex = (1.0f + sinf(degreeoffset*3.14159f/180.0f)) * ((float)(count * 24 ) / 2.0);
  
    for(unsigned int LEDindex = 0; LEDindex < (count); LEDindex++) {
      

      brightnessfactor = expf(0.0f - fabs(scanindex - ((float)LEDindex + 0.5f)) * 1.3f);

      LEDChannels[LEDindex] = 4096-(red * brightnessfactor);
    }    
}

//------------------------------------------------------------------------------------------
// Sets up TLC5947 and MMA7455
//------------------------------------------------------------------------------------------
void LED_Init(void)
{

 // Setup TLC5947 pins
//	DATDDR	|= ( SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
//	DATPORT	|= ( SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
	
	PORTD.OUT &= ~(LATPIN);
	PORTD.OUT &= ~(BLANK_PIN);

	// clear the array
	memset(LEDChannels,0,sizeof(LEDChannels)*sizeof(uint16_t));

	// push off empty, do this before the sleep setup
	WriteLEDArray(NUM_TLC5947);

}
