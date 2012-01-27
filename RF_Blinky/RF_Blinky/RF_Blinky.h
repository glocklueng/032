#ifndef _RF_BLINKY_H_
#define _RF_BLINKY_H_ ( 1 )



// includes

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

#include <stdlib.h>
#include <string.h>

#include <math.h>

// typedefs

typedef unsigned char uchar;
typedef unsigned int uint;

// structures

struct{

	uchar fUpdate:1;		// Update configuration parameters Flag
	uchar fRTESTx:1;		// Range test initiator on=1/off=0 Flag
	uchar fRTESTRx:1;		// Range test receive on=1/off=0 Flag
	uchar fTermAct:1;		// Terminal active flag
	uchar fFlshClr:1;		// Clear flash flag
	uchar fTermDat:1;		// Terminal Data active flag
	uchar fWaitforAck:1;	// Wait for Ack/nAck flag - this flag keeps from Ack'ing and Ack
	uchar fRTMD:1;			// Range Test Mode active
} status;


// defines

#define	fUpdate		status.fUpdate	// Update configuration parameters Flag
#define fRTESTx		status.fRTESTx		// Range test initiator on=1/off=0 Flag
#define fRTESTRx 	status.fRTESTRx		// Range test receive on=1/off=0 Flag
#define fTermAct 	status.fTermAct		// Terminal active flag
//#define fFlshClr 	status.fFlshClr		// Clear flash flag
#define fTermDat 	status.fTermDat		// Terminal Data active flag
#define fWaitforAck status.fWaitforAck	// Wait for Ack/nAck flag - this flag keeps from Ack'ing an Ack
#define fRTMD		status.fRTMD		// Range Test Mode Active flag


#define bit_set(reg, bit) reg |= 1 << bit 
#define bit_clr(reg, bit) reg &= ~(1 << bit) 


#define DATPORT PORTD
#define CLKPORT PORTD
#define LATPORT PORTD

#define DATPIN PD1	//	SIN
#define CLKPIN PD2
#define LATPIN PD4

#define SIN_PIN		_BV( DATPIN )
#define SCLK_PIN	_BV( CLKPIN )
#define BLANK_PIN	_BV( PD3 )
#define XLAT_PIN	_BV( LATPIN )


// prototypes

void LED_Init(void);
void Config_Start(void);
void Config_TX(void);
void Config_RX(void);
void Config_SLP(void);
void SPI_CFG (uchar addr,uchar dat);
void uSecDly (uint uSecDlyCnt);
void Range_TX(void);
void Range_RX(void);

void Range_Config(void);
void TmrB1 (uint val);
void Ack(void);
void nAck(void);
void Read(void);
uchar ASC2HEX (uchar byte);
void HEX2ASC (uchar byte,uchar *pByte1,uchar *pByte2);
void ReadCfg(void);
void Do_TX(uchar *pMem);
void Wait_for_SPI_RXF(void);
void Wait_for_SPI_TXF(void);
void Wait_for_UART_Tx(void);
void Wait_for_UART_Rx(void);

uchar Read_Reg(uchar addr);

void SetforRx(void);
void SetforTx(void);

void mRX(void);
void mTX(void);
void M_LPWR(void);
void M_STBY(void);
void M_SPI(void);
void M_ACTIVE(void);
void SPI_WR(void);
void SPI_RD(void);
void SPI_RST(void);
void StoreRegs();
void SHUTDOWN(void);
void pUART(char *pStr);
void Hop(void);


#endif
