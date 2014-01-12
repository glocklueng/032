#if 0

//-----------------------------------------------------------------------------
// Routines to load the FPGA image, and then to configure the FPGA's major
// mode once it is configured.
//
// Jonathan Westhues, April 2006
//-----------------------------------------------------------------------------
#include <proxmark3.h>
#include "apps.h"

//-----------------------------------------------------------------------------
// Set up the synchronous serial port, with the one set of options that we
// always use when we are talking to the FPGA. Both RX and TX are enabled.
//-----------------------------------------------------------------------------
void FpgaSetupSsc(void)
{
	// First configure the GPIOs, and get ourselves a clock.
	AT91C_BASE_PIOA->PIO_ASR =
		GPIO_SSC_FRAME	|
		GPIO_SSC_DIN	|
		GPIO_SSC_DOUT	|
		GPIO_SSC_CLK;
	AT91C_BASE_PIOA->PIO_PDR = GPIO_SSC_DOUT;

	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_SSC);

	// Now set up the SSC proper, starting from a known state.
	AT91C_BASE_SSC->SSC_CR = AT91C_SSC_SWRST;

	// RX clock comes from TX clock, RX starts when TX starts, data changes
	// on RX clock rising edge, sampled on falling edge
	AT91C_BASE_SSC->SSC_RCMR = SSC_CLOCK_MODE_SELECT(1) | SSC_CLOCK_MODE_START(1);

	// 8 bits per transfer, no loopback, MSB first, 1 transfer per sync
	// pulse, no output sync, start on positive-going edge of sync
	AT91C_BASE_SSC->SSC_RFMR = SSC_FRAME_MODE_BITS_IN_WORD(8) |
		AT91C_SSC_MSBF | SSC_FRAME_MODE_WORDS_PER_TRANSFER(0);

	// clock comes from TK pin, no clock output, outputs change on falling
	// edge of TK, start on rising edge of TF
	AT91C_BASE_SSC->SSC_TCMR = SSC_CLOCK_MODE_SELECT(2) |
		SSC_CLOCK_MODE_START(5);

	// tx framing is the same as the rx framing
	AT91C_BASE_SSC->SSC_TFMR = AT91C_BASE_SSC->SSC_RFMR;

	AT91C_BASE_SSC->SSC_CR = AT91C_SSC_RXEN | AT91C_SSC_TXEN;
}

//-----------------------------------------------------------------------------
// Set up DMA to receive samples from the FPGA. We will use the PDC, with
// a single buffer as a circular buffer (so that we just chain back to
// ourselves, not to another buffer). The stuff to manipulate those buffers
// is in apps.h, because it should be inlined, for speed.
//-----------------------------------------------------------------------------
void FpgaSetupSscDma(BYTE *buf, int len)
{
  // charlie
   //AT91C_BASE_PDC_SSC->PDC_PTCR = AT91C_PDC_RXTDIS; // disable Rx DMA

	AT91C_BASE_PDC_SSC->PDC_RPR = (uint32_t) buf;
	AT91C_BASE_PDC_SSC->PDC_RCR = len;
	
	// next buffer pointer
	AT91C_BASE_PDC_SSC->PDC_RNPR = (uint32_t) buf;
	AT91C_BASE_PDC_SSC->PDC_RNCR = len;
	
	// enable rx transfer
	AT91C_BASE_PDC_SSC->PDC_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTDIS;
}

static void DownloadFPGA_byte(unsigned char w)
{
#define SEND_BIT(x) { if(w & (1<<x) ) HIGH(GPIO_FPGA_DIN); else LOW(GPIO_FPGA_DIN); HIGH(GPIO_FPGA_CCLK); LOW(GPIO_FPGA_CCLK); }
	SEND_BIT(7);
	SEND_BIT(6);
	SEND_BIT(5);
	SEND_BIT(4);
	SEND_BIT(3);
	SEND_BIT(2);
	SEND_BIT(1);
	SEND_BIT(0);
}

// Download the fpga image starting at FpgaImage and with length FpgaImageLen bytes
// If bytereversal is set: reverse the byte order in each 4-byte word
static void DownloadFPGA(const char *FpgaImage, int FpgaImageLen, int bytereversal)
{
	int i=0;
	
	HIGH(NVDD_ON);					// ensure everything is powered on

	SpinDelay(50);

	LED_D_ON();

	// These pins are inputs
    AT91C_BASE_PIOA->PIO_ODR =
    	GPIO_FPGA_NINIT |
    	GPIO_FPGA_DONE;
	// PIO controls the following pins
    AT91C_BASE_PIOA->PIO_PER =
    	GPIO_FPGA_NINIT |
    	GPIO_FPGA_DONE;
	// Enable pull-ups
	AT91C_BASE_PIOA->PIO_PPUER =
		GPIO_FPGA_NINIT |
		GPIO_FPGA_DONE;

	// setup initial logic state
	HIGH(GPIO_FPGA_NPROGRAM);
	LOW(GPIO_FPGA_CCLK);
	LOW(GPIO_FPGA_DIN);
	// These pins are outputs
	AT91C_BASE_PIOA->PIO_OER =
		GPIO_FPGA_NPROGRAM	|
		GPIO_FPGA_CCLK		|
		GPIO_FPGA_DIN;

	// enter FPGA configuration mode
	LOW(GPIO_FPGA_NPROGRAM);
	SpinDelay(50);
	HIGH(GPIO_FPGA_NPROGRAM);

	i=100000;
	// wait for FPGA ready to accept data signal
	while ((i) && ( !(AT91C_BASE_PIOA->PIO_PDSR & GPIO_FPGA_NINIT ) ) ) {
		i--;
	}

	// crude error indicator, leave both red LEDs on and return
	if (i==0){
		LED_C_ON();
		LED_D_ON();
		return;
	}

	if(bytereversal) {
		/* This is only supported for DWORD aligned images */
		if( ((int)FpgaImage % sizeof(DWORD)) == 0 ) {
			i=0;
			while(FpgaImageLen-->0)
				DownloadFPGA_byte(FpgaImage[(i++)^0x3]);
			/* Explanation of the magic in the above line: 
			 * i^0x3 inverts the lower two bits of the integer i, counting backwards
			 * for each 4 byte increment. The generated sequence of (i++)^3 is
			 * 3 2 1 0 7 6 5 4 11 10 9 8 15 14 13 12 etc. pp. 
			 */
		}
	} else {
		while(FpgaImageLen-->0)
			DownloadFPGA_byte(*FpgaImage++);
	}

	// continue to clock FPGA until ready signal goes high
	i=100000;
	while ( (i--) && ( !(AT91C_BASE_PIOA->PIO_PDSR & GPIO_FPGA_DONE ) ) ) {
		HIGH(GPIO_FPGA_CCLK);
		LOW(GPIO_FPGA_CCLK);
	}
	// crude error indicator, leave both red LEDs on and return
	if (i==0){
		LED_C_ON();
		LED_D_ON();
		return;
	}
	LED_D_OFF();
}

static char *bitparse_headers_start;
static char *bitparse_bitstream_end;
static int bitparse_initialized;
/* Simple Xilinx .bit parser. The file starts with the fixed opaque byte sequence
 * 00 09 0f f0 0f f0 0f f0 0f f0 00 00 01
 * After that the format is 1 byte section type (ASCII character), 2 byte length
 * (big endian), <length> bytes content. Except for section 'e' which has 4 bytes
 * length.
 */
static const char _bitparse_fixed_header[] = {0x00, 0x09, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x00, 0x00, 0x01};
static int bitparse_init(void * start_address, void *end_address)
{
	bitparse_initialized = 0;
	
	if(memcmp(_bitparse_fixed_header, start_address, sizeof(_bitparse_fixed_header)) != 0) {
		return 0; /* Not matched */
	} else {
		bitparse_headers_start= ((char*)start_address) + sizeof(_bitparse_fixed_header);
		bitparse_bitstream_end= (char*)end_address;
		bitparse_initialized = 1;
		return 1;
	}
}

int bitparse_find_section(char section_name, char **section_start, unsigned int *section_length)
{
	char *pos = bitparse_headers_start;
	int result = 0;

	if(!bitparse_initialized) return 0;

	while(pos < bitparse_bitstream_end) {
		char current_name = *pos++;
		unsigned int current_length = 0;
		if(current_name < 'a' || current_name > 'e') {
			/* Strange section name, abort */
			break;
		}
		current_length = 0;
		switch(current_name) {
		case 'e':
			/* Four byte length field */
			current_length += (*pos++) << 24;
			current_length += (*pos++) << 16;
		default: /* Fall through, two byte length field */
			current_length += (*pos++) << 8;
			current_length += (*pos++) << 0;
		}
		
		if(current_name != 'e' && current_length > 255) {
			/* Maybe a parse error */
			break;
		}
		
		if(current_name == section_name) {
			/* Found it */
			*section_start = pos;
			*section_length = current_length;
			result = 1;
			break;
		}
		
		pos += current_length; /* Skip section */
	}
	
	return result;
}

//-----------------------------------------------------------------------------
// Find out which FPGA image format is stored in flash, then call DownloadFPGA
// with the right parameters to download the image
//-----------------------------------------------------------------------------
static const char _binary_fpga_bit_start[] = {
  //#include "fpga.h"
  #include "fpga_test.h"
};

static const char *_binary_fpga_bit_end=_binary_fpga_bit_start+sizeof(_binary_fpga_bit_start);

void FpgaDownloadAndGo(void)
{
	/* Check for the new flash image format: Should have the .bit file at &_binary_fpga_bit_start
	 */
	if(bitparse_init((void*)&_binary_fpga_bit_start, (void*)_binary_fpga_bit_end)) {
		/* Successfully initialized the .bit parser. Find the 'e' section and
		 * send its contents to the FPGA.
		 */
		char *bitstream_start;
		unsigned int bitstream_length;
		if(bitparse_find_section('e', &bitstream_start, &bitstream_length)) {
			DownloadFPGA(bitstream_start, bitstream_length, 0);
			
			return; /* All done */
		}
	}
	
	/* Fallback for the old flash image format: Check for the magic marker 0xFFFFFFFF
	 * 0xAA995566 at address 0x102000. This is raw bitstream with a size of 336,768 bits 
	 * = 10,524 DWORDs, stored as DWORDS e.g. little-endian in memory, but each DWORD
	 * is still to be transmitted in MSBit first order. Set the invert flag to indicate
	 * that the DownloadFPGA function should invert every 4 byte sequence when doing
	 * the bytewise download.
	 */
	if( *(uint32_t*)0x102000 == 0xFFFFFFFF && *(uint32_t*)0x102004 == 0xAA995566 )
		DownloadFPGA((char*)0x102000, 10524*4, 1);
}

void FpgaGatherVersion(char *dst, int len)
{
	char *fpga_info; 
	unsigned int fpga_info_len;
	dst[0] = 0;
	if(!bitparse_find_section('e', &fpga_info, &fpga_info_len)) {
		strncat(dst, "FPGA image: legacy image without version information", len-1);
	} else {
		strncat(dst, "FPGA image built", len-1);
		/* USB packets only have 48 bytes data payload, so be terse */
#if 0
		if(bitparse_find_section('a', &fpga_info, &fpga_info_len) && fpga_info[fpga_info_len-1] == 0 ) {
			strncat(dst, " from ", len-1);
			strncat(dst, fpga_info, len-1);
		}
		if(bitparse_find_section('b', &fpga_info, &fpga_info_len) && fpga_info[fpga_info_len-1] == 0 ) {
			strncat(dst, " for ", len-1);
			strncat(dst, fpga_info, len-1);
		}
#endif
		if(bitparse_find_section('c', &fpga_info, &fpga_info_len) && fpga_info[fpga_info_len-1] == 0 ) {
			strncat(dst, " on ", len-1);
			strncat(dst, fpga_info, len-1);
		}
		if(bitparse_find_section('d', &fpga_info, &fpga_info_len) && fpga_info[fpga_info_len-1] == 0 ) {
			strncat(dst, " at ", len-1);
			strncat(dst, fpga_info, len-1);
		}
	}
}

//-----------------------------------------------------------------------------
// Send a 16 bit command/data pair to the FPGA.
// The bit format is:  C3 C2 C1 C0 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
// where C is the 4 bit command and D is the 12 bit data
//-----------------------------------------------------------------------------
void FpgaSendCommand(WORD cmd, WORD v)
{
	SetupSpi(SPI_FPGA_MODE);
	while ((AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY) == 0);		// wait for the transfer to complete
	AT91C_BASE_SPI->SPI_TDR = AT91C_SPI_LASTXFER | cmd | v;		// send the data
}
//-----------------------------------------------------------------------------
// Write the FPGA setup word (that determines what mode the logic is in, read
// vs. clone vs. etc.). This is now a special case of FpgaSendCommand() to
// avoid changing this function's occurence everywhere in the source code.
//-----------------------------------------------------------------------------
void FpgaWriteConfWord(BYTE v)
{
	FpgaSendCommand(FPGA_CMD_SET_CONFREG, v);
}

#endif

#include "stm32f10x_lib.h"

#include "hw_config.h"
#include "l1_board.h"

#include "proxmark3.h"
#include "integer.h"
#include "apps.h"
#include "util.h"
#include "string.h"


#define	FPGA_SCLK_HIGH() 		HIGH(GPIO_SSC_CLK)
#define	FPGA_SCLK_LOW() 	        LOW(GPIO_SSC_CLK)
#define	FPGA_SDIN_HIGH() 		HIGH(GPIO_SSC_DIN)
#define	FPGA_SDIN_LOW()			LOW(GPIO_SSC_DIN)

#define	FPGA_NCS_LOW()			LOW(NCS)
#define	FPGA_NCS_HIGH()			HIGH(NCS)
	

// Temporarily putting my SPI routines in here til they work.

//Define all SPI Pins
#define SPI_OUT_PORT            	GPIOB
#define SPI_OUT_PIN			GPIO_Pin_5	
#define SPI_IN_PORT      		GPIOB
#define SPI_IN_PIN			GPIO_Pin_6
#define SPI_CLK_PORT			GPIOB
#define SPI_CLK_PIN			GPIO_Pin_2
#define SPI_CS_PORT			GPIOB
#define SPI_CS_PIN			GPIO_Pin_1

extern void __no_operation(void);

/**
 * softspi_rxtx(data) - send and receive a byte on a software SPI
 *
 * @param data - 8 bits to send
 *
 * @return 8 bits read back
*/
unsigned char softspi_rxtx(unsigned char data) 
{
  unsigned char ret;
  unsigned char bitmask;
  
  LOW( SPI_CLK  );
  
  LOW( SPI_CS ) ;
  
  // delay
  __no_operation();
  
  bitmask = 0x80;                			
  ret = 0;                    			
  
  do  {
    
    LOW( SPI_OUT ); 
    
    if (data & bitmask) {
      HIGH(SPI_OUT );
    }
    
    HIGH( SPI_CLK );     
    
    if (GETBIT(SPI_IN)) {
      ret |= bitmask;   
    }
  
    // delay
    __no_operation();                    
    
    LOW( SPI_CLK  );                
    bitmask = bitmask >> 1;          
  
    __no_operation();                   
  } while ( bitmask != 0);
  
  
  // delay
  __no_operation();
  __no_operation();
  
  HIGH(SPI_CS);
  
  return ret;
}

static void FPGASpiInit(void)
{
        FPGA_SDIN_HIGH() ;       
        FPGA_SCLK_HIGH();

        DelayuS(50);
}

static void FPGASpiSendByte(unsigned char data)
{
  unsigned char  i ;
  unsigned char  tbyte = data ;       
  
  FPGA_SCLK_HIGH( );
  
  for(i = 0 ; i < 8 ; i++) {
	 
	if( tbyte & 0x80 ) {            
	  FPGA_SDIN_HIGH() ;                       
	} else {
	  FPGA_SDIN_LOW() ;         
	}
	
	FPGA_SCLK_LOW() ;                    
	FPGA_SCLK_HIGH( );  
	
	tbyte  <<= 1;           
  }
}

static void FPGASpiSendWord(unsigned short cmdword)
{
  unsigned char  i ;
  unsigned short  tbyte = cmdword ;       
  
  FPGA_SCLK_HIGH( );
  
  for(i = 0 ; i <  16 ; i++) {
    
    if( tbyte & 0x80 ) {            
      FPGA_SDIN_HIGH() ;                       
    } else {
      FPGA_SDIN_LOW() ;         
    }
    
    FPGA_SCLK_LOW() ;                    
    FPGA_SCLK_HIGH( );  
    
    tbyte  <<= 1;           
  }
}

//-----------------------------------------------------------------------------
// Send a 16 bit command/data pair to the FPGA.
// The bit format is:  C3 C2 C1 C0 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
// where C is the 4 bit command and D is the 12 bit data
//-----------------------------------------------------------------------------
void FpgaSendCommand(WORD cmd, WORD v)
{
 	// SetupSpi(SPI_FPGA_MODE);
	FPGASpiSendWord( cmd | v );
  
	// no done on Software SPI
}

//-----------------------------------------------------------------------------
// Write the FPGA setup word (that determines what mode the logic is in, read
// vs. clone vs. etc.). This is now a special case of FpgaSendCommand() to
// avoid changing this function's occurence everywhere in the source code.
//-----------------------------------------------------------------------------
void FpgaWriteConfWord(BYTE v)
{
	FPGA_NCS_LOW();
 	FpgaSendCommand(FPGA_CMD_SET_CONFREG, v);
	FPGA_NCS_HIGH();
}

//-----------------------------------------------------------------------------
// Set up the CMOS switches that mux the ADC: four switches, independently
// closable, but should only close one at a time. Not an FPGA thing, but
// the samples from the ADC always flow through the FPGA.
//-----------------------------------------------------------------------------
void _SetAdcMuxFor(GPIO_TypeDef *port, u16 pin)
{
	LOW(GPIO_MUXSEL_HIPKD);
	LOW(GPIO_MUXSEL_HIRAW);
	LOW(GPIO_MUXSEL_LORAW);
	LOW(GPIO_MUXSEL_LOPKD);

	GPIO_WriteBit(port,pin,Bit_SET);
}

static void DownloadFPGA_byte(unsigned char w)
{
	#define SEND_BIT(x) { ;if(w & (1<<x) ) HIGH(GPIO_FPGA_DIN); else LOW(GPIO_FPGA_DIN); HIGH(GPIO_FPGA_CCLK); LOW(GPIO_FPGA_CCLK); }
  
	SEND_BIT(7);
	SEND_BIT(6);
	SEND_BIT(5);
	SEND_BIT(4);
	SEND_BIT(3);
	SEND_BIT(2);
	SEND_BIT(1);
	SEND_BIT(0);

}

// Download the fpga image starting at FpgaImage and with length FpgaImageLen bytes
// If bytereversal is set: reverse the byte order in each 4-byte word
static void DownloadFPGA(const char *FpgaImage, int FpgaImageLen, int bytereversal)
{
	int i=0;
	
  	OLEDPutstr("FPGA SETUP\n");
  	OLEDDraw();
	
	LOW(NVDD_ON);					// Enable p-Channel MOSFET
	SpinDelay(50);
	HIGH(FPGAON);					// Enable VREG's

  	OLEDPutstr("FPGA POWERED\n");
  	OLEDDraw();

									// Let VREG's settle down
	DelaymS(200);

	// setup initial logic state
	HIGH(GPIO_FPGA_NPROGRAM);
	LOW(GPIO_FPGA_CCLK);
	LOW(GPIO_FPGA_DIN);
	
	DelayuS(100);
	
	// enter FPGA configuration mode
	LOW(GPIO_FPGA_NPROGRAM);
	SpinDelay(50);
	HIGH(GPIO_FPGA_NPROGRAM);

	i=100000;
	// wait for FPGA ready to accept data signal, see UG331.PDF
	while ((i) && ( !(GETBIT(GPIO_FPGA_NINIT) ) ) ) {
		i--;
	}

	// error indicator, tell user failed at ninit
	if (i==0){
  		OLEDPutstr("FPGA FAILED NINIT\n");
	  	OLEDDraw();
		i = 50;
		// get users attention
		while( i-- ) {
			InvertOLED( 1 );
			DelaymS( 100 );
			InvertOLED( 0 );
			DelaymS( 100 );
		}
		return;
	}

  	OLEDPutstr("FPGA BITSTREAM DOWNLOAD\n");
	OLEDDraw();

	if(bytereversal) {
		/* This is only supported for DWORD aligned images */
		if( ((int)FpgaImage % sizeof(DWORD)) == 0 ) {
			i=0;
			while(FpgaImageLen-->0)
				DownloadFPGA_byte(FpgaImage[(i++)^0x3]);
			/* Explanation of the magic in the above line: 
			 * i^0x3 inverts the lower two bits of the integer i, counting backwards
			 * for each 4 byte increment. The generated sequence of (i++)^3 is
			 * 3 2 1 0 7 6 5 4 11 10 9 8 15 14 13 12 etc. pp. 
			 */
		}
	} else {
	  while(FpgaImageLen-->0) {
		DownloadFPGA_byte(*FpgaImage++);
	  }
	}

	// continue to clock FPGA until ready signal goes high
	i=100000;
	while ( (i) && ( !(GETBIT( GPIO_FPGA_DONE ) ) ) ) {
		HIGH(GPIO_FPGA_CCLK);
		LOW(GPIO_FPGA_CCLK);
		i--;
	}
	// error indicator
	if ( i== 0 ) {

	  	OLEDPutstr("FPGA FAILED TO ACK DONE\nCHECK FPGA\n");
	  	OLEDDraw();
		
		i = 50;
		
		// get users attention
		while( i-- ) {
		  
			InvertOLED( 1 );
			DelaymS( 100 );
			InvertOLED( 0 );
			DelaymS( 100 );
		}
		
		return;
	}

 	OLEDPutstr("FPGA BITSTREAM LOADED\n");
  	OLEDDraw();
	
	FPGA_NCS_HIGH();
	
}

static char *bitparse_headers_start;
static char *bitparse_bitstream_end;
static int bitparse_initialized;
/* Simple Xilinx .bit parser. The file starts with the fixed opaque byte sequence
 * 00 09 0f f0 0f f0 0f f0 0f f0 00 00 01
 * After that the format is 1 byte section type (ASCII character), 2 byte length
 * (big endian), <length> bytes content. Except for section 'e' which has 4 bytes
 * length.
 */
static const char _bitparse_fixed_header[] = {0x00, 0x09, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x00, 0x00, 0x01};
static int bitparse_init(void * start_address, void *end_address)
{
	bitparse_initialized = 0;
	
	if(memcmp(_bitparse_fixed_header, start_address, sizeof(_bitparse_fixed_header)) != 0) {
		return 0; /* Not matched */
	} else {
		bitparse_headers_start= ((char*)start_address) + sizeof(_bitparse_fixed_header);
		bitparse_bitstream_end= (char*)end_address;
		bitparse_initialized = 1;
		return 1;
	}
}

int bitparse_find_section(char section_name, char **section_start, unsigned int *section_length)
{
	char *pos = bitparse_headers_start;
	int result = 0;

	if(!bitparse_initialized) return 0;

	while(pos < bitparse_bitstream_end) {
		char current_name = *pos++;
		unsigned int current_length = 0;
		if(current_name < 'a' || current_name > 'e') {
			/* Strange section name, abort */
			break;
		}
		current_length = 0;
		switch(current_name) {
		case 'e':
			/* Four byte length field */
			current_length += (*pos++) << 24;
			current_length += (*pos++) << 16;
		default: /* Fall through, two byte length field */
			current_length += (*pos++) << 8;
			current_length += (*pos++) << 0;
		}
		
		if(current_name != 'e' && current_length > 255) {
			/* Maybe a parse error */
			break;
		}
		
		if(current_name == section_name) {
			/* Found it */
			*section_start = pos;
			*section_length = current_length;
			result = 1;
			break;
		}
		
		pos += current_length; /* Skip section */
	}
	
	return result;
}


//-----------------------------------------------------------------------------
// Find out which FPGA image format is stored in flash, then call DownloadFPGA
// with the right parameters to download the image
//-----------------------------------------------------------------------------
static const char _binary_fpga_bit_start[] = {
  #include "fpga.h"
  //#include "fpga_test.h"
};

static const char *_binary_fpga_bit_end=_binary_fpga_bit_start+sizeof(_binary_fpga_bit_start);

void FpgaDownloadAndGo(void)
{
	/* Check for the new flash image format: Should have the .bit file at &_binary_fpga_bit_start
	 */
	if(bitparse_init((void*)&_binary_fpga_bit_start, (void*)_binary_fpga_bit_end)) {
		/* Successfully initialized the .bit parser. Find the 'e' section and
		 * send its contents to the FPGA.
		 */
		char *bitstream_start;
		unsigned int bitstream_length;
		if(bitparse_find_section('e', &bitstream_start, &bitstream_length)) {
			DownloadFPGA(bitstream_start, bitstream_length, 0);
			
			return; /* All done */
		}
	}
	
	/* Fallback for the old flash image format: Check for the magic marker 0xFFFFFFFF
	 * 0xAA995566 at address 0x102000. This is raw bitstream with a size of 336,768 bits 
	 * = 10,524 DWORDs, stored as DWORDS e.g. little-endian in memory, but each DWORD
	 * is still to be transmitted in MSBit first order. Set the invert flag to indicate
	 * that the DownloadFPGA function should invert every 4 byte sequence when doing
	 * the bytewise download.
	 */
	if( *(uint32_t*)0x102000 == 0xFFFFFFFF && *(uint32_t*)0x102004 == 0xAA995566 )
		DownloadFPGA((char*)0x102000, 10524*4, 1);
}