
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

#include "user_board.h"

// *****************************************************************************
//
//! \addtogroup link_buff_api
//! @{
//
// *****************************************************************************

#include "leds.h"
#include "hci.h"
#include "spi.h"

#include "spi_version.h"




#define SYSCTL_PERIPH_SPI           SYSCTL_PERIPH_SSI3

#define SPI_CLK_MUX_SEL             GPIO_PH0_SSI3CLK
#define SPI_RX_MUX_SEL              GPIO_PH2_SSI3RX
#define SPI_TX_MUX_SEL              GPIO_PH3_SSI3TX

#define SPI_UDMA_RX_CHANNEL          UDMA_CH14_SSI3RX
#define SPI_UDMA_TX_CHANNEL          UDMA_CH15_SSI3TX

#define FWT_DELAY               4000
#define DMA_WINDOW_SIZE         1024

#define SPI_WINDOW_SIZE         DMA_WINDOW_SIZE
#define DMA_CHANNEL_CONTROL_STRUCTURE_SIZE  (512)
#define READ                    3
#define WRITE                   1

#define HI(value)               (((value) & 0xFF00) >> 8)
#define LO(value)               ((value) & 0x00FF)

#define ASSERT_CS()          {PORTC.OUTSET = WLAN_SS_bm;}
//(MAP_GPIOPinWrite(sSpiInformation.sHwSettings.ulPioSpiPort, sSpiInformation.sHwSettings.ulPioSpiCs, 0))

#define DEASSERT_CS()        {PORTC.OUTCLR = WLAN_SS_bm;}
//(MAP_GPIOPinWrite(sSpiInformation.sHwSettings.ulPioSpiPort, sSpiInformation.sHwSettings.ulPioSpiCs, sSpiInformation.sHwSettings.ulPioSpiCs))


#define HEADERS_SIZE_EVNT       (SPI_HEADER_SIZE + 5)

#define SPI_HEADER_SIZE         (5)

#define     eSPI_STATE_POWERUP               (0)
#define     eSPI_STATE_INITIALIZED           (1)
#define     eSPI_STATE_IDLE                  (2)
#define     eSPI_STATE_WRITE_IRQ             (3)
#define     eSPI_STATE_WRITE_FIRST_PORTION   (4)
#define     eSPI_STATE_WRITE_EOT             (5)
#define     eSPI_STATE_READ_IRQ              (6)
#define     eSPI_STATE_READ_FIRST_PORTION    (7)
#define     eSPI_STATE_READ_EOT              (8)



typedef struct {
	unsigned long ulPioPortAddress;
	unsigned long ulPioSpiPort;
	unsigned long ulPioSpiCs;
	unsigned long ulPortInt;
	unsigned long ulPioSlEnable;

	unsigned long uluDmaPort;
	unsigned long uluDmaRxChannel;
	unsigned long uluDmaTxChannel;

	unsigned long ulSsiPort;
	unsigned long ulSsiPortAddress;
	unsigned long ulSsiTx;
	unsigned long ulSsiRx;
	unsigned long ulSsiClck;
	unsigned long ulSsiPortInt;
} tSpiHwConfiguration;

typedef struct {
	gcSpiHandleRx  SPIRxHandler;

	unsigned short usTxPacketLength;
	unsigned short usRxPacketLength;
	volatile unsigned long  ulSpiState;
	unsigned char *pTxPacket;
	unsigned char *pRxPacket;
	tSpiHwConfiguration sHwSettings;
} tSpiInformation;


tSpiInformation sSpiInformation;

char spi_buffer[CC3000_RX_BUFFER_SIZE];
//unsigned char wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];

// The magic number that resides at the end of the TX/RX buffer (1 byte after
// the allocated size) for the purpose of detection of the overrun. The location
// of the memory where the magic number  resides shall never be written.
// In case it is written - the overrun occurred and either receive function or
// send function will stuck forever.

#define CC3000_BUFFER_MAGIC_NUMBER (0xDEU)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//#pragma is used for determine the memory location for a specific variable.                            ///
//__no_init is used to prevent the buffer initialization in order to prevent hardware WDT expiration    ///
// before entering to 'main()'.                                                                         ///
//for every IDE, different syntax exists :          1.   __CCS__ for CCS v5                             ///
//                                                  2.  __IAR_SYSTEMS_ICC__ for IAR Embedded Workbench  ///
// *CCS does not initialize variables - therefore, __no_init is not needed.                             ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __CCS__
unsigned char wlan_rx_buffer[CC3000_RX_BUFFER_SIZE];
unsigned char wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];
unsigned char chBuffer[CC3000_RX_BUFFER_SIZE];
#pragma DATA_ALIGN(ucDMAChannelControlStructure, 1024);
static unsigned char ucDMAChannelControlStructure[DMA_CHANNEL_CONTROL_STRUCTURE_SIZE];
#elif __IAR_SYSTEMS_ICC__
__no_init unsigned char wlan_rx_buffer[CC3000_RX_BUFFER_SIZE];
__no_init unsigned char wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];
__no_init unsigned char chBuffer[CC3000_RX_BUFFER_SIZE];
#pragma data_alignment=1024
__no_init static unsigned char ucDMAChannelControlStructure[DMA_CHANNEL_CONTROL_STRUCTURE_SIZE];
#else
unsigned char wlan_rx_buffer[CC3000_RX_BUFFER_SIZE];
unsigned char wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];
unsigned char chBuffer[CC3000_RX_BUFFER_SIZE];
#pragma DATA_ALIGN(ucDMAChannelControlStructure, 1024);
static unsigned char ucDMAChannelControlStructure[DMA_CHANNEL_CONTROL_STRUCTURE_SIZE];

#endif

// Static buffer for 5 bytes of SPI HEADER
unsigned char tSpiReadHeader[] = {READ, 0, 0, 0, 0};

static SPI_Master_t spiMasterC;

void SpiWriteDataSynchronous ( const unsigned char *data, unsigned short size );
void SpiWriteAsync ( const unsigned char *data, unsigned short size );
void SpiReadData ( unsigned char *data, unsigned short size );
void SpiDisableInterrupts ( void );
void SpiResumeSpi ( void );

ISR ( SPIC_INT_vect )
{
	SPI_MasterInterruptHandler ( &spiMasterC );
}


//*****************************************************************************
//
//!  SpiSysDelay
//!
//!  @param  ulTicks ticks
//!
//!  @return none
//!
//!  @brief  This function delay for a few sys ticks.
//!
//
//*****************************************************************************

void
SpiSysDelay ( unsigned long ulTicks )
{
	while ( ulTicks ) {

		_delay_us ( 1 );
		ulTicks--;
	}
}

//*****************************************************************************
//
//!  SpiConfigureHwMapping
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief
//
//*****************************************************************************
void
SpiConfigureHwMapping ( void )
{
	__PROLOG ( "SpiConfigureHwMapping\n" );

}

//*****************************************************************************
//
//!  SpiCleanGPIOISR
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  This function get the reason for the GPIO interrupt
//!          and clear corresponding interrupt flag
//
//*****************************************************************************
void
SpiCleanGPIOISR ( void )
{
	//__PROLOG("SpiCleanGPIOISR\n");
}

//*****************************************************************************
//
//!  SSIConfigure
//!
//!  @param  ulSSIFreq
//!  @param  bForceGpioConfiguration
//!  @param  ulSysClck
//!
//!  @return none
//!
//!  @brief  Configure the SSI
//
//*****************************************************************************
void
SSIConfigure ( unsigned long ulSSIFreq, unsigned long bForceGpioConfiguration, unsigned long ulSysClck )
{
	__PROLOG ( "SSIConfigure\n" );
}


//*****************************************************************************
//
//!  SpiClose
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  Close the SPI interface
//
//*****************************************************************************
void
SpiClose ( void )
{
	__PROLOG ( "SpiClose\n" );

	if ( sSpiInformation.pRxPacket ) {
		sSpiInformation.pRxPacket = 0;
	}

	//
	//  Disable Interrupt in GPIOA module...
	//
	tSLInformation.WlanInterruptDisable();
}


//*****************************************************************************
//
//!  SpiOpen
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  Open the SPI interface
//
//*****************************************************************************
void
SpiOpen ( gcSpiHandleRx pfRxHandler )
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


	//
	// Enable interrupt on the GPIOA pin of WLAN IRQ
	//
	tSLInformation.WlanInterruptEnable();

}

//*****************************************************************************
//
//!  init_spi
//!
//!  @param  buffer
//!
//!  @return none
//!
//!  @brief  initializes an SPI interface
//
//*****************************************************************************

int init_spi ( void )
{
	// Configure SPI with CC3000

	// Init the SPI for the CC3000

	__PROLOG ( "init_spi\n" );

	SPI_MasterInit (
	    &spiMasterC,
	    &SPIC,
	    &PORTC,
	    false,
	    SPI_MODE_0_gc,
	    SPI_INTLVL_OFF_gc,
	    false,
	    SPI_PRESCALER_DIV4_gc
	);

//	SSIConfigure(1000000, 1, 50000000);

	return ( ESUCCESS );
}

//*****************************************************************************
//
//! SpiCheckDMAStatus
//!
//! @param  ch
//!
//! @return none
//!
//! @brief  This function get the current state of the SSI DMA channels
//
//*****************************************************************************
unsigned long
SpiCheckDMAStatus ( long ch )
{
	return 0;

}

//*****************************************************************************
//
//!  SpiIsDmaStop
//!
//!  @param  ch
//!
//!  @return 1 if DMA is stopped, 0 otherwise
//!
//!  @brief  Check DMA status
//
//*****************************************************************************
long
SpiIsDmaStop ( long ch )
{

	return ( 0 );
}

//*****************************************************************************
//
//! SpiFlushRxFifo
//!
//!  \param  none
//!
//!  \return none
//!
//!  \brief  flush RX
//
//*****************************************************************************
void
SpiFlushRxFifo ( void )
{
}

//*****************************************************************************
//
//!  SpiFirstWrite
//!
//!  @param  ucBuf buffer to write
//!  @param  ucBuf buffer's length
//!
//!  @return none
//!
//!  @brief  Spi first write operation
//
//*****************************************************************************
long
SpiFirstWrite ( unsigned char *ucBuf, unsigned short usLength )
{

	return ( 0 );
}



//*****************************************************************************
//
//! SpiWrite
//!
//!  @param  pUserBuffer buffer to write
//!  @param  pUserBuffer buffer's length
//!
//!  @return none
//!
//!  @brief  Spi write operation
//
//*****************************************************************************
long
SpiWrite ( unsigned char *pUserBuffer, unsigned short usLength )
{
	unsigned char ucPad = 0;

	__PROLOG ( "SpiWrite\n" );

	// Figure out the total length of the packet in order to figure out if there is padding or not
	if ( ! ( usLength & 0x0001 ) ) {
		ucPad++;
	}

	pUserBuffer[0] = WRITE;
	pUserBuffer[1] = HI ( usLength + ucPad );
	pUserBuffer[2] = LO ( usLength + ucPad );
	pUserBuffer[3] = 0;
	pUserBuffer[4] = 0;

	usLength += ( SPI_HEADER_SIZE + ucPad );


	// The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
	// for the purpose of detection of the overrun. If the magic number is overwritten - buffer overrun
	// occurred - and we will stuck here forever!
	if ( wlan_tx_buffer[CC3000_TX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER ) {
		while ( 1 )
			;
	}

	__PROLOG ( "2\n" );

	if ( sSpiInformation.ulSpiState == eSPI_STATE_POWERUP ) {
		while ( sSpiInformation.ulSpiState != eSPI_STATE_INITIALIZED );
	}

	__PROLOG ( "3\n" );

	if ( sSpiInformation.ulSpiState == eSPI_STATE_INITIALIZED ) {

		// This is time for first TX/RX transactions over SPI: the IRQ is down - so need to send read buffer size command
		SpiFirstWrite ( pUserBuffer, usLength );

		// Due to the fact that we are currently implementing a blocking situation
		// here we will wait till end of transaction

	} else {

		// We need to prevent here race that can occur in case 2 back to back packets are sent to the
		// device, so the state will move to IDLE and once again to not IDLE due to IRQ

		while ( sSpiInformation.ulSpiState != eSPI_STATE_IDLE ) {
			;
		}

		__PROLOG ( "4\n" );

		sSpiInformation.ulSpiState = eSPI_STATE_WRITE_IRQ;
		sSpiInformation.pTxPacket = pUserBuffer;
		sSpiInformation.usTxPacketLength = usLength;

		// Assert the CS line and wait till SSI IRQ line is active and then initialize write operation
		ASSERT_CS();
	}

	__PROLOG ( "5\n" );

	// Due to the fact that we are currently implementing a blocking situation
	// here we will wait till end of transaction
	while ( eSPI_STATE_IDLE != sSpiInformation.ulSpiState ) {
		;
	}

	__PROLOG ( "SpiWrite end\n" );
	return ( 0 );
}


//*****************************************************************************
//
//! SpiDisableSSIDMAChannels
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  This function disables the SSI DMA RX/TX Channels
//
//*****************************************************************************

void
SpiDisableSSIDMAChannels ( void )
{
}

//*****************************************************************************
//
//! SpiEnableSSIDMAChannels
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  This function enables the SSI DMA Channels
//
//*****************************************************************************

void
SpiEnableSSIDMAChannels ( void )
{
}

//*****************************************************************************
//
//! This function enter point for write flow
//!
//!  @param  data  buffer to read to
//!  @param  size  buffer's size
//!
//!  @return none
//!
//!  @brief  Spi read operation
//
//*****************************************************************************
void
SpiReadData ( unsigned char *data, unsigned short size )
{


}
//*****************************************************************************
//
//! SpiWriteAsync
//!
//!  @param  data  buffer to write
//!  @param  size  buffer's size
//!
//!  @return none
//!
//!  @brief  Spi write operation
//
//*****************************************************************************
void
SpiWriteAsync ( const unsigned char *data, unsigned short size )
{

}

//*****************************************************************************
//
//! SpiNewWriteAsync
//!
//!  @param  data  buffer to write
//!  @param  size  buffer's size
//!
//!  @return none
//!
//!  @brief
//
//*****************************************************************************
void
SpiNewWriteAsync ( unsigned char *data, unsigned short size )
{

}
//*****************************************************************************
//
//! SpiWriteDataSynchronous
//!
//!  @param  data  buffer to write
//!  @param  size  buffer's size
//!
//!  @return none
//!
//!  @brief  Spi write operation
//
//*****************************************************************************
void
SpiWriteDataSynchronous ( const unsigned char *data, unsigned short size )
{

unsigned long ulDummyData;

while (size)
{

	while(SPIF.STATUS&0x80);
	SSIDataPut(sSpiInformation.sHwSettings.ulSsiPort, (unsigned long)*data);
		while(SPIF.STATUS&0x80);
	SSIDataGet(sSpiInformation.sHwSettings.ulSsiPort, &ulDummyData);
	size --;
	data++;

}


//*****************************************************************************
//
//! SpiReadHeader
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  SPI read operation. first we read minimal 5 SPI header bytes and 5
//!          Event Data bytes
//
//*****************************************************************************
void
SpiReadHeader ( void )
{
	SpiReadData ( sSpiInformation.pRxPacket, 10 );
}


//*****************************************************************************
//
//! SpiReadDataCont
//!
//!  \param  None
//!
//!  \return None
//!
//!  \brief  This function processes received SPI Header and in accordance with
//!          it - continues reading the packet
//
//*****************************************************************************
long
SpiReadDataCont ( void )
{

	long data_to_recv;
	unsigned char *evnt_buff, type;

	//determine what type of packet we have
	evnt_buff =  sSpiInformation.pRxPacket;
	data_to_recv = 0;
	STREAM_TO_UINT8 ( ( char * ) ( evnt_buff + SPI_HEADER_SIZE ), HCI_PACKET_TYPE_OFFSET, type );

	switch ( type ) {

		case HCI_TYPE_DATA: {
				STREAM_TO_UINT16 ( ( char * ) ( evnt_buff + SPI_HEADER_SIZE ), HCI_DATA_LENGTH_OFFSET, data_to_recv );

				if ( data_to_recv >= SPI_WINDOW_SIZE ) {
					data_to_recv = eSPI_STATE_READ_FIRST_PORTION;
					SpiReadData ( evnt_buff + 10, SPI_WINDOW_SIZE );
					sSpiInformation.ulSpiState = eSPI_STATE_READ_FIRST_PORTION;

				} else {
					// We need to read the rest of data..
					if ( ! ( ( HEADERS_SIZE_EVNT + data_to_recv ) & 1 ) ) {
						data_to_recv++;
					}

					if ( data_to_recv ) {
						SpiReadData ( evnt_buff + 10, data_to_recv );
					}

					sSpiInformation.ulSpiState = eSPI_STATE_READ_EOT;
				}

				break;
			}

		case HCI_TYPE_EVNT: {

				// Calculate the rest length of the data
				STREAM_TO_UINT8 ( ( char * ) ( evnt_buff + SPI_HEADER_SIZE ), HCI_EVENT_LENGTH_OFFSET, data_to_recv );
				data_to_recv -= 1;

				// Add padding byte if needed
				if ( ( HEADERS_SIZE_EVNT + data_to_recv ) & 1 ) {

					data_to_recv++;
				}

				if ( data_to_recv ) {
					SpiReadData ( evnt_buff + 10, data_to_recv );
				}

				sSpiInformation.ulSpiState = eSPI_STATE_READ_EOT;
				break;
			}
	}

	return ( data_to_recv );
}


//*****************************************************************************
//
//! SpiDisableInterrupts
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  Disable SPI interrupts
//
//*****************************************************************************

void
SpiDisableInterrupts ( void )
{
	//IntDisable(sSpiInformation.sHwSettings.ulPortInt);
}


//*****************************************************************************
//
//! SpiResumeSpi
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  Spi resume operation
//
//*****************************************************************************

void
SpiResumeSpi ( void )
{
	//MAP_IntEnable(sSpiInformation.sHwSettings.ulSsiPortInt);
	//MAP_IntEnable(sSpiInformation.sHwSettings.ulPortInt);
}


//*****************************************************************************
//
//! SpiTriggerRxProcessing
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  Processing operation for SPI RX
//
//*****************************************************************************
void
SpiTriggerRxProcessing ( void )
{	
	//
	// Trigger Rx processing
	//
	SpiDisableInterrupts();			//SpiPauseSpi();

	DEASSERT_CS();
	
	

	sSpiInformation.ulSpiState = eSPI_STATE_IDLE;
	sSpiInformation.SPIRxHandler(sSpiInformation.pRxPacket + sizeof(btspi_hdr));
}

//*****************************************************************************
//
//! SSIContReadOperationw
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  Spi read operation
//
//*****************************************************************************

void
SSIContReadOperation ( void )
{

	// The header was read - continue with  the payload read
	if ( !SpiReadDataCont() ) {
		// All the data was read - finalize handling by switching to the task
		//  and calling from task Event Handler
		SpiTriggerRxProcessing();
	}
}

//*****************************************************************************
//
//!  IntSpiGPIOHandler
//!
//!  @param  none
//!
//!  @return none
//!
//!  @brief  GPIO A interrupt handler. When the external SSI WLAN device is
//!          ready to interact with Host CPU it generates an interrupt signal.
//!          After that Host CPU has registered this interrupt request
//!          it set the corresponding /CS in active state.
//
//*****************************************************************************
void
IntSpiGPIOHandler ( void )
{


	unsigned long    ulStatus;
//
// Get the reason for the interrupt
//

	ulStatus = ReadWlanInterruptPin();

	if ( ulStatus ) {
		SpiCleanGPIOISR();

		if ( !GPIOPinRead ( SPI_GPIO_IRQ_BASE, SPI_IRQ_PIN ) ) {
			if ( sSpiInformation.ulSpiState == eSPI_STATE_POWERUP ) {
				/* This means IRQ line was low call a callback of HCI Layer to inform on event */
				sSpiInformation.ulSpiState = eSPI_STATE_INITIALIZED;

			} else if ( sSpiInformation.ulSpiState == eSPI_STATE_IDLE ) {
				sSpiInformation.ulSpiState = eSPI_STATE_READ_IRQ;



				/* IRQ line goes down - we are start reception */
				ASSERT_CS();
				//
				// Wait for TX/RX Compete which will come as DMA interrupt
				//
				SpiReadHeader();
				sSpiInformation.ulSpiState = eSPI_STATE_READ_EOT;
				SSIContReadOperation();


			} else if ( sSpiInformation.ulSpiState == eSPI_STATE_WRITE_IRQ ) {


				SpiWriteDataSynchronous ( sSpiInformation.pTxPacket, sSpiInformation.usTxPacketLength );
				sSpiInformation.ulSpiState = eSPI_STATE_IDLE;
				DEASSERT_CS();


			}
		}
	} // end of

	SpiCleanGPIOISR();
}


//*****************************************************************************
//
//! SpiIntHandler
//!
//! @param  none
//!
//! @return none
//!
//! @brief  SSI interrupt handler. Get command/data packet from the external
//!         WLAN device and pass it through the UART to the PC
//
//*****************************************************************************
void
SpiIntHandler ( void )
{
}



//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
