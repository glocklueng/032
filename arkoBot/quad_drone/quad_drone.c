/*  

Quadrotor Firmware 
By: Arko

Quadrotor UAV Project - Nullspace Labs
http://wiki.032.la/nsl/Quadrotor

Processor: TI LM3S9B96
Archecture: ARM Cortex M3

*/

#include "inc/lm3s9b96.h"
#include "math.h"
#include "IQmath/IQmathLib.h"

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

#include "../drivers/set_pinout.h"

#include "init.h"
#include "imu.h"
#include "control.h"
#include "xbee.h"


// Command Angle
// [0]  : Command Angle X - Translational X (deg)
// [1]  : Command Angle Y - Translational Y (deg)
// [2]  : Command Angle Z - Yaw             (deg)
// [3]  : Command Altitude Z - Altitude     (cm)
//
float cmd_angles[4] = {0.0f,0.0f,0.0f,0.0f};
//

// IMU Values
// [0]  : X Angle - roll   (deg)
// [1]  : Y Angle - pitch  (deg)
// [2]  : Z Angle - yaw    (deg)
// [3]  : X Rate  - roll angular speed  (deg/sec)
// [4]  : Y Rate  - pitch angular speed (deg/sec)
// [5]  : Z Rate  - yaw angular speed   (deg/sec)    
// [6]  : X Acceleration
// [7]  : Y Acceleration
// [8]  : Z Acceleration
// [9]  : X Compass Gauss
// [10] : Y Compass Gauss
// [11] : Z Compass Gauss
// [12] : dt
// [13] : Temperature
//
float imu[14] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
                 0.0f,0.0f,0.0f,0.0f,0.0f,0.05f,94.0f};
//

// dt - Measurement of time per loop
float dt = 0.05f;
//

//
// Control Telemetry Data
float control_telemetry[4];
//
unsigned char toggle = 0x00;

//***************************************************************************** 
// 
// Main Loop 
// 
//***************************************************************************** 
int main(void)
{
    // INIT - Initialization Code 
    // **************************** 
  
    // Set system clock to 20Mhz
    //SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL 
    //              | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  
    // Set system clock to 50Mhz
    //SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL 
    //              | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  
    // Set system clock to 80Mhz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL 
                  | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    
    // Initialize System Peripherals
    InitADC();
    InitPWM();         
    InitI2C();
    InitUART();
    
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
    // ****************************
  
    
    GPIO_PORTF_DATA_R |= 0x08;        // Turn red LED on (Calibrating Light)
    imuStartup();                     // Start up the IMU by calibrating
    GPIO_PORTF_DATA_R &= ~(0x08);     // Turn red LED off (Calibrating Light)
    
    GPIO_PORTF_DATA_R |= 0x02;        // Turn blue LED on (Motor Spinup Light)
    motorSpinup();                    // Start up the motors
    GPIO_PORTF_DATA_R &= ~(0x02);     // Turn blue LED off (Motor Spinup Light)
    
    // Initialize Timers and Start
    InitTIMER();
    
    // MAIN LOOP
    // ****************************
    while(1)
    {   
        GPIO_PORTF_DATA_R |= 0x04;    // Turn green LED on (Flight Mode)

        // IMU
        // ****************************
        readIMU(&imu[0]);             // Read IMU and filter
        // **************************** 
        
        // Send Telemetry
        // ****************************        
        
        // Serial Debug Mode
        // *****************
        //sendDataTelemetry(&imu[0], dt);                                   // Send IMU Data Telemetry
        //sendControlTelemetry(control_telemetry[0], control_telemetry[1], 
        //                     control_telemetry[2], control_telemetry[3]);  // Send Control Telemetry
        // *****************
        
        // MavLink Data Telemetry
        // **********************
        sendMAVLinkData(&imu[0]);                                        // Send IMU Data Telemetry
        // **********************
        
        // ****************************   
    }
    // ****************************
}



//***************************************************************************** 
// 
// The interrupt handler for the Control Loop
// 
//***************************************************************************** 
void 
ControlLoopTimerInt(void) 
{ 
    IntMasterDisable();
    
    // CONTROL LOOP
    // Timer clocked at 100Hz
    // ****************************
    control(&imu[0], &cmd_angles[0], &control_telemetry[0]);  // Control drone by feeding IMU data
    
    if(toggle)
    {
      GPIO_PORTF_DATA_R |= 0x02;        // Turn blue LED on (Motor Spinup Light)
      toggle = 0x00;
    }
    else
    {
      GPIO_PORTF_DATA_R &= ~(0x02);     // Turn blue LED off (Motor Spinup Light)
      toggle = 0x01;
    }
    // ****************************
    
    
    IntMasterEnable();

    //
    // Clear the timer interrupt.
    //
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    
    TimerLoadSet(TIMER1_BASE, TIMER_A, 0x000c3500);         // Load Timer - 100Hz
}

//***************************************************************************** 
// 
// The UART interrupt handler. 
// 
//***************************************************************************** 
void 
UARTIntHandler(void) 
{ 
   IntMasterDisable();
   
   unsigned long ulStatus;

   char readCmd;
   
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
        readCmd = (char)UARTCharGetNonBlocking(UART0_BASE);
        PIDTune(readCmd);
    }
    
    IntMasterEnable(); 
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
