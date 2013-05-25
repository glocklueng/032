/*****************************************************************************

XMEGA Oscilloscope and Development Kit

*** THIS IS A DEMO VERSION OF THE MSO.C FILE ***

Gabotronics
February 2012

Copyright 2012 Gabriel Anzziani

This file is proprietary software.
This file shall no be distributed.

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "main.h"
#include "mso.h"
#include "logic.h"
#include "awg.h"
#include "interface.h"
#include "USB\usb_xmega.h"

// Function prototypes
void Reduce(void);
void RestorefromMeter(void);		// Restores srate and gains
void GoingtoMeter(void);			// Saves srate and gains
uint8_t fft_stuff(uint8_t *p1);
void AutoCursorV(void);             // Automatically set vertical the cursors
static inline void Measurements(void);  // Measurements for Meter Mode
static inline void ShowCursorV(void);   // Display Vertical Cursor
static inline void ShowCursorH(void);   // Display Horizontal Cursor
void CheckMax(void);                // Check variables
void CheckPost(void);               // Check Post Trigger
void Apply(void);                   // Apply oscilloscope settings

// Global variables
uint8_t  adjusting = 0;             // Auto setup adjusting step
uint16_t slowval;                   // Slow sampling rate time value
uint8_t old_s, old_g1, old_g2;      // sampling, gains before entering meter mode
uint8_t shortcuti  = 0;             // shortcut index

ACHANNEL CH1,CH2;                   // Analog Channel 1, Channel 2
DATA D;                             // Data samples
uint8_t EEMEM EECHREF1[256] = {0};  // Reference waveform CH1
uint8_t EEMEM EECHREF2[256] = {0};  // Reference waveform CH2

// ADC system clock timers

// TCE0 controls Interrupt ADC, srate: 6    7    8    9    10
const uint16_t TCE0val[5] PROGMEM = { 249, 499, 999, 2499, 4999 };

// TCE0 timer is fixed for slow sampling rates (20mS/div and under)
// slowcnt controls the sampling rate.
const uint16_t slowcnt[11] PROGMEM = { 1,2,4,8,20,40,80,200,400,800,2000 };
// milivolts or volts per pixels * 100000 / 32
// Range                      5.12V 2.56V 1.28V 0.64V  320mV  160mV   80mV
const int16_t milivolts[7] = { 1000,  500,  250,  125, 62500, 31250, 15625 };

const char menustxt[][35] PROGMEM = {           // Menus:
    " CH ON   \0      GAIN-    \0   GAIN+",     // 0  Channel
    "LOGIC ON \0     SELECT   \0 PROTOCOL",     // 1  Logic
    " NORMAL   \0    SINGLE   \0 CIRCULAR",     // 2  Sniffer mode
    " NORMAL   \0    SINGLE   \0    AUTO ",     // 3  Trigger Type
    " VCURSOR \0   HCUR CH1  \0  HCUR CH2",     // 4  Cursor
    " HAMMING   \0    HANN   \0  BLACKMAN",     // 5  Spectrum Analyzer Window
    "  CH1    \0       CH2     \0  LOGIC ",     // 6  Trigger Source
    " GRID   \0  FLIP DISPLAY \0  INVERT ",     // 7  Display
    "  VDC      \0   V P-P  \0 FREQUENCY ",     // 8  Meter mode
    "  SINE    \0    SQUARE  \0  TRIANGLE",     // 9  AWG
    "POSITION   \0   INVERT   \0    MATH ",     // 10 Channel options
    "POSITION   \0   INVERT  \0   THICK0 ",     // 11 Logic options 1
    " CH1 \0          CH2 \0        LOGIC",     // 12 Menu Select 1 - Channel
    "TRIGTYPE  \0   TRIGSRC   \0 MORETRIG",     // 13 Menu Select 2 - Trigger
    " SCOPE    \0    METER   \0      FFT ",     // 14 Menu Select 3 - Mode
    " IQ FFT   \0  FFT WINDOW  \0    LOG ",     // 15 Menu Select 4 - FFT
    "CURSORS \0     DISPLAY  \0       AWG",     // 16 Menu Select 5 - Misc
    "WAVE TYPE  \0   SWEEP   \0 FREQUENCY",     // 17 AWG Menu 2
    " WINDOW   \0    EDGE    \0    SLOPE ",     // 18 Trigger Mode
    "  AUTO     \0   TRACK   \0 REFERENCE",     // 19 More Cursor Options
    "  ALL     \0    BIT_0    \0    BIT_1",     // 20 Logic Channel Select
    " BIT_2    \0    BIT_3    \0    BIT_4",     // 21 Logic Channel Select
    " BIT_5    \0    BIT_6    \0    BIT_7",     // 22 Logic Channel Select
    " EXT TRIG \0    BIT_0    \0    BIT_1",     // 23 Logic Trigger Select
    " PARALLEL  \0   SERIAL  \0      PULL",     // 24 Logic options 2
    " I2C      \0     UART    \0     SPI ",     // 25 Protocol
    " NO PULL  \0   PULL UP  \0 PULL DOWN",     // 26 Logic Inputs Pull
    " PERSISTENT  \0   LINE   \0    SHOW ",     // 27 Display
    " CPOL     \0     CPHA    \0         ",     // 28 SPI Clock polarity and phase
    " SUBTRACT \0  MULTIPLY  \0  AVERAGE ",     // 29 Channel math
    "AMPLITUDE \0  DUTY CYCLE \0   OFFSET",     // 30 AWG Menu 3
    " ROLL     \0   ELASTIC  \0  XY MODE ",     // 31 Scope options
    "TRIGMODE  \0  POSTTRIG \0   TRIGHOLD",     // 32 Menu Trigger 2
    " EXP      \0    NOISE   \0   CUSTOM ",     // 33 AWG Menu 4
    " SPEED    \0    MODE    \0    RANGE ",     // 34 AWG Menu 5
    "SW FREQ    \0   SW AMP  \0  SW DUTY ",     // 35 AWG Menu 6
    "  DOWN    \0  PINGPONG   \0  ACCEL\0",     // 36 Sweep Mode Menu
//    " SWEEP    \0  CV/GATE  \0 POS. RANGE",     // 37 Advanced Sweep Settings
//    "CV/GATE   \0  CONTINUOUS \0   C1=1V ",     // 38 CV/Gate Menu
//    " CAN      \0    1 WIRE   \0    MIDI ",     // 39 More Sniffer protocols
};

const char menupoint[] PROGMEM = {  // Menu text table
    0,  // MCH1 Channel 1
    0,  // MCH2 Channel 2
    1,  // MCHD Logic
    2,  // MSNIFFER Sniffer mode
    3,  // MTRIGTYPE Trigger Type
    4,  // MCURSOR1 Cursor
    5,  // MWINDOW Spectrum Analyzer Window
    6,  // MSOURCE Trigger Source
    7,  // MDISPLAY1 Display
    8,  // MMETER Meter mode
    9,  // MAWG AWG
    10, // MCH1OPT Channel 1 options
    10, // MCH2OPT Channel 2 options
    11, // MCHDOPT1 Logic options 1
    12, // MMAIN1 Menu Select 1 - Channel
    13, // MMAIN2 Menu Select 2 - Trigger
    14, // MMAIN3 Menu Select 3 - Mode
    15, // MMAIN4 Menu Select 4 - FFT
    16, // MMAIN5 Menu Select 5 - Misc
    17, // MAWG2 AWG Menu 2
    33, // MAWG4 AWG Menu 4
    34, // MAWG5 AWG Menu 5
    35, // MAWG6 AWG Menu 6
//    37, // MAWG7 AWG Menu 7
//    38, // MCVG CV/Gate Menu
    31, // MSCOPEOPT Scope options
    32, // MTRIG2 Trigger menu 2
    18, // MTRIGMODE Trigger edge and mode
    19, // MCURSOR2 More Cursor Options
    20, // MCHDSEL1 Logic Channel Select
    21, // MCHDSEL2 Logic Channel Select
    22, // MCHDSEL3 Logic Channel Select
    23, // MTSEL1 Logic Trigger Select
    21, // MTSEL2 Logic Trigger Select
    22, // MTSEL3 Logic Trigger Select
    24, // MCHDOPT2 Logic options 2
    25, // MPROTOCOL Protocol
//    39, // MPROTOCOL2 More Protocols
    26, // MCHDPULL Logic Inputs Pull
    27, // MDISPLAY2 Display
    28, // MSPI SPI Clock polarity and phase
    29, // MCH1MATH Channel 1 math
    29, // MCH2MATH Channel 2 math
    30, // MAWG3 AWG Menu 3
    36, // MSWMODE Sweep Mode Menu
};

const char Next[] PROGMEM = {  // Next Menu
//  Next:          Current:
    MMAIN1,     // Mdefault default
    MCH1OPT,    // MCH1 Channel 1
    MCH2OPT,    // MCH2 Channel 2
    MCHDOPT1,   // MCHD Logic
    Mdefault,   // MSNIFFER Sniffer mode
    MMAIN2,     // MTRIGTYPE Trigger Type
    MCURSOR2,   // MCURSOR1 Cursor
    MMAIN4,     // MWINDOW Spectrum Analyzer Window
    MMAIN2,     // MSOURCE Trigger Source
    Mdefault,   // MDISPLAY1 Display
    MMAIN3,     // MMETER Meter mode
    MAWG4,      // MAWG AWG
    Mdefault,   // MCH1OPT 1 Channel 1
    Mdefault,   // MCH2OPT 2 Channel 2
    MCHDOPT2,   // MCHDOPT1 3 Logic
    MMAIN2,     // MMAIN1 Menu Select 1 - Channel
    MMAIN3,     // MMAIN2 Menu Select 2 - Trigger
    MMAIN5,     // MMAIN3 Menu Select 3 - Mode
    MMAIN3,     // MMAIN4 Menu Select 4 - FFT
    Mdefault,   // MMAIN5 Menu Select 5 - Misc
    MAWG3,      // MAWG2 AWG Menu 2
    MAWG2,      // MAWG4 AWG Menu 4
    Mdefault,   // MAWG5 AWG Menu 5
    MAWG5,      // MAWG6 AWG Menu 6
//    Mdefault,   // MAWG7 AWG Menu 7
//    Mdefault,   // MCVG CV/Gate Menu
    MMAIN3,     // MSCOPEOPT Scope options
    Mdefault,   // MTRIG2 Trigger Menu 2
    MTRIG2,     // MTRIGMODE Trigger edge and mode
    Mdefault,   // MCURSOR2 More Cursor Options
    MCHDSEL2,   // MCHDSEL1 Logic Channel Select
    MCHDSEL3,   // MCHDSEL2 Logic Channel Select
    Mdefault,   // MCHDSEL3 Logic Channel Select
    MTSEL2,     // MTSEL1 Logic Trigger Select
    MTSEL3,     // MTSEL2 Logic Trigger Select
    Mdefault,   // MTSEL3 Logic Trigger Select
    Mdefault,   // MCHDOPT2 Decode
    Mdefault,   // MPROTOCOL Protocol
//    Mdefault,   // MPROTOCOL2 More Protocols
    Mdefault,   // MCHDPULL Logic Inputs Pull
    MDISPLAY1,  // MDISPLAY2 Display
    MSNIFFER,   // MSPI SPI Clock polarity and phase
    Mdefault,   // MCH1MATH Channel 1 math
    Mdefault,   // MCH2MATH Channel 2 math
    Mdefault,   // MAWG3 AWG Menu 3
    MAWG5,      // MSWMODE Sweep mode menu
    MSNIFFER,   // MUART UART Settings
    MTRIG2,     // MPOSTT Post Trigger
    MAWG2,      // MAWGFREQ Frequency
    MSOURCE,    // MTLEVEL Trigger Level
    MSOURCE,    // MTW1 Trigger Window 1
    MSOURCE,    // MTW2 Trigger Window 2
    MAWG5,      // MSW1 Sweep Start
    MAWG5,      // MSW2 Sweep End
    MMAIN1,     // MHPOS Run/Stop - Horizontal Scroll
    MAWG5,      // MSWSPEED Sweep Speed
    MAWG3,      // MAWGAMP Amplitude
    MAWG3,      // MAWGOFF Offset
    MAWG3,      // MAWGDUTY Duty Cycle
    MTRIG2,     // MTHOLD Trigger holdoff
    MCH1OPT,    // MCH1POS CH1 Channel position
    MCH2OPT,    // MCH2POS CH2 Channel position
    MCURSOR1,   // MVC1 V Cursor 1
    MCURSOR1,   // MVC2 V Cursor 2
    MCURSOR1,   // MCH1HC1 H Cursor 1 CH1
    MCURSOR1,   // MCH1HC2 H Cursor 2 CH1
    MCURSOR1,   // MCH2HC1 H Cursor 1 CH2
    MCURSOR1,   // MCH2HC2 H Cursor 2 CH2
};

const char gaintxt[][5] PROGMEM = {        // Gain Text with x1 probe
    "5.12", "2.56", "1.28", "0.64",             //  5.12,  2.56, 1.28, 0.64
    "0.32", "0.16", { '8', '0', 0x1A, 0x1B, 0 } //  0.32,  0.16,  80m, invalid
};

const char ratetxt[][5] PROGMEM = {
    { ' ', ' ', '8', 0x17, 0 },         // 8u
    { ' ', '1', '6', 0x17, 0 },         // 16u
    { ' ', '3', '2', 0x17, 0 },         // 32u
    { ' ', '6', '4', 0x17, 0 },         // 64u
    { '1', '2', '8', 0x17, 0 },         // 128u
    { '2', '5', '6', 0x17, 0 },         // 256u
    { '5', '0', '0', 0x17, 0 },         // 500u
    { ' ', '1',0x1A, 0x1B, 0 },         // 1m
    { ' ', '2',0x1A, 0x1B, 0 },         // 2m
    { ' ', '5',0x1A, 0x1B, 0 },         // 5m
    { '1', '0',0x1A, 0x1B, 0 },         // 10m
    { '2', '0',0x1A, 0x1B, 0 },         // 20m
    { '5', '0',0x1A, 0x1B, 0 },         // 50m
    " 0.1",                             // 0.1,
    " 0.2", " 0.5", "   1", "   2",     // 0.2, 0.5,    1,    2
    "   5", "  10", "  20", "  50"      //   5,  10,   20,   50
};

const char freqtxt[][5] PROGMEM = {
    "  1M", "500K", "250K", "125K",
    " 62K", " 31K", " 16K", "  8K",
    "  4K", "1.6K", " 800", " 800",
    " 320", " 160", "  80", "  32",
    "  16", "   8", " 3.2", " 1.6",
    " 0.8", "0.32"
};

const char baudtxt[][7] PROGMEM = {        // Baud text
    " 1200 ", " 2400 ", " 4800 ", " 9600 ",
    "19200 ", "38400 ", "57600 ", "115200",
};

const char shortcuts[][5] PROGMEM = {
    {   1,  16,  32,  48,   64 },   // MSWSPEED   1,     16,   32,   48,     64
    {   0, -32, -64, -96, -128 },   // MAWGAMP   0V,     1V,   2V,   3V,     4V
    { -64, -32,   0,  32,   64 },   // MAWGOFF  -1V,  -0.5V,   0V,   0.5V,   1V
    {   1,  64, 128, 192,  255 },   // MAWGDUTY  0.39%, 25%,   50%,  75%, 99.61%
    {   0,  64, 128, 192,  255 },   // MTHOLD    0ms,  64ms, 128ms, 192ms, 255ms
    {  -4, -32, -64, -96, -126 },   // MCH1POS
    {  -4, -32, -64, -96, -126 },   // MCH2POS
};

const uint16_t movetable[] PROGMEM = {
    &M.SWSpeed,
    &M.AWGamp,
    &M.AWGoffset,
    &M.AWGduty,
    &M.Thold,
    &M.CH1pos,
    &M.CH2pos,
};

static inline uint8_t average (uint8_t a, uint8_t b) {
    asm ("add  %0, %1"   "\n\t"
         "ror %0"
         : "+d" (a)
         : "r" (b));
    return a;
}

// Main MSO Application
void MSO(void) {
    uint8_t i,j,ypos;
    uint8_t temp1,temp2,temp3,temp4,temp5,temp6,temp7;
    uint8_t  *p1, *p2, *p3;         // temp pointers to unsigned 8 bits
    int8_t   *q1, *q2, *q3;         // temp pointers to signed 8 bits
    uint16_t Tpost;
    int16_t AWGsweepi;              // AWG sweep counter
    uint8_t AWGspeed;               // AWG sweep increment
    uint16_t circular;              // Index of circular buffer
    const char *text;               // Pointer to constant text
    uint8_t *pmove;                 // pointer for move- move+ menus

    old_s=Srate; old_g1=M.CH1gain; old_g2=M.CH2gain;
    Menu = Mdefault;                // Set default menu
    setbit(MStatus, update);        // Force second layer to update
    setbit(MStatus, updatemso);     // Apply settings
    setbit(MStatus, updateawg);     // Generate wave
    setbit(Misc, redraw);           // Clear logo

    for(;;) {
		if(testbit(MStatus, updatemso)) Apply();

///////////////////////////////////////////////////////////////////////////////
// Check User Input
        if(testbit(Key, userinput)) {
            clrbit(Key, userinput);
            // Check key inputs KA thru KD depending on the menu
            if(testbit(Key,KD)) {
                if(Menu==MMAIN5) SaveEE();       // Save MSO settings
                else if(Menu==MSNIFFER) setbit(MStatus, gosniffer);
                Menu=pgm_read_byte_near(Next+Menu); // Menu flow
            }
            else {
                // Don't clear menu if pressing the menu key
                if(Srate>=11) setbit(Misc, redraw);
            }
            // Shortcuts
            if(Menu>=MSWSPEED && Menu<=MCH2POS && testbit(Key,KA)) {
                pmove = pgm_read_word(movetable+Menu-MSWSPEED);
                *pmove=pgm_read_byte_near((&shortcuts[0][0])+(shortcuti++)+5*(Menu-MSWSPEED));
                if(shortcuti>4) shortcuti=0;
            }
            switch(Menu) {
                case Mdefault:     // default menu
                    cli();   // Disable all interrupts
                    if(testbit(Key,KA)) {    // Next Channel
                        // Trigger is Single -> Enable one more trace
                        if(testbit(Trigger, single)) {
                            Index=0;
                            clrbit(MStatus, stop);
                        }
                        // Trigger is Normal or Free -> Toggle stop / run
                        else {
                            togglebit(MStatus, stop);
                            if(testbit(MStatus, stop)) {
                                clrbit(MStatus, update);    // prevent screen erase
                                clrbit(Misc, redraw);
                                if(Srate<11) Menu=MHPOS;    // Horizontal scroll
                            }
                        }
                    }
                    if(testbit(Key,KB) && testbit(Key,KC)) M.HPos = 64;
                    else {
                        if(testbit(Key,KB)) {    // Sampling rate
                            if(!testbit(MStatus,stop)) {
                                if(Srate<21) Srate++;
                                else Srate=21;
                                i=0; do {
                                    D.CH1data[i]=128;
                                    D.CH2data[i]=128;
                                    D.CHDdata[i]=0;
                                } while (++i);
                                setbit(Misc, redraw);
                                Index=0;
                                TCE0.INTCTRLA = 0;
                                clrbit(MStatus,triggered);
                            }
                        }
                        if(testbit(Key,KC)) {    // Sampling rate
                            if(!testbit(MStatus,stop)) {
                                if(Srate) Srate--;
                                i=0; do {
                                    D.CH1data[i]=128;
                                    D.CH2data[i]=128;
                                    D.CHDdata[i]=0;
                                } while (++i);
                                setbit(Misc, redraw);
                                Index=0;
                                TCE0.INTCTRLA = 0;
                                clrbit(MStatus,triggered);
                            }
                        }
                    }                        
                    sei(); // Enable all interrupts
                break;
                case MCH1:     // Channel 1 menu
                    if(testbit(Key,KA)) {    // Channel 1 on/off
                        togglebit(CH1ctrl,chon);
                        setbit(Misc, redraw);
                    }                        
                    if(testbit(Key,KB)) {    // Less gain
                        if(M.CH1gain) {
                            M.CH1gain--;
                            i=0; do {   // resize
                                D.CH1data[i] = half(D.CH1data[i]);
                            } while (++i);
                        }
                    }                        
                    if(testbit(Key, KC)) {    // More gain
                        if(M.CH1gain<6) {
                            M.CH1gain++;
                            i=0; do {   // resize
                                D.CH1data[i] = twice(D.CH1data[i]);
                            } while (++i);
                        }
                    }
                break;
                case MCH2:                     // Channel 2 menu
                    if(testbit(Key,KA)) {    // Channel 2 on/off
                        togglebit(CH2ctrl,chon);
                        setbit(Misc, redraw);
                    }                    
                    if(testbit(Key,KB)) {    // Less gain
                        if(M.CH2gain) {
                            M.CH2gain--;
                            i=0; do {   // resize
                                D.CH2data[i] = half(D.CH2data[i]);
                            } while (++i);
                        }
                    }                    
                    if(testbit(Key,KC)) {    // More gain
                        if(M.CH2gain<6) {
                            M.CH2gain++;
                            i=0; do {   // resize
                                D.CH2data[i] = twice(D.CH2data[i]);
                            } while (++i);
                        }
                    }                    
                break;
                case MCHD:                     // Logic Analyzer menu
                    if(testbit(Key,KA)) togglebit(CHDctrl,chon);   // Logic on/off
                    if(testbit(Key,KB)) Menu = MCHDSEL1;            // Bit Select
                    if(testbit(Key,KC)) Menu = MPROTOCOL;           // Protocol Sniffer
                break;
                case MSNIFFER:      // Sniffer mode
                    if(testbit(Key,KA)) clrbit(Mcursors, singlesniff);    // Normal buffer
                    if(testbit(Key,KB)) setbit(Mcursors, singlesniff);    // Single buffer
                    if(testbit(Key,KC)) togglebit(Trigger, round);   // Circular buffer
                break;
                case MTRIGTYPE:     // Trigger Type
                    if(testbit(Key,KA)) {    // Trigger Normal
                        if(testbit(Trigger, normal) && !testbit(Trigger, single)) {
                            clrbit(Trigger, normal);
                        } else {
                            clrbit(Trigger,autotrg);
                            clrbit(Trigger,single);
                            setbit(Trigger, normal);
                        }
                        clrbit(MStatus, stop);
                    }                    
                    if(testbit(Key,KB)) {    // Trigger Single
                        clrbit(Trigger, autotrg);
                        setbit(Trigger, normal);
                        setbit(Trigger, single);
                        clrbit(MStatus, stop);
                    }                    
                    if(testbit(Key,KC)) {    // Trigger Auto
                        if(testbit(Trigger, autotrg)) {
                            clrbit(Trigger, autotrg);
                        } else {
                            setbit(Trigger,autotrg);
                            clrbit(Trigger,single);
                            clrbit(Trigger, normal);
                        }
                        clrbit(MStatus, stop);
                    }                    
                break;
                case MCURSOR1:     // Cursor menu
                    if(testbit(Key,KA)) {    // Vertical Cursors
                        if(testbit(Mcursors, cursorv)) {
                            clrbit(Mcursors, cursorv);
                        }
                        else {
                            setbit(Mcursors, cursorv);
                            Menu = MVC1;
                        }
                    }                    
                    if(testbit(Key,KB)) {    // CH1 Horizontal Cursors
                        togglebit(Mcursors, cursorh1);
                        if(testbit(Mcursors, cursorh1)) {
                            clrbit(Mcursors, cursorh2);
                            Menu = MCH1HC1;
                        }
                    }                    
                    if(testbit(Key,KC)) {    // CH2 Horizontal Cursors
                        togglebit(Mcursors, cursorh2);
                        if(testbit(Mcursors, cursorh2)) {
                            clrbit(Mcursors, cursorh1);
                            Menu = MCH2HC1;
                        }
                    }                    
                break;
                case MWINDOW:     // Spectrum Analyzer menu
                    if(testbit(Key,KA)) {    // Use Hamming Window
						if(testbit(MFFT,hamming)) clrbit(MFFT,hamming);
                        else {
                            setbit(MFFT, hamming);
                            clrbit(MFFT, hann);
                            clrbit(MFFT, blackman);
                        }
                    }                    
                    if(testbit(Key,KB)) {    // Use Hann Window
						if(testbit(MFFT,hann)) clrbit(MFFT,hann);
                        else {
                            clrbit(MFFT, hamming);
                            setbit(MFFT, hann);
                            clrbit(MFFT, blackman);
                        }
                    }                    
                    if(testbit(Key,KC)) {    // Use Cosine Window
						if(testbit(MFFT,blackman)) clrbit(MFFT,blackman);
                        else {
                            clrbit(MFFT, hamming);
                            clrbit(MFFT, hann);
                            setbit(MFFT, blackman);
                        }
                    }                    
                break;
                case MSOURCE:     // Trigger Source
                    if(testbit(Key,KA)) {    // Trigger source is CH1
                        M.Tsource = 0;
                        if(testbit(Trigger,window)) Menu=MTW1;
                        else Menu= MTLEVEL;
                    }                    
                    if(testbit(Key,KB)) {    // Trigger source is CH2
                        M.Tsource = 1;
                        if(testbit(Trigger,window)) Menu=MTW1;
                        else Menu= MTLEVEL;
                    }                    
                    if(testbit(Key,KC)) Menu = MTSEL1;  // Trigger source is LOGIC
                break;
                case MDISPLAY1:     // More Display Options
                    if(testbit(Key,KA)) {    // Grid type
                        if(testbit(Display,grid0)) togglebit(Display,grid1);
                        togglebit(Display,grid0);
                    }                    
                    if(testbit(Key,KB)) togglebit(Display, flip);    // Flip Display
                    if(testbit(Key,KC)) togglebit(Display,disp_inv);    // Invert
                break;
                case MMETER:     // Voltmeter menu
                    if(testbit(Key,KA)) {       // VDC
                        setbit(MStatus,vdc);
                        clrbit(MStatus,vp_p);
                    }
                    if(testbit(Key,KB)) {       // VPP
                        clrbit(MStatus,vdc);
                        setbit(MStatus,vp_p);
                    }
                    if(testbit(Key,KC)) {       // FREQUENCY
                        clrbit(MStatus,vdc);
                        clrbit(MStatus,vp_p);
                    }
                break;
                case MAWG:    // AWG Control
                    if(testbit(Key,KA)) M.AWGtype = 1; // Sine
                    if(testbit(Key,KB)) M.AWGtype = 2; // Square
                    if(testbit(Key,KC)) M.AWGtype = 3; // Triangle
                    setbit(MStatus, updateawg);
                break;
                case MCH1OPT:    // CH1 Menu 2
                    if(testbit(Key,KA)) Menu = MCH1POS;  // CH1 Position
                    if(testbit(Key,KB)) {   // Invert Channel
                        togglebit(CH1ctrl,chinvert);
                        setbit(Misc, redraw);
                    }                    
                    if(testbit(Key,KC)) Menu = MCH1MATH;    // Math
                break;
                case MCH2OPT:    // CH2 Menu 2
                    if(testbit(Key,KA)) Menu=MCH2POS;   // CH2 Position
                    if(testbit(Key,KB)) {   // Invert Channel
                        togglebit(CH2ctrl,chinvert);
                        setbit(Misc, redraw);
                    }                    
                    if(testbit(Key,KC)) Menu=MCH2MATH;  // Math
                break;
                case MCHDOPT1:    // Logic Analyzer Options
                    if(testbit(Key,KA)) M.CHDpos+=8;              // Logic Position
                    if(testbit(Key,KB)) togglebit(CHDctrl,chinvert);    // Invert Channel
                    if(testbit(Key,KC)) togglebit(CHDctrl,low);         // Thick line when logic '0'
                break;
                case MMAIN1:     // Menu Select 1: Channel
                    if(testbit(Key,KA)) Menu = MCH1;     // CH1
                    if(testbit(Key,KB)) Menu = MCH2;     // CH2
                    if(testbit(Key,KC)) Menu = MCHD;     // LOGIC
                break;
                case MMAIN2:     // Menu Select 2: Trigger
                    if(testbit(Key,KA)) Menu = MTRIGTYPE;   // Trigger Type
                    if(testbit(Key,KB)) Menu = MSOURCE;     // Trigger Source
                    if(testbit(Key,KC)) Menu = MTRIG2;      // Post Trigger
                break;
                case MMAIN3:     // Menu Select 3: Mode
                    // Check Tactile Switches
                    temp1=MFFT;
        		    if(MFFT<0x20) RestorefromMeter();
                    if(testbit(Key,KA) && testbit(Key,KC)) { // Scope or XY + FFT
                        if(testbit(MFFT,xymode)) {
                            clrbit(MFFT,scopemode);
                            setbit(MFFT,xymode);
                            setbit(MFFT,fftmode);
                        }
                        else {
                            setbit(MFFT,scopemode);
                            clrbit(MFFT,xymode);
                            setbit(MFFT,fftmode);
                        }
                        while(Key);
                    } 
                    else {
                        if(testbit(Key,KA)) {   // Set Scope mode
                            if(testbit(MFFT,fftmode) || !testbit(MFFT,xymode)) {
                                setbit(MFFT,scopemode);
                                clrbit(MFFT,xymode);
                                clrbit(MFFT,fftmode);
                            }
                            Menu=MSCOPEOPT;
                        }
                        if(testbit(Key,KB)) {   // Set Meter mode
                            clrbit(MFFT,scopemode);
                            clrbit(MFFT,xymode);
                            clrbit(MFFT,fftmode);
                            Menu=MMETER;
                        }
                        if(testbit(Key,KC)) {   // Set FFT mode
                            clrbit(MFFT,scopemode);
                            clrbit(MFFT,xymode);
                            setbit(MFFT,fftmode);
                            Menu=MMAIN4;
                        }
                    }
					if(MFFT<0x20) GoingtoMeter();
                    if(temp1!=MFFT) setbit(Misc,redraw);    // Mode changed
                break;
                case MMAIN4:     // Menu Select 4: FFT
                    if(testbit(Key,KA)) togglebit(MFFT, iqfft);      // Set IQ FFT
                    if(testbit(Key,KB)) Menu=MWINDOW;                // FFT Window Menu
                    if(testbit(Key,KC)) togglebit(MFFT, uselog);     // Use logarithmic display
                break;
                case MMAIN5:     // Menu Select 5: Miscellaneous
                    if(testbit(Key,KA)) Menu=MCURSOR1;       // Cursor
                    if(testbit(Key,KB)) Menu=MDISPLAY2;      // Display Menu
                    if(testbit(Key,KC)) Menu=MAWG2;          // AWG Menu
                break;
                case MAWG2:     // AWG Menu 2
                    if(testbit(Key,KA)) Menu = MAWG;         // Waveform Type
                    if(testbit(Key,KB)) Menu = MAWG6;        // Go to Advanced Settings
                    if(testbit(Key,KC)) Menu = MAWGFREQ;     // Frequency
                break;
                case MAWG4:     // AWG Menu 4
                    if(testbit(Key,KA)) M.AWGtype = 4;  // Exponential
                    if(testbit(Key,KB)) M.AWGtype = 0;  // Noise
                    if(testbit(Key,KC)) M.AWGtype = 5;  // Custom
                    setbit(MStatus, updateawg);
                break;
                case MAWG5:     // AWG Menu 5
                    if(testbit(Key,KA)) Menu=MSWSPEED;   // Sweep Menu
                    if(testbit(Key,KB)) Menu=MSWMODE;    // Sweep Mode Menu
                    if(testbit(Key,KC)) Menu=MSW1;       // Range Menu
                break;
                case MAWG6:     // AWG Menu 6
                    if(testbit(Key,KA)) togglebit(Sweep,SweepF);    // Toggle F sweep
                    if(testbit(Key,KB)) togglebit(Sweep,SweepA);    // Toggle A sweep
                    if(testbit(Key,KC)) togglebit(Sweep,SweepD);    // Toggle D sweep
                break;
/*                case MAWG7:     // AWG Menu 7
                    if(testbit(Key,KA)) Menu = MAWG6;   // Go to Sweep menu
                    if(testbit(Key,KB)) Menu = MCVG;    // Go to CV/Gate
                    if(testbit(Key,KC)) togglebit(MStatus,AWGPositive);     // Positive Range
                break;
                case MCVG:     // CV/Gate Menu
                    if(testbit(Key,KA)) Menu = MCVG;  // Go to CV/Gate
                    if(testbit(Key,KB)) Menu = MAWG6;   // Go to Sweep menu
                    togglebit(MStatus,AWGPositive);     // Positive Range*/
                break;
                case MSCOPEOPT:
                    if(testbit(Key,KA)) {   // Roll
                        togglebit(Mcursors,roll);
                        if(testbit(Mcursors,roll)) clrbit(Display,elastic);
                    }                    
                    if(testbit(Key,KB)) {   // Elastic
                        togglebit(Display, elastic);
                        if(testbit(Display,elastic)) clrbit(Mcursors,roll);
                    }                    
                    if(testbit(Key,KC)) {   // Toggle XY Mode
                        if(testbit(MFFT,xymode)) {
                            setbit(MFFT,scopemode);
                            clrbit(MFFT,xymode);
                        }
                        else {
                            clrbit(MFFT,scopemode);
                            setbit(MFFT,xymode);
                        }
                    }                    
                break;
                case MTRIG2: // Trigger edge and mode
                    if(testbit(Key,KA)) Menu = MTRIGMODE;    // Trigger mode
                    if(testbit(Key,KB)) Menu = MPOSTT;       // Post trigger menu
                    if(testbit(Key,KC)) Menu = MTHOLD;       // Trigger holdoff menu
                break;
                case MTRIGMODE: // Trigger mode
                    if(testbit(Key,KA)) {   // Set Window Mode
						if(testbit(Trigger,window)) clrbit(Trigger,window);
						else {
    						clrbit(Trigger, edge);
    						clrbit(Trigger, slope);
    						setbit(Trigger, window);
						}
                    }                    
                    if(testbit(Key,KB)) {   // Set Edge Mode
						if(testbit(Trigger,edge)) clrbit(Trigger,edge);
						else {
    						setbit(Trigger, edge);
    						clrbit(Trigger, slope);
    						clrbit(Trigger, window);
						}
                    }                    
                    if(testbit(Key,KC)) {   // Set Slope Mode
						if(testbit(Trigger,slope)) clrbit(Trigger,slope);
						else {
    						clrbit(Trigger, edge);
    						setbit(Trigger, slope);
    						clrbit(Trigger, window);
						}
                    }                    
                break;
                case MCURSOR2:     // More Cursors Options
                    if(testbit(Key,KA)) togglebit(Mcursors, autocur);   // Auto Cursors
                    if(testbit(Key,KB)) togglebit(Mcursors, track);     // Track Vertical Cursors
                    if(testbit(Key,KC)) {   // Show Reference
                        togglebit(Mcursors, reference);
                        if(testbit(Mcursors, reference)) {
                            // Save waveform to EEPROM
                            tiny_printp(50,4,PSTR("SAVING...")); show_display();
                            i=0;
                            do {     // Apply position
                                temp1=addwsat(D.CH1data[i],M.CH1pos);
                                temp1=temp1>>1; // Scale to LCD (128x64)
                                if(temp1>=64) temp1=63;
                                temp2=addwsat(D.CH2data[i],M.CH2pos);
                                temp2=temp2>>1; // Scale to LCD (128x64)
                                if(temp2>=64) temp2=63;
                                eeprom_write_byte(&EECHREF1[i], temp1);
                                eeprom_write_byte(&EECHREF2[i], temp2);
                            } while(++i);
                        }
                    }
                break;
                case MCHDSEL1:     // Logic bit select
                    M.CHDpos = 0;
                    if(testbit(Key,KA)) {   // Select All / None
                        if(CHDmask==0xFF) CHDmask=0;
                        else CHDmask = 0xFF;
                    }
                    if(testbit(Key,KB)) togglebit(CHDmask, 0);
                    if(testbit(Key,KC)) togglebit(CHDmask, 1);
                break;
                case MCHDSEL2:     // Logic bit select
                    M.CHDpos = 0;
                    if(testbit(Key,KA)) togglebit(CHDmask, 2);
                    if(testbit(Key,KB)) togglebit(CHDmask, 3);
                    if(testbit(Key,KC)) togglebit(CHDmask, 4);
                break;
                case MCHDSEL3:     // Logic bit select
                    M.CHDpos = 0;
                    if(testbit(Key,KA)) togglebit(CHDmask, 5);
                    if(testbit(Key,KB)) togglebit(CHDmask, 6);
                    if(testbit(Key,KC)) togglebit(CHDmask, 7);
                break;
                case MTSEL1:     // Digital Trigger source
                    if(testbit(Key,KA)) M.Tsource = 10;    // External Trigger
                    if(testbit(Key,KB)) M.Tsource = 2;     // 0
                    if(testbit(Key,KC)) M.Tsource = 3;     // 1
                break;
                case MTSEL2:     // Digital Trigger source
                    if(testbit(Key,KA)) M.Tsource = 4;     // 2
                    if(testbit(Key,KB)) M.Tsource = 5;     // 3
                    if(testbit(Key,KC)) M.Tsource = 6;     // 4
                break;
                case MTSEL3:     // Digital Trigger source
                    if(testbit(Key,KA)) M.Tsource = 7;     // 5
                    if(testbit(Key,KB)) M.Tsource = 8;     // 6
                    if(testbit(Key,KC)) M.Tsource = 9;     // 7
                break;
                case MCHDOPT2:                     // Logic Options 2
                    if(testbit(Key,KA)) togglebit(CHDctrl,hexp);     // Parallel
                    if(testbit(Key,KB)) togglebit(CHDctrl,hexs);     // Serial
                    if(testbit(Key,KC)) Menu=MCHDPULL;               // Pull Resistors
                break;
                case MPROTOCOL:                     // Protocol Decoding
                    if(testbit(Key,KA)) {   // I2C
						M.CHDdecode = i2c;
                        Menu=MSNIFFER;
                    }
                    if(testbit(Key,KB)) {   // UART
						M.CHDdecode = rs232;
                        Menu=MUART;
                    }
                    if(testbit(Key,KC)) {   // SPI
						M.CHDdecode = spi;
                        Menu=MSPI;
                    }
                break;
/*                case MPROTOCOL2:                     // Protocol Decoding
                    if(testbit(Key,KA)) {   // CAN
                        M.CHDdecode = can;
                        Menu=MSNIFFER;
                    }
                    if(testbit(Key,KB)) {   // 1WIRE
                        M.CHDdecode = onewire;
                        Menu=MSNIFFER;
                    }
                    if(testbit(Key,KC)) {   // MIDI
                        M.CHDdecode = midi;
                        Menu=MSNIFFER;
                    }
                break;*/
                case MCHDPULL:     // Logic Input Pull
                    if(testbit(Key,KA)) clrbit(CHDctrl,pull);   // No Pull
                    if(testbit(Key,KB)) {   // Pull Up
                        setbit(CHDctrl,pull);
                        setbit(CHDctrl,pullup);
                    }
                    if(testbit(Key,KC)) {   // Pull Down
                        setbit(CHDctrl,pull);
                        clrbit(CHDctrl,pullup);
                    }
                break;
                case MDISPLAY2:     // Display menu
                    if(testbit(Key,KA)) {   // Persistent mode
                        togglebit(Display, persistent);
                        if(!testbit(Mcursors,roll)) Index=0;
                    }
                    if(testbit(Key,KB)) togglebit(Display, line);        // Line mode
                    if(testbit(Key,KC)) togglebit(Display, showset);     // Show channel settings
                break;
                case MSPI:    // SPI Menu
                    if(testbit(Key,KA)) togglebit(Sniffer,CPOL);        // Toggle CPOL
                    if(testbit(Key,KB)) togglebit(Sniffer,CPHA);        // Toggle CPHA
                break;
                case MCH1MATH:    // Channel 1 math
                    if(testbit(Key,KA)) {   // Subtract
                        if(testbit(CH1ctrl,chmath)) {
                            togglebit(CH1ctrl,submult);
                            if(!testbit(CH1ctrl,submult)) clrbit(CH1ctrl,chmath);
                        }
                        else {
                            setbit(CH1ctrl,chmath);
                            setbit(CH1ctrl,submult);
                        }
                    }
                    if(testbit(Key,KB)) {   // Multiply
                        if(testbit(CH1ctrl,chmath)) {
                            togglebit(CH1ctrl,submult);
                            if(testbit(CH1ctrl,submult)) clrbit(CH1ctrl,chmath);
                        }
                        else {
                            setbit(CH1ctrl,chmath);
                            clrbit(CH1ctrl,submult);
                        }
                    }
                    if(testbit(Key,KC)) togglebit(CH1ctrl,chaverage);   // Average
                break;
                case MCH2MATH:    // Channel 2 math
                    if(testbit(Key,KA)) {   // Subtract
                        if(testbit(CH2ctrl,chmath)) {
                            togglebit(CH2ctrl,submult);
                            if(!testbit(CH2ctrl,submult)) clrbit(CH2ctrl,chmath);
                        }
                        else {
                            setbit(CH2ctrl,chmath);
                            setbit(CH2ctrl,submult);
                        }
                    }
                    if(testbit(Key,KB)) {   // Multiply
                        if(testbit(CH2ctrl,chmath)) {
                            togglebit(CH2ctrl,submult);
                            if(testbit(CH2ctrl,submult)) clrbit(CH2ctrl,chmath);
                        }
                        else {
                            setbit(CH2ctrl,chmath);
                            clrbit(CH2ctrl,submult);
                        }
                    }
                    if(testbit(Key,KC)) togglebit(CH2ctrl,chaverage);   // Average
                break;
                case MAWG3:     // AWG Menu 3
                    if(testbit(Key,KA)) Menu=MAWGAMP;    // Amplitude
                    if(testbit(Key,KB)) Menu=MAWGDUTY;   // Duty Cycle
                    if(testbit(Key,KC)) Menu=MAWGOFF;    // Offset
                break;
                case MSWMODE:
                    if(testbit(Key,KA)) togglebit(Sweep,swdown);    // Sweep direction
                    if(testbit(Key,KB)) togglebit(Sweep,pingpong);   // Ping Pong
                    if(testbit(Key,KC)) {   // Accelerate up or accelerate down
                        if(testbit(Sweep,SWAccel) && testbit(Sweep, SWAcceldir)) {
                            clrbit(Sweep,SWAccel);
                            clrbit(Sweep, SWAcceldir);
                        }
                        else if(testbit(Sweep,SWAccel)) setbit(Sweep,SWAcceldir);
                        else setbit(Sweep,SWAccel);
                    }
                break;
                case MUART:    // Baud Rate Menu 1
                    if(testbit(Key,KA)) {   // Change Baud Rate
                        temp1=Sniffer&0x07;
                        temp1++;
                        temp1=temp1&0x07;
                        Sniffer&=0xF8;
                        Sniffer|=temp1;
                    }
                    if(testbit(Key,KB)) {   // Parity
                        if(!testbit(Sniffer,parmode)) {
                            setbit(Sniffer,parmode);
                            clrbit(Sniffer,parity);
                        }
                        else {
                            if(!testbit(Sniffer,parity)) setbit(Sniffer,parity);
                            else clrbit(Sniffer,parmode);
                        }
                    }
                    if(testbit(Key,KC)) togglebit(Sniffer,stopbit);    // Stop Bits
                break;
                case MPOSTT:
                    Tpost = M.Tpost;
                    if(testbit(Key,KA)) {   // Shortcut values
                        if(Tpost == 128) Tpost = 256;
                        else if(Tpost == 256) Tpost = 0;
                        else Tpost = 128;
                    }
                    if(testbit(Key,KB)) { if(Tpost) Tpost--; }
                    if(testbit(Key,KC)) Tpost++;
                    M.Tpost=Tpost;
                    CheckPost();
                break;
                case MAWGFREQ:     // Frequency
                    if(testbit(Key,KA)) moveF();    // Shortcuts: 10KHz, 1KHz, 100Hz, 10Hz, 1Hz
                    if(testbit(Key,KB)) {
                        setbit(Misc, fullprint);    // decrease
                        moveF();
                    }                            
                    if(testbit(Key,KC)) {
                        setbit(Misc, bigfont);      // increase
                        moveF();
                    }
                    clrbit(Misc, fullprint);
                    clrbit(Misc, bigfont);                        
                    setbit(MStatus, updateawg);
                break;
                case MTLEVEL:     // Trigger Level
                    if(testbit(Key,KA)) {   // 0V or average
                        if(M.Tlevel==128) {
                            if(M.Tsource==0) {
                                M.Tlevel = CH1.min + (CH1.vpp/2);
                                if(testbit(CH1ctrl,chinvert)) M.Tlevel=255-M.Tlevel;
                            }
                            else if(M.Tsource==1) {
                                M.Tlevel = CH2.min + (CH2.vpp/2);
                                if(testbit(CH2ctrl,chinvert)) M.Tlevel=255-M.Tlevel;
                            }
                        }
                        else M.Tlevel=128;
                    }
                    if(testbit(Key,KB)) {   // decrease
                        if(M.Tlevel<255) M.Tlevel++;
                        setbit(Trigger,trigdir);
                    }
                    if(testbit(Key,KC)) {  // increase
                        if(M.Tlevel) M.Tlevel--;
                        clrbit(Trigger,trigdir);
                    }
                break;
                case MTW1:     // Window Level 1
                    if(testbit(Key,KA)) Menu=MTW2;    // Select Cursor
                    if(testbit(Key,KB)) { if(M.Window1<254) M.Window1+=2; }
                    if(testbit(Key,KC)) {
                        if(M.Window1) M.Window1--;
                        if(M.Window1) M.Window1--;
                    }                    
                break;
                case MTW2:     // Window Level 1
                    if(testbit(Key,KA)) Menu=MTW1;    // Select Cursor
                    if(testbit(Key,KB)) { if(M.Window2<254) M.Window2+=2; }
                    if(testbit(Key,KC)) {
                        if(M.Window2) M.Window2--;
                        if(M.Window2) M.Window2--;
                    }
                break;
                case MSW1:     // Sweep start frequency
                    if(testbit(Key,KA)) Menu=MSW2;    // Select Cursor
                    if(testbit(Key,KB)) { if(M.Sweep1)     M.Sweep1--; }
                    if(testbit(Key,KC)) { if(M.Sweep1<255) M.Sweep1++; }
                break;
                case MSW2:     // Sweep end frequency
                    if(testbit(Key,KA)) Menu=MSW1;    // Select Cursor
                    if(testbit(Key,KB)) { if(M.Sweep2)     M.Sweep2--; }
                    if(testbit(Key,KC)) { if(M.Sweep2<255) M.Sweep2++; }
                break;
                case MHPOS:     // Stop - Horizontal Scroll
                    if(testbit(Key,KB) && testbit(Key,KC)) M.HPos = 64;   // KB and KC pressed simultaneously
                    else {
                        if(testbit(Key,KA)) {   // Start acquisition
                            clrbit(MStatus, stop);
                            Menu=Mdefault;
                        }
                        if(testbit(Key,KB)) { if(M.HPos) M.HPos--; }
                        if(testbit(Key,KC)) M.HPos++;
                    }                    
                break;
                case MSWSPEED:  // Sweep speed
                    if(testbit(Key,KB)) { if(M.SWSpeed>1)    M.SWSpeed--; }
                    if(testbit(Key,KC)) { if(M.SWSpeed<127)  M.SWSpeed++; }
                    AWGspeed=M.SWSpeed;
                break;
                case MAWGAMP:     // Amplitude
                    if(testbit(Key,KB)) M.AWGamp++;   // Decrease
                    if(testbit(Key,KC)) { if(M.AWGamp>-128) M.AWGamp--; }  // Increase
                    setbit(MStatus, updateawg);
                break;
                case MAWGOFF:     // Offset
                    if(testbit(Key,KB)) { if(M.AWGoffset<127) M.AWGoffset++;  }   // decrease
                    if(testbit(Key,KC)) { if(M.AWGoffset>-128) M.AWGoffset--; }   // increase
                    setbit(MStatus, updateawg);
                break;
                case MAWGDUTY:     // Duty Cycle
                    if(testbit(Key,KB)) { if(M.AWGduty>0)   M.AWGduty--; }    // decrease
                    if(testbit(Key,KC)) { if(M.AWGduty<255) M.AWGduty++; }    // increase
                    setbit(MStatus, updateawg);
                break;
                case MTHOLD:
                    if(testbit(Key,KB)) { if(M.Thold)     M.Thold--; }
                    if(testbit(Key,KC)) { if(M.Thold<255) M.Thold++; }
                break;
                case MCH1POS:     // CH1 Position
                    setbit(Misc, redraw);
                    if(testbit(Key,KB)) M.CH1pos+=2;
                    if(testbit(Key,KC)) {
                        if(M.CH1pos>-128) M.CH1pos--;
                        if(M.CH1pos>-128) M.CH1pos--;
                    }
                break;
                case MCH2POS:     // CH2 Position
                    setbit(Misc, redraw);
                    if(testbit(Key,KB)) M.CH2pos+=2;
                    if(testbit(Key,KC)) {
                        if(M.CH2pos>-128) M.CH2pos--;
                        if(M.CH2pos>-128) M.CH2pos--;
                    }
                break;
                case MVC1:     // V Cursor 1
                    if(testbit(Key,KA)) Menu=MVC2;   // Select Cursor
                    if(testbit(Key,KB)) {
                        clrbit(Mcursors, autocur);
                        if(M.VcursorA) M.VcursorA--;
                    }
                    if(testbit(Key,KC)) {
                        clrbit(Mcursors, autocur);
                        M.VcursorA++;
                    }
                break;
                case MVC2:     // V Cursor 2
                    if(testbit(Key,KA)) Menu=MVC1;    // Select Cursor
                    if(testbit(Key,KB)) {
                        clrbit(Mcursors, autocur);
                        if(M.VcursorB) M.VcursorB--;
                    }
                    if(testbit(Key,KC)) {
                        clrbit(Mcursors, autocur);
                        M.VcursorB++;
                    }
                break;
                case MCH1HC1:     // H Cursor 1
                    if(testbit(Key,KA)) Menu=MCH1HC2;   // Select Cursor
                    if(testbit(Key,KB)) M.Hcursor1A+=2; 
                    if(testbit(Key,KC)) {
                        if(M.Hcursor1A) M.Hcursor1A--;
                        if(M.Hcursor1A) M.Hcursor1A--;
                    }
                break;
                case MCH1HC2:     // H Cursor 2
                    if(testbit(Key,KA)) Menu=MCH1HC1;    // Select Cursor
                    if(testbit(Key,KB)) M.Hcursor1B+=2; 
                    if(testbit(Key,KC)) {
                        if(M.Hcursor1B) M.Hcursor1B--;
                        if(M.Hcursor1B) M.Hcursor1B--;
                    }
                break;
                case MCH2HC1:     // H Cursor 1
                    if(testbit(Key,KA)) Menu=MCH2HC2;   // Select Cursor
                    if(testbit(Key,KB)) M.Hcursor2A+=2; 
                    if(testbit(Key,KC)) {
                        if(M.Hcursor2A) M.Hcursor2A--;
                        if(M.Hcursor2A) M.Hcursor2A--;
                    }
                break;
                case MCH2HC2:     // H Cursor 2
                    if(testbit(Key,KA)) Menu=MCH2HC1;    // Select Cursor
                    if(testbit(Key,KB)) M.Hcursor2B+=2; 
                    if(testbit(Key,KC)) {
                        if(M.Hcursor2B) M.Hcursor2B--;
                        if(M.Hcursor2B) M.Hcursor2B--;
                    }
                break;
            }  // end switch(menu)
            if(Menu==Mdefault && testbit(MStatus, stop))
                if(Srate<11) Menu=MHPOS;  // Horizontal Scroll
            CheckMax(); // Check variables
        }
///////////////////////////////////////////////////////////////////////////////
// Display info (Menu, Grid, cursors, reference, settings...)
        if(testbit(MStatus, update)) {
            clrbit(MStatus, update);
            Apply();        // Apply new oscilloscope settings
            if(testbit(Misc, redraw) || testbit(Display, persistent)) {
                clrbit(Misc, redraw);
                clr_display();
            }
            else {  // Only clear menu area
                for(uint16_t i=896; i<1024; i++) display_buffer[i]=0;
            }
        }
        // Print menu, also determine which items are selected and
        // print them on a black background (inverted print)
        if(Menu) {
            if(Menu<MUART) {
                lcd_goto(0,7);  // Menu position
                q1=menustxt[pgm_read_byte_near(menupoint+Menu-1)];
                for(i=0; i<3; i++) {    // 3 Items in the menu
                    clrbit(Misc,negative);    // negative font
                    switch(Menu) {
                        case MCH1:
                            if(i==0 && testbit(CH1ctrl,chon)) setbit(Misc,negative);
                        break;
                        case MCH2:
                            if(i==0 && testbit(CH2ctrl,chon)) setbit(Misc,negative);
                        break;
                        case MCHD:
                            if(i==0 && testbit(CHDctrl,chon)) setbit(Misc,negative);
                        break;
                        case MSNIFFER:      // Sniffer mode
                            if( (i==0 && !testbit(Mcursors, singlesniff)) ||
                                (i==1 && testbit(Mcursors, singlesniff)) ||
                                (i==2 && testbit(Trigger, round)) ) setbit(Misc,negative);
                        break;
                        case MTRIGTYPE:
                            if( (i==0 && testbit(Trigger, normal) && !testbit(Trigger, single)) ||
                                (i==1 && testbit(Trigger, single)) ||
                                (i==2 && testbit(Trigger, autotrg)) ) setbit(Misc,negative);
                        break;
                        case MCURSOR1:
                            if( (i==0 && testbit(Mcursors, cursorv)) ||
                                (i==1 && testbit(Mcursors, cursorh1)) ||
                                (i==2 && testbit(Mcursors, cursorh2)) ) setbit(Misc,negative);
                        break;
                        case MWINDOW:
                            if( (i==0 && testbit(MFFT, hamming)) ||
                                (i==1 && testbit(MFFT, hann)) ||
                                (i==2 && testbit(MFFT, blackman)) ) setbit(Misc,negative);
                        break;
                        case MSOURCE:
                            if( (i==0 && M.Tsource==0) ||
                                (i==1 && M.Tsource==1) ||
                                (i==2 && M.Tsource>=2) ) setbit(Misc,negative);
                        break;
                        case MDISPLAY1:
                            if( (i==0 && (Display&0x03)) ||
                                (i==1 && testbit(Display, flip)) ||
                                (i==2 && testbit(Display, disp_inv)) ) setbit(Misc,negative);
                        break;
                        case MMETER:
                            if( (i==0 && testbit(MStatus, vdc)) ||
                                (i==1 && testbit(MStatus, vp_p)) ||
                                (i==2 && (!testbit(MStatus, vdc) && !testbit(MStatus, vp_p)) ) ) setbit(Misc,negative);
                        break;
                        case MAWG:
                            if( (i==0 && M.AWGtype==1) ||
                                (i==1 && M.AWGtype==2) ||
                                (i==2 && M.AWGtype==3) ) setbit(Misc,negative);
                        break;
                        case MCH1OPT:
                            if( (i==1 && testbit(CH1ctrl,chinvert)) ||
                                (i==2 && testbit(CH1ctrl,chmath)) ) setbit(Misc,negative);
                        break;
                        case MCH2OPT:
                            if( (i==1 && testbit(CH2ctrl,chinvert)) ||
                                (i==2 && testbit(CH2ctrl,chmath)) ) setbit(Misc,negative);
                        break;
                        case MCHDOPT1:
                            if( (i==1 && testbit(CHDctrl,chinvert)) ||
                                (i==2 && testbit(CHDctrl,low)) ) setbit(Misc,negative);
                        break;
                        case MMAIN3:
                            if( (i==0 && (testbit(MFFT, scopemode) || testbit(MFFT,xymode))) ||
                                (i==1 && (MFFT<0x20)) ||
                                (i==2 && testbit(MFFT, fftmode)) ) setbit(Misc,negative);
                        break;
                        case MMAIN4:
                            if( (i==0 && testbit(MFFT, iqfft)) ||
                                (i==2 && testbit(MFFT, uselog)) ) setbit(Misc,negative);
                        break;
                        case MAWG4:
                            if( (i==0 && M.AWGtype==4) ||
                                (i==1 && M.AWGtype==0) ||
                                (i==2 && M.AWGtype==5) ) setbit(Misc,negative);
                        break;
                        case MAWG6:
                            if( (i==0 && testbit(Sweep,SweepF)) ||
                                (i==1 && testbit(Sweep,SweepA)) ||
                                (i==2 && testbit(Sweep,SweepD)) ) setbit(Misc,negative);
                        break;
/*                        case MAWG7:
                            if( (i==2 && testbit(MStatus,AWGPositive)) ) setbit(Misc,negative);
                        break;*/
                        case MSCOPEOPT:
                            if( (i==0 && testbit(Mcursors, roll)) ||
                            (i==1 && testbit(Display, elastic)) ||
                            (i==2 && testbit(MFFT, xymode)) ) setbit(Misc,negative);
                        break;
                        case MTRIGMODE:
                            if( (i==0 && testbit(Trigger, window)) ||
                                (i==1 && testbit(Trigger, edge)) ||
                                (i==2 && testbit(Trigger, slope)) ) setbit(Misc,negative);
                        break;
                        case MCURSOR2:
                            if( (i==0 && testbit(Mcursors, autocur)) ||
							    (i==1 && testbit(Mcursors, track)) ||
                                (i==2 && testbit(Mcursors, reference)) ) setbit(Misc,negative);
                        break;
                        case MCHDSEL1:
                            if( (i==0 && CHDmask==0xFF) ||
                                (i==1 && testbit(CHDmask, 0)) ||
                                (i==2 && testbit(CHDmask, 1)) ) setbit(Misc,negative);
                        break;
                        case MCHDSEL2:
                            if( (i==0 && testbit(CHDmask, 2)) ||
                                (i==1 && testbit(CHDmask, 3)) ||
                                (i==2 && testbit(CHDmask, 4)) ) setbit(Misc,negative);
                        break;
                        case MCHDSEL3:
                            if( (i==0 && testbit(CHDmask, 5)) ||
                                (i==1 && testbit(CHDmask, 6)) ||
                                (i==2 && testbit(CHDmask, 7)) ) setbit(Misc,negative);
                        break;
                        case MTSEL1:
                            if( (i==0 && M.Tsource==10) ||
                                (i==1 && M.Tsource==2) ||
                                (i==2 && M.Tsource==3) ) setbit(Misc,negative);
                        break;
                        case MTSEL2:
                            if( (i==0 && M.Tsource==4) ||
                                (i==1 && M.Tsource==5) ||
                                (i==2 && M.Tsource==6) ) setbit(Misc,negative);
                        break;
                        case MTSEL3:
                            if( (i==0 && M.Tsource==7) ||
                                (i==1 && M.Tsource==8) ||
                                (i==2 && M.Tsource==9) ) setbit(Misc,negative);
                        break;
                        case MCHDOPT2:
                            if( (i==0 && testbit(CHDctrl,hexp)) ||
                                (i==1 && testbit(CHDctrl,hexs)) ) setbit(Misc,negative);
                        break;
                        case MCHDPULL:
                            if( (i==0 && !testbit(CHDctrl,pull)) ||
                                (i==1 && testbit(CHDctrl,pull) && testbit(CHDctrl,pullup)) ||
                                (i==2 && testbit(CHDctrl,pull) && !testbit(CHDctrl,pullup)) ) setbit(Misc,negative);
                        break;
                        case MDISPLAY2:
                            if( (i==0 && testbit(Display, persistent)) ||
                                (i==1 && testbit(Display, line)) ||
                                (i==2 && testbit(Display, showset)) ) setbit(Misc,negative);
                        break;
                        case MSPI:
                            if( (i==0 && testbit(Sniffer,CPOL)) ||
                                (i==1 && testbit(Sniffer,CPHA)) ) setbit(Misc,negative);
                        break;
                        case MCH1MATH:
                            if( (i==0 && testbit(CH1ctrl,chmath) && testbit(CH1ctrl,submult)) ||
                                (i==1 && testbit(CH1ctrl,chmath) && !testbit(CH1ctrl,submult)) ||
                                (i==2 && testbit(CH1ctrl,chaverage)) ) setbit(Misc,negative);
                        break;
                        case MCH2MATH:
                            if( (i==0 && testbit(CH2ctrl,chmath) && testbit(CH2ctrl,submult)) ||
                                (i==1 && testbit(CH2ctrl,chmath) && !testbit(CH2ctrl,submult)) ||
                                (i==2 && testbit(CH2ctrl,chaverage)) ) setbit(Misc,negative);
                        break;
                        case MSWMODE:
                            if( (i==0 && testbit(Sweep,swdown)) ||
                                (i==1 && testbit(Sweep,pingpong)) ||
                                (i==2 && testbit(Sweep,SWAccel)) ) setbit(Misc,negative);
                        break;
                    }
                    // Print text
                    while ((temp1=pgm_read_byte(q1++)) != 0x00) {
                        GLCD_Putchar(temp1);
                    }
                    clrbit(Misc,negative);
                }
            }
			if(Menu>=MPOSTT) {
				if(Menu>=MOUTSIDE) Menu=Mdefault; // Menu outside range
				else {
				    tiny_printp(56,7,PSTR("MOVE-        MOVE+"));
				    if(Menu>=MCH1POS) {
                        lcd_goto(0,7);
                        if(Menu<=MCH2POS) lcd_putsp(menustxt[10]);  // "Position"
                        else {
                            if(Menu<=MVC2) GLCD_Putchar('V'); else GLCD_Putchar('H');
                            lcd_putsp(menustxt[4]+2);  // "Cursor"
                            if(testbit(Menu,0)) GLCD_Putchar('1'); else GLCD_Putchar('2');
                        }
                    }
				}
			}
        }            
        if(testbit(Misc, stop)) tiny_printp(0,7, PSTR("STOP"));
        if(MFFT>=0x20) { // Not Meter mode
            // Grid
            if(!testbit(MFFT, fftmode)) {
                temp1 = (M.CH1pos+128)/2;
                if(temp1>=64) temp1=63;
                temp2 = (M.CH2pos+128)/2;
                if(temp2>=64) temp2=63;
                if(testbit(MFFT, xymode)) {   //XY Mode
                    temp1 = temp2 = 32;
                }
                switch(Display&0x03) {
                    case 1:
                        set_pixel(64,16);       // Vertical dot
                        set_pixel(64,32);       // Vertical dot
                        set_pixel(64,48);       // Vertical dot
                    case 2:
                        for(i=16; i<=112; i+=16) {
                            if(testbit(CH1ctrl,chon)) set_pixel(i,temp1);           // CH1 ground
                            if(testbit(CH2ctrl,chon) && Srate) set_pixel(i,temp2);  // CH2 ground
                        }
                    break;
                    case 3: // Graticule
                        for(j=16; j<=56; j+=16) {
                            for(i=16; i<=112; i+=16) set_pixel(i,j);
                        }
                    break;
                }
            }
            if(!testbit(MFFT,xymode)) { // Cursors
                if((testbit(Mcursors, cursorh1) && testbit(CH1ctrl,chon)) ||
				   (testbit(Mcursors, cursorh2) && testbit(CH2ctrl,chon))) ShowCursorH();
                if(testbit(Mcursors, cursorv)) ShowCursorV();
            }
            // Display time and gain settings
            ypos=0;
            if(testbit(Display, showset)) {
                if(testbit(CH1ctrl,chon)) {
                    if(testbit(CH1ctrl,chmath)) {
                        lcd_goto(88,0);
                        if(testbit(CH1ctrl,chinvert)) GLCD_Putchar('-'); else GLCD_Putchar(' ');
                        lcd_putsp(menustxt[12]+1);  // CH1 text
                        if(testbit(CH1ctrl,submult)) {
                            if(testbit(CH2ctrl,chinvert)) GLCD_Putchar('+'); else GLCD_Putchar('-');
                        }
                        else {
                            GLCD_Putchar('X');
                            if(testbit(CH2ctrl,chinvert)) { GLCD_Putchar('-'); u8CursorX-=4; }
                        }
                        lcd_putsp(menustxt[12]+15); // CH2 text
                    }
                    else {
                        lcd_goto(76,0);
                        if(testbit(CH1ctrl,chinvert)) GLCD_Putchar('-'); else GLCD_Putchar(' ');
                        lcd_putsp(menustxt[12]+1);  // CH1 text
                        lcd_putsp(gaintxt[M.CH1gain]);
                        lcd_putsp(Vdiv);    // V/div
                    }
                    ypos++;
                }
                if(testbit(CH2ctrl,chon) && Srate) {
                    if(testbit(CH2ctrl,chmath)) {
                        lcd_goto(88,ypos);
                        if(testbit(CH2ctrl,chinvert)) GLCD_Putchar('-'); else GLCD_Putchar(' ');
                        lcd_putsp(menustxt[12]+16); // CH2 text
                        if(testbit(CH2ctrl,submult)) {
                            if(testbit(CH1ctrl,chinvert)) GLCD_Putchar('+'); else GLCD_Putchar('-');
                        }
                        else {
                            GLCD_Putchar('X');
                            if(testbit(CH2ctrl,chinvert)) { GLCD_Putchar('-'); u8CursorX-=4; }
                        }
                        lcd_putsp(menustxt[12]);  // CH1 text
                    }
                    else {
                        lcd_goto(76,ypos);
                        if(testbit(CH2ctrl,chinvert)) GLCD_Putchar('-'); else GLCD_Putchar(' ');
                        lcd_putsp(menustxt[12]+16); // CH2 text
                        lcd_putsp(gaintxt[M.CH2gain]);
                        lcd_putsp(Vdiv);    // V/div                        
                    }
                    ypos++;
                }
                if(testbit(MFFT,fftmode)) {
                    tiny_printp(89,ypos,freqtxt[Srate]);    // Display Nyquist Frequency
                    lcd_putsp(PSTR("HZ MAX"));
                }
                else {
                    tiny_printp(96,ypos,ratetxt[Srate]);    // Display time base
                    lcd_putsp(Sdiv);    // S/div
                }
                ypos++;
            }
        }
///////////////////////////////////////////////////////////////////////////////
// Finished using DMA, now use a DMA to transfer data to the display
        show_display();
		if(testbit(MStatus, updateawg)) BuildWave();
        // Wait for display transfer
        WaitDisplay();
///////////////////////////////////////////////////////////////////////////////
// Limit LCD refresh rate
        if(MFFT>=0x20 || (adjusting==0)) { // Not meter mode, Auto setup has not yet locked on settings
            if(!testbit(TCD1.INTFLAGS, TC1_OVFIF_bp)) {         //  If not overflowed, go to idle mode
                TCD1.INTCTRLA = 0x01;                           // Enable interrupt to wakeup
                SLEEP.CTRL = SLEEP_SMODE_IDLE_gc | SLEEP_SEN_bm;    //
                asm("sleep");
                TCD1.INTCTRLA = 0;                              // Disable interrupt to wakeup
            }
            SLEEP.CTRL = 0x00;
            setbit(TCD1.INTFLAGS, TC1_OVFIF_bp);                // Clear flag
        }
    }
}

void AutoSet(void) {
}

// Exit Meter mode, restore settings
void RestorefromMeter(void) {

}

// Set Meter mode, save settings
void GoingtoMeter(void) {

}

uint8_t fft_stuff(uint8_t *p1) {

}

// Automatically set vertical cursors
void AutoCursorV(void) {
    uint8_t i, mid, *p, samples;
    if(Srate>=11) { // Slow sampling rates (below 50mS/div) use 2 samples per vertical line
        samples = 255;
    }
    else samples = 127;
    if(testbit(MFFT, fftmode)) {
        M.VcursorA = CH1.f;
        M.VcursorB = CH2.f;
        return;
    }
    else {  // Scan data
        // Start with cursors at the edges
        M.VcursorA=0;
        M.VcursorB=samples;
        // Decide which channel to use for vertical cursors
        if((testbit(CH1ctrl,chon) && !testbit(CH2ctrl,chon)) ||               // CH2 off, use CH1
        (testbit(CH1ctrl,chon) && (CH1.vpp > CH2.vpp)) ) { // CH1 has more amplitude
            p = D.CH1data+M.HPos;
            mid = CH1.min + CH1.vpp/2;
            if(CH1.vpp<8) goto end_scan;    // Signal too small
        }
        else {                              // Use CH2
            p = D.CH2data+M.HPos;
            mid = CH2.min + CH2.vpp/2;
            if(CH2.vpp<8) goto end_scan;    // Signal too small
        }
        i=0;
        if(p[0]<mid) {
            // Find first cross
            while(*p++<mid) { i++; if(i==samples) goto end_scan; }
            M.VcursorA=i;
            // Half cycle
            while(*p++>mid) { i++; if(i==samples) goto end_scan; }
            M.VcursorB = ++i;
            // Full cycle
            while(*p++<mid) { i++; if(i==samples) goto end_scan; }
            M.VcursorB=++i;
        }
        else {
            // Find first cross
            while(*p++>mid) { i++; if(i==samples) goto end_scan; }
            M.VcursorA=i;
            // Half cycle
            while(*p++<mid) { i++; if(i==samples) goto end_scan; }
            M.VcursorB = ++i;
            // Full cycle
            while(*p++>mid) { i++; if(i==samples) goto end_scan; }
            M.VcursorB=++i;
        }
    }
end_scan:
    if(Srate>=11) { // Slow sampling rates (below 50mS/div) use 2 samples per vertical line
        M.VcursorA=M.VcursorA>>1;
        M.VcursorB=M.VcursorB>>1;
    }
}

// Measurements for Meter Mode
static inline void Measurements(void) {

}

// Display Vertical Cursor
static inline void ShowCursorV(void) {

}

// Display Horizontal Cursor
static inline void ShowCursorH(void) {
    uint8_t i,y,gain,HcursorA,HcursorB,*data;
	int8_t CHPos;
	ACHANNEL *CH;
    int8_t adjustedA,adjustedB;
    char *Hcursorunit;                  // Horizontal cursor units
    // Display Horizontal Cursor, cursor disabled during FFT mode
    if(!testbit(MFFT, fftmode)) {
	    if(testbit(Mcursors,cursorh1)) {
    		gain=M.CH1gain;
		    HcursorA=M.Hcursor1A;
		    HcursorB=M.Hcursor1B;
		    CHPos=M.CH1pos;
		    CH=&CH1; data=D.CH1data;
	    }
	    else {
    		gain=M.CH2gain;
		    HcursorA=M.Hcursor2A;
		    HcursorB=M.Hcursor2B;
		    CHPos=M.CH2pos;
		    CH=&CH2; data=D.CH2data;
	    }
		if(testbit(Mcursors,track)) {
   			HcursorA=addwsat(data[M.VcursorA+M.HPos],CHPos);
    		HcursorB=addwsat(data[M.VcursorB+M.HPos],CHPos);
		}
		else if(testbit(Mcursors,autocur)) {
            // Apply position
            HcursorA=addwsat(CH->max,CHPos);
            HcursorB=addwsat(CH->min,CHPos);
		}
        if(HcursorA>=128) HcursorA=127;
        if(HcursorB>=128) HcursorB=127;
        HcursorA=HcursorA>>1;
        HcursorB=HcursorB>>1;
        if(gain>=4) Hcursorunit = unitmV;
        else Hcursorunit = unitV;
        for(i=1; i<=126; i+=4) {
            set_pixel(i,HcursorA);      // Line for X1
            set_pixel(i+2,HcursorB);    // Line for X2
        }
        // Apply position
        adjustedA=32-HcursorA+(CHPos+64)/2;
        adjustedB=32-HcursorB+(CHPos+64)/2;
        // Decide where to print text
        if(HcursorA<HcursorB) y=0;
        else y=6;
        // Display values
        lcd_goto(8,y); printV(adjustedA*256, gain);
        lcd_putsp(Hcursorunit);
        if(HcursorA<HcursorB) y=6;
        else y=0;
        lcd_goto(8,y); printV(adjustedB*256, gain);
        lcd_putsp(Hcursorunit);
        tiny_printp(76,6, delta_V);  // delta V =
        lcd_goto(88,6); printV(((int16_t)HcursorA-HcursorB)*256, gain);
        lcd_putsp(Hcursorunit);
    }
}

// Check variables
extern const NVMVAR MAXM;
void CheckMax(void) {

}

void CheckPost(void) {
}

// Apply oscilloscope settings
void Apply(void) {
    PMIC.CTRL = 0x04;   // Only high level interrupts
    TCE0.CTRLA = 0;		// TCE0 controls Interrupt ADC, srate: 6, 7, 8, 9, 10 and fixed value for slow sampling
    TCE0.INTCTRLA = 0;
    // Validate variables
    CheckMax();
    CheckPost();
    // Count CHD enabled pins
    uint8_t temp1,temp2;
    temp1=0;
    temp2=CHDmask;
    do {
        temp1 += temp2 & 1;
    } while (temp2 >>= 1);
    temp1 = (8-temp1)*8;    // Max position
    if(M.CHDpos>temp1)  M.CHDpos= 0;

	if(Srate==0) {  // xymode and iqfft need both channels
		if(testbit(MFFT,xymode) || (testbit(MFFT,fftmode) && testbit(MFFT,iqfft))) Srate++;
	}
    ADCA.CTRLB = 0x14;          // signed mode, no free run, 8 bit
    ADCA.EVCTRL = 0x45;         // Sweep channels 0,1, Event CH0 (TCE0), Sync Sweep
    ADCA.PRESCALER = 0x03;      // Prescaler: 32
    // Set ADC Clock
    if(Srate>=11) { // Slow Sample rates >= 20mS/div
        TCD1.CTRLA = 1;     // Refresh rate per pixel 1024Hz.
        TCE0.CTRLA = 0x03;  // DIV4
        if(Srate==11) TCE0.PER = 4999;      // 1600 Hz
        else TCE0.PER = 6249;               // 1280 Hz
        slowval = (uint16_t)pgm_read_word_near(slowcnt-11+Srate);
    }
    else {  // Fast sampling
        Index = 0;
        // TCD1 controls LCD refresh rate
        if(MFFT<0x20) TCD1.CTRLA = 6;   // Meter mode: DIV 256, LCD refresh rate 4Hz
        else TCD1.CTRLA = 4;                // DIV   8, LCD refresh rate 128Hz
        if(Srate>=6) {
            TCE0.PER = (uint16_t)pgm_read_word_near(TCE0val+Srate-6); // ADC clock
        }
        else {  // sampling rate 256uS/div and under, use DMA
            ADCA.CTRLB  = 0x1C;     // signed mode, free run, 8 bit
            if(Srate>1) {
                ADCA.PRESCALER = Srate; // Prescaler is 16, 32, 64, ...
                ADCA.EVCTRL = 0x40;     // Sweep channels 0,1
            }
            else if(Srate) {
                ADCA.PRESCALER = 0x02;
                ADCA.EVCTRL = 0x40;     // Sweep channels 0,1
            }
            else {  // 8uS / div - 2MSPS
                ADCA.EVCTRL = 0x00;     // Only channel 0
                ADCA.PRESCALER = 0x02;
            }
        }
    }
    // Display settings
	GLCD_setting();
    // CH gains
    ADCA.CH0.CTRL = 0x03 | (M.CH1gain<<2);       // Set gain
    ADCA.CH1.CTRL = 0x03 | (M.CH2gain<<2);       // Set gain
	// CH offset
	uint8_t srateoff;
	srateoff=Srate;
	if(Srate>7) srateoff=7;
    CH1.offset=-(eeprom_read_byte((uint8_t *)&offset8CH1[srateoff][M.CH1gain]));
    if(Srate) CH2.offset=-(eeprom_read_byte((uint8_t *)&offset8CH2[srateoff-1][M.CH2gain]));
    PMIC.CTRL = 0x07;   // Enable all interrupts
    // Logic input options
    uint8_t temp;
    temp=0;
    if(testbit(CHDctrl,chinvert)) temp |=0x40;    // Invert logic inputs
    if(testbit(CHDctrl,pull)) {                 // Pull
        temp|=0x10;
        if(testbit(CHDctrl,pullup)) temp|=0x08;     // Pull up
    }
    PORTCFG.MPCMASK = 0xFF;
    PORTC.PIN0CTRL  = temp;
	clrbit(MStatus, updatemso);
 }

// Slow sampling control - 640Hz or 1600Hz
ISR(TCE0_OVF_vect) {

}

#if 0 
// Timer for auto trigger and auto key repeat
ISR(TCC1_OVF_vect) {
    if (Key) setbit(Misc,keyrep);
    // Trigger for AUTO mode:
    if(!testbit(MStatus, stop) && testbit(Trigger, autotrg) && MFFT>=0x20) {
        if(!Key && !testbit(MStatus, triggered) && !(testbit(Mcursors,roll) && Srate>=11)) {    // To prevent updates with low sampling rate
            setbit(MStatus, update);
            setbit(MStatus, triggered);
        }
    }
}

#endif

void StartDMAs(void) {
    setbit(DMA.CH0.CTRLA,6);    // reset DMA CH0
    setbit(DMA.CH2.CTRLA,6);    // reset DMA CH2
    setbit(DMA.CH1.CTRLA,6);    // reset DMA CH1
    // DMA for ADC CH0
    DMA.CH0.ADDRCTRL  = 0b00000101;         // Source fixed, incr dest, reload dest @ end block
    DMA.CH0.TRIGSRC   = 0x10;               // ADCA CH0 is trigger source
    DMA.CH0.TRFCNT    = 512;                // buffer size
    DMA.CH0.DESTADDR0 = (((uint16_t) Temp.IN.CH1)>>0*8) & 0xFF;
    DMA.CH0.DESTADDR1 = (((uint16_t) Temp.IN.CH1)>>1*8) & 0xFF;
//    DMA.CH0.DESTADDR2 = 0;
    DMA.CH0.SRCADDR0  = (((uint16_t)(&ADCA.CH0.RESL))>>0*8) & 0xFF;
    DMA.CH0.SRCADDR1  = (((uint16_t)(&ADCA.CH0.RESL))>>1*8) & 0xFF;
//    DMA.CH0.SRCADDR2  = 0;
    DMA.CH0.CTRLA     = 0b00100100;         // repeat, 1 byte burst
    // DMA for logic
    DMA.CH2.ADDRCTRL  = 0b00000101;         // Source fixed, incr dest, reload dest @ end block
    DMA.CH2.TRIGSRC   = 0x10;               // ADCA CH0 is trigger source
    DMA.CH2.TRFCNT    = 512;                // buffer size
    DMA.CH2.DESTADDR0 = (((uint16_t) Temp.IN.CHD)>>0*8) & 0xFF;
    DMA.CH2.DESTADDR1 = (((uint16_t) Temp.IN.CHD)>>1*8) & 0xFF;
//    DMA.CH2.DESTADDR2 = 0;
    DMA.CH2.SRCADDR0  = (((uint16_t)(&VPORT2.IN))>>0*8) & 0xFF;
    DMA.CH2.SRCADDR1  = (((uint16_t)(&VPORT2.IN))>>1*8) & 0xFF;
//    DMA.CH2.SRCADDR2  = 0;
    DMA.CH2.CTRLA     = 0b00100100;         // repeat, 1 byte burst
    // DMA for ADC CH1
    DMA.CH1.ADDRCTRL  = 0b00000101;         // Source fixed, incr dest, reload dest @ end block
    DMA.CH1.TRIGSRC   = 0x11;               // ADCA CH1 is trigger source
    DMA.CH1.TRFCNT    = 512;                // buffer size
    DMA.CH1.DESTADDR0 = (((uint16_t) Temp.IN.CH2)>>0*8) & 0xFF;
    DMA.CH1.DESTADDR1 = (((uint16_t) Temp.IN.CH2)>>1*8) & 0xFF;
//    DMA.CH1.DESTADDR2 = 0;
    DMA.CH1.SRCADDR0  = (((uint16_t)(&ADCA.CH1.RESL))>>0*8) & 0xFF;
    DMA.CH1.SRCADDR1  = (((uint16_t)(&ADCA.CH1.RESL))>>1*8) & 0xFF;
//    DMA.CH1.SRCADDR2  = 0;
    DMA.CH1.CTRLA     = 0b00100100;     // repeat, 1 byte burst

    setbit(DMA.CH0.CTRLA, 7);           // Start DMAs
    setbit(DMA.CH2.CTRLA, 7);
    setbit(DMA.CH1.CTRLA, 7);

    if(Srate<6) ADCA.CTRLB = 0x1C;  // signed mode, free run, 8 bit
    else TCE0.CTRLA  = 0x02;        // Enable Timer, Prescaler: clk/2
    // Minimum time: 128us, Maximum time: 160mS
    uint16_t i;
    while(!testbit(DMA.CH0.CTRLB,4)) {   // Capture one full buffer of pre-trigger samples
        _delay_us(3);
        i++;
        if(testbit(MStatus,update)) {
            clrbit(MStatus,triggered);   // Invalidate data, since the user is interacting
            break;
        }
        if(i==0) break;     // timeout ~ 197mS
    }
}
