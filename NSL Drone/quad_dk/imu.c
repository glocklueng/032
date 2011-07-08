#include "imu.h"
  
//  Natural Constants
const float pi = 3.141592f;		    // Pi
const float convert_pi_180 = 0.017453f;	    // Pi/180 - Convert Degrees to Radians
const float convert_180_Pi = 57.29577f;	    // 180/Pi - Convert Radians to Degrees


//  IMU Variables and Constants
unsigned long x_acc_raw = 0;		// X Accelerometer Reading
unsigned long y_acc_raw = 0;		// Y Accelerometer Reading
unsigned long z_acc_raw = 0;		// Z Accelerometer Reading

const int x_acc_offset = 0;		// X Accelerometer Offset
const int y_acc_offset = 0;		// Y Accelerometer Offset
const int z_acc_offset = 0;		// Z Accelerometer Offset

const float x_acc_scale = 0.0;		// X Accelerometer Scale
const float y_acc_scale = 0.0;		// Y Accelerometer Scale
const float z_acc_scale = 0.0;		// Z Accelerometer Scale

float x_acc_rad = 0.0;			// X Axis Angle - Raw angle in radians
float y_acc_rad = 0.0;			// Y Axis Angle - Raw angle in radians
float z_acc_rad = 0.0;			// Z Axis Angle - Raw angle in radians

float x_angle = 0.0;			// X Axis Angle - State Estimation	
float y_angle = 0.0;			// Y Axis Angle - State Estimation	
float z_angle = 0.0;			// Z Axis Angle - State Estimation	

float x_bias = 0.0;			// X Bias for State Matrix
float y_bias = 0.0;			// Y Bias for State Matrix
float z_bias = 0.0;			// Z Bias for State Matrix

int x_gyro_raw = 0;			// X Gyro Reading
int y_gyro_raw = 0;			// Y Gyro Reading
int z_gyro_raw = 0;		        // Z Gyro Reading

const int x_gyro_offset = 0;		// X Gyro Offset
const int y_gyro_offset = 0;		// Y Gyro Offset
const int z_gyro_offset = 0;		// Z Gyro Offset

const float x_gyro_scale = 0.0;		// X Gyro Scale
const float y_gyro_scale = 0.0;		// Y Gyro Scale
const float z_gyro_scale = 0.0;		// Z Gyro Scale

float x_gyro_rad_sec = 0.0 ;		// X Gyro - Radians/Second - State Estimation	
float x_gyro_rad_sec_1 = 0.0;		// X Gyro - Runge-Kutta Integration - Itteration 1
float x_gyro_rad_sec_2 = 0.0;		// X Gyro - Runge-Kutta Integration - Itteration 2
float x_gyro_rad_sec_3 = 0.0;		// X Gyro - Runge-Kutta Integration - Itteration 3
float x_gyro_bias = 0.0;		// X Gyro - Bias for Integration

float y_gyro_rad_sec = 0.0 ;		// Y Gyro - Radians/Second - State Estimation	
float y_gyro_rad_sec_1 = 0.0;		// Y Gyro - Runge-Kutta Integration - Itteration 1
float y_gyro_rad_sec_2 = 0.0;		// Y Gyro - Runge-Kutta Integration - Itteration 2
float y_gyro_rad_sec_3 = 0.0;		// Y Gyro - Runge-Kutta Integration - Itteration 3
float y_gyro_bias = 0.0;		// Y Gyro - Bias for Integration

float z_gyro_rad_sec = 0.0 ;		// Z Gyro - Radians/Second - State Estimation	
float z_gyro_rad_sec_1 = 0.0;		// Z Gyro - Runge-Kutta Integration - Itteration 1
float z_gyro_rad_sec_2 = 0.0;		// Z Gyro - Runge-Kutta Integration - Itteration 2
float z_gyro_rad_sec_3 = 0.0;		// Z Gyro - Runge-Kutta Integration - Itteration 3
float z_gyro_bias = 0.0;		// Z Gyro - Bias for Integration

float x_cent_force = 0.0;		// Centreptial Force - X Axis
float y_cent_force = 0.0;		// Centreptial Force - Y Axis
float z_cent_force = 0.0;		// Centreptial Force - Z Axis


//  Kalman Variables and Constants
const float dt = 0.02f;			// dt = 0.02 seconds per sample
const float Q_angle =  0.001f;		// 0.001 - Q constant for angle
const float Q_gyro = 0.003f;		// 0.003 - Q constant for gyro
const float R_angle = 0.7f;		// 0.7   - R constant for noise    

float x_y;				// Difference between previous raw angle reading and previous state angle - X-Axis
float xS;				// S Variable - X Axis
float xK_0;				// K Matrix - X Axis
float xK_1;				// K Matrix - X Axis
float xP_00,xP_01,xP_10,xP_11;          // P Matrix - X Axis

float y_y;				// Difference between previous raw angle reading and previous state angle - Y-Axis
float yS;				// S Variable - Y Axis
float yK_0;				// K Matrix - Y Axis
float yK_1;				// K Matrix - Y Axis
float yP_00,yP_01,yP_10,yP_11;		// P Matrix - Y Axis

float z_y;				// Difference between previous raw angle reading and previous state angle - Z-Axis
float zS;				// S Variable - Z Axis
float zK_0;				// K Matrix - Z Axis
float zK_1;				// K Matrix - Z Axis
float zP_00,zP_01,zP_10,zP_11;		// P Matrix - Z Axis



// THIS SHOULD NOT BE VOID, CHANGE IT LATER
void readIMU()
{
    //ADCSequenceDataGet(,,x_acc_raw,4);	// Read ADC - X Axis Acc
    //ADCSequenceDataGet(,,y_acc_raw,4);	// Read ADC - Y Axis Acc
    //ADCSequenceDataGet(,,z_acc_raw,4);	// Read ADC - Z Axis Acc
    
    //x_acc_raw = 0x08;                 // Read ADC - X Axis Acc
    //y_acc_raw = 0x08;			// Read ADC - Y Axis Acc
    //z_acc_raw = 0x08;			// Read ADC - Z Axis Acc
    
    x_gyro_raw = 0x08;                  // Read ADC - X Axis Gyro
    y_gyro_raw = 0x08;                  // Read ADC - Y Axis Gyro
    z_gyro_raw = 0x08;	                // Read ADC - Z Axis Gyro
    
    x_acc_rad  = ((float)(x_acc_raw - x_acc_offset)* x_acc_scale);		 // Convert raw data bytes to angle - X Axis
    y_acc_rad  = ((float)(y_acc_raw - y_acc_offset)* y_acc_scale);		 // Convert raw data bytes to angle - Y Axis
    z_acc_rad  = ((float)(z_acc_raw - z_acc_offset)* z_acc_scale);	         // Convert raw data bytes to angle - Z Axis
    
    x_gyro_rad_sec  = ((float)(x_gyro_raw - x_gyro_offset)* x_gyro_scale);	 // Convert raw data bytes to angular velocity - X Axis
    y_gyro_rad_sec  = ((float)(y_gyro_raw - y_gyro_offset)* y_gyro_scale);	 // Convert raw data bytes to angular velocity - Y Axis
    z_gyro_rad_sec  = ((float)(z_gyro_raw - z_gyro_offset)* z_gyro_scale);	 // Convert raw data bytes to angular velocity - Z Axis
    
    
    // Runge-Kutta Integration 
    // -----------------------
    // The Runge-Kutta Integration will average 
    // out the gyroscope readings.
    
    // X-Axis Gyro - Runge-Kutta Integration 
    x_gyro_rad_sec = (x_gyro_rad_sec_3 + (2 * x_gyro_rad_sec_2) + (2 * x_gyro_rad_sec_1) + x_gyro_rad_sec)/6.0;
    x_gyro_rad_sec_1 = x_gyro_rad_sec;
    x_gyro_rad_sec_2 = x_gyro_rad_sec_1;
    x_gyro_rad_sec_3 = x_gyro_rad_sec_2;
    
    // Y-Axis Gyro - Runge-Kutta Integration 
    y_gyro_rad_sec = (y_gyro_rad_sec_3 + (2 * y_gyro_rad_sec_2) + (2 * y_gyro_rad_sec_1) + y_gyro_rad_sec)/6.0;
    y_gyro_rad_sec_1 = y_gyro_rad_sec;
    y_gyro_rad_sec_2 = y_gyro_rad_sec_1;
    y_gyro_rad_sec_3 = y_gyro_rad_sec_2;
    
    // Z-Axis Gyro - Runge-Kutta Integration 
    z_gyro_rad_sec = (z_gyro_rad_sec_3 + (2 * z_gyro_rad_sec_2) + (2 * z_gyro_rad_sec_1) + z_gyro_rad_sec)/6.0;
    z_gyro_rad_sec_1 = z_gyro_rad_sec;
    z_gyro_rad_sec_2 = z_gyro_rad_sec_1;
    z_gyro_rad_sec_3 = z_gyro_rad_sec_2;
    
    // Integrate the gyro axis with a bias to convert to angle then add to angle
    // To Do: Add weighting to this process
    x_angle += ((x_gyro_rad_sec - x_bias)) * dt;			// integrate x axis gyro with bias then add to x angle
    y_angle += ((y_gyro_rad_sec - y_bias)) * dt;			// integrate y axis gyro with bias then add to y angle
    z_angle += ((z_gyro_rad_sec - z_bias)) * dt;			// integrate z axis gyro with bias then add to z angle
    
    
    // Kalman Filter 
    // ----------------------
    
    // Filter X Axis
    // ----------------------
    // Predict 
    xP_00 -=  dt * (xP_10 + xP_01) + Q_angle * dt;
    xP_01 -=  dt * xP_11;
    xP_10 -=  dt * xP_11;
    xP_11 +=  Q_gyro * dt; 
    
    // Update with new data
    x_y = x_acc_rad - x_angle;    
    xS = xP_00 + R_angle;
    xK_0 = xP_00 / xS;
    xK_1 = xP_10 / xS;
    
    // Update State
    x_angle +=  xK_0 * x_y;
    x_bias  +=  xK_1 * x_y;
    
    // Update P Matrix
    xP_00 -= xK_0 * xP_00;
    xP_01 -= xK_0 * xP_01;
    xP_10 -= xK_1 * xP_00;
    xP_11 -= xK_1 * xP_01;
    
    
    // Filter Y Axis
    // ----------------------
    // Predict 
    yP_00 -=  dt * (yP_10 + yP_01) + Q_angle * dt;
    yP_01 -=  dt * yP_11;
    yP_10 -=  dt * yP_11;
    yP_11 +=  Q_gyro * dt; 
    
    // Update with new data
    y_y = y_acc_rad - y_angle;
    yS = yP_00 + R_angle;
    yK_0 = yP_00 / yS;
    yK_1 = yP_10 / yS;
    
    // Update State
    y_angle +=  yK_0 * y_y;
    y_bias  +=  yK_1 * y_y;
    
    // Update P Matrix
    yP_00 -= yK_0 * yP_00;
    yP_01 -= yK_0 * yP_01;
    yP_10 -= yK_1 * yP_00; 
    yP_11 -= yK_1 * yP_01;
    
    
    // Filter Z Axis
    // ----------------------
    // Predict 
    zP_00 -=  dt * (zP_10 + zP_01) + Q_angle * dt;
    zP_01 -=  dt * zP_11;
    zP_10 -=  dt * zP_11;
    zP_11 +=  Q_gyro * dt; 
    
    // Update with new data
    z_y = z_acc_rad - z_angle;
    zS = zP_00 + R_angle;
    zK_0 = zP_00 / zS;
    zK_1 = zP_10 / zS;
    
    // Update State
    z_angle +=  zK_0 * z_y;
    z_bias  +=  zK_1 * z_y;
    
    // Update P Matrix
    zP_00 -= zK_0 * zP_00;
    zP_01 -= zK_0 * zP_01;
    zP_10 -= zK_1 * zP_00; 
    zP_11 -= zK_1 * zP_01;
    
    
}