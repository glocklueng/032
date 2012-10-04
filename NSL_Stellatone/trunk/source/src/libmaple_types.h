#ifndef LIB_MAPLE_H_
#define LIB_MAPLE_H_ (1)

#include <stdlib.h>

#define PREENFM_VERSION "1.0"

typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

typedef char int8;
typedef short int16;
typedef long int32;

typedef unsigned char boolean;


#ifndef NULL
#define NULL (0)
#endif

//#define INLINE __attribute__((always_inline))
#define INLINE
#define __FLASH__

//#define __USER_FLASH__ __attribute__ ((section (".USER_FLASH")))

#define __USER_FLASH__

//#define __CONSTRUCTOR__ __attribute__(( constructor )) 
#define __CONSTRUCTOR__ 



#define	INPUT	( 0 )
#define	OUTPUT	( 0 )
#define __io
#define gpio_dev int *
#define rcc_clk_id int
#define nvic_irq_num int

#define PCB_R4

#define		BIT(x)	(1<<x)

#define	HIGH	(1)
#define	LOW		(0)


/*
	macros.h:
	Binary constant generator macro
	By Tom Torfs - donated to the public domain
 */

/* All macro's evaluate to compile-time constants */

/* *** helper macros *** */

/* turn a numeric literal into a hex constant
	(avoids problems with leading zeroes)
	8-bit constants max value 0x11111111, always fits in unsigned long
 */ 
#define HEX__(n) 0x##n##LU

/* 8-bit conversion function */
#define B8__(x) ((x&0x0000000FLU)?1:0) \
				+((x&0x000000F0LU)?2:0) \
				+((x&0x00000F00LU)?4:0) \
				+((x&0x0000F000LU)?8:0) \
				+((x&0x000F0000LU)?16:0) \
				+((x&0x00F00000LU)?32:0) \
				+((x&0x0F000000LU)?64:0) \
				+((x&0xF0000000LU)?128:0)

/* *** user macros *** */

/* for upto 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* for upto 16-bit binary constants, MSB first */
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8 \
+ B8(dlsb))

/* for upto 32-bit binary constants, MSB first */
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
								+ ((unsigned long)B8(db2)<<16) \
								+ ((unsigned long)B8(db3)<<8 \
								+ B8(dlsb))

#define and &&

#define NVIC_USB_LP_CAN_RX0 (0)

#define TIMER_CH1 (1)
#define TIMER_CH2 (2)
#define TIMER_OUTPUTCOMPARE ( 1)

/* Sample usage:
B8(01010101) = 85
B16(10101010,01010101) = 43605
B32(10000000,11111111,10101010,01010101) = 2164238933
*/

#define MSBFIRST (1)
#define SPI_18MHZ (2)

void delay(int);

void nvic_irq_disable(int);


class Serial
{
public:

	void begin(int baud){
	};

	unsigned char read(void){
		return 0xaa;
	};

	int print(unsigned char ch )
	{
		return 0;
	}

	boolean available( void )
	{
		return 0;
	}

	void end(void)
	{
	}

};

extern Serial Serial1;
extern Serial Serial2;
extern Serial Serial3;
extern Serial SerialUSB;


class HardwareSPI {
public:
	HardwareSPI(int count){
	};

	int write(uint8 *ptr, int len){
		return len;
	}

	void begin(int,int,int)
	{
	}
};

class HardwareTimer {
public:
	HardwareTimer(int count)
	{

	}

	void pause(void)
	{

	}

	void setOverflow( int val)
	{
	}

	void setPrescaleFactor(int val)
	{
	}

	void refresh()
	{
	}

	void resume()
	{
	}

	void setMode(int mode,int mode1)
	{
	}

	void attachInterrupt(int, void*)
	{
	}

	void setCompare(int a,int b)
	{
	}
};

void delayMicroseconds(long ms);
long micros(void);
void pinMode(int,int);
void digitalWrite(int,int);
unsigned char digitalRead(int);
void togglePin(int);
void delay(int);


#endif
