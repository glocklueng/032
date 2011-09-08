// Pin mappings

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
#define XHM     ( 26 ) 
#define YHM      ( 41 ) 

#define HEADDN     ( 1 )
#define DEG90      ( 2 )
#define TACSENSE   (3 )
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

// maximum number of pulses before machine has hit a hard limit
#define MAX_X_PULSES ( 14868L )
#define MAX_Y_PULSES ( 21172L )


// maximum speed the motors can travel
#define X_SPEED ( 70 )
#define Y_SPEED ( 80 )

// length of pulse
#define SHORT_X_PULSE ( 1 )
#define SHORT_Y_PULSE ( 1 )


// limits
unsigned char xLimit1=0,xLimit2=0,xLimit3=0,yLimit1=0,yLimit2=0,yLimit3=0;
unsigned char xPlus=0,xMinus=0,yMinus=0,yPlus=0,fast=0,teach=0,lastteach=0;

// home sensors
unsigned char xHome=0,yHome=0;
unsigned char  tacsense;

// motor speed pulses

// motors need |__

const unsigned int DELAY_X1 =(150*2); // 148 lowest (maybe 150)
const unsigned int DELAY_X2 =(150*2);
const unsigned int DELAY_Y1 =(208*2); //200 lowest
const unsigned int DELAY_Y2 =(208*2);

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

// homed?
boolean homed = false;

/////////////////////////////////////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////////////////////////////////////

void setup( void ) {


	// Set all pinsd as input
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
	Serial.print("25mm = pulses =");
	Serial.print(mmtopulses(25),DEC); 
	Serial.print(" x max mm=");
	Serial.print(pulsestomm(MAX_X_PULSES*1000),DEC);
	Serial.print(" y max mm=");
	Serial.print(pulsestomm(MAX_Y_PULSES*1000),DEC);
	Serial.println("");

}

// read the front panel switches (on the beta board yMinus is mapped to the arudinos serial port)
void readPanel( void )
{
        // number of pulses to move by
	unsigned char fspeed = 1;

        // read state of cursor keys
	xPlus  =  digitalRead( XPLUS );
	xMinus =  digitalRead( XMINUS );
	yPlus  =  digitalRead( YPLUS );
	yMinus =  digitalRead( FAST );
	//fast    = !digitalRead( FAST );
	teach   = !digitalRead( TEACH );

        // don't allow both
        if( xPlus == 0 && xMinus == 0 ) {
          xMinus = 1;
        }
        
        // don't allow both
        if( yPlus == 0 && yMinus == 0 ) {
           yMinus = 1;
        }
        
	// fast isn't enabled.
	if( fast ) {
		fspeed = 1;
	}

        // Read the home key, and home machine if set
	if( digitalRead( MORG ) == 0   ) {
		home();
	}

        // if teach is held down, toggle spot light 
	if( teach == 1 ) {

  		lastteach = 1 - lastteach;

		spot(lastteach);

		// simple 'debounce'
		delay(100);
	}

        // remember last setting
	lastteach = teach;

	if( digitalRead( T_VAC ) == 0 ) {
		vacuum( 1) ;
	}
	if( digitalRead( T_HEAD ) == 0 ) {
		head( 1) ;
	}
        // handle cursor keys
	if( !xPlus ) goright(fspeed,1);
	if( !xMinus ) goleft(fspeed,1);
	if( yPlus == 0 ) goback(fspeed,1);
	if( !yMinus ) goforward(fspeed,1);
}

unsigned int readLimitSwitches( void ) 
{
	unsigned int  limit;

        // Read limit switches
	xLimit1 =  digitalRead( XL1 ); limit  = xLimit1;
	xLimit2 =  digitalRead( XL2 ); limit += xLimit2;
	xLimit3 =  digitalRead( XL3 ); limit += xLimit3;

	yLimit1 =  digitalRead( YL1 ); limit += yLimit1;
	yLimit2 =  digitalRead( YL2 ); limit += yLimit2;
	yLimit3 =  digitalRead( YL3 ); limit += yLimit3;

	// not counted as a limit switche, since it'll upset the home
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
	Serial.println("XP, YP");
	Serial.print(gXPulses,DEC);
	Serial.print(" ");
	Serial.print(gYPulses,DEC);
	Serial.println("");

	// if any of the limit switches are reached, machine is no longer considerd to be homed!
	if( limit ) {
                Serial.println("out of home!!");
		homed = false;
	}

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
		digitalWrite(SPARE3,HIGH);  ;
		digitalWrite(SPARE4,HIGH);  ;
		digitalWrite(SPARE5,HIGH);  ;          
		break;
	case 1 :
		digitalWrite(SPARE3,LOW);  ;
		digitalWrite(SPARE4,LOW);  ;
		digitalWrite(SPARE5,LOW);  ;          
		break;
	case 2 :
		digitalWrite(SPARE3,LOW);  ;
		digitalWrite(SPARE4,LOW);  ;
		digitalWrite(SPARE5,HIGH);  ;          
		break;
	case 3 :
		digitalWrite(SPARE3,LOW);  ;
		digitalWrite(SPARE4,HIGH);  ;
		digitalWrite(SPARE5,LOW);  ;          
		break;
	case 4 :
		digitalWrite(SPARE3,LOW);  ;
		digitalWrite(SPARE4,HIGH);  ;
		digitalWrite(SPARE5,HIGH);  ;          
		break;
	case 5 :
		digitalWrite(SPARE3,HIGH);  ;
		digitalWrite(SPARE4,LOW);  ;
		digitalWrite(SPARE5,LOW);  ;          
		break;
	case 6 :
		digitalWrite(SPARE3,HIGH);  ;
		digitalWrite(SPARE4,LOW);  ;
		digitalWrite(SPARE5,HIGH);  ;          
		break;
	}
	if( on_off ) { 
		digitalWrite(SPARE2,LOW);
	} else {
		digitalWrite(SPARE2,HIGH);
	}
}

// get the state of the 90 degree roration?
unsigned char degree90( void ) 
{
	return digitalRead(DEG90);
}

void vacuum( unsigned char on_off ) 
{
	static unsigned char lastState = 0xff;

	if( lastState == on_off ) {
		return;
	}

	lastState = on_off;

	Serial.println("Vacuum");

	if( on_off ) { 
		digitalWrite(VAC,LOW);
	} else {
		digitalWrite(VAC,HIGH);
	}   
}

// return the status of the head
unsigned char headDown( void ) 
{
	return digitalRead( HEADDN ) ;
} 


void loop() 
{
	char sbyte;

	readPanel();

	if (Serial.available() > 0) {
		// read the incoming byte:
		sbyte = Serial.read();

		switch( sbyte ) 
		{
		case 'A':
			home();
			gotoxyp(12275-40 + (20*40),0);
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
			rotate();break;
		case 'c':
			center();break;          
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
			vacuum(0);          break;
		case 'V':          
			vacuum(1);          break;

		case 'w':
			walk();break;

		case 'z':          
			tool(1,1);delay(3);
			tool(1,2);delay(3);
			tool(1,3);delay(3);
			tool(1,4);delay(3);
			tool(1,5);delay(3);
			tool(1,6);delay(3);
			tool(1,5);delay(3);
			tool(1,4);delay(3);
			tool(1,3);delay(3);
			tool(1,2);delay(3);
			tool(1,1);delay(3);

			tool(0,0);
			break;

		case 'f':
			goright(1,1);break;
		case 'F':
			goleft(1,1);break;

		case 'q':
			goback(1,1);break;
		case 'Q':
			goforward(1,1);break;

		case '8':
			goright(500,1);break;
		case '9':
			goleft(500,1);break;

		case '1':
			tool(1,1);break;
		case '2':
			tool(1,2);break;
		case '3':
			tool(1,3);break;
		case '4':
			tool(1,4);break;
		case '5':
			tool(1,5);break;
		case '6':
			tool(1,6);break;
		case '0':
			tool(0,0);break;

		case '.':
			spot(1);
			break;
		case ',':
			spot(0);
			break;     

		case '/':
			centerof();break;

		case 'o':
			findypluslimit();break;
		case 'p':
			findyminuslimit();break;
		case '[':
			findleftlimit();break;
		case ']':
			findrightlimit();break;

		case 'g':
			Serial.println( headDown(),HEX);break;


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
		}
	}
}

/* 
 *  home function 
 */

void home( void ) 
{
	unsigned char xhomed= 0 , yhomed= 0;

	// reset the pulse counter 
	gXPulses = -1;
	gYPulses = -1;

	Serial.println("Homing machine");
	findleftlimit();
	delay(10);
	findypluslimit();

	homed = false;

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

                STEPxcw
		digitalWrite(XCCW,HIGH);
		delayMicroseconds(1); 
		digitalWrite(XCCW,LOW);
		delayMicroseconds(DELAY_X2+DELAY_X1);   

		delay(1);

		digitalWrite(YCCW,HIGH);
		delayMicroseconds(1); 
		digitalWrite(YCCW,LOW);
		delayMicroseconds(DELAY_Y2+DELAY_Y2); 
	}

	digitalWrite(XCCW,HIGH);
	digitalWrite(YCCW,HIGH);

	readLimitSwitches();

	for(int i = 0 ; i < 1100 ; i++ ) {

		if( digitalRead( XHM )) {
			xhomed = 1;
			break;
		}

		if( digitalRead( XL1 )) {
			digitalWrite(XCW,HIGH);
			return;
		}

		if( xhomed == 0 ) {
			digitalWrite(XCW,HIGH);
			delayMicroseconds(1); 
			digitalWrite(XCW,LOW);
			delayMicroseconds(DELAY_X2+DELAY_X1); 
		}

	}

	digitalWrite(XCW,HIGH);

	for(int i = 0 ; i < 1100 ; i++ ) {

		if( digitalRead( YHM )) {
			yhomed = 1;
			break;
		}
		// hit a limit
		if( digitalRead( YL1 )) {
			digitalWrite(YCW,HIGH);
			return;
		}      

		if( yhomed == 0 ){
			digitalWrite(YCW,HIGH);
			delayMicroseconds(1); 
			digitalWrite(YCW,LOW);
			delayMicroseconds(DELAY_Y2+DELAY_Y1); 
		}
	}

	digitalWrite(YCW,HIGH);

	// read limit switches
	xHome = 0; yHome = 0;
	readLimitSwitches();

	if( !( xHome == 1 && yHome == 1 )) {
		Serial.println("Failed to home");
		homed = false;
		return;
	}

	gXPulses = 0;
	gYPulses = 0;

	Serial.println("HOMED");
	homed = true;
}

void centerof(void)
{
	// light offf
	spot(0);

	// check if homed
	if( homed == 0 ) {

		home();

		if( homed == 0 ) {
			Serial.println("Failed to home");
			return;
		}
	}
	// 2800 = 14mm
	Serial.println("Center");

	Serial.println("X home");
	findlefthome(); readLimitSwitches();
	Serial.println("Y home");
	findyplushome(); readLimitSwitches();

	Serial.println("right");
	goright(2500,1); readLimitSwitches();
	Serial.println("back");
	goback(4600,1); readLimitSwitches();

	if( homed == 0 ) {
		Serial.println("Something went wrong, hit a limit");
		return;
	}

	spot(1);

	Serial.println("centerof done");

}

void walk(void)
{
	Serial.println("Walk mode enabled, press a key to stop after limit reached");
	Serial.read();
	while(1){
		findrightlimit(); readLimitSwitches();
		if (Serial.available() > 0) break;
		findleftlimit(); readLimitSwitches();
		if (Serial.available() > 0) break;
		findyminuslimit(); readLimitSwitches();
		if (Serial.available() > 0) break;
		findrightlimit(); readLimitSwitches();
		if (Serial.available() > 0) break;
		findypluslimit(); readLimitSwitches();
		if (Serial.available() > 0) break;
	}

	while(Serial.available()) {
		Serial.read();
	}

	Serial.println("Test Ended");

}

void tapeKnock( unsigned char on_off )
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

void spot( unsigned char on)
{
	Serial.println("Spot");

	if( on ) 
		digitalWrite(SPOT,LOW);  
	else  
		digitalWrite(SPOT,HIGH);
}

void rotate( void )
{

	digitalWrite(ROT,LOW);  
	delay(100);
	digitalWrite(ROT,HIGH);

}

void center( void )
{
	digitalWrite(CENTERING,LOW);  
	delay(500);
	digitalWrite(CENTERING,HIGH); 
}

void head( unsigned char on )
{
	if( on ) 
		digitalWrite(HEAD,LOW);  
	else  
		digitalWrite(HEAD,HIGH);
}

char findlefthome(void) {

	while(1) {

		if( digitalRead( XL1) ) {

			digitalWrite(XCW,HIGH);

			homed = 0;

			Serial.println("X limit!");

			return false;

		}


		if( digitalRead( XHM ) ) {

			digitalWrite(XCW,HIGH);

			Serial.println("Found left home");

			return false;
		}

		digitalWrite(XCW,HIGH);
		delayMicroseconds(DELAY_X1); 
		digitalWrite(XCW,LOW);
		delayMicroseconds(DELAY_X2); 
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

			homed = 0;
			Serial.print("Found x left limit = ");
			Serial.println(counter,DEC);

                        // Reset Xum counter 
                        gCurrentXum = 0;
                        
			delay(200);
			return false;
		}

		digitalWrite(XCW,HIGH);
		delayMicroseconds(SHORT_X_PULSE); 
		digitalWrite(XCW,LOW);
		delayMicroseconds(length); 

		counter ++ ;

		if (length <= X_SPEED ) length = X_SPEED;
	}  
}

char findrightlimit(void) {

	unsigned long counter = 0;

	unsigned int length = ( DELAY_X1 + DELAY_X2 ) ;

	while( 1 ) {

		if( digitalRead( XL2 ) ) {

			digitalWrite(XCCW,HIGH);
			delay(200);

			homed = 0;
			Serial.print("Found X right limit = ");
			Serial.println(counter,DEC);

			return false;
		}

		digitalWrite(XCCW,HIGH);
		delayMicroseconds(1); 
		digitalWrite(XCCW,LOW);
		delayMicroseconds(length); 

		counter++;

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

			return false;

		}

		//    Serial.print(digitalRead(YHM),HEX);
		if( digitalRead( YHM ) ) {

			digitalWrite(YCW,HIGH);

			Serial.println("Found Y home");

			return false;
		}

		digitalWrite(YCW,HIGH);
		delayMicroseconds(DELAY_Y1); 
		digitalWrite(YCW,LOW);
		delayMicroseconds(DELAY_Y2); 
	}  

	// reset it back
	digitalWrite(YCW,HIGH);

	return true;
}

// Step once forward
void stepYCW( long length ) 
{
    digitalWrite(YCW,HIGH);
    delayMicroseconds(SHORT_Y_PULSE); 
    digitalWrite(YCW,LOW);
    delayMicroseconds(length); 

    DecrementYPulses();
}

// Step once back
void stepYCCW( long length ) 
{
    digitalWrite(YCCW,HIGH);
    delayMicroseconds(SHORT_Y_PULSE); 
    digitalWrite(YCCW,LOW);
    delayMicroseconds(length);  
    
    IncrementYPulses();
}

// Step once left
void stepXCW( long length ) 
{
    digitalWrite(XCW,HIGH);
    delayMicroseconds(SHORT_X_PULSE); 
    digitalWrite(XCW,LOW);
    delayMicroseconds(length); 

    DecrementXPulses();
}

// Step once right
void stepXCCW( long length ) 
{
    digitalWrite(XCCW,HIGH);
    delayMicroseconds(SHORT_X_PULSE); 
    digitalWrite(XCCW,LOW);
    delayMicroseconds(length);    

    IncrementXPulses();

}

char findypluslimit(void) {

	unsigned long counter = 0;
	unsigned int length = DELAY_X1 + DELAY_X2;

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

		counter++;
		if (length <= Y_SPEED ) length = Y_SPEED;
	}  
}

char findyminuslimit(void) {

	unsigned long counter = 0;
	unsigned int length = DELAY_X1 + DELAY_X2;

	while( 1 ) {

		if(  digitalRead( YL2 )) {

			digitalWrite(YCCW,HIGH);
			delay(200);

			homed = 0;
			Serial.print("Found Y- limit = ");
			Serial.println(counter,DEC);
			return false;
		}

		digitalWrite(YCCW,HIGH);
		delayMicroseconds(1); 
		digitalWrite(YCCW,LOW);
		delayMicroseconds(length); 
		counter++;

		if (length <= Y_SPEED ) length = Y_SPEED;
	}  
}

char goleft(long steps ,char nolimit) {

	for( int y = 0 ; y < steps ; y ++ )
	{
		if( nolimit == 1) {
			if( digitalRead( XL1 ) ) {
				digitalWrite(XCW,HIGH);
				Serial.println("goleft limit");
				return false;
			}
		}
		digitalWrite(XCW,HIGH);
		delayMicroseconds(DELAY_X1); 
		digitalWrite(XCW,LOW);
		delayMicroseconds(DELAY_X2); 

		// global pulse counter
		DecrementXPulses();

	}  

	// reset it back
	digitalWrite(XCW,HIGH);

	return true;
}

// pulses = ( microm * 40000 )

long mmtopulses( long mm ) 
{
	return ( mm *  40 ) ;
}

long umtopulses( long microm ) 
{
	return ( microm *  40000 ) ;
}

long pulsestoum( long pulses ) 
{
	return ( pulses /  40000 ) ;
}

long pulsestomm( long pulses ) 
{
	return ( pulses /  40 ) ;
}


char gotoxy(int xum,int yum)
{
      // direction to travel, defaults to right and back
        char xDir=1,yDir;
        long dx,dy;
	long numXPulses=0,numYPulses=0;

        // calculate dx/dy
        dx = ( gCurrentXum - xum );
        dy = ( gCurrentYum - yum );
        
        // if needs to go left
        if( dx < 0  ) {
          dx = abs( dx ) ;
          xDir = 0;
        }
        if( dy < 0  ) {
          dy = abs( dy ) ;
          yDir = 0;
        }
        
	numXPulses = umtopulses( dx );
	numYPulses = umtopulses( dy );

        if( xDir == 0 ) {
          goleft( numXPulses, 1);
        } else {
          goright( numXPulses, 1 ) ;
        }
  
        if( yDir == 0 ) {
          goback( numYPulses, 1);
        } else {
          goforward( numYPulses, 1 ) ;
        }
        
}

// pulses
char gotoxyp(int xp,int yp)
{
	home();
	goright(xp,1);
	goback(yp,1);

}

char goright(long steps ,char nolimit ) {

	for( int x = 0 ; x < steps ; x ++ )
	{
		if( nolimit == 1) {
			if( digitalRead( XL2 ) ) {
				digitalWrite(XCCW,HIGH);
				Serial.println("goright limit");
				//return false;
			}
		}

                stepXCCW( DELAY_X2+DELAY_X1 );

	}  

	// reset it back
	digitalWrite(XCCW,HIGH);

	return true;        
}

char goforward(long steps ,char nolimit ) {

	for( int y = 0 ; y < steps ; y ++ )
	{
		if( nolimit == 1) {
			if( digitalRead( YL1 ) ) {
				digitalWrite(YCW,HIGH);
				Serial.println("goforward limit");
				//return false;
			}
		}
		stepYCW( DELAY_Y2 + DELAY_Y1 );
	}  

	// reset it back
	digitalWrite(YCW,HIGH);
	return true;
}


char goback(long steps ,char nolimit ) {

	for( int y = 0 ; y < steps ; y ++ )
	{
		if( nolimit == 1) {
			if( digitalRead( YL2 ) ) {
				digitalWrite(YCCW,HIGH);
				Serial.println("goback limit");
				return false;
			}
		}
		stepYCCW( DELAY_Y2 + DELAY_Y1 ); 
	}  

	// reset it back
	digitalWrite(YCCW,HIGH);

	return true;        
}


void testmode(void)
{
	digitalWrite(TAPEKNOCK,LOW);  
	delay(5500);
	digitalWrite(TAPEKNOCK,HIGH);
	delay(5500);

	readPanel();

	digitalWrite(ROT,LOW);
	for( int y = 0 ; y < 1000 ; y ++ )
	{
		if( readLimitSwitches() == 1 ) {
			Serial.println("limit");
			return;
		}

                stepXCCW(DELAY_X1+DELAY_X2);


	}  
	digitalWrite(XCW,HIGH);
	delay(1500);
	digitalWrite(HEAD,LOW);
	digitalWrite(CENTERING,LOW);
	for( int y = 0 ; y < 1000 ; y ++ )
	{
		if( readLimitSwitches() == 1 ) {
			Serial.println("limit");
			return;
		}

                stepXCCW(DELAY_X1+DELAY_X2);

	}

	digitalWrite(XCCW,HIGH);

	delay(1500);
	digitalWrite(ROT,HIGH);

	for( int y = 0 ; y < 1000 ; y ++ )
	{
		if( readLimitSwitches() == 1 ) {
			Serial.println("limit");
			return;
		}

                stepYCCW(DELAY_Y1+DELAY_Y2);

	}

	digitalWrite(YCCW,HIGH);
	delay(1500);
	digitalWrite(CENTERING,HIGH);

	for( int y = 0 ; y < 1000 ; y ++ )
	{
		if( readLimitSwitches() == 1 ) {
			Serial.println("limit");
			return;
		}
                stepYCW(DELAY_Y1+DELAY_Y2);
            }

	digitalWrite(YCW,HIGH);
	delay(1500);
	digitalWrite(HEAD,HIGH); 
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
	gYPulses --;
	if( gYPulses < 0 ) {
		Serial.print("Error negative Y pulses, must be loosing some ");
		Serial.print(gXPulses,DEC);
		Serial.println(gYPulses,DEC);
		homed = false;
		return;
	}
}

