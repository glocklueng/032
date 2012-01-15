#include <LEDStrip.h>

#define SIPIN 9
#define DIPIN 3
#define CLKPIN 4
#define LATCHPIN 5

#include "WProgram.h"
void setup();
void dosPulse();
void docolor(uint8_t red, uint8_t green, uint8_t blue);
void docolor(uint8_t red, uint8_t green, uint8_t blue, uint16_t n, uint8_t speed);
void loop();
LEDStrip mystrip(DIPIN,SIPIN,LATCHPIN,CLKPIN);

#define DELAY 0
#define SPULSES 4

void setup()
{
  Serial.begin(38400); 
}  

void dosPulse()
{
  uint8_t i;

  for (i=0; i < SPULSES; i++) {
    mystrip.sPulse();
  } 
}

void docolor(uint8_t red, uint8_t green, uint8_t blue)
{
  docolor(red, green,blue, (128/SPULSES) - 1, 1);
}

void docolor(uint8_t red, uint8_t green, uint8_t blue, uint16_t n, uint8_t speed)
{
  uint8_t i;

  if (speed == 1)
    mystrip.rgbPush(red,green,blue);
  else
    mystrip.rgbPush2X(red,green,blue);
  mystrip.latch();
  dosPulse();
 
  for (i=0; i< n; i++) {
    mystrip.blankPush();
    mystrip.latch();
    dosPulse();
    //delay(DELAY);
  }
  
  
}


void loop()
{

  docolor(UP,OFF,OFF,128/(2*SPULSES),2);
  docolor(ON,OFF,OFF,128/(2*SPULSES),1); 
  docolor(ON,UP,OFF,128/(2*SPULSES),2);
  docolor(ON,ON,OFF,128/(4*SPULSES),1);
  docolor(DOWN,ON,OFF,128/(2*SPULSES),2);
  docolor(OFF,ON,OFF,128/(2*SPULSES),1); 
  docolor(OFF,ON,UP,128/(2*SPULSES),2);
  docolor(OFF,ON,ON,128/(4*SPULSES),1);
  docolor(OFF,DOWN,ON,128/(2*SPULSES),2);
  docolor(OFF,OFF,ON,128/(2*SPULSES),1); 
  docolor(OFF,OFF,DOWN,128/(2*SPULSES),2);
  docolor(OFF,OFF,OFF,128/(4*SPULSES),1); 

}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

