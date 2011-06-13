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

float x_Pgain = 8.5f;           // X-Axis - P Gain - 8.5
float x_Igain = 0.0f;          // X-Axis - I Gain - 0.01
float x_Dgain = 0.0f;          // X-Axis - D Gain - 85.0
float x_Fgain = 0.0f;           // X-Axis - F Gain - 0.0


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

float y_Pgain = 8.5f;           // Y-Axis - P Gain - 8.5
float y_Igain = 0.01f;          // Y-Axis - I Gain - 0.01
float y_Dgain = 85.0f;          // Y-Axis - D Gain - 85.0
float y_Fgain = 0.0f;           // Y-Axis - F Gain - 0.0


float y_Dterm_1 = 0.0f;		// Y-Axis - D Term Filter Interation 1 
float y_Dterm_2 = 0.0f;		// Y-Axis - D Term Filter Interation 2
float y_Dterm_3 = 0.0f;		// Y-Axis - D Term Filter Interation 3


float y_angle_error = 0.0f;     // Y Angle Error
float y_cmd_angle = 0.0f;       // Y Angle Commanded
float y_old_angle = 0.0f;       // Y Angle Old Reading

float y_torque = 0.0f;          // Y Axis - Rotational Torque
//

// Z-Axis Data
float z_thrust = 76530.0f;      // Z Axis Thrust // Idle Thrust = 76530.0
//

// IMU Data
static float x_angle;                  // X Angle - IMU
static float x_ang_vel;                // X Angular Rotation - IMU

static float y_angle;                  // Y Angle - IMU
static float y_ang_vel;                // Y Angular Rotation - IMU

//static float z_angle;                // Z Angle - IMU
//static float z_ang_vel;              // Z Angular Rotation - IMU

static float dt;
//

// Motor Data
unsigned long y_motor_dutycycle_m1 = 64000;    // Motor 1 - Y-Axis
unsigned long y_motor_dutycycle_m2 = 64000;    // Motor 2 - Y-Axis
unsigned long x_motor_dutycycle_m3 = 64000;    // Motor 3 - X-Axis
unsigned long x_motor_dutycycle_m4 = 64000;    // Motor 4 - X-Axis

unsigned long motor_upperbound = 80000;
unsigned long motor_lowerbound = 74000;

float m1_scale = 1.0018f;     //1.0018         // Motor 1 - Scalar - Y-Axis
float m2_scale = 1.0000f;                      // Motor 2 - Scalar - Y-Axis
float m3_scale = 1.0018f;     //1.0018         // Motor 3 - Scalar - X-Axis
float m4_scale = 1.0000f;                      // Motor 4 - Scalar - X-Axis
//

// Reinforcement Learning Data
unsigned long learnSample = 0;                  // Sample Rate
float rho = 0.001;                              // Learning Rate
float epsilon = 16.0f;                          // Epsilon - Acceptable Angle Error Range
float vel_epsilon = 10.0f;                      // Velocity Epsilon - Acceptable Angular Velocity Error Range
float b_epsilon = 10.0f;                         // Epsilon - Balanced
//


//*****************************************************************************
// Control
//
// Control function to stabilize the drone
//
void Control(float *imu)
{   
    // Load IMU data to Control
    x_angle = imu[0];                           // Load IMU X-Angle
    y_angle = imu[1];                           // Load IMU Y-Angle
    //z_angle = imu[2];                         // Load IMU Z-Angle
    
    x_ang_vel = imu[3];                         // Load IMU X-Angular Velocity
    y_ang_vel = imu[4];                         // Load IMU Y-Angular Velocity
    //z_ang_vel = imu[5];                       // Load IMU Z-Angular Velocity
    //
    
    dt = imu[6];                                // Load IMU dt
    dt = dt*667;                                // Convert dt so gains are small
    
    // PID Controller for X-Axis
    // *************************
    
    // Determine Error by difference of command angle and state angle
    x_angle_error = (x_cmd_angle - x_angle);    
    
    x_Pterm = x_Pgain*x_angle_error;                              // Determine P Term
    x_Iterm = x_Igain*(x_Iterm + (x_angle_error*dt));             // Determine I Term
    x_Dterm = x_Dgain*((x_angle_error - x_old_angle)/dt);         // Determine D Term
    x_angle_vel_term = x_Fgain*x_ang_vel;                         // Determine F Term
    
    // Set current angle as old angle for next loop
    x_old_angle = x_angle_error;                
 
    // Filter D Term - Prevent noise near stable zones
    x_Dterm = (x_Dterm_3 + (2 * x_Dterm_2) + (2 * x_Dterm_1) + x_Dterm)/6.0;
    x_Dterm_1 = x_Dterm;
    x_Dterm_2 = x_Dterm_1;
    x_Dterm_3 = x_Dterm_2;
    
    // Add up the PID Terms + F Term to determine X Torque
    x_torque = x_Pterm + x_Iterm + x_Dterm + x_angle_vel_term;
    // *************************
    
    
    // PID Controller for Y-Axis
    // *************************
    // Determine Error by difference of command angle and state angle
    y_angle_error = (y_cmd_angle - y_angle);
    
    y_Pterm = y_Pgain*y_angle_error;                              // Determine P Term
    y_Iterm = y_Igain*(y_Iterm + (y_angle_error*dt));             // Determine I Term
    y_Dterm = y_Dgain*((y_angle_error - y_old_angle)/dt);         // Determine D Term
    y_angle_vel_term = y_Fgain*y_ang_vel;                         // Determine F Term
    
    // Set current angle as old angle for next loop
    y_old_angle = y_angle_error; 
    
    // Filter D Term - Prevent noise near stable zones
    y_Dterm = (y_Dterm_3 + (2 * y_Dterm_2) + (2 * y_Dterm_1) + y_Dterm)/6.0;
    y_Dterm_1 = y_Dterm;
    y_Dterm_2 = y_Dterm_1;
    y_Dterm_3 = y_Dterm_2;
    
    // Add up the PID Terms + F Term to determine Y Torque
    y_torque = y_Pterm + y_Iterm + y_Dterm + y_angle_vel_term;
    // *************************
    
    
    // Motor Control
    //
    // *************************
    
    // Combine Z-Thrust, Roll & Pitch Torque, and Yaw Torque 
    x_motor_dutycycle_m3 = (unsigned long)(m3_scale*(z_thrust + x_torque));
    x_motor_dutycycle_m4 = (unsigned long)(m4_scale*(z_thrust - x_torque)); 
    y_motor_dutycycle_m1 = (unsigned long)(m1_scale*(z_thrust + y_torque));
    y_motor_dutycycle_m2 = (unsigned long)(m2_scale*(z_thrust - y_torque));
 
    // Motor control limiting (Sanitize outputs)
    //
    if(y_motor_dutycycle_m1 > motor_upperbound)      // Motor 1 - Y-Axis
    {
      y_motor_dutycycle_m1 = motor_upperbound;
    }
    if(y_motor_dutycycle_m1 < motor_lowerbound)
    {
      y_motor_dutycycle_m1 = motor_lowerbound;
    } 
    
    if(y_motor_dutycycle_m2 > motor_upperbound)      // Motor 2 - Y-Axis
    {
      y_motor_dutycycle_m2 = motor_upperbound;
    }
    if(y_motor_dutycycle_m2 < motor_lowerbound)
    {
      y_motor_dutycycle_m2 = motor_lowerbound;
    }
    
    if(x_motor_dutycycle_m3 > motor_upperbound)      // Motor 3 - X-Axis
    {
      x_motor_dutycycle_m3 = motor_upperbound;
    }
    if(x_motor_dutycycle_m3 < motor_lowerbound)
    {
      x_motor_dutycycle_m3 = motor_lowerbound;
    }
    
    if(x_motor_dutycycle_m4 > motor_upperbound)      // Motor 4 - X-Axis
    {
      x_motor_dutycycle_m4 = motor_upperbound;
    }
    if(x_motor_dutycycle_m4 < motor_lowerbound)
    {
      x_motor_dutycycle_m4 = motor_lowerbound;
    }
    //
    
    // Set Motor PWM's
    //
    // Y-Axis Motor Control - Motor 1 & 2
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 81840);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, y_motor_dutycycle_m1);   // Motor 1 - PWM1 - Pin 35
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, y_motor_dutycycle_m2);   // Motor 2 - PWM0 - Pin 34
    
    // X-Axis Motor Control - Motor 3 & 4
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 81840);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, x_motor_dutycycle_m3);   // Motor 3 - PWM7 - Pin 31
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, x_motor_dutycycle_m4);   // Motor 4 - PWM6 - Pin 30
    // *************************
    
    
    // Send Control Data Telemetry
    // *************************
    //sendControlTelemetry(x_torque, x_Pterm, x_Iterm, x_Dterm);       // Send back current PID state
    sendControlTelemetry(x_angle_error, x_Pgain, x_Igain, x_Dgain);  // Send back PID gains
    // *************************
    
    
    // Reinforcement Learning for PID
    // *************************
    if(learnSample > 5)
    {
      learnSample = 0;
      
      if(fabsf(x_angle-x_cmd_angle) > b_epsilon)             // If out of Balance
      {  
        if(fabsf(x_angle-x_cmd_angle) <= epsilon)            // In Epsilon                   
        {
            if(fabsf(x_ang_vel-x_cmd_angle) <= vel_epsilon)  // Reward Slow Omega +                                    
            {
                x_Pgain = x_Pgain + rho*fabsf(x_angle-x_cmd_angle);
            }
            else                                             // Punish Fast Omega +++
            {
                x_Pgain = x_Pgain - 0.01; 
            }        
        }
        else                                                  // Out of Epsilon
        {
            if(fabsf(x_ang_vel-x_cmd_angle) <= vel_epsilon)   // Reward Slow Omega ++                                   
            {
                x_Pgain = x_Pgain + rho*fabsf(x_angle-x_cmd_angle);
            }
            else                                              // Punish Fast Omega +
            {
                x_Pgain = x_Pgain - 0.01; 
            } 
        }
      }
      
    }
    else
    {
      learnSample++;
    }
    // *************************
}
//



//*****************************************************************************
// Motor Spin Up
//
// Spin up motors to start flight
//
void motorSpinup()
{
  volatile unsigned long delay;
  
  while(x_motor_dutycycle_m4 < z_thrust) //76530 
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

    y_motor_dutycycle_m1 += 40;                                   // Increment Motors for spinup
    y_motor_dutycycle_m2 += 40;   

    x_motor_dutycycle_m3 += 40;
    x_motor_dutycycle_m4 += 40;
  }
  
  
}
//
  

//*****************************************************************************
// PID Tune
//
// Tune the PID loop via commands from the XBee (UART)
//
void PIDTune(char cmd)
{
  if(cmd == '1')
  {
     x_Pgain += 1.0f;     // Increase P Gain
  }
  else if(cmd == '2')
  {
     x_Pgain -= 1.0f;     // Decrease P Gain
  }
  else if(cmd == '3')
  {
     x_Igain += 0.01f;   // Increase I Gain
  }
  else if(cmd == '4')
  {
     x_Igain -= 0.01f;   // Decrease I Gain
  }
  else if(cmd == '5')
  {
     x_Dgain += 1.0f;     // Increase D Gain
  }
  else if(cmd == '6')
  {
     x_Dgain -= 1.0f;     // Decrease D Gain
  }
  else if(cmd == 'p')
  {
     sendControlTelemetry(x_torque, x_Pterm, x_Iterm, x_Dterm);       // Send back current PID state
  }
  
}
//