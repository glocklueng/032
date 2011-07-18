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
// *************************
float x_Pterm = 0.0f;           // X-Axis - P Term                                                          
float x_Iterm = 0.0f;           // X-Axis - I Term
float x_Dterm = 0.0f;           // X-Axis - D Term
float x_angle_vel_term = 0.0f;  // X-Axis - Angular Velocity Term

const float x_Pgain = 8.5f;     // X-Axis - P Gain - 8.5
const float x_Igain = 0.01f;    // X-Axis - I Gain - 0.01
const float x_Dgain = 0.1f;     // X-Axis - D Gain - 85.0
const float x_Fgain = 0.0f;     // X-Axis - F Gain - 0.0

float x_Dterm_1 = 0.0f;		// X-Axis - D Term Filter Interation 1 
float x_Dterm_2 = 0.0f;		// X-Axis - D Term Filter Interation 2
float x_Dterm_3 = 0.0f;		// X-Axis - D Term Filter Interation 3

float x_angle_error = 0.0f;     // X Angle Error
float x_cmd_angle = 0.0f;       // X Angle Commanded
float x_old_angle = 0.0f;       // X Angle Old Reading

float x_torque = 0.0f;          // X Axis - Rotational Torque
// *************************
//

// Y-Axis PID Data
// *************************
float y_Pterm = 0.0f;           // Y-Axis - P Term
float y_Iterm = 0.0f;           // Y-Axis - I Term
float y_Dterm = 0.0f;           // Y-Axis - D Term 
float y_angle_vel_term = 0.0f;  // Y-Axis - Angular Velocity Term

const float y_Pgain = 8.5f;     // Y-Axis - P Gain - 8.5
const float y_Igain = 0.01f;    // Y-Axis - I Gain - 0.01
const float y_Dgain = 0.1f;     // Y-Axis - D Gain - 85.0
const float y_Fgain = 0.0f;     // Y-Axis - F Gain - 0.0


float y_Dterm_1 = 0.0f;		// Y-Axis - D Term Filter Interation 1 
float y_Dterm_2 = 0.0f;		// Y-Axis - D Term Filter Interation 2
float y_Dterm_3 = 0.0f;		// Y-Axis - D Term Filter Interation 3


float y_angle_error = 0.0f;     // Y Angle Error
float y_cmd_angle = 0.0f;       // Y Angle Commanded
float y_old_angle = 0.0f;       // Y Angle Old Reading

float y_torque = 0.0f;          // Y Axis - Rotational Torque
// *************************
//

// Z-Axis Data
// *************************
const float z_thrust = 5400.0f; // Z Axis Thrust - Idle Thrust = 6000.0
// *************************
//

// IMU Data
// *************************
static float dt;
// *************************
//

// Motor Data
// *************************
//  Motor Dutycycles
//  [0] : Motor 1 - Y-Axis - Duty Cycle
//  [1] : Motor 2 - Y-Axis - Duty Cycle
//  [2] : Motor 3 - X-Axis - Duty Cycle
//  [3] : Motor 4 - X-Axis - Duty Cycle
unsigned long motor_dutycycle[4] = {0,0,0,0};

//  Motor Scale Factor
//  [0] : Motor 1 - Y-Axis - Scale Factor
//  [1] : Motor 2 - Y-Axis - Scale Factor
//  [2] : Motor 3 - X-Axis - Scale Factor
//  [3] : Motor 4 - X-Axis - Scale Factor
const float motor_scale[4] = {1.00f,1.00f,1.00f,1.00f};

//  Motor Upper and Lower Bound Values
const unsigned long motor_upperbound = 10000;        // 100000
const unsigned long motor_lowerbound = 5180;          // 5180
// *************************
//

// Reinforcement Learning Data
// *************************
unsigned long learnSample = 0;                  // Sample Rate
float rho = 0.001;                              // Learning Rate
float epsilon = 16.0f;                          // Epsilon - Acceptable Angle Error Range
float vel_epsilon = 10.0f;                      // Velocity Epsilon - Acceptable Angular Velocity Error Range
float b_epsilon = 10.0f;                        // Epsilon - Balanced
// *************************
//


//*****************************************************************************
// Control
//
// Control function to stabilize the drone
// Refresh Rate: 100Hz
void control(float *imu, float *data_telemetry)
{   
    // Load dt Time Variable to Control
    dt = imu[12];                               // Load IMU dt
    //
    
    // PID Controller for X-Axis
    // *************************
    // Determine Error by difference of command angle and state angle
    x_angle_error = (x_cmd_angle - imu[0]);    
    
    x_Pterm = x_Pgain*x_angle_error;                              // Determine P Term
    x_Iterm = x_Igain*(x_Iterm + (x_angle_error*dt));             // Determine I Term
    x_Dterm = x_Dgain*((x_angle_error - x_old_angle)/dt);         // Determine D Term
    x_angle_vel_term = x_Fgain*imu[3];                            // Determine F Term
    
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
    y_angle_error = (y_cmd_angle - imu[1]);
    
    y_Pterm = y_Pgain*y_angle_error;                              // Determine P Term
    y_Iterm = y_Igain*(y_Iterm + (y_angle_error*dt));             // Determine I Term
    y_Dterm = y_Dgain*((y_angle_error - y_old_angle)/dt);         // Determine D Term
    y_angle_vel_term = y_Fgain*imu[4];                            // Determine F Term
    
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
    // *************************
    //
    
    // Combine Z-Thrust, Roll & Pitch Torque, and Yaw Torque
    // *************************
    motor_dutycycle[2] = (unsigned long)(motor_scale[2]*(z_thrust + x_torque));
    motor_dutycycle[3] = (unsigned long)(motor_scale[3]*(z_thrust - x_torque)); 
    motor_dutycycle[0] = (unsigned long)(motor_scale[0]*(z_thrust + y_torque));
    motor_dutycycle[1] = (unsigned long)(motor_scale[1]*(z_thrust - y_torque));
    // *************************
    
    // Motor control limiting (Sanitize outputs)
    // *************************
    if(motor_dutycycle[0] > motor_upperbound)      // Motor 1 - Y-Axis
    {
      motor_dutycycle[0] = motor_upperbound;
    }
    if(motor_dutycycle[0] < motor_lowerbound)
    {
      motor_dutycycle[0] = motor_lowerbound;
    } 
    
    if(motor_dutycycle[1] > motor_upperbound)      // Motor 2 - Y-Axis
    {
      motor_dutycycle[1] = motor_upperbound;
    }
    if(motor_dutycycle[1] < motor_lowerbound)
    {
      motor_dutycycle[1] = motor_lowerbound;
    }
    
    if(motor_dutycycle[2] > motor_upperbound)      // Motor 3 - X-Axis
    {
      motor_dutycycle[2] = motor_upperbound;
    }
    if(motor_dutycycle[2] < motor_lowerbound)
    {
      motor_dutycycle[2] = motor_lowerbound;
    }
    
    if(motor_dutycycle[3] > motor_upperbound)      // Motor 4 - X-Axis
    {
      motor_dutycycle[3] = motor_upperbound;
    }
    if(motor_dutycycle[3] < motor_lowerbound)
    {
      motor_dutycycle[3] = motor_lowerbound;
    }
    // *************************
    
    
    // Set Motor PWM's
    // *************************
    // Y-Axis Motor Control - Motor 1 & 2
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 100000);   // get this to 20kHz - 81840
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, motor_dutycycle[0]);   // Motor 1 - PWM1 - Pin 35
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, motor_dutycycle[1]);   // Motor 2 - PWM0 - Pin 34
    
    // X-Axis Motor Control - Motor 3 & 4
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 100000);   // get this to 20kHz - 81840
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, motor_dutycycle[2]);   // Motor 3 - PWM7 - Pin 31
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, motor_dutycycle[3]);   // Motor 4 - PWM6 - Pin 30
    // *************************
    
    //
    // *************************
    
    // Send Control Data Telemetry
    // *************************
    data_telemetry[0] = motor_dutycycle[2];
    data_telemetry[1] = x_Pterm;
    data_telemetry[2] = x_Iterm;
    data_telemetry[3] = x_Dterm;
    // *************************
    
    
    // Reinforcement Learning for PID
    // *************************
    /*
    if(learnSample > 5)
    {
      learnSample = 0;
      
      if(fabsf(imu[0]-x_cmd_angle) > b_epsilon)             // If out of Balance
      {  
        if(fabsf(imu[0]-x_cmd_angle) <= epsilon)            // In Epsilon                   
        {
            if(fabsf(imu[3]-x_cmd_angle) <= vel_epsilon)  // Reward Slow Omega +                                    
            {
                x_Pgain = x_Pgain + rho*fabsf(imu[0]-x_cmd_angle);
            }
            else                                             // Punish Fast Omega +++
            {
                x_Pgain = x_Pgain - 0.01; 
            }        
        }
        else                                                  // Out of Epsilon
        {
            if(fabsf(imu[3]-x_cmd_angle) <= vel_epsilon)   // Reward Slow Omega ++                                   
            {
                x_Pgain = x_Pgain + rho*fabsf(imu[0]-x_cmd_angle);
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
    */
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
  
  while(motor_dutycycle[3] < z_thrust) //76530 
  {
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 100000);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, motor_dutycycle[0]);   // Motor 1 - PWM1 - Pin 35
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, motor_dutycycle[1]);   // Motor 2 - PWM0 - Pin 34
           
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 100000);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, motor_dutycycle[2]);   // Motor 3 - PWM7 - Pin 31
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, motor_dutycycle[3]);   // Motor 4 - PWM6 - Pin 30

    for(unsigned long ulLoop = 0; ulLoop < 20000; ulLoop++)       // Delay for motor spinup
    {
       delay = 0;
       delay = delay + 100;
       delay = delay - 100;
    }

    motor_dutycycle[0] += 5;                                  // Increment Motors for spinup
    motor_dutycycle[1] += 5;   

    motor_dutycycle[2] += 5;
    motor_dutycycle[3] += 5;
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

}
//