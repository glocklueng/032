
#include "RF_Blinky.h"

// Modified version of Garrett's octobrite code



// For octobrite code

#define NUM_TLC5947 ( 1 )

uint16_t LEDChannels[NUM_TLC5947*8][3];


//------------------------------------------------------------------------------------------
// Read all bits in the LEDChannels array and send them on the selected pins
//------------------------------------------------------------------------------------------
void WriteLEDArray() {

  unsigned int tempOne = 0;

  for (unsigned int i = 0; i < (NUM_TLC5947  * 24); i++) {

    tempOne = *(&LEDChannels[0][0] + i);

    for (int j = 0; j < 12; j++) {
      if ((tempOne >> (11 - j)) & 1) {
        DATPORT |= (1 << DATPIN);
      } 
      else {
        DATPORT &= ~(1 << DATPIN);
      }
      CLKPORT |= (1 << CLKPIN);
      CLKPORT &= ~(1 << CLKPIN); 
    } 

  }
  LATPORT |= (1 << LATPIN);
  LATPORT &= ~(1 << LATPIN);
}

//------------------------------------------------------------------------------------------
// Sample function to draw a scanning pattern with fading
//------------------------------------------------------------------------------------------
void LEDscan(int red, int green, int blue, float degreeoffset) 
{

  float brightnessfactor = 0.0f;
  
  float scanindex = (1.0f + sinf(degreeoffset*3.14159f/180.0f)) * ((float)(NUM_TLC5947 * 8 ) / 2.0);
  
    for(unsigned int LEDindex = 0; LEDindex < (NUM_TLC5947  * 8); LEDindex++) {
      

      brightnessfactor = expf(0.0f - fabs(scanindex - ((float)LEDindex + 0.5f)) * 1.3f);

      LEDChannels[LEDindex][0] = blue * brightnessfactor;
      LEDChannels[LEDindex][1] = green * brightnessfactor;  
      LEDChannels[LEDindex][2] = red * brightnessfactor;  
    }
    
    WriteLEDArray();  

}

//------------------------------------------------------------------------------------------
// Sets up TLC5947 and TRC104
//------------------------------------------------------------------------------------------
void LED_Init(void)
{

 // Setup TLC5947 pins
	DATDDR	|= ( SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
	DATPORT	|= ( SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
	
	DATPORT |= (LATPIN);
	DATPORT &= ~(BLANK_PIN);

}

