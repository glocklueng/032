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

#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>
#include <avr/eeprom.h>
#include "mygccdef.h"
#include "asmutil.h"
#include "data.h"
#include "sed1335.h"
#include "ff.h"

#define NOKEY   0
#define K1      1
#define K2      2
#define K3      3
#define K4      5
#define K5      6
#define K6      7
#define K7      9
#define K8      10
#define K9      11
#define KS      13
#define K0      14
#define KP      15
#define KA      17   // (ON Button)
#define KB      4
#define KC      8
#define KD      12
#define KE      16
#define KEnter  255
#define KBack   254

// PORT DEFINITIONS
#define ONBUTTON    3   // PORTQ
#define BUZZER1     2   // PORTC
#define BUZZER2     3   // PORTC
#define CHSEL       3   // PORTF
#define ADS931NOUT  6   // PORTH
#define ADS931POW   7   // PORTH
#define BACKLITE    0   // PORTF

#define ONRED()     PORTA.OUTSET = 0x20
#define OFFRED()    PORTA.OUTCLR = 0x20
#define TGLRED()    PORTA.OUTTGL = 0x20
#define ONGRN()     PORTA.OUTSET = 0x10
#define OFFGRN()    PORTA.OUTCLR = 0x10
#define TGLBKLT()   togglebit(VPORT3.OUT, BACKLITE)
#define ONBKLT()    setbit(VPORT3.OUT, BACKLITE)
#define OFFBKLT()   clrbit(VPORT3.OUT, BACKLITE)

#define SDWP		2	// Write protect switch (PF2)
#define SDCD		1	// Card detect switch (PE1)

// Global variables, using GPIO for optimized access
#define Signals GPIO0   // Signals
#define MStatus GPIO1   // MSO Status
#define Mcursor GPIO2   // MSO Cursors
#define MFFT    GPIO3   // MSO FFT Options
#define Mset    GPIO4   // MSO settings
#define srate   GPIO5   // Sampling rate
#define tdelay  GPIO6   // Trigger delay
#define tlevelo GPIO7   // Trigger level, offset adjusted
#define tsource GPIO8   // Trigger source
#define tpre    GPIO9   // Number of Pre Trigger samples
#define timeout GPIOA   // Trigger timeout
#define key     GPIOB   // Current key pressed
#define ROT1    GPIOC   // Rotary encoder 1 value
#define ROT2    GPIOD   // Rotary encoder 2 value
#define MAX1    GPIOE   // Rotary encoder 1 max value
#define MAX2    GPIOF   // Rotary encoder 2 max value

// Signal bits (GPIO0)
#define update      0       // Update
#define keydown     1       // A key is being pressed
#define redraw      2       // Redraw screen
#define userinput   3       // User input received
#define nextsamp    4       // Get next sample (used in slow sampling rates)
#define testing     5       // Testing
#define serialin    6       // Received command from serial port
#define acquired    7       // Data has been acquired (for slow sampling)


uint8_t prandom(void);
void sound(uint8_t tone);
int8_t Sin(uint8_t angle);
int8_t Cos(uint8_t angle);
uint8_t addwsat(uint8_t a, int8_t b);
uint16_t readVCC(void);
void ReadSerialRAM(uint16_t Addr, uint8_t *Data, uint16_t n);
void WriteSerialRAM(uint16_t Addr, uint8_t *Data, uint16_t n);
void CCPWrite( volatile uint8_t * address, uint8_t value );

extern uint8_t EEMEM EEapp;
extern uint8_t Letter[17];
extern uint8_t AWGBuffer[256];
extern uint8_t minutes;
extern uint8_t hours;
extern uint8_t oldkey;

int  SD(void);
void MSO(void);
void AWG(void);
void CHESS(void);
void CALC(void);

// Chess application variables
typedef union {
  long w;
  unsigned char c[4];
} TIMER;

extern volatile TIMER timer;       // wird alle 4.096ms erhoeht
extern FATFS fatfs;	               // File system object
extern FILINFO finfo;              // File info object

#endif
