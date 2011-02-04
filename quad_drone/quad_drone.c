#include "inc/lm3s9b96.h"

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
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"


#include "../drivers/kitronix320x240x16_ssd2119_8bit.h"
#include "../drivers/set_pinout.h"
#include "init.h"
#include "imu.h"
#include "display.h"

 /*  

Quadrotor Firmware 
By: Arko

Quadrotor UAV Project - Nullspace Labs
http://wiki.032.la/nsl/Quadrotor

Processor: TI LM3S9B96
Archecture: ARM Cortex M3

*/

int main(void)
{

    
    // INIT - Initialization Code 
    // -------------------------- 
    InitADC();
    InitPWM();         // Make sure to comment out the LED Code
    //InitI2C();
    
    volatile unsigned long ulLoop;

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    ulLoop = SYSCTL_RCGC2_R;

    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIO_PORTF_DIR_R = 0x0e;
    GPIO_PORTF_DEN_R = 0x0e;
    // --------------------------
                     
    // ADC Value Matrix:
    // [0] : Temperature
    // [1] : Y - Axis Accelerometer
    // [2] : Z - Axis Accelerometer
    // [3] : X - Axis Accelerometer
    unsigned long adc_values[4];
    
    // Some value that will eventually go away..
    unsigned long blink_delay = 0;
    unsigned long motor_dutycycle = 0; 
    // -----------------------------------------   
    
    

    
    
    // MAIN LOOP           
    while(1)
    {

        // Trigger the sample sequence.
        ADCProcessorTrigger(ADC_BASE, 0);

        // Wait until the sample sequence has completed.
        while(!ADCIntStatus(ADC_BASE, 0, false)){}
        
        // Grab ADC values and load to buffer
        ADCSequenceDataGet(ADC_BASE,0,adc_values);
        
        // Clear ADC sequencer
        ADCIntClear(ADC_BASE, 0);
        
        // Read ADC and write accordingly
        blink_delay = adc_values[1] * 300;
        motor_dutycycle = adc_values[1] * 20;
        
        
        // 20460
        if(motor_dutycycle < 20460)
        {
          PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 20460);   // get this to 20kHz
          PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, motor_dutycycle);   // Motor 1 - PWM1 - Pin 35
          PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, motor_dutycycle);   // Motor 2 - PWM0 - Pin 34
          
          PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 20460);   // get this to 20kHz
          PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, motor_dutycycle);   // Motor 3 - PWM7 - Pin 31
          PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, motor_dutycycle);   // Motor 4 - PWM6 - Pin 30
        }
        
        
        
        // Cylon Mode
        GPIO_PORTF_DATA_R |= 0x04;
        for(ulLoop = 0; ulLoop < blink_delay; ulLoop++)
        {
        }
        GPIO_PORTF_DATA_R &= ~(0x04);
        GPIO_PORTF_DATA_R |= 0x08;
        for(ulLoop = 0; ulLoop < blink_delay; ulLoop++)
        {
        }
        GPIO_PORTF_DATA_R &= ~(0x08);
        GPIO_PORTF_DATA_R |= 0x02; 
        for(ulLoop = 0; ulLoop < blink_delay; ulLoop++)
        {
        }
        GPIO_PORTF_DATA_R &= ~(0x02);
        GPIO_PORTF_DATA_R |= 0x08;        
        for(ulLoop = 0; ulLoop < blink_delay; ulLoop++)
        {
        }
        GPIO_PORTF_DATA_R &= ~(0x08);
        
        
        
    }
}
 
        
//*****************************************************************************
//
//                              DEBUG
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif
