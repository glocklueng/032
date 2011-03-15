#include "inc/lm3s9b96.h"
#include "math.h"

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

#include <stdio.h>

// UART Variables
unsigned long uartDelay = 0;
unsigned long controlDelay = 0;

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
        while(UARTBusy(UART0_BASE));
    }
} 

//*****************************************************************************
void sendDataTelemetry(float *imu, float dt)
{
    if(uartDelay > 10)
    {
      uartDelay = 0;
      for(int i=0; i < 7; i++)
      {
        char sendBuf[32];
        unsigned long n = 0;
        volatile unsigned long delay = 0;
        
        if(i <= 5)
        {
          int imumeasure = (int)imu[i];
          if(imumeasure < 0)
          {
            imumeasure = -1*imumeasure;
            n = sprintf(sendBuf, "-%i", imumeasure);
          }
          else
          {
            n = sprintf(sendBuf, "%i", imumeasure);
          }
          
          switch(i)
          {
          case 0: 
            UARTSend((unsigned char*)"X-Angle: ", 9); 
            break;
          case 1:
            UARTSend((unsigned char*)"Y-Angle: ", 9);
            break;
          case 2:
            UARTSend((unsigned char*)"Z-Angle: ", 9);
            break;
          case 3:
            UARTSend((unsigned char*)"X-Omega: ", 9);
            break;
          case 4:
            UARTSend((unsigned char*)"Y-Omega: ", 9);
            break;
          case 5:
            UARTSend((unsigned char*)"Z-Omega: ", 9);
            break;
          default:
            UARTSend((unsigned char*)"ERROR", 5);
            break;
          }
        }
        else
        {
          int freq = (int)(1/dt);
          n = sprintf(sendBuf, "%i Hz", (int)freq);
          
          UARTSend((unsigned char*)"dt: ", 4);
        }

        UARTSend((unsigned char*)sendBuf, n);
        UARTSend((unsigned char*)"  ", 1);
      }                                   
      //UARTSend((unsigned char*)"\n", 1);
    }
    else
    {
      uartDelay++; 
    }
  
}

//*****************************************************************************
void sendControlTelemetry(float torque)
{
    if(controlDelay > 10)
    {
        controlDelay = 0;
      
        char sendBuf[32];
        unsigned long n = 0;
        volatile unsigned long delay = 0;
        
        int torq;
        if(torque < 0)
        {
          torq = (int)(-1*torque);
          n = sprintf(sendBuf, "  Torque:  -%i", torq);
        }
        else
        {
          torq = (int)(torque);
          n = sprintf(sendBuf, "  Torque:  %i", torq);
        }
  
        UARTSend((unsigned char*)sendBuf, n);
        UARTSend((unsigned char*)"  ", 1);
                                         
        UARTSend((unsigned char*)"\n", 1);
    }
    else
    {
      controlDelay++; 
    }
  
  
}