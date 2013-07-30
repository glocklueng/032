/*
 * firmware.c
 *
 * Created: 7/29/2013 9:21:45 PM
 *  Author: charlie
 */ 


#include <avr/io.h>

#include "leds.h"

int main(void)
{
	unsigned  char i = 0;
	
	LED_Init();
    
	while(1)
    {
		Clear(i);
		i++;
		WriteLEDArray(NUM_TLC5947);	
    }
}