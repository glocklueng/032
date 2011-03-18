#include "inc/lm3s9b96.h"
#include "math.h"

#include "control.h"
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

// X-Axis PID Data
float x_Pterm;                  // X-Axis - P Term
float x_Iterm;                  // X-Axis - I Term
float x_Dterm;                  // X-Axis - D Term
float x_angle_vel_term;         // X-Axis - Angular Velocity Term

const float x_Pgain = 8.5f;     // X-Axis - P Gain - 8.5
const float x_Igain = 0.01f;    // X-Axis - I Gain - 0.01
const float x_Dgain = 85.0f;    // X-Axis - D Gain - 85.0
const float x_Fgain = 0.0f;     // X-Axis - F Gain - 0.0


float x_Dterm_1 = 0.0f;		// X-Axis - D Term Filter Interation 1 
float x_Dterm_2 = 0.0f;		// X-Axis - D Term Filter Interation 2
float x_Dterm_3 = 0.0f;		// X-Axis - D Term Filter Interation 3


float x_angle_error = 0.0f;     // X Angle Error
float x_cmd_angle = 0.0f;       // X Angle Commanded
float x_old_angle = 0.0f;       // X Angle Old Reading

float x_torque = 0.0f;          // X Axis - Rotational Torque
//


// Y-Axis PID Data
float y_Pterm;                  // Y-Axis - P Term
float y_Iterm;                  // Y-Axis - I Term
float y_Dterm;                  // Y-Axis - D Term 
float y_angle_vel_term;         // Y-Axis - Angular Velocity Term

const float y_Pgain = 8.5f;     // Y-Axis - P Gain - 8.5
const float y_Igain = 0.01f;    // Y-Axis - I Gain - 0.01
const float y_Dgain = 85.0f;    // Y-Axis - D Gain - 85.0
const float y_Fgain = 0.0f;     // Y-Axis - F Gain - 0.0


float y_Dterm_1 = 0.0f;		// Y-Axis - D Term Filter Interation 1 
float y_Dterm_2 = 0.0f;		// Y-Axis - D Term Filter Interation 2
float y_Dterm_3 = 0.0f;		// Y-Axis - D Term Filter Interation 3


float y_angle_error = 0.0f;     // Y Angle Error
float y_cmd_angle = 0.0f;       // Y Angle Commanded
float y_old_angle = 0.0f;       // Y Angle Old Reading

float y_torque = 0.0f;          // Y Axis - Rotational Torque
//

// IMU Data
static float x_angle;                  // X Angle - IMU
static float x_ang_vel;                // X Angular Rotation - IMU

static float y_angle;                  // Y Angle - IMU
static float y_ang_vel;                // Y Angular Rotation - IMU

//static float z_angle;                  // Z Angle - IMU
//static float z_ang_vel;                // Z Angular Rotation - IMU
//

// Motor Data
unsigned long y_motor_dutycycle_m1 = 64000;    // Motor 1 - Y-Axis
unsigned long y_motor_dutycycle_m2 = 64000;    // Motor 2 - Y-Axis
unsigned long x_motor_dutycycle_m3 = 64000;    // Motor 3 - X-Axis
unsigned long x_motor_dutycycle_m4 = 64000;    // Motor 4 - X-Axis

float m1_scale = 1.0018f;                      // Motor 1 - Scalar - Y-Axis
float m2_scale = 1.0000f;                      // Motor 2 - Scalar - Y-Axis
float m3_scale = 1.0018f;                      // Motor 3 - Scalar - X-Axis
float m4_scale = 1.0000f;                      // Motor 4 - Scalar - X-Axis
//


// Control function
//
void Control(float *imu)
{ 
    
    // Load IMU data to Control
    x_angle = imu[0];
    y_angle = imu[1];
    //z_angle = imu[2];
    
    x_ang_vel = imu[3];
    y_ang_vel = imu[4];
    //z_ang_vel = imu[5];
    //
    
    
    // PID Controller for X-Axis
    x_angle_error = (x_cmd_angle - x_angle);
    
    x_Pterm = x_Pgain*x_angle_error;
    x_Iterm = ((x_Igain*x_angle_error) + x_Iterm) * 0.99999;
    x_Dterm = x_Dgain*(x_angle_error - x_old_angle);
    
    x_old_angle = x_angle_error;
    
    x_angle_vel_term = x_Fgain*x_ang_vel;  
    
    x_Dterm = (x_Dterm_3 + (2 * x_Dterm_2) + (2 * x_Dterm_1) + x_Dterm)/6.0;
    x_Dterm_1 = x_Dterm;
    x_Dterm_2 = x_Dterm_1;
    x_Dterm_3 = x_Dterm_2;
    
    x_torque = x_Pterm + x_Iterm + x_Dterm + x_angle_vel_term;
    
    
    x_motor_dutycycle_m3 = (unsigned long)(m3_scale*((float)(76530.0f) + x_torque));
    x_motor_dutycycle_m4 = (unsigned long)(m4_scale*((float)(76530.0f) - x_torque));
    //
    
    // PID Controller for Y-Axis
    y_angle_error = (y_cmd_angle - y_angle);
    
    y_Pterm = y_Pgain*y_angle_error;
    y_Iterm = ((y_Igain*y_angle_error) + y_Iterm) * 0.99999;
    y_Dterm = y_Dgain*(y_angle_error - y_old_angle);
    
    y_old_angle = y_angle_error;
    
    y_angle_vel_term = y_Fgain*y_ang_vel;  
    
    y_Dterm = (y_Dterm_3 + (2 * y_Dterm_2) + (2 * y_Dterm_1) + y_Dterm)/6.0;
    y_Dterm_1 = y_Dterm;
    y_Dterm_2 = y_Dterm_1;
    y_Dterm_3 = y_Dterm_2;
    
    y_torque = y_Pterm + y_Iterm + y_Dterm + y_angle_vel_term;
    
    
    y_motor_dutycycle_m1 = (unsigned long)(m1_scale*((float)(76530.0f) + y_torque));
    y_motor_dutycycle_m2 = (unsigned long)(m2_scale*((float)(76530.0f) - y_torque));
    //
    
    
    // Motor control limiting (Sanitize outputs)
    // Prevent motors from going out of bounds
    if(y_motor_dutycycle_m1 > 80000)      // Motor 1 - Y-Axis
    {
      y_motor_dutycycle_m1 = 80000;
    }
    if(y_motor_dutycycle_m1 < 73100)
    {
      y_motor_dutycycle_m1 = 73100;
    } 
    
    if(y_motor_dutycycle_m2 > 80000)      // Motor 2 - Y-Axis
    {
      y_motor_dutycycle_m2 = 80000;
    }
    if(y_motor_dutycycle_m2 < 73100)
    {
      y_motor_dutycycle_m2 = 73100;
    }
    
    if(x_motor_dutycycle_m3 > 80000)      // Motor 3 - X-Axis
    {
      x_motor_dutycycle_m3 = 80000;
    }
    if(x_motor_dutycycle_m3 < 73100)
    {
      x_motor_dutycycle_m3 = 73100;
    }
    
    if(x_motor_dutycycle_m4 > 80000)      // Motor 4 - X-Axis
    {
      x_motor_dutycycle_m4 = 80000;
    }
    if(x_motor_dutycycle_m4 < 73100)
    {
      x_motor_dutycycle_m4 = 73100;
    }
    //
    
    // Y-Axis Motor Control - Motor 1 & 2
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 81840);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, y_motor_dutycycle_m1);   // Motor 1 - PWM1 - Pin 35
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, y_motor_dutycycle_m2);   // Motor 2 - PWM0 - Pin 34
    
    // X-Axis Motor Control - Motor 3 & 4
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 81840);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, x_motor_dutycycle_m3);   // Motor 3 - PWM7 - Pin 31
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, x_motor_dutycycle_m4);   // Motor 4 - PWM6 - Pin 30
    
    // Send Data Telemetry
    sendControlTelemetry(x_torque, x_Pterm, x_Iterm, x_Dterm);
}
//


// Motor Spinup:
// Spin up motors to start flight
//
void motorSpinup()
{
  volatile unsigned long delay;
  
  while(x_motor_dutycycle_m4 < 76530) //76530 
  {
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 81840);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, y_motor_dutycycle_m1);   // Motor 1 - PWM1 - Pin 35
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, y_motor_dutycycle_m2);   // Motor 2 - PWM0 - Pin 34
           
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 81840);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, x_motor_dutycycle_m3);   // Motor 3 - PWM7 - Pin 31
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, x_motor_dutycycle_m4);   // Motor 4 - PWM6 - Pin 30
    
    
    for(unsigned long ulLoop = 0; ulLoop < 20000; ulLoop++)       // Delay for motor spinup
    {
       delay = 0;
       delay = delay + 100;
       delay = delay - 100;
    }

    y_motor_dutycycle_m1 += 40;     
    y_motor_dutycycle_m2 += 40;   

    x_motor_dutycycle_m3 += 40;
    x_motor_dutycycle_m4 += 40;
  }
  
  
}
//