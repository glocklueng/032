/****************************************************************************

XMEGA Oscilloscope and Development Kit

Gabotronics
February 2012

Copyright 2012 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "mygccdef.h"
#include "asmutil.h"
#include "data.h"
#include "display.h"

// PORT DEFINITIONS
#define LEDGRN  4           // PORTD
#define LEDRED  0           // PORTD

#define ONGRN()     setbit(VPORT3.OUT, LEDGRN)
#define OFFGRN()    clrbit(VPORT3.OUT, LEDGRN)
#define ONRED()     setbit(VPORT3.OUT, LEDRED)
#define OFFRED()    clrbit(VPORT3.OUT, LEDRED)

// Global variables, using GPIO for optimized access
#define Srate       GPIO0   // Sampling rate
#define CH1ctrl     GPIO1   // CH1 controls
#define CH2ctrl     GPIO2   // CH2 controls
#define CHDctrl     GPIO3   // CHD controls 1
#define CHDmask     GPIO4   // Mask inputs
#define Trigger     GPIO5   // MSO Trigger
#define Mcursors    GPIO6   // MSO Cursors
#define Display     GPIO7   // Display options
#define MFFT        GPIO8   // MSO FFT Options and mode settings
#define Sweep       GPIO9   // Sweep Options
#define Sniffer     GPIOA   // Sniffer controls
#define MStatus     GPIOB   // MSO Trigger mode and cursors
#define Misc        GPIOC   // Miscellaneous bits
#define Index       GPIOD   // sample index
#define Menu        GPIOE   // Menu
#define Key         GPIOF   // Current key pressed

// CH1ctrl bits    (GPIO1)
// CH2ctrl bits    (GPIO2)
#define chon        0       // Channel on
#define x10         1       // x10 probe
#define bwlimit     2       // Bandwidth Limit
#define acdc        3       // AC/DC Select
#define chinvert    4       // Invert channel
#define chaverage   5       // Average samples
#define chmath      6       // math (Subtract or Multiply active)
#define submult     7       // Subtract or Multiply

// CHDctrl bits    (GPIO3)
#define chon        0       // Channel on
#define pull        1       // Pull input
#define pullup      2       // Pull up / down
#define low         3       // Thick line when logic '0'
#define chinvert    4       // Invert channel
#define hexs        5       // Serial Hex Display
#define hexp        6       // Parallel Hex Display
#define ascii       7       // Use ASCII with RS232

// Trigger bits    (GPIO5)
#define normal      0       // Normal trigger
#define single      1       // Single trigger
#define autotrg     2       // Auto trigger
#define trigdir     3       // trigger falling or rising
#define round       4       // Sniffer circular buffer
#define slope       5       // Trigger mode edge or slope
#define window      6       // Window trigger
#define edge        7       // Edge trigger
                            // Dual Edge trigger if others not set

// Mcursors bits    (GPIO6)
#define roll        0       // Roll scope on slow sampling rates
#define autocur     1       // Auto cursors
#define track       2       // Track vertical with horizontal
#define cursorh1    3       // CH1 Horizontal Cursor on
#define cursorh2    4       // CH2 Horizontal Cursor on
#define cursorv     5       // Vertical Cursor on
#define reference   6       // Reference waveforms on
#define singlesniff 7       // Stop when Sniffer buffer is full

// Display bits     (GPIO7) // Display options
#define grid0       0       // Grid settings (2 bits)
#define grid1       1
#define elastic     2       // Average on successive traces
#define disp_inv    3       // Invert display
#define flip        4       // Flip display
#define persistent  5       // Persistent Display
#define line        6       // Continuous Drawing
#define showset     7       // Show scope settings (time/div volts/div)

// MFFT bits        (GPIO8)
#define hamming     0       // FFT Hamming window
#define hann        1       // FFT Hann window
#define blackman    2       // FFT Blackman window
#define uselog      3       // Apply logarithm to FFT
#define iqfft       4       // Perform IQFFT
#define scopemode   5       // MSO mode
#define xymode      6       // XY mode
#define fftmode     7       // FFT mode
                            // Meter mode enabled when other modes are 0

// Sweep bits       (GPIO9)
#define SWAcceldir  0       // Acceleration direction
#define SWAccel     1       // Accelerate
#define swdown      2       // Sweep direction
#define pingpong    3       // Ping Pong Sweep
#define SweepF      4       // Sweep Frequency
#define SweepA      5       // Sweep Amplitude
#define SweepO      6       // Sweep Offset
#define SweepD      7       // Sweep Duty Cycle

// Sniffer bits    (GPIOA)
#define baud0       0       // UART Baud rate
#define baud1       1       // UART Baud rate
#define baud2       2       // UART Baud rate
#define CPOL        3       // SPI Clock Polarity
#define CPHA        4       // SPI Clock Phase
#define parmode     5       // Use UART parity
#define parity      6       // UART parity odd or even
#define stopbit     7       // UART 1 or 2 stop bits

// MStatus bits     (GPIOB)
#define update      0       // Update
#define updateawg   1       // Update AWG
#define updatemso   2       // Settings changed, need to call Apply() function
#define gosniffer   3       // Enter protocol sniffer
#define stop        4       // Scope stopped
#define triggered   5       // Scope triggered
#define vdc         6       // Calculate VDC
#define vp_p        7       // Calculate VPP

// Misc             (GPIOC) // Miscellaneous bits
#define keyrep      0       // Automatic key repeat
#define negative    1       // Print Negative font
#define bigfont     2       // Small or large font, this bit is normally 0
#define redraw      3       // Redraw screen
#define sacquired   4       // Data has been acquired (for slow sampling)
#define slowacq     5       // Acquired one set of samples a slow sampling rates
#define alternate   6       // Bit to alternate freq. measuring
#define autosend    7       // Continuously send data to UART

// Key              (GPIOF) // Key input
#define KA          0       // KA pressed
#define KB          1       // KB pressed
#define KC          2       // KC pressed
#define KD          3       // KD pressed
#define userinput   4       // User input received

void WaitDisplay(void);
uint8_t SP_ReadCalibrationByte(uint8_t location);	// Read out calibration byte.
void WaitRefresh(void);
void PowerDown(void);
char NibbleToChar(uint8_t nibble);  // Converts a nibble to the corresponding ASCII representing the HEX value
void printhex(uint8_t n);           // Prints a HEX number
uint8_t half(uint8_t number);
uint8_t twice(uint8_t number);

void CCPWrite( volatile uint8_t * address, uint8_t value );

extern uint8_t EEMEM EESleepTime;     // Sleep timeout in minutes

// Variables that need to be stored in NVM

void SaveEE(void);          // Save settings to EEPROM

#endif
