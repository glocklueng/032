#include <proxmark3.h>
#include "apps.h"

//Duty cycle value 0 is not permitted on SAM7S chips.
//Duty cycle value 1 is not permitted in left-aligned mode on SAM7S chips.
void PWMC_SetDutyCycle(BYTE channel, WORD duty)
{
    // If channel is disabled, write to CDTY
    if ((AT91C_BASE_PWMC->PWMC_SR & (1 << channel)) == 0) {
        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CDTYR = duty;
    }
    // Otherwise use update register
    else {
        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CMR &= (unsigned int)~AT91C_PWMC_CPD;
        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CUPDR = duty;
    }
}

void PWMC_SetPeriod(BYTE channel, WORD period)
{
    // If channel is disabled, write to CPRD
    if ((AT91C_BASE_PWMC->PWMC_SR & (1 << channel)) == 0) {
        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CPRDR = period;
    }
    // Otherwise use update register
    else {
        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CMR |= AT91C_PWMC_CPD;
        AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CUPDR = period;
    }
}

void PWMC_ConfigureChannel(BYTE channel, BYTE prescaler, BYTE alignment, BYTE polarity, DWORD mode)
{
  unsigned int count ;
	
loop:
   count = 100000;
   
  AT91C_BASE_PIOA->PIO_ASR = AT91C_PA1_PWM1;
	AT91C_BASE_PIOA->PIO_PDR = AT91C_PA1_PWM1;

	// Disable channel (effective at the end of the current period)
    if ((AT91C_BASE_PWMC->PWMC_SR & (1 << channel)) != 0) {
	  
        AT91C_BASE_PWMC->PWMC_DIS = (1<< channel);

        while ( (AT91C_BASE_PWMC->PWMC_SR & (1 << channel)) != 0 ) {
          
		  if( !(count--) ) 
			break;
          ;
		}
    }

    // Configure channel
    AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CMR = prescaler | alignment | polarity;

    // Configure clocks
    AT91C_BASE_PWMC->PWMC_MR = mode;
}

// frequency range 190Hz to 48Mhz
void PWMC_Beep(BYTE channel, DWORD frequency, DWORD duration)
{
	DWORD i = 31, result;

	result = 48000000/frequency;
        
	while ((i>7) && ((result>>i)==0)) 
          i--;

	LED_D_ON();
	// cannot synthesise required frequency
	if ( i > 19 )  {
	  
	  i = 10;
	LED_D_OFF();
	}
	
//	i=10;
    
	// create prescaler/divisor pair and pack it in required register format
	if (i>7) {
		i-=7;
		result = (i<<8) | (result>>i);
	}

	PWMC_ConfigureChannel(channel,AT91C_PWMC_CPRE_MCKA,0,0,result);
	PWMC_SetPeriod(channel, 5);
	PWMC_SetDutyCycle(channel, 10);
	
	PWMC_EnableChannel(channel);
	
  SpinDelay(duration);
  PWMC_DisableChannel(channel);
}
// frequency range 190Hz to 48Mhz
void PWMC_BeepN(BYTE channel, DWORD frequency)
{
	DWORD i = 31, result;

	result = 48000000/frequency;
        
	while ((i>7) && ((result>>i)==0)) 
          i--;

	// cannot synthesise required frequency
	if ( i > 19 )  {
	  
	  i = 10;
	}
	
//	i=10;
    
	// create prescaler/divisor pair and pack it in required register format
	if (i>7) {
		i-=7;
		result = (i<<8) | (result>>i);
	}

	PWMC_ConfigureChannel(channel,AT91C_PWMC_CPRE_MCKA,0,0,result);
	PWMC_SetPeriod(channel, 5);
	PWMC_SetDutyCycle(channel, 10);
	
	PWMC_EnableChannel(channel);
	
}
