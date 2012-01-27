// todo:
// convert the spi to software bitbang.

#include "RF_Blinky.h"

// variables for RF chip
uchar	hop_channel;
uchar	current_reg;
uchar	chan_num;		// TX/RX Channel
uchar	addr_len;		// TX/RX Address length (1 byte to 5 bytes) according to reg 0x08
static 	uchar *pBuf;	// 8-bit pointer to access buffer memory (must be static to retain addr between calls)
uchar 	Buffer[128];	// Allocate Buffer memory
uchar 	Reg_Buf[25];	// Buffer to hold Reg settings

//-----------------------------------------------------------------------------
uchar	AMODE;					// Set to: 0=RX
								//		   1=TX
	
// counter for reading config data from RFDA
uchar cfgcnt;					

static const uchar rangeTestStr[] = "\x02TRC104 Range Test Demo\x03";		// Range Test String
static const uchar AckStr[]  = "ACK";										// Acknowledge String
static const uchar nAckStr[] = "NACK";										// Error String
																			// 2=SLEE

// temporary 
uchar SPIRX = 0;

static const uchar freq_hop[25] = {			// Hop table for 25 channels

	80,
	49,
	69,
	25,
	50,
	70,
	29,
	54,
	74,
	30,
	55,
	75,
	34,
	59,
	79,
	35,
	60,
	39,
	64,
	40,
	65,
	44,
	66,
	45,
	67
};

// For octobrite code

// Variables for sample function
float offset = 0;

#define NUM_TLC5947 (1)

uint16_t LEDChannels[NUM_TLC5947*8][3];

//------------------------------------------------------------------------------------------
// Read INT state, data transaction
//   Transmit or receive complete interrupt output PIN 21
//------------------------------------------------------------------------------------------
inline unsigned char INT(void) 
{
	if( bit_is_clear( PORTC, PC1 ) ) {
		return 0;
	}

	return 1;
}

//------------------------------------------------------------------------------------------
// MODE ( on or off ) output
// Operating mode select input - used with PMODE and CS Pin 1 (I)
//
//                            P C M
// Sleep Mode                 0 X 1
// Stop Mode                  0 0 0
// Standby Mode               1 0 0
// Configuration Mode         1 1 0
// Transmit Load/Receive Mode 1 0 1
// Burst Transmit             1 0 (1->0)
//
//------------------------------------------------------------------------------------------
inline void MODE( unsigned char mode ) 
{
	if( mode ) 
		bit_set(PORTC,PC2) ; 
	else  
		bit_clr(PORTC,PC2);
}

//------------------------------------------------------------------------------------------
// Set SDAT state
// Serial data input/output for SPI mode and TX/RX active mode Pin 3 (IO)
//------------------------------------------------------------------------------------------
inline void SDAT( unsigned char mode ) 
{ 
	if( mode ) 
		bit_set(PORTC,PC4) ; 
	else  
		bit_clr(PORTC,PC4);
}

//------------------------------------------------------------------------------------------
// Set clock for SPI to TRC104
//Serial clock input for burst mode, serial data output for continuous mode, and
// SPI/FIFO clock signal Pin 2 (IO)
//------------------------------------------------------------------------------------------
inline void SCLK( unsigned char mode ) 
{ 
	if( mode ) 
		bit_set(PORTD,PD2) ; 
	else  
		bit_clr(PORTD,PD2);
}

//------------------------------------------------------------------------------------------
// Set pmode for TRC104
// Power mode select input - used with MODE to select standby or sleep mode Pin 10 (I)
//------------------------------------------------------------------------------------------
inline void PMODE( unsigned char mode ) 
{ 
	if( mode ) 
		bit_set(PORTD,PD0) ; 
	else  
		bit_clr(PORTD,PD0);
}

//------------------------------------------------------------------------------------------
// Set chip select for TRC104
// SPI serial interface select, active high Pin 4 (I)
//------------------------------------------------------------------------------------------
inline void CS( unsigned char mode )
{ 
	if( mode ) 
		bit_set(PORTC,PC5) ; 
	else  
		bit_clr(PORTC,PC5);
}

//------------------------------------------------------------------------------------------
// Bitbang to the SPI
//------------------------------------------------------------------------------------------
void WriteSPI ( unsigned char data )
{

#ifdef LSB
	for(unsigned char i = 0 ; i < 7 ; i++ ) 
#else

	for(unsigned char i = 7 ; i> 0 ; i-- ) 
#endif
	{
		SCLK(1);

		if(data&&(1<<i) )
			bit_set(PORTC,PC4); 
		else  
			bit_clr(PORTC,PC4);
		 
		SCLK(0);
	}

}

//------------------------------------------------------------------------------------------
// Read all bits in the LEDChannels array and send them on the selected pins
//------------------------------------------------------------------------------------------
void WriteLEDArray() {

  unsigned int tempOne = 0;

  for (unsigned int i = 0; i < (NUM_TLC5947  * 24); i++) {

    tempOne = *(&LEDChannels[0][0] + i);

    for (int j = 0; j < 12; j++) {
      if ((tempOne >> (11 - j)) & 1) {
        DATPORT |= (1 << DATPIN);
      } 
      else {
        DATPORT &= ~(1 << DATPIN);
      }
      CLKPORT |= (1 << CLKPIN);
      CLKPORT &= ~(1 << CLKPIN); 
    } 

  }
  LATPORT |= (1 << LATPIN);
  LATPORT &= ~(1 << LATPIN);
}

//------------------------------------------------------------------------------------------
// Sample function to draw a scanning pattern with fading
//------------------------------------------------------------------------------------------
void LEDscan(int red, int green, int blue, float degreeoffset) 
{

  float brightnessfactor = 0.0f;
  
  float scanindex = (1.0f + sinf(degreeoffset*3.14159f/180.0f)) * ((float)(NUM_TLC5947 * 8 ) / 2.0);
  
    for(unsigned int LEDindex = 0; LEDindex < (NUM_TLC5947  * 8); LEDindex++) {
      

      brightnessfactor = expf(0.0f - fabs(scanindex - ((float)LEDindex + 0.5f)) * 1.3f);

      LEDChannels[LEDindex][0] = blue * brightnessfactor;
      LEDChannels[LEDindex][1] = green * brightnessfactor;  
      LEDChannels[LEDindex][2] = red * brightnessfactor;  
    }
    
    WriteLEDArray();  

}

//------------------------------------------------------------------------------------------
// Sets up TLC5947 and TRC104
//------------------------------------------------------------------------------------------
void LED_Init(void)
{

 // Setup TLC5947 pins
	DATDDR	|= ( SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
	DATPORT	|= ( SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
	
	DATPORT |= (LATPIN);
	DATPORT &= ~(BLANK_PIN);

}


//------------------------------------------------------------------------------------------
// entry point
//------------------------------------------------------------------------------------------
int main(void)
{
	// Setup chip
	unsigned short r,g,b;
	unsigned short tr,tg,tb;

	//outputs
	DATDDR &= ~(_BV( PC2 ) | _BV( PC5 ) | _BV( PC4 ) );
	DATPORT |= (_BV( PC2 ) | _BV( PC5 ) | _BV( PC4 ) );

	DDRD &= ~(_BV( PD0 ) | _BV( PD2 ) );
	PORTD |= (_BV( PD0 ) | _BV( PD2 ));
	
	// inputs
	// SPI interrupt
	DDRC	|= ( _BV( PC1 ) | SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
	PORTC	|= ( _BV( PC1 ) | SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );


	// end chip setup

	pBuf = Reg_Buf;					// assign ptr addr
	cfgcnt = 0;						// initialize cfg data count

	chan_num = 80;					// Was 100
	addr_len = 3;  					// ADDRESS LENGTH (# bytes)

	SPI_CFG(0x08,0x00 + addr_len);	// ADDRESS LENGTH
//	SPI_CFG(0x09,0xCA);				// DEST ADDR 0
//	SPI_CFG(0x0E,0xCA);				// SRC ADDR 0
	SPI_CFG(0x02,0x20);				// CONT MODE,1Mbps,no CRC EN,

	LED_Init();						// Sequence/Flash LEDs on startup
	_delay_ms( 250 );
	
	Config_TX();					// Configure device for RX mode on startup
	
	M_ACTIVE();						// Enable Active Mode


	while( 0 ) {

		PMODE( 1 );
		_delay_ms( 1 ) ;

		PMODE( 0 );
		_delay_ms( 1 ) ;
	}

	r = 4096;
	g = 4096;
	b = 4096;
	
	tr = 4096;
	tg = 0;
	tb = 0;
	
#define SUB	( 2 )

	while(1)					// Loop Here
	{
		sei();					// Enable all interrupts

		if(fRTESTx)				// Test if Range test ON?
		{
			Range_Config();		// Do Configuration once

		}



		for (offset = 0; offset < 360; offset += 1 ) {

		Range_TX();

			LEDscan(r,g,b, offset);


			if (tr != r ) {if( r < tr ) r+=(abs(tr-r)/8); else r-=(abs(tr-r)/8);}
			if (tg != g ) {if( g < tg ) g+=(abs(tg-g)/8); else g-=(abs(tg-g)/8);}
			if (tb != b ) {if( b < tb ) b+=(abs(tb-b)/8); else b-=(abs(tb-b)/8);}
	
		}

	
		// new colour target
		if((rand() % 10) > 5  )
		{
		 tr = rand()%4096;
		 tg = rand()%4096;
		 tb = rand()%4096;
		}



	}

	return 0;
}

//------------------------------------------------------------------------------------------
// Transmit Data: This routine passes in the pointer and length of the string to send
// and configures the device for transmit mode. The data is then written to the FIFO
// via the SPI and is sent over the air. 
//------------------------------------------------------------------------------------------
void Do_TX(uchar *pMem)
{
	uchar len = 17;
	
	cli();						// Temporarily disable interrupts	
	mTX();						// TX LEDs
	M_ACTIVE();					// Prepare to write data to FIFO
	SPI_WR();					// SET SPI FOR TX
	_delay_us(15);				// Delay ~20usec (1.6usec/tick)

//---  Destination Address dummy byte write before data payload  ----------
 
/*	SPITX = 0x0A;			// Write Dest Addr
	Wait_for_SPI_TXF();
	SPITX = 0xA0;			
	Wait_for_SPI_TXF();
	SPITX = 0x0A;			
	Wait_for_SPI_TXF();

	SPITX = 0;			// Write dummy bytes for Src Addr
	Wait_for_SPI_TXF();
	SPITX = 0;			
	Wait_for_SPI_TXF();
	SPITX = 0;			
	Wait_for_SPI_TXF();
*/

//---  Write actual data payload  -----------------------------------------
	while(len)				// while not null character
	{
		WriteSPI( *pMem );			// Write data back-to-back to TX FIFO
		Wait_for_SPI_TXF();
		pMem++;					// Increment pointer to get next byte
		len--;
	}
/*
//---  Write actual data payload  -----------------------------------------
	while(*pMem)				// while not null character
	{
		SPITX = *pMem;			// Write data back-to-back to TX FIFO
		Wait_for_SPI_TXF();
		pMem++;					// Increment pointer to get next byte
	}
*/

//---  write 3 bit dummy cycle after transmit  ----------------------------
	WriteSPI( 0xC0 );
	SCLK(1);					// 3-bit dummy cycle
	SCLK(0);
	SCLK(1);
	SCLK(0);
	SCLK(1);
	SCLK(0);
	SPI_RST();					// RESET SPI
	M_STBY();					// Initiate actual TX (Turn on PLL and PA)	
}

//------------------------------------------------------------------------------------------
// Config_TX: Configure device for TX mode.
// Address Reg for writing Config info:
//
//    				nR/W | A7 | A6 | A5 | A3 | A2 | A1 | A0 |
//
//------------------------------------------------------------------------------------------
void Config_TX(void)	// **CONTINUOUS MODE ONLY
{
	mTX();				 // TX LEDs
	SPI_CFG(0x00,0x80 + chan_num);	 // Set for RX, CHANNEL 100
	SPI_CFG(0x01,0x1B);	 // max TX pwr,16MHz xtal
	SPI_CFG(0x02,0x20);	 // CONT MODE,1Mbps,NO CRC, DISCARD CRC ERR
	SPI_CFG(0x03,0x90);	 // NO RSSI REFRSH,DRSSI THRHLD
	SPI_CFG(0x04,0x00);	 // AGC ON,RSSI HI GAIN
//	SPI_CFG(0x05,0x00);	 // SADDR NO APPEND,SADDR NO INSERT,DADDR FROM REGS
//	SPI_CFG(0x06,0x30);	 // NO ZERO INSRT,16-bit preamble
//	SPI_CFG(0x07,0x00);	 // RMP-UP 0uS,RMP-DWN 5uS,PA-ON 0uS
//	SPI_CFG(0x08,0x04);	 // 4-byte ADDR LNGTH
//	SPI_CFG(0x09,0x00);	 // DEST ADDR 0
//	SPI_CFG(0x0A,0x00);	 // DEST ADDR 1
//	SPI_CFG(0x0B,0x00);	 // DEST ADDR 2
//	SPI_CFG(0x0C,0x00);	 // DEST ADDR 3
//	SPI_CFG(0x0D,0x00);	 // DEST ADDR 4
//	SPI_CFG(0x0E,0x00);	 // SRC ADDR 0
//	SPI_CFG(0x0F,0x00);  // SRC ADDR 1
//	SPI_CFG(0x10,0x00);	 // SRC ADDR 2
//	SPI_CFG(0x11,0x00);	 // SRC ADDR 3
//	SPI_CFG(0x12,0x00);	 // SRC ADDR 4
//	SPI_CFG(0x14,0x00);	 // PLLON-TIME 20uS
//	SPI_CFG(0x15,0x04);	 // PLL LOCK TIME 50uS,ANALOG ON 0uS
//	SPI_CFG(0x17,0x23);	 // INT LVL 'HI',DRSSI LVL 'HI'

//	SPI_CFG(0x06,0xB0);	 // ZERO INSRT,16-bit preamble 250kb
//	SPI_CFG(0x39,0xB8);	 // 250kbps
//	SPI_CFG(0x4F,0x66);	 // 250kbps
//	SPI_CFG(0x77,0x5C);	 // 250kbps

}

//------------------------------------------------------------------------------------------
// Config_RX: Configure device for RX continuous mode.
// Address Reg for writing Config info:
//
//    				nR/W | A7 | A6 | A5 | A3 | A2 | A1 | A0 |
//
//------------------------------------------------------------------------------------------
void Config_RX(void)	// **CONTINUOUS MODE ONLY
{
	mRX();				 // RX LEDs
	SPI_CFG(0x00,0x00 + chan_num);	 // Set for RX, CHANNEL 100
	SPI_CFG(0x01,0x1B);	 // Max TX pwr,16MHz xtal
//	SPI_CFG(0x02,0x20);	 // CONT MODE,1Mbps,NO CRC,DISCARD CRC ERR
	SPI_CFG(0x03,0x90);	 // RSSIA REFRSH on,DRSSI THRHLD
	SPI_CFG(0x04,0x00);	 // AGC ON,RSSI HI GAIN
//	SPI_CFG(0x05,0x00);	 // SADDR NO APPEND,SADDR NO INSERT,DADDR FROM REGS
//	SPI_CFG(0x06,0x30);	 // NO ZERO INSRT,16-bit preamble
//	SPI_CFG(0x07,0x00);	 // RMP-UP 0uS,RMP-DWN 5uS,PA-ON 0uS
//	SPI_CFG(0x08,0x04);	 // 4-byte ADDR LNGTH
//	SPI_CFG(0x09,0x00);	 // DEST ADDR 0
//	SPI_CFG(0x0A,0x00);	 // DEST ADDR 1
//	SPI_CFG(0x0B,0x00);	 // DEST ADDR 2
//	SPI_CFG(0x0C,0x00);	 // DEST ADDR 3
//	SPI_CFG(0x0D,0x00);	 // DEST ADDR 4
//	SPI_CFG(0x0E,0x00);	 // SRC ADDR 0
//	SPI_CFG(0x0F,0x00);  // SRC ADDR 1
//	SPI_CFG(0x10,0x00);	 // SRC ADDR 2
//	SPI_CFG(0x11,0x00);	 // SRC ADDR 3
//	SPI_CFG(0x12,0x00);	 // SRC ADDR 4
//	SPI_CFG(0x14,0x00);	 // PLLON-TIME 20uS
//	SPI_CFG(0x15,0x04);	 // PLL LOCK TIME 50uS,ANALOG ON 0uS
//	SPI_CFG(0x17,0x23);	 // INT LVL 'HI',DRSSI LVL 'HI'

//	SPI_CFG(0x06,0xB0);	 // ZERO INSRT,16-bit preamble 250kb
//	SPI_CFG(0x39,0xB8);	 // 250kbps
//	SPI_CFG(0x4F,0x66);	 // 250kbps
//	SPI_CFG(0x77,0x5C);	 // 250kbps

}

//------------------------------------------------------------------------------------------
// Range_Config: Configure device for Range Test mode.
// This routine will configure the TX and RX functions for Buffered mode.
// After it is sent, the device will configure to RX mode to receive
// the acknowledge.
//------------------------------------------------------------------------------------------
void Range_Config(void)	// **BURST MODE ONLY
{
	SPI_CFG(0x00,0x00 + chan_num);	 // RX, CHANNEL 100
	SPI_CFG(0x01,0x1B);	 // max TX pwr,16MHz xtal
	SPI_CFG(0x02,0x68);	 // BURST MODE,1Mbps,CRC EN,no-DUMP PKT ON ERR WAS 68 = 1mb, 48 = 250kb
	SPI_CFG(0x03,0x9F);	 // RSSIA ON,DRSSI THRHLD
//	SPI_CFG(0x04,0x20);	 // AGC On,RSSI HI GAIN
	SPI_CFG(0x05,0x00);	 // sets des address enable from confi regs
//	SPI_CFG(0x06,0x30);	 // ZERO INSRT,16-bit preamble
	SPI_CFG(0x07,0x20);	 // RMP-UP >0uS,RMP-DWN 5uS,PA-ON 0uS Was 10
	SPI_CFG(0x08,0x03);	 // 3-byte ADDR LNGTH
	SPI_CFG(0x09,0x0A);	 // DEST ADDR 0
	SPI_CFG(0x0A,0xA0);	 // DEST ADDR 1
	SPI_CFG(0x0B,0x0A);	 // DEST ADDR 2
//	SPI_CFG(0x0C,0x00);	 // DEST ADDR 3
//	SPI_CFG(0x0D,0x00);	 // DEST ADDR 4
	SPI_CFG(0x0E,0x0A);	 // SRC ADDR 0
	SPI_CFG(0x0F,0xA0);  // SRC ADDR 1
	SPI_CFG(0x10,0x0A);	 // SRC ADDR 2
//	SPI_CFG(0x11,0x00);	 // SRC ADDR 3
//	SPI_CFG(0x12,0x00);	 // SRC ADDR 4
//	SPI_CFG(0x14,0x00);	 // NO PLL PRE-ON TIME
//	SPI_CFG(0x15,0x04);	 // PLL LOCK TIME 50uS,ANALOG ON 0uS
	SPI_CFG(0x17,0x23);	 // INT LVL 'HI',DRSSI LVL 'HI'

	SPI_CFG(0x06,0xB0);	 // ZERO INSRT,16-bit preamble
	SPI_CFG(0x39,0xB9);	 // 250kbps was B8
	SPI_CFG(0x4F,0x66);	 // 250kbps
	SPI_CFG(0x77,0x5C);	 // 250kbps	

//********* INIT HOPPER *******
	hop_channel = 0;

//	SPI_CFG(0x00,freq_hop[hop_channel]);		// load first reg1
//	SPI_CFG(0x00,0x00 + freq_hop[hop_channel]);
//	Hop();
//*****************************	

}


//------------------------------------------------------------------------------------------
// Range_TX: Configures the device for TX mode and sends the Range Test packet.
//------------------------------------------------------------------------------------------
void Range_TX(void)
{
	uchar *pMem;			// pointer to access memory
	uchar tmp_buf;
	uchar len;

	pMem = (uchar*)rangeTestStr;	// Assign addr of string location
//	len = (strlen((char*)rangeTestStr) + addr_len) - 1;	// calc data packet length

	len = strlen((char*)rangeTestStr);	// calc data packet length

	SetforTx();
	tmp_buf = Read_Reg(0x05);				// Read contents of register in TRC104
	tmp_buf = ((tmp_buf & 0xE0) | len);		// OR len to retain other bits
	SPI_CFG(0x05,tmp_buf);					// Write register
	Do_TX(pMem);
	while(INT ()== 0)		// loop til TX finished when INT='1'
	{
	}
	sei();		// Enable interrupts	

}

//-----------------------------------------------------------------------------
// Wait_for_SPI_TXF: Wait for the SPI to finish sending byte
// 21 O INT Transmit or receive complete interrupt output
// PC1
//-----------------------------------------------------------------------------
void Wait_for_SPI_TXF(void)
{
	while(bit_is_clear(PORTC,PC1));
}

//-----------------------------------------------------------------------------
// Wait_for_SPI_RXF: Wait for the SPI to finish receiving byte
//-----------------------------------------------------------------------------
void Wait_for_SPI_RXF(void)
{
}

//-------------------------------------------------------------------
//  Channel Hop routine
//-------------------------------------------------------------------
void Hop(void)			// Change channels and reg's
{
	uchar tmp_buf;

	tmp_buf = Read_Reg(0x00);	// Read contents of register in TRC104
	tmp_buf = (tmp_buf & 0x80) + freq_hop[hop_channel]; 	// Set up for RX mode,keep channel settings
//	SPI_CFG(0x00, freq_hop[hop_channel]);	
//	SPI_CFG(0x00,tmp_buf);		// load hop channel
	
	if (hop_channel++ >= 24) 
	{
		hop_channel = 0;	// max 25 channels
	}
}

//-----------------------------------------------------------------------------
// Read_Reg: Read the current value of a register.  Pass in addr, pass out val 
//-----------------------------------------------------------------------------
uchar Read_Reg(uchar addr)
{
	uchar tmp_buf2;

	cli();						// disable interrupts temporarily
	
	M_SPI();					// SPI MODE
	SPI_WR();					// SET SPI FOR TX
	_delay_us(14);				// Delay ~20usec (1.6usec/tick)
	WriteSPI(addr);				// Write config Addr
	Wait_for_SPI_TXF();			// Wait for SPI to finish
	SPI_RD();					// SET SPI FOR RX
	_delay_us(14);				// Delay ~20usec (1.6usec/tick)
	tmp_buf2 = SPIRX;			// Dummy Read
	Wait_for_SPI_RXF();
	tmp_buf2 = SPIRX;			// Read
	Wait_for_SPI_RXF();
	SDAT(0);
	M_STBY();					// Standby Mode
	SPI_RST();					// RESET SPI

	sei();						// re-enable interrupts

	return tmp_buf2;

}

// Control LED's
void TXMD( unsigned char state)
{
}

void RXMD( unsigned char state)
{
}

void YEL2LED( unsigned char state ) 
{

}

//-----------------------------------------------------------------------------
// RX Mode LEDs
//-----------------------------------------------------------------------------
void mRX(void)
{
	AMODE=0;			// RX mode configure
	TXMD(0);			// Turn off TX LED
	RXMD(1);			// Turn on RX LED
}

//-----------------------------------------------------------------------------
// TX Mode LEDs
//-----------------------------------------------------------------------------
void mTX(void)
{
	AMODE=1;			// TX mode configure
	TXMD(1);			// Turn on TX LED
	RXMD(0);			// Turn off RX LED
}

//-----------------------------------------------------------------------------
// Set for RX: sets the TRC104 into RX mode and retains the other settings
//-----------------------------------------------------------------------------
void SetforRx(void)
{
	uchar tmp_buf;

	tmp_buf = Read_Reg(0x00);			// Read contents of register in TRC104
//	tmp_buf = tmp_buf & 0x7F; 			// Set up for RX mode,keep channel settings
	tmp_buf = 0x00 + freq_hop[hop_channel]; 			// Set up for RX mode and channel setting
	SPI_CFG(0x00,tmp_buf);				// write SPI
	M_ACTIVE();							// Activate RX Mode
}

//-----------------------------------------------------------------------------
// Set for TX: sets the TRC104 into TX mode and retains the other settings
//-----------------------------------------------------------------------------
void SetforTx(void)
{
	uchar tmp_buf;

//	tmp_buf = Read_Reg(0x00);						// Read contents of register in TRC104
	tmp_buf = 0x80 + freq_hop[hop_channel]; 		// Set up for TX mode,keep channel setting
	SPI_CFG(0x00,tmp_buf);							// write SPI
}

//------------------------------------------------------------------------------------
// Get Address length: Reads the length (#bytes) of the Address header from reg 0x08
//------------------------------------------------------------------------------------
uchar getAddrlen(void)
{
	uchar tmp_buf;

	tmp_buf = Read_Reg(0x08);		// Read contents of register in TRC104
	tmp_buf = tmp_buf & 0x07;		// Set up for TX mode,keep channel setting

	return tmp_buf;
}

//-----------------------------------------------------------------------------
// Set for Low Power Mode.
//-----------------------------------------------------------------------------
void SHUTDOWN(void)
{
	MODE(1);
	PMODE(0);
	CS(0);
}

//-----------------------------------------------------------------------------
// Set for Low Power Mode.
//-----------------------------------------------------------------------------
void M_LPWR(void)
{
	MODE(0);
	PMODE(0);
	CS(0);
	_delay_ms(1);
}

//-----------------------------------------------------------------------------
// Set for STANDBY Mode.
//-----------------------------------------------------------------------------
void M_STBY(void)
{
	MODE(0);
	PMODE(1);
	CS(0);
	_delay_ms(1);
}

//-----------------------------------------------------------------------------
// Set for SPI ACCESS Mode.
//-----------------------------------------------------------------------------
void M_SPI(void)
{
	MODE(0);
	PMODE(1);
	CS(1);
	_delay_ms(1);
}

//-----------------------------------------------------------------------------
// Set for TX/RX Mode.
//-----------------------------------------------------------------------------
void M_ACTIVE(void)
{
	MODE(1);
	PMODE(1);
	CS(0);
	_delay_ms(1);
}

//-----------------------------------------------------------------------------
// Set for SPI WRITE Mode.
//-----------------------------------------------------------------------------
void SPI_WR(void)
{
/*todo */
//	SSU.SSER.BYTE = 0x00;			// 
//	IO.PCR9 = 0xF;					
//	SSU.SSCRH.BYTE = 0xC4;		
	SDAT(0);
//	SSU.SSER.BYTE = 0xA0;			
}

//-----------------------------------------------------------------------------
// Set for SPI READ Mode.
//-----------------------------------------------------------------------------
void SPI_RD(void)
{
/*todo */
//	SSU.SSER.BYTE = 0x00;			// 
//	IO.PCR9 = 0xB;					
//	SSU.SSCRH.BYTE = 0xC4;
	SDAT(0);
//	SSU.SSER.BYTE = 0x60;		

}

//-----------------------------------------------------------------------------
// RESet SPI TO POWERUP SETTINGS (ALL PORT PINS I/Os)
//-----------------------------------------------------------------------------
void SPI_RST(void)
{
/*todo
	SSU.SSER.BYTE = 0x20;			// 
	IO.PCR9 = 0x9;					// Set 90,93 as output
	SSU.SSCRH.BYTE = 0xC0;			// 
*/
	SDAT(0);
}

//-------------------------------------------------------------------
//  Read the internal registers of the TRC104
//-------------------------------------------------------------------
void StoreRegs(void)
{
		uchar i=0;
		uchar tmp;

		pBuf = Reg_Buf;
		while( i < 24 )
		{
			tmp = Read_Reg(i);
			*pBuf = tmp;
			pBuf++;
			i++;
		}
}
//------------------------------------------------------------------------------------------
// SPI_CFG: Routine to write and/or read using SPI
//
//    				nR/W | A6 | A5 | A4 | A3 | A2 | A1 | A0 | 
//
// The address must be shifted left one bit for proper positioning.
//
// 	READ = 0x00-0x7F
// 	WRITE = 0x80-0xFF
//
//------------------------------------------------------------------------------------------
void SPI_CFG(uchar addr,uchar dat)
{
	cli();					// disable interrupts temporarily

	YEL2LED (1);			// LED On
	M_SPI();
	SPI_WR();				// SET SPI FOR TX
	_delay_us(15);			// Delay ~20usec (1.6usec/tick)
	WriteSPI( addr | 0x80);	// Write config Addr
	Wait_for_SPI_TXF();
	WriteSPI(dat);			// Write config DATA to SPI
	Wait_for_SPI_TXF();		// Wait for SPI to finish
	M_STBY();
	SPI_RST();				// RESET SPI
	YEL2LED(0);				// LED Off
  
	sei();					// re-enable interrupts
}
