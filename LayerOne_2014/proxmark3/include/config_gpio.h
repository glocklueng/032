//-----------------------------------------------------------------------------
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// GPIO pin mapping for the Proxmark3
//-----------------------------------------------------------------------------

#ifndef __CONFIG_GPIO_H
#define __CONFIG_GPIO_H

#ifdef WITH_OLED
#define GPIO_LED_A		AT91C_PIO_PA0
#define GPIO_LED_B		AT91C_PIO_PA0
#define GPIO_LED_C		AT91C_PIO_PA0
#define GPIO_LED_D		AT91C_PIO_PA0
#else
#endif
#define GPIO_FPGA_NINIT		AT91C_PIO_PA4
#define GPIO_SPEAKER		AT91C_PIO_PA5
#define GPIO_PCK0		AT91C_PA6_PCK0
#ifdef WITH_LCD

#define GPIO_LRST		AT91C_PIO_PA7
#endif

#ifdef WITH_OLED
#define GPIO_DC			AT91C_PIO_PA7

#define LCD_SCLK			AT91C_PIO_PA10
#define LCD_CS				AT91C_PIO_PA1
#define LCD_SDIN			AT91C_PIO_PA8 // kynar
#define LCD_DC				AT91C_PIO_PA7
#define LCD_RES				AT91C_PIO_PA3
#define SPEAKER				AT91C_PIO_PA5

#else
#endif

#define GPIO_NCS2		AT91C_PA10_NPCS2
#define GPIO_NCS0		AT91C_PA11_NPCS0

#define GPIO_MISO		AT91C_PA12_MISO
#define GPIO_MOSI		AT91C_PA13_MOSI
#define GPIO_SPCK		AT91C_PA14_SPCK
#define GPIO_SSC_FRAME		AT91C_PA15_TF
#define GPIO_SSC_CLK		AT91C_PA16_TK
#define GPIO_SSC_DOUT		AT91C_PA17_TD
#define GPIO_SSC_DIN		AT91C_PA18_RD
#define GPIO_MUXSEL_HIPKD	AT91C_PIO_PA19
#define GPIO_MUXSEL_LOPKD	AT91C_PIO_PA20
#define GPIO_MUXSEL_HIRAW	AT91C_PIO_PA21
#define GPIO_MUXSEL_LORAW	AT91C_PIO_PA22
#define GPIO_BUTTON		AT91C_PIO_PA23
//#define GPIO_BUTTON_B		AT91C_PIO
#define GPIO_USB_PU		AT91C_PIO_PA24
#define GPIO_RELAY		AT91C_PIO_PA25
#define GPIO_FPGA_ON		AT91C_PIO_PA26
#define GPIO_FPGA_DONE		AT91C_PIO_PA27
#define GPIO_FPGA_NPROGRAM	AT91C_PIO_PA28
#define GPIO_FPGA_CCLK		AT91C_PIO_PA29
#define GPIO_FPGA_DIN		AT91C_PIO_PA30
#define GPIO_FPGA_DOUT		AT91C_PIO_PA31

// SD-CARD
#define GPIO_SD_CS		AT91C_PIO_PA2			// !CS_SD
#define GPIO_SD_MOSI		AT91C_PIO_PA8			// SD_MOSI_DO
#define GPIO_SD_MISO		AT91C_PIO_PA9			// MISO_SDOUT_SD
#define GPIO_SD_SCLK		AT91C_PIO_PA10			// SCLK_OLED_SD

#endif
