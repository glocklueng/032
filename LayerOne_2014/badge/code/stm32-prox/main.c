/*
								XXXXXX                                                         XXX
								 XX  XX                                                         XX
								 XX  XX  XX XXX     XXXX   XX   XX  XXX XX    XXXXX   XX XXX    XX  XX
								 XXXXX    XXX XX   XX  XX   XX XX   XX X XX       X    XXX XX   XX XX
								 XX       XX       XX  XX    XXX    XX X XX  XXXXXX    XX       XXX
								 XX       XX       XX  XX   XX XX   XX   XX  X   XX    XX       XX XX
								XXXX     XXXX       XXXX   XX   XX  XXX  XXX XXXXX X  XXXX     XXX  XX 		
								  
								  													L12 2014 Edition 10th Anniversary
								  

								  LayerOne 2014 - www.layerone.org

 
 charliex - nullspacelabs.com

*/


// STM32 Library
#include "stm32f10x_lib.h"
#include "usb_lib.h"

// defines for hardware board
#include "l1_board.h"
#include "hw_config.h"
#include "proxmark3.h"

#include "logos.h"

int  RCC_Configuration(void);
void NVIC_Configuration(void);
void LEDsSet (unsigned int State);
void TimingDelay_Decrement(void);

// Data

static vu32 TimingDelay;

SPI_InitTypeDef  SPI_InitStructure;   

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
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
void LEDSet (unsigned char State)
{
  GPIO_WriteBit(LED_PORT,LED_PIN  ,State?Bit_SET:Bit_RESET);
}

void SetSpeaker(unsigned int delay)
{
  GPIO_WriteBit(SPEAKER_PORT,SPEAKER_PIN,Bit_SET); 
  DelayuS(delay );	
  GPIO_WriteBit(SPEAKER_PORT,SPEAKER_PIN,Bit_RESET);  
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void DelayuS(vu32 nCount)
{  
  while (nCount--);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void DelaymS(u32 nTime)
{
  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  TimingDelay = nTime;

  while(TimingDelay != 0);

  /* Disable SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);
  
  /* Clear SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
* Function Name  : TimingDelay_Decrement
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


// Setup CPU GPIO's etc.
void InitBoard( void ) 
{
	USB_Interrupts_Config();
	Set_USBClock();
	USB_Init();
}

void FpgaDownloadAndGo(void );

#define DEF_SPEED   0
// 100==0 1.3923
//10 = 12.7
// 1 = 70khz
// 0 = 140khz

// PCK0 needs to run at 24Mhz, main clock is 72MHz, so div 3
void SetupPCK0Clock(void)
{
  uint16_t Period;
  
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  
  Period = (SystemCoreClock / 24000000); // Must be divisable
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // Map TIM1 OC2/OC3
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);
  
  // Enable Timer1 clock and release reset
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1,DISABLE);
  
  TIM_InternalClockConfig(TIM1);
  
  // Time base configuration
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = Period-1; 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
  
  // Channel 2 Configuration in PWM1 mode
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Period/2;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);
  TIM_CtrlPWMOutputs(TIM1,ENABLE);
  
  // TIM1 counter enable
  TIM_Cmd(TIM1,ENABLE);
}

// starts here
int main(void)
{
  
#ifdef DEBUG
  debug();
#endif
  
  /* System Clocks Configuration **********************************************/
  // if this fails (xtal fault ) LED will blink indefinitely short off ,long on, repeat
  if( Set_System() == 0 ) {
	return -1;
  }
  
  LEDSet( 1 );
  
  /* NVIC Configuration *******************************************************/
  NVIC_Configuration();
  
  /* note DelaymS uses this, so don't use DelaymS before this, DelayuS doesnt but is just a counter */
  
  /* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
  SysTick_SetReload(9000);
  
  /* Enable SysTick interrupt */
  SysTick_ITConfig(ENABLE);

#if 0
	DelaymS(100);
	HIGH(FPGAON);	
	DelaymS(100);
	LOW(FPGAON);	
	DelaymS(100);
	HIGH(NVDD_ON);					// ensure everything is powered on
	DelaymS(100);
#endif
	
  // USB Enable
  InitBoard();
  // Startup OLED
  InitOLED();

  LEDSet(0);
  
  // SetupPCK0Clock();
  
  OLEDClear();
  OLEDPutstr("FPGA INIT\n");
  OLEDDraw();
  
#if 0
  HIGH(FPGAON);					// Enable VREG's
  DelaymS(100);
	
  // about 1.3MHz
  while(0) {
  	LOW(GPIO_FPGA_CCLK);
  	HIGH(GPIO_FPGA_CCLK);
  }

  while(1) {
  	LOW(GPIO_FPGA_DIN);
  	HIGH(GPIO_FPGA_DIN);
  }
#endif
  
  FpgaDownloadAndGo();
  
  OLEDClear();
  OLEDPutstr("SYSTEM INIT\n");
  OLEDPutstr("SYSTEM READY\n");
  OLEDDraw();
  
  DelaymS(1000);
  
  OLEDDisplayPicture( l1logo_inv );
  OLEDDraw();
  
  DelaymS(1000);
  
  while(1) {
	
	OLEDClear();
	
	if( !GETBIT( SW_K1 ) ){
	  OLEDPutstr("K1 ON\n");
	  
	} 
	else
	  OLEDPutstr("K1 OFF\n");
	
	if( !GETBIT( SW_K2 ) ){
	  OLEDPutstr("K2 ON\n");
	}
	else
	  OLEDPutstr("K2 OFF\n");
	
#ifdef SW_K3_PORT
	if( !GETBIT( SW_K3 ) ){
	  OLEDPutstr("K3 ON\n");
	}else
	  OLEDPutstr("K3 OFF\n");
#endif
	
	if( !GETBIT( GPIO_FPGA_DONE ) ){
	  OLEDPutstr("FPGA_DONE ON\n");
	}else
	  OLEDPutstr("FPGA_DONE OFF\n");
	
	if( !GETBIT( GPIO_FPGA_NINIT ) ){
	  OLEDPutstr("FPGA_NINIT ON\n");
	}else
	  OLEDPutstr("FPGA_NINIT OFF\n");
	
	OLEDDraw();
  }
  
  DelaymS(25000);
  
  ////////////////// Shut down 
  
  LOW(FPGAON);					// ensure everything is powered off
  HIGH(NVDD_ON);				// ensure everything is powered off
  
  OLEDClear();
  OLEDPutstr("SHUTTING DOWN\n\nBYE BYE\n\nLAYERONE 2014");
  OLEDDraw();
  
  return 0;
}
