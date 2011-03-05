#include "inc/lm3s9b96.h"
#include "math.h"

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


#include "../drivers/kitronix320x240x16_ssd2119_8bit.h"
#include "../drivers/set_pinout.h"
#include "init.h"
#include "imu.h"
#include "control.h"
#include "display.h"

 /*  

Quadrotor Firmware 
By: Arko

Quadrotor UAV Project - Nullspace Labs
http://wiki.032.la/nsl/Quadrotor

Processor: TI LM3S9B96
Archecture: ARM Cortex M3

*/

//***************************************************************************** 
// 
// Main Loop 
// 
//***************************************************************************** 
int main(void)
{

    
    // INIT - Initialization Code 
    // -------------------------- 
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL 
                  | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    InitADC();
    InitPWM();         
    InitI2C();
    InitUART();
    //
    // --------------------------
    
    
    // Cylon Init
    // --------------------------
    // Some value that will eventually go away..
    //unsigned long blink_delay = 200000;
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
    
    // IMU Values
    // [0] : X Angle
    // [1] : Y Angle
    // [2] : Z Angle
    // [3] : X Rate
    // [4] : Y Rate
    // [5] : Z Rate
    // [6] : Magnetic North Heading
    // [7] : Temperature
    float imu[8];
    
    GPIO_PORTF_DATA_R |= 0x08;        // Turn red LED on (Calibrating Light)
    imuStartup();                     // Start up the IMU by calibrating
    GPIO_PORTF_DATA_R &= ~(0x08);     // Turn red LED off (Calibrating Light)
    
    GPIO_PORTF_DATA_R |= 0x02;        // Turn blue LED on (Motor Spinup Light)
    motorSpinup();                    // Start up the motors
    GPIO_PORTF_DATA_R &= ~(0x02);     // Turn blue LED off (Motor Spinup Light)
    
    // MAIN LOOP
    // --------------------------
    while(1)
    {   
        GPIO_PORTF_DATA_R |= 0x04;
        readIMU(&imu[0]);

        Control(imu[1],imu[4]);
          
        // Cylon Mode
        /*
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
        */
    
        //UARTSend((unsigned char *)"Enter text: ", 12);
    }
    // --------------------------
}


//***************************************************************************** 
// 
// The UART interrupt handler. 
// 
//***************************************************************************** 
void 
UARTIntHandler(void) 
{ 
   unsigned long ulStatus;

    //
    // Get the interrrupt status.
    //
    ulStatus = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART0_BASE, ulStatus);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
        UARTCharPutNonBlocking(UART0_BASE,
                                   UARTCharGetNonBlocking(UART0_BASE));
    }
} 
 
//***************************************************************************** 
// 
// Send a string to the UART. 
// 
//***************************************************************************** 
void 
UARTSend(const unsigned char *pucBuffer, unsigned long ulCount) 
{ 
    //
    // Loop while there are more characters to send.
    //
    while(ulCount--)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
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
