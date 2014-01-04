/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "usb_type.h"

// OLED Controller
#include "ssd1306.h"

/* Exported types ------------------------------------------------------------*/

typedef  unsigned short uint16_t;
typedef  unsigned char uint8_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define _BV(bit) (1 << (bit)) 

/* Exported define -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void GPIO_Configuration(void);
void EXTI_Configuration(void);
void ADC_Configuration(void);
void Get_SerialNum(void);
void SetSpeaker(unsigned int delay);
void DelayuS(vu32 nCount);	 			// 1uS Delay
void DelaymS(vu32 nTime);				// 1mS Delay
void GLCD_LcdInit(void);
void OLEDDraw(void);
void OLEDSetPixel(unsigned char x, unsigned char y, unsigned char color);
void OLEDClear( void );
void OLEDBigchar (unsigned char u8Char);
void OLEDPutchar(char u8Char);
void OLEDPutstr (const char *ptr);
void LEDSet(unsigned char state);

#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
