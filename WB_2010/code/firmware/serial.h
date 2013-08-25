/**
 * \file serial.h
 *
 * Serial header file
 *
 */

#ifndef SERIAL_H_
#define SERIAL_H_
#include <ctype.h>

#if !defined(F_CPU)
        #error "No MCU crystal speed defined!"
#endif

extern void usart_init(void);
extern int pc_putc(char data);
extern int16_t pc_getc(void);
extern void pc_puts(char *s);
extern void pc_puts_P(const char *s);
extern void putnum_ud(uint16_t n);
extern void print_div(void);
extern uint16_t pc_read16(void);

#endif /* SERIAL_H_ */
