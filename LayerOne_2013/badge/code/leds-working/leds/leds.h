#ifndef _RF_BLINKY_H_
#define _RF_BLINKY_H_ ( 1 )


#define __AVR_ATxmega32A4__

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
#define LEDS_HEIGHT ( 48 )
#define LEDS_WIDTH ( 1 )

#define NUM_TLC5947 ( 48 )

#define bit_set(reg, bit) reg |= 1 << bit 
#define bit_clr(reg, bit) reg &= ~(1 << bit) 

#ifndef TWO_PI
#define TWO_PI                 ( 6.283185307f )
#endif


#define DATPIN		( PIN0_bp )    //    SIN
#define CLKPIN		( PIN2_bp )
#define BLANKPIN	( PIN4_bp ) 
#define LATPIN		( PIN5_bp )

#define SIN_PIN     ( PIN0_bm ) /// PC0
#define SCLK_PIN	( PIN2_bm ) /// PC1
#define BLANK_PIN	( PIN4_bm ) /// PC2
#define XLAT_PIN    ( PIN5_bm ) /// PC3a


extern uint16_t LEDChannels[(NUM_TLC5947)];
extern unsigned char brightnessShift;


// prototypes


void Clear(unsigned short val);

// TLC
void WriteLEDArray(unsigned int count);
void LEDscan(int red, float degreeoffset,unsigned int count);
void LEDscan2(int red, float degreeoffset,unsigned int count);
void LEDscan3(int red, float degreeoffset,unsigned int count);
void LED_Init(void);
void LEDscan2Add(int red, float degreeoffset,unsigned int count);
void WriteArrayOffset(unsigned int count, unsigned int offset);

#endif
