/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "LUFA/Common/Common.h"

#include "LUFA/LUFA/Drivers/USB/USB.h"

#include "LUFA\LUFA\Platform\XMEGA\ClockManagement.h"
#include "user_board.h"

#include "HID.h"
#include "spi_driver.h"

void USB_ConfigureClock ( void );

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[GENERIC_REPORT_SIZE];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Generic_HID_Interface = {
	.Config =
	{
		.InterfaceNumber              = INTERFACE_ID_GenericHID,
		.ReportINEndpoint             =
		{
			.Address              = GENERIC_IN_EPADDR,
			.Size                 = GENERIC_EPSIZE,
			.Banks                = 1,
		},
		.PrevReportINBuffer           = PrevHIDReportBuffer,
		.PrevReportINBufferSize       = sizeof ( PrevHIDReportBuffer ),
	},
};





/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware ( void )
{


#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~ ( 1 << WDRF );
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set ( clock_div_1 );
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL ( CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU );
	XMEGACLK_SetCPUClockSource ( CLOCK_SRC_PLL );

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator ( CLOCK_SRC_INT_RC32MHZ );
	XMEGACLK_StartDFLL ( CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB );

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;

#endif

	//outputs
	PORTC.DIRSET = WLAN_SS_bm;
	PORTC.DIRSET = WLAN_MOSI_bm;
	PORTC.DIRSET = WLAN_SCK_bm;
	PORTC.DIRSET = VIO_bm;

	//Inputs
	PORTC.DIRCLR = WLAN_MISO_bm;

	/* Hardware Initialization */
	USB_Init( );
}

void dump_pin ( register8_t port, const unsigned char bitmask )
{
	if ( port & bitmask )
	{ circular_buffer_put ( '1' ); }

	else
	{ circular_buffer_put ( '0' ); }
}

void dump_register ( const unsigned char *const str, register8_t port )
{
	unsigned char i;

	if ( str ) {
		circular_buffer_puts ( str );
	}

	circular_buffer_puts ( "\n" );

	circular_buffer_puts ( "01234567\n" );

	for ( i  = 0 ; i < 8 ; i++ ) {
		dump_pin ( port, 1<<i );
	}

	circular_buffer_puts ( "\n" );

}
extern void wlan_Setup ( void );

int main ( void )
{

	if ( 0 ) {
		PORTC.DIRSET = WLAN_MOSI_bm;
		PORTC.DIRSET = WLAN_SS_bm;
		PORTC.DIRSET = WLAN_SCK_bm;

		PORTC.DIRCLR = WLAN_MISO_bm;

		while ( 1 ) {

			PORTC.OUTSET = WLAN_MOSI_bm;
			PORTC.OUTSET = WLAN_SS_bm;
			PORTC.OUTSET = WLAN_SCK_bm;

			_delay_ms ( 2  );

			PORTC.OUTCLR = WLAN_MOSI_bm;
			PORTC.OUTCLR = WLAN_SS_bm;
			PORTC.OUTCLR = WLAN_SCK_bm;

			_delay_ms ( 2  );
		}
	}

	// early need
	circular_buffer_init();

	// setup ports
	board_init();

	// Setup  clock and USB
	SetupHardware();

	LED_Init();

	GlobalInterruptEnable();

	// Insert application code here, after the board has been initialized.

	_delay_ms ( 3000 );

	circular_buffer_puts ( "booting WLAN\n" );


	wlan_Setup();

	volatile unsigned short tempOne = 0;
	float offset;

	circular_buffer_puts ( "LED starting\n" );

	///------------------------

	while ( 1 ) {

		for ( offset = 0; offset < 360; offset += 0.5 ) {
			RGBscan ( 4095, 0, 0, offset );
			//_delay_us(1);
		}

		for ( offset = 0; offset < 360; offset += 0.5 ) {
			RGBscan ( 0, 4095, 0, offset );
			_delay_us ( 1 );
		}

		for ( offset = 0; offset < 360; offset += 0.5 ) {
			RGBscan ( 0, 0, 4095, offset );
			//_delay_us(1);
		}

		for ( offset = 0; offset < 360; offset += 0.5 ) {
			RGBscan ( 4095, 4095, 4095, offset );
			//_delay_us(1);
		}

		SetRGB ( 4095, 0, 0 );

		_delay_ms ( 5000 );

		SetRGB ( 0, 4095, 0 );

		_delay_ms ( 5000 );

		SetRGB ( 0, 0, 4095 );

		_delay_ms ( 5000 );

		SetRGB ( 4095, 4095, 4095 );
		_delay_ms ( 5000 );

	}


	for ( ;; ) {

	}

}
void SendNextReport ( void );
uint8_t cc3000_read_irq_pin ( void );

/*
 * USB housekeeper.
 */
void timer ( void )
{
	//circular_buffer_put('A'+cc3000_read_irq_pin());

	HID_Device_USBTask ( &Generic_HID_Interface );

	SendNextReport();

	USB_USBTask();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect ( void )
{

}
/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect ( void )
{
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged ( void )
{
	bool ConfigSuccess = true;

	ConfigSuccess &= HID_Device_ConfigureEndpoints ( &Generic_HID_Interface );

	USB_Device_EnableSOFEvents();

	//LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest ( void )
{
	HID_Device_ProcessControlRequest ( &Generic_HID_Interface );
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame ( void )
{
	HID_Device_MillisecondElapsed ( &Generic_HID_Interface );
}

/** HID class driver callback function for the creation of HID reports to the host.
*
*  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
*  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
*  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
*  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
*  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
*
*  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
*/
bool CALLBACK_HID_Device_CreateHIDReport (
    USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
    uint8_t* const ReportID,
    const uint8_t ReportType,
    void* ReportData,
    uint16_t* const ReportSize )
{

	uint8_t* Data        = ( uint8_t* ) ReportData;
	uint8_t  CurrLEDMask = 0xaa;

	Data[0] = ( CurrLEDMask );
	Data[1] = ( CurrLEDMask );
	Data[2] = ( CurrLEDMask );
	Data[3] = ( CurrLEDMask );

	*ReportSize = GENERIC_REPORT_SIZE;

	return false;
}
/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport ( USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
        const uint8_t ReportID,
        const uint8_t ReportType,
        const void* ReportData,
        const uint16_t ReportSize )
{
	uint8_t* Data       = ( uint8_t* ) ReportData;

	switch ( Data[0] ) {

		case 0:
			dump_register ( "PORTC.IN",PORTC.IN );
			break;

		case 1:
			dump_register ( "PORTC.OUT",PORTC.OUT );
			break;

		case 2:
			dump_register ( "PORTC.DIR",PORTC.DIR );
			break;

		case 3:
			dump_register ( "PORTC.PIN6CTRL",PORTC.PIN6CTRL );
			break;

		case 4:
			dump_register ( "PORTC.INTCTRL",PORTC.INTCTRL );
			break;

		case 5:
			dump_register ( "PORTC.INT0MASK",PORTC.INT0MASK );
			break;

		case 6:
			dump_register ( "PORTC.INT1MASK",PORTC.INT1MASK );
			break;

	}
}

/** Sends the next HID report to the host, via the serial  data endpoint. */
void SendNextReport ( void )
{
	static unsigned char SerialBuffer[128];
	unsigned char SendReport;
	int i;

	SendReport = 0;

	///memset(SerialBuffer,0,sizeof( SerialBuffer ));

	// ( how big is the buffer it'll send in one msg
	for ( i = 0 ; i  < sizeof ( SerialBuffer ) ;  i++ ) {

		if ( 0 == circular_buffer_get ( &SerialBuffer[i] ) ) {
			break;
		}

		// got data
		SendReport = 1;
	}

	/* Select the  Report Endpoint */
	Endpoint_SelectEndpoint ( GENERIC_IN_EPADDR );

	/* Check if  Endpoint Ready for Read/Write and if we should send a new report */
	if ( Endpoint_IsReadWriteAllowed() && SendReport ) {
		/* Write  Report Data */
		Endpoint_Write_Stream_LE ( &SerialBuffer, i, NULL );

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
	}
}