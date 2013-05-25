/****************************************************************************

XMEGA Oscilloscope and Development Kit

Gabotronics
February 2012

Copyright 2012 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#include <avr/pgmspace.h>
#include "main.h"

// Commonly used text
const char VERSION[]    PROGMEM = " LAYERONE 2013";

const unsigned char GABOTRONICS[] PROGMEM = {
    '\x7e','\xff','\x81','\x91','\x91','\xf1','\x60','\x00', // 0x0008
    '\x60','\xf4','\x94','\x94','\xfc','\xf8','\x00','\xff', // 0x0010
    '\xff','\x84','\x84','\xfc','\x78','\x00','\x78','\xfc', // 0x0018
    '\x84','\x84','\xfc','\x78','\x00','\x04','\x7f','\xff', // 0x0020
    '\x84','\x84','\x00','\xfc','\xfc','\x08','\x0c','\x0c', // 0x0028
    '\x00','\x78','\xfc','\x84','\x84','\xfc','\x78','\x00', // 0x0030
    '\xf8','\xfc','\x04','\x04','\xfc','\xf8','\x00','\xfd', // 0x0038
    '\xfd','\x00','\x78','\xfc','\x84','\x84','\x84','\x00', // 0x0040
    '\x98','\xbc','\xb4','\xf4','\x64',
};

