//Seeeduino ADK Demo using Niels Brouwers' MicroBridge library. 
//Connect an LED to D12 and a variable resistor(POT) to A0

#include <Wire.h>
#include <SPI.h>
#include <Adb.h>
#include <Servo.h>
#include <CapSense.h>

// Message for each of the items on the ADK Board
enum ADK_Message {

	// Nothing
	ADK_NULL,
	
	ADK_R1_ON,
	ADK_R1_OFF,
	
	ADK_R2_ON,
	ADK_R2_OFF,

	ADK_LED1R,
	ADK_LED1G,
	ADK_LED1B,
	
	ADK_LED2R,
	ADK_LED2G,
	ADK_LED2B,
	
	ADK_LED3R,
	ADK_LED3G,
	ADK_LED3B,	
};


// Arduino pin outs

#define  LED3_RED       2
#define  LED3_GREEN     4
#define  LED3_BLUE      3

#define  LED2_RED       5
#define  LED2_GREEN     7
#define  LED2_BLUE      6

#define  LED1_RED       8
#define  LED1_GREEN     10
#define  LED1_BLUE      9

#define  SERVO1         11
#define  SERVO2         12
#define  SERVO3         13

#define  TOUCH_RECV     14
#define  TOUCH_SEND     15

#define  RELAY1         A0
#define  RELAY2         A1

#define  LIGHT_SENSOR   A2
#define  TEMP_SENSOR    A3

#define  BUTTON1        A6
#define  BUTTON2        A7
#define  BUTTON3        A8

#define  JOY_SWITCH     A9      // pulls line down when pressed
#define  JOY_nINT       A10     // active low interrupt input
#define  JOY_nRESET     A11     // active low reset output



// Adb connection.
Connection * connection;

// Elapsed time for ADC sampling. The rate at which ADC value is sent to Android device.
long lastTime;

//State of LED. Initially OFF. 
uint8_t LEDState=0;

// State of Relays ( off )
uint8_t R1State=0;
uint8_t R2State=0;

byte b1, b2, b3, b4, c;

// 10M ohm resistor on demo shield
CapSense   touch_robot = CapSense(TOUCH_SEND, TOUCH_RECV);

// Event handler for the shell connection. 
// This event handler is called whenever data is sent from Android Device to Seeeduino ADK.
// Any data / command to be sent to I/O of ADK has to be handled here.
//
// For eg: 1.Controlling an ouput port 2.Interacting with a device connected
// to ADK via IIC or Serial Port.

void adbEventHandler(Connection * connection, adb_eventType event, uint16_t length, uint8_t * data)
{
  
  // In this example Data packets contain one byte and it decides the state of a LED connected to D12
  // The size of data is predetermined for this application. Android device also uses the same size.
  if (event == ADB_CONNECTION_RECEIVE)
  {
    switch (data[ 0] ) {
     
      case ADK_R1_OFF:
        digitalWrite(A0, 0);   // Change the state of Relay 1
        R1State = 0;
        break;
  
      case ADK_R1_ON:
        R1State = 1;
        digitalWrite(A0, 1);   // Change the state of Relay 1
        break;
     
      case ADK_R2_OFF:
        digitalWrite(A1, 0);   // Change the state of Relay 1
        R2State = 0;
        break;
  
      case ADK_R2_ON:
        R2State = 1;
        digitalWrite(A1, 1);   // Change the state of Relay 1
        break;
     }	
     
     
  }
}

void init_buttons()
{
	pinMode(BUTTON1, INPUT);
	pinMode(BUTTON2, INPUT);
	pinMode(BUTTON3, INPUT);
	pinMode(JOY_SWITCH, INPUT);

	// enable the internal pullups
	digitalWrite(BUTTON1, HIGH);
	digitalWrite(BUTTON2, HIGH);
	digitalWrite(BUTTON3, HIGH);
	digitalWrite(JOY_SWITCH, HIGH);
}


void init_relays()
{
	pinMode(RELAY1, OUTPUT);
	pinMode(RELAY2, OUTPUT);
}


void init_leds()
{
	digitalWrite(LED1_RED, 1);
	digitalWrite(LED1_GREEN, 1);
	digitalWrite(LED1_BLUE, 1);

	pinMode(LED1_RED, OUTPUT);
	pinMode(LED1_GREEN, OUTPUT);
	pinMode(LED1_BLUE, OUTPUT);

	digitalWrite(LED2_RED, 1);
	digitalWrite(LED2_GREEN, 1);
	digitalWrite(LED2_BLUE, 1);

	pinMode(LED2_RED, OUTPUT);
	pinMode(LED2_GREEN, OUTPUT);
	pinMode(LED2_BLUE, OUTPUT);

	digitalWrite(LED3_RED, 1);
	digitalWrite(LED3_GREEN, 1);
	digitalWrite(LED3_BLUE, 1);

	pinMode(LED3_RED, OUTPUT);
	pinMode(LED3_GREEN, OUTPUT);
	pinMode(LED3_BLUE, OUTPUT);
}
Servo servos[3];

// ==============================================================================

// Austria Microsystems i2c Joystick
void init_joystick(int threshold)
{
	byte status = 0;

	pinMode(JOY_SWITCH, INPUT);
	digitalWrite(JOY_SWITCH, HIGH);

	pinMode(JOY_nINT, INPUT);
	digitalWrite(JOY_nINT, HIGH);

	pinMode(JOY_nRESET, OUTPUT);

	digitalWrite(JOY_nRESET, 1);
	delay(1);
	digitalWrite(JOY_nRESET, 0);
	delay(1);
	digitalWrite(JOY_nRESET, 1);

	Wire.begin();

	do {
		status = read_joy_reg(0x0f);
	} while ((status & 0xf0) != 0xf0);

	// invert magnet polarity setting, per datasheet
	write_joy_reg(0x2e, 0x86);

	calibrate_joystick(threshold);
}


int offset_X, offset_Y;

void calibrate_joystick(int dz)
{
	char iii;
	int x_cal = 0;
	int y_cal = 0;

	// Low Power Mode, 20ms auto wakeup
	// INTn output enabled
	// INTn active after each measurement
	// Normal (non-Reset) mode
	write_joy_reg(0x0f, 0x00);
	delay(1);

	// dummy read of Y_reg to reset interrupt
	read_joy_reg(0x11);

	for(iii = 0; iii != 16; iii++) {
		while(!joystick_interrupt()) {}

		x_cal += read_joy_reg(0x10);
		y_cal += read_joy_reg(0x11);
	}

	// divide by 16 to get average
	offset_X = -(x_cal>>4);
	offset_Y = -(y_cal>>4);

	write_joy_reg(0x12, dz - offset_X);  // Xp, LEFT threshold for INTn
	write_joy_reg(0x13, -dz - offset_X);  // Xn, RIGHT threshold for INTn
	write_joy_reg(0x14, dz - offset_Y);  // Yp, UP threshold for INTn
	write_joy_reg(0x15, -dz - offset_Y);  // Yn, DOWN threshold for INTn

	// dead zone threshold detect requested?
	if (dz)
		write_joy_reg(0x0f, 0x04);
}


void read_joystick(int *x, int *y)
{
	*x = read_joy_reg(0x10) + offset_X;
	*y = read_joy_reg(0x11) + offset_Y;  // reading Y clears the interrupt
}

char joystick_interrupt()
{
	return digitalRead(JOY_nINT) == 0;
}


#define  JOY_I2C_ADDR    0x40

char read_joy_reg(char reg_addr)
{
	char c;

	Wire.beginTransmission(JOY_I2C_ADDR);
	Wire.send(reg_addr);
	Wire.endTransmission();

	Wire.requestFrom(JOY_I2C_ADDR, 1);

	while(Wire.available())
		c = Wire.receive();

	return c;
}

void write_joy_reg(char reg_addr, char val)
{
	Wire.beginTransmission(JOY_I2C_ADDR);
	Wire.send(reg_addr);
	Wire.send(val);
	Wire.endTransmission();
}
// a2 = lightsensor
// 5, middle led demo board
// a0, a1

void setup()
{
  //Serial port debug purpose
  Serial.begin(57600);
  
  // Note start time
  lastTime = millis();

  init_leds();
  init_relays();
  init_buttons();
  init_joystick( 5 );
  
  // autocalibrate OFF
  touch_robot.set_CS_AutocaL_Millis(0xFFFFFFFF);


  analogWrite(LED1_RED,0);
  analogWrite(LED1_GREEN, 255);
  analogWrite(LED1_BLUE,255);
  
  analogWrite(LED2_RED,255);
  analogWrite(LED2_GREEN, 0);
  analogWrite(LED2_BLUE,255);
  
  analogWrite(LED3_RED,255);
  analogWrite(LED3_GREEN, 255);
  analogWrite(LED3_BLUE,0);
  
  servos[0].attach(SERVO1);
  servos[0].write(90);
  servos[1].attach(SERVO2);
  servos[1].write(90);
  servos[2].attach(SERVO3);
  servos[2].write(90);
  
  b1 = digitalRead(BUTTON1);
  b2 = digitalRead(BUTTON2);
  b3 = digitalRead(BUTTON3);
  b4 = digitalRead(JOY_SWITCH);


  // Initialise the ADB subsystem.  
  ADB::init();

  // Open an ADB stream to the phone's shell. Auto-reconnect. Use any unused port number eg:4568
  connection = ADB::addConnection("tcp:4568", true, adbEventHandler);  


}

void loop()
{

  //Check if ADC needs to be sampled.
  if ((millis() - lastTime) > 20)
  {
    //Read ADC value
    uint16_t data = analogRead(A2);
    uint16_t temperature = analogRead(A3);

    uint32_t  packet;
    
   // send both together
    packet = ((uint32_t)data << 16) + temperature; 
    
    //Send the ADC value to Android device as four bytes of data.
    connection->write(sizeof(packet),(uint8_t*)&packet);
    lastTime = millis();

#if 1

// capSense
   byte tc = touch_robot.capSense(100);
   analogWrite(LED3_BLUE,tc);
    
   int x,y;
    
   read_joystick(&x, &y);
    
   servos[0].write(map(constrain(x, -128, 127), 0, 255, 0, 180));   
   servos[1].write(map(constrain(y, -128, 127), 0, 255, 0, 180));   
  }
  
  #endif

  b1 = digitalRead(BUTTON1)?255:0;
  b2 = digitalRead(BUTTON2)?255:0;
  b3 = digitalRead(BUTTON3)?255:0;

  analogWrite(LED2_RED, b1);
  analogWrite(LED2_GREEN, b2);
  analogWrite(LED2_BLUE, b3);

  // Poll the ADB subsystem.
  ADB::poll();
  
 
}

