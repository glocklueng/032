/****************************************************************************

Xmegalab / XMultiKit:
XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
July 2010

Copyright 2009 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#ifndef _ASMUTIL_H
#define _ASMUTIL_H

signed char fmuls_8(signed char a, signed char b);
fixed fixmult(fixed x,fixed y);

void trigup(void);
void trigdown(void);
void trigcompa(void);
void trigext(void);
void trigpb4(void);
void trigpb5(void);
void trigpb6(void);
void trigpb7(void);
void acquire1(unsigned char *);
void acquire2(unsigned char *, unsigned char *);
void acquire3(unsigned char *, unsigned char *);
void acquire4(unsigned char *, unsigned char *, unsigned char *);
void acquire5(unsigned char *, unsigned char *, unsigned char *);

#endif
