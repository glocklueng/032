/*
 * leds.c
 *
 * Created: 5/23/2013 8:13:35 PM
 *  Author: charliex, oled code from abotronics.com
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"
#include "mygccdef.h"
#include "display.h"

static int brite = 1;
static int britedir = 10;
int gMode = 0;

const char VERSION[]    PROGMEM = "FW 1.1";

const unsigned char LOGO[] PROGMEM = {
	'\x7e','\xff','\x81','\x91','\x91','\xf1','\x60','\x00', // 0x0008
	'\x60','\xf4','\x94','\x94','\xfc','\xf8','\x00','\xff', // 0x0010
	'\xff','\x84','\x84','\xfc','\x78','\x00','\x78','\xfc', // 0x0018
	'\x84','\x84','\xfc','\x78','\x00','\x04','\x7f','\xff', // 0x0020
	'\x84','\x84','\x00','\xfc','\xfc','\x08','\x0c','\x0c', // 0x0028
	'\x00','\x78','\xfc','\x84','\x84','\xfc','\x78','\x00', // 0x0030
	'\xf8','\xfc','\x04','\x04','\xfc','\xf8','\x00','\xfd', // 0x0038
	'\xfd','\x00','\x78','\xfc','\x84','\x84','\x84','\x00', // 0x0040
	'\x98','\xbc','\xb4','\xf4','\x64',
};

// check a button

unsigned char mode_switch( void ) 
{
	return 0;
}


void fadeup(void)
{
	return;

	int quit = 0;

	while(quit == 0 ){

		quit = 1;

		for(unsigned int i = 0 ; i < (NUM_TLC5947); i++) {

			if( LEDChannels[ i ] < 4095 ) {
				LEDChannels[ i ]*=2;
				quit = 0;
			}
		}

		WriteLEDArray(NUM_TLC5947);
	}
}

void fade(void)
{
	int quit = 0;
	while(quit == 0 ){

		quit = 1;

		for(unsigned int i = 0 ; i < (NUM_TLC5947); i++) {
			if( LEDChannels[ i ] > 2 ) {
				LEDChannels[i]/=2;
				quit = 0;
			}
		}

		WriteLEDArray(NUM_TLC5947);
	}
}

ISR(TCC0_OVF_vect)
{
	static unsigned int counter = 0;
	
	counter ++ ;
	
	if( counter == 1000 )	{
		counter = 0;
	
		gMode++;
	}
 
 }

void Setup_OLED(void )
{
	// Initialize USARTD0 for OLED
	USARTD0.BAUDCTRLA = 0x01;   // BSEL=1 (SPI clock = 8MHz)
	USARTD0.CTRLC     = 0xC0;   // Master SPI mode,
	USARTD0.CTRLB     = 0x08;   // Enable TX


	USARTD0.CTRLA=(USARTD0.CTRLA & (~(USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm))) | USART_RXCINTLVL_OFF_gc | USART_TXCINTLVL_LO_gc | USART_DREINTLVL_OFF_gc;
}



// From Application Note AVR1003
void CCPWrite( volatile uint8_t * address, uint8_t value ) {
	uint8_t volatile saved_sreg = SREG;
	cli();

	#ifdef __ICCAVR__
	asm("movw r30, r16");
	#ifdef RAMPZ
	RAMPZ = 0;
	#endif
	asm("ldi  r16,  0xD8 \n"
	"out  0x34, r16  \n"
	#if (__MEMORY_MODEL__ == 1)
	"st     Z,  r17  \n");
	#elif (__MEMORY_MODEL__ == 2)
	"st     Z,  r18  \n");
	#else /* (__MEMORY_MODEL__ == 3) || (__MEMORY_MODEL__ == 5) */
	"st     Z,  r19  \n");
	#endif /* __MEMORY_MODEL__ */

	#elif defined __GNUC__
	volatile uint8_t * tmpAddr = address;
	#ifdef RAMPZ
	RAMPZ = 0;
	#endif
	asm volatile(
	"movw r30,  %0"	      "\n\t"
	"ldi  r16,  %2"	      "\n\t"
	"out   %3, r16"	      "\n\t"
	"st     Z,  %1"       "\n\t"
	:
	: "r" (tmpAddr), "r" (value), "M" (CCP_IOREG_gc), "i" (&CCP)
	: "r16", "r30", "r31"
	);

	#endif
	SREG = saved_sreg;
}
uint8_t AWGBuffer[256]; // AWG Output Buffer

void Setup_OSC( void )
{
	
	 OSC.XOSCCTRL = 0xCB;    // Crystal type 0.4-16 MHz XTAL - 16K CLK Start Up time
	 OSC.PLLCTRL = 0xC2;     // XOSC is PLL Source - 2x Factor (32MHz)
	 OSC.CTRL = OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_XOSCEN_bm;
	 _delay_ms(2);
	 // Switch to internal 2MHz if crystal fails
	 if(!testbit(OSC.STATUS,OSC_XOSCRDY_bp)) {   // Check XT ready flag
		 OSC.XOSCCTRL = 0x00;    // Disable external oscillators
		 // Not entering, comment to save
		 //OSC.PLLCTRL = 0x10;     // 2MHz is PLL Source - 16x Factor (32MHz)
	 }
	 OSC.CTRL = OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_PLLEN_bm | OSC_XOSCEN_bm;
	 _delay_ms(2);
	 CCPWrite(&CLK.CTRL, CLK_SCLKSEL_PLL_gc);    // Switch to PLL clock
	 // Clock OK!
	 OSC.CTRL = OSC_RC32MEN_bm | OSC_PLLEN_bm | OSC_XOSCEN_bm;    // Disable internal 2MHz
	 

}

// Read out calibration byte.
uint8_t SP_ReadCalibrationByte(uint8_t location) {
	uint8_t result;
	/* Load the NVM Command register to read the calibration row. */
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	result = pgm_read_byte(location);
	/* Clean up NVM Command register. */
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;
	return result;
}

int main(void)
{
	unsigned short l = 40;
	unsigned short ld = 1;

	// Power reduction: Stop unused peripherals
	PR.PRGEN = 0x18;        // Stop: AES, EBI
	PR.PRPC  = 0x74;        // Stop: TWI, USART0, USART1, HIRES
	PR.PRPD  = 0x7C;        // Stop: TWI, USART0, USART1, SPI, HIRES
	PR.PRPE  = 0x60;        // Stop: TWI, USART1, (XMEGA bug - HIRES cannot be turned off)
	PR.PRPF  = 0x78;        // Stop: TWI, USART0, USART1, SPI, (XMEGA bug - HIRES cannot be turned off)

	// USB Clock
	OSC.DFLLCTRL	= OSC_RC32MCREF_USBSOF_gc; // Configure DFLL for 48MHz, calibrated by USB SOF
	DFLLRC32M.CALB	= SP_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, USBRCOSC));
	DFLLRC32M.COMP1 = 0x1B; // xMega AU manual, p41
	DFLLRC32M.COMP2 = 0xB7;
	DFLLRC32M.CTRL	= DFLL_ENABLE_bm;

	Setup_OSC();

	// PORTS CONFIGURATION
	// Initial value PORTA.DIR       = 0x00; // CH2, CH1, 1V, K1, K2, K3, K4, REF
	PORTA.PIN1CTRL  = MENUPULL; // Pull up or pull down on pin PA1
	PORTA.PIN2CTRL  = 0x18; // Pull up on pin PA2
	PORTA.PIN3CTRL  = 0x18; // Pull up on pin PA3
	PORTA.PIN4CTRL  = 0x18; // Pull up on pin PA4
		PORTA.PIN5CTRL  = 0x07; // Input Disable on pin PA5
	PORTA.PIN6CTRL  = 0x07; // Input Disable on pin PA6
	PORTA.PIN7CTRL  = 0x07; // Input Disable on pin PA7
	PORTA.INTCTRL   = 0x02; // PORTA will generate medium level interrupts
	PORTA.INT0MASK  = 0x1E; // PA4, PA3, PA2, PA1 will be the interrupt 0 sources
	PORTB.DIR       = 0x0B; // RES, AWG, D/C, R/W
   
	// Initial Value PORTB.OUT       = 0x00; //
	// Initial Value PORTC.DIR       = 0x00; // LOGIC
	PORTC.INT0MASK  = 0x01; // PC0 (SDA) will be the interrupt 0 source
	PORTC.INT1MASK  = 0x80; // PC7 (SCK) will be the interrupt 1 source
 
	PORTD.DIR		= 0xFF;  // LEDs
	PORTCFG.MPCMASK = 0xFF; // Configure all pins on PORTD the same way
	PORTD.PIN0CTRL	= 0x00;  // Pull up on pin PortD (Keypad and K2-K5 buttons)
	PORTD.INTCTRL	= 0x00;  // PORTD will generate no low level interrupts
	PORTD.INT0MASK	= 0x00;

	//PORTD.OUT     = 0x00;
	PORTD.DIR       = 0x1F; // USB, EXT, GREEN, TX, RX, CLK, RED
	PORTD.OUT       = 0x04; // LCD voltage off
	PORTE.DIR       = 0x09; // TX, RX, RTS (input), CTS (power external board)
	PORTE.PIN1CTRL  = 0x18; // Pull up on pin PE1 (RTS)
	PORTE.OUT       = 0x01; // Power to external board
	PORTCFG.VPCTRLA = 0x41; // VP1 Map to PORTE, VP0 Map to PORTB
	//PORTCFG.VPCTRLB = 0x32; // VP3 Map to PORTD, VP2 Map to PORTC
 	
	Setup_OLED();

	// Initialize USARTE0 for External Interface Port
	USARTE0.BAUDCTRLA = 0x17;   // BSCALE = -6, BSEL = 1047
	USARTE0.BAUDCTRLB = 0xA4;   // ==> 115211 bps (~115.2kbps)
	USARTE0.CTRLC     = 0x03;   // Async, No Parity, 1 stop bit, 8 data bits
	USARTE0.CTRLB     = 0x18;   // Enable RX and TX
	USARTE0.CTRLA     = 0x20;   // Enable RX interrupt

	// Event System
	EVSYS.CH0MUX    = 0xE0;     // Event CH0 = TCE0 overflow used for ADC
	EVSYS.CH1MUX    = 0x20;     // Event CH1 = ADCA CH0 conversion complete
	//EVSYS.CH2MUX    = 0x6D;     // Event CH2 = PORTD Pin 5 (External Trigger)
	EVSYS.CH3MUX    = 0xD8;     // Event CH3 = TCD1 overflow used for DAC
	EVSYS.CH4MUX    = 0xC0;     // Event CH4 = TCC0 overflow used for freq. measuring
	EVSYS.CH5MUX    = 0xC8;     // Event CH5 = TCC1 overflow used for freq. measuring
	EVSYS.CH6MUX    = 0x8F;     // Event CH6 = CLKPER / 32768
	EVSYS.CH7MUX    = 0xD0;     // Event CH7 = TCD0 underflow

	// DAC
	DACB.CTRLB        = 0x01;   // CH0 auto triggered by an event
	DACB.CTRLC        = 0x11;   // Use AREFA (2.0V), data is left adjusted
	DACB.EVCTRL       = 0x03;   // Event CH3 triggers the DAC Conversion
	DACB.TIMCTRL      = 0x50;   // Minimum 32 CLK between conversion (1uS)
	//DACB.CH0GAINCAL = eeprom_read_byte(&EEDACgain);      // Load DAC gain calibration
	//DACB.CH0OFFSETCAL = eeprom_read_byte(&EEDACoffset);  // Load DAC offset calibration
	DACB.CTRLA = 0x05;          // Enable DACB and CH0

	// DMA for DAC
	DMA.CH3.ADDRCTRL  = 0xD0;   // Reload after transaction, Increment source
	DMA.CH3.TRIGSRC   = 0x25;   // Trigger source is DACB CH0
	DMA.CH3.TRFCNT    = 256;    // AWG Buffer is 256 bytes
	DMA.CH3.SRCADDR0  = (((uint16_t) AWGBuffer)>>0*8) & 0xFF;
	DMA.CH3.SRCADDR1  = (((uint16_t) AWGBuffer)>>1*8) & 0xFF;
	//	DMA.CH3.SRCADDR2  = 0;
	DMA.CH3.DESTADDR0 = (((uint16_t)(&DACB.CH0DATAH))>>0*8) & 0xFF;
	DMA.CH3.DESTADDR1 = (((uint16_t)(&DACB.CH0DATAH))>>1*8) & 0xFF;
	//	DMA.CH3.DESTADDR2 = 0;
	DMA.CH3.CTRLA     = 0b10100100;     // Enable CH3, repeat mode, 1 byte burst, single

	DMA.CTRL          = 0x80;           // Enable DMA, single buffer, round robin

	// Interrupt Configuration
	
	/* Enable HIGH level interrupts in the PMIC. */

	PMIC.CTRL		= PMIC_HILVLEN_bm;

	TCC0.CTRLA		= TC_CLKSEL_DIV8_gc;
	TCC0.PER		= 185;
	TCC0.CNT		= 0;
	TCC0.CTRLFSET = 0x0; /* count upwards, but don't start to interrupt yet */
	TCC0.INTCTRLA = TC_OVFINTLVL_HI_gc; /* enable interrupts */

	PMIC.CTRL = 0x07;       // Enable High, Medium and Low level interrupts
    
	sei();      // Enable global interrupts
	
	// Initialize LCD
	// wrong baud rate?
	
	//GLCD_LcdInit();

//	memcpy_P(display_buffer+286,  &LOGO, 69);
	//GLCD_setting();
	//tiny_printp(50,7,VERSION);
	  
	// setup the TLC
	LED_Init();
  
		// push out some data
	WriteLEDArray(NUM_TLC5947);

	l = 0;
	ld = 40;

	gMode = 0;
	 
 // i wrote these modes for the blinky ball and tron disc, so they'll need fixing up for the badge.
	while( 1 )					// Loop Here
	{

		if( mode_switch( ) ){
			brightnessShift++;
			brightnessShift%=6;
			_delay_ms( 100 );
		}


		// mode
		// 2 = all fade on off
		// 3 = none
		// 4 = not working
		// 5 = smooth around scroll
		// 6 = err
		// 7 = nothing
		// 8 = nothing
		// 9 = ok
		// 10 = none
		// 11 = flash pulse
		// 12 = fade pulse
		// 13 = not much
		// 14 =

		switch( gMode) {

			case 0:
				{
					unsigned int offset ;
					offset = 0;

					{
						for(float i =1 ; i < 30 ;i+=.08){

							offset = (int)( (1.0f+sinf(i)) *60.0f);

							LEDscan2(4096,(offset*2),1);
							WriteLEDArray(1);
							_delay_ms(10);

						}
					}
				}

				l = 0;
				ld = 40;

				break;
		
			case 1:
			{
	
				Clear( l );

				l += ld;

				if( l == 4000 )
				ld = -40;

				if( l == 40 )
				ld = 40;

				WriteLEDArray(NUM_TLC5947);
				_delay_ms(10);

			}
			break;

			case 2:

				fadeup();

				for(int i =0 ; i < 2 ;i++)
				for (int offset = 240; offset < 300; offset += 1) {

					LEDscan(4096, offset,1);
				}



				break;		

			case 3:

				gMode++;
			break;

			case 4:
				gMode++;
				break;

			case 5:

				fadeup();

				for(int i =0 ; i < 5 ;i++) {

					for (int offset = 240; offset < 300; offset += 1) {

						LEDscan2(4096, offset,1);
						WriteLEDArray(1);
						_delay_ms( 20 );
					}
				}

				break;

			case 6:
				
				for(int i =0 ; i < 4 ;i++)
				for (int offset = 240; offset < 300; offset += 1) {

					LEDscan2(4096, offset,i/2);

					WriteLEDArray(i/2);
					_delay_ms( 5 );
				}
			

				break;
			case 7:
			
				{

					for(int i =1 ; i < 6 ;i++)
					for (int offset = 240; offset < 300; offset += 2) {

						//LEDscan2(4096, offset,1);
						LEDscan2(4096/i,360-offset,1);
						WriteLEDArray(1);

					}
				}
				break;
				
			case 8:
		
				{

					fade();


					for(unsigned int i =1 ; i < 6 ;i++){

						for (unsigned int offset = 240; offset < 300; offset += 16) {

							LEDscan2(4096/i,offset,1);
							for(int k =1 ; k <= 16 ;k++)
							WriteLEDArray(1);

						}
					}
				}
				break;

			case 9:
			{
				unsigned int offset ;
				offset = 0;
				float mil;

				mil = 50+(rand()%70);

				{

					for(float i =1 ; i < 10 ;i+=.1){

						offset = (int)( (1.0f+sinf(i)) *mil);

						LEDscan2(4096,(offset*2),1);
						WriteLEDArray(1);
						
						for(int id=0;id<6;id++)
							_delay_ms(6);

					}
				}

			}
			break;

			case 10:
			{

				fade();

				for(unsigned int j =0 ; j < 50 ;j++) {
					for(unsigned int i =0 ; i < NUM_TLC5947 ;i++){
						LEDChannels[i]=rand()%2047;
					}


					WriteLEDArray(NUM_TLC5947);
				}

			}
			
			brite =1;
			britedir=10;
			
			break;


			case 11:
			for(unsigned int i=0;i<80;i+=4) {
				//Clear(i);
				brite+=britedir;
				if (brite>6050) britedir=-30;
				if (brite<40) britedir=30;

				LEDscan2(brite,i,1);
				LEDscan2Add(brite,i-80,1);
				for(int t = 0 ; t < NUM_TLC5947 ; t++ )
				WriteLEDArray(1);
				_delay_ms(20);
			}

			break;

			case 12:
			{

				for(unsigned int i =1 ; i < 6 ;i++){
					LEDscan(0,0,1);
					for (unsigned int offset = 0; offset < 360; offset += 16) {

						LEDscan2Add(4096/i,offset,1);
						for(int k =1 ; k <= 16 ;k++) {
							WriteLEDArray(1);
						}
						_delay_ms(10);
					}
				}
			}
			break;

			case 13:
			for (int r=0;r<NUM_TLC5947;r+=16){
				for (int x=r;x<r+15;x+=1) {
					LEDChannels[x]=rand()/1024;
					if (LEDChannels[x]>30) LEDChannels[x]=4095;
					else if (LEDChannels[x]<10) LEDChannels[x]=0;
				}
				for(int i=0;i<NUM_TLC5947;i++){
					WriteArrayOffset(NUM_TLC5947,i);
					
				}
			}


			gMode = 0;

			break;

			case 14:

			/// back to first mode
			gMode = 0;

			break;
		}
	}

}