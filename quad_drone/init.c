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
#include "driverlib/uart.h"
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
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);   
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4);   //ADC 3         PE4    X
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_6);   //ADC 1         PE6    Y
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_5);   //ADC 2         PE5    Z
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);   //ADC 8         PE3    Temperature
    
    ADCSequenceDisable(ADC_BASE, 0);
    ADCSequenceConfigure(ADC_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC_BASE, 0 , 0 , ADC_CTL_CH8);   // PE3
    ADCSequenceStepConfigure(ADC_BASE, 0 , 1 , ADC_CTL_CH1);   // PE6
    ADCSequenceStepConfigure(ADC_BASE, 0 , 2 , ADC_CTL_CH2);   // PE5
    ADCSequenceStepConfigure(ADC_BASE, 0 , 3 , ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);  //PE4
    ADCSequenceEnable(ADC_BASE, 0);
    ADCIntEnable(ADC_BASE, 0);
    ADCIntClear(ADC_BASE, 0);
    
    // Add or make sure the voltage reference is set to the internal
    // 3.3v reference.
}


//============================================================================//
// 
// Initalize PWM
void InitPWM()
{
    
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);             // Set PWM Clock
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    // Enable GPIO Port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);      // Enable PWM
    
    // Set current strength on GPIO pin
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    
    GPIOPinConfigure(GPIO_PA6_PWM0);                // Select PWM 0 as the synthesizer
    GPIOPinConfigure(GPIO_PA7_PWM1);                // Select PWM 1 as the synthesizer
    GPIOPinConfigure(GPIO_PA4_PWM6);                // Select PWM 6 as the synthesizer
    GPIOPinConfigure(GPIO_PA5_PWM7);                // Select PWM 7 as the synthesizer
    
    GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_4);    // Select which pin gets the PWM
    GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_5);    // Select which pin gets the PWM
    GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6);    // Select which pin gets the PWM
    GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_7);    // Select which pin gets the PWM
    
    // Configure the PWM
    PWMGenConfigure(PWM_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 7812);         // Set PWM Gen 0 Timer
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, 6000);        // Set PWM0 Duty Cycle
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 6000);        // Set PWM1 Duty Cycle
    
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_3, 7812);         // Set PWM Gen 3 Timer
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_6, 6000);        // Set PWM6 Duty Cycle
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_7, 6000);        // Set PWM7 Duty Cycle
    
    PWMOutputState(PWM_BASE, PWM_OUT_0_BIT, true);      // Set the PWM0 pin to an output
    PWMOutputState(PWM_BASE, PWM_OUT_1_BIT, true);      // Set the PWM1 pin to an output
    PWMOutputState(PWM_BASE, PWM_OUT_6_BIT, true);      // Set the PWM6 pin to an output
    PWMOutputState(PWM_BASE, PWM_OUT_7_BIT, true);      // Set the PWM7 pin to an output
    
    
    PWMGenEnable(PWM_BASE, PWM_GEN_0);                  // Enable PWM Generator 0
    PWMGenEnable(PWM_BASE, PWM_GEN_3);                  // Enable PWM Generator 3
    
}


//============================================================================//
// 
// Initalize I2C
void InitI2C()
{ 
    // TRY FAST MODE AT 400Kpbs!!!! (ONLY ON GYRO)
  
    // Initialize the I2C - Channel 0 - Compass
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3 );

    // Set the clock 100kbps
    I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);
    I2CMasterEnable(I2C0_MASTER_BASE); 
    
    #define I2C_SEND false
    #define I2C_RECV true
                             
  
    #define COMP_ADDRESS 0x1e         // Compass Address         00011110b  
    #define COMP_READ_ADDRESS 0x3d    // Compass Read Address    00111101b
    #define COMP_WRITE_ADDRESS 0x3c   // Compass Write Address   00111100b 
    #define COMP_MODE_REG 0x02        // Compass Mode Register   00000010b 
    
    I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, COMP_ADDRESS, I2C_SEND);        // Set I2C to send  (WRITE)
    I2CMasterDataPut(I2C0_MASTER_BASE, COMP_MODE_REG);                      // Setup to start writing Mode Register 
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);    // Start Sending      

    while(I2CMasterBusy(I2C0_MASTER_BASE)){}                                // Wait for SAK
    
    I2CMasterDataPut(I2C0_MASTER_BASE, 0x00);                               // Clear Bit 1 - MD1 Bit  
    I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);   // Start Sending      

    while(I2CMasterBusy(I2C0_MASTER_BASE)){}                                // Wait for SAK
    
    
    // Initialize the I2C - Channel 1 - Gyro
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    GPIOPinConfigure(GPIO_PG0_I2C1SCL);
    GPIOPinConfigure(GPIO_PG1_I2C1SDA);
    GPIOPinTypeI2C(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1 );

    // Set the clock 100kbps
    I2CMasterInitExpClk(I2C1_MASTER_BASE, SysCtlClockGet(), false);
    I2CMasterEnable(I2C1_MASTER_BASE); 
}


//============================================================================//
// 
// Initalize UART
void InitUART()
{  
    //
    // Enable the (non-GPIO) peripherals used by this example.  PinoutSet()
    // already enabled GPIO Port A.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 9600, 8-N-1 operation.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}