/****************************************************************************

XMEGA Oscilloscope and Development Kit

Gabotronics
March 2012

Copyright 2012 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#ifndef _ASMUTIL_H
#define _ASMUTIL_H

uint8_t addwsat(uint8_t a, int8_t b);
uint8_t saddwsat(int8_t a, int8_t b);
void    windowCH1(uint8_t w1, uint8_t w2);
void    windowCH2(uint8_t w1, uint8_t w2);
void    slopedownCH1(unsigned char);
void    slopeupCH1(unsigned char);
void    slopedownCH2(unsigned char);
void    slopeupCH2(unsigned char);
void    trigdownCH1(unsigned char);
void    trigupCH1(unsigned char);
void    trigdownCH2(unsigned char);
void    trigupCH2(unsigned char);
void    trigdownCHD(unsigned char);
void    trigupCHD(unsigned char);

#endif
