/*****************************************************************************

Xmegalab / XMultiKit: XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
July 2009

Copyright 2009 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "main.h"
#include "awg.h"

uint8_t EEMEM EEtype    = 1;    // Type
uint8_t EEMEM EEamp     = 0; 	// Amplitude
int8_t  EEMEM EEoffset  = 0;    // Offset
uint8_t EEMEM EEduty    = 256;  // Duty cycle high byte
uint32_t EEMEM EEdesiredF = 100000;

// Global AWG variable
int8_t      AWGAmp;         // Amplitude range: [0,-128]
uint8_t     AWGtype;        // Waveform type
uint16_t    AWGduty;        // Duty cycle range: [0,511]
int8_t      AWGcycles;      // Cycles in buffer
int8_t 		AWGOffset;		// Offset
uint32_t    desiredF;       // Desired frequency

void AWG(void) {
    uint8_t i,j;
    char text[17];
    uint32_t oldF=0;         // Old frequency
    uint8_t encoder=0;      // encoder mode (freq/amp or offset/duty cycle)
    lcd_init(GRAPHTEXT);
    setbit(Signals, update);
    eeprom_write_byte(&EEapp, KB);    // Save AWG application state

    // Load settings from EEPROM
    LoadAWGvars();

    // Set rotary encoders with internal quadrature decoder
    PORTA.PIN6CTRL = 0x1B;  // Pull up on pin PA6, sense low level (Encoder SW6)
    PORTA.PIN7CTRL = 0x1B;  // Pull up on pin PA7, sense low level (Encoder SW6)
    PORTA.INTCTRL = 0x01;   // PORTA will generate low level interrupts
    PORTC.PIN0CTRL = 0x1B;  // Pull up on pin PC0, sense low level (Encoder SW7)
    PORTC.PIN1CTRL = 0x1B;  // Pull up on pin PC1, sense low level (Encoder SW7)
    PORTC.INTCTRL = 0x00;   // no interrupts
    EVSYS.CH2MUX = 0x56;    // Event CH4 = PORTA Pin 6 (Encoder SW6)
    EVSYS.CH2CTRL = 0x0F;   // Enable quadrature decoder with 8 sample filter
    EVSYS.CH4MUX = 0x60;    // Event CH6 = PORTC Pin 0 (Encoder SW7)
    EVSYS.CH4CTRL = 0x0F;   // Enable quadrature decoder with 8 sample filter
    // Quadrature decoder on Timers TCC0 and TCC1 used for each rotary encoder
    TCC0.CTRLA = 0x0A;      // Event CH2 is clk
    TCC0.CTRLD = 0x6A;      // Quadrature decode, source is CH2
    TCC0.INTCTRLA = 0;
    TCC1.CTRLA = 0x0C;      // Event CH4 is clk
    TCC1.CTRLD = 0x6C;      // Quadrature decode, source is CH4
    // Load data to Rotary Encoders counters
    TCC0.CNT = 128;
    TCC0.PER = 0x00FF;
    TCC1.CNT = 128+AWGAmp;  // Load Amplitude
    TCC1.PER = 128;         // Rollover for rotary encoder

    for(;;) {
        if(testbit(Signals, update)) {   // Update
            clrbit(Signals, update);
            BuildWave();
            // Update LCD
            lcd_clear_graphics();
            lcd_clear_text();
            bitmap(0,119,waves);   // Display waves menu
            lcd_goto(0,9);
            // Show actual frequency
            sprintf(text,"Freq:%4.3fHz",(double)AWGcycles*(F_CPU/256)/(TCD0.PER+1));
            lcd_puts(text);
            lcd_goto(0,10);
            sprintf(text,"P-P Amp: %2.3f V",(float)(-AWGAmp)/32);
            lcd_puts(text);
            lcd_goto(0,11);
            sprintf(text,"Offset: %+2.3f V",(float)(-AWGOffset)/64);
            lcd_puts(text);
            lcd_goto(0,12);
            sprintf(text,"Duty cycle:%2.1f",(float)AWGduty*100.0/512);
            lcd_puts(text);
            lcd_goto(0,13); lcd_putsp(PSTR("encoders: "));
            lcd_goto(1,14);
            if(encoder) lcd_putsp(PSTR("Offset - duty"));
            else lcd_putsp(PSTR("  Freq - Amp"));
            i=0;
            do {    // Show waveform on LCD
                set_pixel(i>>1,AWGBuffer[i]>>2); i++;
                set_pixel(i>>1,AWGBuffer[i]>>2); i++;
            } while(i);
        }
        else _delay_ms(10);
        // Encoders input
        if(encoder) {   // offset / duty cycle
            if(AWGOffset != (signed char)(128-TCC0.CNTL)) {
                AWGOffset = (signed char)(128-TCC0.CNTL);    // offset
                setbit(Signals, update);
            }
            if(AWGduty != TCC1.CNT) {
                AWGduty = TCC1.CNT;                   // duty cycle
                setbit(Signals, update);
            }
        }
        else {          // frequency / amplitude
            if(TCC0.CNTL<128) increaseF();
            if(TCC0.CNTL>128) decreaseF();
            TCC0.CNT=128;
            if(oldF!=desiredF) {
                oldF = desiredF;
                setbit(Signals, update);
            }
            if((128+AWGAmp) != TCC1.CNTL) {
                AWGAmp = (int8_t)(TCC1.CNTL-128);             // amplitude
                setbit(Signals, update);
            }
        }
        if(key) {
            switch(key) {
                case KA: AWGtype = 1; break;   // Sine
                case KB: AWGtype = 2; break;   // Square
                case KC: AWGtype = 3; break;   // Triangle
                case KD: AWGtype = 4; break;   // Exponential
                case KE: AWGtype = 5; break;   // Random
                case KS: // Exit
                    // Save settings to EEPROM
                    SaveAWGvars();
                    eeprom_write_byte(&EEapp, 0);    // Default main menu
                    // restore encoders behavior
                    PORTA.PIN6CTRL = 0x18;  // Pull up on pin PA6, sense both edges
                    PORTA.PIN7CTRL = 0x18;  // Pull up on pin PA7, sense both edges
                    PORTA.INTCTRL = 0x05;   // PORTA will generate low level interrupts                
                    PORTC.PIN0CTRL = 0x18;  // Pull up on pin PC0, sense both edges
                    PORTC.PIN1CTRL = 0x18;  // Pull up on pin PC1, sense both edges
                    PORTC.INTCTRL = 0x04;   // PORTC will generate low level interrupts
                    EVSYS.CH2MUX = 0;
                    EVSYS.CH2CTRL = 0;
                    EVSYS.CH4MUX = 0;
                    EVSYS.CH4CTRL = 0;
                    return;
                case KP: encoder=!encoder;
                         if(encoder) {
                            TCC0.CNT = (uint8_t)(128-AWGOffset);
                            TCC1.CNT = AWGduty;
                            TCC1.PER = 512;
                         }
                         else {
                            TCC0.CNT = TCD0.PER;
                            TCC1.CNT = AWGAmp+128;
                            TCC1.PER = 128;
                         }
                break;
                case K0: AWGOffset=0;
                         if(encoder) TCC0.CNT = (uint8_t)(128-AWGOffset);                
                break;
                case K1: desiredF=200; break;
                case K2: desiredF=1000; break;
                case K3: desiredF=5000; break;
                case K4: desiredF=10000; break;
                case K5: desiredF=50000; break;
                case K6: desiredF=100000; break;
                case K7: desiredF=500000; break;
                case K8: desiredF=1000000; break;
                case K9: desiredF=3125000; break;
            }
            oldkey=0; key=0;
        }
    }
}

void increaseF() {
    if(desiredF<1000) {
        desiredF+=1;
    }
    else if(desiredF<10000) {
        desiredF+=10;
    }
    else if(desiredF<100000) {
        desiredF+=100;
    }
    else if(desiredF<1000000) {
        desiredF+=1000;
    }
    else {
        desiredF+=10000;
        if(desiredF > 3125000) desiredF = 3125000;
    }
}

void decreaseF() {
    if(desiredF<=1000) {
        desiredF-=1;
        if(desiredF < 191) desiredF = 191;  // Minimum Freq= 1.91Hz
    }
    else if(desiredF<=10000) {
        desiredF-=10;
    }
    else if(desiredF<=100000) {
        desiredF-=100;
    }
    else if(desiredF<=1000000) {
        desiredF-=1000;
    }
    else desiredF-=10000;
}

// Find Period and number of cycles depending on the desired frequency
void FindValue() {
    if(desiredF<97656) {
        AWGcycles = 1;
    }
    else if(desiredF<195313) {
        AWGcycles = 2;
    }
    else if(desiredF<390625) {
        AWGcycles = 4;
    }
    else {
        AWGcycles = 8;
    }
    TCD0.PER = (12500000 / ( desiredF / AWGcycles)) - 1;
    if(TCD0.PER<31) TCD0.PER=31;
}

void LoadAWGvars(void) {
    AWGtype     = eeprom_read_byte(&EEtype);            // Load waveform type
    AWGAmp      = eeprom_read_byte(&EEamp);             // Load amplitude
    if(AWGAmp>0) AWGAmp=0;                              // AWGAmp must be negative
    AWGduty     = eeprom_read_word(&EEduty);            // Load duty cycle
    if(AWGduty>512) AWGduty = 256;                      // Duty cycle must be inside [0,512]
    desiredF = eeprom_read_dword(&EEdesiredF);          // Load frequency
	AWGOffset 	= (int8_t)eeprom_read_byte(&EEoffset);  // Load offset
}

void SaveAWGvars(void) {
    eeprom_write_byte(&EEtype, AWGtype);            // Save waveform type
    eeprom_write_byte(&EEamp, AWGAmp);              // Save amplitude
    eeprom_write_word(&EEduty, AWGduty);            // Save duty cycle
    eeprom_write_dword(&EEdesiredF, desiredF);      // Save frequency
	eeprom_write_byte(&EEoffset, AWGOffset);		// Save offset
}

void BuildWave(void) {
    uint8_t i,j,k;
    uint16_t step;          // used in duty cycle calculation
    uint16_t d;             // used in duty cycle calculation
    int8_t AWGTemp1[256];   // AWG Temp1
    int8_t AWGTemp2[256];   // AWG Temp2

    // Find Period and number of cycles depending on the desired frequency
    FindValue();
    // Construct waveform
    i=0;
    switch(AWGtype) {
        case 0: // DC
            do { AWGTemp1[i] = 0; } while(++i);
        break;
        case 1: // Sine
            do { AWGTemp1[i] = Cos(i); } while(++i);
        break;
        case 2: // Square
            do { AWGTemp1[i] = (i<128)?127:-127; } while(++i);
        break;
        case 3: // Triangle
            for(;i<128;i++) AWGTemp1[i] = 127-((i)<<1);
            do { AWGTemp1[i] = -127+(i<<1); } while(++i);
        break;
        case 4: // Exponential
            for(;i<128;i++) {
                AWGTemp1[i]     = 128-pow(256,(float)(128-i)/128);
                AWGTemp1[i+128] = pow(256,(float)(128-i)/128)-128;
            }
        break;
        case 5: // Random
            do { AWGTemp1[i] = prandom(); } while(++i);
        break;
        default:
            do { AWGTemp1[i] = AWGtype; } while(++i);
    }
    // Prepare buffer:
    // ******** Duty cycle ********
    i=0; j=0; step=0; d=512-AWGduty;
    do {
        j=hibyte(step);
        AWGTemp2[j] = AWGTemp1[i];
        if(j<255) AWGTemp2[j+1] = (AWGTemp1[i]+AWGTemp1[i+1])/2;
        step+=d;
        if(i==127) d=AWGduty;
    } while(++i);
    // ******** Multiply by Gain ********
    i=0;
    do {
        j=128+FMULS8(AWGAmp,AWGTemp2[(uint8_t)(i*AWGcycles)]);
    // ******** Add Offset ********
        k = j+ AWGOffset;
        if(AWGOffset>=0) {
            if(j>255-AWGOffset) k=255;
        }
        else {
            if(j<(-AWGOffset)) k=0;
        }
        AWGBuffer[i]=k;
    } while(++i);	
}
