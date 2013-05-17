/***********************************************************************

Xmegalab / XMultiKit:
AVR Oscilloscope and Development Kit

Gabotronics C.A.
July 2010

Copyright 2010 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

Xmodem BMP and and storage functions
    19200 bps
    8 data bits
    1 stop bit
    no parity

*****************************************************************************/

#include <avr/io.h>
#include <util/delay_basic.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>
#include "main.h"
#include "ex4bit.h"
#include "diskio.h"
#include "uart.h"
#include "ff.h"
#include "mso.h"
#include "storage.h"

#define SOH     0x01    // start of heading
#define STX     0x02    // start of text
#define EOT     0x04    // end of transmission
#define ACK     0x06    // acknowledge
#define NAK     0x15    // negative acknowledge
#define CAN     0x18    // cancel

// Monochorome 128x128 bitmap header
uint8_t BMP[62] PROGMEM = {
    0x42,0x4D,0x3E,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x28,0x00,
    0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,
    0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00 };

const char clear[] PROGMEM = "                                ";
const char savedtext[] PROGMEM = "FILE SAVED: ";
void errorsound(void);

// Send data from LCD to PC via RS-232 using the XMODEM protocol
void SendBMP(void) {
    uint8_t rx,data,Packet=1, n=0,i,j,time_out=0;
    uint8_t low, high;
    uint16_t Address;
    CRC16_High=0;
    CRC16_Low=0;

    lcd_or_layers();    // OR second layer on to first layer

    // First Block
    uart_put(SOH);
    uart_put(Packet);       // Send Packet number
    uart_put(255-Packet);   // Send Packet number 1's complement
    // Send BMP Header
    for(i=0; i<62; i++) {
        n++;
        data = pgm_read_byte_near(BMP+i);
        CRC16_Update(data);
        uart_put(data);
    }
    lcd_write_command(CSR_RIGHT);

    // Send LCD data, 128 lines
    for(i=127; i!=255; i--) {
        Address = (unsigned int)i<<4;
        low = (uint8_t) (Address & 0x00ff);
        high = (uint8_t) ((Address & 0xff00) >> 8);
        lcd_write_command(CSRW);        // Set cursor address
        lcd_write_data(low);
        lcd_write_data(high);
        lcd_write_command(MREAD);

        // Each LCD line consists of 16 bytes
        for(j=0; j<16; j++) {
            data=~lcd_read_data();
            CRC16_Update(data);
            uart_put(data);
            n++;
            if(n==128)  {   // end of 128byte block
                n=0;
                uart_put(CRC16_High);
                uart_put(CRC16_Low);
                time_out=0;
                // Wait for ACK
                while (!uart_test()) {
                    time_out++; if(time_out>200) return;
                    _delay_ms(1);
                }
                rx = uart_get();
                if(rx!=ACK) return; // Error -> cancel transmission
                Packet++;
                uart_put(SOH);
                uart_put(Packet);
                uart_put(255-Packet);
                CRC16_High=0;
                CRC16_Low=0;
            }
        }
    }

    // End of last block
    for(; n<128; n++) { // send remainder of block
        data= 0x1A;     // pad with 0x1A which marks the end of file
        CRC16_Update(data);
        uart_put(data);
    }
    uart_put(CRC16_High);
    uart_put(CRC16_Low);
    time_out=0;
    while (!uart_test()) {
        time_out++; if(time_out>200) return;
        _delay_ms(1);
    }
    rx = uart_get();
    if(rx!=ACK) return;

    uart_put(EOT);
    time_out=0;
    while (!uart_test()) {
        time_out++; if(time_out>200) return;
        _delay_ms(1);
    }
    rx = uart_get();
    if(rx!=NAK) return;
    uart_put(EOT);
    time_out=0;
    while (!uart_test()) {
        time_out++; if(time_out>200) return;
        _delay_ms(1);
    }
    rx = uart_get();
    if(rx!=ACK) return;
    lcd_write_command(CSR_DOWN);
    sound(150);
}

// Open file and show errors
static uint8_t openfile(FIL *file, char filename[], uint8_t mode) {
    uint8_t data,d,d1,d2,d3,roll;
    static uint8_t next=0;

    data=disk_status();
    // Check SD Card
    if(data&STA_NODISK) {          // No medium in the drive
        tiny_printp(0,122, PSTR("NO SD CARD "));
        errorsound();
        return data;
    }
    if(data&STA_NOINIT) {     // Drive not initialized
        tiny_printp(0,122, PSTR("SD INIT FAILED "));
        errorsound();
        return data;
    }
    if(mode&FA_WRITE) {
        if(data&STA_PROTECT) {    // Write protected
            tiny_printp(0,122, PSTR("SD CARD WRITE PROTECTED "));
            errorsound();
            return data;
        }
        next = 0;           // Creating a file, search for next available
    }

    // Erase menu
    tiny_printp(0,122,clear);
    setbit(Signals, redraw);

    roll=next;
    // Scan files
    do {
        set_pixel(next>>1,254);        // progress bar
        // Prepare filename to test
        d1=d2=d3=0;
        d=next;
        while (d>=100)  { d1++; d-=100; }
	    while (d>=10)   { d2++; d-=10; }
	    while (d>=1)    { d3++; d-=1; }
        filename[2] = d1+0x30;
        filename[3] = d2+0x30;
        filename[4] = d3+0x30;
        next++;
        if(mode&FA_WRITE) {  // Try to create the file
            data=f_open(file, filename, FA_CREATE_NEW | FA_WRITE);
            if(data==FR_OK) {
                tiny_printp(0,122,clear);
                return FR_OK;
            }
        }
        else {              // Try to open the file
            data=f_open(file, filename, FA_OPEN_EXISTING | FA_READ);
            if(data==FR_OK) {
                strcpy(finfo.fname, filename);  // Save name to be used by erase function
                return FR_OK;
            }
        }
    } while(roll!=next);

    if(mode&FA_WRITE) {  // All 256 slots are used
        tiny_printp(0,122, PSTR("SLOTS FULL"));
    }
    else {
        tiny_printp(0,122,PSTR("CANNOT OPEN FILE"));
    }
    errorsound();
    return 1;   // return with error
}

// Save oscilloscope data to file in SD card, internal WFM format
void LoadWFM(void) {
    FIL file;
    uint8_t c[2],d;
    char text[10]="GT000.WFM";

    if(openfile(&file, text, FA_OPEN_EXISTING | FA_READ)) return;

    f_read(&file,&c,2,&d);
    if(c[0]!='X' || c[1]!='0') {
        errorsound();
    }
    tiny_printp(0,122,PSTR("FILE LOADED:"));
    tiny_print(6,122,text);

    f_read(&file,1,10,&d);             // Save GPIO1 thru GPIOA which contains settings
    f_read(&file,&VcursorA,1,&d);      // Vertical cursor A
    f_read(&file,&VcursorB,1,&d);      // Vertical cursor B
    f_read(&file,&Hcursor1A,1,&d);     // CH1 Horizontal cursor A
    f_read(&file,&Hcursor1B,1,&d);     // CH1 Horizontal cursor B
    f_read(&file,&Hcursor2A,1,&d);     // CH2 Horizontal cursor A
    f_read(&file,&Hcursor2B,1,&d);     // CH2 Horizontal cursor B
    f_read(&file,&tlevel,1,&d);        // Trigger level
    f_read(&file,&HPos,1,&d);          // Horizontal position
    f_read(&file,&CH1,sizeof(CH1),&d); // Save CH1
    f_read(&file,&CH2,sizeof(CH2),&d); // Save CH2
    f_read(&file,&CHD,sizeof(CHD),&d); // Save Digital

    setbit(MStatus, stop);
    f_close(&file);
    sound(150);
}

// Erase last opened WFM file
void EraseWFM(void) {
    FRESULT data;
    data = f_unlink(finfo.fname);
    if(data) {
        errorsound();
        return;
    }
    tiny_printp(0,122,clear);
    tiny_printp(0,122,PSTR("FILE ERASED:"));
    tiny_print(6,122,finfo.fname);
    sound(150);
}

// Save oscilloscope data to file in SD card, internal WFM format
void SaveWFM(void) {
    FIL file;
    uint8_t n;
    char text[10]="GT000.WFM";

    if(openfile(&file, text, FA_CREATE_ALWAYS | FA_WRITE)) return;

    tiny_printp(0,122,savedtext);
    tiny_print(6,122,text);

    f_putc('X', &file);
    f_putc('0', &file);

    f_write(&file,1,10,&n);             // Save GPIO1 thru GPIOA which contains settings
    f_write(&file,&VcursorA,1,&n);      // Vertical cursor A
    f_write(&file,&VcursorB,1,&n);      // Vertical cursor B
    f_write(&file,&Hcursor1A,1,&n);     // CH1 Horizontal cursor A
    f_write(&file,&Hcursor1B,1,&n);     // CH1 Horizontal cursor B
    f_write(&file,&Hcursor2A,1,&n);     // CH2 Horizontal cursor A
    f_write(&file,&Hcursor2B,1,&n);     // CH2 Horizontal cursor B
    f_write(&file,&tlevel,1,&n);        // Trigger level
    f_write(&file,&HPos,1,&n);          // Horizontal position
    f_write(&file,&CH1,sizeof(CH1),&n); // Save CH1
    f_write(&file,&CH2,sizeof(CH2),&n); // Save CH2
    f_write(&file,&CHD,sizeof(CHD),&n); // Save Digital
   
    f_close(&file);
    sound(150);
}

// Save oscilloscope data to file in SD card, CSV format
void SaveCSV(void) {
    static char unituS[] = "uS,";   // uS
    static char unitmS[] = "mS,";   // mS
    static char unitS[]  = " S,";   // S
    static char unitV[]  = " V,";   // V
    static char unitmV[] = "mV,";   // mV
    FIL file;
    char *units;
    uint8_t i;
    char text[32]="GT000.CSV";

    if(openfile(&file, text, FA_CREATE_ALWAYS | FA_WRITE)) return;

    tiny_printp(0,122,savedtext);
    tiny_print(6,122,text);
    
    // Header
    f_printf(&file, "Time,CH1,CH2,Logic\n");
    // Time units
    if(srate<=8) units = unituS;
    else if(srate<=17) units = unitmS;
    else units = unitS;
    f_printf(&file, units);
    // CH1 units
    if(CH1.ctrl.gain<4) units = unitmV;
    else units = unitV;
    f_printf(&file, units);
    // CH2 units
    if(CH2.ctrl.gain<4) units = unitmV;
    else units = unitV;
    f_printf(&file, units);
    // Logic
    f_printf(&file, "Data\n");

    i=0;
    do {
        set_pixel(i>>1,255);        // progress bar
        sprintf(text,"%f,",(float)i*pgm_read_dword_near(timeval+srate)/100000);
        f_printf(&file, "%s", text);
        sprintf(text,"%f,",-((float)CH1.data[i]-128)*milivolts[CH1.ctrl.gain]/200000);
        f_printf(&file, "%s", text);
        sprintf(text,"%f,",-((float)CH2.data[i]-128)*milivolts[CH2.ctrl.gain]/200000);
        f_printf(&file, "%s", text);
        sprintf(text,"%X\n",CHD.data[i]);
        f_printf(&file, "%s", text);
    } while(++i);

    tiny_printp(0,122,savedtext);
    tiny_print(6,122,text);

    sound(150);
    f_close(&file);
}


// Save LCD screenshot to file in SD card, BMP format
void SaveBMP(void) {
    FIL file;
    uint8_t data,i,j,n;
    uint8_t low, high;
    uint16_t Address;
    char text[10]="GT000.BMP";

    if(openfile(&file, text, FA_CREATE_ALWAYS | FA_WRITE)) return;

    lcd_or_layers();    // OR second layer on to first layer

    // Save BMP Header
    for(i=0; i<62; i++) {
        data = pgm_read_byte_near(BMP+i);
        f_write(&file,&data,1,&n);
    }
    lcd_write_command(CSR_RIGHT);
    
    // Send LCD data
    for(i=127; i!=255; i--) {
        Address = (unsigned int)i<<4;
        low = (uint8_t) (Address & 0x00ff);
        high = (uint8_t) ((Address & 0xff00) >> 8);
        lcd_write_command(CSRW);        // Set cursor address
        lcd_write_data(low);
        lcd_write_data(high);
        lcd_write_command(MREAD);
        for(j=0; j<16; j++) {
            data=~lcd_read_data();
            f_write(&file,&data,1,&n);
        }
    }
    tiny_printp(0,122,savedtext);
    tiny_print(6,122,text);
    f_close(&file);
    sound(150);
}

void errorsound(void) {
    ONRED();
    sound(200); _delay_ms(75);
    sound(200); _delay_ms(75);
    sound(255); _delay_ms(500);
    OFFRED();
}
