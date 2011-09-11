
/*
     __  __           ___    ___       ____                                        __                __                
 /\ \/\ \         /\_ \  /\_ \     /\  _`\                                     /\ \              /\ \               
 \ \ `\\ \  __  __\//\ \ \//\ \    \ \,\L\_\  _____      __      ___     __    \ \ \         __  \ \ \____    ____  
 \ \ , ` \/\ \/\ \ \ \ \  \ \ \    \/_\__ \ /\ '__`\  /'__`\   /'___\ /'__`\   \ \ \  __  /'__`\ \ \ '__`\  /',__\ 
 \ \ \`\ \ \ \_\ \ \_\ \_ \_\ \_    /\ \L\ \ \ \L\ \/\ \L\.\_/\ \__//\  __/    \ \ \L\ \/\ \L\.\_\ \ \L\ \/\__, `\
 \ \_\ \_\ \____/ /\____\/\____\   \ `\____\ \ ,__/\ \__/.\_\ \____\ \____\    \ \____/\ \__/.\_\\ \_,__/\/\____/
 \/_/\/_/\/___/  \/____/\/____/    \/_____/\ \ \/  \/__/\/_/\/____/\/____/     \/___/  \/__/\/_/ \/___/  \/___/ 
 \ \_\                                                               
 \/_/                                      
 
 
 88888888ba   88              88                     88888888ba                                       
 88      "8b  ""              88                     88      "8b                                      
 88      ,8P                  88                     88      ,8P                                      
 88aaaaaa8P'  88   ,adPPYba,  88   ,d8   ,adPPYba,   88aaaaaa8P'   ,adPPYba,  ,adPPYYba,  8b,dPPYba,  
 88""""""'    88  a8"     ""  88 ,a8"   a8"     "8a  88""""""8b,  a8P_____88  ""     `Y8  88P'   "Y8  
 88           88  8b          8888[     8b       d8  88      `8b  8PP"""""""  ,adPPPPP88  88          
 88           88  "8a,   ,aa  88`"Yba,  "8a,   ,a8"  88      a8P  "8b,   ,aa  88,    ,88  88          
 88           88   `"Ybbd8"'  88   `Y8a  `"YbbdP"'   88888888P"    `"Ybbd8"'  `"8bbdP"Y8  88        
 */

#include <JukiStepper.h>

// for the watchdog based reset ( which didn't seem to work)
#include <avr/io.h>
#include <avr/wdt.h>

// xy lIMITS these should be now on pins capable of hardware interrupts in arduino
#define XL1 ( 18 )
#define XL2 ( 19 ) 
#define YL2 ( 20 ) 
#define YL1 ( 21 ) 

#define XL3 ( 24 ) 
#define YL3 ( 29 ) 
#define XL4 ( 25 ) 
#define YL4 ( 40 ) 

// x/y home
#define XHM     ( 26 ) 
#define YHM      ( 41 ) 

// these two seem mixed up
#define HEADDN     ( 37 )
#define DEG90      ( 36 )


#define TACSENSE   ( 38 )
#define PCBDETECTOR ( 39 )
#define PCBIN ( 5 )
#define PCBOUT  ( 6 )
#define PINUP ( 7 )
#define READYIN ( 8 )
#define DISPENSEREND ( 9 )
#define MORG ( 11 )
#define MCENT ( 12 )

#define CWSW ( 13 )
#define CCWSW ( 35 )

#define XPLUS  ( 4 )
#define XMINUS ( 3 )
#define YPLUS  ( 2  )
#define YMINUS ( 14 )


#define FAST ( 15 )
#define TEACH ( 16 )
//switch on touch pad
#define T_VAC ( 17 )
// switch on touch pad
#define T_HEAD ( 33 )

#define SPAREA ( 32 )
#define SPAREB ( 34 )
#define SPAREC ( 10 )

#define SPARE7 ( 53 )
#define SPARE8 ( A12 )
#define SPOT ( 52 )
#define VCHANGE ( 51 )
#define DISPENSERSTART ( 50 )
#define CVYM ( 49 )
#define STOPPER ( 48 )
#define LOCATOR ( 47 )
#define SUPPORTER ( 46 )
#define VAC ( 45 ) 
#define HEAD ( 44 ) 
#define ROT (43 ) 
#define CENTERING (42  )
#define TAPEKNOCK ( A15 )
#define SPARE6 ( A14 )

#define ACENT ( A13 )
#define SPARE1 ( A7 )
#define SPARE2 ( A8 )
#define SPARE3 ( A9 )
#define SPARE4 ( A10 )
#define SPARE5 ( A11 )
#define READYOUT ( A6 )
#define CW_CCW ( A5 )
#define PULSE ( A4 )

// motors
#define YCW ( A2 )
#define YCCW ( A3 )
#define XCW ( A0 )
#define XCCW ( A1 )


// This should cause a proper reset it doesn't.
#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}

// Maximum number of pulses before machine has hit a hard limit
#define MAX_X_PULSES ( 14868L )
#define MAX_Y_PULSES ( 21172L )


// motor speed pulses
// motors need |__

unsigned short DELAY_X1 =(150*2); // 148 lowest (maybe 150)
unsigned short DELAY_X2 =(150);
unsigned short DELAY_Y1 =(208*2); //200 lowest
unsigned short DELAY_Y2 =(208);

// 500 pulses = 25mm
// 100 pulses = 5mm
// 7431 pulses (left to right) = 370mm
// 10597 pulses for the Y table (limit to limit ) = 529.85

// 1/2 mode
// 21171 Y 
// 14867 X

// maximum speed the motors can travel at
// 100 is the maxiumum speed without ramping
int X_SPEED = ( 100 );
int Y_SPEED = ( 110 );

// length of pulse sent to motor controller
#define SHORT_X_PULSE ( 1 )
#define SHORT_Y_PULSE ( 1 )

// define the stepper motors
JukiStepper m_StepperX(25*40,DELAY_X2,XCW,XCCW);
JukiStepper m_StepperY(25*40,DELAY_Y2,YCW,YCCW);

void setupLimits(void )
{
  digitalWrite(XL1,HIGH);
  digitalWrite(XL2,HIGH);
  digitalWrite(XL3,HIGH);
  digitalWrite(XL4,HIGH);

  digitalWrite(YL1,HIGH);
  digitalWrite(YL2,HIGH);
  digitalWrite(YL3,HIGH);
  digitalWrite(YL4,HIGH);
}

void setup( void ) {

  // Set all pins as input
  for( int i = 0; i < 54 ; i ++ ) {

    pinMode(i, INPUT);   
  }

  Serial.begin(9600);

  Serial.println("PickoBear Test\n");

  //Turn on limit switches
  setupLimits();

  m_StepperX.setDirection( RS_FORWARD ) ;
  
  while( digitalRead( XL2 ) == false) {
    m_StepperX.pulse();
  }
  
}
boolean readLimits ( void ) {
  
  if( digitalRead( XL1 ) ) return true;
  if( digitalRead( XL2 ) ) return true;
  if( digitalRead( XL1 ) ) return true;
  if( digitalRead( YL2 ) ) return true;

  return false;
}

void loop(void)
{
  m_StepperX.setDirection( RS_REVERSE ) ;
  m_StepperY.setDirection( RS_REVERSE ) ;
  
  for( int i =0  ; i < 12000 ; i++  )
  {
    m_StepperX.pulse();
  }
  delay(100);
  
  m_StepperX.setDirection( RS_FORWARD ) ;
  m_StepperY.setDirection( RS_FORWARD ) ;

  for( int i =0  ; i < 12000 ; i++  )
  {
    m_StepperX.pulse();
  }

  delay(100);
  char sbyte;

  while (Serial.available()   ) {

    // read the incoming byte:
    char sbyte = Serial.read();

    switch( sbyte ) {
       case 'a':
         if(X_SPEED > 2  ) 
           X_SPEED--;
         break;
       
       case 's':
         X_SPEED++;
         break;
      case 'r':
        X_SPEED= 100;
        Y_SPEED= 100;
        break;
        
      case 'i':
        Serial.println("x,y");
        Serial.print(X_SPEED,DEC);
        Serial.print(" ");
        Serial.println(Y_SPEED,DEC);
        break; 
    }
  
    m_StepperX.setDelay( X_SPEED);
  }
  
}

