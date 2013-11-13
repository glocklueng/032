/*
 * cc3300.c
 *
 * Created: 11/11/2013 6:27:14 PM
 *  Author: charlie
 */


#include "leds.h"


static volatile char ccspi_is_in_irq = 0;
static volatile char ccspi_int_enabled = 0;

/* *********************************************************************** */
/*                                                                         */
/* PRIVATE FIELDS (SmartConfig)                                            */
/*                                                                         */
/* *********************************************************************** */
volatile unsigned long ulSmartConfigFinished,
         ulCC3000Connected,
         ulCC3000DHCP,
         OkToDoShutDown,
         ulCC3000DHCP_configured;
volatile unsigned char ucStopSmartConfig;
volatile long ulSocket;
uint8_t pingReportnum;
netapp_pingreport_args_t pingReport;
unsigned char useSmartConfigData = 0 ;

const static char _deviceName[] = "CC3000";
const static char _cc3000_prefix[] = { 'T', 'T', 'T' };
const static unsigned char _smartConfigKey[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                                          0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35
                                        };
// AES key for smart config = "0123456789012345"


#define CC3000_SUCCESS                        (0)
#define CHECK_SUCCESS(func,Notify,errorCode)  {if ((func) != CC3000_SUCCESS) { if (CC3KPrinter != 0) CC3KPrinter->println(F(Notify)); return errorCode;}}

#define MAXSSID                   (32)
#define MAXLENGTHKEY            (32)  /* Cleared for 32 bytes by TI engineering 29/08/13 */

#define MAX_SOCKETS 32  // can change this
unsigned char closed_sockets[MAX_SOCKETS] = {false, false, false, false};

//*****************************************************************************
//
//! CC3000_UsynchCallback
//!
//! @param  lEventType Event type
//! @param  data
//! @param  length
//!
//! @return none
//!
//! @brief  The function handles asynchronous events that come from CC3000
//!         device and operates a led for indicate
//
//*****************************************************************************
void CC3000_UsynchCallback ( long lEventType, char * data, unsigned char length )
{
	if ( lEventType == HCI_EVNT_WLAN_ASYNC_SIMPLE_CONFIG_DONE ) {
		ulSmartConfigFinished = 1;
		ucStopSmartConfig     = 1;
	}

	if ( lEventType == HCI_EVNT_WLAN_UNSOL_CONNECT ) {
		ulCC3000Connected = 1;
	}

	if ( lEventType == HCI_EVNT_WLAN_UNSOL_DISCONNECT ) {
		ulCC3000Connected = 0;
		ulCC3000DHCP      = 0;
		ulCC3000DHCP_configured = 0;
	}

	if ( lEventType == HCI_EVNT_WLAN_UNSOL_DHCP ) {
		ulCC3000DHCP = 1;
	}

	if ( lEventType == HCI_EVENT_CC3000_CAN_SHUT_DOWN ) {
		OkToDoShutDown = 1;
	}

	if ( lEventType == HCI_EVNT_WLAN_ASYNC_PING_REPORT ) {
		//PRINT_F("CC3000: Ping report\n\r");
		pingReportnum++;
		memcpy ( &pingReport, data, length );
	}

	if ( lEventType == HCI_EVNT_BSD_TCP_CLOSE_WAIT ) {
		uint8_t socketnum;
		socketnum = data[0];

		//PRINT_F("TCP Close wait #"); printDec(socketnum);
		if ( socketnum < MAX_SOCKETS )
		{ closed_sockets[socketnum] = true; }
	}
}

//*****************************************************************************
//
//! sendWLFWPatch
//!
//! @param  pointer to the length
//!
//! @return none
//!
//! @brief  The function returns a pointer to the FW patch:
//!         since there is no patch in the host - it returns 0
//
//*****************************************************************************
char *sendWLFWPatch ( unsigned long *Length )
{
	*Length = 0;
	return NULL;
}
//*****************************************************************************
//
//! sendDriverPatch
//!
//! @param  pointer to the length
//!
//! @return none
//!
//! @brief  The function returns a pointer to the driver patch:
//!         since there is no patch in the host - it returns 0
//
//*****************************************************************************
char *sendDriverPatch ( unsigned long *Length )
{
	*Length = 0;
	return NULL;
}

//*****************************************************************************
//
//! sendBootLoaderPatch
//!
//! @param  pointer to the length
//!
//! @return none
//!
//! @brief  The function returns a pointer to the boot loader patch:
//!         since there is no patch in the host - it returns 0
//
//*****************************************************************************
char *sendBootLoaderPatch ( unsigned long *Length )
{
	*Length = 0;
	return NULL;
}
/**************************************************************************/
/*!

 */
/**************************************************************************/
void WlanInterruptEnable ( void )
{
	__PROLOG("WlanInterruptEnable\n");
	//_delay_us(100);
	ccspi_int_enabled = 1;
	
	// External interrupt SPI_IRQ from CC3000 on PC2
	PORTC.PIN2CTRL =  PORT_ISC_FALLING_gc;
	// PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	
	PORTC.INT0MASK = WLAN_SPI_IRQ_bm;
	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;

	//attachInterrupt(g_IRQnum, SPI_IRQ, FALLING);

}

/**************************************************************************/
/*!

 */
/**************************************************************************/
void WlanInterruptDisable(void)
{
	__PROLOG("WlanInterruptDisable\n");
		
	ccspi_int_enabled = 0;

	// External interrupt SPI_IRQ from CC3000 on PC2
	PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_LEVEL_gc;
	// PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	
	bit_clr( PORTC.INT0MASK , WLAN_SPI_IRQ_bp );

	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;

//  detachInterrupt(g_IRQnum);
}
/**************************************************************************/
/*!

 */
/**************************************************************************/
long ReadWlanInterruptPin ( void )
{
	return ((PORTC.IN & WLAN_SPI_IRQ_bm ) == 0 ) ? 1: 0;
}

void WriteWlanPin( unsigned char val )
{	
	if ( val ) {
		PORTC.OUTSET = VBAT_SW_EN_bm;
	} else {
		PORTC.OUTCLR = VBAT_SW_EN_bm;
	}
}

unsigned char patchReq = 0;

int wlan_Setup ( void )
{
	__PROLOG("wlan_Setup\n");

	init_spi();

	// SPI_IRQ is input from CC3000
		
	wlan_init ( CC3000_UsynchCallback,
	            sendWLFWPatch, sendDriverPatch, sendBootLoaderPatch,
	            ReadWlanInterruptPin,
	            WlanInterruptEnable,
	            WlanInterruptDisable,
	            WriteWlanPin );


  wlan_start(patchReq);
  	
	  
	  circular_buffer_puts("wlan_Setup(config)\n");
	  
  // Check if we should erase previous stored connection details
  // (most likely written with data from the SmartConfig app)
  if (!useSmartConfigData)
  {
	  // Manual connection only (no auto, profiles, etc.)
	  wlan_ioctl_set_connection_policy(0, 0, 0);
	  // Delete previous profiles from memory
	  wlan_ioctl_del_profile(255);
  }
  else
  {
	  // Auto Connect - the C3000 device tries to connect to any AP it detects during scanning:
	  // wlan_ioctl_set_connection_policy(1, 0, 0)
	  
	  // Fast Connect - the CC3000 device tries to reconnect to the last AP connected to:
	  // wlan_ioctl_set_connection_policy(0, 1, 0)

	  // Use Profiles - the CC3000 device tries to connect to an AP from profiles:
	  wlan_ioctl_set_connection_policy(0, 0, 1);
  }
  return 0;
}
