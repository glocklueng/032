#include "init.h"

#include "inc/hw_adc.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"

#include "../drivers/kitronix320x240x16_ssd2119_8bit.h"
#include "../drivers/set_pinout.h"


//============================================================================//
// 
// Initalize ADC
void InitADC()
{
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);   
    GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4);   //ADC 10        PB4
        
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_6);   //ADC 1         PE6
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_7);   //ADC 0         PE7
    
    ADCSequenceDisable(ADC_BASE, 0);
    ADCSequenceConfigure(ADC_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC_BASE, 0 , 0 , ADC_CTL_CH0);   // PE7
    ADCSequenceStepConfigure(ADC_BASE, 0 , 1 , ADC_CTL_CH1);   // PE6
    ADCSequenceStepConfigure(ADC_BASE, 0 , 2 , ADC_CTL_CH10 | ADC_CTL_IE | ADC_CTL_END);  //PB4
    ADCSequenceEnable(ADC_BASE, 0);
    ADCIntEnable(ADC_BASE, 0);
    ADCIntClear(ADC_BASE, 0);
}

void InitPWM()
{
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);             // Set PWM Clock
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    // Enable GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);      // Enable PWM
    
    // Set current strength on GPIO pin
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    
    GPIOPinConfigure(GPIO_PF3_PWM3);                // Select PWM 3 as the synthesizer
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);    // Select which pin gets the PWM
    
    // Configure the PWM
    PWMGenConfigure(PWM_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    
    
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, 7812);         // Set PWM Timer
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_3, 7812 / 4);    // Set PWM Duty Cycle
    
    
    PWMOutputState(PWM_BASE, PWM_OUT_3_BIT, true);      // Set the PWM pin to an output
    
    PWMGenEnable(PWM_BASE, PWM_GEN_1);                  // Enable PWM Generator
  
}

void InitI2C()
{
     // Initialize the I2C channel the sensor is connected to
    SysCtlPeripheralEnable( SYSCTL_PERIPH_I2C0 );
    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOB );
    GPIOPinConfigure( GPIO_PB2_I2C0SCL );
    GPIOPinConfigure( GPIO_PB3_I2C0SDA );
    GPIOPinTypeI2C( GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3 );

    // Set the clock (false = "slow" = 100kbps)
    I2CMasterInitExpClk( I2C0_MASTER_BASE, SysCtlClockGet(), false );
    I2CMasterEnable( I2C0_MASTER_BASE );
  
}