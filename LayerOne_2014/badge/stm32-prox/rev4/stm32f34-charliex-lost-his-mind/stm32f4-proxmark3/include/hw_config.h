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

#include <stdint.h>

#include "proxmark3.h"
#include "l1_board.h"

# define SYSCLK_FREQ_72MHz (72000000)
# define SYSCLK_FREQ_168MHz (168000000)

/* Includes ------------------------------------------------------------------*/

// OLED Controller
#include "ssd1306.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define _BV(bit) (1 << (bit))

/* Exported define -----------------------------------------------------------*/

extern uint32_t SystemCoreClock;

/* Exported functions ------------------------------------------------------- */
int Set_System ( void );
void Set_USBClock ( void );
void Enter_LowPowerMode ( void );
void Leave_LowPowerMode ( void );
void USB_Interrupts_Config ( void );
void GPIO_Configuration ( void );
void EXTI_Configuration ( void );
void ADC_Configuration ( void );
void Get_SerialNum ( void );
void SetSpeaker ( unsigned int delay );
void DelayuS ( uint32_t nCount );           // 1uS Delay
void DelaymS ( uint32_t nTime );            // 1mS Delay
void GLCD_LcdInit ( void );
void OLEDDraw ( void );
void OLEDSetPixel ( unsigned char x, unsigned char y, unsigned char color );
void OLEDClear ( void );
void OLEDBigchar ( unsigned char u8Char );
void OLEDPutchar ( char u8Char );
void OLEDPutstr ( const char *ptr );
void LEDSet ( unsigned char state );
void OLEDDisplayPicture ( const uint8_t *pointer ) ;
void InvertOLED ( unsigned char state );
void OLEDPutBigstr ( const char *ptr );
uint16_t softspi_rx ( void );
void DbpBigString ( char *str );
void FPGASpiSendWord ( unsigned short cmdword );
void FpgaSetupSsc ( unsigned char on_off );
void test_spi ( void );
void RCC_Configuration(void);

//#define __enable_irq()    asm("cpsie i")
//#define __disable_irq()   asm("cpsid i")

#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
