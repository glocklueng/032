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

#include <string.h>
#include <stdarg.h>

// STM32 Library
#include "stm32f10x_lib.h"
#include "usb_lib.h"

// defines for hardware board
#include "l1_board.h"
#include "hw_config.h"
#include "proxmark3.h"
#include "apps.h"

#include "logos.h"
#include "printf.h"

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
//=============================================================================
// Debug print functions, to go out over USB, to the usual PC-side client.
//=============================================================================

void DbpString(char *str)
{
  if (str) {
  	OLEDPutstr(str);
  	OLEDDraw();
  }
}


void Dbprintf(const char *fmt, ...) {
// should probably limit size here; oh well, let's just use a big buffer
	char output_string[128];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(output_string, fmt,ap);
	va_end(ap);

	DbpString(output_string);
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

extern uint16_t  ADC_Ampl[2];

//-----------------------------------------------------------------------------
// Read an ADC channel and block till it completes, then return the result
// in ADC units (0 to 1023). Also a routine to average 32 samples and
// return that.
// STM32 gives us (0..4095) 12 bit
//-----------------------------------------------------------------------------
static unsigned int ReadAdc(int ch)
{
	return ADC_Ampl[1-ch];
}

int AvgAdc(int ch) // was static - merlok
{
	unsigned int i;
	unsigned int a = 0;

	for(i = 0; i < 32; i++) {
		a += ReadAdc(ch);
	}

	// convert to what the proxmark expects 0..1025 (we should change the code to allow the better resolution
	a /= 4;
	
	return (a + 15) >> 5;
}

bool cmd_send(uint32_t cmd, uint32_t arg0, uint32_t arg1, uint32_t arg2, void* data, size_t len)
{
  return 0;
}

// The large multi-purpose buffer, typically used to hold A/D samples,
// maybe processed in some way.
uint32_t BigBuf[BIG_BUFFER];

void MeasureAntennaTuning(void)
{
	uint8_t *dest = (uint8_t *)BigBuf+FREE_BUFFER_OFFSET;
	int i, adcval = 0, peak = 0, peakv = 0, peakf = 0; //ptr = 0 
	int vLf125 = 0, vLf134 = 0, vHf = 0;	// in mV

//	UsbCommand c;
  
	OLEDClear();
  
  LED_B_ON();
	DbpString("Measuring antenna\nPlease wait...\n");
	memset(dest,0,sizeof(FREE_BUFFER_SIZE));

/*
 * Sweeps the useful LF range of the proxmark from
 * 46.8kHz (divisor=255) to 600kHz (divisor=19) and
 * read the voltage in the antenna, the result left
 * in the buffer is a graph which should clearly show
 * the resonating frequency of your LF antenna
 * ( hopefully around 95 if it is tuned to 125kHz!)
 */
  
	FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_READER);
	for (i=255; i>19; i--) {
    	
	  	WDT_HIT();
		
		FpgaSendCommand(FPGA_CMD_SET_DIVISOR, i);
		SpinDelay(20);
		// Vref = 3.3V, and a 10000:240 voltage divider on the input
		// can measure voltages up to 137500 mV
		adcval = ((137500 * AvgAdc(ADC_CHAN_LF)) >> 10);
		if (i==95) 	vLf125 = adcval; // voltage at 125Khz
		if (i==89) 	vLf134 = adcval; // voltage at 134Khz

		dest[i] = adcval>>8; // scale int to fit in byte for graphing purposes
		if(dest[i] > peak) {
			peakv = adcval;
			peak = dest[i];
			peakf = i;
			//ptr = i;
		}
	}

  LED_A_ON();
	// Let the FPGA drive the high-frequency antenna around 13.56 MHz.
	FpgaWriteConfWord(FPGA_MAJOR_MODE_HF_READER_RX_XCORR);
	SpinDelay(20);
	// Vref = 3300mV, and an 10:1 voltage divider on the input
	// Vref = 3340mV, and an 10:1 voltage divider on the input
	// can measure voltages up to 33000 mV
	vHf = (VDD_MV * AvgAdc(ADC_CHAN_HF)) >> 10;

//	c.cmd = CMD_MEASURED_ANTENNA_TUNING;
//	c.arg[0] = (vLf125 << 0) | (vLf134 << 16);
//	c.arg[1] = vHf;
//	c.arg[2] = peakf | (peakv << 16);

  OLEDClear();
  DbpString("Results\n");
  Dbprintf("LF %5.2fV @ 125kHz\n",vLf125/1000.0f);
  Dbprintf("LF %5.2fV @ 134.00kHz\n\n",vLf134/1000.0f);
  
  Dbprintf("LF optimal: %5.2fV @%.2f kHz\n", peakv / 1000.0, (12000.0 / (peakf + 1.0)) );
  Dbprintf("HF antenna: %5.2fV @13.56 MHz\n", (vHf / 1000.0));
  
  if(  peakv < 2000 )
    DbpString("Your LF antenna is unusable.\n");
  else if (peakv < 10000 )
    DbpString("Your LF antenna is marginal.\n");
  else 
    DbpString("Your LF antenna is good.\n");
    
  if ( vHf < 2000 )
    DbpString("Your HF antenna is unusable.\n");
  else if ( vHf < 5000  )
    DbpString("Your HF antenna is marginal.\n");
  else 
    DbpString("Your HF antenna is good.\n");
  
	      
  cmd_send(CMD_MEASURED_ANTENNA_TUNING,vLf125|(vLf134<<16),vHf,peakf|(peakv<<16),0,0);
//	UsbSendPacket((uint8_t *)&c, sizeof(c));
  FpgaWriteConfWord(FPGA_MAJOR_MODE_OFF);
  LED_A_OFF();
  LED_B_OFF();
  return;
}

void MeasureAntennaTuningHf(void)
{
	int vHf = 0;	// in mV
	
	OLEDClear();
	
	DbpString("Measuring HF antenna\nPress button to exit\n\n");

	for (;;) {
		// Let the FPGA drive the high-frequency antenna around 13.56 MHz.
		FpgaWriteConfWord(FPGA_MAJOR_MODE_HF_READER_RX_XCORR);
		SpinDelay(20);
		// Vref = 3300mV, and an 10:1 voltage divider on the input
		// Vref = 3400mV, and an 10:1 voltage divider on the input
		// can measure voltages up to 33000 mV
		vHf = (VDD_MV * AvgAdc(ADC_CHAN_HF)) >> 10;

		Dbprintf("\r%d mV           ",vHf);
		
		if (BUTTON_PRESS()) 
		  break;
	}
	DbpString("\nCancelled");
}

// ARM Pins that can generate a 24Mhz signal via TIMx (HW Rev2)
//----------------------------------------------------------
// TIM1_CH1 pin  (PA8)  - FPGA_CCLK - cant be moved, programming pins only
// TIM1_CH1N pin (PB13) - MUX not connected to FPGA

// TIM1_CH2 pin  (PA9)  - FPGA_DIN cant be moved, programming pins only
// TIM1_CH2N pin (PB14) - MUX not connected to FPGA

// TIM1_CH3 pin  (PA10) - FPGA_DOUT cant be moved (not used) can be reassigned!
// TIM1_CH3N pin (PB15) - MUX not connected to FPGA

// TIM1_CH4 pin  (PA11) - USB cant use


// PCK0 needs to run at 24Mhz, main clock is 72MHz, so div 3
// Moved to DOUT PA10, FPGA matches now too.
void SetupPCK0Clock(void)
{
  uint16_t Period;
  
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  
  Period = (SystemCoreClock / 24000000); // Must be divisable
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
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
  
  // TI CH3 Init
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);
  
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

  // USB Enable
  InitBoard();
  // Startup OLED
  InitOLED();

  LEDSet(0);
  
  
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

  
  SetupPCK0Clock();
  
  OLEDClear();
  OLEDPutstr("SYSTEM INIT\n");
  OLEDPutstr("SYSTEM READY\n");
  OLEDDraw();
  
  DelaymS(1000);
  
  OLEDDisplayPicture( l1logo_inv );
  OLEDDraw();
  
  DelaymS(1000);
  
  // button test
  while(0) {
	
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
  

 OLEDClear();
  
  MeasureAntennaTuning();
  
  DelaymS( 5000 );
  
  MeasureAntennaTuningHf();
    
  DelaymS(25000);
    
  ////////////////// Shut down 
  
  LOW(FPGAON);					// ensure everything is powered off
  HIGH(NVDD_ON);				// ensure everything is powered off
  
  OLEDClear();
  OLEDPutstr("SHUTTING DOWN\n\nBYE BYE\n\nLAYERONE 2014");
  OLEDDraw();
  
  return 0;
}
