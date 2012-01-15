
/*
* Octobrite Test
 * 6/8/2009 - yergacheffe@yerga.com
 * 
 * Cycles a continuous color cycle through the 8 leds
 */

#include <digitalWriteFast.h>
#include <avr/delay.h>


// Color channel constants
#define RED 0
#define GREEN 1
#define BLUE 2

// TLC5947 maintains 12 bits of grayscale per LED channel
#define CHANNEL_BITS (12)
#define CHANNEL_MAX ( (1<<CHANNEL_BITS) - 1)

// TLC5947 pins
unsigned int SIN_PIN = 1;
unsigned int SCLK_PIN = 2;
unsigned int BLANK_PIN = 3;
unsigned int XLAT_PIN = 4;

// Interpolation constants
int lerpsteps = 5;
int digitalWriteFast22lay =1;

int lerpindex = 0;

// Color table and current/prev color state. The interpolation looks
// best when only one channel is changing during an interpolation
// between a pair of adjacent colors.
unsigned char  colorCount = 6;
unsigned char clutred[6] =   { 
  1, 1, 0, 0, 0, 1};
unsigned char  clutgreen[6] = { 
  0, 0, 0, 1, 1, 1};
unsigned char clutblue[6] =  { 
  0, 1, 1, 1, 0, 0};
int currentColorIndex = 0;
unsigned char previousColor[3];
unsigned char currentColor[3];

void setup()
{
  // Setup TLC5947 pins
  pinMode(SIN_PIN, OUTPUT);
  pinMode(SCLK_PIN, OUTPUT);
  pinMode(BLANK_PIN, OUTPUT);
  pinMode(XLAT_PIN, OUTPUT);

  // Turn off all LEDs
  digitalWriteFast2(BLANK_PIN, HIGH);

  // Default state for clock and data latch
  digitalWriteFast2(SCLK_PIN, LOW);
  digitalWriteFast2(XLAT_PIN, LOW);

  // Init comms for debug info
  // Serial.begin(9600);

  // Reset color state
  currentColor[RED]   = clutred[currentColorIndex];
  currentColor[GREEN] = clutgreen[currentColorIndex];
  currentColor[BLUE]  = clutblue[currentColorIndex];
  NextColor();

  // And write the colors to the octobrite
  WriteColors();

  // Finally enable the LEDs
  digitalWriteFast2(BLANK_PIN, LOW);
}

// Called when we've completed an interpolation between a pair of
// colors. This sets us up to interpolate to the next color in
// the lookup table
void NextColor()
{
  // We've finished interpolating to the current color, so it becomes the
  // previoud color we're interpolating from
  previousColor[RED]   = currentColor[RED];  
  previousColor[GREEN] = currentColor[GREEN];  
  previousColor[BLUE]  = currentColor[BLUE];

  // And pick the next color in the CLUT for the current color
  // currentColorIndex = (currentColorIndex + 1) % colorCount;
  currentColorIndex = (currentColorIndex+1) - colorCount * (currentColorIndex / colorCount);

  lerpindex = 0;
  currentColor[RED]   = clutred[currentColorIndex];
  currentColor[GREEN] = clutgreen[currentColorIndex];
  currentColor[BLUE]  = clutblue[currentColorIndex];
}

// Writes out a single 12-bit grayscale channel
void WriteChannel(int value)
{
  int bit;

  // Write value, MSB first
  for (bit=11; bit>=0; --bit)
  {
    if (value & (1<<bit))
    {
      digitalWriteFast2(SIN_PIN, HIGH);
    }
    else
    {
      digitalWriteFast2(SIN_PIN, LOW);
    }

    // We need to wait 30ns after writing data before clocking it in.
    // Fortunately, our AVR is slow enough that we don't need to 
    // do an explicit wait here
    digitalWriteFast2(SCLK_PIN, HIGH);
    digitalWriteFast2(SCLK_PIN, LOW);
  }
}

void WriteColors()
{
  // First calculate the current interpolated color value
  int red   = (CHANNEL_MAX*previousColor[RED]*(lerpsteps-lerpindex)   + CHANNEL_MAX*currentColor[RED]*lerpindex)   / lerpsteps;
  int green = (CHANNEL_MAX*previousColor[GREEN]*(lerpsteps-lerpindex) + CHANNEL_MAX*currentColor[GREEN]*lerpindex) / lerpsteps;
  int blue  = (CHANNEL_MAX*previousColor[BLUE]*(lerpsteps-lerpindex)  + CHANNEL_MAX*currentColor[BLUE]*lerpindex)  / lerpsteps;
  /*
 // Disbaled debug output
   if (false)
   {
   Serial.print("RGB=(");
   Serial.print(red);
   Serial.print(", ");
   Serial.print(green);
   Serial.print(", ");
   Serial.print(blue);
   Serial.println(")");
   }
   */
  // Now write the next RGB value, which will shift the colors
  // down along the other LEDs
  WriteChannel(red);
  WriteChannel(green);
  WriteChannel(blue);

  // Finally latch in the new color values
  digitalWriteFast2(XLAT_PIN, HIGH);
  _delay_ms(1);
  digitalWriteFast2(XLAT_PIN, LOW);
}



void loop()
{
  // Step the interpolation by one frame
  ++lerpindex;
 
  if (lerpindex > lerpsteps)
  {
    // We finished the interpolation between the current
    // color pair. Set up the next color pair.
    NextColor();
  }

  // Update the LEDs
  WriteColors();

  // Throttle the animation
   //_delay_ms(stepdelay);
}

