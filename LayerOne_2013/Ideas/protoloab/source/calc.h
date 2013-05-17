/*****************************************************************************

Xmegalab / XMultiKit:
XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
July 2010

Copyright 2009 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#ifndef _CALC_H
#define _CALC_H

// Shift definitions
#define NSHFT	0		// no shift
#define SHFT	1       // Shift
#define	ALFA	2       // Alpha Shift
#define LOCK	3       // Lock Alpha Shift
#define alfa    4       // alpha shift
#define lock    5       // alpha shift lock

void graph2d(char *expression);
void graph3d(char *expression);

#endif