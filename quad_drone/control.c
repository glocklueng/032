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

float x_Pterm;
float x_Iterm;
float x_Dterm;
float x_angle_vel_term;

const float x_Pgain = 12.0f;         // Retune
const float x_Igain = 0.00001f;
const float x_Dgain = 3.0f;   
const float x_Fgain = 1.0f;

float x_angle_error = 0;
float x_cmd_angle = 0;
float x_old_angle = 0;

float x_torque = 0;

unsigned long motor_dutycycle_l = 64000; 
unsigned long motor_dutycycle_r = 64000; 

void Control(float x_angle, float x_ang_vel)
{ 
    x_angle_error = (x_cmd_angle - x_angle);
    
    x_Pterm = x_Pgain*x_angle_error;
    x_Iterm = ((x_Igain*x_angle_error) + x_Iterm) * 0.0015;
    x_Dterm = x_Dgain*(x_angle_error - x_old_angle);
    
    x_old_angle = x_angle_error;
    
    x_angle_vel_term = x_Fgain*x_ang_vel;  
    
    x_torque = x_Pterm + x_Iterm + x_Dterm + x_angle_vel_term;
    
    
    motor_dutycycle_l = (unsigned long)((float)77500 + x_torque);
    motor_dutycycle_r = (unsigned long)((float)77500 - x_torque);
    
    if(motor_dutycycle_l > 80000)
    {
      motor_dutycycle_l = 80000;
    }
    
    if(motor_dutycycle_l < 75000)
    {
      motor_dutycycle_l = 75000;
    }
    
    
    if(motor_dutycycle_r > 80000)
    {
      motor_dutycycle_r = 80000;
    }
    if(motor_dutycycle_r < 75000)
    {
      motor_dutycycle_r = 75000;
    }
    
    //   UNCOMMENT TO ENABLE MOTORS
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 81840);   // get this to 20kHz
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, motor_dutycycle_l);   // Motor 3 - PWM7 - Pin 31
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, motor_dutycycle_r);   // Motor 4 - PWM6 - Pin 30
    //
    
    ///PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 20460);   // get this to 20kHz
    ///PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, motor_dutycycle_l);   // Motor 3 - PWM7 - Pin 31
    
    
    sendControlTelemetry(x_torque);
}



void motorSpinup()
{
  volatile unsigned long delay;
  
  while(motor_dutycycle_l < 77500)   
  {
    if(motor_dutycycle_l < 80000 && motor_dutycycle_r < 80000)
    {
      //PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 20460);   // get this to 20kHz
      //PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, motor_dutycycle);   // Motor 1 - PWM1 - Pin 35
      //PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, motor_dutycycle);   // Motor 2 - PWM0 - Pin 34
      
      //   UNCOMMENT TO ENABLE MOTORS
      PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 81840);   // get this to 20kHz
      PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, motor_dutycycle_l);   // Motor 3 - PWM7 - Pin 31
      PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, motor_dutycycle_r);   // Motor 4 - PWM6 - Pin 30
      //
    }
    
    
    for(unsigned long ulLoop = 0; ulLoop < 20000; ulLoop++)
    {
       delay = 0;
       delay = delay + 100;
       delay = delay - 100;
    }

    
    motor_dutycycle_l += 40;
    motor_dutycycle_r += 40;
  }
  
}