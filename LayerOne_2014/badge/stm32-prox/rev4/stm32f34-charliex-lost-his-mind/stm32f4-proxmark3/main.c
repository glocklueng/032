#include "stm32f4_discovery.h"
 
#include "l1_board.h"
#include "proxmark3.h"
#include "apps.h"
#include <string.h>
#include "logos.h"

#include "stm32f4xx_dma.h"

uint32_t BigBuf[BIG_BUFFER];

// PCK0 needs to run at 24Mhz, main clock is 72MHz, so div 3
// Moved to DOUT PA10, FPGA matches now too.
void SetupPCK0Clock ( void )
{
	uint16_t Period;

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	Period = ( SystemCoreClock / 24000000 ); // Must be divisable

	memset ( &GPIO_InitStructure,0,sizeof ( GPIO_InitStructure ) );
	memset ( &TIM_TimeBaseStructure,0,sizeof ( TIM_TimeBaseStructure ) );
	memset ( &TIM_OCInitStructure,0,sizeof ( TIM_OCInitStructure ) );

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );

	// Map TIM1 OC2/OC3
	//GPIO_PinRemapConfig ( GPIO_PartialRemap_TIM1,ENABLE );

	// Enable Timer1 clock and release reset
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_TIM1,ENABLE );
	RCC_APB2PeriphResetCmd ( RCC_APB2Periph_TIM1,DISABLE );

	TIM_InternalClockConfig ( TIM1 );

	// Time base configuration
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = Period-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit ( TIM1,&TIM_TimeBaseStructure );

	// Channel 2 Configuration in PWM1 mode
	// PWM1 gives a rounder pulse
	// PWM2 gives a more pointed
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState  = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Period/2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

	// TI CH3 Init
	TIM_OC3Init ( TIM1,&TIM_OCInitStructure );

	TIM_CtrlPWMOutputs ( TIM1,ENABLE );

	// TIM1 counter enable
	TIM_Cmd ( TIM1,ENABLE );
}


void DrawHIPKD_OLED ( void )
{
	uint16_t *dest = ( uint16_t * ) &BigBuf[0];
	uint8_t v = 0;
	uint16_t temp[256];

	uint16_t r;
	int i;
	int p = 0;

	memset ( temp,0,sizeof ( temp ) );

	OLEDClear();

	OLEDPutstr ( "DrawHIPKD_OLED" );
	OLEDDraw();


	// We're using this mode just so that I can test it out; the simulated
	// tag mode would work just as well and be simpler.
	FpgaWriteConfWord ( FPGA_MAJOR_MODE_HF_READER_RX_XCORR | FPGA_HF_READER_RX_XCORR_848_KHZ | FPGA_HF_READER_RX_XCORR_SNOOP );

	// We need to listen to the high-frequency, peak-detected path.
	SetAdcMuxFor ( GPIO_MUXSEL_HIPKD );

	FpgaSetupSsc ( 1 );

#define USE_FPGA_DMA ( 1 )

#ifdef  USE_FPGA_DMA
	//where too, amount to transfer in bytes
	FpgaSetupSscDma ( ( uint8_t* ) dest, 1024 ) ;
	FpgaEnableSscDma();
#else
	__disable_irq();
#endif

	i = 0;

	for ( ;; ) {

#ifdef  USE_FPGA_DMA

		while ( !DMA_GetITStatus ( DMA1_Stream3, DMA_IT_TCIF3  ) );

		// wait for a full transfer
		while ( DMA_GetCurrDataCounter ( DMA1_Stream3 )  > 2 );

		memcpy ( temp,dest,sizeof ( temp ) );

		//Clear DMA1 Channel1 Half Transfer, Transfer Complete and Global interrupt pending bits
		DMA_ClearITPendingBit (DMA1_Stream3, DMA_IT_TEIF3 | DMA_IT_DMEIF3 | DMA_IT_FEIF3 | DMA_IT_TCIF3 | DMA_IT_HTIF3 );

		while ( DMA_GetFlagStatus ( DMA1_Stream3, DMA_FLAG_TCIF3 ) == RESET ) {}

#endif
		OLEDClear();

		for ( int x =  0 ; x < 127; x ++ ) {

			register uint16_t xt;

#ifndef  USE_FPGA_DMA
			// 16 bit read
			temp[x] = softspi_rx();
#endif
			OLEDLine ( x,0,x, temp[x]>>12,1 );
			
//			OLEDLine ( x,0,x, ( ( temp[x]&0xff ) /4 ),1 );
//			OLEDLine ( x+1,0,x+1, ( temp[x] >> 8 ) /4,1 );
		}

		OLEDDraw();


		//memset(dest,0,128);

	}

	FpgaDisableSscDma();

}

void Draw_ADC_LOW_OLED ( void )
{
	uint16_t *dest = ( uint16_t * ) BigBuf+FREE_BUFFER_OFFSET;
	uint8_t v = 0;
	uint16_t r;
	int i;
	uint16_t p = 0;

	OLEDClear();

	OLEDPutstr ( "Draw_ADC_LOW_OLED" );
	OLEDDraw();

	// We're using this mode just so that I can test it out; the simulated
	// tag mode would work just as well and be simpler.
	//FpgaWriteConfWord ( FPGA_MAJOR_MODE_HF_READER_RX_XCORR | FPGA_HF_READER_RX_XCORR_848_KHZ | FPGA_HF_READER_RX_XCORR_SNOOP );

	FpgaWriteConfWord ( FPGA_MAJOR_MODE_LF_PASSTHRU );

	SetAdcMuxFor ( GPIO_MUXSEL_LORAW );

	__disable_irq();

	i = 0;

	HIGH(FPGA_DOUT);

	for ( ;; ) {

		OLEDClear();

		for ( unsigned char  x =  0 ; x < 127; x++ ) {

			
			//OLEDLine ( x,0,x, dest[x] ,1 );
			OLEDSetPixel ( x,ADC_Ampl[1]/16 ,1 );
			//LOW(FPGA_DOUT);
		}

		OLEDDraw();
	}

	FpgaDisableSscDma();

}
void Draw_ADC_HIGH_OLED ( void )
{
	uint16_t *dest = ( uint16_t * ) BigBuf+FREE_BUFFER_OFFSET;
	uint8_t v = 0;
	uint16_t r;
	int i;
	int p = 0;

	OLEDClear();

	OLEDPutstr ( "Draw_ADC_HIGH_OLED" );
	OLEDDraw();

	// We're using this mode just so that I can test it out; the simulated
	// tag mode would work just as well and be simpler.
	FpgaWriteConfWord ( FPGA_MAJOR_MODE_HF_READER_RX_XCORR | FPGA_HF_READER_RX_XCORR_848_KHZ | FPGA_HF_READER_RX_XCORR_SNOOP );

	// We need to listen to the high-frequency, peak-detected path.
	SetAdcMuxFor ( GPIO_MUXSEL_HIPKD );

	__disable_irq();
	
	i = 0;

	for ( ;; ) {

		OLEDClear();

		for ( int x =  0 ; x < 127; x++ ) {

			dest[x] = ReadAdc ( 1 )/3;
			//OLEDLine ( x,0,x, ( dest[x] ) ,1 );
			OLEDSetPixel ( x,dest[x]/2 ,1 );
		}

		OLEDDraw();
	}

	FpgaDisableSscDma();
}

int main()
{

  RCC_Configuration();
  
  SysTick_Config(SystemCoreClock / 1000);
  
  Set_System();
  
  // Startup OLED
  InitOLED();
  
  //OLEDTest(  );
  
  FpgaDownloadAndGo();
	
  SetupPCK0Clock();

  OLEDClear();
  OLEDPutstr ( "SYSTEM INIT\n" );
  OLEDPutstr ( "SYSTEM READY\n" );
  OLEDDraw();

  DelaymS ( 100 );

  OLEDDisplayPicture ( l1logo_inv );
  OLEDDraw();

  DelaymS ( 100 );
  
  while ( 1 ) {
	  //DrawHIPKD_OLED();
	  //Draw_ADC_HIGH_OLED();
	  Draw_ADC_LOW_OLED();
  }

  
  return 0;
}
