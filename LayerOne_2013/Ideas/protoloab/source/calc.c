/*****************************************************************************

Xmegalab / XMultiKit:
XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
July 2010

Copyright 2009 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/


#include <avr/io.h>
#include <util/delay_basic.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "calc.h"
#include "parse.h"

char *fn[] = {    // Function keys
    "+",    "-",    "/",    "*", ".",
  "ans",    "^",    "e",    "(", ")",
  "log",   "ln",  "abs", "sqrt", ")",
  "sin",  "cos",  "tan",   "pi", ")",
 "asin", "acos", "atan",   "pi", ")",
 "sinh", "cosh", "tanh",   "pi", ")",
 "last", "Gr2D", "Gr3D",    "y", "x"  };

void CALC(void) {
    uint8_t len,i=0, shift=NSHFT,data,tempROT1=0;
    uint16_t addr=0;
    char entry[168] = "";       // current entry
    char lastentry[168] = "";   // last entry
    char result[168] = "";      // string result

    lcd_init(TEXT);
    lcd_show_cursor(3); // Flash cursor at approx 2Hz
    lcd_cursor_type(0); // Block cursor type
    ROT1 = 0;
    shift = 0;
    ROT2 = 128;
    MAX1=5;
    MAX2 = 255;
    key=0;
    setbit(Signals, update);
    for(;;) {
        while (!testbit(Signals, update)) {
        }
        clrbit(Signals, update);
        if(tempROT1!=ROT1) { tempROT1=ROT1; shift=ROT1; }
        if(ROT2<128) {      // Rotary Encoder 2
            key=KBack;      // Back space
            ROT2=128;
        }
        if(ROT2>128) {
            shift = 6;
            ROT2=128;
        }

        if(key==KA || key==KB || key==KC || key==KD || key==KE) {
            if(shift<6) {
                switch(key) {
                    case KA: i=shift*5;   break;
                    case KB: i=shift*5+1; break;
                    case KC: i=shift*5+2; break;
                    case KD: i=shift*5+3; break;
                    case KE: i=shift*5+4; break;
                }
                lcd_puts(fn[i]);
                strcat(entry,fn[i]);
                if(shift>1 && i!=14 && i!=19 && i!=24 &&
                    i!=29 && i!=18 && i!=23 && i!=28) {
                    len=strlen(entry);
                    put_char('(');
                    entry[len+1]='\0';
                    entry[len]='(';
                }
            }
            else {
                switch(key) {
                    case KA:    // Restore last entry
                        strcpy(entry,lastentry);
                        //lcd_set_cursor_addr(addr);   // set cursor
                        lcd_puts(entry);
                    break;
                    case KB:
                        graph2d(entry);
                        strcpy(lastentry,entry);        // save to last entry
                        entry[0]='\0';                  // erase current entry    
                    break;
                    case KC:
                        graph3d(entry);
                        strcpy(lastentry,entry);        // save to last entry
                        entry[0]='\0';                  // erase current entry  
                    break;
                    case KD:
                        put_char('y');
                        len=strlen(entry);
                        entry[len+1]='\0';
                        entry[len]='y';
                    break;
                    case KE:
                        put_char('x');
                        len=strlen(entry);
                        entry[len+1]='\0';
                        entry[len]='x';
                    break;
                }
            }
        }
        else if(key==KP) {                  // Process entry
            addr = lcd_read_cursor_addr();  // cursor address
            if(strlen(entry)==0) {          // null entry
                strcpy(entry,lastentry);    // process previous entry
                addr = addr-addr%21;        // use same line
            }
            else addr = addr+21-addr%21;    // next line
            evaluate(entry);                // result stored in ans
            if(!m_error) sprintf(result,"%G",ans);  // convert to string
            else { 
                switch(m_error) {
                    case NoNumber: strcpy(result, "error");
                    break;
                    case INVALID: strcpy(result, "Invalid expression");
                    break;
                }
            }
            len=strlen(result);
            lcd_set_cursor_addr(addr);
            lcd_putsp(emptyline);
            if(len<21) {    // Right justify
                lcd_set_cursor_addr(addr+21-strlen(result)); // next line
            }
            else lcd_set_cursor_addr(addr); // next line
            lcd_puts(result);
            strcpy(lastentry,entry);        // save to last entry
            entry[0]='\0';                  // erase current entry
            while(addr<lcd_read_cursor_addr()) addr+=21;
            lcd_set_cursor_addr(addr);   // next line
        }
        else if(key==KBack) {  // Backspace
            len=strlen(entry);
            if(len>0) {
                entry[len-1]='\0';
                addr = lcd_read_cursor_addr();
                lcd_set_cursor_addr(addr-1);
                put_char(' ');
                lcd_set_cursor_addr(addr-1);
            }
        }
        else if(key==KS) {     // Clear
            if(strlen(entry)==0) return;
            else {
                len=strlen(entry);
                lcd_set_cursor_addr(addr-len);   // set cursor
                for(i=0; i<len; i++) put_char(' ');
                entry[0]='\0';
                lcd_set_cursor_addr(addr-len);   // set cursor
            }
        }
        else if(key>0) {                  // Add text
            i=pgm_read_byte(&Letter[key-1]);
            put_char(i);
            len=strlen(entry);
            entry[len+1]='\0';
            entry[len]=i;
        }

        addr = lcd_read_cursor_addr();
        while(addr>=SAD1+315) {     // scroll down
            lcd_putsp(emptyline);
            lcd_set_cursor_addr(addr);
            lcd_scroll(21,0,21,0);
        }
        if(SAD1>966) {  // reached end of memory
            lcd_set_cursor_addr(SAD1);
            lcd_write_command(MREAD);
            for(addr=SAD1; addr<(SAD1+294); addr++) {   // copy data from LCD
                data=lcd_read_data();
                WriteSerialRAM(addr-SAD1,&data,1);
            }
            lcd_clear_text();
            lcd_write_command(MWRITE);
            for(addr=SAD1; addr<(SAD1+294); addr++) {   // copy data to LCD
                ReadSerialRAM(addr-SAD1,&data,1);
                lcd_write_data(data);
            }
        }
        key=0;
        // Print Menu
        addr = lcd_read_cursor_addr();          // save cursor address
        lcd_set_cursor_addr(SAD1+21*15); lcd_putsp(emptyline);
        for(i=0; i<5; i++ ) {
            lcd_set_cursor_addr(SAD1+21*15+i*5);
            lcd_puts(fn[shift*5+i]);
        }
        lcd_set_cursor_addr(addr);              // restore cursor address*/
    }
}

void graph2d(char *expression) {
    uint8_t i;
    int py,oy=-1;
    double minx = -8;
    double maxx =  8;
    double out;
    lcd_init(GRAPHICS);
    lcd_write_command(CSR_DOWN);

    // Grid
    for(i=8; i<=120; i+=8) { set_pixel(i,191); set_pixel(65,i+128); }
    lcd_line(0,192,127,192); lcd_line(64,128,64,255);
    
    for(i=0, x=minx; i<128; i++,x+=(maxx-minx)/128) {
        evaluate(expression);
        out=8*ans;
        py=64-out;
        if(py>=0 && py<128) {
            set_pixel(i, py);
            if (oy>=0 && oy<128) lcd_line(i,py,i-1,oy);
        }
        oy=py;
    }

    while (!testbit(Signals, update)) ;
    clrbit(Signals, update);
    lcd_init(TEXT);
    lcd_show_cursor(3); // Flash cursor at approx 2Hz
    lcd_cursor_type(0); // Block cursor type
}

void graph3d(char *expression) {
    int px,py,ox=-1,oy=-1;
    uint8_t i;
    double minx = -2*M_PI;
    double maxx =  2*M_PI;
    double miny = -2*M_PI;
    double maxy =  2*M_PI;
    double oxs[64];
    double oys[64];
    lcd_init(GRAPHICS);
    lcd_write_command(CSR_DOWN);

    for(x=minx; x<maxx; x+=4*(maxx-minx)/128) {
        for(y=miny, i=0; y<maxy; y+=4*(maxy-miny)/128, i++) {
            evaluate(expression);
            px = 16 * (x + y/2) + 64;
            py = 64- 16 * (ans + y/2);
            if(px>=0 && px<128 && py>=0 && py<128) {
                set_pixel(px, py);
                // draw connecting lines to visualize the grid
                if(ox>=0 && ox<128 && oy>=0 && oy<128) {
                    lcd_line( px, py, ox, oy);  
                }
                if(oxs[i]>=0 && oxs[i]<128 && oys[i]>=0 && oys[i]<128) {
                    lcd_line( px, py, oxs[i], oys[i]);
                }
            }
            ox=px;     oy=py;
            oxs[i]=px; oys[i]=py;
        }
    }
    while (!testbit(Signals, update)) ;
    clrbit(Signals, update);
    lcd_init(TEXT);
    lcd_show_cursor(3); // Flash cursor at approx 2Hz
    lcd_cursor_type(0); // Block cursor type
}
