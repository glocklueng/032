/****************************************************************************

XMEGA Oscilloscope and Development Kit

Gabotronics
February 2012

Copyright 2012 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#ifndef _AWG_H
#define _AWG_H

void moveF(void);
void LoadAWGvars(void);
void SaveAWGvars(void);
void BuildWave(void);

// Global AWG variable
extern uint8_t  AWGBuffer[256];
extern uint8_t  cycles;     // Cycles in AWG buffer

#endif
