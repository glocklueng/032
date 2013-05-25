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

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stddef.h>
#include <avr/fuse.h>
#include "main.h"

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
uint8_t prandom(void);
void Fade(void);
void clearlines(void);

//uint16_t readVCC(void);

// EEProm variables
uint8_t EEMEM EESleepTime = 64;     // Sleep timeout in minutes
uint8_t EEMEM EEDACgain   = 0;      // DAC gain calibration
uint8_t EEMEM EEDACoffset = 0;      // DAC offset calibration

int main(void) {
    // Power reduction: Stop unused peripherals
    PR.PRGEN = 0x18;        // Stop: AES, EBI
    PR.PRPA  = 0x04;        // Stop: DAC
    PR.PRPB  = 0x03;        // Stop: ADC, AC
    PR.PRPC  = 0x7C;        // Stop: TWI, USART0, USART1, SPI, HIRES
    PR.PRPD  = 0x6C;        // Stop: TWI,       , USART1, SPI, HIRES
    PR.PRPE  = 0x6C;        // Stop: TWI,       , USART1, SPI, HIRES

    // PORTS CONFIGURATION
    // Initial value PORTA.DIR       = 0x00; // CH2, CH1, 1V, K1, K2, K3, K4, REF
    PORTA.PIN1CTRL  = 0x18; // Pull up on pin PA1
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
    PORTD.OUT       = 0x04; // LCD voltage off
    PORTE.DIR       = 0x09; // TX, RX, RTS (input), CTS (power external board)
    PORTE.PIN1CTRL  = 0x18; // Pull up on pin PE1 (RTS)
	PORTE.OUT       = 0x01; // Power to external board
    PORTCFG.VPCTRLA = 0x41; // VP1 Map to PORTE, VP0 Map to PORTB
    PORTCFG.VPCTRLB = 0x32; // VP3 Map to PORTD, VP2 Map to PORTC

    // Clock Settings
	// USB Clock
	OSC.DFLLCTRL = OSC_RC32MCREF_USBSOF_gc; // Configure DFLL for 48MHz, calibrated by USB SOF
	DFLLRC32M.CALB = SP_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, USBRCOSC));
	DFLLRC32M.COMP1 = 0x1B; // Xmega AU manual, p41
	DFLLRC32M.COMP2 = 0xB7;
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;

    uint8_t i=1;
    OSC.XOSCCTRL = 0xCB;    // Crystal type 0.4-16 MHz XTAL - 16K CLK Start Up
    OSC.PLLCTRL = 0xC2;     // XOSC is PLL Source - 2x Factor (32MHz)
    OSC.CTRL = OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_XOSCEN_bm;
    while(i && !testbit(OSC.STATUS,OSC_XOSCRDY_bp)) {   // wait until crystal stable
        i++;
        _delay_us(100);
    }
    // Switch to internal 2MHz if crystal fails
    if(i==0) {   // Timed out, use internal oscillator
        OSC.XOSCCTRL = 0x00;    // Disable external oscillators
        OSC.PLLCTRL = 0x10;     // 2MHz is PLL Source - 16x Factor (32MHz)
    }
    OSC.CTRL = OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_PLLEN_bm | OSC_XOSCEN_bm;
    while(i && !testbit(OSC.STATUS,OSC_PLLRDY_bp)) {            // wait until PLL stable
        i++;
        _delay_us(100);
    }
    CCPWrite(&CLK.CTRL, CLK_SCLKSEL_PLL_gc);    // Switch to PLL clock
	// Clock OK!
	if(i) OSC.CTRL = OSC_RC32MEN_bm | OSC_PLLEN_bm | OSC_XOSCEN_bm;    // Disable internal 2MHz

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
    EVSYS.CH3MUX    = 0xD0;     // Event CH3 = TCD0 overflow used for DAC
    EVSYS.CH4MUX    = 0xC8;     // Event CH4 = TCC0 overflow used for freq. measuring
    EVSYS.CH5MUX    = 0xC0;     // Event CH5 = TCC1 overflow used for freq. measuring

    // Timer TCD0: 1MHz timer for DAC
    TCD0.PER          = 31;     // 1MHz (Value set at AWG.c, but needed for faster boot)
    DMA.CTRL          = 0x80;           // Enable DMA, single buffer, round robin

    // Interrupt Configuration
    PMIC.CTRL = 0x07;       // Enable High, Medium and Low level interrupts
    sei();      // Enable global interrupts

    // Initialize LCD
    GLCD_LcdInit();
	GLCD_setting();
    memcpy_P(display_buffer+286,  &GABOTRONICS, 69);   // Gabotronics
    tiny_printp(29,5,DEVICE); lcd_putsp(VERSION);

    show_display();
	_delay_ms(3000);
    

    for(;;) {   // Show images
        switch(prandom()>>4) {
            case 0:
                GLCD_DisplayPicture(tian1);
                show_display();
                 WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 1:
                GLCD_DisplayPicture(tian2);
                show_display();
                 WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 2:
                GLCD_DisplayPicture(tian3);
                show_display();
                 WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 3:
                GLCD_DisplayPicture(tian4);
                show_display();
                 WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 4:
                GLCD_DisplayPicture(tian5);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 5:
                GLCD_DisplayPicture(tian6);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 6:
                GLCD_DisplayPicture(tian7);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 7:
                GLCD_DisplayPicture(tian8);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 8:
                GLCD_DisplayPicture(tian9);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 9:
                GLCD_DisplayPicture(tian10);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 10:
                GLCD_DisplayPicture(tian11);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 11:
                GLCD_DisplayPicture(tian12);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 12:
                GLCD_DisplayPicture(tian13);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 13:
                GLCD_DisplayPicture(tian14);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 14:
                GLCD_DisplayPicture(tian15);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
            case 15:
                GLCD_DisplayPicture(tian16);
                show_display();
                WaitDisplay();
                _delay_ms(2000);
                Fade();
            break;
        }
    }    
    return 0;
}

void Fade(void) {
    for(uint16_t n=0; n<2048; n++) {
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        clr_pixel(prandom()>>1, prandom()>>2);
        show_display();
        WaitDisplay();
    }    
}

void clearlines(void) {
    for(uint8_t i=0; i<128; i++) {
        lcd_line(i,0,i,63,0);
        _delay_ms(2);
        show_display();
        WaitDisplay();        
    }
}

// Pseudo Random Number - Linear congruential generator
uint8_t prandom(void) {
    static uint16_t Seed = 0;
    Seed = 25173 * Seed + 1;
    return hibyte(Seed);
}


// Waits for the DMA to complete (the DMA's ISR will SET LCD_CS)
void WaitDisplay(void) {
    uint8_t n=0;
    while(!testbit(LCD_CTRL,LCD_CS)) {   // Wait for transfer complete
        _delay_us(8);
        n++;
        if(n==0) break;     // timeout ~ 2.048mS
    }
}

// Limits the display's refresh rate
void WaitRefresh(void) {
    while(!testbit(TCD1.INTFLAGS, TC1_OVFIF_bp)) {   // wait for refresh timeout
        if(testbit(Key, userinput)) break;       // cancel if user input
    }
    setbit(TCD1.INTFLAGS, TC1_OVFIF_bp);
}

// Tactile Switches - This is configured as a low level interrupt
ISR(PORTA_INT0_vect) {
    uint8_t i,in,j=0;
    // Debounce: need to read 10 consecutive equal numbers
    OFFGRN();   // Avoid having the LED on during the interrupt
    for(i=10; i>0; i--) {
		_delay_ms(1);
		in = PORTA.IN & 0x1E;       // Read port
		if(j!=in) { j=in; i=10; }    // Value changed
	}
    Key=0;
    if(!testbit(in,1)) setbit(Key, KD); // Menu key
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
    if(Key) {
        setbit(MStatus, update);         // Valid key
        setbit(Key, userinput);
    }
    else clrbit(Misc,keyrep);
    RTC.CNT=0;  // Clear screen saver timer
    // TCC1 used for auto repeat key
    TCC1.CNT = 0;            // Clear TCC1
    setbit(TCC1.INTFLAGS, TC1_OVFIF_bp);    // Clear timeout interrupt
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

// RTC clock, this function is called when the sleep timeout has been reached
ISR(RTC_OVF_vect) {
    PowerDown();
}

void PowerDown(void) {
	USB.CTRLB = 0;          // USB Disattach
    GLCD_LcdOff();
    OFFGRN();               // In case the LED was on
    PORTE.OUTCLR = 0x01;    // Power up clear
	USB.ADDR = 0;
	USB.CTRLA = 0;
    SLEEP.CTRL = SLEEP_SMODE_PDOWN_gc | SLEEP_SEN_bm;
    asm("sleep");
    SLEEP.CTRL = 0x00;
    GLCD_LcdInit();
    PORTE.OUTSET = 0x01;    // Power up
	Key=0;
}

// Wake up from sleep
ISR(TCD1_OVF_vect) {
    SLEEP.CTRL = 0x00;
}