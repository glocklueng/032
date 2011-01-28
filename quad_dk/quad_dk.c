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
    
    
    volatile unsigned long ulLoop;

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    //SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    ulLoop = SYSCTL_RCGC2_R;

    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    //GPIO_PORTF_DIR_R = 0x08;
    //GPIO_PORTF_DEN_R = 0x08;
    // --------------------------
                     
    
    unsigned long adc_values[3];
    unsigned long blink_delay = 0;
     
    // -----------------------------------------   
    
    
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    
    GPIOPinConfigure(GPIO_PF3_PWM3);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
    
    PWMGenConfigure(PWM_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, 7812);
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_3, 7812 / 4);
    
    
    PWMOutputState(PWM_BASE, PWM_OUT_3_BIT, true);
    
    PWMGenEnable(PWM_BASE, PWM_GEN_1);
    
    
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
        
        
        blink_delay = adc_values[2] * 20;
        
        if(blink_delay != 0 && blink_delay < 20460)
        {
          PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, 20460);
          PWMPulseWidthSet(PWM_BASE, PWM_OUT_3, blink_delay);
        }
        
        //                                          
        // Turn on the LED.
        //
        //GPIO_PORTF_DATA_R |= 0x08;

        //
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < blink_delay; ulLoop++)
        {
        }

        //UpdateDisplay(); 
        
        //
        // Turn off the LED.
        //
        //GPIO_PORTF_DATA_R &= ~(0x08);

        //
        // Delay for a bit.
        //
        //for(ulLoop = 0; ulLoop < blink_delay; ulLoop++)
        //{
        //}
        
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
