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

#define SWSPI_PORT PORTD
#define SWSPI_DIR  DDRD
#define SWSPI_IN   PIND


#define SWSPI_SS   (1<<1) // (CSn of the SPI can also be connected to GND permanently.) 
#define SWSPI_SCK  (1<<2)  // CLK  SCL/SPC clock signal 
#define SWSPI_MOSI (1<<4)  // mosi sda/sdi/sdo master out,slave in PD2 
#define SWSPI_MISO (1<<4)  // miso SDO

// rf blink

#if 1

#define DATPORT PORTD
#define CLKPORT PORTD //pc1
#define LATPORT PORTD
#define DATDDR DDRD

#define DATPIN PD1	//	SIN
#define CLKPIN PD2
#define LATPIN PD4

#define SIN_PIN		_BV( DATPIN ) /// PC0
#define SCLK_PIN	_BV( CLKPIN ) /// PC1
#define BLANK_PIN	_BV( PD3 )    /// PC2
#define XLAT_PIN	_BV( LATPIN ) /// PC3

#else

#define DATPORT PORTC
#define CLKPORT PORTC //pc1
#define LATPORT PORTC
#define DATDDR DDRC

#define DATPIN PC0	//	SIN
#define CLKPIN PC1
#define LATPIN PC3

#define SIN_PIN		_BV( PC0 ) /// PC0
#define SCLK_PIN	_BV( PC1 ) /// PC1
#define BLANK_PIN	_BV( PC2 )    /// PC2
#define XLAT_PIN	_BV( PC3 ) /// PC3
#endif



// prototypes


// TLC
void WriteLEDArray();
void LEDscan(int red, int green, int blue, float degreeoffset);
void LED_Init(void);

//sw spi
void SWSPI_init();
void SWSPI_write( uint8_t address, uint8_t value );
uint8_t SWSPI_read( uint8_t address );



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
void SCLK( unsigned char mode ) ;
void SDAT( unsigned char mode );
void MODE( unsigned char mode ) ;


extern uchar	hop_channel;
extern uchar	current_reg;
extern uchar	chan_num;		// TX/RX Channel
extern uchar	addr_len;		// TX/RX Address length (1 byte to 5 bytes) according to reg 0x08
extern uchar	*pBuf;			// 8-bit pointer to access buffer memory (must be static to retain addr between calls)
extern uchar 	Buffer[128];	// Allocate Buffer memory
extern uchar 	Reg_Buf[25];	// Buffer to hold Reg settings



#endif
