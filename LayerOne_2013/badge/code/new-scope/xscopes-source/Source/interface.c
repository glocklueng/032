/*****************************************************************************

XMEGA Oscilloscope and Development Kit

Gabotronics
February 2012

Copyright 2012 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <util/crc16.h>
#include <avr/interrupt.h>
#include "main.h"
#include "mso.h"
#include "logic.h"
#include "display.h"
#include "awg.h"
#include "interface.h"

#define SOH     0x01    // start of heading
#define STX     0x02    // start of text
#define EOT     0x04    // end of transmission
#define ACK     0x06    // acknowledge
#define NAK     0x15    // negative acknowledge
#define CAN     0x18    // cancel

// Monochrome 128x64 bitmap header
const uint8_t BMP[62] PROGMEM = {
    0x42,0x4D,0x3E,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x28,0x00,
    0x00,0x00,0x80,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,
    0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00 };

static inline void SendBMP(void);

static uint8_t read(void);

ISR(USARTE0_RXC_vect) {
    char command;
    uint8_t *p;
    uint8_t i=0,j;
    RTC.CNT=0;  // Clear screen saver timer
    OFFGRN();   // In case the LED was on
    command = USARTE0.DATA;
    switch(command) {
        case 'a':   // Send firmware version
            p = (uint8_t *)(VERSION+3);
			for(;i<4;i++) send(pgm_read_byte(p++)); // Send 4 characters
            return;
		case 'b':	// Write byte to GPIO or M
		    i=read();   // Read index
		    j=read();   // Read value
            WriteByte(i,j);
            return;
		case 'c':	// Set Frequency (4 bytes)
			p=(uint8_t *)&M.AWGdesiredF;
			*p++=read();
			*p++=read();
			*p++=read();
			*p++=read();
			setbit(MStatus, updateawg);
            return;
		case 'd':   // Store settings
		    SaveEE();
		    return;
		case 'e':   // Save AWG in RAM to EE
		    eeprom_write_block(AWGBuffer, EEwave, 256);
		    return;
        case 'f':   // Stop
            setbit(MStatus,update);
            setbit(MStatus,stop);
		    return;
        case 'g':   // Start
            setbit(MStatus,update);
            clrbit(MStatus,stop);
		    return;
        case 'h':   // Force Trigger
            setbit(MStatus,update);
            setbit(MStatus,triggered);
		    return;
        case 'i':   // Auto Setup
            setbit(MStatus,update);
            AutoSet();
		    return;            
		case 'j':	// Set Post Trigger (2 bytes)
		    p=(uint8_t *)&M.Tpost;
		    *p++=read();
		    *p++=read();
            CheckPost();
		return;
        case 'p': clrbit(Misc,autosend); return;    // Do not automatically send data to UART
        case 'q': setbit(Misc,autosend); return;    // Automatically send data to UART
        case 'r':   // Send CH1 data
            p=D.CH1data;
            do {
                send(*p++);
            } while(++i);
            return;
        case 's':   // Send CH2 data
            p=D.CH2data;
            do {
                send(*p++);
            } while(++i);
            return;
        case 't':   // Send CHD data
            p=D.CHDdata;
            do {
                send(*p++);
            } while(++i);
            return;
        case 'u':   // Send settings to PC
            p=(uint8_t *)0;  for(; i<12; i++) send(*p++);   // GPIO
            p=(uint8_t *)&M; for(; i<44; i++) send(*p++);   // M
            return;
        case 'w':   // Send waveform stored in EE
            do { send(eeprom_read_byte(EEwave+i)); } while(++i);
            return;
        case 'x':   // Read data and store in AWG buffer
            send('G');   // confirmation
            p=AWGBuffer;
            do { *p++ = read(); } while(++i);
            setbit(MStatus, updateawg);
            send('T');   // confirmation
            return;
        case 'C': SendBMP(); return; // Send BMP
    }
}

void WriteByte(uint8_t i, uint8_t value) {
    uint8_t *p;
    if(i==0 && Srate>10) clrbit(MStatus, triggered);    // prevents bad wave when changing from 20 to 10ms/div
    if(i<=5 || i==35 || i==38 || i==12 || i==13 || (i>=24 && i<=28))  setbit(MStatus, update);      // Changing trigger
    if(i<=13) setbit(MStatus, updatemso);   // Settings are changing
    if(i>=36) setbit(MStatus, updateawg);
	if(i<12) p=(uint8_t *)i;	    // Accessing GPIO
	else {
        i-=12;
        p=(uint8_t *)(&M);			// Accessing M
        p+=i;
    }
	*p=value;		                // Write data
}

void send(uint8_t tx) {
    while ( !testbit(USARTE0.STATUS,USART_DREIF_bp) ) ;  // wait until ready to transmit
    USARTE0.DATA = tx;
}

// Waits for a character from the serial port for a certain time, 
// If no character is received, it returns 0
static uint8_t read(void) {
    uint16_t timeout=25000; // 250 ms
    while(!testbit(USARTE0.STATUS,USART_RXCIF_bp)) {
        _delay_us(10);
        if(--timeout==0) return 0;
    }
    return USARTE0.DATA;
}

// Test pixel on display buffer
static inline uint8_t test_pixel(uint8_t x, uint8_t y) {
    if(display_buffer[((uint16_t)(y<<4)&0xFF80) + x] & (uint8_t)(0x01 << (y & 0x07))) return 0;
    return 1;
}

static inline uint8_t transpose(uint8_t x, uint8_t y) {
    uint8_t data=0;
	for(uint8_t i=0; i<8; i++) {
		data=data<<1;
        data  |= test_pixel(x+i,y); 
	}
    return data;
}

// Send data from LCD to PC via RS-232 using the XMODEM protocol
static inline void SendBMP(void) {
    uint8_t rx,data,Packet=1, n=0,i,j;
    uint16_t crc=0;

    // First Block
    send(SOH);
    send(Packet);       // Send Packet number
    send(255-Packet);   // Send Packet number 1's complement
    // Send BMP Header
    for(i=0; i<62; i++) {
        n++;
        data = pgm_read_byte_near(BMP+i);
        crc=_crc_xmodem_update(crc,data);
        send(data);
    }

    // Send LCD data, 64 lines
    for(i=63; i!=255; i--) {
        // Each LCD line consists of 16 bytes
        for(j=0; j<16; j++) {
            data=transpose(j*8,i);
            crc=_crc_xmodem_update(crc,data);
            send(data);
            n++;
            if(n==128)  {   // end of 128byte block
                n=0;
                send(hibyte(crc));
                send(lobyte(crc));
                // Wait for ACK
                rx = read();
                if(rx!=ACK) return; // Error -> cancel transmission
                Packet++;
                send(SOH);
                send(Packet);
                send(255-Packet);
                crc=0;
            }
        }
    }

    // End of last block
    for(; n<128; n++) { // send remainder of block
        data= 0x1A;     // pad with 0x1A which marks the end of file
        crc=_crc_xmodem_update(crc,data);
        send(data);
    }
    send(hibyte(crc));
    send(lobyte(crc));
    rx = read();
    if(rx!=ACK) return;
    send(EOT);
    rx = read();
    if(rx!=NAK) return;
    send(EOT);
    rx = read();
    if(rx!=ACK) return;
}
