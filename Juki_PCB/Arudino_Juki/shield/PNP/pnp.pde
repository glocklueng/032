
/*
    _   ____  ____    __   _____ ____  ___   ______________    ___    ____ _____
   / | / / / / / /   / /  / ___// __ \/   | / ____/ ____/ /   /   |  / __ ) ___/
  /  |/ / / / / /   / /   \__ \/ /_/ / /| |/ /   / __/ / /   / /| | / __  \__ \ 
 / /|  / /_/ / /___/ /______/ / ____/ ___ / /___/ /___/ /___/ ___ |/ /_/ /__/ / 
/_/ |_/\____/_____/_____/____/_/   /_/  |_\____/_____/_____/_/  |_/_____/____/  
                                                                                
 
 
 88888888ba   88              88                     88888888ba                                       
 88      "8b  ""              88                     88      "8b                                      
 88      ,8P                  88                     88      ,8P                                      
 88aaaaaa8P'  88   ,adPPYba,  88   ,d8   ,adPPYba,   88aaaaaa8P'   ,adPPYba,  ,adPPYYba,  8b,dPPYba,  
 88""""""'    88  a8"     ""  88 ,a8"   a8"     "8a  88""""""8b,  a8P_____88  ""     `Y8  88P'   "Y8  
 88           88  8b          8888[     8b       d8  88      `8b  8PP"""""""  ,adPPPPP88  88          
 88           88  "8a,   ,aa  88`"Yba,  "8a,   ,a8"  88      a8P  "8b,   ,aa  88,    ,88  88          
 88           88   `"Ybbd8"'  88   `Y8a  `"YbbdP"'   88888888P"    `"Ybbd8"'  `"8bbdP"Y8  88        
 
 */

// for the watchdog based reset ( which didn't seem to work)
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/delay.h>

// Arduino specific
#include <JukiStepper.h>
#include <Time.h>

// Pickobear includes
#include "shieldpins.h"

#define 	WDTO_8S   ( 9 )
#define         NDEBUG    ( 1 )

// Maximum number of pulses before machine has hit a hard limit
#define MAX_X_PULSES       ( 14868L )
#define MAX_Y_PULSES       ( 21172L )

// maximum speed the motors can travel at
int X_SPEED =( 100 );
int Y_SPEED =( 110 );

// for diagonal move
int X2_SPEED =( 40 );
int Y2_SPEED =( 40 );

// length of pulse sent to motor controller
#define SHORT_X_PULSE ( 1 )
#define SHORT_Y_PULSE ( 1 )

// Limit switches
volatile boolean xLimit1=false,xLimit2=false,xLimit3=false,yLimit1=false,yLimit2=false,yLimit3=false;

/// Spot light on/off
unsigned char spotState = 0;
unsigned char headState = 0;
unsigned char vacuumState = 0;

// State of the keys on the keypad
boolean xPlus=false,xMinus=false,yMinus=false,yPlus=false,fastSwitch=false,teachSwitch=false,lastTeach=false,lastVac=false,lastHead=false,vacSwitch=false,headSwitch=false;

// home sensors (set if currently in home position for each axis)
boolean xHome=false,yHome=false;

// motor speed pulses
// motors need |__

unsigned short DELAY_X1 =(150*2); // 148 lowest (maybe 150)
unsigned short DELAY_X2 =(150*2);
unsigned short DELAY_Y1 =(208*2); //200 lowest
unsigned short DELAY_Y2 =(208*2);

// 500 pulses = 25mm
// 100 pulses = 5mm
// 7431 pulses (left to right) = 370mm
// 10597 pulses for the Y table (limit to limit ) = 529.85

// 1/2 mode
// 21171 Y 
// 14867 X

// global count of pulses
long gXPulses = 0;
long gYPulses = 0;

// current X,Y position of head in micrometers
//.long gCurrentXum = 0;
long gCurrentYum = 0;

// machine homed ?
volatile boolean homed = false;

// last x,y
static long lx=0,ly=0;

JukiStepper m_Stepper(25*40,DELAY_X2,XCCW,XCW);

// This should cause a proper reset it doesn't.
#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}

/////////////////////////////////////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////////////////////////////////////

void setup( void ) 
{

  //  wdt_enable( WDTO_8S );

  homed = false;

  // Set all pins as input
  for( int i = 0; i < 54 ; i ++ ) {

    pinMode(i, INPUT);   
  }

  // Serial port init
  Serial.begin(9600);

  Serial.println("PickoBear initialise\n");

  // set pullups for sensors and switches
  digitalWrite(XPLUS,HIGH);
  digitalWrite(YPLUS,HIGH);
  digitalWrite(XMINUS,HIGH);
  digitalWrite(YMINUS,HIGH);
  digitalWrite(T_VAC,HIGH);
  digitalWrite(MORG,HIGH);
  digitalWrite(T_HEAD,HIGH);
  digitalWrite(TEACH,HIGH);

  digitalWrite(TACSENSE,HIGH);
  digitalWrite(FAST,HIGH);

  digitalWrite(XL1,HIGH);
  digitalWrite(XL2,HIGH);
  digitalWrite(XL3,HIGH);
  digitalWrite(XL4,HIGH);

  digitalWrite(YL1,HIGH);
  digitalWrite(YL2,HIGH);
  digitalWrite(YL3,HIGH);
  digitalWrite(YL4,HIGH);
  digitalWrite(DEG90,HIGH);

  digitalWrite(XHM,HIGH);
  digitalWrite(YHM,HIGH);

  digitalWrite(HEADDN,HIGH);

  // set outputs for servos and solenoids etc
  pinMode(TAPEKNOCK,OUTPUT);
  digitalWrite(TAPEKNOCK,HIGH);

  pinMode(HEAD,OUTPUT);
  digitalWrite(HEAD,HIGH);

  pinMode(ROT,OUTPUT);
  digitalWrite(ROT,HIGH);

  pinMode(CENTERING,OUTPUT);
  digitalWrite(CENTERING,HIGH);

  pinMode(XCW,OUTPUT);
  digitalWrite(XCW,HIGH);

  pinMode(XCCW,OUTPUT);
  digitalWrite(XCCW,HIGH);

  pinMode(YCW,OUTPUT);
  digitalWrite(YCW,HIGH);

  pinMode(YCCW,OUTPUT);
  digitalWrite(YCCW,HIGH);

  pinMode(SPARE1,OUTPUT);
  digitalWrite(SPARE1,HIGH);

  pinMode(SPARE2,OUTPUT);
  digitalWrite(SPARE2,HIGH);

  pinMode(SPARE3,OUTPUT);
  digitalWrite(SPARE3,HIGH);

  pinMode(SPARE4,OUTPUT);
  digitalWrite(SPARE4,HIGH);

  pinMode(SPARE5,OUTPUT);
  digitalWrite(SPARE5,HIGH);

  pinMode(SPARE6,OUTPUT);
  digitalWrite(SPARE6,HIGH);

  pinMode(SPOT,OUTPUT);
  digitalWrite(SPOT,HIGH);

  pinMode(VAC,OUTPUT);
  digitalWrite(VAC,HIGH);

#if 1
  // setup the limit switches

  //These pins can be configured to trigger an interrupt on a low value, a rising or falling edge, or a change in value. See the attach nterrupt() function for details.
  //External Interrupts: 2 (interrupt 0), 3 (interrupt 1), 18 (interrupt 5), 19 (interrupt 4), 20 (interrupt 3), and 21 (interrupt 2).

  attachInterrupt(2, y1Limit, CHANGE );
  attachInterrupt(3, y2Limit, CHANGE );
  attachInterrupt(4, x2Limit, CHANGE );
  attachInterrupt(5, x1Limit, CHANGE );

#endif

    xLimit1 = digitalRead( XL1 );
    yLimit1 = digitalRead( YL1 );
    xLimit2 = digitalRead( XL2 );
    yLimit2 = digitalRead( YL2 );
}



// called when limit switches are hit

//            YL2
// ---------------------------
// |                         |
// |                         |
// |                         |
// |                         |
// |                         |
// |                         |
// |                         |
// XL1---------------------XL2
//             YL1

void x1Limit( void ) 
{
  homed = false;
  xLimit1 = digitalRead( XL1 );
}

void x2Limit( void ) 
{
  homed = false;
  xLimit2 = digitalRead( XL2 );
}

void y1Limit( void ) 
{
  homed = false;
  yLimit1 = digitalRead( YL1 );
}

void y2Limit( void ) 
{
  homed = false;
  yLimit2 = digitalRead( YL2 );
}

//
// read the front panel switches (on the beta board yMinus is mapped to the arudinos serial port)
// not sure if this should process things other than buttons.
//
void readPanel( void )
{
  // number of pulses to move by 
  unsigned char fspeed = 1;

  // read state of cursor keys
  xPlus  =  !digitalRead( XPLUS );
  xMinus =  !digitalRead( XMINUS );
  yPlus  =  !digitalRead( YPLUS );
  yMinus =  !digitalRead( YMINUS );

  fastSwitch    = !digitalRead( FAST );
  teachSwitch   = !digitalRead( TEACH );
  vacSwitch     = !digitalRead( T_VAC );
  headSwitch    = !digitalRead( T_HEAD ) ;

  // don't allow both
  if( xPlus == true && xMinus == true ) {
    xMinus = false;
  }

  // don't allow both
  if( yPlus == true && yMinus == true ) {
    yMinus = false;
  }

  // fast mode
  if( fastSwitch == true ) {
    // make it go 500 pulses
    fspeed = 500;
  }

  if( !digitalRead( MORG ) == true && fastSwitch  ) {
    park();
    return;
  } 
  else
    // Read the home key, and home machine if set
    if( !digitalRead( MORG ) == true   ) {
      home();
    }

  // if teach is held down, toggle spot light 
  if( teachSwitch == true ) {

    if( lastTeach != true ) {
      // toggle state
      spotState = 1 - spotState;
      spot(spotState);
    }
  }

  // remember last setting
  lastTeach = teachSwitch;

  // if vac is held down, toggle vacuum
  if( vacSwitch == true ) {    
    vacuum(1);
    // remember last setting
    lastVac = true;

  } 
  else if ( lastVac == true ) {
    vacuum(0 ) ;
    lastVac = false;
  }

  // if head is held down, toggle head
  if( headSwitch == true ) {

    // toggle state
    lastHead = true;
    head(1);

  } 
  else if (lastHead == true ) {
    head(0);
    lastHead = false;
  }


  // handle cursor keys
  if( xPlus  == true ) {
    if(!fastSwitch)delay(100);
    goright(fspeed,1);
    //DELAY_X2 ++;
  }
  if( xMinus == true ) {
    if(!fastSwitch)delay(100);
    goleft(fspeed,1);
    //DELAY_X2 --;
  }

  if( yPlus  == true ) {
    if(!fastSwitch)delay(100);
    goback(fspeed,1);
    //DELAY_Y2 ++;
  }

  if( yMinus == true ) {
    if(!fastSwitch)delay(100);
    goforward(fspeed,1);
    //DELAY_Y2 --;
  }
}

bool tacSense(void) {
  return !digitalRead( TACSENSE) ;
}

// limits should be set by interrupts now
unsigned int readLimitSwitches( void ) 
{
  unsigned char  limit;
  //  xLimit1 = digitalRead( XL1 );
  //  yLimit1 = digitalRead( YL1 );
  //  xLimit2 = digitalRead( XL2 );
  // yLimit2 = digitalRead( YL2 );

  limit  = xLimit1;
  limit += xLimit2;
  limit += xLimit3;

  limit += yLimit1;
  limit += yLimit2;
  limit += yLimit3;

  // not counted as a limit switches, since it'll upset the home
  xHome =  digitalRead( XHM ); 
  yHome =  digitalRead( YHM ); 

  // debug code

  Serial.println("Limit");
  Serial.println("123123XY");
  Serial.print(xLimit1,HEX);
  Serial.print(xLimit2,HEX);
  Serial.print(xLimit3,HEX);
  Serial.print(yLimit1,HEX);
  Serial.print(yLimit2,HEX);
  Serial.print(yLimit3,HEX);
  Serial.print(xHome,HEX);
  Serial.print(yHome,HEX);
  Serial.println("");
  Serial.print ("gXPulses, gYPulses = ");
  Serial.print(gXPulses,DEC);
  Serial.print(",");
  Serial.println(gYPulses,DEC);

  // update the global position in micrometers
  gCurrentXum = pulsestoum( gXPulses ) ;
  gCurrentYum = pulsestoum( gYPulses ) ;


  Serial.print("cxmicrom,cymicrom = ");
  Serial.print(gCurrentXum);
  Serial.print(",");
  Serial.println(gCurrentYum);

  // if any of the limit switches are reached, machine is no longer considered to be homed
  // set in interrupts
  if( limit ) {

    Serial.println("out of home!!");

    // limit switch interrupt will update this, but just in case.
    homed = false;
  }

  // let caller know if a limit was hit.
  return limit;
}
/*

 spare2 = 1 air on/off
 
 spare 3,4,5 
 345
 000 = tool 1
 001 = tool 2
 010 = tool 3
 011 = tool 4 
 100 = tool 5
 101 = tool 6
 */

void testPad( void ) 
{
  move2(pulsestoum(12308),pulsestoum(5002));
}

// check to see if head has a tool
boolean  hastool( void )
{
  boolean status = false;

  // set the head to be up
  head( 0 );


  // move to the test pad
  testPad();
  delay(100);
  // head down
  head(1);
  delay(100);

  //vacuum on
  vacuum(1);
  delay(500);

  // test the vacuum
  status = tacSense();

  // vacuum off
  vacuum(0);
  // head up
  head(0);

  return status;
}

boolean putdown(void )
{
#ifndef NDEBUG
  Serial.println("putdown");
#endif

  // head down wait
  head(1);
  delay(600);

  // toolchanger up
  digitalWrite(SPARE2,LOW);
  //wait
  delay(1000);
  // toolchanger down , wait for lock        
  digitalWrite(SPARE2,HIGH);
  delay(1000);
  //head up
  head(0); 

  if( hastool() == true ) {
#ifndef NDEBUG
    Serial.println("Failed to drop tool");
#endif
    return false;
  }
  return true;
}

boolean pickup(void )
{
#ifndef NDEBUG
  Serial.println("pickup");
#endif

  // head down wait
  //00:14:11
  head(1);
  //14:12 its down
  delay(600);
  //0:14:15

  // toolchanger up
  digitalWrite(SPARE2,LOW);
  //wait
  delay(1000);
  //00:15:18
  //head up
  head(0); 
  delay(1000);
  // toolchanger down         
  digitalWrite(SPARE2,HIGH);
  delay(300);

  if( hastool() == false ) {

    Serial.println("Failed to pickup tool");
    return false;
  }
  return true;
}

/* handle the tool changer */

void tooloff( void )
{
  digitalWrite(SPARE2,HIGH);
}

void tool( unsigned char on_off, unsigned char toolid ) 
{
  boolean pick = false;

  head(0);

  if( hastool() == true ) {
    pick = true;
  }

#ifndef NDEBUG
  Serial.print("Tool change ");
  Serial.println(toolid,DEC);
#endif
  switch( toolid ) {
  case 0 :

    digitalWrite(SPARE3,HIGH);  
    ;
    digitalWrite(SPARE4,HIGH);  
    ;
    digitalWrite(SPARE5,HIGH);  
    ;          
    break;
  case 1 :


    //13077 4657
    // move to toolchanger position, bring up tool change.
    move2(pulsestoum(13077),pulsestoum(4657));

    delay(200);

    digitalWrite(SPARE3,LOW);  
    digitalWrite(SPARE4,LOW);  
    digitalWrite(SPARE5,LOW);  

    if(pick) 
      putdown();
    else
      pickup();

    break;
  case 2 :
    move2(pulsestoum(13077),pulsestoum(3684));
    delay(200);

    if(pick) 
      putdown();
    else
      pickup();

    digitalWrite(SPARE3,LOW);  
    digitalWrite(SPARE4,LOW);  
    digitalWrite(SPARE5,HIGH);  

    break;
  case 3 :
    digitalWrite(SPARE3,LOW);  
    digitalWrite(SPARE4,HIGH);  
    digitalWrite(SPARE5,LOW);  
    break;
  case 4 :
    digitalWrite(SPARE3,LOW);  
    digitalWrite(SPARE4,HIGH);  
    digitalWrite(SPARE5,HIGH);  
    break;
  case 5 :
    digitalWrite(SPARE3,HIGH);  
    digitalWrite(SPARE4,LOW);  
    digitalWrite(SPARE5,LOW);  
    break;
  case 6 :
    digitalWrite(SPARE3,HIGH);  
    digitalWrite(SPARE4,LOW);  
    digitalWrite(SPARE5,HIGH);  
    break;
  }

  digitalWrite(SPARE2,HIGH);
}

// get the state of the 90 degree roration?
boolean degree90( void ) 
{
  return !digitalRead(DEG90);
}
void parkleft(void) { 
  move2( 
  0,
  517000
    );
}
void parkforward(void) { 
  move2( 
  364600,
  0
    );
}
void gohome(void) { 
  move2( 
  0,
  0
    );
}

void park(void) { 
  move2( 
  364000,
  517000
    );
}


void vacuum( boolean on_off ) 
{
  static unsigned char lastState = 0xff;

  /// don't change if its the same
  if( lastState == on_off ) {
    return;
  }

  lastState = on_off;

#ifndef NDEBUG
  Serial.println("Vacuum");
#endif

  if( on_off ) { 
    digitalWrite(VAC,LOW);
  } 
  else {
    digitalWrite(VAC,HIGH);
  }   
}

// return the status of the head
// o down
// 1 up
boolean headDown( void ) 
{
  return digitalRead( HEADDN ) ?1:0;
} 

long startTime;
long CPH =0,avgCPH = 0,avgCount=0;

void xtoolchange(void)
{
  int dpart = 200;

  startTime = (minute()*60)+second();


  // move to toolchange
  //  move2(0,pulsestoum(12299));
knock:;
  // move to feeder one
  //  move2(pulsestoum(34),pulsestoum(12271));
  // move to feeder three
  //move2(pulsestoum(34),pulsestoum(14823));

  move2(pulsestoum(65),pulsestoum(13215));


  // 
  tapeKnock(1);
  delay(100);
  tapeKnock(0);
  delay(100);
  head( 1 );
  delay(150);
  vacuum(1);
  delay(200);
  head( 0 );
  delay(200);

  if( tacSense() == true ) 
  {
    vacuum(0);

    Serial.println("Failed to pick up part");

    delay(dpart);

    dpart += 2000;

    goto knock;
  }
}
long endTime;

void xend(void)
{
  head(1);
  delay(100);
  vacuum(0);
  // hold on part
  delay(200);
  head(0);

  endTime = (minute()*60)+second();

  Serial.print("took seconds = ");
  Serial.println(endTime - startTime ,DEC);
  Serial.print("CPH = ");
  CPH =   3600 / (endTime-startTime );
  Serial.println(CPH,DEC);
  avgCPH += CPH; 
  avgCount ++;
  Serial.print("Average CPH = ");
  Serial.println(avgCPH/avgCount ,DEC);

}

boolean testpart(void )
{
  return true;

  if( tacSense() == false ) 
  {

    move2(pulsestoum(0),pulsestoum(0));

    vacuum(0);

    delay(10000);

    return false;
  }
  return true;
}

void loop() 
{
  char sbyte;

#if 0
  Serial.print (xLimit1,DEC);
  Serial.print (xLimit2,DEC);
  Serial.print (yLimit1,DEC);
  Serial.print (yLimit2,DEC);
  Serial.println (homed,DEC);

  return;
#endif

  // handle watchdog
  //   wdt_reset();

  if (Serial.available() > 0) {

    // read the incoming byte:
    sbyte = Serial.read();

    switch( sbyte ) 
    {
    case '*':


      move2(  50000,100000);

      delay(100);

      Serial.print("Start:");
      Serial.print(minute(),DEC);
      Serial.print(":");
      Serial.println(second(),DEC);


      if( hastool() == false ) 
        tool(1,1);

      speakerBadge();

      head(0);
      tool(1,1);

      home();
      Serial.print("End:");
      Serial.print(minute(),DEC);
      Serial.print(":");
      Serial.println(second(),DEC);


      break;


    case 'A':
      movebackright(5000,3000,1);
      delay(20);
      movebackleft(5000,3000,1);
      delay(20);
      moveforwardleft(5000,3000,1);
      delay(20);
      moveforwardright(5000,3000,1);
      delay(20);

      /*
                        movebackright(1000,1000,1);
       
       			move2(0,0);
       			move2( 100000,100000 );
       			move2(0,0);
       */
      break;
    case 'a':
      movebackright(1000,1000,1);
      delay(100);
      break;

      delay(100);

      if( hastool() == false ) 
        tool(1,1);

      for( int i = 0 ; i < 10; i++ ) {
        do {

          xtoolchange();
          move2(50188+60000,181426+(i*2000) );

        } 
        while( testpart() ) ;
        xend();
      }
      for( int i = 0 ; i < 10; i++ ) {
        do {
          xtoolchange();
          move2(50188+65000,181426+(i*2000));
        } 
        while(testpart() );
        xend();
      }

      park();
      break;
    case '-':
      gotoxyp(52,12330);
      tapeKnock(1);
      delay(200);
      tapeKnock(0);
      delay(100);
      vacuum(1);
      delay(100);
      head(1);
      delay(500);
      head(0);
      gotoxyp(6080,8171);
      delay(500);
      head(1);
      delay(500);
      head(0);
      delay(10);
      vacuum(0);
      home();

      break;

    case 't':
      tapeKnock(1);
      delay(500);
      tapeKnock(0);
      break;
    case 'r':
      rotate(true);
      break;
    case 'R':
      rotate(false);
      break;
    case 'c':
      center(true);
      break;          
    case 'C':
      center(false);
      break;          
    case 'h':
      head(0);
      break;
    case 'H':
      head(1);
      break;
    case 'd':
      Serial.println(degree90(),HEX);
      break;
    case 'v':          
      vacuum(0);          
      break;
    case 'V':          
      vacuum(1);          
      break;

    case 'w':
      walk();
      break;

    case 'f':
      goright(1,1);
      break;
    case 'F':
      goleft(1,1);
      break;

    case 'q':
      head(1);
      delay(200);
      center(true);
      delay(200);
      center(false);
      delay(100);
      head(0);
      //        move2(pulsestoum(1560), pulsestoum(5766));
      //            goback(100,1);
      break;
    case 'Q':
      goforward(100,1);
      break;

    case '8':
      goright(500,1);
      break;
    case '9':
      goleft(500,1);
      break;

    case '1':
      tool(1,1);
      break;
    case '2':
      tool(1,2);
      break;
    case '3':
      tool(1,3);
      break;
    case '4':
      tool(1,4);
      break;
    case '5':
      tool(1,5);
      break;
    case '6':
      tool(1,6);
      break;
    case '0':
      tool(0,0);
      break;

    case '.':
      spot(1);
      break;
    case ',':
      spot(0);
      break;     

    case '/':
      centerof();
      break;

    case 'o':
      findypluslimit();
      break;
    case 'p':
      park();
      break;
    case 'P':
      gohome();
      break;
    case 'k':
      parkleft();
      break;
    case 'K':
      parkforward();     
      break;
    case '[':
      findleftlimit();
      break;
    case ']':
      findrightlimit();
      break;
    case 'l':
      readLimitSwitches();
      break;

    case ' ':
      home();
      break;

    case '?':
      Serial.println("123456");
      Serial.print(digitalRead(SPARE1),HEX);      
      Serial.print(digitalRead(SPARE2),HEX);      
      Serial.print(digitalRead(SPARE3),HEX);      
      Serial.print(digitalRead(SPARE4),HEX);      
      Serial.print(digitalRead(SPARE5),HEX);      
      Serial.print(digitalRead(SPARE6),HEX);      
      Serial.println("");
      break;
    default:
      Serial.println("none");
      break;
    }
  } 
  else {
    readPanel();
  }

}

/* 
 *  home function 
 */

void home( void ) 
{
  boolean xhomed = false , yhomed = false;

  if( headDown() == true ) {
    head(0) ;
  }

  // reset the pulse counter 
  gXPulses = -1;
  gYPulses = -1;
  lx = 0;
  ly=0;
  // just in case

  head(0);
  //tool changer off
  tooloff();

  Serial.println("Homing machine");

  // doesn't need to find left/Y+ limits    
  findleftlimit();
  delay(100);

  findypluslimit();
  delay(100);

  homed = false;

  // duty cycle length
  long length = ( DELAY_X2 + DELAY_X1) ;

  // next step is to move right a bit, so that its past the home switch
  // check XL1,YL1 limits if its in either, move out? 
  for(int i = 0 ; i < 1000 ; i++ ) {

    if( xLimit2 ) {
      digitalWrite(XCCW,HIGH);
      break;
    }

    // hit a limit
    if( yLimit2 ) {
      digitalWrite(YCCW,HIGH);
      break;
    }

    // move both x and y
    digitalWrite(XCCW,HIGH);
    digitalWrite(YCCW,HIGH);
    delayMicroseconds(SHORT_X_PULSE); 
    digitalWrite(XCCW,LOW);
    digitalWrite(YCCW,LOW);
    // decrease delay ( ramp )
    delayMicroseconds(length);   

    // clamp speed to slowest axis ( Y Axis)
    if (length <= Y_SPEED ) 
      length = Y_SPEED;
  }

  digitalWrite(XCCW,HIGH);
  digitalWrite(YCCW,HIGH);

  readLimitSwitches();

  for(int i = 0 ; i < 1100 ; i++ ) {

    if( digitalRead( XHM )) {
      xhomed = true;
      break;
    }

    // failed
    if( xLimit1 ) {
      digitalWrite(XCW,HIGH);
      return;
    }

    if( xhomed == false ) {
      stepXCW(DELAY_X2+DELAY_X1);
    }

  }

  digitalWrite(XCW,HIGH);

  for(int i = 0 ; i < 1100 ; i++ ) {

    if( digitalRead( YHM ) ) {
      yhomed = 1;
      break;
    }
    
    // hit a limit
    // failed
    if( yLimit1 ) {
      digitalWrite(YCW,HIGH);
      return;
    }      

    if( yhomed == 0 ){
      stepYCW(DELAY_Y2+DELAY_Y1);
    }
  }

  // stop
  digitalWrite(YCW,HIGH);

  // retest home position
  xHome = false; 
  yHome = false;

  // read limit switches
  readLimitSwitches();

  if( !( xHome == true && yHome == true )) {
    Serial.println("Failed to home");
    homed = false;
    return;
  }

  // reset global pulse counter to zero ( this changes, since home won't be zero/zero )
  gXPulses = 0;
  gYPulses = 0;

#ifndef NDEBUG
  Serial.println("HOMED");
#endif

  homed = true;
}

void centerof(void)
{
  // light offf
  spot(false);

  // check if homed
  if( homed == false ) {

    home();

    if( homed == false ) {
      Serial.println("Failed to home");
      return;
    }
  }
  
  // 2800 = 14mm
  Serial.println("Center");

  Serial.println("X home");
  findlefthome(); 
  readLimitSwitches();
  Serial.println("Y home");
  findyplushome(); 
  readLimitSwitches();

  Serial.println("right");
  goright(2500,1); 
  readLimitSwitches();
  Serial.println("back");
  goback(4600,1); 
  readLimitSwitches();

  if( homed == 0 ) {
    Serial.println("Something went wrong, hit a limit");
    return;
  }

  spot(true);

  Serial.println("centerof done");

}

void walk(void)
{
  Serial.println("Walk limit mode enabled, press a key to stop after limit reached");
  Serial.read();
 
  while(1){
    findrightlimit(); 
    readLimitSwitches();
    if (Serial.available() > 0) break;
    findleftlimit(); 
    readLimitSwitches();
    if (Serial.available() > 0) break;
    findyminuslimit(); 
    readLimitSwitches();
    if (Serial.available() > 0) break;
    findrightlimit(); 
    readLimitSwitches();
    if (Serial.available() > 0) break;
    findypluslimit(); 
    readLimitSwitches();
    if (Serial.available() > 0) break;
  }

  while(Serial.available()) {
    Serial.read();
  }

  Serial.println("Test Ended");

}

void tapeKnock( boolean on_off )
{
  static unsigned char lastState = 0xff;

  if( lastState == on_off ) 
    return;

  lastState = on_off;

  if( on_off ) 
    // debug
    digitalWrite(TAPEKNOCK,LOW);  
  else  
    digitalWrite(TAPEKNOCK,HIGH);

}

void spot( boolean on)
{
#ifndef NDEBUG
  Serial.println("Spot");
#endif

  if( on ) 
    digitalWrite(SPOT,LOW);  
  else  
    digitalWrite(SPOT,HIGH);
}

void rotate( boolean on )
{
#ifndef NDEBUG
  Serial.println("rotate");
#endif
  if( on ) 
    digitalWrite(ROT,LOW);  
  else  
    digitalWrite(ROT,HIGH);
}

void center( boolean on )
{
#ifndef NDEBUG
  Serial.println("center");
#endif
  if( on ) 
    digitalWrite(CENTERING,LOW);  
  else  
    digitalWrite(CENTERING,HIGH);
}

boolean head( boolean on )
{
#ifndef NDEBUG
  Serial.println("head");
#endif

  if( on ) {
    digitalWrite(HEAD,LOW);  
    headState = true;

    while( headDown() == false) ;

  } else {
    digitalWrite(HEAD,HIGH);
    headState = false;
    while( headDown() == true) ;
  }

  //if the head down sensor doesn't equal the headState, something is wrong
  if (headDown() != headState ) {
    // problem!
    return false;
  }

  return true;
}

boolean findlefthome(void) {

  unsigned long counter = 0;

  while(1) {

    if( digitalRead( XL1) ) {

      digitalWrite(XCW,HIGH);

      homed = 0;

#ifndef NDEBUG
      Serial.println("X limit!");
#endif
      return false;

    }

    if( digitalRead( XHM ) ) {

      digitalWrite(XCW,HIGH);

#ifndef NDEBUG
      Serial.println("Found left home");
#endif               
      return false;
    }


    ( DELAY_Y1 + DELAY_Y2);
    counter++;
  }  

  // reset it back
  digitalWrite(XCW,HIGH);

  return true;
}

boolean findleftlimit(void) 
{

  unsigned long counter = 0;

  unsigned int length = DELAY_X1 + DELAY_X2;

  while( 1 ) {

    if(  digitalRead( XL1 )) {

      digitalWrite(XCW,HIGH);

      // hit a limit not homed
      homed = 0;

#ifndef NDEBUG
      Serial.print("Found x left limit = ");
      Serial.println(counter,DEC);
#endif

      // Reset X micrometers counter 
      gCurrentXum = 0;

      // settle time
      delay( 250 );

      return false;
    }

    // step and decrease speed
    stepXCW(length--);

    // counts steps done        
    counter++;

    // cap speed
    if (length <= X_SPEED )  {
      length = X_SPEED;
    }
  }  
}

boolean findrightlimit(void) {

  unsigned long counter = 0;

  unsigned int length = ( DELAY_X1 + DELAY_X2 ) ;

  while( 1 ) {

    if( digitalRead( XL2 ) ) {

      digitalWrite(XCCW,HIGH);


      homed = 0;

#ifndef NDEBUG
      Serial.print("Found X right limit = ");
      Serial.println(counter,DEC);
#endif
      delay(250);

      return false;
    }

    stepXCCW(length--);
    counter ++;

    //clamp value
    if (length <= X_SPEED ) 
      length = X_SPEED;
  }  
}

boolean findyplushome(void) {

  while(1)
  {
    if( digitalRead( YL1) ) {

      digitalWrite(YCW,HIGH);

      homed = 0;

      Serial.println("Y+ limit!");

      // Reset Yum counter 
      gCurrentYum = 0;
      delay(500);

      return false;

    }

    //    Serial.print(digitalRead(YHM),HEX);
    if( digitalRead( YHM ) ) {

      digitalWrite(YCW,HIGH);

      Serial.println("Found Y home");
      delay(200);

      return false;
    }

    stepYCW(DELAY_Y1+DELAY_Y2);
  }  

  // reset it back
  digitalWrite(YCW,HIGH);

  return true;
}

// this function should be called before moving either axis.
void beforeMoving( void ) 
{
  // head up
  if( headState )
    head( false );
}

// Step once forward
void stepYCW( long length ) 
{    
  beforeMoving();
  cli();
  digitalWrite(YCW,HIGH);
  _delay_us( SHORT_X_PULSE ) ;
  digitalWrite(YCW,LOW);
  _delay_us(length); 
  sei();

  DecrementYPulses();
}

// Step once back
void stepYCCW( long pulselength ) 
{
  beforeMoving();
  
  cli();
  digitalWrite(YCCW,HIGH);
  _delay_us( SHORT_X_PULSE ) ;
  digitalWrite(YCCW,LOW);
  _delay_us(pulselength);  
  sei();

  IncrementYPulses();
}

// Step once left
void stepXCW( long pulselength ) 
{
  beforeMoving();

  cli();
  digitalWrite(XCW,HIGH);
  _delay_us( SHORT_X_PULSE ) ;
  delayMicroseconds(SHORT_X_PULSE); 
  digitalWrite(XCW,LOW);
  _delay_us(pulselength); 
  sei();

  DecrementXPulses();
}

// Step once right
void stepXCCW( long pulselength ) 
{
  beforeMoving();

  digitalWrite(XCCW,HIGH);
  _delay_us( SHORT_X_PULSE ) ;
  digitalWrite(XCCW,LOW);
  _delay_us(pulselength);    

  sei();
  IncrementXPulses();
}

boolean findypluslimit(void) {

  unsigned long counter = 0;
  unsigned int length = DELAY_Y1 + DELAY_Y2;

  while( 1 ) {

    if(  digitalRead( YL1 )) {

      digitalWrite(YCW,HIGH);

      homed = 0;
      Serial.print("Found Y+ limit = ");
      Serial.println(counter,DEC);
      delay(200);

      // reset Y micrometers
      gCurrentYum =  0;

      return false;
    }

    stepYCW(length--);

    counter++;
    
    if (length <= Y_SPEED ) 
      length = Y_SPEED;
  } 
 return true; 
}

boolean findyminuslimit(void) {

  unsigned long counter = 0;
  unsigned int length = DELAY_Y1 + DELAY_Y2;

  while( 1 ) {

    if(  digitalRead( YL2 )) {

      digitalWrite(YCCW,HIGH);
      delay( 200 );

      homed = false ;
      Serial.print("Found Y- limit = ");
      Serial.println(counter,DEC);
      return false;
    }

    stepYCCW(length--);
    counter++;

    if (length <= Y_SPEED ) 
      length = Y_SPEED;
  } 
  return true; 
}

// pulses = ( microm * 40000 )
// conversions betweem mm,um and pusles
inline long mmtopulses( long mm ) 
{
  return ( mm *  40 ) ;
}

inline long umtopulses( long microm ) 
{
  return ( microm /  25 ) ;
}

inline long pulsestoum( long pulses ) 
{
  return ( pulses *  25 ) ;
}

inline long pulsestomm( long pulses ) 
{
  return ( pulses /  40 ) ;
}

/*

 xum,yum = 150000 150000 going to in um
 gx,gy = 6000 6000  at in pulses
 cx,cy = 150000 150000 at in um
 dx,dy = 0 0 difference
 nx,ny = 0 0
 
 xum,yum = 25000 25000 wanted in um
 gx,gy = 6000 6000 at in pulses
 cx,cy = 150000 150000 currently in um
 dx,dy = 125000 125000 difference in um
 nx,ny = 5000 5000 pulses
 
 
 none
 
 */

boolean gotoxy(long xum,long yum)
{
  // direction to travel, defaults to right and back
  boolean xDir=true,yDir=true;
  long dx,dy;
  long numXPulses=0,numYPulses=0;

  // if not homed, drop out
  if( homed == false ) {
    return false;
  }
#ifndef NDEBUG
  Serial.print("xum,yum um= ");
  Serial.print(xum);
  Serial.print(" ");
  Serial.println(yum);

  Serial.print("gx,gy pulses= ");
  Serial.print(gXPulses);
  Serial.print(" ");
  Serial.println(gYPulses);
#endif

  gCurrentXum = pulsestoum( gXPulses ) ;
  gCurrentYum = pulsestoum( gYPulses ) ;

#ifndef NDEBUG
  Serial.print("cx,cy = ");
  Serial.print(gCurrentXum);
  Serial.print(" ");
  Serial.println(gCurrentYum);
#endif           

  // calculate dx/dy
  dx = ( gCurrentXum - xum );
  dy = ( gCurrentYum - yum );

#ifndef NDEBUG
  Serial.print("dx,dy um = ");
  Serial.print(dx);
  Serial.print(" ");
  Serial.println(dy);
#endif       
  // if needs to go left
  if( dx < 0  ) {
    dx = abs( dx ) ;
    xDir = false;
  }
  if( dy < 0  ) {
    dy = abs( dy ) ;
    yDir = false;
  }

  // Convert from micrometers to pulses
  numXPulses = umtopulses( dx );
  numYPulses = umtopulses( dy );

#ifndef NDEBUG
  Serial.print("nx,ny pulses = ");
  Serial.print(numXPulses);
  Serial.print(" ");
  Serial.println(numYPulses);
#endif    

  if( numXPulses > 0 ) {
    if( xDir == true ) {
      goleft( numXPulses, 1);
    } 
    else {
      goright( numXPulses, 1 ) ;
    }
  }

  if( numYPulses > 0 ) {
    if( yDir == true ) {
      goforward( numYPulses, 1 ) ;
    } 
    else {
      goback( numYPulses, 1);
    }
  }
  return true;       
}

/*******
 * 
 * moveline - move head from x0,y0 to x1,y1 
 *
 */
void moveLine(long x0, long y0, long x1, long y1)
{
  long dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  long dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  long err = dx+dy, e2; /* error value e_xy */

  for(;;){  /* loop */

    gotoxy(x0,y0);

    if (x0==x1 && y0==y1) 
      break;

    e2 = 2*err;

    if (e2 >= dy) { 
      err += dy; 
      x0 += sx; 
    }
    /* e_xy+e_x > 0 */
    if (e2 <= dx) { 
      err += dx; 
      y0 += sy; 
    } /* e_xy+e_y < 0 */
  }
}

// pulses
boolean gotoxyp(int xp,int yp)
{
  goright(xp,1);
  goback(yp,1);
}

boolean goleft(long steps ,boolean nolimit) {

  long length = DELAY_X1+DELAY_X2;
  long perc = (steps /100 ) * 5;
  if( (steps-perc) < 0 ) perc = 0;

  for( int i = 0 ; i < steps ; i ++ )
  {
    if( nolimit == true ) {
      if( digitalRead( XL1 ) ) {
        digitalWrite(XCW,HIGH);
#ifndef NDEBUG
        Serial.println("goleft limit");
#endif
        return false;
      }
    }
    stepXCW( length );

    if( i < (steps-perc)) 
      length--;
    else
      length+=2;

    if (length < X_SPEED ) 
      length = X_SPEED;

    if (length > DELAY_Y2 + DELAY_Y1 ) 
      length = DELAY_Y2 + DELAY_Y1;
  }  

  // reset it back
  digitalWrite(XCW,HIGH);

  return true;
}

boolean goright(long steps ,boolean nolimit ) 
{
  long length=DELAY_X2+DELAY_X1;
  long perc = (steps /100 ) * 5;

  if(steps-perc <0 ) perc = 0;

  for( int i = 0 ; i < steps ; i ++ )
  {
    if( nolimit == true ) {
      if( digitalRead( XL2 ) ) {
        digitalWrite(XCCW,HIGH);
#ifndef NDEBUG
        Serial.println("goright limit");
#endif
        return false;
      }
    }

    stepXCCW( length );

    if( i < (steps-perc)) 
      length--;
    else
      length+=2;

    if (length < X_SPEED ) 
      length = X_SPEED;

    if (length >DELAY_Y2 + DELAY_Y1 ) 
      length = DELAY_Y2 + DELAY_Y1;    
  }  

  // reset it back
  digitalWrite(XCCW,HIGH);

  return true;        
}

boolean goforward(long steps ,boolean nolimit )
{
  long length = DELAY_Y2 + DELAY_Y1;
  long perc = (steps /100 ) * 2;

  if( (steps-perc) < 0 ) perc = 0;

  for( int i = 0 ; i < steps ; i ++ )
  {
    if( nolimit == true ) {
      if( digitalRead( YL1 ) ) {
        digitalWrite(YCW,HIGH);
#ifndef NDEBUG
        Serial.println("goforward limit");
#endif
        return false;
      }
    }
    stepYCW( length );

    if( i < (steps-perc)) 
      length--;
    else
      length+=1;

    if (length < Y_SPEED ) 
      length = Y_SPEED;
    if (length >DELAY_Y2 + DELAY_Y1 ) 
      length = DELAY_Y2 + DELAY_Y1;          

  }  

  // reset it back
  digitalWrite(YCW,HIGH);
  return true;
}


boolean goback(long steps ,boolean nolimit ) {

  long length = DELAY_Y2 + DELAY_Y1;
  long perc = (steps /100 ) * 2;

  if( (steps-perc) < 0 ) perc = 0;

  for( int i = 0 ; i < steps ; i ++ )
  {
    if( nolimit == true ) {
      if( digitalRead( YL2 ) ) {
        digitalWrite(YCCW,HIGH);
#ifndef NDEBUG
        Serial.println("goback limit");
#endif
        return false;
      }
    }
    stepYCCW( length ); 

    if( i < (steps-perc)) 
      length--;
    else
      length+=1;


    if (length < Y_SPEED ) 
      length = Y_SPEED;

    if (length >DELAY_Y2 + DELAY_Y1 ) 
      length = DELAY_Y2 + DELAY_Y1;
  }  


  // reset it back
  digitalWrite(YCCW,HIGH);

  return true;        
}

void IncrementXPulses( void ) 
{
  gXPulses ++;

  if( gXPulses >= MAX_X_PULSES ) {
    Serial.print("Error too many X pulses, must be loosing some ");
    Serial.print(gXPulses,DEC);
    Serial.println(gYPulses,DEC);

    homed = false;
    return;
  }
}

void DecrementXPulses( void ) 
{
  gXPulses --;

  if( gXPulses < 0 && homed ) {
    Serial.print("Error negative X pulses, must be loosing some ");
    Serial.print(gXPulses,DEC);
    Serial.print(gYPulses,DEC);
    Serial.println("");
    homed = false;
    return;
  }
}

void IncrementYPulses( void ) 
{
  gYPulses ++;

  if( gYPulses >= MAX_Y_PULSES && homed) {
    Serial.print("Error too many Y pulses, must be loosing some ");
    Serial.print(gXPulses,DEC);
    Serial.println(gYPulses,DEC);

    homed = false;
    return;
  }
}

void DecrementYPulses( void ) 
{
  gYPulses --;

  if( gYPulses < 0 && homed ) {
    Serial.print("Error negative Y pulses, must be loosing some ");
    Serial.print(gXPulses,DEC);
    Serial.println(gYPulses,DEC);
    readLimitSwitches();
    homed = false;
    return;
  }
}


float distance(float x1, float y1, float x2, float y2)
{
  return sqrt(pow(x2-x1,2) + pow(y2-y1,2));
}


void plotLine(long x0, long y0, long x1, long y1,unsigned long col,int fast)
{
  static long lastpx=0,lastpy=0;
  long dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  long dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  long err = dx+dy, e2; /* error value e_xy */
  long seg1,seg2;	
  float dist = distance(x0,y0,x1,y1);

  lastpx=0;
  lastpy=0;

  // calculate 20% of distance
  seg1  = ( dist / 100.0f ) * 20.0f;

  // subtract
  dist -= seg1;

  for(;;){  /* loop */


    if( seg1 > 0 ) {
      // first segment, slow
      gotoxy(x0,y0);
      delay(20);
      seg1	--;
      dist--;
    } 
    else {
      if (dist >0 ) {
        // fast 
        gotoxy(x0,y0);
        delay(20);
        dist--;
      } 
      else {
        //slow last
        gotoxy(x0,y0);
        delay(20);
      }
    }

    if (x0==x1 && y0==y1) 
      break;

    e2 = 2*err;

    if (e2 >= dy) { 
      err += dy; 
      x0 += sx; 
    } 

    if (e2 <= dx) { 
      err += dx; 
      y0 += sy; 
    } 
  }
}

#define ACCEL_STEP ( 16 ) 

boolean movebackleft(long x0_pulses,long y0_pulses, boolean nolimit )
{
  long length = DELAY_Y2 + DELAY_Y1;
  long llength = DELAY_X2 + DELAY_X1;
  long fperc,lperc;
  long fsteps,lsteps;
  long li,fi;
#ifndef NDEBUG
  Serial.print("movebackleft ");
  Serial.print(x0_pulses, DEC);
  Serial.print(" pulses ");
  Serial.print(y0_pulses, DEC);
  Serial.println(" pulses");
#endif

  lsteps = ( x0_pulses ) ;
  fsteps = ( y0_pulses ) ;

  fperc = (fsteps / 100 ) * 5;
  lperc = (lsteps / 100 ) * 5;

  if( (fsteps-fperc) < 0 ) fperc = 0;
  if( (lsteps-lperc) < 0 ) lperc = 0;

  fi = 0;
  li = 0;

  // if li or fi > 0
  while( fsteps || lsteps )
  {
    if( nolimit == true ) {
      if( digitalRead( YL2 ) ) {
        digitalWrite(YCCW,HIGH);
        digitalWrite(XCW,HIGH);
#ifndef NDEBUG
        Serial.println("YL2 limit");
#endif
        return false;
      }
    }

    if( nolimit == true ) {
      if( digitalRead( XL1 ) ) {
        digitalWrite(YCW,HIGH);
        digitalWrite(XCW,HIGH);
#ifndef NDEBUG
        Serial.println("XL1 limit");
#endif
        return false;
      }
    }


    /// handle forward/back Y axis
    if( fsteps ) {
      stepYCCW( length );

      if( fi < (y0_pulses-fperc)) 
        length-=ACCEL_STEP;
      else
        length+=1;

      if (length < Y2_SPEED ) 
        length = Y2_SPEED;

      if (length >DELAY_Y2 + DELAY_Y1 ) 
        length = DELAY_Y2 + DELAY_Y1; 

      fi ++;
      fsteps--;
    }

    /// handle left, right X axis
    if( lsteps ) {

      stepXCW( llength );

      if( li < (x0_pulses-lperc)) 
        llength-=ACCEL_STEP;
      else
        llength+=1;

      if (llength < X2_SPEED ) 
        llength = X2_SPEED;

      if (llength > DELAY_Y2 + DELAY_Y1 ) 
        llength = DELAY_Y2 + DELAY_Y1;

      li ++;
      lsteps--;
    }  
  }  

  // reset servos back
  digitalWrite(YCCW,HIGH);
  digitalWrite(XCW,HIGH);

  return true;
}

boolean movebackright(long x0_pulses,long y0_pulses, boolean nolimit )
{
  long length = DELAY_Y2 + DELAY_Y1;
  long llength = DELAY_X2 + DELAY_X1;
  long fperc,lperc;
  long fsteps,lsteps;
  long li,fi;
#ifndef NDEBUG
  Serial.print("movebackright ");
  Serial.print(x0_pulses, DEC);
  Serial.print(" pulses ");
  Serial.print(y0_pulses, DEC);
  Serial.println(" pulses");
#endif

  lsteps = ( x0_pulses ) ;
  fsteps = ( y0_pulses ) ;

  fperc = (fsteps / 100 ) * 5;
  lperc = (lsteps / 100 ) * 5;

  if( (fsteps-fperc) < 0 ) fperc = 0;
  if( (lsteps-lperc) < 0 ) lperc = 0;

  fi = 0;
  li = 0;

  // if li or fi > 0
  while( fsteps || lsteps )
  {
    if( nolimit == true ) {
      if( digitalRead( YL2 ) ) {
        digitalWrite(YCCW,HIGH);
        digitalWrite(XCCW,HIGH);
#ifndef NDEBUG
        Serial.println("YL2 limit");
#endif
        return false;
      }
    }

    if( nolimit == true ) {
      if( digitalRead( XL2 ) ) {
        digitalWrite(YCCW,HIGH);
        digitalWrite(XCCW,HIGH);
#ifndef NDEBUG
        Serial.println("XL2 limit");
#endif
        return false;
      }
    }

    /// handle forward/back Y axis
    if( fsteps ) {
      stepYCCW( length );

      if( fi < (y0_pulses-fperc)) 
        length-=ACCEL_STEP;
      else
        length+=1;

      if (length < Y2_SPEED ) 
        length = Y2_SPEED;

      if (length >DELAY_Y2 + DELAY_Y1 ) 
        length = DELAY_Y2 + DELAY_Y1; 

      fi ++;
      fsteps--;
    }

    /// handle left, right X axis
    if( lsteps ) {

      stepXCCW( llength );

      if( li < (x0_pulses-lperc)) 
        llength-=ACCEL_STEP;
      else
        llength+=1;

      if (llength < X2_SPEED ) 
        llength = X2_SPEED;

      if (llength > DELAY_Y2 + DELAY_Y1 ) 
        llength = DELAY_Y2 + DELAY_Y1;

      li ++;
      lsteps--;
    }  


  }  

  // reset servos back
  digitalWrite(YCCW,HIGH);
  digitalWrite(XCCW,HIGH);

  return true;
}
boolean moveforwardright(long x0_pulses,long y0_pulses, boolean nolimit )
{
  long length = DELAY_Y2 + DELAY_Y1;
  long llength = DELAY_X2 + DELAY_X1;
  long fperc,lperc;
  long fsteps,lsteps;
  long li,fi;

#ifndef NDEBUG
  Serial.print("moveforwardright ");
  Serial.print(x0_pulses, DEC);
  Serial.print(" pulses ");
  Serial.print(y0_pulses, DEC);
  Serial.println(" pulses");
#endif

  fsteps = ( y0_pulses ) ;
  lsteps = ( x0_pulses ) ;

  fperc = (fsteps / 100 ) * 5;
  lperc = (lsteps / 100 ) * 5;

  if( ( fsteps-fperc) < 0 ) fperc = 0;
  if( ( lsteps-lperc) < 0 ) lperc = 0;

  fi = 0;
  li = 0;

  // if li or fi > 0
  while( fsteps || lsteps )
  {
    if( nolimit == true ) {
      if( digitalRead( YL1 ) ) {
        digitalWrite(YCW,HIGH);
        digitalWrite(XCCW,HIGH);
#ifndef NDEBUG
        Serial.println("YL1 limit");
#endif
        return false;
      }
    }

    if( nolimit == true ) {
      if( digitalRead( XL2 ) ) {
        digitalWrite(YCW,HIGH);
        digitalWrite(XCCW,HIGH);
#ifndef NDEBUG
        Serial.println("XL2 limit");
#endif
        return false;
      }
    }

    // handle front/back Y axis
    if( fsteps ) {
      stepYCW( length );

      if( fi < (y0_pulses-fperc)) 
        length-=ACCEL_STEP;
      else
        length+=1;

      if (length < Y2_SPEED ) 
        length = Y2_SPEED;
      if (length >DELAY_Y2 + DELAY_Y1 ) 
        length = DELAY_Y2 + DELAY_Y1; 

      fi ++;
      fsteps--;
    }

    // handleleft/right X axis
    if( lsteps ) {

      stepXCCW( llength );

      if( li < (x0_pulses-lperc)) 
        llength-=4;
      else
        llength+=1;

      if (llength < X2_SPEED ) 
        llength = X2_SPEED;

      if (llength > DELAY_Y2 + DELAY_Y1 ) 
        llength = DELAY_Y2 + DELAY_Y1;

      li ++;
      lsteps--;
    }  
  }  

  // reset servos back
  digitalWrite(YCW,HIGH);
  digitalWrite(XCCW,HIGH);

  return true;
}

boolean moveforwardleft(long x0_pulses,long y0_pulses, boolean nolimit )
{
  long length = DELAY_Y2 + DELAY_Y1;
  long llength = DELAY_X2 + DELAY_X1;
  long fperc,lperc;
  long fsteps,lsteps;
  long li,fi;

#ifndef NDEBUG
  Serial.print("moveforwardleft ");
  Serial.print(x0_pulses, DEC);
  Serial.print(" pulses ");
  Serial.print(y0_pulses, DEC);
  Serial.println(" pulses");
#endif

  fsteps = ( y0_pulses ) ;
  lsteps = ( x0_pulses ) ;

  fperc = (fsteps / 100 ) * 5;
  lperc = (lsteps / 100 ) * 5;

  if( ( fsteps-fperc) < 0 ) fperc = 0;
  if( ( lsteps-lperc) < 0 ) lperc = 0;

  fi = 0;
  li = 0;

  // if li or fi > 0
  while( fsteps || lsteps )
  {
    if( nolimit == true ) {
      if( digitalRead( YL1 ) ) {
        digitalWrite(YCW,HIGH);
        digitalWrite(XCW,HIGH);
#ifndef NDEBUG
        Serial.println("YL1 limit");
#endif
        return false;
      }
    }

    if( nolimit == true ) {
      if( digitalRead( XL1 ) ) {
        digitalWrite(YCW,HIGH);
        digitalWrite(XCW,HIGH);
#ifndef NDEBUG
        Serial.println("XL1 limit");
#endif
        return false;
      }
    }

    // handle front/back Y axis
    if( fsteps ) {
      stepYCW( length );

      if( fi < (y0_pulses-fperc)) 
        length-=ACCEL_STEP;
      else
        length+=1;

      if (length < Y2_SPEED ) 
        length = Y2_SPEED;
      if (length >DELAY_Y2 + DELAY_Y1 ) 
        length = DELAY_Y2 + DELAY_Y1; 

      fi ++;
      fsteps--;
    }

    // handleleft/right X axis
    if( lsteps ) {

      stepXCW( llength );

      if( li < (x0_pulses-lperc)) 
        llength-=2;
      else
        llength+=1;

      if (llength < X2_SPEED ) 
        llength = X2_SPEED;

      if (llength > DELAY_Y2 + DELAY_Y1 ) 
        llength = DELAY_Y2 + DELAY_Y1;

      li ++;
      lsteps--;
    }  
  }  

  // reset servos back
  digitalWrite(YCW,HIGH);
  digitalWrite(XCW,HIGH);

  return true;
}

enum {
  GO_NONE,
  GO_FL,
  GO_FR,
  GO_BR,
  GO_BL
};

void moveLine2o( long x0, long y0, long x1, long y1)
{
  long xdiff,ydiff;

  if(headDown() == true ) {
    head(0) ;
  }

  //convert to pulses
  x0 = umtopulses(x0);
  y0 = umtopulses(y0);
  x1 = umtopulses(x1);
  y1 = umtopulses(y1);

  while( x0 != x1 || y0 != y1 )
  {

    if( x0 != x1 ) {

      xdiff = x0-x1;
      xdiff = abs(xdiff);

      // move right
      if( x0 < x1 ) {

        if ( false == goright( xdiff,1) ) return;
        delay(20);
        x0 += xdiff;
      } 
      else { 
        if ( false == goleft( xdiff,1)) return;
        delay(20);

        x0 -= xdiff;
      }
    }

    if( y0 != y1 ) {

      ydiff = y0-y1;
      ydiff = abs(ydiff);

      // move right
      if( y0 < y1 ) {
        if ( false == goback(ydiff,1)) return;
        delay(20);
        y0 += ydiff;
      } 
      else { 
        if ( false == goforward(ydiff,1)) return;
        delay(20);
        y0 -= ydiff;
      }
    }
  }
}

void moveLine2( long x0, long y0, long x1, long y1)
{
  long xdiff,ydiff;
  unsigned char direction = GO_NONE ;

#ifndef NDEBUG
  Serial.print("moveLine2 ");
  Serial.print(x0, DEC);
  Serial.print(" ");
  Serial.print(y0, DEC);
  Serial.print(" ");
  Serial.print(x1, DEC);
  Serial.print(" ");
  Serial.println(y1, DEC);
#endif

  if( headDown() == true ) {
    head(0);
  }

  //convert to pulses
  x0 = umtopulses(x0);
  y0 = umtopulses(y0);
  x1 = umtopulses(x1);
  y1 = umtopulses(y1);

  while( x0 != x1 || y0 != y1 )
  {
    if( y0 > y1 && x0 > x1 ) {
      direction = GO_FL;
    }
    else 

      if( y0 > y1 && x0 < x1 ) {
      direction = GO_FR;
    } 
    else 

      if( y0 < y1 && x0 > x1 ) {
      direction = GO_BL;
    }
    else 

      if( y0 < y1 && x0 < x1 ) {
      direction = GO_BR;
    } 
    else {
      direction = GO_NONE;
    }

    // forward and left
    switch( direction ) {

    case GO_FL:
      Serial.print ("GO_FL ");
      xdiff = x0-x1;
      xdiff = abs(xdiff);
      ydiff = y0-y1;
      ydiff = abs(ydiff);

      Serial.print(xdiff,DEC);
      Serial.print(" "); 
      Serial.println(ydiff,DEC); 

      moveforwardleft(xdiff,ydiff,true);
      y0 -= ydiff;
      x0 -= xdiff;
      break;
    case GO_BR:
      Serial.print("GO_BR ");
      xdiff = x0-x1;
      xdiff = abs(xdiff);
      ydiff = y0-y1;
      ydiff = abs(ydiff);

      Serial.print(xdiff,DEC);
      Serial.print(" "); 
      Serial.println(ydiff,DEC); 

      movebackright(xdiff,ydiff,true);
      y0 += ydiff;
      x0 += xdiff;
      break;      
      // Don't work
    case GO_FR:
      Serial.print("GO_FR ");
      xdiff = x0-x1;
      xdiff = abs(xdiff);
      ydiff = y0-y1;
      ydiff = abs(ydiff);

      Serial.print(xdiff,DEC);
      Serial.print(" "); 
      Serial.println(ydiff,DEC); 

      moveforwardright(xdiff,ydiff,true);
      y0 -= ydiff;
      x0 += xdiff;
      break;
    case GO_BL:
      Serial.print("GO_BL ");
      xdiff = x0-x1;
      xdiff = abs(xdiff);
      ydiff = y0-y1;
      ydiff = abs(ydiff);

      Serial.print(xdiff,DEC);
      Serial.print(" "); 
      Serial.println(ydiff,DEC); 

      movebackleft(xdiff,ydiff,true);
      y0 += ydiff;
      x0 -= xdiff;
      break;      

    case GO_NONE:
      Serial.println("GO_NONE");
      if( x0 != x1 ) {

        xdiff = x0-x1;
        xdiff = abs(xdiff);

        // move right
        if( x0 < x1 ) {

          if ( false == moveforwardright( xdiff,0,true) ) return;
          x0 += xdiff;
        } 
        else { 
          if ( false == moveforwardleft( xdiff,0,1) ) return;

          x0 -= xdiff;
        }
      }

      if( y0 != y1 ) {

        // calculate dy abs
        ydiff = y0-y1;
        ydiff = abs(ydiff);

        // move back
        if( y0 < y1 ) {
          if ( false == movebackleft(0,ydiff,1)) return;
          y0 += ydiff;
        } 
        else { 
          if ( false == moveforwardleft(0,ydiff,1)) return;
          y0 -= ydiff;
        }
      }

      break;
    default:
      Serial.println("GO_DERP");

      break;
    }
  }
}

void move2(long x0,long y0 )
{
  if( headDown() == true ) {
    head(0) ;
  }

  moveLine2o(lx,ly,x0,y0);

  lx = x0;
  ly = y0;
}

void setupTimer1(void)
{
  // Tells what part of speed ramp we are in.
  //  srd.run_state = STOP;
  // Timer/Counter 1 in mode 4 CTC (Not running).
  TCCR1B = (1<<WGM12);
  // Timer/Counter 1 Output Compare A Match Interrupt enable.
  TIMSK1 = (1<<OCIE1A);
}

void speakerBadge(void)
{
  xtoolchange();
  move2(61860,159240);
  xend();
  xtoolchange();
  move2(55510,152890);
  xend();
  xtoolchange();
  rotate(true);
  move2(49160,159240);
  rotate(false);
  xend();
  xtoolchange();
  move2(55510,165590);
  xend();
  xtoolchange();
  move2(79640,146540);
  xend();
  xtoolchange();
  move2(41540,240520);
  xend();
  xtoolchange();
  move2(45350,240520);
  xend();
  xtoolchange();
  move2(49160,240520);
  xend();
  xtoolchange();
  move2(52970,240520);
  xend();
  xtoolchange();
  move2(56780,240520);
  xend();
  xtoolchange();
  move2(60590,240520);
  xend();
  xtoolchange();
  move2(64400,240520);
  xend();
  xtoolchange();
  move2(68210,240520);
  xend();
  xtoolchange();
  move2(72020,240520);
  xend();
  xtoolchange();
  move2(75830,240520);
  xend();
  xtoolchange();
  move2(79640,240520);
  xend();
  xtoolchange();
  move2(83450,240520);
  xend();
  xtoolchange();
  move2(87260,240520);
  xend();
  xtoolchange();
  move2(91070,240520);
  xend();
  xtoolchange();
  move2(94880,240520);
  xend();
  xtoolchange();
  move2(98690,240520);
  xend();
  xtoolchange();
  move2(102500,240520);
  xend();
  xtoolchange();
  move2(106310,240520);
  xend();
  xtoolchange();
  move2(41540,236710);
  xend();
  xtoolchange();
  move2(45350,236710);
  xend();
  xtoolchange();
  move2(49160,236710);
  xend();
  xtoolchange();
  move2(52970,236710);
  xend();
  xtoolchange();
  move2(56780,236710);
  xend();
  xtoolchange();
  move2(60590,236710);
  xend();
  xtoolchange();
  move2(64400,236710);
  xend();
  xtoolchange();
  move2(68210,236710);
  xend();
  xtoolchange();
  move2(72020,236710);
  xend();
  xtoolchange();
  move2(75830,236710);
  xend();
  xtoolchange();
  move2(79640,236710);
  xend();
  xtoolchange();
  move2(83450,236710);
  xend();
  xtoolchange();
  move2(87260,236710);
  xend();
  xtoolchange();
  move2(91070,236710);
  xend();
  xtoolchange();
  move2(94880,236710);
  xend();
  xtoolchange();
  move2(98690,236710);
  xend();
  xtoolchange();
  move2(102500,236710);
  xend();
  xtoolchange();
  move2(106310,236710);
  xend();
  xtoolchange();
  move2(41540,232900);
  xend();
  xtoolchange();
  move2(45350,232900);
  xend();
  xtoolchange();
  move2(49160,232900);
  xend();
  xtoolchange();
  move2(52970,232900);
  xend();
  xtoolchange();
  move2(56780,232900);
  xend();
  xtoolchange();
  move2(60590,232900);
  xend();
  xtoolchange();
  move2(64400,232900);
  xend();
  xtoolchange();
  move2(68210,232900);
  xend();
  xtoolchange();
  move2(72020,232900);
  xend();
  xtoolchange();
  move2(75830,232900);
  xend();
  xtoolchange();
  move2(79640,232900);
  xend();
  xtoolchange();
  move2(83450,232900);
  xend();
  xtoolchange();
  move2(87260,232900);
  xend();
  xtoolchange();
  move2(91070,232900);
  xend();
  xtoolchange();
  move2(94880,232900);
  xend();
  xtoolchange();
  move2(98690,232900);
  xend();
  xtoolchange();
  move2(102500,232900);
  xend();
  xtoolchange();
  move2(106310,232900);
  xend();
  xtoolchange();
  move2(41540,229090);
  xend();
  xtoolchange();
  move2(45350,229090);
  xend();
  xtoolchange();
  move2(49160,229090);
  xend();
  xtoolchange();
  move2(52970,229090);
  xend();
  xtoolchange();
  move2(56780,229090);
  xend();
  xtoolchange();
  move2(60590,229090);
  xend();
  xtoolchange();
  move2(64400,229090);
  xend();
  xtoolchange();
  move2(68210,229090);
  xend();
  xtoolchange();
  move2(72020,229090);
  xend();
  xtoolchange();
  move2(75830,229090);
  xend();
  xtoolchange();
  move2(79640,229090);
  xend();
  xtoolchange();
  move2(83450,229090);
  xend();
  xtoolchange();
  move2(87260,229090);
  xend();
  xtoolchange();
  move2(91070,229090);
  xend();
  xtoolchange();
  move2(94880,229090);
  xend();
  xtoolchange();
  move2(98690,229090);
  xend();
  xtoolchange();
  move2(102500,229090);
  xend();
  xtoolchange();
  move2(106310,229090);
  xend();
  xtoolchange();
  move2(41540,225280);
  xend();
  xtoolchange();
  move2(45350,225280);
  xend();
  xtoolchange();
  move2(49160,225280);
  xend();
  xtoolchange();
  move2(52970,225280);
  xend();
  xtoolchange();
  move2(56780,225280);
  xend();
  xtoolchange();
  move2(60590,225280);
  xend();
  xtoolchange();
  move2(64400,225280);
  xend();
  xtoolchange();
  move2(68210,225280);
  xend();
  xtoolchange();
  move2(72020,225280);
  xend();
  xtoolchange();
  move2(75830,225280);
  xend();
  xtoolchange();
  move2(79640,225280);
  xend();
  xtoolchange();
  move2(83450,225280);
  xend();
  xtoolchange();
  move2(87260,225280);
  xend();
  xtoolchange();
  move2(91070,225280);
  xend();
  xtoolchange();
  move2(94880,225280);
  xend();
  xtoolchange();
  move2(98690,225280);
  xend();
  xtoolchange();
  move2(102500,225280);
  xend();
  xtoolchange();
  move2(106310,225280);
  xend();
  xtoolchange();
  move2(41540,221470);
  xend();
  xtoolchange();
  move2(45350,221470);
  xend();
  xtoolchange();
  move2(49160,221470);
  xend();
  xtoolchange();
  move2(52970,221470);
  xend();
  xtoolchange();
  move2(56780,221470);
  xend();
  xtoolchange();
  move2(60590,221470);
  xend();
  xtoolchange();
  move2(64400,221470);
  xend();
  xtoolchange();
  move2(68210,221470);
  xend();
  xtoolchange();
  move2(72020,221470);
  xend();
  xtoolchange();
  move2(75830,221470);
  xend();
  xtoolchange();
  move2(79640,221470);
  xend();
  xtoolchange();
  move2(83450,221470);
  xend();
  xtoolchange();
  move2(87260,221470);
  xend();
  xtoolchange();
  move2(91070,221470);
  xend();
  xtoolchange();
  move2(94880,221470);
  xend();
  xtoolchange();
  move2(98690,221470);
  xend();
  xtoolchange();
  move2(102500,221470);
  xend();
  xtoolchange();
  move2(106310,221470);
  xend();
  xtoolchange();
  move2(41540,217660);
  xend();
  xtoolchange();
  move2(45350,217660);
  xend();
  xtoolchange();
  move2(49160,217660);
  xend();
  xtoolchange();
  move2(52970,217660);
  xend();
  xtoolchange();
  move2(56780,217660);
  xend();
  xtoolchange();
  move2(60590,217660);
  xend();
  xtoolchange();
  move2(64400,217660);
  xend();
  xtoolchange();
  move2(68210,217660);
  xend();
  xtoolchange();
  move2(72020,217660);
  xend();
  xtoolchange();
  move2(75830,217660);
  xend();
  xtoolchange();
  move2(79640,217660);
  xend();
  xtoolchange();
  move2(83450,217660);
  xend();
  xtoolchange();
  move2(87260,217660);
  xend();
  xtoolchange();
  move2(91070,217660);
  xend();
  xtoolchange();
  move2(94880,217660);
  xend();
  xtoolchange();
  move2(98690,217660);
  xend();
  xtoolchange();
  move2(102500,217660);
  xend();
  xtoolchange();
  move2(106310,217660);
  xend();
  xtoolchange();
  move2(41540,213850);
  xend();
  xtoolchange();
  move2(45350,213850);
  xend();
  xtoolchange();
  move2(49160,213850);
  xend();
  xtoolchange();
  move2(52970,213850);
  xend();
  xtoolchange();
  move2(56780,213850);
  xend();
  xtoolchange();
  move2(60590,213850);
  xend();
  xtoolchange();
  move2(64400,213850);
  xend();
  xtoolchange();
  move2(68210,213850);
  xend();
  xtoolchange();
  move2(72020,213850);
  xend();
  xtoolchange();
  move2(75830,213850);
  xend();
  xtoolchange();
  move2(79640,213850);
  xend();
  xtoolchange();
  move2(83450,213850);
  xend();
  xtoolchange();
  move2(87260,213850);
  xend();
  xtoolchange();
  move2(91070,213850);
  xend();
  xtoolchange();
  move2(94880,213850);
  xend();
  xtoolchange();
  move2(98690,213850);
  xend();
  xtoolchange();
  move2(102500,213850);
  xend();
  xtoolchange();
  move2(106310,213850);
  xend();
  xtoolchange();
  move2(41540,210040);
  xend();
  xtoolchange();
  move2(45350,210040);
  xend();
  xtoolchange();
  move2(49160,210040);
  xend();
  xtoolchange();
  move2(52970,210040);
  xend();
  xtoolchange();
  move2(56780,210040);
  xend();
  xtoolchange();
  move2(60590,210040);
  xend();
  xtoolchange();
  move2(64400,210040);
  xend();
  xtoolchange();
  move2(68210,210040);
  xend();
  xtoolchange();
  move2(72020,210040);
  xend();
  xtoolchange();
  move2(75830,210040);
  xend();
  xtoolchange();
  move2(79640,210040);
  xend();
  xtoolchange();
  move2(83450,210040);
  xend();
  xtoolchange();
  move2(87260,210040);
  xend();
  xtoolchange();
  move2(91070,210040);
  xend();
  xtoolchange();
  move2(94880,210040);
  xend();
  xtoolchange();
  move2(98690,210040);
  xend();
  xtoolchange();
  move2(102500,210040);
  xend();
  xtoolchange();
  move2(106310,210040);
  xend();
  xtoolchange();
  move2(41540,206230);
  xend();
  xtoolchange();
  move2(45350,206230);
  xend();
  xtoolchange();
  move2(49160,206230);
  xend();
  xtoolchange();
  move2(52970,206230);
  xend();
  xtoolchange();
  move2(56780,206230);
  xend();
  xtoolchange();
  move2(60590,206230);
  xend();
  xtoolchange();
  move2(64400,206230);
  xend();
  xtoolchange();
  move2(68210,206230);
  xend();
  xtoolchange();
  move2(72020,206230);
  xend();
  xtoolchange();
  move2(75830,206230);
  xend();
  xtoolchange();
  move2(79640,206230);
  xend();
  xtoolchange();
  move2(83450,206230);
  xend();
  xtoolchange();
  move2(87260,206230);
  xend();
  xtoolchange();
  move2(91070,206230);
  xend();
  xtoolchange();
  move2(94880,206230);
  xend();
  xtoolchange();
  move2(98690,206230);
  xend();
  xtoolchange();
  move2(102500,206230);
  xend();
  xtoolchange();
  move2(106310,206230);
  xend();
  xtoolchange();
  move2(41540,202420);
  xend();
  xtoolchange();
  move2(45350,202420);
  xend();
  xtoolchange();
  move2(49160,202420);
  xend();
  xtoolchange();
  move2(52970,202420);
  xend();
  xtoolchange();
  move2(56780,202420);
  xend();
  xtoolchange();
  move2(60590,202420);
  xend();
  xtoolchange();
  move2(64400,202420);
  xend();
  xtoolchange();
  move2(68210,202420);
  xend();
  xtoolchange();
  move2(72020,202420);
  xend();
  xtoolchange();
  move2(75830,202420);
  xend();
  xtoolchange();
  move2(79640,202420);
  xend();
  xtoolchange();
  move2(83450,202420);
  xend();
  xtoolchange();
  move2(87260,202420);
  xend();
  xtoolchange();
  move2(91070,202420);
  xend();
  xtoolchange();
  move2(94880,202420);
  xend();
  xtoolchange();
  move2(98690,202420);
  xend();
  xtoolchange();
  move2(102500,202420);
  xend();
  xtoolchange();
  move2(106310,202420);
  xend();
  xtoolchange();
  move2(41540,198610);
  xend();
  xtoolchange();
  move2(45350,198610);
  xend();
  xtoolchange();
  move2(49160,198610);
  xend();
  xtoolchange();
  move2(52970,198610);
  xend();
  xtoolchange();
  move2(56780,198610);
  xend();
  xtoolchange();
  move2(60590,198610);
  xend();
  xtoolchange();
  move2(64400,198610);
  xend();
  xtoolchange();
  move2(68210,198610);
  xend();
  xtoolchange();
  move2(72020,198610);
  xend();
  xtoolchange();
  move2(75830,198610);
  xend();
  xtoolchange();
  move2(79640,198610);
  xend();
  xtoolchange();
  move2(83450,198610);
  xend();
  xtoolchange();
  move2(87260,198610);
  xend();
  xtoolchange();
  move2(91070,198610);
  xend();
  xtoolchange();
  move2(94880,198610);
  xend();
  xtoolchange();
  move2(98690,198610);
  xend();
  xtoolchange();
  move2(102500,198610);
  xend();
  xtoolchange();
  move2(106310,198610);
  xend();
  xtoolchange();
  move2(41540,194800);
  xend();
  xtoolchange();
  move2(45350,194800);
  xend();
  xtoolchange();
  move2(49160,194800);
  xend();
  xtoolchange();
  move2(52970,194800);
  xend();
  xtoolchange();
  move2(56780,194800);
  xend();
  xtoolchange();
  move2(60590,194800);
  xend();
  xtoolchange();
  move2(64400,194800);
  xend();
  xtoolchange();
  move2(68210,194800);
  xend();
  xtoolchange();
  move2(72020,194800);
  xend();
  xtoolchange();
  move2(75830,194800);
  xend();
  xtoolchange();
  move2(79640,194800);
  xend();
  xtoolchange();
  move2(83450,194800);
  xend();
  xtoolchange();
  move2(87260,194800);
  xend();
  xtoolchange();
  move2(91070,194800);
  xend();
  xtoolchange();
  move2(94880,194800);
  xend();
  xtoolchange();
  move2(98690,194800);
  xend();
  xtoolchange();
  move2(102500,194800);
  xend();
  xtoolchange();
  move2(106310,194800);
  xend();
  xtoolchange();
  move2(41540,190990);
  xend();
  xtoolchange();
  move2(45350,190990);
  xend();
  xtoolchange();
  move2(49160,190990);
  xend();
  xtoolchange();
  move2(52970,190990);
  xend();
  xtoolchange();
  move2(56780,190990);
  xend();
  xtoolchange();
  move2(60590,190990);
  xend();
  xtoolchange();
  move2(64400,190990);
  xend();
  xtoolchange();
  move2(68210,190990);
  xend();
  xtoolchange();
  move2(72020,190990);
  xend();
  xtoolchange();
  move2(75830,190990);
  xend();
  xtoolchange();
  move2(79640,190990);
  xend();
  xtoolchange();
  move2(83450,190990);
  xend();
  xtoolchange();
  move2(87260,190990);
  xend();
  xtoolchange();
  move2(91070,190990);
  xend();
  xtoolchange();
  move2(94880,190990);
  xend();
  xtoolchange();
  move2(98690,190990);
  xend();
  xtoolchange();
  move2(102500,190990);
  xend();
  xtoolchange();
  move2(106310,190990);
  xend();
  xtoolchange();
  move2(41540,187180);
  xend();
  xtoolchange();
  move2(45350,187180);
  xend();
  xtoolchange();
  move2(49160,187180);
  xend();
  xtoolchange();
  move2(52970,187180);
  xend();
  xtoolchange();
  move2(56780,187180);
  xend();
  xtoolchange();
  move2(60590,187180);
  xend();
  xtoolchange();
  move2(64400,187180);
  xend();
  xtoolchange();
  move2(68210,187180);
  xend();
  xtoolchange();
  move2(72020,187180);
  xend();
  xtoolchange();
  move2(75830,187180);
  xend();
  xtoolchange();
  move2(79640,187180);
  xend();
  xtoolchange();
  move2(83450,187180);
  xend();
  xtoolchange();
  move2(87260,187180);
  xend();
  xtoolchange();
  move2(91070,187180);
  xend();
  xtoolchange();
  move2(94880,187180);
  xend();
  xtoolchange();
  move2(98690,187180);
  xend();
  xtoolchange();
  move2(102500,187180);
  xend();
  xtoolchange();
  move2(106310,187180);
  xend();
  xtoolchange();
  move2(41540,183370);
  xend();
  xtoolchange();
  move2(45350,183370);
  xend();
  xtoolchange();
  move2(49160,183370);
  xend();
  xtoolchange();
  move2(52970,183370);
  xend();
  xtoolchange();
  move2(56780,183370);
  xend();
  xtoolchange();
  move2(60590,183370);
  xend();
  xtoolchange();
  move2(64400,183370);
  xend();
  xtoolchange();
  move2(68210,183370);
  xend();
  xtoolchange();
  move2(72020,183370);
  xend();
  xtoolchange();
  move2(75830,183370);
  xend();
  xtoolchange();
  move2(79640,183370);
  xend();
  xtoolchange();
  move2(83450,183370);
  xend();
  xtoolchange();
  move2(87260,183370);
  xend();
  xtoolchange();
  move2(91070,183370);
  xend();
  xtoolchange();
  move2(94880,183370);
  xend();
  xtoolchange();
  move2(98690,183370);
  xend();
  xtoolchange();
  move2(102500,183370);
  xend();
  xtoolchange();
  move2(106310,183370);
  xend();
  xtoolchange();
  move2(41540,179560);
  xend();
  xtoolchange();
  move2(45350,179560);
  xend();
  xtoolchange();
  move2(49160,179560);
  xend();
  xtoolchange();
  move2(52970,179560);
  xend();
  xtoolchange();
  move2(56780,179560);
  xend();
  xtoolchange();
  move2(60590,179560);
  xend();
  xtoolchange();
  move2(64400,179560);
  xend();
  xtoolchange();
  move2(68210,179560);
  xend();
  xtoolchange();
  move2(72020,179560);
  xend();
  xtoolchange();
  move2(75830,179560);
  xend();
  xtoolchange();
  move2(79640,179560);
  xend();
  xtoolchange();
  move2(83450,179560);
  xend();
  xtoolchange();
  move2(87260,179560);
  xend();
  xtoolchange();
  move2(91070,179560);
  xend();
  xtoolchange();
  move2(94880,179560);
  xend();
  xtoolchange();
  move2(98690,179560);
  xend();
  xtoolchange();
  move2(102500,179560);
  xend();
  xtoolchange();
  move2(106310,179560);
  xend();
  xtoolchange();
  move2(41540,175750);
  xend();
  xtoolchange();
  move2(45350,175750);
  xend();
  xtoolchange();
  move2(49160,175750);
  xend();
  xtoolchange();
  move2(52970,175750);
  xend();
  xtoolchange();
  move2(56780,175750);
  xend();
  xtoolchange();
  move2(60590,175750);
  xend();
  xtoolchange();
  move2(64400,175750);
  xend();
  xtoolchange();
  move2(68210,175750);
  xend();
  xtoolchange();
  move2(72020,175750);
  xend();
  xtoolchange();
  move2(75830,175750);
  xend();
  xtoolchange();
  move2(79640,175750);
  xend();
  xtoolchange();
  move2(83450,175750);
  xend();
  xtoolchange();
  move2(87260,175750);
  xend();
  xtoolchange();
  move2(91070,175750);
  xend();
  xtoolchange();
  move2(94880,175750);
  xend();
  xtoolchange();
  move2(98690,175750);
  xend();
  xtoolchange();
  move2(102500,175750);
  xend();
  xtoolchange();
  move2(106310,175750);
  xend();
  xtoolchange();
  rotate(true);
  move2(101230,160510);
  rotate(false);
  xend();
  xtoolchange();
  rotate(true);
  move2(87260,160510);
  rotate(false);
  xend();
  xtoolchange();
  move2(65670,159240);
  xend();
  xtoolchange();
  move2(50430,152890);
  xend();
  xtoolchange();
  move2(46620,154160);
  xend();
  xtoolchange();
  move2(50430,165590);
  xend();
  xtoolchange();
  move2(83450,159240);
  xend();
  xtoolchange();
  move2(97420,159240);
  xend();
}


void stdTest( void )
{

  xtoolchange();
  move2(50188,181426);
  xend();

  xtoolchange();
  move2(53484,181426);
  xend();
  xtoolchange();
  move2(59192,185480);
  xend();
  xtoolchange();
  move2(61683,185480);
  xend();

  xtoolchange();
  rotate(true);
  move2(41929,209188);
  xend();
  rotate(false);

  xtoolchange();
  rotate(true);
  move2(59200,181508);
  rotate(false);
  xend();
  xtoolchange();
  center(true);
  delay(10);
  center(false);
  move2(54666,202537);
  xend();
  xtoolchange();
  move2(62896,210238);
  xend();
  xtoolchange();
  move2(57408,178552);
  xend();
  xtoolchange();
  move2(62300,179442);
  xend();
  xtoolchange();
  move2(67033,165146);
  xend();
  xtoolchange();
  move2(60023,165009);
  xend();
  xtoolchange();
  move2(61312,161566);
  xend();
  xtoolchange();
  move2(57086,215584);
  xend();
  xtoolchange();
  move2(54712,158199);
  xend();
  xtoolchange();
  move2(59676,158199);
  xend();
  xtoolchange();
  move2(63939,220169);
  xend();
  xtoolchange();
  move2(50996,200426);
  xend();
  xtoolchange();
  move2(58610,172304);
  xend();
  xtoolchange();
  move2(66726,192156);
  xend();
  xtoolchange();
  move2(65697,172314);
  xend();
  xtoolchange();
  move2(51717,171414);
  xend();
  xtoolchange();
  move2(61902,192979);
  xend();
  xtoolchange();
  move2(67503,211651);
  xend();
  xtoolchange();
  move2(61539,216191);
  xend();
  xtoolchange();
  move2(57725,216191);
  xend();
  xtoolchange();
  move2(55834,216191);
  xend();
  xtoolchange();
  move2(62991,202407);
  xend();
  xtoolchange();
  move2(40926,218469);
  xend();
  xtoolchange();
  move2(68576,199343);
  xend();
  xtoolchange();
  move2(51712,158199);
  xend();
  xtoolchange();
  move2(68767,159693);
  xend();
  xtoolchange();
  move2(49848,165338);
  xend();
  xtoolchange();
  move2(54996,224363);
  xend();

}


