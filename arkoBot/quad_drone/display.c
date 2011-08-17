#include "display.h"

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
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"

#include "../drivers/kitronix320x240x16_ssd2119_8bit.h"
#include "../drivers/set_pinout.h"
//*****************************************************************************
//
// Graphics context used to show text on the CSTN display.
//
//*****************************************************************************
tContext g_sContext;

//============================================================================//
// 
// Initalize ADC
void InitDisplay()
{
    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);


    

}
    

//============================================================================//
void UpdateDisplay()
{
    //
    // Set the device pinout appropriately for this board.
    //
    PinoutSet();
    
    //
    // Initialize the display driver.
    //
    Kitronix320x240x16_SSD2119Init();
    
    //
    // Initialize the graphics context and find the middle X coordinate.
    //
    GrContextInit(&g_sContext, &g_sKitronix320x240x16_SSD2119);
    
    tRectangle sRect;

    //
    // Fill the top 15 rows of the screen with blue to create the banner.
    //
    sRect.sXMin = 0;
    sRect.sYMin = 0;
    sRect.sXMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.sYMax = 23;
    GrContextForegroundSet(&g_sContext, ClrDarkBlue);
    GrRectFill(&g_sContext, &sRect);

    //
    // Put a white box around the banner.
    //
    GrContextForegroundSet(&g_sContext, ClrWhite);
    GrRectDraw(&g_sContext, &sRect);

    //
    // Put the application name in the middle of the banner.
    //
    GrContextFontSet(&g_sContext, &g_sFontCm20);
    GrStringDrawCentered(&g_sContext, "NSL UAV - Simulation Mode", -1,
                         GrContextDpyWidthGet(&g_sContext) / 2, 11, 0);

    //
    // Initialize the CSTN display and write status.
    //
    GrContextFontSet(&g_sContext, &g_sFontCmss22b);
    GrStringDraw(&g_sContext, "X-Axis Acc:",        -1,  70, 40, 0);
    GrStringDraw(&g_sContext, "Y-Axis Acc:",        -1,  70, 65, 0);
    GrStringDraw(&g_sContext, "Z-Axis Acc:",        -1,  70, 90, 0);
    GrStringDraw(&g_sContext, "X-Axis Gyro:",       -1,  70, 115, 0);
    GrStringDraw(&g_sContext, "Y-Axis Gyro:",       -1,  70, 140, 0);
    GrStringDraw(&g_sContext, "Z-Axis Gyro:",       -1,  70, 165, 0);
    GrStringDraw(&g_sContext, "No Data",       -1, 220, 40, 0);
    GrStringDraw(&g_sContext, "No Data",       -1, 220, 65, 0);
    GrStringDraw(&g_sContext, "No Data",       -1, 220, 90, 0);
    GrStringDraw(&g_sContext, "No Data",       -1, 220, 115, 0);
    GrStringDraw(&g_sContext, "No Data",       -1, 220, 140, 0);
    GrStringDraw(&g_sContext, "No Data",       -1, 220, 165, 0);
    /*
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
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    //
    // Prompt for text to be entered.
    //
    UARTSend((unsigned char *)"Enter text: ", 12);
    */
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

//============================================================================//