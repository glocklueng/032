/*****************************************************************************
*
*  spi.c - CC3000 Host Driver Implementation.
*  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/
#include "hci.h"
#include "netapp.h"
#include "evnt_handler.h"
#include "cc3000_common.h"
#include "user_board.h"
#include "spi.h"
#include "LUFA/Drivers/Peripheral/SPI.h"




extern uint8_t g_csPin, g_irqPin, g_vbatPin, g_IRQnum, g_SPIspeed;

#define READ                            (3)
#define WRITE                           (1)
#define HI(value)                       (((value) & 0xFF00) >> 8)
#define LO(value)                       ((value) & 0x00FF)
#define HEADERS_SIZE_EVNT               (SPI_HEADER_SIZE + 5)
#define SPI_HEADER_SIZE                 (5)

#define eSPI_STATE_POWERUP              (0)
#define eSPI_STATE_INITIALIZED          (1)
#define eSPI_STATE_IDLE                 (2)
#define eSPI_STATE_WRITE_IRQ            (3)
#define eSPI_STATE_WRITE_FIRST_PORTION  (4)
#define eSPI_STATE_WRITE_EOT            (5)
#define eSPI_STATE_READ_IRQ             (6)
#define eSPI_STATE_READ_FIRST_PORTION   (7)
#define eSPI_STATE_READ_EOT             (8)

#define     SPI_ORDER_MSB_FIRST   0
#define     SPI_ORDER_LSB_FIRST   SPI_DORD_bm

/*
these variables store the SPI configuration
so they can be modified and restored
this is so that we can have multiple SPI devices
on the same bus, so they can operate at different speeds
and in different modes
*/

uint8_t ccspi_mySPICTRL, ccspi_oldSPICTRL;
#define SpiConfigStoreOld()     do { ccspi_oldSPICTRL = SPCR; } while (0)
#define SpiConfigStoreMy()      do { ccspi_mySPICTRL = SPCR; } while (0)
#define SpiConfigPush()         do { ccspi_oldSPICTRL = SPCR; SPCR = ccspi_mySPICTRL; } while (0)
#define SpiConfigPop()          do { SPCR = ccspi_oldSPICTRL; } while (0)


typedef struct {
	gcSpiHandleRx  SPIRxHandler;

	unsigned short usTxPacketLength;
	unsigned short usRxPacketLength;
	unsigned long  ulSpiState;
	unsigned char *pTxPacket;
	unsigned char *pRxPacket;

} tSpiInformation;

tSpiInformation sSpiInformation;

/* Static buffer for 5 bytes of SPI HEADER */
unsigned char tSpiReadHeader[] = {READ, 0, 0, 0, 0};

void SpiWriteDataSynchronous ( unsigned char *data, unsigned short size );
void SpiWriteAsync ( const unsigned char *data, unsigned short size );
void SpiPauseSpi ( void );
void SpiResumeSpi ( void );
void SSIContReadOperation ( void );
void cc3k_int_poll ( void );



void detachInterrupt(int x) {
	
}
void attachInterrupt(int x) {
	
}

// The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
// for the purpose of detection of the overrun. The location of the memory where the magic number
// resides shall never be written. In case it is written - the overrun occurred and either receive function

// or send function will stuck forever.
#define CC3000_BUFFER_MAGIC_NUMBER (0xDE)

char spi_buffer[CC3000_RX_BUFFER_SIZE];
unsigned char wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];

static volatile char ccspi_is_in_irq = 0;
static volatile char ccspi_int_enabled = 0;

/* Mandatory functions are:
    - SpiOpen
    - SpiWrite
    - SpiRead
    - SpiClose
    - SpiResumeSpi
    - ReadWlanInterruptPin
    - WlanInterruptEnable
    - WlanInterruptDisable
    - WriteWlanPin
 */
unsigned char  SPI_transfer ( unsigned char by );

void SPI_setDataMode ( int mode );

unsigned char  SPI_transfer ( unsigned char by )
{
	return SPI_TransferByte( &SPIC, by );
}

void SPI_setDataMode ( int mode )
{

}

void SPI_setBitOrder ( int order )
{

}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void SpiClose ( void )
{
	__PROLOG ( "SpiClose" );

	if ( sSpiInformation.pRxPacket ) {
		sSpiInformation.pRxPacket = 0;
	}

	/*  Disable Interrupt in GPIOA module... */
	tSLInformation.WlanInterruptDisable();
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void SpiOpen ( gcSpiHandleRx pfRxHandler )
{
	__PROLOG ( "SpiOpen\n" );

	sSpiInformation.ulSpiState = eSPI_STATE_POWERUP;

	memset ( spi_buffer, 0, sizeof ( spi_buffer ) );
	memset ( wlan_tx_buffer, 0, sizeof ( spi_buffer ) );

	sSpiInformation.SPIRxHandler      = pfRxHandler;
	sSpiInformation.usTxPacketLength  = 0;
	sSpiInformation.pTxPacket         = NULL;
	sSpiInformation.pRxPacket         = ( unsigned char * ) spi_buffer;
	sSpiInformation.usRxPacketLength  = 0;

	spi_buffer[CC3000_RX_BUFFER_SIZE - 1]     = CC3000_BUFFER_MAGIC_NUMBER;
	wlan_tx_buffer[CC3000_TX_BUFFER_SIZE - 1] = CC3000_BUFFER_MAGIC_NUMBER;

	/* Enable interrupt on the GPIO pin of WLAN IRQ */
	tSLInformation.WlanInterruptEnable();

	__PROLOG ( "Finished SpiOpen\n" );
}

/**************************************************************************/
/*!
 */
/**************************************************************************/
int init_spi ( void )
{
	__PROLOG ( "init_spi\n" );


	/* Set POWER_EN pin to output and disable the CC3000 by default */
	PORTC.OUTCLR = VIO_bm;

	_delay_ms ( 500 );
/*
	SPI_Init ( &SPIC,   SPI_SPEED_FCPU_DIV_4 |
	SPI_ORDER_MSB_FIRST |
	SPI_SCK_LEAD_FALLING |
	SPI_SAMPLE_TRAILING |
	SPI_MODE_MASTER |
	SPI_INTLVL_LO_gc |
	SPI_MODE_1_gc
*/

	SPI_Init ( &SPIC,   SPI_PRESCALER_DIV16_gc | 
						SPI_MODE_0_gc 
	);
	
	PORTC.OUTSET = VIO_bm;
	
	PMIC.CTRL |= PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;

	dump_register("SPIC.INTCTRL",SPIC.INTCTRL);


	return ( ESUCCESS );
}
/**************************************************************************/
/*!

 */
/**************************************************************************/
long SpiFirstWrite ( unsigned char *ucBuf, unsigned short usLength )
{

	__PROLOG ( "SpiFirstWrite\n" );
	/* Workaround for the first transaction */
	CC3000_ASSERT_CS;

	/* delay (stay low) for ~50us */
	_delay_us ( 50 );

	/* SPI writes first 4 bytes of data */
	SpiWriteDataSynchronous ( ucBuf, 4 );

	_delay_us ( 50 );

	SpiWriteDataSynchronous ( ucBuf + 4, usLength - 4 );

	/* From this point on - operate in a regular manner */
	sSpiInformation.ulSpiState = eSPI_STATE_IDLE;

	CC3000_DEASSERT_CS;

	return ( 0 );
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
long SpiWrite ( unsigned char *pUserBuffer, unsigned short usLength )
{
	unsigned char ucPad = 0;

	__PROLOG ( "SpiWrite\n" );

	/* Figure out the total length of the packet in order to figure out if there is padding or not */
	if ( ! ( usLength & 0x0001 ) ) {
		ucPad++;
	}

	pUserBuffer[0] = WRITE;
	pUserBuffer[1] = HI ( usLength + ucPad );
	pUserBuffer[2] = LO ( usLength + ucPad );
	pUserBuffer[3] = 0;
	pUserBuffer[4] = 0;

	usLength += ( SPI_HEADER_SIZE + ucPad );

	/* The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
	 * for the purpose of overrun detection. If the magic number is overwritten - buffer overrun
	 * occurred - and we will be stuck here forever! */
	if ( wlan_tx_buffer[CC3000_TX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER ) {
		__PROLOG ( "Error No magic number in SpiWrite\n" );

		while ( 1 );
	}
	
	// if this fails its not getting an IRQ from the CC30000
	if ( sSpiInformation.ulSpiState == eSPI_STATE_POWERUP ) {
		
		while ( sSpiInformation.ulSpiState != eSPI_STATE_INITIALIZED ) 
			_delay_us(1);
	}

	if ( sSpiInformation.ulSpiState == eSPI_STATE_INITIALIZED ) {
		/* This is time for first TX/RX transactions over SPI: the IRQ is down - so need to send read buffer size command */
		SpiFirstWrite ( pUserBuffer, usLength );

	} else {
		/* We need to prevent here race that can occur in case two back to back packets are sent to the
		 * device, so the state will move to IDLE and once again to not IDLE due to IRQ */
		tSLInformation.WlanInterruptDisable();

		while ( sSpiInformation.ulSpiState != eSPI_STATE_IDLE );

		sSpiInformation.ulSpiState = eSPI_STATE_WRITE_IRQ;
		sSpiInformation.pTxPacket = pUserBuffer;
		sSpiInformation.usTxPacketLength = usLength;

		/* Assert the CS line and wait till SSI IRQ line is active and then initialize write operation */
		CC3000_ASSERT_CS;

		/* Re-enable IRQ - if it was not disabled - this is not a problem... */
		tSLInformation.WlanInterruptEnable();

		/* Check for a missing interrupt between the CS assertion and enabling back the interrupts */
		if ( tSLInformation.ReadWlanInterruptPin() == 0 ) {
			SpiWriteDataSynchronous ( sSpiInformation.pTxPacket, sSpiInformation.usTxPacketLength );

			sSpiInformation.ulSpiState = eSPI_STATE_IDLE;

			CC3000_DEASSERT_CS;
		}
	}

	/* Due to the fact that we are currently implementing a blocking situation
	 * here we will wait till end of transaction */
	while ( eSPI_STATE_IDLE != sSpiInformation.ulSpiState );

	return ( 0 );
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void SpiWriteDataSynchronous ( unsigned char *data, unsigned short size )
{
	unsigned char dummy;

	__PROLOG ( "SpiWriteDataSynchronous Start\n" );

	uint8_t loc;

	for ( loc = 0; loc < size; loc ++ )  {
		dummy = SPI_transfer ( data[loc] );
	}

	__PROLOG ( "\nSpiWriteDataSynchronous End\n" );
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void SpiReadDataSynchronous ( unsigned char *data, unsigned short size )
{
	int i = 0;

	__PROLOG ( "SpiReadDataSynchronous\n" );

//	SPI_setDataMode ( SPI_MODE1 );

	for ( i = 0; i < size; i ++ ) {
		data[i] = SPI_transfer ( 0x03 );
	}
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void SpiReadHeader ( void )
{
	__PROLOG ( "SpiReadHeader\n" );

	SpiReadDataSynchronous ( sSpiInformation.pRxPacket, 10 );
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
long SpiReadDataCont ( void )
{
	long data_to_recv;
	unsigned char *evnt_buff, type;

	__PROLOG ( "SpiReadDataCont\n" );

	/* Determine what type of packet we have */
	evnt_buff =  sSpiInformation.pRxPacket;
	data_to_recv = 0;
	STREAM_TO_UINT8 ( ( uint8_t * ) ( evnt_buff + SPI_HEADER_SIZE ), HCI_PACKET_TYPE_OFFSET, type );

	switch ( type ) {
		case HCI_TYPE_DATA: {
				/* We need to read the rest of data.. */
				STREAM_TO_UINT16 ( ( char * ) ( evnt_buff + SPI_HEADER_SIZE ), HCI_DATA_LENGTH_OFFSET, data_to_recv );

				if ( ! ( ( HEADERS_SIZE_EVNT + data_to_recv ) & 1 ) ) {
					data_to_recv++;
				}

				if ( data_to_recv ) {
					SpiReadDataSynchronous ( evnt_buff + 10, data_to_recv );
				}

				break;
			}

		case HCI_TYPE_EVNT: {
				/* Calculate the rest length of the data */
				STREAM_TO_UINT8 ( ( char * ) ( evnt_buff + SPI_HEADER_SIZE ), HCI_EVENT_LENGTH_OFFSET, data_to_recv );
				data_to_recv -= 1;

				/* Add padding byte if needed */
				if ( ( HEADERS_SIZE_EVNT + data_to_recv ) & 1 ) {
					data_to_recv++;
				}

				if ( data_to_recv ) {
					SpiReadDataSynchronous ( evnt_buff + 10, data_to_recv );
				}

				sSpiInformation.ulSpiState = eSPI_STATE_READ_EOT;
				break;
			}
	}

	return ( 0 );
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void SpiPauseSpi ( void )
{
	__PROLOG ( "SpiPauseSpi\n" );

	ccspi_int_enabled = 0;

	SPIC.INTCTRL = SPI_INTLVL_OFF_gc;
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void SpiResumeSpi ( void )
{
	__PROLOG ( "SpiResumeSpi\n" );

	ccspi_int_enabled = 1;

	SPIC.INTCTRL = SPI_INTLVL_MED_gc;
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void SpiTriggerRxProcessing ( void )
{
	__PROLOG ( "SpiTriggerRxProcessing\n" );

	/* Trigger Rx processing */
	SpiPauseSpi();
	CC3000_DEASSERT_CS;

	//__PROLOG("Magic?\n");
	/* The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
	 * for the purpose of detection of the overrun. If the magic number is overriten - buffer overrun
	 * occurred - and we will stuck here forever! */
	if ( sSpiInformation.pRxPacket[CC3000_RX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER ) {
		/* You've got problems if you're here! */
		__PROLOG ( "ERROR - magic number missing!\n" );

		while ( 1 );
	}

	sSpiInformation.ulSpiState = eSPI_STATE_IDLE;
	sSpiInformation.SPIRxHandler ( sSpiInformation.pRxPacket + SPI_HEADER_SIZE );
}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void SSIContReadOperation ( void )
{
	__PROLOG ( "SpiContReadOperation\n" );

	/* The header was read - continue with  the payload read */
	if ( !SpiReadDataCont() ) {
		/* All the data was read - finalize handling by switching to teh task
		 *  and calling from task Event Handler */
		//__PROLOG("SPItrig\n");
		SpiTriggerRxProcessing();
	}
}


/**************************************************************************/
/*!

 */
/**************************************************************************/

void SPI_IRQ ( void )
{
	circular_buffer_put('s');

	ccspi_is_in_irq = 1;
	
	if ( sSpiInformation.ulSpiState == eSPI_STATE_POWERUP ) {
		__PROLOG(" powerup\n");
		/* IRQ line was low ... perform a callback on the HCI Layer */
		sSpiInformation.ulSpiState = eSPI_STATE_INITIALIZED;

	} else if ( sSpiInformation.ulSpiState == eSPI_STATE_IDLE ) {
	
		__PROLOG(" IDLE\n");
	
		sSpiInformation.ulSpiState = eSPI_STATE_READ_IRQ;
		/* IRQ line goes down - start reception */

		CC3000_ASSERT_CS;

		// Wait for TX/RX Compete which will come as DMA interrupt
		SpiReadHeader();
		sSpiInformation.ulSpiState = eSPI_STATE_READ_EOT;
		__PROLOG(" SSICont\n");
		
		SSIContReadOperation();

	} else if ( sSpiInformation.ulSpiState == eSPI_STATE_WRITE_IRQ ) {
		__PROLOG(" write irq\n");
		SpiWriteDataSynchronous ( sSpiInformation.pTxPacket, sSpiInformation.usTxPacketLength );
		sSpiInformation.ulSpiState = eSPI_STATE_IDLE;
		CC3000_DEASSERT_CS;
	} else{
		
			circular_buffer_put( 'a' + sSpiInformation.ulSpiState);
		__PROLOG(" something else\n");
	}

	ccspi_is_in_irq = 0;
	return;
}

//*****************************************************************************
//
//!  cc3k_int_poll
//!
//!  \brief               checks if the interrupt pin is low
//!                       just in case the hardware missed a falling edge
//!                       function is in ccspi.cpp
//
//*****************************************************************************

void cc3k_int_poll(void)
{
	if ( ( ReadWlanInterruptPin() == 0 ) && ccspi_is_in_irq == 0 && ccspi_int_enabled != 0 ) {
		SPI_IRQ();
	}
}


ISR(SPIC_INT_vect)
{
	circular_buffer_put('v');
	
	SPI_IRQ();	
}