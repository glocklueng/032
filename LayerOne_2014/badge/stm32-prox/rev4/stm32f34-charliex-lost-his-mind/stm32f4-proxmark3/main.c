#include "stm32f4_discovery.h"
 
void RCC_Configuration(void)
{
  /* --------------------------- System Clocks Configuration -----------------*/
  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  /* GPIOA clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}
  
/**************************************************************************************/
  
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /*-------------------------- GPIO Configuration ----------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Connect TIM1 pins to AF */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
}

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




int main()
{

  RCC_Configuration();
  
  GPIO_Configuration();
  
  SetupPCK0Clock();
  
  
  return 0;
}
