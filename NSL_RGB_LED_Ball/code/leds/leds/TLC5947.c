#include "leds.h"

uint16_t LEDChannels[(NUM_TLC5947)];
// Array storing color values
//  BLUE: LEDChannels[x][0]   Range: {0 to 4095}
// GREEN: LEDChannels[x][1]   Range: {0 to 4095}
//   RED: LEDChannels[x][2]   Range: {0 to 4095}
uint16_t RGBChannels[3*8][3] = {0};

unsigned char brightnessShift= 0;

typedef struct LED_tag {
	unsigned short red;
	unsigned short green;
	unsigned short blue;
} LED;


typedef struct Slice_tag {
	LED led[ 16 ];
} Slice;

typedef struct Ball_Tag {
	
	Slice slice[ 16];

} Ball;
	

static inline void SetLEDChannel( unsigned int offset , unsigned short value)
{
	if( offset >= NUM_TLC5947 ) {
		offset = NUM_TLC5947-1;
	}
	LEDChannels[ offset ] = value ;
}

static inline unsigned int GetLEDChannel( unsigned int offset )
{
	if( offset >= NUM_TLC5947 ) {
		offset = NUM_TLC5947-1;
	}
	
	return LEDChannels[ offset ] ;
}



void SetPoint( unsigned short x, unsigned short y,unsigned short val)
{
	unsigned int offset ;

	offset = (  x * LEDS_WIDTH ) + ( LEDS_HEIGHT - y );
	
	SetLEDChannel( offset,val);
}

// 0.4095
void Clear(unsigned short val)
{

	for(unsigned short LEDindex = 0; LEDindex < (NUM_TLC5947 ); LEDindex++)
	{
		SetLEDChannel( LEDindex,val);
	}
}

//------------------------------------------------------------------------------------------
// Read all bits in the LEDChannels array and send them on the selected pins
//------------------------------------------------------------------------------------------
void WriteLEDArray(unsigned int count) {

	unsigned int tempOne = 0;

	if ( count > NUM_TLC5947 ){
		count = NUM_TLC5947;
	}
	for (unsigned int i = 0; i < (count); i++) {

		tempOne = GetLEDChannel(i) >> brightnessShift;

		for (int j = 0; j < 12; j++) {
			if ((tempOne >> (11 - j)) & 1) {
				PORT_OUT |= (1 << DATPIN);
			}
			else {
				PORT_OUT &= ~(1 << DATPIN);
			}
			PORT_OUT |= (1 << CLKPIN);

			PORT_OUT &= ~(1 << CLKPIN);

		}
	}

	PORT_OUT |= (1 << LATPIN | 1 << BLANKPIN);
	PORT_OUT &= ~(1 << LATPIN | 1 << BLANKPIN);
	
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

		SetLEDChannel( LEDindex , red * brightnessfactor);
	}
	
	WriteLEDArray(count);
}

void LEDscan2(int red, float degreeoffset,unsigned int count)
{

	float brightnessfactor = 0.0f;
	
	float scanindex = (1.0f + sinf(degreeoffset*3.14159f/180.0f)) * ((float)(count * 24 ) / 2.0);
	
	for(unsigned int LEDindex = 0; LEDindex < (count  ); LEDindex++) {
		

		brightnessfactor = expf(0.0f - fabs(scanindex - ((float)LEDindex + 0.5f)) * 1.3f);

		SetLEDChannel( LEDindex , red * brightnessfactor);
	}
}

void LEDscan2Add(int red, float degreeoffset,unsigned int count)
{

	float brightnessfactor = 0.0f;
	
	float scanindex = (1.0f + sinf(degreeoffset*3.14159f/180.0f)) * ((float)(count * 24 ) / 2.0);
	
	for(unsigned int LEDindex = 0; LEDindex < (count  ); LEDindex++) {
		

		brightnessfactor = expf(0.0f - fabs(scanindex - ((float)LEDindex + 0.5f)) * 1.3f);

		LEDChannels[LEDindex] += red * brightnessfactor;

		if( LEDChannels[LEDindex] > 4095 )  
			LEDChannels[LEDindex] = 4095;
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

void WriteArrayOffset(unsigned int count, unsigned int offset)
{

	unsigned short tempOne = 0;
	unsigned int 	 tempOffset = 0;

	if ( count > NUM_TLC5947 ){
		count = NUM_TLC5947;
	}

	PORT_OUT &= ~(1 << LATPIN);

	for (unsigned int i = 0; i < (count ); i++) {
		if (i+offset>=count)
			tempOffset = offset-(count);
		else
			tempOffset = offset;
		
		tempOne = GetLEDChannel(i+tempOffset);

		for (int j = 0; j < 12; j++) {
			if ((tempOne >> (11 - j)) & 1) {
				PORT_OUT |= (1 << DATPIN);
			}
			else {
				PORT_OUT &= ~(1 << DATPIN);
			}


			PORT_OUT |= (1 << CLKPIN);
			PORT_OUT &= ~(1 << CLKPIN);

		}
	}

	PORT_OUT |= (1 << LATPIN);

}

//------------------------------------------------------------------------------------------
// Sets up TLC5947 and MMA7455
//------------------------------------------------------------------------------------------
void LED_Init(void)
{

	// Setup TLC5947 pins
	
	PORT_OUT &= ~(1<<LATPIN);
	
	PORT_OUT &= ~(1<<BLANKPIN);
	
	// full bright
	brightnessShift = 0;


	// clear the array
	//memset(LEDChannels,0,sizeof(LEDChannels)*sizeof(uint16_t));

	// push off empty, do this before the sleep setup
	WriteLEDArray(NUM_TLC5947);

}
// Read all bits in the LEDChannels array and send them on the selected pins
void WriteRGBArray(void)
{

	unsigned int tempOne = 0;

	for (int i = 0; i < (3 * 24); i++) {

		tempOne = *(&RGBChannels[0][0] + i);

		for (int j = 0; j < 12; j++) {
			if ((tempOne >> (11 - j)) & 1) {
				PORT_OUT |= (1 << DATPIN);
			}
			else {
				PORT_OUT &= ~(1 << DATPIN);
			}
			PORT_OUT |= (1 << CLKPIN);
			PORT_OUT &= ~(1 << CLKPIN);
		}

	}
	PORT_OUT |= (1 << LATPIN);
	PORT_OUT &= ~(1 << LATPIN);
}

void RGBscan(int red, int green, int blue, float degreeoffset) 
{

	float brightnessfactor = 0;
	
	float scanindex = (1.0 + sin(degreeoffset*3.14159/180.0)) * ((float)(3 * 8) / 2.0);
	
	for(int LEDindex = 0; LEDindex < (3 * 8); LEDindex++) {
		
		brightnessfactor = exp(0.0 - fabs(scanindex - ((float)LEDindex + 0.5)) * 1.3);
		
		RGBChannels[LEDindex][0] = blue * brightnessfactor;
		RGBChannels[LEDindex][1] = green * brightnessfactor;
		RGBChannels[LEDindex][2] = red * brightnessfactor;
	}
	
	WriteRGBArray();

}