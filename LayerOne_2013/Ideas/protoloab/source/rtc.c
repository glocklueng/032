/*--------------------------------------------------------------------------*/
/*  RTCLK controls                                                            */

#include <avr/io.h>
#include <string.h>
#include "rtc.h"
#include "main.h"

BOOL rtc_gettime (RTCLK *rtc) {
   //This code is just to provide some kind of a valid response.
   rtc->sec = RTC.CNT;
   rtc->min = minutes;
   rtc->hour = hours;
   rtc->mday = 27;
   rtc->month = 7;
   rtc->year = 2010;
   return TRUE; 
}

BOOL rtc_settime (const RTCLK *rtc) {
	BYTE buf[8];

	buf[0] = rtc->sec / 10 * 16 + rtc->sec % 10;
	buf[1] = rtc->min / 10 * 16 + rtc->min % 10;
	buf[2] = rtc->hour / 10 * 16 + rtc->hour % 10;
	buf[3] = rtc->wday & 7;
	buf[4] = rtc->mday / 10 * 16 + rtc->mday % 10;
	buf[5] = rtc->month / 10 * 16 + rtc->month % 10;
	buf[6] = (rtc->year - 2000) / 10 * 16 + (rtc->year - 2000) % 10;
	return TRUE;
}

