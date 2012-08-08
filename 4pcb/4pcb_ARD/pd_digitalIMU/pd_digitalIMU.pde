#include <Wire.h>
#include "minimu9.h"

#define START 255

#define DIR1 2
#define MAG1 3

#define DT 0.01         // [s/loop] loop period
#define AA 0.99         // complementary filter constant

#define V_MIN 100
#define V_MAX 230
#define V_RANGE 130.0

#define A_GAIN 0.0573      // [deg/LSB]
#define G_GAIN 0.00875     // [deg/s/LSB]

#define MAX_ANGLE 20.0     // [deg]
#define MAX_RATE 180.0     // [deg/s]
#define INT_SAT 0.5

#define KP 1.0             // [command/deg]
#define KI 0.0             //
#define KD 0.3             // [command/deg/s]
#define KY 0.3             // [command/deg/s]

void writeGyroReg(byte reg, byte value);
void writeAccReg(byte reg, byte value);
void writeMagReg(byte reg, byte value);
void readGyro(void);
void readAcc(void);
void readMag(void);
void rx();

int gyro_x, gyro_y, gyro_z;
int accel_x, accel_y, accel_z;
int mag_x, mag_y, mag_z;

float angle_pitch = 0.0;  // [deg]
float angle_roll = 0.0;   // [deg]
float rate_pitch = 0.0;   // [deg/s]
float rate_roll = 0.0;    // [deg/s]
float rate_yaw = 0.0;     // [deg/s]

float pitch_error_int = 0.0;
float roll_error_int = 0.0;

signed int a_pitch_z = 0;
signed int a_roll_z = 0;
signed int a_z_z = 0;
signed int g_pitch_z = 0;
signed int g_roll_z = 0;
signed int g_yaw_z = 0;

volatile unsigned char rx_i;
volatile unsigned char rx_packet[5];
volatile unsigned char rx_timeout = 0;

signed int throttle_command;
signed int pitch_command;
signed int roll_command;
signed int yaw_command;

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  
  
  Wire.begin();
  
  Serial.begin(57600);
  UCSR0A |= (1 << U2X0);
  UBRR0L = 33;
  
  writeGyroReg(L3G4200D_CTRL_REG1, 0x4F);
  writeAccReg(LSM303_CTRL_REG1_A, 0x27);
  writeMagReg(LSM303_MR_REG_M, 0x00);
  
  // go really fast 62.5kHz???
  TCCR0A = 0xA3;
  TCCR0B = 0x01; 
  TCCR2A = 0xA3;
  TCCR2B = 0x01;
  
  // what does this do?
  digitalWrite(2, HIGH);
  digitalWrite(8, HIGH);
}

void loop()
{
  signed int a_roll = 0;
  signed int a_pitch = 0;
  signed int a_z = 0;
  signed int g_roll = 0;
  signed int g_pitch = 0;
  signed int g_yaw = 0;
  signed int vref = 0;
  
  float pitch_error = 0.0;
  float roll_error = 0.0;
  
  unsigned char rx_len = 0;
  unsigned char rx_n = 0;
  unsigned char rx_byte = 0;
  
  float front_command_f = 0.0;
  float rear_command_f = 0.0;
  float left_command_f = 0.0;
  float right_command_f = 0.0;
  
  signed int front_command = 0;
  signed int rear_command = 0;
  signed int left_command = 0;
  signed int right_command = 0;
  
  unsigned int rate_pitch_int = 0;
  unsigned int angle_pitch_int = 0;
  
  unsigned char tx_packet[8];

  digitalWrite(13, HIGH);
  
  rx_len = Serial.available();
  if(rx_len > 0)
  {
    for(rx_n = 0; rx_n < (rx_len - 1); rx_n++)
    {
      rx_byte = Serial.read();
      if(rx_byte == START) 
      {
        rx_packet[0] = START;
        rx_i = 1; 
      }
      else if(rx_i <= 4)
      {
        rx_packet[rx_i] = rx_byte;
        rx_i++;
      }
      if(rx_i == 5)
      {
        rx();
        rx_timeout = 0;
        rx_i = 6;
      }
    }
  }
      
  rx_timeout++;
  if(rx_timeout >= 50)
  {
    throttle_command = 0;
  }
  
  readGyro();
  readAcc();  
      
  a_pitch = a_pitch_z - accel_x;
  a_roll = accel_y - a_roll_z;
  // a_z = (signed int) analogRead(A_Z) - a_z_z;
  g_pitch = gyro_y - g_pitch_z;
  g_roll = gyro_x - g_roll_z;
  g_yaw = gyro_z - g_yaw_z;
  
  rate_pitch = (float) g_pitch * G_GAIN;
  rate_roll = (float) g_roll * G_GAIN;
  rate_yaw = (float) g_yaw * G_GAIN;
  
  angle_pitch = AA * (angle_pitch + rate_pitch * DT); 
  angle_pitch += (1.0 - AA) * (float) a_pitch * A_GAIN;
  pitch_error = (float) (pitch_command - 127) / 127.0 * MAX_ANGLE - angle_pitch;
  //pitch_error_int += pitch_error * DT;
  //if(pitch_error_int >= INT_SAT) { pitch_error_int = INT_SAT; }
  //if(pitch_error_int <= -INT_SAT) { pitch_error_int = -INT_SAT; }
  
  angle_roll = AA * (angle_roll + rate_roll * DT); 
  angle_roll += (1.0 - AA) * (float) a_roll * A_GAIN;
  roll_error = (float) (roll_command - 127) / 127.0 * MAX_ANGLE - angle_roll;
  //roll_error_int += roll_error * DT;
  //if(roll_error_int >= INT_SAT) { roll_error_int = INT_SAT; }
  //if(roll_error_int <= -INT_SAT) { roll_error_int = -INT_SAT; }
  
  // Control
  front_command_f -= pitch_error * KP;
  front_command_f -= pitch_error_int * KI;
  front_command_f += rate_pitch * KD;
  front_command_f -= ((float) (yaw_command - 127) / 127.0 * MAX_RATE - rate_yaw) * KY;
  
  rear_command_f += pitch_error * KP;
  rear_command_f += pitch_error_int * KI;
  rear_command_f -= rate_pitch * KD;
  rear_command_f -= ((float) (yaw_command - 127) / 127.0 * MAX_RATE - rate_yaw) * KY;
  
  right_command_f -= roll_error * KP;
  right_command_f -= roll_error_int * KI;
  right_command_f += rate_roll * KD;
  right_command_f += ((float) (yaw_command - 127) / 127.0 * MAX_RATE - rate_yaw) * KY;
  
  left_command_f += roll_error * KP;
  left_command_f += roll_error_int * KI;
  left_command_f -= rate_roll * KD;
  left_command_f += ((float) (yaw_command - 127) / 127.0 * MAX_RATE - rate_yaw) * KY;
  
  if(throttle_command > V_MIN)
  {
    front_command = (signed int) front_command_f + throttle_command;
    rear_command = (signed int) rear_command_f + throttle_command;
    left_command = (signed int) left_command_f + throttle_command;
    right_command = (signed int) right_command_f + throttle_command;

    if(front_command < V_MIN) { front_command = V_MIN; }
    if(front_command > V_MAX) { front_command = V_MAX; }
    if(rear_command < V_MIN) { rear_command = V_MIN; }
    if(rear_command > V_MAX) { rear_command = V_MAX; }
    if(left_command < V_MIN) { left_command = V_MIN; }
    if(left_command > V_MAX) { left_command = V_MAX; }
    if(right_command < V_MIN) { right_command = V_MIN; }
    if(right_command > V_MAX) { right_command = V_MAX; }
  }
  else
  {
    pitch_error_int = 0.0;
    roll_error_int = 0.0;
  }
  
  analogWrite(5, left_command); // LEFT
  analogWrite(3, front_command); // FRONT
  analogWrite(9, right_command); // RIGHT
  analogWrite(10, rear_command); // REAR
  
  rate_pitch_int = (unsigned int)(rate_pitch * 10.0 + 8192.0);
  angle_pitch_int = (unsigned int)(angle_pitch * 10.0 + 8192.0); 
  
  tx_packet[0] = 255;
  tx_packet[1] = (unsigned char) (rate_pitch_int >> 7);
  tx_packet[2] = (unsigned char) (rate_pitch_int & 0x7F);
  tx_packet[3] = (unsigned char) (angle_pitch_int >> 7);
  tx_packet[4] = (unsigned char) (angle_pitch_int & 0x7F); 
  tx_packet[5] = rear_command;
  tx_packet[6] = (unsigned char) (vref >> 7);
  tx_packet[7] = (unsigned char) (vref & 0x7F);
  
  Serial.write(tx_packet, 8);
 
  digitalWrite(13, LOW);
  delay(375);
}

void rx()
{
  throttle_command = rx_packet[1];
  pitch_command = rx_packet[2];
  roll_command = rx_packet[3];
  yaw_command = rx_packet[4];
}

void readGyro()
{
  Wire.beginTransmission(GYR_ADDRESS);
  // assert the MSB of the address to get the gyro 
  // to do slave-transmit subaddress updating.
  Wire.send(L3G4200D_OUT_X_L | (1 << 7)); 
  Wire.endTransmission();
  Wire.requestFrom(GYR_ADDRESS, 6);

  while (Wire.available() < 6);

  uint8_t xla = Wire.receive();
  uint8_t xha = Wire.receive();
  uint8_t yla = Wire.receive();
  uint8_t yha = Wire.receive();
  uint8_t zla = Wire.receive();
  uint8_t zha = Wire.receive();

  gyro_x = xha << 8 | xla;
  gyro_y = yha << 8 | yla;
  gyro_z = zha << 8 | zla;
}

void readAcc(void)
{
	Wire.beginTransmission(ACC_ADDRESS);
	// assert the MSB of the address to get the accelerometer 
	// to do slave-transmit subaddress updating.
	Wire.send(LSM303_OUT_X_L_A | (1 << 7)); 
	Wire.endTransmission();
	Wire.requestFrom(ACC_ADDRESS, 6);

	while (Wire.available() < 6);

	byte xla = Wire.receive();
	byte xha = Wire.receive();
	byte yla = Wire.receive();
	byte yha = Wire.receive();
	byte zla = Wire.receive();
	byte zha = Wire.receive();

	accel_x = (xha << 8 | xla) >> 4;
	accel_y = (yha << 8 | yla) >> 4;
	accel_z = (zha << 8 | zla) >> 4;
}

void readMag(void)
{
	Wire.beginTransmission(MAG_ADDRESS);
	Wire.send(LSM303_OUT_X_H_M);
	Wire.endTransmission();
	Wire.requestFrom(MAG_ADDRESS, 6);

	while (Wire.available() < 6);

	byte xhm = Wire.receive();
	byte xlm = Wire.receive();

	byte yhm, ylm, zhm, zlm;

        zhm = Wire.receive();
	zlm = Wire.receive();
	yhm = Wire.receive();
	ylm = Wire.receive();

	mag_x = (xhm << 8 | xlm);
	mag_y = (yhm << 8 | ylm);
	mag_z = (zhm << 8 | zlm);
}

void writeGyroReg(byte reg, byte value)
{
  Wire.beginTransmission(GYR_ADDRESS);
  Wire.send(reg);
  Wire.send(value);
  Wire.endTransmission();
}

void writeAccReg(byte reg, byte value)
{
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.send(reg);
  Wire.send(value);
  Wire.endTransmission();
}

void writeMagReg(byte reg, byte value)
{
  Wire.beginTransmission(MAG_ADDRESS);
  Wire.send(reg);
  Wire.send(value);
  Wire.endTransmission();
}
