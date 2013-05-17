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

#ifndef _AWG_H
#define _AWG_H

void increaseF(void);
void decreaseF(void);
void LoadAWGvars(void);
void SaveAWGvars(void);
void BuildWave(void);

// Global AWG variable
extern int8_t   AWGAmp;         // Amplitude range: [0,127]
extern uint8_t  AWGtype;        // Waveform type
extern uint16_t AWGduty;        // Duty cycle range: [0,511]
extern uint32_t desiredF;		// Desired frequency
extern int8_t 	AWGOffset;		// Offset

#endif
