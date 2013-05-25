/****************************************************************************

XMEGA Oscilloscope and Development Kit

Gabotronics
February 2012

Copyright 2012 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#ifndef _DATA_H
#define _DATA_H

#include <stdint.h>
#include "main.h"

extern const char VERSION[];
extern const char Minustext[];
extern const char multinv[];
extern const char one_over_delta_T[];
extern const char delta_V[];
extern const char unitkHz[];
extern const char unitMHz[];
extern const char unitmV[];
extern const char unitV[];
extern const char Vdiv[];
extern const char Sdiv[];
extern const char Fone[];
extern const char Ftwo[];
extern int16_t EEMEM offset16CH1;
extern int16_t EEMEM offset16CH2;
extern int16_t EEMEM gain16CH1;
extern int16_t EEMEM gain16CH2;
extern int8_t  EEMEM offset8CH1[8][7];
extern int8_t  EEMEM offset8CH2[7][7];
extern const uint8_t FLGPIO[12];
extern uint8_t EEMEM EEGPIO[12];
extern const int8_t Hamming[128];
extern const int8_t Hann[128];
extern const int8_t Blackman[128];
extern const int8_t Exp[128];
extern int8_t EEMEM EEwave[256];
extern const unsigned char GABOTRONICS[];
extern const unsigned char LOGO[];
extern const unsigned char tian1[];
extern const unsigned char tian2[];
extern const unsigned char tian3[];
extern const unsigned char tian4[];
extern const unsigned char tian5[];
extern const unsigned char tian6[];
extern const unsigned char tian7[];
extern const unsigned char tian8[];
extern const unsigned char tian9[];
extern const unsigned char tian10[];
extern const unsigned char tian11[];
extern const unsigned char tian12[];
extern const unsigned char tian13[];
extern const unsigned char tian14[];
extern const unsigned char tian15[];
extern const unsigned char tian16[];

#endif
