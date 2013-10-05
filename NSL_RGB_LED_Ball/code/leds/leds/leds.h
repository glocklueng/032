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


#define NUM_TLC5947 ( 3 )

#define bit_set(reg, bit) reg |= 1 << bit 
#define bit_clr(reg, bit) reg &= ~(1 << bit) 

#ifndef TWO_PI
#define TWO_PI                 ( 6.283185307f )
#endif

#define DATPIN		( PIN0_bp )    //    SIN
#define CLKPIN		( PIN1_bp )	   //    SCLK
#define BLANKPIN	( PIN2_bp )		// BLANK
#define LATPIN		( PIN3_bp )		// XLAT

#define VIO			(PIN0_bp )
#define VBAT_SW_EN	(PIN1_bp )

#define PORT_OUT	PORTD.OUT

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
void SetPoint( unsigned short x, unsigned short y,unsigned short val);

void WriteRGBArray(void);
void RGBscan(int red, int green, int blue, float degreeoffset);
void SetRGB( unsigned short red, unsigned short green , unsigned short blue );

#endif
