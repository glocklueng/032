/*********************************************************************
**  Device: AD9850/9851                                             **
**  File:   EF_AD9850_Parallel.pde                                    **
**								    **
**  Created by ElecFreaks Robi.W /28 Oct 2011                       **
**                                                                  **
**  Description:                                                    **
**  This file is a sample code for your reference.About AD9850/9851 **
**  module use on Arduino with Parallel  mode. And then show the       **
**  square wave and sine wave by LXARDOSCOPE which is an free       **
**  Arduino based oscilloscope Linux, using the Xforms library.     **
**                                                                  **
**  This demo code is free software; you can redistribute it and/or **
**  modify it under the terms of the GNU Lesser General Public	    **
**  License as published by the Free Software Foundation; either    **	
**  version 2.1 of the License, or (at your option)   		    **
**  any later version.						    **
**                                                                  **
**  Copyright (C) 2011 ElecFreaks Corp.                     	    **
**                                                                  **
**  http://www.elecfreaks.com                                       **
*********************************************************************/

//Define for AD9850        
#define REST      11
#define FQUP      10
#define CLK        9

#define BitData_Port PORTD
#define BitData_DIR  DDRD
#define BitData_IN   PIND

//Define for LXARDOSCOPE
int sensorValue = 0;        // value read from the pot
byte lb;
byte hb;

void AD9850_Init(){
   pinMode(REST, OUTPUT);
   pinMode(FQUP, OUTPUT);
   pinMode(CLK , OUTPUT);
   digitalWrite(REST, 0);
   digitalWrite(FQUP, 0);
   digitalWrite(CLK, 0);

   BitData_DIR =  0xFF;    // set IO OUTPUT
   BitData_Port = 0x0;     // init IO

}

void AD9850_Reset(){
  digitalWrite(CLK, 0);
  digitalWrite(FQUP, 0);
  //Reset signal
  digitalWrite(REST, 0);
  digitalWrite(REST, 1);
  digitalWrite(REST, 0);
}

void AD9850_WR(unsigned char w0,double frequence){
  unsigned char i,w;
  long int y;
  double x;
  
  //Calculate the frequency of the HEX value
  x=4294967295/125;//Suitable for 125M Crystal 
  frequence=frequence/1000000;
  frequence=frequence*x;
  y=frequence;
  //write w0
  w = w0;
  BitData_Port = w;
  digitalWrite(CLK, 1);
  digitalWrite(CLK, 0);
  //write w1
  w=(y>>24);
  BitData_Port = w;
  digitalWrite(CLK, 1);
  digitalWrite(CLK, 0);
  //write w2
  w=(y>>16);
  BitData_Port = w;
  digitalWrite(CLK, 1);
  digitalWrite(CLK, 0);
  //write w3
  w=(y>>8);
  BitData_Port = w;
  digitalWrite(CLK, 1);
  digitalWrite(CLK, 0);
  //write w1
  w=(y>>=0);
  BitData_Port = w;
  digitalWrite(CLK, 1);
  digitalWrite(CLK, 0);
  
  //disable input
  digitalWrite(FQUP, 1);
  digitalWrite(FQUP, 0);
}


void setup(){
  AD9850_Init();
  AD9850_Reset();
  AD9850_WR(0x00, 200); //500Hz
  // initialize serial communications at 115200 bps:
  Serial.begin(115200); 
}

void loop(){
  // read A0:
  sensorValue = analogRead(A0);            
// shift sample by 3 bits, and select higher byte  
  hb=highByte(sensorValue<<3); 
// set 3 most significant bits and send out
  Serial.print(hb|224,BYTE); 
// select lower byte and clear 3 most significant bits
  lb=(lowByte(sensorValue))&31;
// set bits 5 and 6 and send out
  Serial.print(lb|96,BYTE);
// read A1
  sensorValue = analogRead(A1);            
// shift sample by 3 bits, and select higher byte 
  hb=highByte(sensorValue<<3); 
// set bits 5 and 6 and send out
  Serial.print(hb|96,BYTE); 
// select lower byte and clear 3 most significant bits
  lb=(lowByte(sensorValue))&31;
// set bits 5 and 6 and send out
  Serial.print(lb|96,BYTE);
}
