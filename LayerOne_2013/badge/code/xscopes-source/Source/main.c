/*****************************************************************************

XMEGA Oscilloscope and Development Kit

Gabotronics
July 2012

Copyright 2012 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License

ATXMEGA32A4U

Compiled with GCC, -Os optimizations

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

// TODO & Optimizations:
/*      Share buffer between CH1 and CH2
        Make Srate signed, so tests like Srate>=11 become Srate>=0
        Pointer to current channel selected in menus, to reduce redundancy
        Logic Analyzer more SPS, with DMA
		Sniffer: LED on buffer full
        RED LED too bright
        Force trigger, Trigger timeout in menu
		USB Frame counter
        Disable gain on CH1 if Gain=1, to allow simultaneous sampling
        Channel math in meter mode
        Elastic adds one
        Sometimes the display does not shut off when the device enters sleep
        SPI & UART Sniffer in assembly
        UART sniffer number of bits
        Calibration data in User Signature Row
		USE NVM functions from BOOT */
// TODO When 64k parts come out:
/*      Gain Calibration
        Setting profiles
        Continuous data to USB from ADC
        Independent CH1 and CH2 Frequency measurements, up to 1Mhz
    	Sniffer JTAG, 1 Wire, MIDI
        1v/octave (CV/Gate) AWG control
    	Menu timeout
        Frequency counter on any logic pin
        RMS
        Arbitrary expression on AWG
        DAC Calibration
	    Use DMA for USART PC transfers
        Dedicated Bode plots
        Dedicated VI Curve
	    12bit with slow sampling
        Horizontal cursor on FFT
        16MSPS for logic data, 1/sinc(x) for analog
	    Show menu title */
// TODO Expansion boards: SD, Keyboard, Display, RS232, MIDI, Video, RAM

/* Hardware resources:
    Timers:
        RTC   Clock for sleep timeout
        TCC0  Frequency counter time keeper
        TCC0L Controls the auto trigger
        TCC0H Auto key repeat
        TCC1  Counts post trigger samples
              UART sniffer time base
              Frequency counter low 16bits
        TCD0L 0.04096mS period - 24.4140625 Hz
	    TCD0H Controls LCD refresh rate
	    TCD1  Overflow used for AWG
        TCE0  Controls Interrupt ADC, srate: 6, 7, 8, 9, 10
              Fixed value for slow sampling
              Frequency counter high 16bits
    Events:
	    CH0 TCE0 overflow used for ADC
	    CH1 ADCA CH0 conversion complete
        CH2 EXT Trigger for freq. measuring
        CH3 TCD1 overflow used for DAC
        CH4 TCC0 overflow used for freq. measuring
        CH5 TCC1 overflow used for freq. measuring
        CH6 CLKPER / 32768
        CH7 TCD0L underflow: 0.04096mS period - 24.4140625 Hz
	DMA:
	    CH0 ADC CH0
	    CH1 ADC CH1
	    CH2 Port CHD
	    CH3 DAC
    USART:
	    USARTD0 for OLED
	    USARTC0 for sniffer
	    USARTE0 for External Interface Port
	RAM:
	    1024:   Display buffer
         128:   Endpoint0 out + in
         128:   Endpoint1 out + in
         768:   CH1+CH2+CHD Data
         256:   AWG Buffer
          31:   M
		1536:   Temp (FFT, logic sniffer)
        -----
        3871    Total + plus some global variables
    Interrupt Levels:
        TCE0:           High        Slow Sampling
        PORTC INT0      Medium      I2C sniffer
        PORTC_INT1      Medium      SPI sniffer
        TCC1            Medium      UART sniffer
        USARTE0:        Medium      Serial port
        USB BUSEVENT    Medium      USB Bus Event
        USB_TRNCOMPL    Medium      USB Transaction Complete
        PORTA INT0:     Medium      keys
        TCC0L:          Low         auto trigger
        TCC0H:          Low         auto keys
        RTC:            Low         sleep timeout
        TCD0H:          Low         Wake from sleep (LCD refresh rate)
*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/fuse.h>
#include "main.h"
#include "mso.h"
#include "logic.h"
#include "awg.h"
#include "USB\usb_xmega.h"
#include "USB\Descriptors.h"

FUSES = {
    .FUSEBYTE1 = 0x00,  /* Watchdog Configuration */
    .FUSEBYTE2 = 0xBD,  /* Reset Configuration */
    .FUSEBYTE4 = 0xF7,  /* Start-up Configuration */
    .FUSEBYTE5 = 0xE9,  /* EESAVE and BOD Level */
};
/*  Fuses:
    WDWP = 8CLK
    WDP = 8CLK
    BOOTRST = BOOTLDR
    TOSCSEL = XTAL
    BODPD = SAMPLED
    RSTDISBL = [ ]
    SUT = 4MS
    WDLOCK = [ ]
    BODACT = CONTINUOUS
    EESAVE = [ ]
    BODLVL = 2V8    */

uint8_t SP_ReadCalibrationByte(uint8_t location);
static inline void Restore(void);
//uint16_t readVCC(void);

// Big buffer to store large but temporary data
TempData Temp;

// Variables that need to be saved in NVM
NVMVAR M;

// EEProm variables
uint8_t EEMEM EESleepTime = 32;     // Sleep timeout in minutes
uint8_t EEMEM EEDACgain   = 0;      // DAC gain calibration
uint8_t EEMEM EEDACoffset = 0;      // DAC offset calibration

static inline void Calibrate(void);
//static void CalibrateDAC(void);
void SimpleADC(void);
static inline void LoadEE(void);                  // Load settings from EEPROM

int main(void) {
    // Power reduction: Stop unused peripherals
    PR.PRGEN = 0x18;        // Stop: AES, EBI
    PR.PRPA  = 0x04;        // Stop: DAC
    PR.PRPB  = 0x03;        // Stop: ADC, AC
    PR.PRPC  = 0x7C;        // Stop: TWI, USART0, USART1, SPI, HIRES
    PR.PRPD  = 0x6C;        // Stop: TWI,       , USART1, SPI, HIRES
    PR.PRPE  = 0x6C;        // Stop: TWI,       , USART1, SPI, HIRES

    // Clock Settings
	// USB Clock
	OSC.DFLLCTRL = OSC_RC32MCREF_USBSOF_gc; // Configure DFLL for 48MHz, calibrated by USB SOF
	DFLLRC32M.CALB = SP_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, USBRCOSC));
	DFLLRC32M.COMP1 = 0x1B; // Xmega AU manual, p41
	DFLLRC32M.COMP2 = 0xB7;
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;

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
    PORTD.DIR       = 0x1F; // USB, EXT, GREEN, DAT, TP, CLK, RED
    PORTD.PIN5CTRL  = 0x01; // Sense rising edge (Freq. counter)
    PORTD.OUT       = 0x04; // LCD voltage off
    PORTE.DIR       = 0x09; // TX, RX, RTS (input), CTS (power external board)
    PORTE.PIN1CTRL  = 0x18; // Pull up on pin PE1 (RTS)
	PORTE.OUT       = 0x01; // Power to external board
    PORTCFG.VPCTRLA = 0x41; // VP1 Map to PORTE, VP0 Map to PORTB
    PORTCFG.VPCTRLB = 0x32; // VP3 Map to PORTD, VP2 Map to PORTC

    // Initialize USARTD0 for OLED
    USARTD0.BAUDCTRLA = 0x01;   // BSEL=1 (SPI clock = 8MHz)
    USARTD0.CTRLC     = 0xC0;   // Master SPI mode,
    USARTD0.CTRLB     = 0x08;   // Enable TX

    // Initialize USARTE0 for External Interface Port
    USARTE0.BAUDCTRLA = 0x17;   // BSCALE = -6, BSEL = 1047
    USARTE0.BAUDCTRLB = 0xA4;   // ==> 115211 bps (~115.2kbps)
    USARTE0.CTRLC     = 0x03;   // Async, No Parity, 1 stop bit, 8 data bits
    USARTE0.CTRLB     = 0x18;   // Enable RX and TX
	USARTE0.CTRLA     = 0x20;   // Enable RX interrupt

    // Event System
    EVSYS.CH0MUX    = 0xE0;     // Event CH0 = TCE0 overflow used for ADC
    EVSYS.CH1MUX    = 0x20;     // Event CH1 = ADCA CH0 conversion complete
    EVSYS.CH2MUX    = 0x6D;     // Event CH2 = PORTD Pin 5 (External Trigger)
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

    LoadEE();        // Load EE settings

    // Interrupt Configuration
    PMIC.CTRL = 0x07;       // Enable High, Medium and Low level interrupts
    sei();      // Enable global interrupts

	USB_ResetInterface();

    // Initialize LCD
    GLCD_LcdInit();
    memcpy_P(display_buffer+286,  &LOGO, 69);   // Gabotronics
	GLCD_setting();
    tiny_printp(50,7,VERSION);

    ADCA.CTRLA          = 0x01;     // Enable ADC
    ADCA.REFCTRL        = 0x20;     // REF = AREF (2V)
    // ADC Linearity calibration
    ADCA.CALL           = SP_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) );
    ADCA.CALH           = SP_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) );
    ADCA.CH0.CTRL       = 0x03;     // Differential input with gain
    ADCA.CH0.MUXCTRL    = 0x2A;     // Channel 0 input: ADC5 pin - ADC6 pin
    ADCA.CH1.CTRL       = 0x03;     // Differential input with gain
    ADCA.CH1.MUXCTRL    = 0x2B;     // Channel 1 input: ADC5 pin - ADC7 pin
    ADCA.CH2.MUXCTRL    = 0x10;     // Channel 2 input: VCC/10
    ADCA.CH3.MUXCTRL    = 0x28;     // PA5 (2.048 / 2)

    if(CLK.CTRL & CLK_SCLKSEL_RC32M_gc) {   // Clock error?
        tiny_printp(0,7,PSTR("NO XT"));
    }
    show_display();
	_delay_ms(3000);

    // TCD0: TCD0H controls LCD refresh rate, TCD0L is the slow clock generator
    TCD0.CTRLA = 0x0E;      // Event CH6 (CLKPER / 32768)
    TCD0.CTRLE = 0x02;      // Timer TCD0 Split Mode
    TCD0.PERL = 39;         // 40.96mS -  24.4140625 Hz
    TCD0.PERH = 0;          // Without this, there would be a small pause on the first cycle

    // RTC Clock Settings
    /* Set 1.024kHz from internal 32.768kHz RC oscillator */
    uint8_t i;
    CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;
    i=eeprom_read_byte(&EESleepTime);
    // Check if KD pressed
    if(!testbit(PORTA.IN,1)) setbit(Misc,bigfont);   // Use bigfont bit to enter settings
    Key=0;  // Without this, you need to press KD before power up
    while(testbit(Misc, bigfont)) {
        uint8_t Volt;
        WaitRefresh();
        clr_display();
        ADCA.PRESCALER    = 0x07;   // Prescaler 512 (62.5kHZ ADC clock)
        ADCA.CTRLB        = 0x14;   // signed mode, no free run, 8 bit
        // 2.048V measure
        ADCA.REFCTRL      = 0x20;   // REF= VCC/1.6 (~2.06V)
        ADCA.CH3.CTRL     = 0x81;   // Start conversion, single ended input
        _delay_ms(1);
        ADCA.REFCTRL      = 0x20;   // REF = AREF (2V)
        Volt = ADCA.CH3.RESL;
        printF(64,1,(uint32_t)Volt*3248);     // Convert to volts (V * 2.0625*2/127)
//        lcd_goto(0,0); printhex(VPORT2.IN);   // Shows the logic input data
        //tiny_printp(0,3,PSTR("rev")); GLCD_Putchar('A'+MCU.REVID);
        tiny_printp(0,6,PSTR("OFFSET       SLEEP:      RESTORE"));
        u8CursorX=58;   // Already have a new line from previous print
        if(i) printN(i);    // Sleep timeout
        else GLCD_Putchar('-');
        if(testbit(Key, userinput)) {
            clrbit(Key, userinput);
            if(testbit(Key,KA)) Calibrate();
            if(testbit(Key,KB)) i+=16;
            if(testbit(Key,KC)) Restore();
            if(testbit(Key,KD)) {
                eeprom_write_byte(&EESleepTime, i);
                clrbit(Misc,bigfont);   // set bit to normal value
            }
        }
        show_display(); WaitDisplay();
    }
    Key=0;
    while(RTC.STATUS & RTC_SYNCBUSY_bm);  // Wait for RTC / Main clock sync
    RTC.PER = (uint16_t)(i)*60;
    RTC.CNT = 0;
    if(i) {
        RTC.INTCTRL = 0x01;     // Generate low level interrupt
        RTC.CTRL = 0x07;        // Divisor 1024 (1 second per count)
    }
    for(;;) MSO();              // go to MSO
    return 0;
}

// Waits for the DMA to complete (the DMA's ISR will SET LCD_CS)
void WaitDisplay(void) {
    uint8_t n=0;
    while(!testbit(LCD_CTRL,LCD_CS)) {   // Wait for transfer complete
        _delay_us(8);
        n++;
        if(n==0) break;     // timeout ~ 2.048mS
    }
    OFFGRN();
    OFFRED();    
}

// Limits the display's refresh rate
void WaitRefresh(void) {
    while(!testbit(TCD0.INTFLAGS, TC2_HUNFIF_bp)) {   // wait for refresh timeout
        if(testbit(Key, userinput)) break;       // cancel if user input
    }
    setbit(TCD0.INTFLAGS, TC2_HUNFIF_bp);
}

// Tactile Switches - This is configured as medium level interrupt
ISR(PORTA_INT0_vect) {
    uint8_t i,in,j=0;
    // Debounce: need to read 10 consecutive equal numbers
    OFFGRN();   // Avoid having the LED on during the interrupt
    OFFRED();
    for(i=25; i>0; i--) {
        _delay_ms(1);
		in = PORTA.IN & 0x1E;       // Read port
		if(j!=in) { j=in; i=10; }   // Value changed
	}
    Key=0;
    if(!testbit(in,1)) setbit(Key, KD); // Menu key
    #ifdef PORTABLE
        if(testbit(Display, flip)) {
            if(!testbit(in,4)) setbit(Key,KA);
            if(!testbit(in,3)) setbit(Key,KB);
            if(!testbit(in,2)) setbit(Key,KC);
        }
        else {
            if(!testbit(in,4)) setbit(Key,KC);
            if(!testbit(in,3)) setbit(Key,KB);
            if(!testbit(in,2)) setbit(Key,KA);
        }
    #else
        if(testbit(Display, flip)) {
            if(!testbit(in,4)) setbit(Key,KC);
            if(!testbit(in,3)) setbit(Key,KB);
            if(!testbit(in,2)) setbit(Key,KA);
        }
        else {
            if(!testbit(in,4)) setbit(Key,KA);
            if(!testbit(in,3)) setbit(Key,KB);
            if(!testbit(in,2)) setbit(Key,KC);
        }
    #endif
    if(Key) {
        setbit(MStatus, update);         // Valid key
        setbit(Key, userinput);
        // TD0H used for auto repeat key
        if(TCC0.CTRLE!=0) {             // Not doing a frequency count
            TCC0.CNTH = 24;                             // Restart timer
            setbit(TCC0.INTFLAGS, TC2_HUNFIF_bp);       // Clear trigger timeout interrupt
            TCC0.INTCTRLA |= TC2_HUNFINTLVL_LO_gc;      // Enable Auto Key interrupt
        }
    }
    else clrbit(Misc,keyrep);
    RTC.CNT=0;  // Clear screen saver timer
}

// Auto repeat key
ISR(TCC2_HUNF_vect) {
    TCC0.INTCTRLA &= ~TC2_HUNFINTLVL_LO_gc;         // Disable Auto Key interrupt
    if(Key) setbit(Misc,keyrep);
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

// Calibrate offset, inputs must be connected to ground
static inline void Calibrate(void) {
    int8_t  *q1, *q2, avrg8;  // temp pointers to signed 8 bits
    uint8_t i,j,s=0;
    int16_t avrg1, avrg2;
    Key=0;
    clr_display();
    lcd_putsp(PSTR("DISCONNECT CH1,CH2"));
    tiny_printp(116,7,PSTR("GO"));
    show_display(); WaitDisplay();
    while(!Key);
    if(testbit(Key,KC)) {
        clr_display();
	    for(Srate=0; Srate<8; Srate++) {	// Cycle thru first 8 srates
            i=6; do {
                s++;
                M.CH1gain=i;
                M.CH2gain=i;
                SimpleADC();
                q1=Temp.IN.CH1;
                q2=Temp.IN.CH2;
                // Calculate offset for CH1
                avrg1=0;
                avrg2=0;
                j=0; do {
            	    avrg1+= (*q1++);
            	    avrg2+= (*q2++);
                } while(++j);
                avrg8=avrg1>>8;
                eeprom_write_byte((uint8_t *)&offset8CH1[Srate][i], avrg8);
                j = 32+avrg8; // add 32 to center on screen
                if(j<64) lcd_line(s,32,s,j);
                if(Srate) {
                    avrg8=avrg2>>8;
                    eeprom_write_byte((uint8_t *)&offset8CH2[Srate-1][i], avrg8);
                    j = 32+avrg8; // add 32 to center on screen
                    if(j<64) lcd_line(s+64,32,s+64,j);
                }
                show_display(); WaitDisplay();
            } while(i--);
        }
        // Calculate offset for Meter in VDC
        avrg1=0;
        avrg2=0;
        ADCA.CTRLB = 0x10;          // signed mode, no free run, 12 bit right adjusted
        ADCA.PRESCALER = 0x07;      // Prescaler 512 (500kHZ ADC clock)
        i=0;
        do {
            ADCA.CH0.CTRL     = 0x83;   // Start conversion, Differential input with gain
            ADCA.CH1.CTRL     = 0x83;   // Start conversion, Differential input with gain
            _delay_us(400);
            avrg1+= (int16_t)ADCA.CH0.RES;  // Measuring 0V, should not overflow 16 bits
            avrg2+= (int16_t)ADCA.CH1.RES;  // Measuring 0V, should not overflow 16 bits
        } while(++i);
        eeprom_write_word((uint16_t *)&offset16CH1, avrg1/*+0x08*/);
        eeprom_write_word((uint16_t *)&offset16CH2, avrg2/*+0x08*/);
    }
    Key=0;
}

// Fill up channel data buffers
void SimpleADC(void) {
	Apply();
	_delay_ms(64);
    StartDMAs();
	_delay_ms(16);
    ADCA.CTRLB = 0x14;          // Stop free run of ADC (signed mode, no free run, 8 bit)
    // Disable DMAs
    clrbit(DMA.CH0.CTRLA, 7);
    clrbit(DMA.CH2.CTRLA, 7);
    clrbit(DMA.CH1.CTRLA, 7);
}

// Receives a nibble, returns the corresponding ascii that represents the HEX value
char NibbleToChar(uint8_t nibble) {
    nibble=nibble+'0';          // '0' thru '9'
    if(nibble>'9') nibble+=7;   // 'A' thru 'F'
    return nibble;
}

// Prints a HEX number
void printhex(uint8_t n) {
    uint8_t temp;
    temp = n>>4;   GLCD_Putchar(NibbleToChar(temp));
    temp = n&0x0F; GLCD_Putchar(NibbleToChar(temp));
}

// Converts an uint to int, then returns the half
uint8_t half(uint8_t number) {
    int8_t temp;
    temp=(int8_t)(number-128);
    temp=temp/2;
    return (uint8_t)(temp+128);
}

// Converts an uint to int, then return the double, or saturates
uint8_t twice(uint8_t number) {
    int8_t temp;
    if(number<=64) return 0;
    if(number>=192) return 255;
    temp = (int8_t)(number-128);
    temp = temp*2;
    return (uint8_t)(temp+128);
}

/*
// Calibrate DAC gain and offset, connect AWG to CH1
// Adjust with rotary encoders
static void CalibrateDAC(void) {
    uint8_t i, step=0, data, average;
    uint8_t test, bestoffset, bestgain, bestmeasure1;
    uint16_t sum, bestmeasure2;
    clr_display();

    ADCA.CH0.CTRL = 0x03 | (6<<2);       // Set gain 6
    CH1.offset=(signed char)eeprom_read_byte(&offsetsCH1[6]);

    AWGAmp=127;         // Amplitude range: [0,127]
    AWGtype=1;          // Waveform type
    AWGduty=256;        // Duty cycle range: [0,512]
    AWGOffset=0;        // 0V offset
    desiredF = 100000;  // 1kHz
    BuildWave();
    while(step<7) {
        while(!testbit(TCD0.INTFLAGS, TC1_OVFIF_bp));   // wait for refresh timeout
        setbit(TCD0.INTFLAGS, TC1_OVFIF_bp);
        // Acquire data

        // Display waveform
        i=0; sum=0;
        do {
            data=addwsat(CH1.data[i],CH1.offset);
            sum+=data;
            set_pixel(i>>1, data>>2);    // CH1
        } while(++i);
        average=(uint8_t)(sum>>8);

        switch(step) {
            case 0: // Connect AWG to CH1
                tiny_printp(0,0,PSTR("AWG Calibration Connect AWG CH1 Press 5 to start"));
                step++;
            break;
            case 1:
                if(key) {
                    if(key==KC) step++;
                    else step=7;         // Did not press 5 -> exit
                }
            break;
            case 2: // Output 0V from AWG
                AWGAmp=1;         // Amplitude range: [0,127]
                AWGtype=1;        // Waveform type
                BuildWave();
                tiny_printp(0,3,PSTR("Adjusting offset"));
                // ADS931 power, output enable, CH gains
//                PORTE.OUT = 0;
                CH1.offset=(signed char)eeprom_read_byte(&offsetsCH1[0]);
                step++;
                bestoffset = 0;
                test = 0;
                bestmeasure1=0;
                DACB.OFFSETCAL = 0;
            break;
            case 3: // Adjust Offset
                if(abs((int16_t)average-128)<abs((int16_t)bestmeasure1-128)) {    // Current value is better
                    bestoffset = test;
                    bestmeasure1=average;
                    lcd_goto(0,4);
                    if(bestoffset>=0x40) printN(0x40-bestoffset);
                    else printN(bestoffset);
                }
                lcd_line(0,bestmeasure1>>1,127,bestmeasure1>>1);
                test++;
                DACB.OFFSETCAL = test;
                if(test>=128) {
                    step++;
                    DACB.OFFSETCAL = bestoffset;   // Load DACA offset calibration
                }
            break;
            case 4: // Output -1.75V from AWG
                AWGAmp=0;           // Full Amplitude
                AWGtype=1;          // Waveform type
                AWGOffset=112;      // Offset = -1.75
                BuildWave();
                tiny_printp(0,5,PSTR("Adjusting gain"));
//                PORTE.OUT = 4;  // 0.5V / div
                CH1.offset=(signed char)eeprom_read_byte(&offsetsCH1[4]);
                step++;
                bestgain = 0;
                test=0;
                bestmeasure2=0;
                DACB.GAINCAL = 0;
            break;
            case 5: // Adjust gain
                // (1.75/0.5)*32+128)*256 = 61440
                if(abs((int32_t)sum-61696)<abs((int32_t)bestmeasure2-61696)) {    // Current value is better
                    bestgain = test;
                    bestmeasure2=sum;
                    lcd_goto(0,6);
                    if(bestgain>=0x40) printN(0x40-bestgain);
                    else printN(bestgain);
                }
                test++;
                DACB.GAINCAL = test;
                if(test>=128) {
                    step++;
                    DACB.GAINCAL = bestgain;
                }
            break;
            case 6: // Calibration complete
                // Save calibration results
                AWGAmp=0;
                eeprom_write_byte(&EEDACoffset, bestoffset);    // Save offset calibration
                eeprom_write_byte(&EEDACgain, bestgain);        // Save gain calibration
                tiny_printp(0,15,PSTR("Cal complete"));
                step++;
            break;
        }
    }
    // Restore Waveform
    LoadAWGvars();              // Load AWG settings
    BuildWave();                // Construct AWG waveform
}*/

extern const NVMVAR FLM;
static inline void Restore(void) {
    memcpy_P(0,  &FLGPIO, 12);
    memcpy_P(&M, &FLM, sizeof(NVMVAR));
    ONGRN();
    SaveEE();
    OFFGRN();
    Key=0;
}

extern NVMVAR EEMEM EEM;

// Save settings to EEPROM
void SaveEE(void) {
    if(!testbit(Misc,lowbatt)) {
	    if(MFFT<0x20) { // Don't save Meter mode settings
            Srate=old_s;
		    M.CH1gain=old_g1;
		    M.CH2gain=old_g2;
	    }
        eeprom_write_block(0, &EEGPIO, 12);
        eeprom_write_block(&M, &EEM, sizeof(NVMVAR));
        old_s=Srate; old_g1=M.CH1gain; old_g2=M.CH2gain;
    }
}

// Load settings from EEPROM
static inline void LoadEE(void) {
    eeprom_read_block(0, &EEGPIO, 12);
    eeprom_read_block(&M, &EEM, sizeof(NVMVAR));
}

// RTC clock, this function is called when the sleep timeout has been reached
ISR(RTC_OVF_vect) {
    PowerDown();
}

void PowerDown(void) {
	USB.CTRLB = 0;          // USB Disattach
	USB.ADDR = 0;
	USB.CTRLA = 0;
    OFFGRN();               // In case the LED was on
    SaveEE();               // Save MSO settings
    GLCD_LcdOff();
    while(Key);
    PORTE.OUTCLR = 0x01;    // Power up clear
    SLEEP.CTRL = SLEEP_SMODE_PDOWN_gc | SLEEP_SEN_bm;
    asm("sleep");
    SLEEP.CTRL = 0x00;
    GLCD_LcdInit();
    PORTE.OUTSET = 0x01;    // Power up
	Key=0;
	USB_ResetInterface();
}

// Wake up from sleep
ISR(TCD2_HUNF_vect) {
    SLEEP.CTRL = 0x00;
}
