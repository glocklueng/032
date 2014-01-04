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


int  RCC_Configuration(void);
void NVIC_Configuration(void);
void LEDsSet (unsigned int State);
void DelayuS(vu32 nCount);	 			// 1uS Delay
void DelaymS(vu32 nTime);				// 1mS Delay
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
  GPIO_WriteBit(LED_PORT,LED_PIN  ,Bit_SET);
  GPIO_WriteBit(SPEAKER_PORT,SPEAKER_PIN,Bit_SET); 
  DelayuS(delay );	
 GPIO_WriteBit(SPEAKER_PORT,SPEAKER_PIN,Bit_RESET);  
  GPIO_WriteBit(LED_PORT,LED_PIN  ,Bit_RESET);
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

#if 1
	// Configure SPI
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SPI1, &SPI_InitStructure); 
	SPI_Cmd(SPI1, ENABLE);           /* Enable the SPI  */  
#endif
}

// starts here
int main()
{
#ifdef DEBUG
  debug();
#endif

  /* System Clocks Configuration **********************************************/
  // if this fails LED will blink indefinitely short off ,long on, repeat
  if( Set_System() == 0 ) {
	return -1;
  }
  
  LEDSet(1);
  
  /* NVIC Configuration *******************************************************/
  NVIC_Configuration();

  /* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
  SysTick_SetReload(9000);

  
 /* Enable SysTick interrupt */
  SysTick_ITConfig(ENABLE);

  // USB
  InitBoard();

  // Startup OLED
  InitOLED();
  
  // Booted OK
  DelaymS(500);
  
  LEDSet(0);
  
  return 0;
}
