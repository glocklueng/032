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
#include "stm32f10x_lib.h"
#include "l1_board.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((u32)0x4001244C)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

// ADC values
uint16_t   ADC_Ampl[2];
uint32_t SystemCoreClock = SYSCLK_FREQ_72MHz ;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


extern void DelaymS(u32 nTime);
extern void DelayuS(vu32 nCount);

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
int Set_System(void)
{   
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
     
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
	
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS )
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* On STICE the PLL output clock is fixed to 72 MHz */
    /*  PLL configuration: PLLCLK = HSI/1 * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  } else {
	
	  GPIO_InitTypeDef GPIO_InitStructure;
	   
	  // make sure peripheral is on
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_LED, ENABLE);
	   
	  /* Configure the LED IOs as Output PP */
	  GPIO_InitStructure.GPIO_Pin = LED_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  
	  GPIO_Init(LED_PORT, &GPIO_InitStructure);
	  
	  while( 1 ) {

		GPIO_WriteBit(LED_PORT,LED_PIN  ,Bit_RESET );
		DelayuS(100000);
		
		GPIO_WriteBit(LED_PORT,LED_PIN  ,Bit_SET );
		DelayuS(100000);
		DelayuS(100000);
		DelayuS(100000);
		DelayuS(100000);
	  }
	   
	  return 0 ;
  }

	
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
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  
  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else 
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

#if 0

  /* Enable the EXTI0 Interrupt(KEY) */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXTI15_10 Interrupt(TAMP) */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* Enable the DMA1 Channel1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
}

/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{ 

#ifdef USB_DISCONNECT_PORT
  if (NewState != DISABLE)
  {
    GPIO_SetBits(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_ResetBits(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN);
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
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(
#ifdef RCC_APB2Periph_GPIO_DISCONNECT
					 RCC_APB2Periph_GPIO_DISCONNECT | 
#endif
					 RCC_APB2Periph_GPIOA | 
					 RCC_APB2Periph_GPIOB | 
					 RCC_APB2Periph_GPIOC | 
                     RCC_APB2Periph_GPIOD, 
                ENABLE);   


    
  // Disable JTRST , i want to use it for a USB D+ Pull Up (didn't work!)
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOB, GPIO_Pin_4  , Bit_RESET);
 
	/* Setup INPUTs */
  
#ifdef GPIOA_INPUT_FLOAT_MASK
  /* Configure GPIOA Inputs Floating */
  GPIO_InitStructure.GPIO_Pin = GPIOA_INPUT_FLOAT_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
 
#ifdef GPIOB_INPUT_FLOAT_MASK
  /* Configure GPIOB Inputs Floating */
  GPIO_InitStructure.GPIO_Pin = GPIOB_INPUT_FLOAT_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
  
#ifdef GPIOC_INPUT_FLOAT_MASK
  /* Configure GPIOC Inputs Floating */
  GPIO_InitStructure.GPIO_Pin = GPIOC_INPUT_FLOAT_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif

#ifdef GPIOD_INPUT_FLOAT_MASK
  /* Configure GPIOD Inputs Floating */
  GPIO_InitStructure.GPIO_Pin = GPIOD_INPUT_FLOAT_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
  
  
  /* Inputs that are pull downs */
  
#ifdef GPIOA_INPUT_PULLDOWN_MASK
  /* Configure GPIOA Inputs pulled down */
  GPIO_InitStructure.GPIO_Pin = GPIOA_INPUT_PULLDOWN_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
 
#ifdef GPIOB_INPUT_PULLDOWN_MASK
  /* Configure GPIOB Inputs pulled down */
  GPIO_InitStructure.GPIO_Pin = GPIOB_INPUT_PULLDOWN_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
 
#ifdef GPIOC_INPUT_PULLDOWN_MASK
  /* Configure GPIOC Inputs pulled down */
  GPIO_InitStructure.GPIO_Pin = GPIOC_INPUT_PULLDOWN_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
 
#ifdef GPIOD_INPUT_PULLDOWN_MASK
  /* Configure GPIOD Inputs pulled down */
  GPIO_InitStructure.GPIO_Pin = GPIOD_INPUT_PULLDOWN_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
  
   /* Inputs that are pull ups */
  
#ifdef GPIOA_INPUT_PULLUP_MASK
  /* Configure GPIOA Inputs pulled UP */
  GPIO_InitStructure.GPIO_Pin = GPIOA_INPUT_PULLUP_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
 
#ifdef GPIOB_INPUT_PULLUP_MASK
  /* Configure GPIOB Inputs pulled UP */
  GPIO_InitStructure.GPIO_Pin = GPIOB_INPUT_PULLUP_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
 
#ifdef GPIOC_INPUT_PULLUP_MASK
  /* Configure GPIOC Inputs pulled UP */
  GPIO_InitStructure.GPIO_Pin = GPIOC_INPUT_PULLUP_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
 
#ifdef GPIOD_INPUT_PULLUP_MASK
  /* Configure GPIOD Inputs pulled UP */
  GPIO_InitStructure.GPIO_Pin = GPIOD_INPUT_PULLUP_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
 
#ifdef GPIOB_INPUT_PULLDOWN_MASK
  /* Configure GPIOB Inputs pulled UP */
  GPIO_InitStructure.GPIO_Pin = GPIOB_INPUT_PULLDOWN_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
 
	
  /* End INPUTs */

	/* 50Mhz Outputs */
  
#ifdef GPIOA_OUTPUTS_50MHZ_PP_MASK
  /* Configure GPIOA Outputs (pp) 50Mhz*/
  GPIO_InitStructure.GPIO_Pin = GPIOA_OUTPUTS_50MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
  
#ifdef GPIOB_OUTPUTS_50MHZ_PP_MASK
  /* Configure GPIOB Outputs (pp) */
  GPIO_InitStructure.GPIO_Pin = GPIOB_OUTPUTS_50MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif  

#ifdef GPIOC_OUTPUTS_50MHZ_PP_MASK
  /* Configure GPIOC Outputs (pp) */
  GPIO_InitStructure.GPIO_Pin = GPIOC_OUTPUTS_50MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
  
#ifdef GPIOD_OUTPUTS_50MHZ_PP_MASK
  /* Configure GPIOD Outputs (pp) 50MHZ*/
  GPIO_InitStructure.GPIO_Pin = GPIOD_OUTPUTS_50MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif

  
#ifdef GPIOA_OUTPUTS_50MHZ_OD_MASK
  /* Configure GPIOA Outputs (OD) 50Mhz*/
  GPIO_InitStructure.GPIO_Pin = GPIOA_OUTPUTS_50MHZ_OD_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
  
#ifdef GPIOB_OUTPUTS_50MHZ_OD_MASK
  /* Configure GPIOB Outputs (OD) */
  GPIO_InitStructure.GPIO_Pin = GPIOB_OUTPUTS_50MHZ_OD_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif  

#ifdef GPIOC_OUTPUTS_50MHZ_OD_MASK
  /* Configure GPIOC Outputs (pp) */
  GPIO_InitStructure.GPIO_Pin = GPIOC_OUTPUTS_50MHZ_OD_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
  
#ifdef GPIOD_OUTPUTS_50MHZ_OD_MASK
  /* Configure GPIOD Outputs (OD) 50MHZ*/
  GPIO_InitStructure.GPIO_Pin = GPIOD_OUTPUTS_50MHZ_OD_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif

  
 /* 10Mhz Outputs */
  
#ifdef GPIOA_OUTPUTS_10MHZ_PP_MASK
  /* Configure GPIOA Outputs (pp) 10Mhz*/
  GPIO_InitStructure.GPIO_Pin = GPIOA_OUTPUTS_10MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
  
#ifdef GPIOB_OUTPUTS_10MHZ_PP_MASK
  /* Configure GPIOB Outputs (pp) */
  GPIO_InitStructure.GPIO_Pin = GPIOB_OUTPUTS_10MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif  

#ifdef GPIOC_OUTPUTS_10MHZ_PP_MASK
  /* Configure GPIOC Outputs (pp) */
  GPIO_InitStructure.GPIO_Pin = GPIOC_OUTPUTS_10MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
  
#ifdef GPIOD_OUTPUTS_10MHZ_PP_MASK
  /* Configure GPIOD Outputs (pp) 50MHZ*/
  GPIO_InitStructure.GPIO_Pin = GPIOD_OUTPUTS_10MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
  
   
   /* 2MHZ Outputs */
  
#ifdef GPIOA_OUTPUTS_2MHZ_PP_MASK
  /* Configure GPIOA Outputs (pp) 2MHZ*/
  GPIO_InitStructure.GPIO_Pin = GPIOA_OUTPUTS_2MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
  
#ifdef GPIOB_OUTPUTS_2MHZ_PP_MASK
  /* Configure GPIOB Outputs (pp) */
  GPIO_InitStructure.GPIO_Pin = GPIOB_OUTPUTS_2MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif  

#ifdef GPIOC_OUTPUTS_2MHZ_PP_MASK
  /* Configure GPIOC Outputs (pp) */
  GPIO_InitStructure.GPIO_Pin = GPIOC_OUTPUTS_2MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
  
#ifdef GPIOD_OUTPUTS_2MHZ_PP_MASK
  /* Configure GPIOD Outputs (pp) 50MHZ*/
  GPIO_InitStructure.GPIO_Pin = GPIOD_OUTPUTS_2MHZ_PP_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif

  /* End OUTPUTS */
  
	/* Set OUTPUTs default state */
  
#ifdef GPIOA_DEFAULT_LOW_MASK
  GPIO_ResetBits(GPIOA, GPIOA_DEFAULT_LOW_MASK); 
#endif

#ifdef GPIOA_DEFAULT_HIGH_MASK
  GPIO_SetBits(GPIOA, GPIOA_DEFAULT_HIGH_MASK); 
#endif
 
#ifdef GPIOB_DEFAULT_LOW_MASK
  GPIO_ResetBits(GPIOB, GPIOB_DEFAULT_LOW_MASK); 
#endif

#ifdef GPIOB_DEFAULT_HIGH_MASK
  GPIO_SetBits(GPIOB, GPIOB_DEFAULT_HIGH_MASK); 
#endif

#ifdef GPIOC_DEFAULT_LOW_MASK
  GPIO_ResetBits(GPIOC, GPIOC_DEFAULT_LOW_MASK); 
#endif

#ifdef GPIOC_DEFAULT_HIGH_MASK
  GPIO_SetBits(GPIOC, GPIOC_DEFAULT_HIGH_MASK); 
#endif

#ifdef GPIOD_DEFAULT_LOW_MASK
  GPIO_ResetBits(GPIOD, GPIOD_DEFAULT_LOW_MASK); 
#endif

#ifdef GPIOD_DEFAULT_HIGH_MASK
  GPIO_SetBits(GPIOD, GPIOD_DEFAULT_HIGH_MASK); 
#endif

  /* End Default States */
  
  /* ANALOG Setup */
  
#ifdef GPIOA_ANALOG_MASK
  /* Configure GPIOA analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIOA_ANALOG_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif  

#ifdef GPIOB_ANALOG_MASK
  /* Configure GPIOB analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIOB_ANALOG_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif  
  
#ifdef GPIOC_ANALOG_MASK
  /* Configure GPIOC analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIOC_ANALOG_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif  
  
#ifdef GPIOD_ANALOG_MASK
  /* Configure GPIOD analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIOD_ANALOG_MASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif  
  
 
 /* End ANALOG */
  
 
  // Setup Timer 24MHz
  
  GPIO_InitStructure.GPIO_Pin = PCK0_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(PCK0_PORT, &GPIO_InitStructure);
  
  /* Connect TIM1 pins to AF */
//  GPIO_PinAFConfig(PCK0_PORT, GPIO_PinSource4, GPIO_AF_TIM1);
  
  
#if 0
  
  
  /* Configure the FPGA NINIT  IO as Input Floating */
  GPIO_InitStructure.GPIO_Pin = ( FPGA_NINIT_PIN  );
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(FPGA_NINIT_PORT, &GPIO_InitStructure);
 
  /* Configure the FPGA  nprogram and nvdd IOs as Output PP */
  GPIO_InitStructure.GPIO_Pin = ( FPGA_NPROGRAM_PIN | NVDD_ON_PIN );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(FPGAON_PORT, &GPIO_InitStructure);
  // turns off power to FPGA, and leaves it in reset mode
  GPIO_WriteBit(FPGAON_PORT,( FPGA_NPROGRAM_PIN | NVDD_ON_PIN ),Bit_RESET); 
  
  /* Configure the FPGA  DOUT and SSC IOs as Output PP */
  GPIO_InitStructure.GPIO_Pin = ( FPGA_DOUT_PIN );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(FPGA_DOUT_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(FPGA_DOUT_PORT,( FPGA_DOUT_PIN ),Bit_RESET); 

  /* Configure the FPGA DONE  IO as Input Floating */
  /* FPGA Done wil toggle in after the bistream is loaded */
  GPIO_InitStructure.GPIO_Pin = ( FPGA_DONE_PIN  );
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(FPGA_DONE_PORT, &GPIO_InitStructure);
  
  /* Configure AMPL_LO and AMPL_HI IO as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = ( AMPL_LO_PIN | AMPL_HI_PIN );
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(AMPL_PORT, &GPIO_InitStructure);

  /* Configure the LED IOs as Output PP */
  GPIO_InitStructure.GPIO_Pin = LED_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LED_PORT, &GPIO_InitStructure);
  
  /* Configure the SPEAKER IOs as Output PP */
  GPIO_InitStructure.GPIO_Pin = SPEAKER_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPEAKER_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(SPEAKER_PORT,SPEAKER_PIN,Bit_RESET); 
  
  /* Configure the RELAY IO as Output PP */  
  GPIO_InitStructure.GPIO_Pin = RELAY_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(RELAY_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(RELAY_PORT,RELAY_PIN,Bit_RESET); 
  
  //OLED outputs, !RESET and !CS and D/C ( RS)
  GPIO_InitStructure.GPIO_Pin = RES_PIN |CS_PIN | DC_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(OLED_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(OLED_PORT, RES_PIN|CS_PIN |DC_PIN,Bit_SET); 
 
  //OLED outputs, DB1/ DB0
  GPIO_InitStructure.GPIO_Pin = SDIN_DB1_PIN | SCLK_DB0_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(OLED2_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(OLED2_PORT, ( SDIN_DB1_PIN | SCLK_DB0_PIN ) ,Bit_RESET);  

#endif
}



/*******************************************************************************
* Function Name : EXTI_Configuration.
* Description   : Configure the EXTI lines for Key and Tamper push buttons.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable the APB2 GPIOB Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  
    
  /* Connect SSP Frame to the correspondent EXTI line */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
  
  /* Configure SSP Frame EXTI line to generate an interrupt on rising edges */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Clear the SSP FRAME EXTI line pending bit */
  EXTI_ClearITPendingBit(EXTI_Line1);

#if 0  
  /* Connect SSP CLK to the correspondent EXTI line */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
  
  /* Configure SW K2 EXTI Line to generate an interrupt rising edges */  
  EXTI_InitStructure.EXTI_Line 		= EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode 		= EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	= EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
  EXTI_Init(&EXTI_InitStructure);
#endif
  
  FpgaSetupSsc( 0);
  
  /* Clear the EXTI line pending bit */
  EXTI_ClearITPendingBit(EXTI_Line2);
  
}

/*******************************************************************************
* Function Name : ADC_Configuration.
* Description   : Configure the ADC and DMA.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
#if DMA_ADC == 1

  DMA_InitTypeDef DMA_InitStructure;

  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* DMA1 channel1 configuration ---------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Ampl[0];
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* Disable the DMA1 Channel1 Transfer complete interrupt */
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, DISABLE);
#endif
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC0 regular channel configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);

  /* ADC1 regular channel configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);

#if DMA_ADC == 1

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

#endif
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));  
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum( void )
{
  u32 Device_Serial0, Device_Serial1, Device_Serial2;
  
  Device_Serial0 = *(u32*)(0x1FFFF7E8);
  Device_Serial1 = *(u32*)(0x1FFFF7EC);
  Device_Serial2 = *(u32*)(0x1FFFF7F0);
  
  return ;
  
  if(Device_Serial0 != 0)
  {
    
    // starts at 2, to skip the length / type
     CustomHID_StringSerial[2] = (u8)(Device_Serial0 & 0x0000000F);  
     CustomHID_StringSerial[4] = (u8)((Device_Serial0 & 0x00000F00) >> 8);
     CustomHID_StringSerial[6] = (u8)((Device_Serial0 & 0x000F0000) >> 16);
     CustomHID_StringSerial[8] = (u8)((Device_Serial0 & 0x0F000000) >> 24);  
     
     CustomHID_StringSerial[10] = (u8)(Device_Serial1 & 0x0000000F);  
     CustomHID_StringSerial[12] = (u8)((Device_Serial1 & 0x00000F00) >> 8);
     CustomHID_StringSerial[14] = (u8)((Device_Serial1 & 0x000F0000) >> 16);
     CustomHID_StringSerial[16] = (u8)((Device_Serial1 & 0x0F000000) >> 24); 
     
     CustomHID_StringSerial[18] = (u8)(Device_Serial2 & 0x0000000F);  
     CustomHID_StringSerial[20] = (u8)((Device_Serial2 & 0x00000F00) >> 8);
     CustomHID_StringSerial[22] = (u8)((Device_Serial2 & 0x000F0000) >> 16);
     CustomHID_StringSerial[24] = (u8)((Device_Serial2 & 0x0F000000) >> 24); 
  }   
}
/**************************************************************************************/
  
void RCC_Configuration(void)
{
  /* --------------------------- System Clocks Configuration -----------------*/
  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  /* GPIOA clock enable */
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
}
  
/**************************************************************************************
 * Setup the 24Mhz clock
 **************************************************************************************/
  
void TIM1_Configuration(void)
{
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  uint16_t Period;
  
  Period = (SystemCoreClock  / 24000000); // Must be divisable
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0; // Dump 1X clock into timer
  TIM_TimeBaseStructure.TIM_Period = Period - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  /* Enable TIM1 Preload register on ARR */
  TIM_ARRPreloadConfig(TIM1, ENABLE);
  
  /* TIM PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Period / 2; // 50%
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  
  /* Output Compare PWM1 Mode configuration: Channel1 PA.08 */
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  
  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  
  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);
}

/*******************************************************************************
* Function Name  : Timer2 configurations
* Description    : Configure Timer2 in such a way that it can initiate data transfer using DMA on 
*		  rising edge of clock signal received on port pin.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_Configuration(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  /* TIM2 clock enable */
  //RCC_AHB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter    = 0x0;
  TIM_ICInit(TIM1, &TIM_ICInitStructure);
  
  TIM_CtrlPWMOutputs(TIM2, ENABLE);       /* TIM Main Inputs/Output Enable    */
  TIM_Cmd(TIM2, ENABLE);                  /* TIM enable counter               */
  TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE ); /* Enable TIM2_CC1 DMA Requests     */
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

