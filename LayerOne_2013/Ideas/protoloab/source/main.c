/*****************************************************************************

Xmegalab / XMultiKit:
XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
November 2010

Copyright 2010 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

ATXMEGA128A1
Fuse settings:
 BOOTRST: Application Reset
 EESAVE
 JTAG: Checked if using JTAG programmer
       Unchecked if using PDI, so the logic analizer can be used

Compiled with GCC, -Os optimizations

*****************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include "main.h"
#include "uart.h"
#include "xitoa.h"
#include "ff.h"
#include "diskio.h"
#include "rtc.h"
#include "mso.h"
#include "awg.h"

FUSES = {
    .byte[5] = FUSE_EESAVE,
};

// 23K256 Serial RAM commands
#define READ  0x03  // Read data from memory array beginning at selected address
#define WRITE 0x02  // Write data to memory array beginning at selected address
#define RDSR  0x05  // Read STATUS register
#define WRSR  0x01  // Write STATUS register
// SPI Controls  (Platform dependent)
#define SPISELECT()     PORTF.OUTCLR = 0x20   // SRAM CS = L 
#define	SPIDESELECT()	PORTF.OUTSET = 0x20   // SRAM CS = H

uint8_t AWGBuffer[256];  // AWG Output Buffer

// Time variables
uint8_t minutes=0;
uint8_t hours=0;

// EEProm variables
uint8_t EEMEM EEapp       = 0;      // Main menu  
uint8_t EEMEM EEDACgain   = 0;      // DAC gain calibration
uint8_t EEMEM EEDACoffset = 0;      // DAC offset calibration

// FAT File System
FATFS fatfs;				// File system object
FILINFO finfo;              // File info object

uint8_t oldkey;             // Previous key pressed
uint8_t Letter[17] PROGMEM = { '1', '2', '3', 'B',      // Key matrix
                               '4', '5', '6', 'C',
                               '7', '8', '9', 'D',
                               '*', '0', '#', 'E', 'A' };

static void Calibrate(void);
static void CalibrateDAC(void);
static void Factory(void);
static void Lissajous(uint8_t f1, uint8_t f2, uint8_t ph);
uint8_t SP_ReadCalibrationByte(uint8_t location);
uint16_t readVCC(void);

int main(void) {
    uint8_t i=0,x,y,data,f1,f2,ph=0;
    uint16_t a;
    char txt[6]= "     ";

    // Power reduction: Stop unused peripherals
    PR.PRGEN = 0x18;        // Stop: AES, EBI
    PR.PRPC  = 0x74;        // Stop: TWI, USART0, USART1, HIRES
    PR.PRPD  = 0x7C;        // Stop: TWI, USART0, USART1, SPI, HIRES
    PR.PRPE  = 0x60;        // Stop: TWI, USART1, (XMEGA bug - HIRES cannot be turned off)
    PR.PRPF  = 0x78;        // Stop: TWI, USART0, USART1, SPI, (XMEGA bug - HIRES cannot be turned off)

    // PORTS CONFIGURATION
    PORTA.DIR      = 0x30;  // Encoder, LEDS, Volt Monitors, AWG, 2.0V
    PORTA.PIN6CTRL = 0x18;  // Pull up on pin PA6, sense both edges
    PORTA.PIN7CTRL = 0x18;  // Pull up on pin PA7, sense both edges
    PORTA.INTCTRL  = 0x04;  // PORTA will generate low level interrupts
    PORTA.INT1MASK = 0xC0;  // PA6 PA7 will be the interrupt 1 sources
    PORTA.OUT      = 0x00;  
    PORTB.DIR      = 0x00;  // JTAG, CHA, CHB, Ext Trig, 2.0V
    PORTB.OUT      = 0x00;
    PORTC.DIR      = 0xBC;  // SPI, Buzzer, Encoder
    PORTC.PIN0CTRL = 0x18;  // Pull up on pin PC0, sense both edges
    PORTC.PIN1CTRL = 0x18;  // Pull up on pin PC1, sense both edges
    PORTC.INTCTRL  = 0x04;  // PORTC will generate low level interrupts
    PORTC.INT1MASK = 0x03;  // PC0 PC1 will be the interrupt 1 sources
    PORTC.OUT      = 0x14;  // C-SS high, pulse buzzer
    PORTD.DIR      = 0xF0;  // Keypad and switches
    PORTCFG.MPCMASK = 0xFF; // Configure all pins on PORTD the same way
    PORTD.PIN0CTRL = 0x18;  // Pull up on pin PortD (Keypad and K2-K5 buttons)
    PORTD.INTCTRL  = 0x01;  // PORTD will generate low level interrupts
    PORTD.INT0MASK = 0x0F;  // PD0-PD3 will be the interrupt sources
    PORTD.OUT      = 0x00;
    PORTE.DIR      = 0xB9;  // SPI, TX, RX, SD_CD, TEST
    PORTE.PIN3CTRL = 0x40;  // Invert TX output
    PORTE.OUT      = 0x10;  // Deselect SD card
    PORTE.PIN1CTRL = 0x02;  // PE1 will sense falling edges
    PORTE.INTCTRL  = 0x01;  // PORTE will generate low level interrupts
    PORTE.INT0MASK = 0x02;  // PE1 will be the interrupt source
    PORTF.DIR      = 0xFB;  // LCD WR, LCD RD, SRAM CS, ADCCLK, CHSEL, SD_WP, LCD A0, LCD BKLT
    PORTF.PIN3CTRL = 0x80;  // Slew rate control
    PORTF.PIN4CTRL = 0x80;  // Slew rate control
    PORTF.OUT      = 0x21;  // Deselect Serial RAM, backlight on
    PORTH.DIR      = 0xFF;  // PDN, \OE, SELA, SELB
    PORTH.OUT      = 0x80;  // Power Down ADS931
    PORTJ.DIR      = 0x00;  // ADS931 data
    PORTK.DIR      = 0xFF;  // LCD data
    PORTK.OUT      = 0x00;
    PORTQ.DIR      = 0x04;  // On switch, voltage regulator control
    PORTQ.OUT      = 0x04;  // Power ON voltage regulators
    PORTQ.PIN3CTRL = 0x01;  // PQ3 will sense rising edges
    PORTQ.INT0MASK = 0x08;  // PQ3 will be the interrupt 0 source
    PORTQ.INTCTRL  = 0x01;  // PORTQ will generate low level interrupts
    PORTCFG.VPCTRLA = 0x18; // VP1 Map to PORTB, VP0 Map to PORTJ
    PORTCFG.VPCTRLB = 0x59; // VP3 Map to PORTF, VP2 Map to PORTK

    // Clock Settings
    OSC.XOSCCTRL = 0xCB;    // 0.4-16 MHz XTAL - 16K CLK Start Up
    OSC.CTRL = 0x08;        // Enable External Oscillator
    while(!testbit(OSC.STATUS,OSC_XOSCRDY_bp));  // wait until crystal stable
    OSC.PLLCTRL = 0xC2;     // XOSC is PLL Source - 2x Factor (32MHz)
    OSC.CTRL = 0x19;        // Enable PLL & External Oscillator
    while(!testbit(OSC.STATUS,OSC_PLLRDY_bp));  // wait until PLL stable
    CCPWrite(&CLK.CTRL, CLK_SCLKSEL_PLL_gc);    // Switch to PLL clock
    OSC.CTRL = 0x18;        // Disable internal 2MHz
    _delay_ms(100);         // Debounce ON Switch

    // RTC Clock Settings
	/* Set 1kHz from 32kHz crystal as clock source for RTC. */
	CLK.RTCCTRL = CLK_RTCSRC_TOSC_gc | CLK_RTCEN_bm;
    RTC.PER = 59;           // 31.25mS period
    RTC.CNT = 0;
    RTC.INTCTRL = 0x01;     // Generate low level interrupt
    RTC.CTRL = 0x07;        // Divisor 1024 (1Hz)
    // Event System
/*    EVSYS.CH0MUX = 0xD0;    // Event CH0 = TCF0 compare  used for DMA CH0
    EVSYS.CH1MUX = 0xD0;    // Event CH1 = TCF0 overflow used for DMA CH1*/
    EVSYS.CH3MUX = 0xD0;    // Event CH3 = TCD0 overflow used for DAC

    // Timer TCD0: 1MHz timer for DACA
    TCD0.CTRLA = 0x01;      // Prescaler: clk/1
    TCD0.PER   = 31;        // 1MHz
    
    // Timer TCE0: 1KHz Test signal
    TCE0.CTRLA = 0x01;      // Prescaler: clk/1
    TCE0.CTRLB = 0x11;      // CCAEN override, Frequency mode
    TCE0.CCA   = 15999;     // fFRQ = fPER / (2N CCA+1) = 1KHz

    // Timer TCE1: 32Hz timer for SD Card routines and very slow sampling rates
    TCE1.CTRLA = 0x05;      // Prescaler: clk/64
    TCE1.PER   = 15624;     // 32Hz
    TCE1.INTCTRLA = 0x02;   // Timer overflow is a medium level interrupt

    // ADC
	ADCA.CALL = SP_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) );
	ADCA.CALH = SP_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) );
    //tempCal   = SP_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t,TEMPSENSE0));
    //tempCal  |= SP_ReadCalibrationByte(offsetof(NVM_PROD_SIGNATURES_t,TEMPSENSE1)) << 8;

    // DACs
    DACA.CTRLA = 0x15;          // Enable DACA and CH0, also route signal to AC
    DACA.CTRLB = 0x01;          // CH0 auto triggered by an event
    DACA.CTRLC = 0x11;          // Use AREFA (2.0V), data is left adjusted
    DACA.CH0DATAH = 0x80;       // Set CHA so AWG output is near 0V
    DACA.EVCTRL = 0x03;         // Event CH3 triggers the DAC Conversion
    DACA.TIMCTRL = 0x50;        // Minimum 32 CLK between conversion (1uS)
    DACA.GAINCAL = eeprom_read_byte(&EEDACgain);      // Load DACA gain calibration
    DACA.OFFSETCAL = eeprom_read_byte(&EEDACoffset);  // Load DACA offset calibration

    DACB.CTRLA = 0x11;          // Enable DACB for internal use and enable
    DACB.CTRLC = 0x11;          // Use AREFA (2.0V), data is left adjusted
    DACB.TIMCTRL = 0x50;        // Minimum 32 CLK between conversion (1uS)

    // DMAs
    DMA.CTRL = 0x80;            // Enable DMA, single buffer, round robin on CH2 and CH3
 /*   // DMA for CH1 analog input
    DMA.CH0.ADDRCTRL = 0x0D;    // Reload after transaction, Increment destination
    DMA.CH0.TRIGSRC= 0xA0;      // Trigger source is TCF0 overflow
    DMA.CH0.TRFCNT = 256;       // AWG Buffer is 256 bytes
	DMA.CH0.SRCADDR0  = (( (uint32_t) (&VPORT0.IN)) >> 0*8 ) & 0xFF;
	DMA.CH0.SRCADDR1  = (( (uint32_t) (&VPORT0.IN)) >> 1*8 ) & 0xFF;
	DMA.CH0.SRCADDR2  = (( (uint32_t) (&VPORT0.IN)) >> 2*8 ) & 0xFF;
	DMA.CH0.DESTADDR0 = (( (uint32_t) (CH1.data)) >> 0*8 ) & 0xFF;
	DMA.CH0.DESTADDR1 = (( (uint32_t) (CH1.data)) >> 1*8 ) & 0xFF;
	DMA.CH0.DESTADDR2 = (( (uint32_t) (CH1.data)) >> 2*8 ) & 0xFF;
    DMA.CH0.CTRLA = 0xA4;       // Enable CH0, repeat mode, 1 byte burst, single
    // DMA for CH2 analog input
    DMA.CH1.ADDRCTRL = 0x0D;    // Reload after transaction, Increment destination
    DMA.CH1.TRIGSRC= 0xA5;      // Trigger source is TCF0 CCD compare
    DMA.CH1.TRFCNT = 256;       // AWG Buffer is 256 bytes
	DMA.CH1.SRCADDR0  = (( (uint32_t) (&VPORT0.IN)) >> 0*8 ) & 0xFF;
	DMA.CH1.SRCADDR1  = (( (uint32_t) (&VPORT0.IN)) >> 1*8 ) & 0xFF;
	DMA.CH1.SRCADDR2  = (( (uint32_t) (&VPORT0.IN)) >> 2*8 ) & 0xFF;
	DMA.CH1.DESTADDR0 = (( (uint32_t) (CH2.data)) >> 0*8 ) & 0xFF;
	DMA.CH1.DESTADDR1 = (( (uint32_t) (CH2.data)) >> 1*8 ) & 0xFF;
	DMA.CH1.DESTADDR2 = (( (uint32_t) (CH2.data)) >> 2*8 ) & 0xFF;
    DMA.CH1.CTRLA = 0xA4;       // Enable CH1, repeat mode, 1 byte burst, single
*/
    // DMA for DACA
    DMA.CH3.ADDRCTRL = 0xD0;    // Reload after transaction, Increment source
    DMA.CH3.TRIGSRC= 0x15;      // Trigger source is DACA CH0
    DMA.CH3.TRFCNT = 256;       // AWG Buffer is 256 bytes
	DMA.CH3.SRCADDR0  = (( (uint32_t) (&AWGBuffer)) >> 0*8 ) & 0xFF;
	DMA.CH3.SRCADDR1  = (( (uint32_t) (&AWGBuffer)) >> 1*8 ) & 0xFF;
	DMA.CH3.SRCADDR2  = (( (uint32_t) (&AWGBuffer)) >> 2*8 ) & 0xFF;
	DMA.CH3.DESTADDR0 = (( (uint32_t) (&DACA.CH0DATAH)) >> 0*8 ) & 0xFF;
	DMA.CH3.DESTADDR1 = (( (uint32_t) (&DACA.CH0DATAH)) >> 1*8 ) & 0xFF;
	DMA.CH3.DESTADDR2 = (( (uint32_t) (&DACA.CH0DATAH)) >> 2*8 ) & 0xFF;
    DMA.CH3.CTRLA = 0xA4;       // Enable CH3, repeat mode, 1 byte burst, single

    // Arbitrary Waveform Generator
    LoadAWGvars();              // Load AWG settings
    BuildWave();                // Construct AWG waveform

    // Initialize USART
    USARTE0.BAUDCTRLA = 0x17;   // BSCALE = -6, BSEL = 1047
    USARTE0.BAUDCTRLB = 0xA4;   // ==> 115211 bps (~115.2kbps)
    USARTE0.CTRLA = 0x10;       // RX is low level interrupt
    USARTE0.CTRLC = 0x03;       // Async, No Parity, 1 stop bit, 8 data bits
    USARTE0.CTRLB = 0x18;       // Enable RX and TX
    uart_init();                // Initialize Send and Receive buffers

    // Initialize Serial Ram
    SPISELECT();                    // Select
    SPIE.CTRL = 0xD0;               // Enable Master Mode, clkper/2
    SPIE.DATA = WRSR;               // Write 23K256's STATUS register command
    while(!testbit(SPIE.STATUS,7)); // Wait until transfer complete
    SPIE.DATA = 0x40;               // Set sequential mode
    while(!testbit(SPIE.STATUS,7)); // Wait until transfer complete
    data = SPIE.DATA;               // Dummy read to clear SPIF flag
    SPIDESELECT();                  // Deselect
    WriteSerialRAM(0,&ph,1);        // Initialize first 2 bytes with 0
    WriteSerialRAM(1,&ph,1);

    // Initialize LCD
    lcd_init(GRAPHTEXT);
    lcd_putsp(PSTR("  Gabotronics   "));
    lcd_putsp(PSTR(" Xmegalab V1.08 "));
    PORTC.OUTTGL = _BV(BUZZER1); // Toggle pin
    // seed will change depending on the time the key is pressed
    while(testbit(PORTQ.IN,ONBUTTON)) prandom();

    // Interrupt Configuration
    PORTQ.INTFLAGS = 0x3;   // Clear any interrupts (bouncing on ON Switch)
    PMIC.CTRL = 0x07;       // Enable High, Medium and Low level interrupts
    sei();      // Enable global interrupts

    // Rotary encoders ranges
    MAX1 = 255; MAX2 = 255;
    ROT1 = prandom(); ROT2 = prandom();

   // Timer TCF0 - OC0D: Channel Select Clock (used on slow sampling rates)
    TCF0.CTRLB = 0x83;      // CCDEN override, Single Slope PWM
 
    // TCC1 controls LCD refresh rate
    TCC1.CTRLA = 6;         // Prescaler: clk/256
    TCC1.CTRLB = 0;         // Normal Mode
    TCC1.CTRLD = 0;         // No Event
    TCC1.PER = 15624;       // Maximum LCD refresh rate is 8Hz

    if(!testbit(PORTE.IN,SDCD)) {    // SD card present
        // Initialize SD Card, do 4 attempts
        for(i=0; i<4; ) {
            data=disk_initialize();
            if(data&STA_NOINIT) i++;
            else break;
            _delay_ms(50);
        }
        // Initialize FAT file system
        if(!(data&STA_NOINIT)) f_mount(0, &fatfs);
    }

    // Load last application
    key = eeprom_read_byte(&EEapp);
    if(!key) setbit(Signals, redraw);   // Go to main menu

    for(;;) {
        while(!testbit(TCC1.INTFLAGS, TC1_OVFIF_bp)) {   // wait for refresh timeout
            if(testbit(Signals, userinput))
                break;       // cancel if user input
        }
        setbit(TCC1.INTFLAGS, TC1_OVFIF_bp);
        if(testbit(Signals, userinput)) {
            clrbit(Signals, userinput);
            // Enable ADC and CH SEL clocks (used for diagnostic on the welcome screen)
            PORTH.OUT = ((ROT2&0x07)<<3) | (ROT1&0x07);
        }
        // TEST MODE
        if(testbit(Signals, testing)) {
            ph++;
            if(testbit(Signals, redraw)) {
                clrbit(Signals, redraw);
                lcd_goto(0,1); lcd_putsp(PSTR("CH1"));
                lcd_goto(13,1); lcd_putsp(PSTR("CH2"));
                lcd_goto(0,6); lcd_putsp(PSTR("CHD"));
                lcd_goto(13,6); lcd_putsp(PSTR("AWG"));
                lcd_goto(0,12); lcd_putsp(PSTR("VCC:"));
                lcd_goto(0,14); lcd_puts(txt);       // Print buffer on LCD
                lcd_goto(9,15); lcd_putsp(PSTR("FACTORY"));
                // Timer TCF1 - OC1A: ADC Clock
                TCF1.CTRLB = 0x11;      // CCAEN override, Frequency mode
                TCF0.CTRLB = 0x83;      // CCDEN override, Single Slope PWM
                TCF0.PER = 399;
                TCF0.CCD = 199;         // Select clock
                TCF1.CCA = 99;          // ADC clock
                TCF0.CTRLA  = 0x01;     // Enable Timer, Prescaler: clk/1
                TCF1.CTRLA  = 0x01;     // Enable Timer, Prescaler: clk/1
            }
            lcd_clear_graphics();

            if(ph&0x01) {
                ONGRN();                        // Blink on LED at a time
                PORTC.OUTTGL = _BV(BUZZER1);    // Test buzzer too
            }
            else {
                ONRED();
                PORTC.OUTTGL = _BV(BUZZER2);
            }
            cli();      // Disable global interrupts
            acquire5(CH1.data, CHD.data, CH2.data);
            sei();      // Enable global interrupts
            OFFGRN();
            OFFRED();
            i=0; do {
                x=i>>2;
                // Display waveforms
                set_pixel(x,   16+(CH1.data[i]>>3));    // CH1
                set_pixel(x+64,16+(CH2.data[i]>>3));    // CH2
                // CHD
                if(testbit(CHD.data[i],5)) set_pixel(x, 57); else { set_pixel(x, 61); }
                if(testbit(CHD.data[i],6)) set_pixel(x, 63); else { set_pixel(x, 67); }
                if(testbit(CHD.data[i],7)) set_pixel(x, 69); else { set_pixel(x, 73); }
                if(testbit(CHD.data[i],8)) set_pixel(x, 75); else { set_pixel(x, 79); }

                set_pixel(x+64,56+(AWGBuffer[i]>>3));  // AWG
            } while(++i);
            print_timer(8, 14);
            // VCC measurement
            lcd_goto(4,12); printN(readVCC());
            if(key) {
                data=pgm_read_byte(&Letter[key-1]);
                for(i=0; i<4; i++) txt[i]=txt[i+1];     // shift buffer
                txt[4] = data;
                i=key; key=0;
                switch(i) {
                    case KA: break;
                    case KB: break;
                    case KC: break;
                    case KD: break;
                    case KE: Factory(); break;     // Factory Options
                    case KS: clrbit(Signals, testing); break; // Exit  test mode
                    case KP: setbit(Signals, testing); break; // Enter test mode
                    default: uart_put(data);
                }
                setbit(Signals, redraw);
                oldkey=0; key=0;
            }
        }
        // NORMAL MODE
        else {
            if(testbit(Signals, redraw)) {
                clrbit(Signals, redraw);
                lcd_init(GRAPHTEXT);
                lcd_goto(3,0); lcd_putsp(PSTR(" Xmegalab"));
                lcd_goto(11,12); lcd_putsp(PSTR("XMEGA"));
                lcd_goto(12,13); lcd_putsp(PSTR("rev")); put_char('A'+MCU.REVID);
                bitmap(0,73,avrlogo);   // Display AVR logo
                tiny_printp(0,123,PSTR(" SD     AWG    MSO   CHESS  CALC"));
            }
            if(key) {
                data=pgm_read_byte(&Letter[key-1]);
                for(i=0; i<4; i++) txt[i]=txt[i+1];     // shift buffer
                txt[4] = data;
                i=key; key=0;
                switch(i) {
                    case KA: SD();      break;
                    case KB: AWG();     break;
                    case KC: finfo.fname[0] = 0;    // Clear filename
                             MSO();
                             PORTH.OUT = 0x80;  // Power Down ADS931
                             ACA.AC0CTRL = 0;   // Turn off comparator A
                             ACB.AC0CTRL = 0;   // Turn off comparator B
                    break;
                    case KD: CHESS();   break;
                    case KE: CALC();    break;
                    case KS: clrbit(Signals, testing); break; // Exit  test mode
                    case KP: setbit(Signals, testing); break; // Enter test mode
                    default: uart_put(data);
                }
                // TCC1 controls LCD refresh rate
                TCC1.CTRLA = 6;         // Prescaler: clk/256
                TCC1.CTRLB = 0;         // Normal Mode
                TCC1.CTRLD = 0;         // No Event
                TCC1.PER = 15624;       // Maximum LCD refresh rate is 8Hz
                setbit(Signals, redraw);
                oldkey=0; key=0;
            }
            else {
                // Draw lissajous curve
                a=0;
                do {                // Erase old lissajous curve
                    ReadSerialRAM(a++,&x,1);
                    ReadSerialRAM(a++,&y,1);
                    clr_pixel(x,y);
                } while((x!=0) && (y!=0));
                f1 = 1+ROT1;
                f2 = 1+ROT2;
                Lissajous(f1,f2,ph++); // Generate new curve and store in serial RAM
                a=0;
                for(;;) {                // Draw new curve
                    ReadSerialRAM(a++,&x,1);
                    ReadSerialRAM(a++,&y,1);
                    if ((x==0) && (y==0)) break;
                    set_pixel(x,y);
                }
            }
        }
        if(uart_test()) {   // Data in receive buffer?
            for(i=0; i<4; i++) txt[i]=txt[i+1];   // shift text buffer
            txt[4] = uart_get();                 // Receive from serial port
            uart_put(txt[4]);                   // Loop back
            setbit(Signals, redraw);
        }
    }
    return 0;
}

// Return sine fractional value
int8_t Sin(uint8_t angle) {
    if(angle>=128) return - pgm_read_byte(&sint[angle-128]);
    return pgm_read_byte(&sint[angle]);
}

// Return cosine fractional value
int8_t Cos(uint8_t angle) {
    return Sin(angle+64);
}

// Add unsigned with signed using saturation, return unsigned
uint8_t addwsat(uint8_t a, int8_t b) {
    if(b>=0) {
        if(a>255-b) return 255;
    }
    else {
        if(a<(-b)) return 0;
    }
    return a+b;
}

// x = sin(f1 + ph) , y = sin(f2)
// Store data in Serial RAM
static void Lissajous(uint8_t f1, uint8_t f2, uint8_t ph) {
    uint8_t ox=0,oy=0,x,y,x1,x2,y1,y2;
    uint16_t a,i=0,j=0;
    do {
        x1= 64+FMULS8(32,Sin(i+ph)); WriteSerialRAM(0,&x1,1);
        y1= 40+FMULS8(32,Sin(j)); WriteSerialRAM(1,&y1,1);
        i+=f1; j+=f2;
        x2= 64+FMULS8(32,Sin(i+ph)); WriteSerialRAM(2,&x2,1);
        y2= 40+FMULS8(32,Sin(j)); WriteSerialRAM(3,&y2,1);
        i+=f1; j+=f2;
    } while (x1==x2 && y1==y2); // Loop until we get different points
    for(a=4; ; i+=f1,j+=f2) {
        x = 64+FMULS8(32,Sin(i+ph));
        y = 40+FMULS8(32,Sin(j));
        WriteSerialRAM(a++,&x,1);
        WriteSerialRAM(a++,&y,1);
        // Stop when sequence starts to repeat
        if(ox==x1 && oy==y1 && x==x2 && y==y2) break;
        ox=x; oy=y;
    }
    // Mark end with wo consecutive zeroes
    x=0;
    WriteSerialRAM(a++,&x,1);
    WriteSerialRAM(a++,&x,1);
}

// Pseudo Random Number - Linear congruential generator
uint8_t prandom(void) {
    static uint16_t Seed = 0;
    Seed = 25173 * Seed + 1;
    return hibyte(Seed);
}

void sound(uint8_t tone) {
    uint8_t i,j;
    PORTC.OUTCLR = _BV(BUZZER1);
    PORTC.OUTSET = _BV(BUZZER2);
    for(j=80; j; j--) {
        for(i=tone; i; i--) {
            asm("lpm"); asm("lpm");
            asm("lpm"); asm("lpm");
            asm("lpm"); asm("lpm");
            asm("lpm"); asm("lpm");
        }
        PORTC.OUTTGL = _BV(BUZZER1)+_BV(BUZZER2);
	}
    PORTC.OUTCLR = _BV(BUZZER1)+_BV(BUZZER2);
}

// POWER button - This is configured as a low level interrupt
ISR(PORTQ_INT0_vect) {
    uint8_t i=0,j;
    _delay_ms(10);
    oldkey = key;
    if(testbit(PORTQ.IN, ONBUTTON)) {
        PORTC.OUTTGL = _BV(BUZZER1);    // Buzzer sound
        setbit(Signals, update);         // Valid key
        setbit(Signals, userinput);
        key = KA;
    }
    while(testbit(PORTQ.IN, ONBUTTON)) {
        _delay_ms(10);
        i++;
        if(i==200) {    // Key pressed for 2 seconds
            cli();  // permanently disable interrupts
            SaveEE();       // Save MSO settings
            SaveAWGvars();  // Save AWG settings
            PORTC.OUT = _BV(BUZZER1);
            // Shut down sound
            for(i=0; i<40; i++) {
		        for(j=i; j; j--) {
                    _delay_ms(1);
                }
		        PORTC.OUTTGL = _BV(BUZZER1); // Toggle pin
		        PORTC.OUTTGL = _BV(BUZZER2); // Toggle pin
	        }
            for(;;) PORTQ.OUT = 0;      // TURN OFF SYSTEM!!!
        }
    }
}

// SWITCH MATRIX ISR (Keypad and K2,K3,K4,K5 buttons)
ISR(PORTD_INT0_vect) {
    uint8_t In,j=0;
    uint8_t Row, Col=0, i;
    // Debounce
    for(i=10; i>0; i--) {
		_delay_ms(1);
		In = PORTD.IN;            	// Read PORTD
		if(j!=In) { j=In; i++; }
	}
	if	    (In==(0x07)) Row=3;
	else if	(In==(0x0B)) Row=2;
	else if	(In==(0x0D)) Row=1;
	else if	(In==(0x0E)) Row=0;
	else {							// No keys or more than 1 key pressed
		clrbit(Signals, keydown);
        oldkey = key;
        key = 0;
        PORTD.INTFLAGS = 0x01;      // Clear interrupt
		return;
	}
    // Rows processed, now Cols:
    PORTD.DIR = 0x0F;               // ROWS AS OUTPUTS
    // DeBounce
	for(i=5; i>0; i--) {
		_delay_ms(1);
		In = PORTD.IN;              // Read Keyboard Column
		if(j!=In) { j=In; i++; }
	}
    // Restore PORTD direction
    PORTD.DIR = 0xF0;               // ROWS AS INPUTS
	if	    (In==(0x70)) Col=3;
	else if	(In==(0xB0)) Col=2;
	else if	(In==(0xD0)) Col=1;
	else if	(In==(0xE0)) Col=0;
	else {							// No keys or more than 1 key pressed
		clrbit(Signals, keydown);
        oldkey=key;
        key=0;
        PORTD.INTFLAGS = 0x01;      // Clear interrupt
		return;
	}
    oldkey=key;
	key = Row*4+Col+1;
    Signals |= (_BV(update) + _BV(userinput) + _BV(keydown));
    PORTC.OUTTGL = _BV(BUZZER1);    // Toggle pin
    _delay_us(1);
    PORTD.INTFLAGS = 0x01;          // Clear interrupt
}

// Read from 23K256 chip
void ReadSerialRAM(uint16_t Addr, uint8_t *Data, uint16_t n) {
    uint16_t i;
    PORTE.INTCTRL  = 0x00;              // Disable SD insert interrupt
    SPISELECT();                        // Select
    SPIE.DATA = READ;                   // Set sequential mode
    while(!testbit(SPIE.STATUS,7));     // Wait until transfer complete
    SPIE.DATA = hibyte(Addr);           // Address high byte
    while(!testbit(SPIE.STATUS,7));     // Wait until transfer complete
    SPIE.DATA = lobyte(Addr);           // Address low byte
    while(!testbit(SPIE.STATUS,7));     // Wait until transfer complete
    for(i=0; i<n; i++) {
        SPIE.DATA = 0;                  // Dummy send to receive data
        while(!testbit(SPIE.STATUS,7)); // Wait until transfer complete
        Data[i] = SPIE.DATA;            // read data
    }
    i = SPIE.DATA;                      // Dummy read to clear SPIF flag
    SPIDESELECT();                      // Deselect
    PORTE.INTCTRL  = 0x01;              // Enable SD insert interrupt
}

// Write to 23K256 chip
void WriteSerialRAM(uint16_t Addr, uint8_t *Data, uint16_t n) {
    uint16_t i;
    PORTE.INTCTRL  = 0x00;              // Disable SD insert interrupt
    SPISELECT();                        // Select
    SPIE.DATA = WRITE;                  // Set sequential mode
    while(!testbit(SPIE.STATUS,7));     // Wait until transfer complete
    SPIE.DATA = hibyte(Addr);           // Address high byte
    while(!testbit(SPIE.STATUS,7));     // Wait until transfer complete
    SPIE.DATA = lobyte(Addr);           // Address low byte
    while(!testbit(SPIE.STATUS,7));     // Wait until transfer complete
    for(i=0; i<n; i++) {
        SPIE.DATA = Data[i];            // Send data
        while(!testbit(SPIE.STATUS,7)); // Wait until transfer complete
    }
    i = SPIE.DATA;                      // Dummy read to clear SPIF flag
    SPIDESELECT();                      // Deselect
    PORTE.INTCTRL  = 0x01;              // Enable SD insert interrupt
}

// Rotary Encoder on Port C
//                _______    
// OutA:  _______|       |_____
//            _______     
// OutB: ____|       |______
//                      
// Data:   0   1   3   2
//
ISR(PORTC_INT1_vect) {
    static uint8_t oldEnc2=0, Decision2=0;
    uint8_t Enc2, Moved=0;
    Enc2 = (uint8_t)(PORTC.IN & 0x03);
    // Check Encoder 2
    if((Enc2==0) && (Decision2==0)) {       // Encoder is 00
        if(oldEnc2==1) Moved=2;               // Increment
        if(oldEnc2==2) Moved=1;               // Decrement
        Decision2=1;
    }
    else if((Enc2==3) && (Decision2==1)) {  // Encoder is 11
        if(oldEnc2==2) Moved=2;               // Increment
        if(oldEnc2==1) Moved=1;               // Decrement
        Decision2=0;
    }
    oldEnc2 = Enc2;
    if(Moved==1) {
        if(ROT2<MAX2) { ROT2++; PORTC.OUTTGL = _BV(BUZZER1); }
        else { sound(100); ROT2=0; }        // Roll over
    }
    if(Moved==2) {
        if(ROT2>0) { ROT2--; PORTC.OUTTGL = _BV(BUZZER1); }
        else { sound(100); ROT2=MAX2; }     // Roll over
    }
    if(Moved) {
        setbit(Signals, update);
        setbit(Signals, userinput);
    }
}

// Rotary Encoder on Port A
ISR(PORTA_INT1_vect) {
    static uint8_t oldEnc1=0, Decision1=0;
    uint8_t Enc1, Moved=0;

    Enc1 = (uint8_t)((PORTA.IN & 0xC0) >> 6);

    // Check Encoder 1
    if((Enc1==0) && (Decision1==0)) {       // Encoder is 00
        if(oldEnc1==1) Moved=2;               // Increment
        if(oldEnc1==2) Moved=1;               // Decrement
        Decision1=1;
    }
    else if((Enc1==3) && (Decision1==1)) {  // Encoder is 11
        if(oldEnc1==2) Moved=2;               // Increment
        if(oldEnc1==1) Moved=1;               // Decrement
        Decision1=0;
    }
    oldEnc1 = Enc1;

    if(Moved==1) {
        if(ROT1<MAX1) { ROT1++; PORTC.OUTTGL = _BV(BUZZER1); }
        else { sound(100); ROT1=0; }        // Roll over
    }
    if(Moved==2) {
        if(ROT1>0) { ROT1--; PORTC.OUTTGL = _BV(BUZZER1); }
        else { sound(100); ROT1=MAX1; }     // Roll over
    }
    if(Moved) {
        setbit(Signals, update);
        setbit(Signals, userinput);
    }
}

// TCD1 Overflow Interrupt
// Used in Chess application: 244Hz = 4.096ms
// Used in MSO application: variable - sets trigger timeout
ISR(TCD1_OVF_vect) {
    register unsigned char ndx;
    ++timer.w;
    ndx = timer.c[0] & 0x03;
    // Trigger for AUTO mode:
    if(!testbit(MStatus, stop)) {
        setbit(Signals, update);
        setbit(MStatus, trigger);
    }
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

/*! \brief Function for GCC to read out calibration byte.
 *
 *  \note For IAR support, include the adc_driver_asm.S90 file in your project.
 *
 *  \param index The index to the calibration byte.
 *
 *  \return Calibration byte.
 */
uint8_t SP_ReadCalibrationByte( uint8_t location )
{
	uint8_t result;

	/* Load the NVM Command register to read the calibration row. */
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
 	result = pgm_read_byte(location);

	/* Clean up NVM Command register. */
 	NVM_CMD = NVM_CMD_NO_OPERATION_gc;

	return result;
}

// Calibrate offset, inputs must be connected to ground
static void Calibrate(void) {
    uint8_t i,j;
    uint16_t average;
    lcd_clear_text();
    lcd_goto(0,0); lcd_putsp(PSTR(" Offset calibration  Connnect CH1 and CH2 to ground."));
    lcd_goto(0,3); lcd_putsp(PSTR("Press 5 to start"));
    // Show current calibration
    for(i=0; i<8; i++) {
        lcd_goto(0,i+6);
        printN(128-(int8_t)eeprom_read_byte(&offsetsCH1[i]));
        lcd_write_data(' ');
        printN(128-(int8_t)eeprom_read_byte(&offsetsCH2[i]));
    }
    while(key!=0);  // Wait for key release
    while(key==0);  // Wait for new key press
    if(key!=K5) {
        if(key==KA) key=0;
        while(key!=0);      // Wait for key release
        clrbit(Signals, testing); // Exit test mode
        return;
    }
    lcd_goto(0,4);
    lcd_putsp(PSTR("Calibrating..."));
    // Cycle thru all the 8 gains
    for(i=0; i<8; i++) {
        lcd_goto(0,i+6);
        // Enable ADC and CH SEL clocks, set gain
        PORTH.OUT = ((i&0x07)<<3) | (i&0x07);
        _delay_ms(50);
        cli();      // Disable global interrupts
        acquire5(CH1.data, CHD.data, CH2.data);
        sei();      // Enable global interrupts
        // Calculate offset for CH1
        average=0;
        j=0;
        do {
            average+= CH1.data[i];
        } while(++j);
        j = (uint8_t )(average>>8);
        if(j>=128) CH1.offset=-(j-128);
        else CH1.offset = (128-j);
        eeprom_write_byte(&offsetsCH1[i], CH1.offset);
        printN(j);
        lcd_write_data(' ');
        // Calculate offset for CH2
        average=0;
        j=0;
        do {
            average+= CH2.data[i];
        } while(++j);
        j = (uint8_t )(average>>8);
        if(j>=128) CH2.offset=-(j-128);
        else CH2.offset = (128-j);
        eeprom_write_byte(&offsetsCH2[i], CH2.offset);
        printN(j);
    }
    lcd_goto(0,15); lcd_putsp(PSTR("Cal complete"));
}

// Calibrate DAC gain and offset, connect AWG to CH1
// Adjust with rotary encoders
static void CalibrateDAC(void) {
    uint8_t i, step=0, data, average;
    uint8_t test, bestoffset, bestgain, bestmeasure1;
    uint16_t sum, bestmeasure2;
    lcd_init(GRAPHTEXT);

    // ADS931 power, output enable, CH gains
    PORTH.OUT = 6;
    CH1.offset=(signed char)eeprom_read_byte(&offsetsCH1[6]);

    AWGAmp=127;         // Amplitude range: [0,127]
    AWGtype=1;          // Waveform type
    AWGduty=256;        // Duty cycle range: [0,511]
    AWGOffset=0;        // 0V offset
    desiredF = 100000;  // 1kHz
    BuildWave();
    // Timer TCF1 - OC1A: ADC Clock
    TCF1.CTRLB = 0x11;      // CCAEN override, Frequency mode
    TCF0.CTRLB = 0x81;      // CCDEN override, Frequency mode
    TCF0.CCA = 199;         // Select clock
    TCF1.CCA = 99;          // ADC clock
    TCF0.CTRLA  = 0x01;     // Enable Timer, Prescaler: clk/1
    TCF1.CTRLA  = 0x01;     // Enable Timer, Prescaler: clk/1
    while(step<7) {
        while(!testbit(TCC1.INTFLAGS, TC1_OVFIF_bp));   // wait for refresh timeout
        setbit(TCC1.INTFLAGS, TC1_OVFIF_bp);
        // Acquire data
        ONGRN();                        // Blink on LED at a time
        cli();      // Disable global interrupts
        acquire5(CH1.data, CHD.data, CH2.data);
        sei();      // Enable global interrupts
        OFFGRN();
        lcd_clear_graphics();
        // Display waveform
        i=0; sum=0;
        do {
            data=addwsat(CH1.data[i],CH1.offset);
            sum+=data;
            set_pixel(i>>1, data>>1);    // CH1
        } while(++i);
        average=(uint8_t)(sum>>8);

        switch(step) {
            case 0: // Connect AWG to CH1
                lcd_goto(0,0); lcd_putsp(PSTR("AWG Calibration Connect AWG CH1 Press 5 to start"));
                while(key!=0);  // Wait for key release
                step++;
            break;
            case 1:
                if(key) {
                    if(key==K5) step++;
                    else step=7;         // Did not press 5 -> exit
                    if(key==KA) key=0;
                    while(key!=0);      // Wait for key release
                }
            break;
            case 2: // Output 0V from AWG
                AWGAmp=1;         // Amplitude range: [0,127]
                AWGtype=1;        // Waveform type
                BuildWave();
                lcd_goto(0,3); lcd_putsp(PSTR("Adjusting offset"));
                // ADS931 power, output enable, CH gains
                PORTH.OUT = 0;
                CH1.offset=(signed char)eeprom_read_byte(&offsetsCH1[0]);
                step++;
                bestoffset = 0;
                test = 0;
                bestmeasure1=0;
                DACA.OFFSETCAL = 0;
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
                DACA.OFFSETCAL = test;
                if(test>=128) {
                    step++;
                    DACA.OFFSETCAL = bestoffset;   // Load DACA offset calibration
                }
            break;
            case 4: // Output -1.75V from AWG
                AWGAmp=0;           // Full Amplitude
                AWGtype=1;          // Waveform type
                AWGOffset=112;      // Offset = -1.75
                BuildWave();
                lcd_goto(0,5); lcd_putsp(PSTR("Adjusting gain"));
                // ADS931 power, output enable, CH gains
                PORTH.OUT = 4;  // 0.5V / div
                CH1.offset=(signed char)eeprom_read_byte(&offsetsCH1[4]);
                step++;
                bestgain = 0;
                test=0;
                bestmeasure2=0;
                DACA.GAINCAL = 0;
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
                DACA.GAINCAL = test;
                if(test>=128) {
                    step++;
                    DACA.GAINCAL = bestgain;
                }
            break;
            case 6: // Calibration complete
                // Save calibration results
                AWGAmp=0;
                eeprom_write_byte(&EEDACoffset, bestoffset);    // Save offset calibration
                eeprom_write_byte(&EEDACgain, bestgain);        // Save gain calibration
                lcd_goto(0,15); lcd_putsp(PSTR("Cal complete"));
                step++;
            break;
        }
    }
    // Restore Waveform
    LoadAWGvars();              // Load AWG settings
    BuildWave();                // Construct AWG waveform
}

// Restore default settings and clear calibration data
static void Factory(void) {
    uint8_t i=0;
    lcd_init(TEXT);
    lcd_goto(0,0); lcd_putsp(PSTR("Factory options:"));
    lcd_goto(0,2); lcd_putsp(PSTR("1) Calibrate offsets"));
    lcd_goto(0,3); lcd_putsp(PSTR("2) Calibrate AWG"));
    lcd_goto(0,4); lcd_putsp(PSTR("3) Clear CH offset"));
    lcd_goto(0,5); lcd_putsp(PSTR("4) Clear AWG cal"));
    lcd_goto(0,6); lcd_putsp(PSTR("5) Default settings"));
    lcd_goto(0,8);

    while(key!=0);
    while(key==0);  // wait for new key press
    put_char(pgm_read_byte(&Letter[key-1]));
    switch(key) {
        case K1:
            Calibrate();
        break;
        case K2:
            CalibrateDAC();
        break;
        case K3:    // Reset calibration data for each gain
            CH1.offset = 0;
            CH2.offset = 0;
            for(i=0; i<8; i++) {
                eeprom_write_byte(&offsetsCH1[i], 0);
                eeprom_write_byte(&offsetsCH2[i], 0);
            }
        break;
        case K4:    // Reset AWG calibration
            eeprom_write_byte(&EEDACoffset, 0);
            eeprom_write_byte(&EEDACgain, 0);
        break;
        case K5:    // Restore MSO settings
            CH1.position = -64;     // CH1 vertical position
            CH2.position = +64;     // CH2 vertical position
            CHD.position = 57;      // CHD vertical position
            CH1.option   = 0x0D;    // CH1 on, 1V/div
            CH2.option   = 0x0D;    // CH2 on, 1V/div
            CHD.option   = 0x24;    // CHD off, thick line when low
            srate        = 10;      // Sampling rate, start with 2mS/s
            Mcursor      = 0x00;    // Cursors off
            MStatus      = 0x01;    // Set free trigger
            MFFT         = 0x01;    // Use hamming window, no logarithm
            timeout      = 60;      // Trigger timeout, start with aprox. 500mS
            tdelay       = 0;       // Trigger delay
            tlevel       = 128;     // Trigger level
            tpre         = 128;     // Number of Pre Trigger samples
            tsource      = 1;       // Trigger source, start with CH1
            Mset         = 0x1D;    // Set scope mode, show grid, line, backlight on
            ONBKLT();
            SaveEE();
            eeprom_write_byte(&EEapp, 0);    // Default main menu
            // Restore AWG settings
            desiredF    = 100000;   // Frequency
            AWGtype     = 1;        // Type
            AWGAmp      = 0; 	    // Amplitude
            AWGduty     = 0x0100;   // Duty cycle
            AWGOffset   = 0;
            SaveAWGvars();
        break;
        default:
            clrbit(Signals, testing); // Exit test mode
            return;
    }
    lcd_putsp(PSTR(" Ok!"));
    while(key!=0);  // wait for key release
    // Wait a few seconds or until a key is pressed
    i=0;
    while(!key && i<250) {
        i++;
        _delay_ms(25);
    }
    while(key!=0);  // Wait for key release
    clrbit(Signals, testing); // Exit test mode
}

// RTC clock, this function is called every 60 seconds
ISR(RTC_OVF_vect) {
    minutes++;
    if(minutes>=60) {
        minutes=0;
        if(hours<99) hours++;
    }
}



// Return VCC in mV
uint16_t readVCC(void) {
    ADCA.CTRLA = 0x01;          // Enable ADC
    ADCA.CTRLB = 0x00;          // unsigned mode, no free run, 12 bit
    ADCA.REFCTRL =0x20;         // REF= AREF
    ADCA.PRESCALER = 0x07;      // Prescaler 512 (62.5kHZ ADC clock)
    ADCA.CH0.CTRL = 0x00;       // Internal input
    ADCA.CH0.MUXCTRL = 0x02;    // Channel 0 input: VCC/10
    ADCA.CH0.INTCTRL = 0x00;    // Conversion complete IF
    _delay_us(200);              // Wait for temperature reference to settle
    ADCA.CH0.CTRL = 0x80;       // Start conversion
    ADCA.CH0.INTFLAGS = 0x01;   // Clear flag
    while(!testbit(ADCA.CH0.INTFLAGS,0));
    ADCA.CTRLA = 0x00;          // Disable ADC
    // Convert result to mV and substract XMEGA's deltaV (vref * 0.05)
    return ((ADCA.CH0.RES*39/2)-100);
}
