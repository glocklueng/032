

#include "leds.h"


uint8_t LEDChannels[(NUM_TLC5947)];

unsigned char brightnessShift= 0;

// 0.4095
void Clear(unsigned char val)
{

    for(unsigned short LEDindex = 0; LEDindex < (NUM_TLC5947 ); LEDindex++)
	{
      LEDChannels[LEDindex] = val;
    }
}


//------------------------------------------------------------------------------------------
// Read all bits in the LEDChannels array and send them on the selected pins
//------------------------------------------------------------------------------------------
void WriteLEDArray(unsigned char count) 
{


	unsigned short tempOne = 0;

	for (unsigned int i = 0; i < (count); i++) {

		tempOne = (LEDChannels[i]*16) >> brightnessShift;

		for (int j = 0; j < 12; j++) {
			if ((tempOne >> (11 - j)) & 1) {
				PORTB |= (1 << DATPIN);
			}
			else {
				PORTB &= ~(1 << DATPIN);
			}

			PORTB |= (1 << CLKPIN);
			PORTB &= ~(1 << CLKPIN);

		}
	}

	PORTB |= (1 << LATPIN | 1 << BLANKPIN);
	PORTB &= ~(1 << LATPIN | 1 << BLANKPIN);
}

void WriteArrayOffset(unsigned int count, unsigned int offset) 
{

	PORTB &= ~(1 << LATPIN);
	_delay_us(30);

	unsigned short tempOne = 0;
	unsigned int 	 tempOffset = 0;

	for (unsigned int i = 0; i < (count ); i++) {
		if (i+offset>=count) tempOffset = offset-(count);
		else tempOffset = offset;
		tempOne = LEDChannels[i+tempOffset] * 16;

		for (int j = 0; j < 12; j++) {
			if ((tempOne >> (11 - j)) & 1) {
				PORTB |= (1 << DATPIN);
			}
			else {
				PORTB &= ~(1 << DATPIN);
			}

			PORTB |= (1 << CLKPIN);
			PORTB &= ~(1 << CLKPIN);

		}
	}

	PORTB |= (1 << LATPIN);

	_delay_us(30);
}

//------------------------------------------------------------------------------------------
// Sets up TLC5947 and MMA7455
//------------------------------------------------------------------------------------------
void LED_Init(void)
{

 // Setup TLC5947 pins
	DDRB |= ( SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
	PORTB	|= ( SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
	
	// full bright		
	brightnessShift = 0;
		
		
	PORTB &= ~(LATPIN);
	PORTB &= ~(BLANK_PIN);

	// clear the array
	memset(LEDChannels,0,sizeof(LEDChannels)*sizeof(uint8_t));

	// push off empty, do this before the sleep setup
	WriteLEDArray(NUM_TLC5947);

}
