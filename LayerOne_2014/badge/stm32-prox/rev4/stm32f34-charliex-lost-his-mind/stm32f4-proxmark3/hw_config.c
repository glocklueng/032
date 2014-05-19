/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm32f4xx.h"
#include "l1_board.h"
#include "hw_config.h"
#include "stm32f4xx_adc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define ADC1_DR_Address    ((u32)0x4001244C)
#define ADC1_DR_Address    ((uint32_t)0x40012308)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

// ADC values
__IO uint16_t   ADC_Ampl[2];

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern void DelaymS ( u32 nTime );
extern void DelayuS ( vu32 nCount );

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
int Set_System ( void )
{

	/* Configure the used GPIOs*/
	GPIO_Configuration();

	/* Configure the ADC*/
	ADC_Configuration();

	return 1;
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock ( void )
{
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode ( void )
{
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode ( void )
{

}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config ( void )
{

}

/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config ( FunctionalState NewState )
{

#ifdef USB_DISCONNECT_PORT

	if ( NewState != DISABLE ) {
		GPIO_SetBits ( USB_DISCONNECT_PORT, USB_DISCONNECT_PIN );
	}

	else {
		GPIO_ResetBits ( USB_DISCONNECT_PORT, USB_DISCONNECT_PIN );
	}

#endif

}

/*
    GPIO_Mode_AIN           ;Analog in
    GPIO_Mode_IN_FLOATING   ;input floating (save more power compare to IPD or IPU - use if appropriate)
    GPIO_Mode_IPD           ;input pulled down
    GPIO_Mode_IPU           ;input pulled up

*/

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE);  
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB , ENABLE);  
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC , ENABLE);  
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD , ENABLE);  
  

	// Disable JTRST , i want to use it for a USB D+ Pull Up (didn't work!)
	//GPIO_PinRemapConfig ( GPIO_Remap_SWJ_NoJTRST, ENABLE );

      /*-------------------------- GPIO Configuration ----------------------------*/
	
	//PCK0
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      
      /* Connect TIM1 pins to AF */
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
     
 	 memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
	GPIO_WriteBit ( GPIOB, GPIO_Pin_4  , Bit_RESET );

	/* Setup INPUTs */

#ifdef GPIOA_INPUT_FLOAT_MASK
	/* Configure GPIOA Inputs Floating */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOA_INPUT_FLOAT_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
#endif

#ifdef GPIOB_INPUT_FLOAT_MASK
	/* Configure GPIOB Inputs Floating */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOB_INPUT_FLOAT_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
#endif

#ifdef GPIOC_INPUT_FLOAT_MASK
	/* Configure GPIOC Inputs Floating */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOC_INPUT_FLOAT_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init ( GPIOC, &GPIO_InitStructure );
#endif

#ifdef GPIOD_INPUT_FLOAT_MASK
	/* Configure GPIOD Inputs Floating */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOD_INPUT_FLOAT_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
#endif


	/* Inputs that are pull downs */

#ifdef GPIOA_INPUT_PULLDOWN_MASK
	/* Configure GPIOA Inputs pulled down */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOA_INPUT_PULLDOWN_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
#endif

#ifdef GPIOB_INPUT_PULLDOWN_MASK
	/* Configure GPIOB Inputs pulled down */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOB_INPUT_PULLDOWN_MASK; // drops k1 to 2.67V
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
#endif

#ifdef GPIOC_INPUT_PULLDOWN_MASK
	/* Configure GPIOC Inputs pulled down */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOC_INPUT_PULLDOWN_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOC, &GPIO_InitStructure );
#endif

#ifdef GPIOD_INPUT_PULLDOWN_MASK
	/* Configure GPIOD Inputs pulled down */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOD_INPUT_PULLDOWN_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
#endif

	/* Inputs that are pull ups */

#ifdef GPIOA_INPUT_PULLUP_MASK
	/* Configure GPIOA Inputs pulled UP */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOA_INPUT_PULLUP_MASK; //draging k1 to 2.6v
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
#endif

#ifdef GPIOB_INPUT_PULLUP_MASK
	/* Configure GPIOB Inputs pulled UP */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOB_INPUT_PULLUP_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
#endif

#ifdef GPIOC_INPUT_PULLUP_MASK
	/* Configure GPIOC Inputs pulled UP */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOC_INPUT_PULLUP_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOC, &GPIO_InitStructure );
#endif

#ifdef GPIOD_INPUT_PULLUP_MASK
	/* Configure GPIOD Inputs pulled UP */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOD_INPUT_PULLUP_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
#endif

#ifdef GPIOB_INPUT_PULLDOWN_MASK
	/* Configure GPIOB Inputs pulled UP */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOB_INPUT_PULLDOWN_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
#endif


	/* End INPUTs */

	/* 50Mhz Outputs */

#ifdef GPIOA_OUTPUTS_50MHZ_PP_MASK
	/* Configure GPIOA Outputs (pp) 50Mhz*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOA_OUTPUTS_50MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
#endif

#ifdef GPIOA_OUTPUTS_50MHZ_AF_MASK
	/* Configure GPIOA Outputs (AF) 50Mhz*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOA_OUTPUTS_50MHZ_AF_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_AF;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
#endif



#ifdef GPIOB_OUTPUTS_50MHZ_PP_MASK
	/* Configure GPIOB Outputs (pp) */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOB_OUTPUTS_50MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
#endif

#ifdef GPIOC_OUTPUTS_50MHZ_PP_MASK
	/* Configure GPIOC Outputs (pp) */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOC_OUTPUTS_50MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOC, &GPIO_InitStructure );
#endif

#ifdef GPIOD_OUTPUTS_50MHZ_PP_MASK
	/* Configure GPIOD Outputs (pp) 50MHZ*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOD_OUTPUTS_50MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
#endif


#ifdef GPIOA_OUTPUTS_50MHZ_OD_MASK
	/* Configure GPIOA Outputs (OD) 50Mhz*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOA_OUTPUTS_50MHZ_OD_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
#endif

#ifdef GPIOB_OUTPUTS_50MHZ_OD_MASK
	/* Configure GPIOB Outputs (OD) */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOB_OUTPUTS_50MHZ_OD_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	TIM_OCInitStructure ( GPIOB, &GPIO_InitStructure );
#endif

#ifdef GPIOC_OUTPUTS_50MHZ_OD_MASK
	/* Configure GPIOC Outputs (pp) */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOC_OUTPUTS_50MHZ_OD_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init ( GPIOC, &GPIO_InitStructure );
#endif

#ifdef GPIOD_OUTPUTS_50MHZ_OD_MASK
	/* Configure GPIOD Outputs (OD) 50MHZ*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOD_OUTPUTS_50MHZ_OD_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
#endif


	/* 10Mhz Outputs */

#ifdef GPIOA_OUTPUTS_10MHZ_PP_MASK
	/* Configure GPIOA Outputs (pp) 10Mhz*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOA_OUTPUTS_10MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
#endif

#ifdef GPIOB_OUTPUTS_10MHZ_PP_MASK
	/* Configure GPIOB Outputs (pp) */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOB_OUTPUTS_10MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
#endif

#ifdef GPIOC_OUTPUTS_10MHZ_PP_MASK
	/* Configure GPIOC Outputs (pp) */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOC_OUTPUTS_10MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOC, &GPIO_InitStructure );
#endif

#ifdef GPIOD_OUTPUTS_10MHZ_PP_MASK
	/* Configure GPIOD Outputs (pp) 50MHZ*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOD_OUTPUTS_10MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
#endif


	/* 2MHZ Outputs */

#ifdef GPIOA_OUTPUTS_2MHZ_PP_MASK
	/* Configure GPIOA Outputs (pp) 2MHZ*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOA_OUTPUTS_2MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
#endif

#ifdef GPIOB_OUTPUTS_2MHZ_PP_MASK
	/* Configure GPIOB Outputs (pp) */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOB_OUTPUTS_2MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
#endif

#ifdef GPIOC_OUTPUTS_2MHZ_PP_MASK
	/* Configure GPIOC Outputs (pp) */
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOC_OUTPUTS_2MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOC, &GPIO_InitStructure );
#endif

#ifdef GPIOD_OUTPUTS_2MHZ_PP_MASK
	/* Configure GPIOD Outputs (pp) 50MHZ*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOD_OUTPUTS_2MHZ_PP_MASK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
#endif

	/* End OUTPUTS */

	/* Set OUTPUTs default state */

#ifdef GPIOA_DEFAULT_LOW_MASK
	GPIO_ResetBits ( GPIOA, GPIOA_DEFAULT_LOW_MASK );
#endif

#ifdef GPIOA_DEFAULT_HIGH_MASK
	GPIO_SetBits ( GPIOA, GPIOA_DEFAULT_HIGH_MASK );
#endif

#ifdef GPIOB_DEFAULT_LOW_MASK
	GPIO_ResetBits ( GPIOB, GPIOB_DEFAULT_LOW_MASK );
#endif

#ifdef GPIOB_DEFAULT_HIGH_MASK
	GPIO_SetBits ( GPIOB, GPIOB_DEFAULT_HIGH_MASK );
#endif

#ifdef GPIOC_DEFAULT_LOW_MASK
	GPIO_ResetBits ( GPIOC, GPIOC_DEFAULT_LOW_MASK );
#endif

#ifdef GPIOC_DEFAULT_HIGH_MASK
	GPIO_SetBits ( GPIOC, GPIOC_DEFAULT_HIGH_MASK );
#endif

#ifdef GPIOD_DEFAULT_LOW_MASK
	GPIO_ResetBits ( GPIOD, GPIOD_DEFAULT_LOW_MASK );
#endif

#ifdef GPIOD_DEFAULT_HIGH_MASK
	GPIO_SetBits ( GPIOD, GPIOD_DEFAULT_HIGH_MASK );
#endif

	/* End Default States */

	/* ANALOG Setup */

#ifdef GPIOA_ANALOG_MASK
	/* Configure GPIOA analog input -------------------------*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOA_ANALOG_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
#endif

#ifdef GPIOB_ANALOG_MASK
	/* Configure GPIOB analog input -------------------------*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
#endif

#ifdef GPIOC_ANALOG_MASK
	/* Configure GPIOC analog input -------------------------*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOC_ANALOG_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init ( GPIOC, &GPIO_InitStructure );
#endif

#ifdef GPIOD_ANALOG_MASK
	/* Configure GPIOD analog input -------------------------*/
	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	GPIO_InitStructure.GPIO_Pin = GPIOD_ANALOG_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
#endif

	/* End ANALOG */
}



/*******************************************************************************
* Function Name : EXTI_Configuration.
* Description   : Configure the EXTI lines for Key and Tamper push buttons.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void EXTI_Configuration ( void )
{
	FpgaSetupSsc ( 0 );
}

//DMA_Stream_TypeDef ADC_DMA;

/*******************************************************************************
* Function Name : ADC_Configuration.
* Description   : Configure the ADC and DMA.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void ADC_Configuration ( void )
{
	ADC_InitTypeDef ADC_InitStructure;
  	ADC_CommonInitTypeDef ADC_CommonInitStructure;
  
#if DMA_ADC == 1

	DMA_InitTypeDef DMA_InitStructure;

	memset ( &DMA_InitStructure,0,sizeof ( DMA_InitStructure ) );
	memset ( &ADC_InitStructure,0,sizeof ( ADC_InitStructure ) );

	/* Enable DMA2 clock */
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE );

	/* Enable ADC1 clock */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );

	/* DMA2 channel 0 configuration ---------------------------------------------*/
	DMA_DeInit ( DMA2_Stream0 );
	
	DMA_InitStructure.DMA_Channel  = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_Memory0BaseAddr = ( u32 ) &ADC_Ampl[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	DMA_InitStructure.DMA_FIFOMode  = DMA_FIFOMode_Disable;           //Operate in FIFO mode
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	//DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init ( DMA2_Stream0, &DMA_InitStructure );

	/* Enable DMA1 channel1 */
	DMA_Cmd ( DMA2_Stream0, ENABLE );

	/* Disable the DMA1 Channel1 Transfer complete interrupt */
	DMA_ITConfig ( DMA2_Stream0, DMA_IT_TC, ENABLE );
#else
	
	/* Enable ADC1 clock */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );
#endif

	/* ADC1 configuration ------------------------------------------------------*/
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
#if DMA_ADC == 1
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
#else
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
#endif
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 2;
	ADC_Init ( ADC1, &ADC_InitStructure );

	/* ADC0 regular channel configuration */
	ADC_RegularChannelConfig ( ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles );

	/* ADC1 regular channel configuration */
	ADC_RegularChannelConfig ( ADC1, ADC_Channel_1, 2, ADC_SampleTime_3Cycles );

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
  	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  	ADC_CommonInit(&ADC_CommonInitStructure);
  
#if DMA_ADC == 1

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* Enable ADC1 DMA */
	ADC_DMACmd ( ADC1, ENABLE );

#endif

	/* Enable ADC1 */
	ADC_Cmd ( ADC1, ENABLE );

	ADC_SoftwareStartConv ( ADC1 );

}

/**************************************************************************************/

void RCC_Configuration(void)
{
  /* --------------------------- System Clocks Configuration -----------------*/
  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  /* GPIOA clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}
  
/**************************************************************************************/
  


/**************************************************************************************
 * Setup the 24Mhz clock
 **************************************************************************************/

void TIM1_Configuration ( void )
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint16_t Period;

	Period = ( SystemCoreClock  / 24000000 ); // Must be divisable

	memset ( &TIM_OCInitStructure,0,sizeof ( TIM_OCInitStructure ) );
	memset ( &TIM_TimeBaseStructure,0,sizeof ( TIM_TimeBaseStructure ) );

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // Dump 1X clock into timer
	TIM_TimeBaseStructure.TIM_Period = Period - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit ( TIM1, &TIM_TimeBaseStructure );

	/* Enable TIM1 Preload register on ARR */
	TIM_ARRPreloadConfig ( TIM1, ENABLE );

	/* TIM PWM1 Mode configuration */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Period / 2; // 50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* Output Compare PWM1 Mode configuration: Channel1 PA.08 */
	TIM_OC1Init ( TIM1, &TIM_OCInitStructure );
	TIM_OC1PreloadConfig ( TIM1, TIM_OCPreload_Enable );

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs ( TIM1, ENABLE );

	/* TIM1 enable counter */
	TIM_Cmd ( TIM1, ENABLE );
}

/*******************************************************************************
* Function Name  : Timer2 configurations
* Description    : Configure Timer2 in such a way that it can initiate data transfer using DMA on
*         rising edge of clock signal received on port pin.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_Configuration ( void )
{
  
	GPIO_InitTypeDef   GPIO_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	memset ( &TIM_ICInitStructure,0,sizeof ( TIM_ICInitStructure ) );

	RCC_APB2PeriphClockCmd ( RCC_APB1Periph_TIM2, ENABLE ); /* TIM2 clock enable */
	//RCC_AHB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter    = 0x0;
	TIM_ICInit ( TIM1, &TIM_ICInitStructure );

	TIM_CtrlPWMOutputs ( TIM2, ENABLE );    /* TIM Main Inputs/Output Enable    */
	TIM_Cmd ( TIM2, ENABLE );               /* TIM enable counter               */
	TIM_DMACmd ( TIM2, TIM_DMA_CC1, ENABLE ); /* Enable TIM2_CC1 DMA Requests     */
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


static volatile uint32_t TimingDelay =0;


/*******************************************************************************
* Function Name  : TimingDelay_Decrement
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void TimingDelay_Decrement ( void )
{
	if ( TimingDelay != 0x00 ) {
		TimingDelay--;
	}
}
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void DelayuS ( volatile vu32 nCount )
{
	while ( nCount-- );
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void DelaymS ( volatile u32 nTime )
{
	TimingDelay = nTime;
	while ( TimingDelay != 0 );
}


/*************************************************************************
 * Function Name: LEDSet
 * Parameters: unsigned int State, 0 off, 1 on
 *
 * Return: none
 *
 * Description: Set LEDS State
 *
 *************************************************************************/
void LEDSet ( unsigned char State )
{
	GPIO_WriteBit ( LED_PORT,LED_PIN  ,State?Bit_SET:Bit_RESET );
}

void SetSpeaker ( unsigned int delay )
{
	GPIO_WriteBit ( SPEAKER_PORT,SPEAKER_PIN,Bit_SET );
	DelayuS ( delay );
	GPIO_WriteBit ( SPEAKER_PORT,SPEAKER_PIN,Bit_RESET );
}

//-----------------------------------------------------------------------------
// Read an ADC channel and block till it completes, then return the result
// in ADC units (0 to 1023). Also a routine to average 32 samples and
// return that.
// STM32 gives us (0..4095) 12 bit
//-----------------------------------------------------------------------------
#if DMA_ADC == 1

extern __IO uint16_t  ADC_Ampl[2];

unsigned int ReadAdc( int ch )
{
	return ADC_Ampl[1-ch]/4;
}

#else

unsigned int ReadAdc ( int ch )
{

	//ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

	if ( ch == 0  ) {

		ADC_RegularChannelConfig ( ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles );

		ADC_SoftwareStartConv ( ADC1 ); //Start the conversion

		while ( ADC_GetFlagStatus ( ADC1, ADC_FLAG_EOC ) == RESET ); //Processing the conversion

		return ADC_GetConversionValue ( ADC1 ) /4; //Return the converted data
	}

	else {

		ADC_RegularChannelConfig ( ADC1, ADC_Channel_1, 2, ADC_SampleTime_3Cycles );

		ADC_SoftwareStartConv ( ADC1 ); //Start the conversion

		while ( ADC_GetFlagStatus ( ADC1, ADC_FLAG_EOC ) == RESET ); //Processing the conversion

		return ADC_GetConversionValue ( ADC1 ) /4 ; //Return the converted data
	}
}
#endif

int AvgAdc ( int ch ) // was static - merlok
{
	unsigned int i;
	unsigned int a = 0;

	for ( i = 0; i < 32; i++ ) {
		a += ReadAdc ( ch );
	}

	// convert to what the proxmark expects 0..1025 (we should change the code to allow the better resolution
//  a /= 4;

	return ( a + 15 ) >> 5;
}