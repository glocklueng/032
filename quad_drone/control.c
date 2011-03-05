#include "inc/lm3s9b96.h"
#include "math.h"

#include "control.h"

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

float y_Pterm;
float y_Iterm;
float y_Dterm;
float y_angle_vel_term;

const float y_Pgain = 120.0f;
const float y_Igain = 0.001f;
const float y_Dgain = 120.0f;   
const float y_Fgain = 60.0f;

float y_angle_error = 0;
float y_cmd_angle = 0;
float y_old_angle = 0;

float y_torque = 0;

unsigned long motor_dutycycle_l = 16000; 
unsigned long motor_dutycycle_r = 16000; 

void Control(float y_angle, float y_ang_vel)
{ 
    y_angle_error = (y_cmd_angle - y_angle);
    
    y_Pterm = y_Pgain*y_angle_error;
    y_Iterm = ((y_Igain*y_angle_error) + y_Iterm) * 0.9999;
    y_Dterm = y_Dgain*(y_angle_error - y_old_angle);
    
    y_old_angle = y_angle_error;
    
    y_angle_vel_term = y_Fgain*y_ang_vel;  
    
    y_torque = y_Pterm + y_Iterm + y_Dterm + y_angle_vel_term;
    
    
    motor_dutycycle_l = (unsigned long)((float)motor_dutycycle_l + y_torque);
    //motor_dutycycle_r = (unsigned long)((float)motor_dutycycle_r + y_torque);
    
    if(motor_dutycycle_l > 20000)
    {
      motor_dutycycle_l = 20000;
    }
    
    if(motor_dutycycle_l < 18750)
    {
      motor_dutycycle_l = 18750;
    }
    
    /*
    if(motor_dutycycle_r > 20000)
    {
      motor_dutycycle_r = 20000;
    }
    if(motor_dutycycle_r < 18750)
    {
      motor_dutycycle_r = 18750;
    }
    */
    
    
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 20460);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, motor_dutycycle_l);   // Motor 3 - PWM7 - Pin 31
    
    //PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, motor_dutycycle_r);   // Motor 4 - PWM6 - Pin 30
    
}



void motorSpinup()
{
  while(motor_dutycycle_l < 19375)
  {
    if(motor_dutycycle_l < 20000 && motor_dutycycle_r < 20000)
    {
      //PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 20460);   // get this to 20kHz
      //PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, motor_dutycycle);   // Motor 1 - PWM1 - Pin 35
      //PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, motor_dutycycle);   // Motor 2 - PWM0 - Pin 34
      
      PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 20460);   // get this to 20kHz
      PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, motor_dutycycle_l);   // Motor 3 - PWM7 - Pin 31
      PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, motor_dutycycle_r);   // Motor 4 - PWM6 - Pin 30
    }
    
    for(unsigned long ulLoop = 0; ulLoop < 20000; ulLoop++)
    {
    }
  
    motor_dutycycle_l += 10;
    motor_dutycycle_r += 10;
  } 
}