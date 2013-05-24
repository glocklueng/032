#ifndef _RF_BLINKY_H_
#define _RF_BLINKY_H_ ( 1 )


#define F_CPU 16*1000
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

#define NUM_TLC5947 ( 2 )

#define bit_set(reg, bit) reg |= 1 << bit 
#define bit_clr(reg, bit) reg &= ~(1 << bit) 

#define SWSPI_PORT PORTD.DIR
#define SWSPI_DIR  PORTD.DDRD
#define SWSPI_IN   PORTD.IN

#ifndef TWO_PI
#define TWO_PI                 ( 6.283185307f )
#endif

#define SWSPI_SS   (1<<1) // (CSn of the SPI can also be connected to GND permanently.) 
#define SWSPI_SCK  (1<<2)  // CLK  SCL/SPC clock signal 
#define SWSPI_MOSI (1<<4)  // mosi sda/sdi/sdo master out,slave in PD2 
#define SWSPI_MISO (1<<4)  // miso SDO



#define DATPORT PORTC
#define CLKPORT PORTC //pc1
#define LATPORT PORTC
#define DATDDR DDRC

#define DATPIN ( 1<<3 )    //    SIN
#define CLKPIN ( 1<<2 )
#define BLANKPIN ( 1<<4 ) 
#define LATPIN ( 1<<5 )

#define SIN_PIN     ( 1<<3 ) /// PC0
#define SCLK_PIN	( 1<<2 ) /// PC1
#define BLANK_PIN	( 1<<4 ) /// PC2
#define XLAT_PIN    ( 1<<5 ) /// PC3a


extern uint16_t LEDChannels[(NUM_TLC5947)];
extern unsigned char brightnessShift;
extern int gCount;

// prototypes

void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
void SetPoint( unsigned short x, unsigned short y,unsigned short val);
void Clear(unsigned short val);
void Line( int x,int y,int x1,int y1);
void sleep_down( void );

// TLC
void WriteLEDArray(unsigned int count);
void LEDscan(int red, float degreeoffset,unsigned int count);
void LEDscan2(int red, float degreeoffset,unsigned int count);
void LEDscan3(int red, float degreeoffset,unsigned int count);
void LED_Init(void);
void LEDscan2Add(int red, float degreeoffset,unsigned int count);

//sw spi
void SWSPI_init();
void SWSPI_write( uint8_t address, uint8_t value );
uint8_t SWSPI_read( uint8_t address );

#endif
