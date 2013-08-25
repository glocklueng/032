/**
 * \file serial.c
 *
 * USART and serial I/O functions
 *
 */
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include <string.h>
#include "main.h"
#include "serial.h"

/**
 * Init USART
 *
 * Initialize USART for communication with PC.
 *
*/
void usart_init(void) 
{
	#undef BAUD
	  #define BAUD 19200
	  #include <util/setbaud.h>
	  UBRR0H = UBRRH_VALUE;
	  UBRR0L = UBRRL_VALUE;

#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));	
#endif

  // config 8N1
  UCSR0C = _BV(UCSZ01)|_BV(UCSZ00);

  // enable Rx & Tx
  UCSR0B = _BV(RXEN0)|_BV(TXEN0);
  
}


/**
 * Send one byte to PC
 *
 * \param       data    Byte to send
 *
*/
int pc_putc(char data) {
  // wait for USART to become available
	loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
  UDR0 = data;    // send character
  return 0;
}


/**
 * Send RAM string to PC
 *
 * \param       s       String to send
 *
*/
void pc_puts(char *s) {
  while(*s) pc_putc(*s++);   // send string char by char
}


/**
 * Send flash string to PC
 *
 * \param       s       String to send
 *
*/
void pc_puts_P(const char *s) {
  // send string char by char
  while(pgm_read_byte(&*s)) pc_putc(pgm_read_byte(&*s++));
};

/**
 * Get one byte from PC
 *
 * \return      Received byte
 *
*/
int16_t pc_getc_timeout(uint16_t max_delay)
{
	// poll for data available, with timeout
	while ( (UCSR0A & _BV(RXC0)) == 0  && max_delay != 0) {
		max_delay--;
	}
	if ( (UCSR0A & _BV(RXC0)) == 0  ) {
		return -1;
	}
	return UDR0;
}

int16_t pc_getc(void)
{
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}

/**
 * Send unsigned int number in ASCII to PC
 *
 * \param       n       Number to send
 *
*/
void putnum_ud(uint16_t n) {
  uint8_t cnt=0, flag=0;

  while (n >= 10000UL) { flag = 1; cnt++; n -= 10000UL; }
  if (flag) pc_putc('0'+cnt);
  cnt = 0;
  while (n >= 1000UL) { flag = 1; cnt++; n -= 1000UL; }
  if (flag) pc_putc('0'+cnt);
  cnt = 0;
  while (n >= 100UL) { flag = 1; cnt++; n -= 100UL; }
  if (flag) pc_putc('0'+cnt);
  cnt = 0;
  while (n >= 10UL) { flag = 1; cnt++; n -= 10UL; }
  if (flag) pc_putc('0'+cnt);
  cnt = 0;
  pc_putc('0'+n);
  return;
}

/**
 * Print divider on terminal
 *
*/
void print_div(void) {
       pc_puts_P(PSTR("---------------------------------\n"));
};

/**
 * Get unisgned int from PC
 *
 * \return      Received number
 *
*/
uint16_t pc_read16(void) {
  uint8_t c;
  uint16_t t=0;
  while ( (c = pc_getc()) != '\n') {
    if (c == '\r') break;
    if ((c  > '9') || (c < '0'))
            continue;
    pc_putc(c);
    t *= 10;
    t += c-'0';
  }
  pc_putc(c);
  return t;
}
