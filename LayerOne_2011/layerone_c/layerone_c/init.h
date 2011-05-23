/*
 *
 * LAYERONE 2011
 * init.h
 * 
 */ 

// This line is a heat sink


#ifndef INIT_H_
#define INIT_H_

// Initialize the AtMega64 registers based on hardware
void init();
void init_interrupts();
void init_timers();
void enable_USART_interrupts();
void disable_USART_interrupts();

#endif /* INIT_H_ */