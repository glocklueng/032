#include "inc/lm3s9b96.h"
#include "math.h"


#include "imu.h"
#include "xbee.h"

#include "inc/hw_adc.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"

//  Natural Constants
const float pi = 3.141592f;		    // Pi
const float convert_pi_180 = 0.017453f;	    // Pi/180 - Convert Degrees to Radians
const float convert_180_Pi = 57.29577f;	    // 180/Pi - Convert Radians to Degrees


//  IMU Variables and Constants
unsigned long x_acc_raw = 0;		// X Accelerometer Reading
unsigned long y_acc_raw = 0;		// Y Accelerometer Reading
unsigned long z_acc_raw = 0;		// Z Accelerometer Reading

int x_acc_offset = 0;		        // X Accelerometer Offset
int y_acc_offset = 0;		        // Y Accelerometer Offset
int z_acc_offset = 0;		        // Z Accelerometer Offset

float x_angle_offset = -15.0f;          // X Angle Offset
float y_angle_offset = -15.0f;          // Y Angle Offset
float z_angle_offset = -15.0f;          // Z Angle Offset

float x_angle_scale = 1.5f;             // X Angle Scale
float y_angle_scale = 1.5f;             // Y Angle Scale
float z_angle_scale = 1.5f;             // Z Angle Scale

const float x_acc_shift = 1.65f;	// X Accelerometer Voltage Shift
const float y_acc_shift = 1.65f;	// Y Accelerometer Voltage Shift
const float z_acc_shift = 1.65f;	// Z Accelerometer Voltage Shift

const float x_acc_v_convert = 0.003222f; // X Accelerometer Convert Raw to Volts
const float y_acc_v_convert = 0.003222f; // Y Accelerometer Convert Raw to Volts
const float z_acc_v_convert = 0.003222f; // Z Accelerometer Convert Raw to Volts

const float x_acc_g_convert = 3.030303f; // X Accelerometer Convert Volts to G's
const float y_acc_g_convert = 3.030303f; // Y Accelerometer Convert Volts to G's
const float z_acc_g_convert = 3.030303f; // Z Accelerometer Convert Volts to G's

float x_accel = 0.0f;			// X Axis Acceleration - in G's
float y_accel = 0.0f;			// Y Axis Acceleration - in G's
float z_accel = 0.0f;			// Z Axis Acceleration - in G's

float x_acc_rad = 0.0f;			// X Axis Angle - Raw angle in radians
float y_acc_rad = 0.0f;			// Y Axis Angle - Raw angle in radians
float z_acc_rad = 0.0f;			// Z Axis Angle - Raw angle in radians

float x_angle = 0.0f;			// X Axis Angle - State Estimation	
float y_angle = 0.0f;			// Y Axis Angle - State Estimation	
float z_angle = 0.0f;			// Z Axis Angle - State Estimation	

float x_bias = 0.0785385f;		// X Bias for State Matrix - +4.5 degrees
float y_bias = -0.0261795f;		// Y Bias for State Matrix - -1.5 degrees
float z_bias = -0.0261795f;		// Z Bias for State Matrix - -1.5 degrees  

signed long x_gyro_raw = 0;		// X Gyro Reading
signed long y_gyro_raw = 0;		// Y Gyro Reading
signed long z_gyro_raw = 0;		// Z Gyro Reading

int x_gyro_offset = -200;		// X Gyro Offset
int y_gyro_offset = 49;		        // Y Gyro Offset
int z_gyro_offset = 9;		        // Z Gyro Offset     

float x_gyro_scale = 0.007629f;	        // X Gyro Scale
float y_gyro_scale = 0.007629f;	        // Y Gyro Scale
float z_gyro_scale = 0.007629f;	        // Z Gyro Scale

float x_gyro_rad_sec = 0.0f;		// X Gyro - Radians/Second - State Estimation	
float x_gyro_rad_sec_1 = 0.0f;		// X Gyro - Runge-Kutta Integration - Itteration 1
float x_gyro_rad_sec_2 = 0.0f;		// X Gyro - Runge-Kutta Integration - Itteration 2
float x_gyro_rad_sec_3 = 0.0f;		// X Gyro - Runge-Kutta Integration - Itteration 3
float x_gyro_bias = 0.0f;		// X Gyro - Bias for Integration

float y_gyro_rad_sec = 0.0f;		// Y Gyro - Radians/Second - State Estimation	
float y_gyro_rad_sec_1 = 0.0f;		// Y Gyro - Runge-Kutta Integration - Itteration 1
float y_gyro_rad_sec_2 = 0.0f;		// Y Gyro - Runge-Kutta Integration - Itteration 2
float y_gyro_rad_sec_3 = 0.0f;		// Y Gyro - Runge-Kutta Integration - Itteration 3
float y_gyro_bias = 0.0f;		// Y Gyro - Bias for Integration

float z_gyro_rad_sec = 0.0f;		// Z Gyro - Radians/Second - State Estimation	
float z_gyro_rad_sec_1 = 0.0f;		// Z Gyro - Runge-Kutta Integration - Itteration 1
float z_gyro_rad_sec_2 = 0.0f;		// Z Gyro - Runge-Kutta Integration - Itteration 2
float z_gyro_rad_sec_3 = 0.0f;		// Z Gyro - Runge-Kutta Integration - Itteration 3
float z_gyro_bias = 0.0f;		// Z Gyro - Bias for Integration

float x_cent_force = 0.0f;		// Centreptial Force - X Axis
float y_cent_force = 0.0f;		// Centreptial Force - Y Axis
float z_cent_force = 0.0f;		// Centreptial Force - Z Axis


//  Kalman Variables and Constants
unsigned int previousTimer;             // The previous time from the timer
unsigned int currentTimer;              // The current time from the timer
unsigned long startFlag = 0;            // Flag for when dt clock starts
float measuredDt;                       // Measured dt
float dt = 0.0005f;		        // dt = 0.002 seconds per sample

const float Q_angle = 0.001f;		// 0.001 - Q constant for angle
const float Q_gyro = 0.012f;		// 0.012 - Q constant for gyro
const float R_angle = 0.1f;		// 0.1   - R constant for noise    

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

// Compass Variables
signed long x_comp_raw = 0;		// X Compass Reading
signed long y_comp_raw = 0;		// Y Compass Reading
signed long z_comp_raw = 0;		// Z Compass Reading

// Temperature Variables
unsigned long temp_raw;
float temperature;


// Test Values
float x_ang = 0;

// -----------------------------------------  
//
// IMU - Filtered results
//
// [0] : X Angle
// [1] : Y Angle
// [2] : Z Angle
// [3] : X Rate
// [4] : Y Rate
// [5] : Z Rate
// [6] : Magnetic North Heading
// [7] : Temperature
void readIMU(float *imu)
{     
    // Calculate dt for integration
    // ----------------------------
    if(startFlag == 0)
    {
        startFlag = 1;  // dt calculation starts
        
        previousTimer = TimerValueGet(TIMER0_BASE, TIMER_A);
    }
    else
    {
        currentTimer = TimerValueGet(TIMER0_BASE, TIMER_A);
        
        measuredDt = (float)SysCtlClockGet();
        measuredDt = (float)((float)(previousTimer - currentTimer)/(float)SysCtlClockGet());  
        
        if(measuredDt > 0.0000000f)
        {
          dt = measuredDt;
        }
        
        previousTimer = currentTimer;
    }
    // ----------------------------
    
    // Read ADC - X Axis Acc
    // Read ADC - Y Axis Acc
    // Read ADC - Z Axis Acc
    readAccel(&temp_raw, &x_acc_raw, &y_acc_raw, &z_acc_raw);
    
    // Read I2C - X Axis Gyro
    // Read I2C - Y Axis Gyro
    // Read I2C - Z Axis Gyro
    readGyro(&x_gyro_raw, &y_gyro_raw, &z_gyro_raw);
    
    // Read Raw - X Axis Compass
    // Read Raw - Y Axis Compass
    // Read Raw - Z Axis Compass
    readCompass(&x_comp_raw, &y_comp_raw, &z_comp_raw);
    
    // Convert raw adc values to acceleration
    x_accel = ((x_acc_v_convert*(float)((int)(x_acc_raw) - x_acc_offset))-x_acc_shift)*x_acc_g_convert; // Convert raw data bytes to acceleration - X Axis
    y_accel = ((y_acc_v_convert*(float)((int)(y_acc_raw) - y_acc_offset))-y_acc_shift)*y_acc_g_convert; // Convert raw data bytes to acceleration - Y Axis
    z_accel = ((z_acc_v_convert*(float)((int)(z_acc_raw) - z_acc_offset))-z_acc_shift)*z_acc_g_convert; // Convert raw data bytes to acceleration - Z Axis
    
    // Convert acceleration to angles    
    x_acc_rad = (float)((x_angle_scale*((((float)atan(y_accel/sqrt(x_accel*x_accel + z_accel*z_accel)))*convert_180_Pi) + x_angle_offset))*convert_pi_180);
    y_acc_rad = (float)((y_angle_scale*((((float)atan(x_accel/sqrt(y_accel*y_accel + z_accel*z_accel)))*convert_180_Pi) + y_angle_offset))*convert_pi_180);
    z_acc_rad += z_gyro_rad_sec*dt;  // Integrate Z Angular Velocity to obtain yaw angle 
    
    x_gyro_rad_sec  = (float)(((float)(x_gyro_raw - x_gyro_offset)* x_gyro_scale)*convert_pi_180);	 // Convert raw data bytes to angular velocity - X Axis
    y_gyro_rad_sec  = (float)(((float)(y_gyro_raw - y_gyro_offset)* y_gyro_scale)*convert_pi_180);	 // Convert raw data bytes to angular velocity - Y Axis
    z_gyro_rad_sec  = (float)(((float)(z_gyro_raw - z_gyro_offset)* z_gyro_scale)*convert_pi_180);	 // Convert raw data bytes to angular velocity - Z Axis
    
    
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

    //  Pre-Kalman Values
    /*
    imu[0] = x_acc_rad;
    imu[1] = y_acc_rad;
    imu[2] = z_acc_rad;
    imu[3] = (x_gyro_rad_sec - x_bias);
    imu[4] = (y_gyro_rad_sec - y_bias);
    imu[5] = (z_gyro_rad_sec - z_bias);
    imu[6] = x_acc_rad*convert_180_Pi;  
    imu[7] = temp_raw; 
    */
   
    /*
    imu[0] = x_acc_rad;
    imu[1] = y_acc_rad;
    imu[2] = z_acc_rad;
    imu[3] = (x_gyro_rad_sec - x_bias);
    imu[4] = (y_gyro_rad_sec - y_bias);
    imu[5] = (z_gyro_rad_sec - z_bias);
    imu[6] = 12345;
    imu[7] = temp_raw;
    */
    

    
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
    
    // Kalman Filtered Values
    imu[0] = x_angle*convert_180_Pi;
    imu[1] = y_angle*convert_180_Pi;
    imu[2] = z_angle*convert_180_Pi;
    imu[3] = (x_gyro_rad_sec - x_bias)*convert_180_Pi;
    imu[4] = (y_gyro_rad_sec - y_bias)*convert_180_Pi;
    imu[5] = (z_gyro_rad_sec - z_bias)*convert_180_Pi;
    imu[6] = 12345;
    imu[7] = temp_raw; 
    //
    
    // Send Data Telemetry
    sendDataTelemetry(&imu[0], dt);
    
}
// ----------------------------------------- 


// -----------------------------------------   
//
// Read Accelerometer
//
// ADC Value Matrix:
// [0] : Temperature
// [1] : X - Axis Accelerometer
// [2] : Y - Axis Accelerometer
// [3] : Z - Axis Accelerometer
unsigned long adc_values[4];
// -----------------------------------------   
void readAccel(unsigned long *temp, unsigned long *x_acc,
               unsigned long *y_acc, unsigned long *z_acc)
{
     // Trigger the sample sequence.
    ADCProcessorTrigger(ADC_BASE, 0);

    // Wait until the sample sequence has completed.
    while(!ADCIntStatus(ADC_BASE, 0, false)){}
    
    // Grab ADC values and load to buffer
    ADCSequenceDataGet(ADC_BASE,0,adc_values);
    
    // Clear ADC sequencer
    ADCIntClear(ADC_BASE, 0);
    
    *temp = adc_values[0];
    *x_acc = adc_values[1];
    *y_acc = adc_values[2];
    *z_acc = adc_values[3];
}
// ----------------------------------------- 


// -----------------------------------------   
//
// Read Gyroscope
//
// [0] : X - Axis Gyroscope L
// [1] : X - Axis Gyroscope H
// [2] : Y - Axis Gyroscope L
// [3] : Y - Axis Gyroscope H
// [4] : Z - Axis Gyroscope L
// [5] : Z - Axis Gyroscope H
signed char gyro_values[6];
// -----------------------------------------   
void readGyro(signed long *x_gyro, signed long *y_gyro,
              signed long *z_gyro)
{                                                                              
    I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, GYRO_ADDRESS, I2C_SEND);        // ST - SAD+W  
    I2CMasterDataPut(I2C1_MASTER_BASE, L3G4_OUT_X_L | 0x80);                // SUB - send contin
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);         // Set to send
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                // Wait for SAK
    
    I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, GYRO_ADDRESS, I2C_RECV);        // SR - SAD+R
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START); // Set to recieve
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                // Wait for SAK
    
    gyro_values[0] = (signed char)I2CMasterDataGet(I2C1_MASTER_BASE);     // Get X - Axis Gyroscope L
    
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);  // Set to recieve
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                // Wait for SAK
    
    gyro_values[1] = (signed char)I2CMasterDataGet(I2C1_MASTER_BASE);     // Get X - Axis Gyroscope H
    
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);  // Set to recieve
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                // Wait for SAK
    
    gyro_values[2] = (signed char)I2CMasterDataGet(I2C1_MASTER_BASE);     // Get Y - Axis Gyroscope L
    
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);  // Set to recieve
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                // Wait for SAK
    
    gyro_values[3] = (signed char)I2CMasterDataGet(I2C1_MASTER_BASE);     // Get Y - Axis Gyroscope H
    
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);  // Set to recieve
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                // Wait for SAK
    
    gyro_values[4] = (signed char)I2CMasterDataGet(I2C1_MASTER_BASE);     // Get Z - Axis Gyroscope L
    
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);// Set to recieve
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                // Wait for SAK
    
    gyro_values[5] = (signed char)I2CMasterDataGet(I2C1_MASTER_BASE);     // Get Z - Axis Gyroscope H
    
    *x_gyro = (gyro_values[1] << 8) | gyro_values[0];
    *y_gyro = (gyro_values[3] << 8) | gyro_values[2];
    *z_gyro = (gyro_values[5] << 8) | gyro_values[4];
    
}
// ----------------------------------------- 


// -----------------------------------------   
//
// Read Compass
//
// [0] : X - Axis MSB Compass
// [1] : X - Axis LSB Compass
// [2] : Y - Axis MSB Compass
// [3] : Y - Axis LSB Compass
// [4] : Z - Axis MSB Compass
// [5] : Z - Axis LSB Compass
signed char compass_values[6];
// ----------------------------------------- 
void readCompass(signed long *x_axis, signed long *y_axis,
                 signed long *z_axis)
{       
    I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, COMP_ADDRESS, I2C_SEND);        // Set I2C to send (WRITE)
    I2CMasterDataPut(I2C0_MASTER_BASE, COMP_DATA_X_MSB);                    // Setup to start reading at X MSB  
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);         // Start Sending      

    while(I2CMasterBusy(I2C0_MASTER_BASE)){}                                // Wait for SAK
    
    I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, COMP_ADDRESS, I2C_RECV);        // Set I2C to recieve  (READ)
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START); // Set to recieve multiple
    
    while(I2CMasterBusy(I2C0_MASTER_BASE)){}                                // Wait for SAK
    
    compass_values[0] = (signed char)I2CMasterDataGet(I2C0_MASTER_BASE);  // Get Data X MSB
    
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);  // Set to recieve
    
    while(I2CMasterBusy(I2C0_MASTER_BASE)){}                                // Wait for SAK
    
    compass_values[1] = (signed char)I2CMasterDataGet(I2C0_MASTER_BASE);  // Get Data X LSB
    
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);  // Set to recieve
    
    while(I2CMasterBusy(I2C0_MASTER_BASE)){}                                // Wait for SAK
    
    compass_values[2] = (signed char)I2CMasterDataGet(I2C0_MASTER_BASE);  // Get Data Y MSB
    
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);  // Set to recieve
    
    while(I2CMasterBusy(I2C0_MASTER_BASE)){}                                // Wait for SAK
    
    compass_values[3] = (signed char)I2CMasterDataGet(I2C0_MASTER_BASE);  // Get Data Y LSB
    
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);  // Set to recieve
    
    while(I2CMasterBusy(I2C0_MASTER_BASE)){}                                // Wait for SAK
    
    compass_values[4] = (signed char)I2CMasterDataGet(I2C0_MASTER_BASE);  // Get Data Z MSB
    
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);  // Set to recieve
    
    while(I2CMasterBusy(I2C0_MASTER_BASE)){}                                // Wait for SAK
    
    compass_values[5] = (signed char)I2CMasterDataGet(I2C0_MASTER_BASE);  // Get Data Z LSB
    
    *x_axis = (compass_values[0] << 8) | compass_values[1];
    *y_axis = (compass_values[2] << 8) | compass_values[3];
    *z_axis = (compass_values[4] << 8) | compass_values[5];
}
// ----------------------------------------- 


// -----------------------------------------   
// IMU STARTUP
// -----------------------------------------  
// Start up the IMU by calibrating
void imuStartup()
{
   sensorsSelfTest();
}
// ----------------------------------------- 


// -----------------------------------------   
// SELF TEST SYSTEM
// -----------------------------------------   
// [0] : X - Axis Gyroscope Positive (-) Calibration Reading
// [1] : Y - Axis Gyroscope Positive (+) Calibration Reading
// [2] : Z - Axis Gyroscope Positive (-) Calibration Reading
// [3] : X - Axis Gyroscope Negative (+) Calibration Reading
// [4] : Y - Axis Gyroscope Negative (-) Calibration Reading
// [5] : Z - Axis Gyroscope Negative (+) Calibration Reading
// [6] : X - Axis Accelerometer Calibration Reading - Initial
// [7] : Y - Axis Accelerometer Calibration Reading - Initial
// [8] : Z - Axis Accelerometer Calibration Reading - Initial
// [9] : X - Axis Accelerometer Calibration Reading - ST
// [10]: Y - Axis Accelerometer Calibration Reading - ST
// [11]: Z - Axis Accelerometer Calibration Reading - ST

signed long imu_st_cal[12];

void sensorsSelfTest()
{                                                                              
    volatile unsigned long delayVar;
    
    // Calibrate Gyroscope
    // -------------------
    I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, GYRO_ADDRESS, I2C_SEND);         // ST - SAD+W  
    I2CMasterDataPut(I2C1_MASTER_BASE, L3G4_CTRL_REG4);                      // SUB
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);     // Set to send
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                 // Wait for SAK
    
    I2CMasterDataPut(I2C1_MASTER_BASE, CTRL_REG_4_STPLUS);                   // SUB - POS
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);    // Set to send
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                 // Wait for SAK 
   
    for(unsigned long ulLoop = 0; ulLoop < 100000; ulLoop++)
    {
       delayVar = 0;
       delayVar = delayVar + 100;
       delayVar = delayVar - 100;
    }  
    
    readGyro(&x_gyro_raw, &y_gyro_raw, &z_gyro_raw);
    
    while(x_gyro_raw > -8000 || y_gyro_raw < 8000 || z_gyro_raw > -8000)
    {
        readGyro(&x_gyro_raw, &y_gyro_raw, &z_gyro_raw);
    }
    // Calibration Variables for Gyro for Self Test Positive
    imu_st_cal[0] = x_gyro_raw;
    imu_st_cal[1] = y_gyro_raw;
    imu_st_cal[2] = z_gyro_raw;
    
    I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, GYRO_ADDRESS, I2C_SEND);         // ST - SAD+W  
    I2CMasterDataPut(I2C1_MASTER_BASE, L3G4_CTRL_REG4);                      // SUB
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);     // Set to send
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                 // Wait for SAK
    
    I2CMasterDataPut(I2C1_MASTER_BASE, CTRL_REG_4_STMINUS);                  // SUB - NEG
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);    // Set to send
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                 // Wait for SAK
    
    for(unsigned long ulLoop = 0; ulLoop < 100000; ulLoop++)
    {
       delayVar = 0;
       delayVar = delayVar + 100;
       delayVar = delayVar - 100;
    }  
    
    readGyro(&x_gyro_raw, &y_gyro_raw, &z_gyro_raw);
    
    while(x_gyro_raw < 8000 || y_gyro_raw > -8000 || z_gyro_raw < 8000)
    {
        readGyro(&x_gyro_raw, &y_gyro_raw, &z_gyro_raw);
    }
    
    // Calibration Variables for Gyro for Self Test Negative
    imu_st_cal[3] = x_gyro_raw;
    imu_st_cal[4] = y_gyro_raw;
    imu_st_cal[5] = z_gyro_raw;
  
    
    // Set Gyro back to default
    I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, GYRO_ADDRESS, I2C_SEND);         // ST - SAD+W  
    I2CMasterDataPut(I2C1_MASTER_BASE, L3G4_CTRL_REG4);                      // SUB
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);     // Set to send
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                 // Wait for SAK
    
    I2CMasterDataPut(I2C1_MASTER_BASE, CTRL_REG_4_DEFAULT);                  // SUB - NEG
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);    // Set to send
    
    while(I2CMasterBusy(I2C1_MASTER_BASE)){}                                 // Wait for SAK
    
    for(unsigned long ulLoop = 0; ulLoop < 100000; ulLoop++)
    {
       delayVar = 0;
       delayVar = delayVar + 100;
       delayVar = delayVar - 100;
    }  
    
    x_gyro_offset = (int)((imu_st_cal[0])+(imu_st_cal[3]));   // Add the +130 and -130 for offset
    y_gyro_offset = (int)((imu_st_cal[1])+(imu_st_cal[4]));   // Add the +130 and -130 for offset
    z_gyro_offset = (int)((imu_st_cal[2])+(imu_st_cal[5]));   // Add the +130 and -130 for offset
    
    x_gyro_scale = (float)(260/(float)(imu_st_cal[3]-imu_st_cal[0]));
    y_gyro_scale = (float)(260/(float)(imu_st_cal[1]-imu_st_cal[4]));
    z_gyro_scale = (float)(260/(float)(imu_st_cal[5]-imu_st_cal[2]));
    
    
    // Calibrate Accelerometer
    // -----------------------
    
    readAccel(&temp_raw, &x_acc_raw, &y_acc_raw, &z_acc_raw);
    
    imu_st_cal[6] = x_acc_raw; 
    imu_st_cal[7] = y_acc_raw;
    imu_st_cal[8] = z_acc_raw;
    
    
    // Pull ST line high                
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    
    GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_7, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7, 0xff);            

    for(unsigned long ulLoop = 0; ulLoop < 100000; ulLoop++)
    {
       delayVar = 0;
       delayVar = delayVar + 100;
       delayVar = delayVar - 100;
    }                       
    
    readAccel(&temp_raw, &x_acc_raw, &y_acc_raw, &z_acc_raw);
    
    imu_st_cal[9] = x_acc_raw; 
    imu_st_cal[10] = y_acc_raw;
    imu_st_cal[11] = z_acc_raw; 
    
    x_acc_offset = imu_st_cal[6] - imu_st_cal[9] - 127;
    y_acc_offset = imu_st_cal[7] - imu_st_cal[10] + 127;
    z_acc_offset = imu_st_cal[8] - imu_st_cal[11] +  230;
    
    // Pull ST line low                
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7, 0x00);
    
    for(unsigned long ulLoop = 0; ulLoop < 100000; ulLoop++)
    {
       delayVar = 0;
       delayVar = delayVar + 100;
       delayVar = delayVar - 100;
    }                       
}
// ----------------------------------------- 