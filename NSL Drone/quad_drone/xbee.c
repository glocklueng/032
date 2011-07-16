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
#include "pixhawk/mavlink.h"

// UART Variables
volatile static unsigned long uartDelay = 0;
volatile static unsigned long controlDelay = 0;
volatile static unsigned long  mavlinkDelay = 0;

//  Natural Constants
//static const float convert_pi_180 = 0.017453f;	    // Pi/180 - Convert Degrees to Radians

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
//  Send Data Telemetry
//  
//  Sends the IMU and dt data through the Xbee (UART)
//
void sendDataTelemetry(float *imu, float dt)
{
    IntMasterDisable();
    
    if(uartDelay > 20)
    {
      uartDelay = 0;
      
      for(int i=0; i < 8; i++)
      {
        char sendBuf[32];
        unsigned long n = 0;
        
        if(i <= 6)
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
          case 6:
            UARTSend((unsigned char*)"X-Raw: ", 7);
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
    
    IntMasterEnable();
}

//*****************************************************************************
// Send Control Telemetry
//
// Sends the control systems data to through the Xbee (UART)
//
void sendControlTelemetry(float torque, float P, float I, float D)
{
    IntMasterDisable();
    
    if(controlDelay > 20)
    {
      controlDelay = 0;
      for(int i=0; i < 4; i++)
      {
        char sendBuf[32];
        unsigned long n = 0;
        int gainterm;
        
        if(i <= 2)
        {
          
          switch(i)
          {
          case 0: 
            UARTSend((unsigned char*)"P: ", 3); 
            break;
          case 1:
            UARTSend((unsigned char*)"I: ", 3);
            break;
          case 2:
            UARTSend((unsigned char*)"D: ", 3);
            break;
          default:
            UARTSend((unsigned char*)"ERROR", 5);
            break;
          }
          
          switch(i)
          {
            case 0: 
              gainterm = (int)P;
              break;
            case 1:
              gainterm = (int)I;
              break;
            case 2:
              gainterm = (int)D;
              break;
            default:
              UARTSend((unsigned char*)"ERROR", 5);
              break;
          }
          
      
          if(gainterm < 0)
          {
            gainterm = -1*gainterm;
            n = sprintf(sendBuf, "-%i", gainterm);
          }
          else
          {
            n = sprintf(sendBuf, "%i", gainterm);
          }
          
          UARTSend((unsigned char*)sendBuf, n);
          UARTSend((unsigned char*)"  ", 1);
        }
        else
        {
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
        }

      }                                   
      UARTSend((unsigned char*)"\n", 1);
    }
    else
    {
      controlDelay++; 
    }
    
    IntMasterEnable();
}

//*****************************************************************************
// Send MAVLink Data
//  
// Packets are constructed to send to QGroundControl for data
// telemetry.
//
void sendMAVLinkData(float *imu, float dt)
{
    if(mavlinkDelay > 1)
    {
      mavlinkDelay = 0;
      // Define the system type, in this case an airplane
      int system_type = MAV_QUADROTOR;
      int autopilot_type = MAV_AUTOPILOT_GENERIC;
       
      // Initialize the required buffers
      mavlink_message_t msg;
      uint8_t buf[MAVLINK_MAX_PACKET_LEN];
      
      // Pack the message
      // mavlink_message_heartbeat_pack(system id, component id, message container, system type, MAV_AUTOPILOT_GENERIC)
      // try 0x55 0x20
      // else try 26 200
      mavlink_msg_heartbeat_pack(100, 200, &msg, system_type, autopilot_type);
      
      // Copy the message to the send buffer
      uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
       
      // Send the message with the standard UART send function
      UARTSend((unsigned char*)buf, len);
      
      // SEND IMU DATA
      mavlink_msg_scaled_imu_pack(100, 200, &msg, 0, (int16_t)imu[0], (int16_t)imu[1], (int16_t)imu[2], (int16_t)imu[3], (int16_t)imu[4], (int16_t)imu[5], 0, 0, 0);
      
      // Copy the message to the send buffer
      len = mavlink_msg_to_send_buffer(buf, &msg);
       
      // Send the message with the standard UART send function
      UARTSend((unsigned char*)buf, len);  
     
    }
    else
    {
      mavlinkDelay++; 
    }
}


