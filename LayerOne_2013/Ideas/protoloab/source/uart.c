/*------------------------------------------------*/
/* UART functions                                 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "xitoa.h"
#include "main.h"

#define	BAUD		115200

typedef struct _fifo {
	uint8_t	idx_w;      // write index
	uint8_t	idx_r;      // read index
	uint8_t	count;
	uint8_t buff[64];
} FIFO;

static volatile FIFO txfifo, rxfifo;

/* Initialize UART */
void uart_init() {
	rxfifo.idx_r = 0;
	rxfifo.idx_w = 0;
	rxfifo.count = 0;
	txfifo.idx_r = 0;
	txfifo.idx_w = 0;
	txfifo.count = 0;
	xfunc_out = (void (*)(char))uart_put;   // Join xitoa module to put_char
}

/* Get a received character */
uint8_t uart_test () {
	return rxfifo.count;
}

uint8_t uart_get () {
	uint8_t d, i;

	i = rxfifo.idx_r;
	while(rxfifo.count == 0);
	d = rxfifo.buff[i++];
	cli();
	rxfifo.count--;
	sei();
	if(i >= sizeof(rxfifo.buff))
		i = 0;
	rxfifo.idx_r = i;

	return d;
}

/* UART RXC interrupt */
ISR(USARTE0_RXC_vect) {
	uint8_t d, n, i;

	d = USARTE0.DATA;
	n = rxfifo.count;
	if(n < sizeof(rxfifo.buff)) {
		rxfifo.count = ++n;
		i = rxfifo.idx_w;
		rxfifo.buff[i++] = d;
		if(i >= sizeof(rxfifo.buff))
			i = 0;
		rxfifo.idx_w = i;
	}
}

/* Put a character to transmit */
void uart_put (uint8_t d) {
	uint8_t i;

	i = txfifo.idx_w;
    // Check if buffer is full, if so wait...
	while(txfifo.count >= sizeof(txfifo.buff));
	txfifo.buff[i++] = d;
	cli();
	txfifo.count++;
    USARTE0.CTRLA = USART_RXCINTLVL0_bm | USART_DREINTLVL0_bm;  // Enable TX interrupt
	//UCSR0B = _BV(RXEN0)|_BV(RXCIE0)|_BV(TXEN0)|_BV(UDRIE0);
	sei();
	if(i >= sizeof(txfifo.buff))
		i = 0;
	txfifo.idx_w = i;
}

/* UART UDRE interrupt */
ISR(USARTE0_DRE_vect) {
	uint8_t n, i;

	n = txfifo.count;
	if(n) {
		txfifo.count = --n;
		i = txfifo.idx_r;
		USARTE0.DATA = txfifo.buff[i++];
		if(i >= sizeof(txfifo.buff))
			i = 0;
		txfifo.idx_r = i;
	}
	if(n == 0) // All data sent, disable DREIF interrupt
        USARTE0.CTRLA = USART_RXCINTLVL0_bm;    // Only leave RX interrupt on
        //UCSR0B = _BV(RXEN0)|_BV(RXCIE0)|_BV(TXEN0);
}

