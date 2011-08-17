#include "inc/lm3s9b96.h"
#include "math.h"
#include "IQmath/IQmathLib.h"

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

float x_Pgain = 3.5f;     // X-Axis - P Gain - 8.5
float x_Igain = 10.0f;    // X-Axis - I Gain - 0.01
float x_Dgain = 0.030f;     // X-Axis - D Gain - 85.0
const float x_Fgain = 0.0f;     // X-Axis - F Gain - 0.0

float x_Dterm_1 = 0.0f;		// X-Axis - D Term Filter Interation 1 
float x_Dterm_2 = 0.0f;		// X-Axis - D Term Filter Interation 2
float x_Dterm_3 = 0.0f;		// X-Axis - D Term Filter Interation 3

float x_angle_error = 0.0f;     // X Angle Error
float x_cmd_angle = 0.0f;       // X Angle Commanded
float x_old_angle = 0.0f;       // X Angle Old Reading

const float x_ItermMax = 100.00f;  // Maximum value for x_Iterm
const float x_ItermMin = -100.00f; // Minimum value for x_Iterm

float x_torque = 0.0f;          // X Axis - Rotational Torque
// *************************
//

// Y-Axis PID Data
// *************************
float y_Pterm = 0.0f;           // Y-Axis - P Term
float y_Iterm = 0.0f;           // Y-Axis - I Term
float y_Dterm = 0.0f;           // Y-Axis - D Term 
float y_angle_vel_term = 0.0f;  // Y-Axis - Angular Velocity Term

float y_Pgain = 3.5f;     // Y-Axis - P Gain - 8.5
float y_Igain = 10.0f;     // Y-Axis - I Gain - 0.01
float y_Dgain = 0.030f;      // Y-Axis - D Gain - 85.0
const float y_Fgain = 0.0f;     // Y-Axis - F Gain - 0.0

float y_Dterm_1 = 0.0f;		// Y-Axis - D Term Filter Interation 1 
float y_Dterm_2 = 0.0f;		// Y-Axis - D Term Filter Interation 2
float y_Dterm_3 = 0.0f;		// Y-Axis - D Term Filter Interation 3


float y_angle_error = 0.0f;     // Y Angle Error
float y_cmd_angle = 0.0f;       // Y Angle Commanded
float y_old_angle = 0.0f;       // Y Angle Old Reading

const float y_ItermMax = 100.00f;  // Maximum value for y_Iterm
const float y_ItermMin = -100.00f; // Minimum value for y_Iterm

float y_torque = 0.0f;          // Y Axis - Rotational Torque
// *************************
//

// Z-Axis PID Data
// *************************
float z_Pterm = 0.0f;           // Z-Axis - P Term
float z_Iterm = 0.0f;           // Z-Axis - I Term
float z_Dterm = 0.0f;           // Z-Axis - D Term 
float z_angle_vel_term = 0.0f;  // Z-Axis - Angular Velocity Term

const float z_Pgain = 0.0f;     // Z-Axis - P Gain - 8.5
const float z_Igain = 0.0f;     // Z-Axis - I Gain - 0.01
const float z_Dgain = 0.0f;     // Z-Axis - D Gain - 85.0
const float z_Fgain = 0.0f;     // Z-Axis - F Gain - 0.0


float z_Dterm_1 = 0.0f;		// Z-Axis - D Term Filter Interation 1 
float z_Dterm_2 = 0.0f;		// Z-Axis - D Term Filter Interation 2
float z_Dterm_3 = 0.0f;		// Z-Axis - D Term Filter Interation 3


float z_angle_error = 0.0f;     // Z Angle Error
float z_cmd_angle = 0.0f;       // Z Angle Commanded
float z_old_angle = 0.0f;       // Z Angle Old Reading

const float z_ItermMax = 100.00f;  // Maximum value for z_Iterm
const float z_ItermMin = -100.00f; // Minimum value for z_Iterm

float z_torque = 0.0f;          // Z Axis - Rotational Torque
// *************************
//

// Z-Axis Data
// *************************
float z_thrust = 5650.0f;       // Z Axis Thrust - Idle Thrust = 5500.0
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
const unsigned long motor_upperbound = 5900;          // 5750
const unsigned long motor_lowerbound = 5300;          // 5250
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
void control(float *imu, float *cmd_angles, float *data_telemetry)
{   
    // Load dt Time Variable to Control
    dt = imu[12];                               // Load IMU dt
    //
    
    // PID Controller for X-Axis
    // *************************
    // Determine Error by difference of command angle and state angle
    x_angle_error = (cmd_angles[0] - imu[0]);    
    
    x_Pterm =  x_Pgain*x_angle_error;                              // Determine P Term
    x_Iterm += x_Igain*(x_angle_error*dt);                         // Determine I Term
    x_Dterm =  x_Dgain*((imu[0] - x_old_angle)/dt);                // Determine D Term
    x_angle_vel_term = x_Fgain*imu[3];                             // Determine F Term
    
    // Set current angle as old angle for next loop
    x_old_angle = imu[0];                
 
    // Filter D Term - Prevent noise near stable zones
    x_Dterm = (x_Dterm_3 + (2 * x_Dterm_2) + (2 * x_Dterm_1) + x_Dterm)/6.0;
    x_Dterm_1 = x_Dterm;
    x_Dterm_2 = x_Dterm_1;
    x_Dterm_3 = x_Dterm_2;
    
    // Limit the x_Iterm from integrating out too far
    if(x_Iterm > x_ItermMax)
    {  
      x_Iterm = x_ItermMax;
    }
    else if(x_Iterm < x_ItermMin) 
    {
      x_Iterm = x_ItermMin;
    }
    
    // Add up the PID Terms + F Term to determine X Torque
    x_torque = x_Pterm + x_Iterm - x_Dterm + x_angle_vel_term;
    // *************************
    
    
    // PID Controller for Y-Axis  
    // *************************
    // Determine Error by difference of command angle and state angle
    y_angle_error = (cmd_angles[1] - imu[1]);
    
    y_Pterm =  y_Pgain*y_angle_error;                              // Determine P Term
    y_Iterm += y_Igain*(y_angle_error*dt);                         // Determine I Term
    y_Dterm =  y_Dgain*((imu[1] - y_old_angle)/dt);                // Determine D Term
    y_angle_vel_term = y_Fgain*imu[4];                             // Determine F Term
    
    // Set current angle as old angle for next loop
    y_old_angle = imu[1]; 
    
    // Filter D Term - Prevent noise near stable zones
    y_Dterm = (y_Dterm_3 + (2 * y_Dterm_2) + (2 * y_Dterm_1) + y_Dterm)/6.0;
    y_Dterm_1 = y_Dterm;
    y_Dterm_2 = y_Dterm_1;
    y_Dterm_3 = y_Dterm_2;
    
    // Limit the y_Iterm from integrating out too far
    if(y_Iterm > y_ItermMax)
    {  
      y_Iterm = y_ItermMax;
    }
    else if(y_Iterm < y_ItermMin) 
    {
      y_Iterm = y_ItermMin;
    }
       
    // Add up the PID Terms + F Term to determine Y Torque
    y_torque = y_Pterm + y_Iterm - y_Dterm + y_angle_vel_term;
    // *************************
    
    
    // PID Controller for Z-Axis (yaw)
    // *************************
    // Determine Error by difference of command angle and state angle
    z_angle_error = (cmd_angles[2] - imu[2]);
    
    z_Pterm =  z_Pgain*z_angle_error;                              // Determine P Term
    z_Iterm += z_Igain*(z_angle_error*dt);                         // Determine I Term
    z_Dterm =  z_Dgain*((imu[2] - z_old_angle)/dt);         // Determine D Term
    z_angle_vel_term = z_Fgain*imu[4];                             // Determine F Term
    
    // Set current angle as old angle for next loop
    z_old_angle = imu[2]; 
    
    // Filter D Term - Prevent noise near stable zones
    z_Dterm = (z_Dterm_3 + (2 * z_Dterm_2) + (2 * z_Dterm_1) + z_Dterm)/6.0;
    z_Dterm_1 = z_Dterm;
    z_Dterm_2 = z_Dterm_1;
    z_Dterm_3 = z_Dterm_2;
    
    // Limit the z_Iterm from integrating out too far
    if(z_Iterm > z_ItermMax)
    {  
      z_Iterm = z_ItermMax;
    }
    else if(z_Iterm < z_ItermMin) 
    {
      z_Iterm = z_ItermMin;
    }
    
    // Add up the PID Terms + F Term to determine Y Torque
    z_torque = z_Pterm + z_Iterm - z_Dterm + z_angle_vel_term;
    // *************************
    
    // Controller for Z-Altitude (Altitude)
    // *************************
    // Determine Error by difference of command angle and state angle
    z_thrust += cmd_angles[3];
    //
    
    // Motor Control
    // *************************
    //
    
    // Combine Z-Thrust, Roll & Pitch Torque, and Yaw Torque
    // *************************
    motor_dutycycle[2] = (unsigned long)(motor_scale[2]*(z_thrust + x_torque + z_torque));
    motor_dutycycle[3] = (unsigned long)(motor_scale[3]*(z_thrust - x_torque + z_torque)); 
    motor_dutycycle[0] = (unsigned long)(motor_scale[0]*(z_thrust + y_torque - z_torque));
    motor_dutycycle[1] = (unsigned long)(motor_scale[1]*(z_thrust - y_torque - z_torque));
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
    data_telemetry[0] = motor_dutycycle[0];
    data_telemetry[1] = y_Pgain*10000;
    data_telemetry[2] = y_Igain*10000;
    data_telemetry[3] = y_Dgain*1000000;
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
  if(cmd == '1')
  {
     y_Pgain += 0.1f;     // Increase P Gain
  }
  else if(cmd == '2')
  {
     y_Pgain -= 0.1f;     // Decrease P Gain
  }                                             
  else if(cmd == '3')
  {
     y_Igain += 0.1f;   // Increase I Gain
  }
  else if(cmd == '4')
  {
     y_Igain -= 0.1f;   // Decrease I Gain
  }
  else if(cmd == '5')
  {
     y_Dgain += 0.0025f;     // Increase D Gain
  }
  else if(cmd == '6')
  {
     y_Dgain -= 0.0025f;     // Decrease D Gain
  }
  else if(cmd == 'p')
  {
     sendControlTelemetry(y_torque, y_Pgain*10000, y_Igain*10000, y_Dgain*1000000);       // Send back current PID state
  }
}
//