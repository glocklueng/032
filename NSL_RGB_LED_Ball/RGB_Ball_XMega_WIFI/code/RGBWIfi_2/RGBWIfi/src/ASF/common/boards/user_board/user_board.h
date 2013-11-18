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

#ifndef NULL
#	define NULL ( 0) 
#endif

#define debug_str 
//__PROLOG 
#define debug_var(x,y)
#define debug_nl()
//	__PROLOG("\n")
#define debug_int_hex_16bit(x)
#define debug_int(x)
#define debug_putc(x)	
//circular_buffer_put(x)
#define debug_int_hex(x)

typedef unsigned char uint8_t;
typedef unsigned long uint32_t;
typedef signed char int8_t;
typedef long  int32_t;

#include "cc3000_general.h"

void timer(void );

#define NUM_TLC5947 ( 3 )

#define bit_set(reg, x) reg |= 1 << x
#define bit_clr(reg, x) reg &= ~(1 << x)

#define DATPIN		( PIN0_bp )    //    SIN
#define CLKPIN		( PIN1_bp )		//    SCLK
#define BLANKPIN	( PIN2_bp )		// BLANK
#define LATPIN		( PIN3_bp )		// XLAT
#define PORT_OUT	PORTD.OUT

// CC3000 WLAN IRQ pin

/*
PORTC.IN
	01234567
	11100011
PORTC.OUT
	01234567
	11000000
PORTC.DIR
	01234567
	11001101
PORTC.PIN6CTRL
	01234567
	00100000
PORTC.INTCTRL
	01234567
	10000000
PORTC.INT0MASK
	01234567
	00100000
PORTC.INT1MASK
	01234567
	00000000
*/

#define VIO				(PIN0_bp )
#define VIO_bm			(PIN0_bm )
#define VIO_bp			(PIN0_bp )

#define VBAT_SW_EN		(PIN1_bp )
#define VBAT_SW_EN_bm	(PIN1_bm )
#define VBAT_SW_EN_bp	(PIN1_bp )

#define WLAN_SPI_IRQ_bm	(PIN2_bm)	// SPI_IRQ (input)
#define WLAN_SPI_IRQ_bp	(PIN2_bp)	// SPI_IRQ (input)
#define WLAN_SS			(PIN4_bp)	// SPI_CS(output)
#define WLAN_SS_bm		(PIN4_bm)	// SPI_CS (output)
#define WLAN_MISO		(PIN6_bp)	// SPI_DOUT (input)
#define WLAN_MISO_bm	(PIN6_bm)	// SPI_DOUT (input)
#define WLAN_MOSI		(PIN5_bp)	// SPI_DIN(output)
#define WLAN_MOSI_bm	(PIN5_bm)	// SPI_DIN(output)
#define WLAN_SCK		(PIN7_bp)	// SPI_CLK(output)
#define WLAN_SCK_bm		(PIN7_bm)	// SPI_CLK(output)


#define SPI_SS_bm             WLAN_SS_bm		/*!< \brief Bit mask for the SS pin. */
#define SPI_MOSI_bm           WLAN_MOSI_bm		/*!< \brief Bit mask for the MOSI pin. */
#define SPI_MISO_bm           WLAN_MISO_bm		/*!< \brief Bit mask for the MISO pin. */
#define SPI_SCK_bm            WLAN_SCK_bm	/*!< \brief Bit mask for the SCK pin. */
#define SPI_IRQ_bm			  WLAN_SPI_IRQ_bm	/*!< \brief Bit mask for the SPI_IRQ pin/PC2 */


#define cc3000_spi_port	1

#define cc3000_cs_port	PORTC
#define cc3000_cs_pin	PIN4_bm

#define cc3000_int_port PORTC
#define cc3000_int_pin	PIN2_bm

#define cc3000_en_port PORTC
#define cc3000_en_pin  PIN1_bm

#define		set_pin(port,pin) { port.OUTSET=pin;} 
#define		make_output(port,pin)  { port.DIRSET = pin;}
#define		make_input(port,pin) { port.DIRCLR = pin;} 
#define		clear_pin(port,pin) { port.OUTCLR = pin;} 
#define		test_pin(port,pin)  (port.IN & pin)
	
#define clear_bit(port,pin)	{port.OUTCLR = pin;}
#define set_bit(port,pin)	{port.OUTSET = pin;}

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
int circular_buffer_puts ( char *str );
int circular_buffer_get ( unsigned char * const pc );
void circular_buffer_init(void);
int circular_empty(void);

int init_spi(void);

#ifndef __PROLOG
#define __PROLOG(X)		circular_buffer_puts(X)
#endif
#endif // USER_BOARD_H
