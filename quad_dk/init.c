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
