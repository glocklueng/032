/*****************************************************************************

XMEGA Oscilloscope and Development Kit

Gabotronics
September 2012

Copyright 2012 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "main.h"
#include "awg.h"

// Global AWG variables
uint8_t AWGBuffer[256]; // AWG Output Buffer
uint8_t cycles;         // Cycles in AWG buffer

void moveF(void) {
	uint8_t i;
	uint32_t add;
	uint32_t F;
	F=M.AWGdesiredF;
    i=6;
	do {    // Find range
        if(F>=pgm_read_dword_near(Powersof10+i)) break;
    } while(i--);
	add=pgm_read_dword_near(Powersof10+i-2);
	if(testbit(Misc,negative)) F-=add;
	else if(testbit(Misc, bigfont)) F+=add;
	else {  // Shortcuts
		if(i==2) i=7;
        F=pgm_read_dword_near(Powersof10+i-1);
    }
	M.AWGdesiredF = F;
}

// Output Frequency = cycles * 125000 / Period
void BuildWave(void) {
    uint8_t i,j;
    int8_t *p;
    uint16_t step;          // used in duty cycle calculation
    uint16_t d;             // used in duty cycle calculation
    uint32_t Flevel=1600000;
    if(M.AWGamp>0)                  M.AWGamp=0;                 // AWGAmp must be negative
    if(M.AWGduty==0)                M.AWGduty=1;                // Zero is invalid
    if(M.AWGdesiredF<100)           M.AWGdesiredF = 100;        // Minimum Freq= 1Hz
    else if(M.AWGdesiredF>12500000) M.AWGdesiredF = 12500000;	// Maximum Freq= 125KHz

    // Minimum frequency with cycle=32: 61Hz
    // Maximum frequency with cycle=1:  3.906kHz
    // Tradeoff: Frequency resolution  <-> Amplitude resolution
    // Low periods have poor Freq resolution
    // High cycles have poor Amp resolution
    // Determine number of cycles               // F > 16000 -> cycles = 32
    for(i=0, cycles=64; i<6; i++) {             // F >  8000 -> cycles = 16
        if(M.AWGdesiredF>Flevel) break;         // F >  4000 -> cycles =  8
		Flevel=Flevel>>1;                       // F >  2000 -> cycles =  4
        cycles=cycles>>1;                       // F >  1000 -> cycles =  2
    }                                           // F >   500 -> cycles =  1
    // Construct waveform                       // else cycles = 1, prescaler=2
    i=0;
    uint16_t Seed;
    p=(int8_t *)Temp.DATA.AWGTemp1;
    switch(M.AWGtype) {
        case 0: // Random
            Seed = TCD1.CNT;
            do {
                Seed = 25173 * Seed + 1;
                *p++ = hibyte(Seed);
            } while(++i);
        break;
        case 1: // Sine
            do { *p++ = Sin(i+64); } while(++i);
        break;
        case 2: // Square
            do { *p++ = (i<128)?-127:127; } while(++i);
        break;
        case 3: // Triangle
            for(;i<128;i++) *p++ = 127-((i)<<1);
            do { *p++ = -127+(i<<1); } while(++i);
        break;
        case 4: // Exponential
            for(;i<128;i++) *p++ = -pgm_read_byte_near(Exp+i);
            do { *p++ = pgm_read_byte_near(Exp-128+i); } while(++i);
        break;
        case 5: // Custom wave from EEPROM
            eeprom_read_block(Temp.DATA.AWGTemp1, EEwave, 256);
        break;
    }
    // Prepare buffer:
    // ******** Duty cycle ********
    i=0; step=0; d=(256-M.AWGduty)<<1;
	int8_t k;
    p=(int8_t *)Temp.DATA.AWGTemp1;
    do {
        j=hibyte(step);
        Temp.DATA.AWGTemp2[j] = *p;
        k=*p++;
        if(j<255) Temp.DATA.AWGTemp2[j+1] = (k+(*p))/2;
        step+=d;
        if(i==127) d=M.AWGduty<<1;
    } while(++i);
    i=0;
    step = ((6250000 * cycles) / M.AWGdesiredF ) - 1;
    PMIC.CTRL = 0x06;   // Disable low level interrupts
    if(cycles>1) TCD1.CTRLA = 0x01;   // Prescaler: clk/1
    else         TCD1.CTRLA = 0x02;   // Prescaler: clk/2
    // Avoid discontinuity when changing frequency by writing to PERBUF
    TCD1.PERBUF = step;    // Set Period
    uint8_t c;
    c=cycles>>1;
    if(c==0) c++;
    do {
    // ******** Multiply by Gain ********
        j=FMULS8(M.AWGamp,Temp.DATA.AWGTemp2[(uint8_t)(i*c)]);
    // ******** Add Offset ********
        AWGBuffer[i]=saddwsat(j,M.AWGoffset);
    } while(++i);
    clrbit(MStatus, updateawg);
    PMIC.CTRL = 0x07; // Enable all interrupts
}
/*
void VCO(void) {
    // http://www.phy.mtu.edu/~suits/notefreqs.html
    // http://www.phy.mtu.edu/~suits/NoteFreqCalcs.html
    clr_display();
    M.CH1gain=1;    // +/- 10.24V range
    M.CH2gain=1;    // +/- 10.24V range
    Srate = 4;      //
    // CH1 -> Frequency, CH2 -> Amplitude
    while(!testbit(Key,KD)) {
        StartDMAs();                // Takes about 1.024ms
        show_display();             // Takes about 1ms
        ADCA.CTRLB = 0x14;          // Stop free run of ADC (signed mode, no free run, 8 bit)
        // Disable DMAs
        clrbit(DMA.CH0.CTRLA, 7);
        clrbit(DMA.CH2.CTRLA, 7);
        clrbit(DMA.CH1.CTRLA, 7);
    }
}*/
