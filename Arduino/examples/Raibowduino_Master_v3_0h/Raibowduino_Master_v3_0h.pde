#include <stdio.h>
#include <Wire.h>
#include <avr/pgmspace.h>

#include "WireCommands.h"

void sendCMD(byte address, byte CMD, ... );

unsigned char RainbowCMD[20];
unsigned char State = 0;  
unsigned long timeout;

byte remoteAddr = 0x10;

void setup() {
  Wire.begin();    // join I2C bus (address optional for master)
 // Serial.begin(9600);
  
  RainbowCMD[0] = 'r';
  
  sendCMD(0x10, CMD_SET_PAPER, 0, 0, 0);
}

void loop() {     // Do some test with Rainbowduino
  sendCMD(0x10, CMD_CLEAR_PAPER);

  sendCMD(0x10, CMD_SET_INK, 0xD, 0xF, 0);
  sendCMD(0x10, CMD_DRAW_SQUARE, toByte(2), toByte(2), toByte(6), toByte(6));

  sendCMD(0x10, CMD_SET_INK, 0xF, 0x0, 0);
  sendCMD(0x10, CMD_DRAW_LINE,  toByte(2), toByte(2), toByte(6), toByte(6));
  sendCMD(0x10, CMD_DRAW_LINE,  toByte(2), toByte(6), toByte(6), toByte(2));

  sendCMD(0x10, CMD_SET_INK, 0x0, 0x0, 15);
  sendCMD(0x10, CMD_DRAW_PIXEL, toByte(8), toByte(0));
  sendCMD(0x10, CMD_PRINT_CHAR, toByte(1), toByte(1), random('1', '9'+1));
  sendCMD(0x10, CMD_SWAP_BUF);
  
  delay(500);

  byte k = 0;
  for (k = 0; k < 255; k++) {
    sendCMD(0x10, CMD_CLEAR_PAPER);
    sendCMD(0x10, CMD_COPY_FRONT_BUF, toByte(0), toByte(-1));
    sendCMD(0x10, CMD_DRAW_ROW_MASK, toByte(7), toByte(0), k);
    sendCMD(0x10, CMD_SWAP_BUF);
  }
}

void sendCMD(byte address, byte CMD, ... ) {
  int i;
  unsigned char v;
  byte t;
  va_list args;                     // Create a variable argument list
  va_start(args, CMD);              // Initialize the list using the pointer of the variable next to CMD;
  
  RainbowCMD[1] = CMD;              // Stores the command name
  t = pgm_read_byte(&(CMD_totalArgs[CMD]))+2;  // Retrieve the number of arguments for the command
  for (i=2; i < t; i++) {
    v = va_arg(args, int);          // Retrieve the argument from the va_list    
    RainbowCMD[i] = v;              // Store the argument
  }
  
  sendWireCommand(address, t);      // Transmit the command via I2C
}

unsigned char toByte(int i) {
  return map(i, -128, 127, 0, 255);
}

// ### The following lines are adapted from the original code ###

void sendWireCommand(int Add, byte len) {
  unsigned char OK=0;
  unsigned char i,temp;
  
  while(!OK)
  {                          
    switch (State)
    { 	

    case 0:                          
      Wire.beginTransmission(Add);
      for (i=0; i<len ;i++) Wire.send(RainbowCMD[i]);
      Wire.endTransmission();    
      delay(5);   
      State=1;                      
      break;

    case 1:
      Wire.requestFrom(Add,1);   
      if (Wire.available()>0) 
        temp=Wire.receive();    
      else {
        temp=0xFF;
        timeout++;
      }

      if ((temp==1)||(temp==2)) State=2;
      else if (temp==0) State=0;

      if (timeout>5000) {
        timeout=0;
        State=0;
      }

      delay(5);
      break;

    case 2:
      OK=1;
      State=0;
      break;

    default:
      State=0;
      break;
    }
  }
}
