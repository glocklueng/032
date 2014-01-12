//-----------------------------------------------------------------------------
// Jonathan Westhues, Mar 2006
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// Hardware and interface definitions
//-----------------------------------------------------------------------------

#ifndef __PROXMARK3_H
#define __PROXMARK3_H

// Might as well have the hardware-specific defines everywhere.
#include "stm32f10x_lib.h"
#include "hw_config.h"
#include "l1_board.h"

#define WDT_HIT()						

#define PWM_CH_MODE_PRESCALER(x)				((x)<<0)
#define PWM_CHANNEL(x)							(1<<(x))

// ADC Channels
//ampl_lo PA0/ADC0
#define ADC_CHAN_LF								0
//ampl_hi PA1/ADC1
#define ADC_CHAN_HF								1 

#define ADC_MODE_PRESCALE(x)					((x)<<8)
#define ADC_MODE_STARTUP_TIME(x)				((x)<<16)
#define ADC_MODE_SAMPLE_HOLD_TIME(x)			((x)<<24)
#define ADC_CHANNEL(x)							(1<<(x))
#define ADC_END_OF_CONVERSION(x)				(1<<(x))

#define SSC_CLOCK_MODE_START(x)					((x)<<8)
#define SSC_FRAME_MODE_WORDS_PER_TRANSFER(x)	((x)<<8)
#define SSC_CLOCK_MODE_SELECT(x)				((x)<<0)
#define SSC_FRAME_MODE_BITS_IN_WORD(x)			(((x)-1)<<0)

#define MC_FLASH_COMMAND_KEY					((0x5a)<<24)
#define MC_FLASH_MODE_FLASH_WAIT_STATES(x)		((x)<<8)
#define MC_FLASH_MODE_MASTER_CLK_IN_MHZ(x)		(((x)+((x)/2))<<16)
#define MC_FLASH_COMMAND_PAGEN(x)				((x)<<8)

#define RST_CONTROL_KEY							(0xa5<<24)

#define PMC_MAIN_OSC_STARTUP_DELAY(x)			((x)<<8)
#define PMC_PLL_DIVISOR(x)						(x)
#define PMC_PLL_MULTIPLIER(x)					(((x)-1)<<16)
#define PMC_PLL_COUNT_BEFORE_LOCK(x)			((x)<<8)
#define PMC_PLL_FREQUENCY_RANGE(x)				((x)<<14)
#define PMC_PLL_USB_DIVISOR(x)					((x)<<28)

#define UDP_INTERRUPT_ENDPOINT(x)				(1<<(x))
#define UDP_CSR_BYTES_RECEIVED(x)				(((x) >> 16) & 0x7ff)
//**************************************************************

#define LOW(x)	 GPIO_WriteBit(x##_PORT,x##_PIN,Bit_RESET)
#define HIGH(x)	 GPIO_WriteBit(x##_PORT,x##_PIN,Bit_SET)
#define GETBIT(x) GPIO_ReadInputDataBit(x##_PORT,x##_PIN)
#define SETBIT(x, y) (y) ? (HIGH(x)):(LOW(x))
#define INVBIT(x) SETBIT((x), !(GETBIT(x)))

#define SPI_FPGA_MODE	0
#define SPI_LCD_MODE	1

#define TRUE 1
#define FALSE 0

#include <usb_cmd.h>

//#define PACKED __attribute__((__packed__))

#define LED_A_ON()		HIGH(GPIO_LED_A)
#define LED_A_OFF()		LOW(GPIO_LED_A)
#define LED_A_INV()		INVBIT(GPIO_LED_A)
#define LED_B_ON()		HIGH(GPIO_LED_B)
#define LED_B_OFF()		LOW(GPIO_LED_B)
#define LED_B_INV()		INVBIT(GPIO_LED_B)
#define LED_C_ON()		HIGH(GPIO_LED_C)
#define LED_C_OFF()		LOW(GPIO_LED_C)
#define LED_C_INV()		INVBIT(GPIO_LED_C)
#define LED_D_ON()		HIGH(GPIO_LED_D)
#define LED_D_OFF()		LOW(GPIO_LED_D)
#define LED_D_INV()		INVBIT(GPIO_LED_D)
#define RELAY_ON()		HIGH(GPIO_RELAY)
#define RELAY_OFF()		LOW(GPIO_RELAY)
#define BUTTON_PRESS()		!GETBIT( SW_K2 )

#define VERSION_INFORMATION_MAGIC 0x56334d50
PACKED struct version_information {
	int magic; /* Magic sequence to identify this as a correct version information structure. Must be VERSION_INFORMATION_MAGIC */
	char versionversion; /* Must be 1 */
	char present; /* 1 if the version information could be created at compile time, otherwise 0 and the remaining fields (except for magic) are empty */
	char clean; /* 1: Tree was clean, no local changes. 0: Tree was unclean. 2: Couldn't be determined */
	char svnversion[9]; /* String with the SVN revision */
	char buildtime[30]; /* string with the build time */
} ;

#define COMMON_AREA_MAGIC 0x43334d50
#define COMMON_AREA_COMMAND_NONE 0
#define COMMON_AREA_COMMAND_ENTER_FLASH_MODE 1
PACKED struct common_area {
	int magic; /* Magic sequence, to distinguish against random uninitialized memory */
	char version; /* Must be 1 */
	char command;
	struct {
		unsigned int bootrom_present:1; /* Set when a bootrom that is capable of parsing the common area is present */
		unsigned int osimage_present:1; /* Set when a osimage that is capable of parsing the common area is present */
	} PACKED flags;
	int arg1, arg2;
} ;

#endif
