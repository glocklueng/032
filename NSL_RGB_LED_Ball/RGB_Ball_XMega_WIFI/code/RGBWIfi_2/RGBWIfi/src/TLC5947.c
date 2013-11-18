#include "leds.h"

// Array storing color values
//  BLUE: LEDChannels[x][0]   Range: {0 to 4095}
// GREEN: LEDChannels[x][1]   Range: {0 to 4095}
//   RED: LEDChannels[x][2]   Range: {0 to 4095}
uint8_t RGBChannels[NUM_TLC5947 * 8][3] = {{0}};

unsigned char brightnessShift = 0;

typedef struct LED_tag {
	unsigned short red;
	unsigned short green;
	unsigned short blue;
} LED;


typedef struct Slice_tag {
	LED led[ 24 ];
} Slice;

typedef struct Ball_Tag {

	Slice slice[ 16];

} Ball;


//------------------------------------------------------------------------------------------
// Sets up TLC5947 and MMA7455
//------------------------------------------------------------------------------------------
void LED_Init ( void )
{
	// Setup TLC5947 pins

	PORT_OUT &= ~ ( 1 << LATPIN );

	PORT_OUT &= ~ ( 1 << BLANKPIN );

	// full bright
	brightnessShift = 0;


	// clear the array
	memset ( RGBChannels, 0, sizeof ( RGBChannels ) *sizeof ( uint16_t ) );

	// push off empty, do this before the sleep setup
	WriteRGBArray();

}
// Read all bits in the LEDChannels array and send them on the selected pins
void WriteRGBArray ( void )
{
	unsigned int tempOne = 0;

	for ( int i = 0; i < ( NUM_TLC5947 * 24 ); i++ ) {

		tempOne = * ( &RGBChannels[0][0] + i );

		for ( int j = 0; j < 12; j++ ) {
			if ( ( tempOne >> ( 11 - j ) ) & 1 ) {
				PORT_OUT |= ( 1 << DATPIN );

			} else {
				PORT_OUT &= ~ ( 1 << DATPIN );
			}

			PORT_OUT |= ( 1 << CLKPIN );
			PORT_OUT &= ~ ( 1 << CLKPIN );
		}

	}

	PORT_OUT |= ( 1 << LATPIN )  ;
	PORT_OUT &= ~ ( 1 << LATPIN );
}

void RGBscan ( int red, int green, int blue, float degreeoffset )
{

	float brightnessfactor = 0;

	float scanindex = ( 1.0f + sinf ( degreeoffset * 3.14159f / 180.0f ) ) * ( ( float ) ( NUM_TLC5947 * 8 ) / 2.0f );

	for ( int LEDindex = 0; LEDindex < ( NUM_TLC5947 * 8 ); LEDindex++ ) {

		brightnessfactor = expf ( 0.0f - fabs ( scanindex - ( ( float ) LEDindex + 0.5f ) ) * 2.0f );

		RGBChannels[LEDindex][0] = red * brightnessfactor;
		RGBChannels[LEDindex][1] = green * brightnessfactor;
		RGBChannels[LEDindex][2] = blue * brightnessfactor;
	}

	WriteRGBArray();

}

void SetRGB ( unsigned short red, unsigned short green , unsigned short blue )
{
	for ( int LEDindex = 0; LEDindex < ( NUM_TLC5947 * 8 ); LEDindex++ ) {
		RGBChannels[LEDindex][0] = red ;
		RGBChannels[LEDindex][1] = green ;
		RGBChannels[LEDindex][2] = blue ;
	}

	WriteRGBArray();
}