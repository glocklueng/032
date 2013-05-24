/*
 * leds.c
 *
 * Created: 5/23/2013 8:13:35 PM
 *  Author: charlie
 */ 

#include <avr/io.h>

#include "leds.h"


int main(void)
{
	
	// Power reduction: Stop unused peripherals
	PR.PRGEN = 0x18;        // Stop: AES, EBI
	PR.PRPC  = 0x74;        // Stop: TWI, USART0, USART1, HIRES
	PR.PRPD  = 0x7C;        // Stop: TWI, USART0, USART1, SPI, HIRES
	PR.PRPE  = 0x60;        // Stop: TWI, USART1, (XMEGA bug - HIRES cannot be turned off)
	PR.PRPF  = 0x78;        // Stop: TWI, USART0, USART1, SPI, (XMEGA bug - HIRES cannot be turned off)

	PORTD.DIR      = 0xFF;  // Leds
	PORTCFG.MPCMASK = 0xFF; // Configure all pins on PORTD the same way
	PORTD.PIN0CTRL = 0x18;  // Pull up on pin PortD (Keypad and K2-K5 buttons)
	PORTD.INTCTRL  = 0x00;  // PORTD will generate no low level interrupts
	PORTD.INT0MASK = 0x00;  
	PORTD.OUT      = 0x00;
  
  
    while(1) {
		
		PORTD.OUT ^= PORTD.OUT;
    }
}