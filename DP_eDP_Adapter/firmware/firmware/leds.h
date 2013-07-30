#ifndef _RF_BLINKY_H_
#define _RF_BLINKY_H_ ( 1 )

#define F_CPU (8000000)

// includes

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

#include <math.h> 
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/pgmspace.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

// typedefs

typedef unsigned char uchar;
typedef unsigned int uint;

// structures

// defines
#define LEDS_HEIGHT ( 12 )
#define LEDS_WIDTH ( 6 )

#define NUM_TLC5947 (LEDS_HEIGHT * LEDS_WIDTH )

#define bit_set(reg, bit) reg |= 1 << bit 
#define bit_clr(reg, bit) reg &= ~(1 << bit) 

#ifndef TWO_PI
#define TWO_PI                 ( 6.283185307f )
#endif

#define DATPIN		( PB1 )    //    SIN
#define CLKPIN		( PB2 )
#define BLANKPIN	( PB3 ) 
#define LATPIN		( PB4 )

#define SIN_PIN     ( PB1 ) /// PC0
#define SCLK_PIN	( PB2 ) /// PC1
#define BLANK_PIN	( PB3 ) /// PC2
#define XLAT_PIN    ( PB4 ) /// PC3a

extern uint8_t LEDChannels[(NUM_TLC5947)];
extern unsigned char brightnessShift;

// prototypes
void Clear(unsigned char val);

// TLC
void WriteLEDArray(unsigned char count);
void LED_Init(void);
void WriteArrayOffset(unsigned int count, unsigned int offset);

#endif
