
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

// for the watchdog based reset ( which didn't seem to work)
#include <avr/io.h>
#include <avr/wdt.h>

// Pin mappings

#if 1

// new board

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

#define HEADDN     ( 36 )
#define DEG90      ( 37 )
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

#else

// oldbboard

// xy lIMITS
#define XL1 ( 23 )
#define YL1 ( 27 ) 
#define XL2 ( 22 ) 
#define YL2 ( 28 ) 
#define XL3 ( 24 ) 
#define YL3 ( 29 ) 
#define XL4 ( 25 ) 
#define YL4 ( 30 ) 

// x/y home
#define XHM         ( 26 ) 
#define YHM         ( 41 ) 

#define HEADDN      ( 1 )
#define DEG90       ( 2 )
#define TACSENSE    ( 3 )
#define PCBDETECTOR ( )
#define PCBIN ( )
#define PCBOUT  ( )
#define PINUP ( )
#define READYIN ( )
#define DISPENSEREND ( )
#define MORG ( 11 )
#define MCENT ( )

#define CWSW ( )
#define CCWSW ( )

#define XPLUS  ( 40 )
#define XMINUS ( 39 )
#define YPLUS  ( 0  )
#define YMINUS ( 11 )


#define FAST ( 15 )
#define TEACH ( 16 )
#define T_VAC ( 17 )
// switch on touch pad
#define T_HEAD ( 18 )

#define SPAREA ( )
#define SPAREB ( )
#define SPAREC ( )

#define SPARE7 ( )
#define SPARE8 ( )
#define SPOT ( 52 )
#define VCHANGE( )
#define DISPENSERSTART ( )
#define CVYM( )
#define STOPPER ( )
#define LOCATOR ( )
#define SUPPORTER ( )
#define VAC ( 45 ) 
#define HEAD ( 44 ) 
#define ROT (43 ) 
#define CENTERING (42  )
#define TAPEKNOCK ( A14 )
#define SPARE6 ( A13 )

#define ACENT ( )
#define SPARE1 ( A7 )
#define SPARE2 ( A8 )
#define SPARE3 ( A9 )
#define SPARE4 ( A10)
#define SPARE5 ( A11)
#define READYOUT ( )
#define CW_CCW ( )
#define PULSE ( )

// motors
#define YCW ( A2 )
#define YCCW ( A3 )
#define XCW ( A0 )
#define XCCW ( A1 )

#endif

// Maximum number of pulses before machine has hit a hard limit
#define MAX_X_PULSES ( 14868L )
#define MAX_Y_PULSES ( 21172L )


// maximum speed the motors can travel at
#define X_SPEED ( 70 )
#define Y_SPEED ( 80 )

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

const unsigned short DELAY_X1 =(150*2); // 148 lowest (maybe 150)
const unsigned short DELAY_X2 =(150*2);
const unsigned short DELAY_Y1 =(208*2); //200 lowest
const unsigned short DELAY_Y2 =(208*2);

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
long gCurrentXum = 0;
long gCurrentYum = 0;

// machine homed ?
boolean homed = false;

// This should cause a proper reset it doesn't.
#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}

/////////////////////////////////////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////////////////////////////////////

void setup( void ) {
    
    homed = false;
    
    // Set all pins as input
    for( int i = 0; i < 54 ; i ++ ) {

        pinMode(i, INPUT);   
    }

    // Serial port init
    Serial.begin(9600);

    Serial.println("PNP Test\n");

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

    // some test code.
    Serial.print("25mm = pulses = ");
    Serial.println(mmtopulses(25),DEC); 
    Serial.print("25,000um = pulses = ");
    Serial.println(umtopulses(25000),DEC); 
    Serial.print("1,000 pulses = um ");
    Serial.println(pulsestoum(1000),DEC); 
    Serial.print("1,000 pulses = mm ");
    Serial.println(pulsestomm(1000),DEC); 

    Serial.print(" x max mm=");
    Serial.println(pulsestomm(MAX_X_PULSES*1000),DEC);
    Serial.print(" y max mm=");
    Serial.println(pulsestomm(MAX_Y_PULSES*1000),DEC);
    Serial.println("");

    // setup the limit switches
    attachInterrupt(XL1, x1Limit, LOW);
    attachInterrupt(XL2, x2Limit, LOW);
    attachInterrupt(YL1, y1Limit, LOW);
    attachInterrupt(YL2, y2Limit, LOW);
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
    xLimit1 = true;
}

void x2Limit( void ) 
{
    homed = false;
    xLimit2 = true;
}

void y1Limit( void ) 
{
    homed = false;
    yLimit1 = true;
}

void y2Limit( void ) 
{
    homed = false;
    yLimit2 = true;
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
    headSwitch     = !digitalRead( T_HEAD ) ;
    
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

    // Read the home key, and home machine if set
    if( !digitalRead( MORG ) == true   ) {
        home();
    }

    // if teach is held down, toggle spot light 
    if( teachSwitch == true && lastTeach == false ) {

        // toggle state
        spotState = 1 - spotState;
        spot(spotState);
    }

    // remember last setting
    lastTeach = teachSwitch;

    // if vac is held down, toggle vacuum
    if( vacSwitch == true && lastVac== false ) {

        // toggle state
        vacuumState = 1 - vacuumState;
        vacuum(vacuumState);
    }

    // remember last setting
    lastVac = vacSwitch;
    
    // if head is held down, toggle head
    if( headSwitch == true && lastHead == false ) {

        // toggle state
        headState = 1 - headState;
        head(headState);
    }

    // remember last setting
    lastHead = headSwitch;
    
    
    // handle cursor keys
    if( xPlus  == true ) goright(fspeed,1);
    if( xMinus == true ) goleft(fspeed,1);
    if( yPlus  == true ) goback(fspeed,1);
    if( yMinus == true ) goforward(fspeed,1);
    
}

// limits should be set by interrupts now
unsigned int readLimitSwitches( void ) 
{
    unsigned char  limit;

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
#ifndef NDEBUG
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
    Serial.println("gXP, gYP");
    Serial.print(gXPulses,DEC);
    Serial.print(" ");
    Serial.print(gYPulses,DEC);
    Serial.println("");
#endif

    // update the global position in micrometers
    gCurrentXum = pulsestoum( gXPulses ) ;
    gCurrentYum = pulsestoum( gYPulses ) ;

#ifndef NDEBUG
    Serial.print("cx,cy = ");
    Serial.print(gCurrentXum);
    Serial.print(" ");
    Serial.println(gCurrentYum);
#endif

    // if any of the limit switches are reached, machine is no longer considered to be homed
    // set in interrupts
    if( limit ) {

#ifndef NDEBUG
        Serial.println("out of home!!");
#endif

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

/* handle the tool changer */

void tool( unsigned char on_off, unsigned char toolid ) 
{
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
            digitalWrite(SPARE3,LOW);  
            ;
            digitalWrite(SPARE4,LOW);  
            ;
            digitalWrite(SPARE5,LOW);  
            ;          
            break;
        case 2 :
            digitalWrite(SPARE3,LOW);  
            ;
            digitalWrite(SPARE4,LOW);  
            ;
            digitalWrite(SPARE5,HIGH);  
            ;          
            break;
        case 3 :
            digitalWrite(SPARE3,LOW);  
            ;
            digitalWrite(SPARE4,HIGH);  
            ;
            digitalWrite(SPARE5,LOW);  
            ;          
            break;
        case 4 :
            digitalWrite(SPARE3,LOW);  
            ;
            digitalWrite(SPARE4,HIGH);  
            ;
            digitalWrite(SPARE5,HIGH);  
            ;          
            break;
        case 5 :
            digitalWrite(SPARE3,HIGH);  
            ;
            digitalWrite(SPARE4,LOW);  
            ;
            digitalWrite(SPARE5,LOW);  
            ;          
            break;
        case 6 :
            digitalWrite(SPARE3,HIGH);  
            ;
            digitalWrite(SPARE4,LOW);  
            ;
            digitalWrite(SPARE5,HIGH);  
            ;          
            break;
        }
    if( on_off ) { 
        digitalWrite(SPARE2,LOW);
    } 
    else {
        digitalWrite(SPARE2,HIGH);
    }
}

// get the state of the 90 degree roration?
boolean degree90( void ) 
{
    return !digitalRead(DEG90);
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
    } else {
        digitalWrite(VAC,HIGH);
    }   
}

// return the status of the head
boolean headDown( void ) 
{
    return !digitalRead( HEADDN ) ;
} 


void loop() 
{
    char sbyte;

    if (Serial.available() > 0) {
        
        // read the incoming byte:
        sbyte = Serial.read();

        switch( sbyte ) 
        {
        case '*':
            Reset_AVR();
            break;
            
        case 'A':
            gotoxy(25000,25000);
            break;
        case 'a':
            gotoxy(150000,150000);
            break;
        case 'K':
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
            delay(500);
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

        case 'z':          
            tool(1,1);
            delay(3);
            tool(1,2);
            delay(3);
            tool(1,3);
            delay(3);
            tool(1,4);
            delay(3);
            tool(1,5);
            delay(3);
            tool(1,6);
            delay(3);
            tool(1,5);
            delay(3);
            tool(1,4);
            delay(3);
            tool(1,3);
            delay(3);
            tool(1,2);
            delay(3);
            tool(1,1);
            delay(3);

            tool(0,0);
            break;

        case 'f':
            goright(1,1);
            break;
        case 'F':
            goleft(1,1);
            break;

        case 'q':
            Serial.println("q");
            //goback(1,1);
            break;
        case 'Q':
            goforward(1,1);
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
            findyminuslimit();
            break;
        case '[':
            findleftlimit();
            break;
        case ']':
            findrightlimit();
            break;

        case 'g':
            Serial.println( headDown(),HEX);
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

    // reset the pulse counter 
    gXPulses = -1;
    gYPulses = -1;

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

        if( digitalRead( XL2 )) {
            digitalWrite(XCCW,HIGH);
            break;
        }

        // hit a limit
        if( digitalRead( YL2 )) {
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
        delayMicroseconds(length--);   

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
        if( digitalRead( XL1 )) {
            digitalWrite(XCW,HIGH);
            return;
        }

        if( xhomed == false ) {
            stepXCW(DELAY_X2+DELAY_X1);
        }

    }

    digitalWrite(XCW,HIGH);

    for(int i = 0 ; i < 1100 ; i++ ) {

        if( digitalRead( YHM )) {
            yhomed = 1;
            break;
        }
        // hit a limit
         // failed
        if( digitalRead( YL1 )) {
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
    Serial.println("Walk mode enabled, press a key to stop after limit reached");
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
        digitalWrite(ROT,HIGH);

}

boolean head( boolean on )
{
#ifndef NDEBUG
    Serial.println("head");
#endif
    if( on ) {
        digitalWrite(HEAD,LOW);  
        headState = true;
    } else {
        digitalWrite(HEAD,HIGH);
        headState = false;
    }

    delay( 1 );
    
    //if the head down sensor doesn't equal the headState, something is wrong
    if (headDown() != headState ) {
        // problem!
        return false;
    }
        
    return true;
}

char findlefthome(void) {

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

        stepXCW( DELAY_Y1 + DELAY_Y2);
        counter++;
    }  

    // reset it back
    digitalWrite(XCW,HIGH);

    return true;
}

char findleftlimit(void) 
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

char findrightlimit(void) {

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

char findyplushome(void) {

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
    head( false );
}



// Step once forward
void stepYCW( long length ) 
{    
    beforeMoving();
    
    digitalWrite(YCW,HIGH);
    delayMicroseconds(SHORT_Y_PULSE); 
    digitalWrite(YCW,LOW);
    delayMicroseconds(length); 

    DecrementYPulses();
}

// Step once back
void stepYCCW( long pulselength ) 
{
    beforeMoving();

    digitalWrite(YCCW,HIGH);
    delayMicroseconds(SHORT_Y_PULSE); 
    digitalWrite(YCCW,LOW);
    delayMicroseconds(pulselength);  

    IncrementYPulses();
}

// Step once left
void stepXCW( long pulselength ) 
{
    beforeMoving();

    digitalWrite(XCW,HIGH);
    delayMicroseconds(SHORT_X_PULSE); 
    digitalWrite(XCW,LOW);
    delayMicroseconds(pulselength); 

    DecrementXPulses();
}

// Step once right
void stepXCCW( long pulselength ) 
{
    beforeMoving();

    digitalWrite(XCCW,HIGH);
    delayMicroseconds(SHORT_X_PULSE); 
    digitalWrite(XCCW,LOW);
    delayMicroseconds(pulselength);    

    IncrementXPulses();
}

char findypluslimit(void) {

    unsigned long counter = 0;
    unsigned int length = DELAY_Y1 + DELAY_Y2;

    while( 1 ) {

        if(  digitalRead( YL1 )) {

            digitalWrite(YCW,HIGH);

            homed = 0;
            Serial.print("Found Y+ limit = ");
            Serial.println(counter,DEC);
            delay(500);

            // reset Y micrometers
            gCurrentYum =  0;

            return false;
        }

        stepYCW(length--);

        counter++;
        if (length <= Y_SPEED ) 
            length = Y_SPEED;
    }  
}

char findyminuslimit(void) {

    unsigned long counter = 0;
    unsigned int length = DELAY_Y1 + DELAY_Y2;

    while( 1 ) {

        if(  digitalRead( YL2 )) {

            digitalWrite(YCCW,HIGH);
            delay(500);

            homed = false ;
            Serial.print("Found Y- limit = ");
            Serial.println(counter,DEC);
            return false;
        }

        stepYCCW(length--);
        counter++;

        if (length <= Y_SPEED ) length = Y_SPEED;
    }  
}

char goleft(long steps ,boolean nolimit) {

    for( int y = 0 ; y < steps ; y ++ )
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

        stepXCW(DELAY_X1+DELAY_X2);
    }  

    // reset it back
    digitalWrite(XCW,HIGH);

    return true;
}

// pulses = ( microm * 40000 )

// conversions betweem mm,um and pusles
long mmtopulses( long mm ) 
{
    return ( mm *  40 ) ;
}

long umtopulses( long microm ) 
{
    return ( microm /  25 ) ;
}

long pulsestoum( long pulses ) 
{
    return ( pulses *  25 ) ;
}

long pulsestomm( long pulses ) 
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

char gotoxy(long xum,long yum)
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


void moveLine(int x0, int y0, int x1, int y1)
{
    int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
    int err = dx+dy, e2; /* error value e_xy */

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

boolean goright(long steps ,boolean nolimit ) 
{

    for( int x = 0 ; x < steps ; x ++ )
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

        stepXCCW( DELAY_X2+DELAY_X1 );
    }  

    // reset it back
    digitalWrite(XCCW,HIGH);

    return true;        
}

boolean goforward(long steps ,boolean nolimit ) {

    for( int y = 0 ; y < steps ; y ++ )
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
        stepYCW( DELAY_Y2 + DELAY_Y1 );
    }  

    // reset it back
    digitalWrite(YCW,HIGH);
    return true;
}


boolean goback(long steps ,boolean nolimit ) {

    for( int y = 0 ; y < steps ; y ++ )
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
        stepYCCW( DELAY_Y2 + DELAY_Y1 ); 
    }  
    

    // reset it back
    digitalWrite(YCCW,HIGH);

    return true;        
}

void IncrementXPulses( void ) 
{
    if (homed == false ) 
        return; 

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
    if (homed == false ) 
        return; 

    gXPulses --;

    if( gXPulses < 0 ) {
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
    if (homed == false )
        return; 

    gYPulses ++;

    if( gYPulses >= MAX_Y_PULSES ) {
        Serial.print("Error too many Y pulses, must be loosing some ");
        Serial.print(gXPulses,DEC);
        Serial.println(gYPulses,DEC);

        homed = false;
        return;
    }
}

void DecrementYPulses( void ) 
{
    if (homed == false ) 
        return; 

    gYPulses --;

    if( gYPulses < 0 ) {
        Serial.print("Error negative Y pulses, must be loosing some ");
        Serial.print(gXPulses,DEC);
        Serial.println(gYPulses,DEC);

        homed = false;
        return;
    }
}
