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

void DbpBigString(char *str)
{
  if (str) {
  	OLEDPutBigstr(str);
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

void DbBigprintf(const char *fmt, ...) 
{
// should probably limit size here; oh well, let's just use a big buffer
	char output_string[128];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(output_string, fmt,ap);
	va_end(ap);

	DbpBigString(output_string);
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
#if DMA_ADC == 1
static unsigned int ReadAdc(int ch)
{
	return ADC_Ampl[1-ch];
}
#else
static unsigned int ReadAdc(int ch)
{

  if( ch == 0  ){
//    ADC_SoftwareStartConv(ADC1);//Start the conversion
 	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
	 return ADC_GetConversionValue(ADC1); //Return the converted data
  } else {
  //  ADC_SoftwareStartConv(ADC2);//Start the conversion
 	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));//Processing the conversion
	 return ADC_GetConversionValue(ADC2); //Return the converted data
  }
}
#endif

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
//UsbSendPacket((uint8_t *)&c, sizeof(c));
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

		Dbprintf("\r%d mV           ",vHf/10);
		
		if (BUTTON_PRESS()) 
		  break;
	}
	DbpString("\nCancelled");
}


void test_fpga()
{

  return;
  
  int i;
  
      FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_READER);
      
      for (i=255; i>19; i--) {
      
	      WDT_HIT();
	      
	      FpgaSendCommand(FPGA_CMD_SET_DIVISOR, i);
	      
	      DelaymS(5000);
      }
      
      FpgaWriteConfWord(FPGA_MAJOR_MODE_OFF);	
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
  // PWM1 gives a rounder pulse
  // PWM2 gives a more pointed
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


// compose fc/8 fc/10 waveform
static void fc(int c, int *n) {
	uint8_t *dest = (uint8_t *)BigBuf;
	int idx;

	// for when we want an fc8 pattern every 4 logical bits
	if(c==0) {
		dest[((*n)++)]=1;
		dest[((*n)++)]=1;
		dest[((*n)++)]=0;
		dest[((*n)++)]=0;
		dest[((*n)++)]=0;
		dest[((*n)++)]=0;
		dest[((*n)++)]=0;
		dest[((*n)++)]=0;
	}
	//	an fc/8  encoded bit is a bit pattern of  11000000  x6 = 48 samples
	if(c==8) {
		for (idx=0; idx<6; idx++) {
			dest[((*n)++)]=1;
			dest[((*n)++)]=1;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
		}
	}

	//	an fc/10 encoded bit is a bit pattern of 1110000000 x5 = 50 samples
	if(c==10) {
		for (idx=0; idx<5; idx++) {
			dest[((*n)++)]=1;
			dest[((*n)++)]=1;
			dest[((*n)++)]=1;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
			dest[((*n)++)]=0;
		}
	}
}

// prepare a waveform pattern in the buffer based on the ID given then
// simulate a HID tag until the button is pressed
void CmdHIDsimTAG(int hi, int lo, int ledcontrol)
{
	int n=0, i=0;
	/*
	 HID tag bitstream format
	 The tag contains a 44bit unique code. This is sent out MSB first in sets of 4 bits
	 A 1 bit is represented as 6 fc8 and 5 fc10 patterns
	 A 0 bit is represented as 5 fc10 and 6 fc8 patterns
	 A fc8 is inserted before every 4 bits
	 A special start of frame pattern is used consisting a0b0 where a and b are neither 0
	 nor 1 bits, they are special patterns (a = set of 12 fc8 and b = set of 10 fc10)
	*/

	if (hi>0xFFF) {
		DbpString("Tags can only have 44 bits.");
		return;
	}
	fc(0,&n);
	// special start of frame marker containing invalid bit sequences
	fc(8,  &n);	fc(8,  &n);	// invalid
	fc(8,  &n);	fc(10, &n); // logical 0
	fc(10, &n);	fc(10, &n); // invalid
	fc(8,  &n);	fc(10, &n); // logical 0

	WDT_HIT();
	// manchester encode bits 43 to 32
	for (i=11; i>=0; i--) {
		if ((i%4)==3) fc(0,&n);
		if ((hi>>i)&1) {
			fc(10, &n);	fc(8,  &n);		// low-high transition
		} else {
			fc(8,  &n);	fc(10, &n);		// high-low transition
		}
	}

	WDT_HIT();
	// manchester encode bits 31 to 0
	for (i=31; i>=0; i--) {
		if ((i%4)==3) fc(0,&n);
		if ((lo>>i)&1) {
			fc(10, &n);	fc(8,  &n);		// low-high transition
		} else {
			fc(8,  &n);	fc(10, &n);		// high-low transition
		}
	}

	if (ledcontrol)
		LED_A_ON();
	SimulateTagLowFrequency(n, 0, ledcontrol);

	if (ledcontrol)
		LED_A_OFF();
}

void SimulateTagLowFrequency(int period, int gap, int ledcontrol)
{
	int i;
	uint8_t *tab = (uint8_t *)BigBuf;
      
	OLEDClear();
  
      	OLEDPutstr("SimulateTagLowFrequency\n");
	Dbprintf("Period = %d\nGap = %d\n", period, gap );
	OLEDDraw();
	 
	FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_EDGE_DETECT);
    /*
	AT91C_BASE_PIOA->PIO_PER = GPIO_SSC_DOUT | GPIO_SSC_CLK;//Pin Enable Register
    
	AT91C_BASE_PIOA->PIO_OER = GPIO_SSC_DOUT;//Output Enable Register
	AT91C_BASE_PIOA->PIO_ODR = GPIO_SSC_CLK;//Output Disable Register
    */
	

// SSP_DOUT(PB5)(GPIO_SSC_DOUT) -> SSP_DOUT(P5) (ssp_dout)
	
#define SHORT_COIL()	LOW( GPIO_SSC_DOUT )
#define OPEN_COIL()	HIGH( GPIO_SSC_DOUT )
    
	i = 0;
	for(;;) {
		// SSP_CLK(P9(ssp_clk) -> SSP_CLK_PIN GPIOB/GPIO_Pin_2(SSP_CLK) 
		while(!(GETBIT(GPIO_SSC_CLK))) {
			if(BUTTON_PRESS()) {
				DbpString("Stopped");
				return;
			}
			WDT_HIT();
		}
        
		if (ledcontrol)
			LED_D_ON();
        
		if(tab[i])
			OPEN_COIL();
		else
			SHORT_COIL();
        
		if (ledcontrol)
			LED_D_OFF();
        
		while( GETBIT( GPIO_SSC_CLK )) {
			if(BUTTON_PRESS()) {
				DbpString("Stopped");
				return;
			}
			WDT_HIT();
		}
        
		i++;
		if(i == period) {
			i = 0;
			if (gap) {
				SHORT_COIL();
				SpinDelayUs(gap);
			}
		}
	}
}

void FPGASpiSendWord(unsigned short cmdword);

void cycleFPGAMode( void ) {

  static unsigned short pFPGAMode = 0;
  
  // Debug FPGA modes
  switch( pFPGAMode ) {
  	case  0:
       		FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_READER);
		break;
  	case  1:
	  	FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_EDGE_DETECT);
		break;
  	case  2:
  		FpgaWriteConfWord(FPGA_MAJOR_MODE_HF_READER_TX);
		break;
  	case  3:
	  	FpgaWriteConfWord(FPGA_MAJOR_MODE_HF_READER_RX_XCORR);
		break;
  	case  4:
  		FpgaWriteConfWord(FPGA_MAJOR_MODE_HF_SIMULATOR);
		break;
  	case  5:
  		FpgaWriteConfWord(FPGA_MAJOR_MODE_HF_ISO14443A);
		break;
  	case  6:
  		FpgaWriteConfWord(FPGA_MAJOR_MODE_LF_PASSTHRU);
		break;
  	default:
  		FpgaWriteConfWord(FPGA_MAJOR_MODE_OFF);
		pFPGAMode = 0;
		return;

  }
  
  pFPGAMode++; 
  
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
  
  OLEDPutstr("FPGA INIT\n");
  OLEDDraw();
  LEDSet(0);
  
  OLEDClear();
  
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));  
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  
  // Kicks off the 24Mhz PCK0 (DOUT/PA10)
  SetupPCK0Clock();
  
  FpgaDownloadAndGo();
  
#if 0
  OLEDClear();
  OLEDPutstr("FPGA TEST MODE\n");
  OLEDDraw();
  
  HIGH(FPGAON);					// Enable VREG's
  DelaymS(100);
  
  // about 1.3MHz
  while(1) {
    LOW(NCS);
    DelayuS(1);
    HIGH(NCS);
    DelayuS(1);
  }
  
  while(1) {
    LOW(GPIO_FPGA_DIN);
    HIGH(GPIO_FPGA_DIN);
  }
#endif
  
  OLEDClear();
  OLEDPutstr("SYSTEM INIT\n");
  OLEDPutstr("SYSTEM READY\n");
  OLEDDraw();
  
  DelaymS(1000);
  
  OLEDDisplayPicture( l1logo_inv );
  OLEDDraw();
  
  DelaymS(1000);
  
  // button test
  while( 1 ) {
    
    OLEDClear();
    
    if( !GETBIT( SW_K1 ) ){
      OLEDPutstr("K1 ON\n");
      
    } else{
      OLEDPutstr("K1 OFF\n");
    }
    
    if( !GETBIT( SW_K2 ) ){
      OLEDPutstr("K2 ON\n");
      cycleFPGAMode();
      DelaymS(100);
      
    } else {
      OLEDPutstr("K2 OFF\n");
    }
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
    
    if( GETBIT( GPIO_OE1 ) ){ 
      OLEDPutstr("OE1 ");
    }	
    if( GETBIT( GPIO_OE2 ) ){ 
      OLEDPutstr("OE2 ");
    }	
    if( GETBIT( GPIO_OE2 ) ){ 
      OLEDPutstr("OE3 ");
    }	
    if( GETBIT( GPIO_OE4 ) ){ 
      OLEDPutstr("OE4                   \n");
    }	
    
    OLEDDraw();
  }
  
  OLEDClear();
  
  CmdHIDsimTAG(0x20,0x06040ef9 , 1);
  
  SimulateTagLowFrequency(1000,100,1);
  
  MeasureAntennaTuning();
  DelaymS( 5000 );
  
  MeasureAntennaTuningHf();  
  DelaymS( 500 );
  DelaymS(25000);
  
  ////////////////// Shut down 
  
  LOW(FPGAON);					// ensure everything is powered off
  HIGH(NVDD_ON);				// ensure everything is powered off
  
  OLEDClear();
  OLEDPutstr("SHUTTING DOWN\n\nBYE BYE\n\nLAYERONE 2014");
  OLEDDraw();
  
  return 0;
}
