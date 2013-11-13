/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H
#include <avr/io.h>
#include <util/delay.h>

#include <conf_board.h>

#include "cc3000_common.h"

void timer(void );

#define NUM_TLC5947 ( 1 )

#define bit_set(reg, bit) reg |= 1 << bit
#define bit_clr(reg, bit) reg &= ~(1 << bit)

#define DATPIN		( PIN0_bp )    //    SIN
#define CLKPIN		( PIN1_bp )		//    SCLK
#define BLANKPIN	( PIN2_bp )		// BLANK
#define LATPIN		( PIN3_bp )		// XLAT
#define PORT_OUT	PORTD.OUT

// CC3000 WLAN IRQ pin
#define VIO				(PIN0_bp )
#define VIO_bm			(PIN0_bm )
#define VIO_bp			(PIN0_bp )

#define VBAT_SW_EN		(PIN1_bp )
#define VBAT_SW_EN_bm	(PIN1_bm )
#define VBAT_SW_EN_bp	(PIN1_bp )

#define WLAN_SPI_IRQ_bm		(PIN2_bm)	// SPI_IRQ (input)
#define WLAN_SPI_IRQ_bp		(PIN2_bp)	// SPI_IRQ (input)
#define WLAN_SS			(PIN4_bp)	// SPI_CS(output)
#define WLAN_SS_bm		(PIN4_bm)	// SPI_CS (output)
#define WLAN_MISO		(PIN6_bp)	// SPI_DOUT (input)
#define WLAN_MISO_bm	(PIN6_bm)	// SPI_DOUT (input)
#define WLAN_MOSI		(PIN5_bp)	// SPI_DIN(output)
#define WLAN_MOSI_bm	(PIN5_bm)	// SPI_DIN(output)
#define WLAN_SCK		(PIN7_bp)	// SPI_CLK(output)
#define WLAN_SCK_bm		(PIN7_bm)	// SPI_CLK(output)


// External oscillator settings.
// Uncomment and set correct values if external oscillator is used.

// External oscillator frequency
//#define BOARD_XOSC_HZ          8000000

// External oscillator type.
//!< External clock signal
//#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
//!< 32.768 kHz resonator on TOSC
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//!< 0.4 to 16 MHz resonator on XTALS
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

// External oscillator startup time
//#define BOARD_XOSC_STARTUP_US  500000
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


void dump_register(const unsigned char *str, register8_t port );
int circular_buffer_put ( unsigned char c );
int circular_buffer_puts ( const unsigned char *str );
int init_spi(void);
long ReadWlanInterruptPin ( void );

#ifndef __PROLOG
#define __PROLOG(X)		circular_buffer_puts(X)
#endif
#endif // USER_BOARD_H
