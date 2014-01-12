//-----------------------------------------------------------------------------
// Copyright (C) 2010 Hector Martin "marcan" <marcan@marcansoft.com>
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// Common *printf() functions
//-----------------------------------------------------------------------------

#ifndef __PRINTF_H
#define __PRINTF_H

#include <stdarg.h>

int kvsprintf(const char *format, void *arg, int radix, va_list ap);
int vsprintf(char *str, const char *format, va_list ap);
int sprintf(char *str, const char *format, ...);
void printF(uint8_t x, uint8_t y, int32_t Data,uint8_t bigfont);

#endif
