/*
 *
 * LAYERONE 2011
 * init.c
 *
 * Initialize the AtMega64 registers based on hardware
 */ 

// This line is a heat sink

#include <avr/io.h>
#include <avr/interrupt.h> 
#include "init.h"

int cpu_frequency = 16000000;		// CPU Frequency - 16Mhz
int baud_rate = 31250;				// Baud Rate - 31250bps

void init()
{
	// Disable JTAG
	MCUCSR = 1 << JTD;	// Set jtag disable flag
	MCUCSR = 1 << JTD;	// Yes, you need to do this twice, as per the datasheet
			
	// Initialize Port A
	// PA7 PA6 PA5 PA4 PA3 PA2 PA1 PA0
	//  0	0	0	0	0	0	0	0	// Pull-Down Resistor Activated
	// In  In  In  In  In  In  In  In	// Pin Config
	PORTA = 0x00;	// PA0-7 - LOW - LEDs (These can later become outputs due to charlieplexing)
	DDRA = 0x00;	// PA0-7 - Inputs - LEDs (These can later become outputs due to charlieplexing)
	
	// Initialize Port B
	// PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0
	//  1	1	1	1	1	1	1	1	// Pull-Up Resistor Activated
	// In  In  In  In  In  In  In  In	// Pin Config
	PORTB = 0xFF;	// PB0-7 - HIGH   -	Buttons (COLS)
	DDRB = 0x00;	// PB0-7 - Inputs - Buttons (COLS)
	
	// Initialize Port C
	// PC7 PC6 PC5 PC4 PC3 PC2 PC1 PC0
	//  0	0	0	0	0	0	0	0	// PC7-PC0 Set Low
	// Out Out Out Out Out Out Out Out	// Pin Config
	PORTC = 0x00;	// PC0-7 - LOW - DAC
	DDRC = 0xFF;	// PC0-7 - Output - DAC
	
	// Initialize Port D
	// PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0
	//  1	1	1	1	1	1	0	0	// PD7-PD2 Set High , PD1-PD0 Set Low, PD0 Pull-Down Resistor
	// Out Out Out Out Out Out Out In	// Pin Config
	PORTD = 0xFC;	// PD1-7 - HIGH	  -	Buttons (ROWS) - LOW - Input (MIDI-IN)
	DDRD = 0xFE;	// PD1-7 - Output - Buttons (ROWS) - PD0 - Input (MIDI-IN)
	
	// Initialize Port E
	// PE7 PE6 PE5 PC4 PE3 PE2 PE1 PE0
	//  0	0	0	0	0	0	0	1	// PE7-PE1 Set Low , PE0 Set High
	// Out Out Out Out Out Out Out Out	// Pin Config 
	PORTE |= 0x01;	// PE1 - LED ON
	DDRE = 0xFF;	// PE0-7 - Output 
	
	// Initialize ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// Set ADC Prescaler to 128 - 125kHz Sample Rate @ 16MHz 
	ADCSRA |= (1 << ADEN);	// Enable ADC 
	ADCSRA |= (1 << ADSC);  // ADC Start Conversion
}


void init_interrupts() 
{
	//Turn on USART reception and | RX Interrupt
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0);

	//8-bit, 1 stop, Asynch.
	UCSR0C = (0 << UMSEL0) | (0 << UPM00) | (0 << USBS0) | (3 << UCSZ00);

	// Initialize UART
	//UBRR0H = (((cpu_frequency / (baud_rate * 16)) - 1) >> 8) & 0x08;
    //UBRR0L = ((cpu_frequency / (baud_rate * 16)) - 1) & 0x08;
	
	/* Set the baud rate to 31250 for MIDI */
	UBRR0L = 0x27;		// For 20MHz Clock
	//UBRR0L = 0x13;		// For 10MHz Clock

	/* Enable USART Receive interrupt */
	enable_USART_interrupts();
}


void init_timers() 
{
	
    /*----------------------------------------- 
    ; 
    ; Timer/Counter1 
    ; 
    -----------------------------------------*/ 
    //COM1A1 = 0 & COM1A0 = 1 => Toggle OC1A 
    //COM1B1 = 0 & COM1B0 = 0 => normal OC1B 
    //COM1C1 = 0 & COM1C0 = 0 => Normal OC1C 
    //WGM11 = 0 & WGM10 = 0 => CTC 
    TCCR1A = (0<<COM1A1)|(1<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<COM1C1)|(0<<COM1C0)|(0<<WGM11)|(0<<WGM10); 
    
    //WGM13 = 0 & WGM12 = 1 => CTC 
    //CS12 = 0 & CS11 = 0 & CS10 = 1 => Clki_o 
    TCCR1B = (0<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10); 
    
    //FOC1A = 1 => Force output compare A 
    //FOC1B = 0 => Force output compare B 
    //FOC1B = 0 => Force output compare B 
    TCCR1C = (1<<FOC1A)|(0<<FOC1B)|(0<<FOC1C); 
    
    //OCR1A = 6666;    => 600Hz 
    //OCR1A = 3333; => 1.2KHz 
    //OCR1A = 1666; => 2.4KHz <- 
    OCR1A = 8888; 

	//OCIE1A = 1 => Timer/Counter1, Output Compare A Match Interrupt Enable 
	TIMSK = (1<<OCIE1A); 

    //Clear Timer1 Interrupt flag 
    TIFR = (1<<OCF1A); 

	
	
	
	/*
	// Timer/Counter Interrupt Mask Register
	TIMSK = (1<<TOIE0);		// Enable Overflow interrupts for Timer 0
	TIMSK |= (1<<OCIE0);	// Enable A and B compare interrupts
	TIMSK |= (1<<OCIE1A);	// Enable output compare A match interrupt

	
	// Timer/Counter 1
	TCCR1A = 0x00;						// Normal counter operation
	TCCR1A |= (1<<WGM10);				// Fast PWM
	TCCR1B = (1<<CS12) | (1<<CS10);		// Divide by 1024 Prescalar
	TCCR1B |= (1<<WGM13);				// Prescaler 1, Fast PWM


	// Timer 0 - Start terminal count at zero  		
	TCNT0 = 0x00;
	
	// Timer 1 - Start count at zero now
	TCNT1H = 0x00;
	TCNT1L = 0x00;
	*/
}


// Enable UART
void enable_USART_interrupts() 
{
	UCSR0A = (1 << RXCIE0);
}


// Disable UART
void disable_USART_interrupts()
{
	UCSR0A = (0 << RXCIE0);
}