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

#include "user_board.h"
#include "cc3000.h"
#include "cc3000_hci.h"
#include "spi_driver.h"
#include "cc3000_wlan.h"

// typedefs

typedef unsigned char uchar;
typedef unsigned int uint;

// structures

// defines



#define bit_set(reg, bit) reg |= 1 << bit
#define bit_clr(reg, bit) reg &= ~(1 << bit)

#ifndef TWO_PI
#define TWO_PI                 ( 6.283185307f )
#endif



extern uint16_t LEDChannels[ ( NUM_TLC5947 )];
extern unsigned char brightnessShift;

// prototypes

void Clear ( unsigned short val );

// TLC
void WriteLEDArray ( unsigned int count );
void LEDscan ( int red, float degreeoffset,unsigned int count );
void LED_Init ( void );
void LEDscan2Add ( int red, float degreeoffset,unsigned int count );
void WriteArrayOffset ( unsigned int count, unsigned int offset );
void SetPoint ( unsigned short x, unsigned short y,unsigned short val );

void WriteRGBArray ( void );
void RGBscan ( int red, int green, int blue, float degreeoffset );
void SetRGB ( unsigned short red, unsigned short green , unsigned short blue );


void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length);
int wlan_Setup(void);
char *sendWLFWPatch(unsigned long *Length);
char *sendDriverPatch(unsigned long *Length);
char *sendBootLoaderPatch(unsigned long *Length);
void WlanInterruptEnable(void);
void WlanInterruptDisable(void);
long ReadWlanInterruptPin ( void );
void WriteWlanPin( unsigned char val );


#endif
