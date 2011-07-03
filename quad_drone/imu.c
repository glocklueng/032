#include "inc/lm3s9b96.h"
#include "math.h"
#include "fixedmath.h"

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
// *************************
const float pi = 3.141592f;		    // Pi
const float rad_byte = 40.584510;	    // Convert from Radians to Byte (0...255)
const float convert_Pi_180 = 0.017453f;	    // Pi/180 - Convert Degrees to Radians
const float convert_180_Pi = 57.29577f;	    // 180/Pi - Convert Radians to Degrees


//  IMU Variables and Constants
//*****************************************************************************

//  Accelerometer
// *************************
//  Accelerometer Voltage Shift
const float acc_v_convert = 0.003222f;

//  Accelerometer Convert Volts to G's
const float acc_g_convert = 3.030303f; 

//  Raw Accelerometer Readings
//  [0] : X Accelerometer Reading
//  [1] : Y Accelerometer Reading
//  [2] : Z Accelerometer Reading
unsigned long acc_raw[3] = {0,0,0};

//  Accelerometer Offset
//  [0] : X Accelerometer Offset
//  [1] : Y Accelerometer Offset
//  [2] : Z Accelerometer Offset
int acc_offset[3] = {0,0,0};

//  Angle Scale
//  [0] : X Angle Scale
//  [1] : Y Angle Scale
//  [2] : Z Angle Scale
const float angle_scale[3] = {1.5f,1.5f,1.5f};

//  Angle Offset
//  [0] : X Angle Offset
//  [1] : Y Angle Offset
//  [2] : Z Angle Offset
const float angle_offset[3] = {-15.0f,-15.0f,-15.0f};

//  Accelerometer Voltage Shift
//  [0] : X Accelerometer Voltage Shift
//  [1] : Y Accelerometer Voltage Shift
//  [2] : Z Accelerometer Voltage Shift
const float acc_shift[3] = {1.65f,1.65f,1.65f};

//  Acceleration in G's
//  [0] : X Acceleration - in G's
//  [1] : Y Acceleration - in G's
//  [2] : Z Acceleration - in G's
float accel[3] = {0.0f,0.0f,0.0f};

//  Raw Angle in radians
//  [0] : X Axis Angle - Raw angle in radians
//  [1] : Y Axis Angle - Raw angle in radians
//  [2] : Z Axis Angle - Raw angle in radians
float acc_rad[3] = {0.0f,0.0f,0.0f};

//  Angle State Estimation
//  [0] : X Axis Angle - State Estimation
//  [1] : Y Axis Angle - State Estimation
//  [2] : Z Axis Angle - State Estimation
float angle[3] = {0.0f,0.0f,0.0f};

//  Angle Bias for State Estimation
//  [0] : X Bias for State Matrix - +4.5 degrees
//  [1] : Y Bias for State Matrix - -1.5 degrees
//  [2] : Z Bias for State Matrix - -1.5 degrees
float bias[3] = {0.0785385f,-0.0261795f,-0.0261795f};
// *************************

//  Gyroscope
// *************************
//  Gyro Raw Reading
//  [0] : X Gyro Raw Reading
//  [1] : Y Gyro Raw Reading
//  [2] : Z Gyro Raw Reading
signed long gyro_raw[3] = {0,0,0};                            

//  Gyro Offset
//  [0] : X Gyro Offset
//  [1] : Y Gyro Offset
//  [2] : Z Gyro Offset
int gyro_offset[3] = {-200,49,9};   

//  Gyro Scale
float gyro_scale = 0.007629f;	        

//  X Gyro State Estimation
//  [0] : X Gyro - Radians/Second - State Estimation	
//  [1] : X Gyro - Runge-Kutta Integration - Itteration 1
//  [2] : X Gyro - Runge-Kutta Integration - Itteration 2
//  [3] : X Gyro - Runge-Kutta Integration - Itteration 3
float x_gyro_rad_sec[4] = {0.0f,0.0f,0.0f,0.0f};

// Y Gyro State Estimation
//  [0] : Y Gyro - Radians/Second - State Estimation	
//  [1] : Y Gyro - Runge-Kutta Integration - Itteration 1
//  [2] : Y Gyro - Runge-Kutta Integration - Itteration 2
//  [3] : Y Gyro - Runge-Kutta Integration - Itteration 3
float y_gyro_rad_sec[4] = {0.0f,0.0f,0.0f,0.0f};

// Z Gyro State Estimation
//  [0] : Z Gyro - Radians/Second - State Estimation	
//  [1] : Z Gyro - Runge-Kutta Integration - Itteration 1
//  [2] : Z Gyro - Runge-Kutta Integration - Itteration 2
//  [3] : Z Gyro - Runge-Kutta Integration - Itteration 3
float z_gyro_rad_sec[4] = {0.0f,0.0f,0.0f,0.0f};
// *************************

//  Quaternion Variables and Constants
// *************************
//  State in Quaternions
//  [0] : q0
//  [1] : q1
//  [2] : q2
//  [0] : q3
fm_fixed q[4] = {0,0,0,0};

//  Readings in Euler Angles
//  [0] : X Euler Angle - Phi
//  [1] : Y Euler Angle - Theta
//  [2] : Z Euler Angle - Psi
fm_fixed euler_angle[3] = {0,0,0};

//  Readings in Euler Angles
//  [0][0] : X Euler Angle - Cosine Phi
//  [1][0] : Y Euler Angle - Cosine Theta
//  [2][0] : Z Euler Angle - Cosine Psi
//  [0][1] : X Euler Angle - Sine Phi
//  [1][1] : Y Euler Angle - Sine Theta
//  [2][1] : Z Euler Angle - Sine Psi 
fm_fixed euler_trig[3][2] = {{0,0},{0,0},{0,0}};
// *************************

// Time Variables
// *************************
unsigned int previousTimer;             // The previous time from the timer
unsigned int currentTimer;              // The current time from the timer
unsigned long startFlag = 0;            // Flag for when dt clock starts
float measuredDt;                       // Measured dt
extern float dt;		        // dt = 0.002 seconds per sample
// *************************

//  Kalman Variables and Constants
// *************************
const float Q_angle = 0.001f;		// 0.001 - Q constant for angle
const float Q_gyro = 0.012f;		// 0.012 - Q constant for gyro
const float R_angle = 0.1f;		// 0.1   - R constant for noise    

float x_y;				// Difference between previous raw angle reading and previous state angle - X-Axis
float xS;				// S Variable - X Axis
float xK[2];                            // K Matrix - X Axis
float xP[2][2];                         // P Matrix - X Axis

float y_y;				// Difference between previous raw angle reading and previous state angle - Y-Axis
float yS;				// S Variable - Y Axis
float yK[2];                            // K Matrix - Y Axis
float yP[2][2];                         // P Matrix - Y Axis

float z_y;				// Difference between previous raw angle reading and previous state angle - Z-Axis
float zS;				// S Variable - Z Axis
float zK[2];                            // K Matrix - Z Axis
float zP[2][2];                         // P Matrix - Z Axis
// *************************

// Compass Variables
// *************************

//  Compass Raw Values
//  [0] : X Compass Reading
//  [1] : Y Compass Reading
//  [2] : Z Compass Reading
signed long comp_raw[3] = {0,0,0};
// *************************

// Temperature Variables
// *************************
unsigned long temp_raw;                 // Raw Temperature Reading from ADC
float temperature;                      // Temperature in C
// *************************
//*****************************************************************************




//*****************************************************************************
// Read IMU - Filtered results
//
// [0]  : X Angle - roll   (deg)
// [1]  : Y Angle - pitch  (deg)
// [2]  : Z Angle - yaw    (deg)
// [3]  : X Rate  - roll angular speed  (deg/sec)
// [4]  : Y Rate  - pitch angular speed (deg/sec)
// [5]  : Z Rate  - yaw angular speed   (deg/sec)    
// [6]  : X Acceleration
// [7]  : Y Acceleration
// [8]  : Z Acceleration
// [9]  : X Compass Gauss
// [10] : Y Compass Gauss
// [11] : Z Compass Gauss
// [12] : dt
// [13] : Temperature
//
void readIMU(float *imu)
{     
    // Calculate dt for integration
    // ****************************
    if(startFlag == 0)
    {
        startFlag = 1;  // dt calculation starts
        
        previousTimer = TimerValueGet(TIMER0_BASE, TIMER_A);  // Grab First Time
    }
    else
    {
        currentTimer = TimerValueGet(TIMER0_BASE, TIMER_A);   // Grab Current Time
        
        measuredDt = (float)SysCtlClockGet();                 // Debug Reasons
        
        //Subtract Previous Timer by Current Timer then divide by system speed to obtain dt
        measuredDt = (float)((float)(previousTimer - currentTimer)/(float)SysCtlClockGet());  
        
        if(measuredDt > 0.0000000f)    // As long as time is positive set it to dt
        {
          //dt = measuredDt;
        }
        
        previousTimer = currentTimer;  // Set Current Timer to Previous Timer for next loop
    }
    // ****************************
   
    
    // Read Values then convert to real
    // ****************************
    // Read ADC - X Axis Acc
    // Read ADC - Y Axis Acc
    // Read ADC - Z Axis Acc
    readAccel(&temp_raw, &acc_raw[0], &acc_raw[1], &acc_raw[2]);
    
    // Read I2C - X Axis Gyro
    // Read I2C - Y Axis Gyro
    // Read I2C - Z Axis Gyro
    readGyro(&gyro_raw[0], &gyro_raw[1], &gyro_raw[2]);
    
    // Read Raw - X Axis Compass
    // Read Raw - Y Axis Compass
    // Read Raw - Z Axis Compass
    readCompass(&comp_raw[0], &comp_raw[1], &comp_raw[2]);
    
    // Convert raw adc values to acceleration
    accel[0] = ((acc_v_convert*(float)((int)(acc_raw[0]) - acc_offset[0]))-acc_shift[0])*acc_g_convert; // Convert raw data bytes to acceleration - X Axis
    accel[1] = ((acc_v_convert*(float)((int)(acc_raw[1]) - acc_offset[1]))-acc_shift[1])*acc_g_convert; // Convert raw data bytes to acceleration - Y Axis
    accel[2] = ((acc_v_convert*(float)((int)(acc_raw[2]) - acc_offset[2]))-acc_shift[2])*acc_g_convert; // Convert raw data bytes to acceleration - Z Axis
    
    // Convert acceleration to angles    
    acc_rad[0] = (float)((angle_scale[0]*((((float)atan(accel[1]/sqrt(accel[0]*accel[0] + accel[2]*accel[2])))*convert_180_Pi) + angle_offset[0]))*convert_Pi_180);
    acc_rad[1] = (float)((angle_scale[1]*((((float)atan(accel[0]/sqrt(accel[1]*accel[1] + accel[2]*accel[2])))*convert_180_Pi) + angle_offset[1]))*convert_Pi_180);
    acc_rad[2] += z_gyro_rad_sec[0]*dt;  // Integrate Z Angular Velocity to obtain yaw angle 
    
    x_gyro_rad_sec[0]  = (float)(((float)(gyro_raw[0] - gyro_offset[0])* gyro_scale)*convert_Pi_180);	 // Convert raw data bytes to angular velocity - X Axis
    y_gyro_rad_sec[0]  = (float)(((float)(gyro_raw[1] - gyro_offset[1])* gyro_scale)*convert_Pi_180);	 // Convert raw data bytes to angular velocity - Y Axis
    z_gyro_rad_sec[0]  = (float)(((float)(gyro_raw[2] - gyro_offset[2])* gyro_scale)*convert_Pi_180);	 // Convert raw data bytes to angular velocity - Z Axis
    // ****************************
    
    // Prefiltered Values
    // ****************************
    /*
    imu[0] = acc_rad[0]*convert_180_Pi;
    imu[1] = acc_rad[1]*convert_180_Pi;
    imu[2] = acc_rad[2]*convert_180_Pi;
    imu[3] = x_gyro_rad_sec[0]*convert_180_Pi;
    imu[4] = y_gyro_rad_sec[0]*convert_180_Pi;
    imu[5] = z_gyro_rad_sec[0]*convert_180_Pi;
    imu[6] = x_acc
    imu[7] = y_acc
    imu[8] = z_acc
    imu[9] = dt;
    imu[10] = temp_raw; 
    */
    // ****************************
    
    
    // Filter
    // ****************************
    
    // Runge-Kutta Integration 
    // -----------------------
    // The Runge-Kutta Integration will average 
    // out the gyroscope readings.
    
    // X-Axis Gyro - Runge-Kutta Integration 
    x_gyro_rad_sec[0] = (x_gyro_rad_sec[3] + (2 * x_gyro_rad_sec[2]) + (2 * x_gyro_rad_sec[1]) + x_gyro_rad_sec[0])/6.0;
    x_gyro_rad_sec[1] = x_gyro_rad_sec[0];
    x_gyro_rad_sec[2] = x_gyro_rad_sec[1];
    x_gyro_rad_sec[3] = x_gyro_rad_sec[2];
    
    // Y-Axis Gyro - Runge-Kutta Integration 
    y_gyro_rad_sec[0] = (y_gyro_rad_sec[3] + (2 * y_gyro_rad_sec[2]) + (2 * y_gyro_rad_sec[1]) + y_gyro_rad_sec[0])/6.0;
    y_gyro_rad_sec[1] = y_gyro_rad_sec[0];
    y_gyro_rad_sec[2] = y_gyro_rad_sec[1];
    y_gyro_rad_sec[3] = y_gyro_rad_sec[2];
    
    // Z-Axis Gyro - Runge-Kutta Integration 
    z_gyro_rad_sec[0] = (z_gyro_rad_sec[3] + (2 * z_gyro_rad_sec[2]) + (2 * z_gyro_rad_sec[1]) + z_gyro_rad_sec[0])/6.0;
    z_gyro_rad_sec[1] = z_gyro_rad_sec[0];
    z_gyro_rad_sec[2] = z_gyro_rad_sec[1];
    z_gyro_rad_sec[3] = z_gyro_rad_sec[2];
    
    
    // Integrate the gyro axis with a bias to convert to angle then add to angle
    angle[0] += ((x_gyro_rad_sec[0] - bias[0])) * dt;			// integrate x axis gyro with bias then add to x angle
    angle[1] += ((y_gyro_rad_sec[0] - bias[1])) * dt;			// integrate y axis gyro with bias then add to y angle
    angle[2] += ((z_gyro_rad_sec[0] - bias[2])) * dt;			// integrate z axis gyro with bias then add to z angle

        
    // Kalman Filter 
    // **********************
    
    // Filter X Axis
    // ----------------------
    // Predict 
    xP[0][0] -=  dt * (xP[1][0] + xP[0][1]) + Q_angle * dt;
    xP[0][1] -=  dt * xP[1][1];
    xP[1][0] -=  dt * xP[1][1];
    xP[1][1] +=  Q_gyro * dt; 
    
    angle[0] += (x_gyro_rad_sec[0] - bias[0])*dt;
    
    // Update with new data
    x_y = acc_rad[0] - angle[0];    
    xS = xP[0][0] + R_angle;
    xK[0] = xP[0][0] / xS;
    xK[1] = xP[1][0] / xS;
    
    // Update State
    angle[0] +=  xK[0] * x_y;
    bias[0]  +=  xK[1] * x_y;
    
    // Update P Matrix
    xP[0][0] -= xK[0] * xP[0][0];
    xP[0][1] -= xK[0] * xP[0][1];
    xP[1][0] -= xK[1] * xP[0][0];
    xP[1][1] -= xK[1] * xP[0][1];
    
    
    // Filter Y Axis
    // ----------------------
    // Predict 
    yP[0][0] -=  dt * (yP[1][0] + yP[0][1]) + Q_angle * dt;
    yP[0][1] -=  dt * yP[1][1];
    yP[1][0] -=  dt * yP[1][1];
    yP[1][1] +=  Q_gyro * dt; 
    
    angle[1] += (y_gyro_rad_sec[0] - bias[1])*dt;
    
    // Update with new data
    y_y = acc_rad[1] - angle[1];
    yS = yP[0][0] + R_angle;
    yK[0] = yP[0][0] / yS;
    yK[1] = yP[1][0] / yS;
    
    // Update State
    angle[1] +=  yK[0] * y_y;
    bias[1]  +=  yK[1] * y_y;
    
    // Update P Matrix
    yP[0][0] -= yK[0] * yP[0][0];
    yP[0][1] -= yK[0] * yP[0][1];
    yP[1][0] -= yK[1] * yP[0][0]; 
    yP[1][1] -= yK[1] * yP[0][1];
    
    
    // Filter Z Axis
    // ----------------------
    // Predict 
    zP[0][0] -=  dt * (zP[1][0] + zP[0][1]) + Q_angle * dt;
    zP[0][1] -=  dt * zP[1][1];
    zP[1][0] -=  dt * zP[1][1];
    zP[1][1] +=  Q_gyro * dt; 
    
    angle[2] += (z_gyro_rad_sec[0] - bias[2])*dt;
    
    // Update with new data
    z_y = acc_rad[2] - angle[2];
    zS = zP[0][0] + R_angle;
    zK[0] = zP[0][0] / zS;
    zK[1] = zP[1][0] / zS;
    
    // Update State
    angle[2] +=  zK[0] * z_y;
    bias[2]  +=  zK[1] * z_y;
    
    // Update P Matrix
    zP[0][0] -= zK[0] * zP[0][0];
    zP[0][1] -= zK[0] * zP[0][1];
    zP[1][0] -= zK[1] * zP[0][0]; 
    zP[1][1] -= zK[1] * zP[0][1];
    
    // Kalman Filtered Values
    // ----------------------
    imu[0] = angle[0]*convert_180_Pi;
    imu[1] = angle[1]*convert_180_Pi;
    imu[2] = angle[2]*convert_180_Pi;
    imu[3] = (x_gyro_rad_sec[0] - bias[0])*convert_180_Pi;
    imu[4] = (y_gyro_rad_sec[0] - bias[1])*convert_180_Pi;
    imu[5] = (z_gyro_rad_sec[0] - bias[2])*convert_180_Pi;
    imu[6] = acc_raw[0];
    imu[7] = acc_raw[1];
    imu[8] = acc_raw[2];
    imu[9] = comp_raw[0];
    imu[10] = comp_raw[1];
    imu[11] = comp_raw[2];
    imu[12] = dt;
    imu[13] = temp_raw; 
    // ----------------------
    //
    // **********************
    // ****************************
    
    // Update the State of the Quadrotor
    //
    updateState(&imu[0]);
    //
    
    // Send Data Telemetry
    //
    //sendDataTelemetry(&imu[0], dt);
    //sendMAVLinkData(&imu[0], dt);
    //
}
//*****************************************************************************


//*****************************************************************************
// Update State
//
void updateState(float *eulerAngle)
{
    // [0] - eulerAngle - Phi - X
    // [1] - eulerAngle - Theta - Y
    // [2] - eulerAngle - Psi - Z

    // Convert Euler Angles into Quaternions  
    euler_angle[0] =  ftofix(eulerAngle[0]*convert_Pi_180*rad_byte/2);	
    euler_angle[1] = ftofix(eulerAngle[1]*convert_Pi_180*rad_byte/2); 
    euler_angle[2] = ftofix(eulerAngle[2]*convert_Pi_180*rad_byte/2);
    
    euler_trig[0][0] = fixcos(euler_angle[0]);
    euler_trig[1][0] = fixcos(euler_angle[1]);
    euler_trig[2][0] = fixcos(euler_angle[2]);

    euler_trig[0][1] = fixsin(euler_angle[0]);
    euler_trig[1][1] = fixsin(euler_angle[1]);
    euler_trig[2][1] = fixsin(euler_angle[2]);

    q[0] = fixadd((fixmul(fixmul(euler_trig[0][0],euler_trig[1][0]),euler_trig[2][0])),(fixmul(fixmul(euler_trig[0][1],euler_trig[1][1]),euler_trig[2][1])));
    q[1] = fixsub((fixmul(fixmul(euler_trig[0][1],euler_trig[1][0]),euler_trig[2][0])),(fixmul(fixmul(euler_trig[0][0],euler_trig[1][1]),euler_trig[2][1])));
    q[2] = fixadd((fixmul(fixmul(euler_trig[0][0],euler_trig[1][1]),euler_trig[2][0])),(fixmul(fixmul(euler_trig[0][1],euler_trig[1][0]),euler_trig[2][1])));
    q[3] = fixsub((fixmul(fixmul(euler_trig[0][0],euler_trig[1][0]),euler_trig[2][1])),(fixmul(fixmul(euler_trig[0][1],euler_trig[1][1]),euler_trig[2][0])));

    // Convert Quaternions to Euler Angles
    float f_phi = (fixtof(fixatan2((fixmul(ftofix(2),fixadd(fixmul(q[0],q[1]),fixmul(q[2],q[3])))),fixsub(ftofix(1),(fixmul(ftofix(2),fixadd(fixmul(q[1],q[1]),fixmul(q[2],q[2])))))))/(rad_byte))*convert_180_Pi;
    float f_theta = (fixtof(fixasin(fixmul(ftofix(2),fixsub(fixmul(q[0],q[2]),fixmul(q[3],q[1])))))/(rad_byte))*convert_180_Pi;
    float f_psi = (fixtof(fixatan2((fixmul(ftofix(2),fixadd(fixmul(q[0],q[3]),fixmul(q[1],q[2])))),fixsub(ftofix(1),(fixmul(ftofix(2),fixadd(fixmul(q[2],q[2]),fixmul(q[3],q[3])))))))/(rad_byte))*convert_180_Pi;
                             
    
    // TEST CODE - Set the quaternions back to the IMU matrix
    eulerAngle[0] = f_phi;
    eulerAngle[1] = f_theta;
    eulerAngle[2] = f_psi;
}
//*****************************************************************************


//*****************************************************************************
// Read Accelerometer
//
// ADC Value Matrix:
// [0] : Temperature
// [1] : X - Axis Accelerometer
// [2] : Y - Axis Accelerometer
// [3] : Z - Axis Accelerometer
//
unsigned long adc_values[4];
// -----------------------------------------   
void readAccel(unsigned long *temp, unsigned long *x_acc,
               unsigned long *y_acc, unsigned long *z_acc)
{
    IntMasterDisable();
    
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
    
    IntMasterEnable();
}
//*****************************************************************************



//***************************************************************************** 
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
    IntMasterDisable();
    
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
    
    IntMasterEnable();
}
//*****************************************************************************



//*****************************************************************************   
// Read Compass
//
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
    IntMasterDisable();
    
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
    
    IntMasterEnable();
}
//***************************************************************************** 




//*****************************************************************************
// IMU Startup
//
// Start up the IMU by calibrating
//
void imuStartup()
{
   sensorsSelfTest();
}
//*****************************************************************************




//*****************************************************************************
// Sensor Self Test
//
//
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
//
signed long imu_st_cal[12];
// ----------------------------------------- 
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
    
    readGyro(&gyro_raw[0], &gyro_raw[1], &gyro_raw[2]);
    
    while(gyro_raw[0] > -8000 || gyro_raw[1] < 8000 || gyro_raw[2] > -8000)
    {
        readGyro(&gyro_raw[0], &gyro_raw[1], &gyro_raw[2]);
    }
    // Calibration Variables for Gyro for Self Test Positive
    imu_st_cal[0] = gyro_raw[0];
    imu_st_cal[1] = gyro_raw[1];
    imu_st_cal[2] = gyro_raw[2];
    
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
    
    readGyro(&gyro_raw[0], &gyro_raw[1], &gyro_raw[2]);
    
    while(gyro_raw[0] < 8000 || gyro_raw[1] > -8000 || gyro_raw[2] < 8000)
    {
        readGyro(&gyro_raw[0], &gyro_raw[1], &gyro_raw[2]);
    }
    
    // Calibration Variables for Gyro for Self Test Negative
    imu_st_cal[3] = gyro_raw[0];
    imu_st_cal[4] = gyro_raw[1];
    imu_st_cal[5] = gyro_raw[2];
  
    
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
    
    gyro_offset[0] = (int)((imu_st_cal[0])+(imu_st_cal[3]));   // Add the +130 and -130 for offset
    gyro_offset[1] = (int)((imu_st_cal[1])+(imu_st_cal[4]));   // Add the +130 and -130 for offset
    gyro_offset[2] = (int)((imu_st_cal[2])+(imu_st_cal[5]));   // Add the +130 and -130 for offset
    
    gyro_scale = (float)(260/(float)(imu_st_cal[3]-imu_st_cal[0]));
    gyro_scale = (float)(260/(float)(imu_st_cal[1]-imu_st_cal[4]));
    gyro_scale = (float)(260/(float)(imu_st_cal[5]-imu_st_cal[2]));
    
    
    // Calibrate Accelerometer
    // -----------------------
    
    readAccel(&temp_raw, &acc_raw[0], &acc_raw[1], &acc_raw[2]);
    
    imu_st_cal[6] = acc_raw[0]; 
    imu_st_cal[7] = acc_raw[1];
    imu_st_cal[8] = acc_raw[2];
    
    
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
    
    readAccel(&temp_raw, &acc_raw[0], &acc_raw[1], &acc_raw[2]);
    
    imu_st_cal[9] = acc_raw[0]; 
    imu_st_cal[10] = acc_raw[1];
    imu_st_cal[11] = acc_raw[2]; 
    
    acc_offset[0] = imu_st_cal[6] - imu_st_cal[9] - 127;
    acc_offset[1] = imu_st_cal[7] - imu_st_cal[10] + 127;
    acc_offset[2] = imu_st_cal[8] - imu_st_cal[11] +  230;
    
    // Pull ST line low                
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7, 0x00);
    
    for(unsigned long ulLoop = 0; ulLoop < 100000; ulLoop++)
    {
       delayVar = 0;
       delayVar = delayVar + 100;
       delayVar = delayVar - 100;
    }                       
}
//***************************************************************************** 