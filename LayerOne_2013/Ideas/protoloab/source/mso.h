/****************************************************************************

Xmegalab / XMultiKit:
XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
July 2010

Copyright 2010 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#ifndef _MSO_H
#define _MSO_H

#include <stdint.h>
#include <avr/eeprom.h>
#include "ffft.h"

// MStatus bits     (GPIO1)
#define freetrig    0       // Free trigger
#define normal      1       // Normal trigger
#define single      2       // Single trigger
#define autotrg     3       // Auto trigger
#define trigger     4       // oscilloscope triggered
#define stop        5       // Scope stopped
#define trigdir     6       // trigger falling or rising
#define acquired    7       // Data has been acquired

// Mcursor bits     (GPIO2)
#define reference   0       // Reference waveforms on
#define cursorh1    1       // CH1 Horizontal Cursor on
#define cursorh2    2       // CH2 Horizontal Cursor on
#define cursorv     3       // Vertical Cursor on
#define lockcur     4       // Lock Automatic Cursors
#define rotv        5       // Rotary encoder on vertical cursor
#define roth        6       // Rotary encoder on horizontal cursor ch1 or ch2 (if not rotv)

// MFFT bits        (GPIO3)
#define hamming     0       // Hamming window
#define hann        1       // Hann window
#define cosine      2       // Cosine window
#define triangle    3       // Triangular window
#define uselog      4       // Apply logarithm to FFT
#define vac         5       // Calculate VAC
#define vrms        6       // Calculate RMS
#define autoset     7       // Autoset MSO

// Mset bits        (GPIO4)
#define backlight   0       // Backlight on/off
#define persistent  1       // Persistent Display
#define line        2       // Continous Drawing
#define showset     3       // Show scope settings ( time/div volts/div)
#define scope       4       // MSO mode
#define meter       5       // Multimeter mode
#define fft         6       // FFT mode
#define xymode      7       // X-Y mode

typedef struct {
    int8_t  position;           // Channel position
    int8_t  offset;             // Channel offset
    uint8_t max;                // Maximum value
    uint8_t min;                // Minimum value
    uint8_t vpp;                // Peak to peak
    uint8_t data[256];          // Samples
    uint8_t fftm[128];          // FFT magnitude data
    uint8_t f;                  // Maximum frequency
    union {
        uint8_t option;         // All options in a byte
        struct {
            uint8_t gain:3;     // Channel gain
            uint8_t on:1;       // Channel on
            uint8_t x10:1;      // x10 probe
            uint8_t invert:1;   // Invert channel
            uint8_t average:1;  // Average samples
            uint8_t math:1;     // math (CH1+CH2 for CH1, CH2*CH2 for CH2)
        } ctrl;
    };
} ACHANNEL;

typedef struct {
    int8_t  position;           // Channel position
    uint8_t data[256];          // Samples
    union {
        uint8_t option;         // All options in a byte
        struct {
            uint8_t gain:3;     // Line Size
            uint8_t on:1;       // Channel on
            uint8_t invert:1;   // Invert channel
            uint8_t low:1;      // Thick line when logic '0'
            uint8_t hexs:1;     // Serial Hex Display
            uint8_t hexp:1;     // Parallel Hex Display
        } ctrl;
    };
} DCHANNEL;

extern ACHANNEL CH1, CH2;       // Analog Channel 1, Channel 2
extern DCHANNEL CHD;            // Digital Channel

// Function prototypes
void ShowCursorV(void);                 // Display Vertical Cursor
// Display Horizontal Cursor
void ShowCursorH(uint8_t gain, uint8_t HcursorA, uint8_t HcursorB, int8_t CHPos);
void MenuHelp(void);                    // Display Keypad Menu Help
void LoadEE(void);                      // Load settings from EEPROM
void SaveEE(void);                      // Save settings to EEPROM
void Apply(void);                       // Apply oscilloscope settings

extern signed char EEMEM offsetsCH1[8];
extern signed char EEMEM offsetsCH2[8];
extern const uint32_t timeval[24];   // = Time division * 10000 / 16
extern const uint32_t milivolts[8];
extern uint8_t VcursorA;            // Vertical cursor A
extern uint8_t VcursorB;            // Vertical cursor B
extern uint8_t Hcursor1A;           // CH1 Horizontal cursor A
extern uint8_t Hcursor1B;           // CH1 Horizontal cursor B
extern uint8_t Hcursor2A;           // CH2 Horizontal cursor A
extern uint8_t Hcursor2B;           // CH2 Horizontal cursor B
extern uint8_t menu;                // Current menu
extern uint8_t tlevel;              // Trigger level
extern uint8_t HPos;                // Horizontal Position

#endif
