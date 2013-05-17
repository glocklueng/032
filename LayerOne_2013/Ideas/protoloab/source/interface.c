/*****************************************************************************

Xmegalab / XMultiKit:
AVR XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
September 2010

Copyright 2010 Gabriel Anzziani

This file is proprietary software, made available to customers for reference.
This file shall no be distributed.

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#include <util/delay_basic.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"
#include "mso.h"
#include "xitoa.h"
#include "uart.h"
#include "storage.h"

void SendNumber(int32_t Data);

char get_timeout(void);

void interface(void) {
    char c;
    uint8_t i, j, index;
    
    // Gary's variables
    #define N 16                // Maximum length of string
    static uint8_t nodf=0;      // Number of Display Fields
    static char** FieldNames=0;     // Pointer to strings size N
    static char ButtonsData [5][N]={    // Array of strings size N
        {' ',' ',' ',' ',' ',0}, 
        {' ',' ',' ',' ',' ',0},
        {' ',' ',' ',' ',' ',0},
        {' ',' ',' ',' ',' ',0},
        {' ',' ',' ',' ',' ',0},    
    }; 
    static char buttontxt[] = "                                ";

    c = uart_get();
    if(c=='C') {
        SendBMP();
        return;
    }

    // Encoders
    if(c=='q') {    // ROT1 -
        if(ROT1>0) { ROT1--; PORTC.OUTTGL = _BV(BUZZER1); }
        else { sound(100); ROT1=MAX1; }     // Roll over
        setbit(Signals, userinput);
        setbit(Signals, update);
    }
    else if(c=='w') {    // ROT1 +
        if(ROT1<MAX1) { ROT1++; PORTC.OUTTGL = _BV(BUZZER1); }
        else { sound(100); ROT1=0; }        // Roll over
        setbit(Signals, userinput);
        setbit(Signals, update);
    }
    else if(c=='z') {    // ROT2 -
        if(ROT2>0) { ROT2--; PORTC.OUTTGL = _BV(BUZZER1); }
        else { sound(100); ROT2=MAX2; }     // Roll over
        setbit(Signals, userinput);
        setbit(Signals, update);
    }
    else if(c=='x') {    // ROT2 +
        if(ROT2<MAX2) { ROT2++; PORTC.OUTTGL = _BV(BUZZER1); }
        else { sound(100); ROT2=0; }        // Roll over
        setbit(Signals, userinput);
        setbit(Signals, update);
    }

    // Buttons
    else if((c>='a' && c<='e') || (c>='0' && c<='9') || c=='*' || c=='#') {
        if(c=='0') key = K0;
        if(c=='1') key = K1;
        if(c=='2') key = K2;
        if(c=='3') key = K3;
        if(c=='4') key = K4;
        if(c=='5') key = K5;
        if(c=='6') key = K6;
        if(c=='7') key = K7;
        if(c=='8') key = K8;
        if(c=='9') key = K9;
        if(c=='*') key = KS;
        if(c=='#') key = KP;
        if(c=='a') key = KA;
        if(c=='b') key = KB;
        if(c=='c') key = KC;
        if(c=='d') key = KD;
        if(c=='e') key = KE;
        setbit(Signals, userinput);
        setbit(Signals, serialin);
    }

    // Gary's protocol
    // WRITE
    else if(c=='^') {
        uart_put('\n'); uart_put('\r');
        uart_put(c);
        // Wait for command number
        if(!(c=get_timeout())) return;
        uart_put('\n'); uart_put('\r');
        switch(c) {
            case '1':   // Write Config Display
                // Erase previous array
                for (i=0; i<nodf; i++){
                    free(FieldNames[i]);
                }
                free(FieldNames);
                nodf = 0;
                if(!(c=get_timeout())) return;
                nodf += (c-'0')*10;
                if(!(c=get_timeout())) return;
                nodf += (c-'0');
                uart_put('\n'); uart_put('\r');
                // Dynamic array
                FieldNames = (char**) malloc(nodf*sizeof(char*));
                if (FieldNames == NULL) {   // Memory could not be allocated
                    xprintf(PSTR("error\n"));
                    return;
                }
                for (i = 0; i < nodf; i++) {
                    FieldNames[i] = (char*) malloc(N*sizeof(char));
                    FieldNames[i][0] = 0;
                }

            break;
            case '2':   // Write Define Field Name
                // get index
                index=0;
                if(!(c=get_timeout())) return;
                index += (c-'0')*10;
                if(!(c=get_timeout())) return;
                index += (c-'0');
                uart_put('\n'); uart_put('\r');
                if(index>=nodf) {
                    xprintf(PSTR("index error\n"));
                    return;
                }
                // get text
                for(i=0; i<(N-1); i++) {
                    if(!(c=get_timeout())) return;
                    if(c==13) {
                        uart_put('\n');
                        break;
                    }
                    FieldNames[index][i] = c;
                }
                FieldNames[index][i] = 0;
            break;
            case '3':   // Write Button Data
                // get index
                index=0;
                if(!(c=get_timeout())) return;
                index += (c-'0')*10;
                if(!(c=get_timeout())) return;
                index += (c-'0');
                uart_put('\n'); uart_put('\r');
                if(index>=5) {
                    xprintf(PSTR("index error\n"));
                    return;
                }
                // get text
                for(i=0; i<(N-1); i++) {
                    if(!(c=get_timeout())) return;
                    if(c==13) {
                        uart_put('\n');
                        break;
                    }
                    ButtonsData[index][i] = toupper(c);
                }
                ButtonsData[index][i] = 0;
                // Display on the LCD
                index=1;
                for(i=0; i<5; i++) {
                    for(j=0; j<5; j++) {
                        if(ButtonsData[i][j]==0) {
                            for(; j<5; j++) buttontxt[index++]=' ';
                        }
                        else buttontxt[index++] = ButtonsData[i][j];
                    }
                    buttontxt[index++] = ' ';
                }
                tiny_print(0,122, buttontxt);
            break;
        }
    }
    // READ
    else if(c=='%') {
        uart_put('\n'); uart_put('\r');
        uart_put(c);
        // Wait for command number
        if(!(c=get_timeout())) return;
        uart_put('\n'); uart_put('\r');
        switch(c) {
            case '1': xprintf(PSTR("N=%2u\n"), nodf);   // Read Config Display
            break;
            case '2':   // Read Define Field Name
                // get index
                index=0;
                if(!(c=get_timeout())) return;
                index += (c-'0')*10;
                if(!(c=get_timeout())) return;
                index += (c-'0');
                uart_put('\n'); uart_put('\r');
                if(index>=nodf) {
                    xprintf(PSTR("index error\n"));
                    return;
                }
                xprintf(PSTR("%s\n"), FieldNames[index]);
            break;
            case '3':   // Read Button Data
                // get index
                index=0;
                if(!(c=get_timeout())) return;
                index += (c-'0')*10;
                if(!(c=get_timeout())) return;
                index += (c-'0');
                uart_put('\n'); uart_put('\r');
                if(index>=5) {
                    xprintf(PSTR("index error\n"));
                    return;
                }
                xprintf(PSTR("%s\n"), ButtonsData[index]);
            break;
            case '4':
                ReadMeter();
            break;
        }
    }
}

// Waits for a character from the serial port for a certain time, 
// If no character is received in 5 seconds, it returns 0
char get_timeout(void) {
    uint8_t time_out=0,t;
    char c;
    do {
        time_out++; if(time_out>100) return 0;
        _delay_ms(50);
        t=uart_test();
    } while (t==0);
    c = uart_get();
    uart_put(c);
    return c;
}

// Measurements for Meter Mode
void ReadMeter(void) {
    // milivolts or volts per pixels * 100000
    // Range                           20mV    50mV    0.1V     0.2V  0.5V    1V     2V     5V
    const int32_t milivolts[8]   = { 125000, 312500, 625000, 1250000, 3125, 6250, 12500, 31250 };
    double avrg;                 // Average
    uint8_t i,j,n;

    int8_t temp;
    
    AutoCursorV(1);
    if(VcursorA<VcursorB) { i=VcursorA; j=VcursorB; }
    else                  { i=VcursorB; j=VcursorA; }
    // Channel 1
    avrg=0; n=0;
    while (i<=j) {
        temp = (128-(int)CH1.data[i+HPos]);
        avrg+= temp;
        i++;
        n++;
    }
    avrg=avrg/n;    // average
    avrg=avrg*milivolts[CH1.ctrl.gain]/2;  // Convert to volts

    SendNumber(avrg);
    if(CH1.ctrl.gain<4) xprintf(PSTR(" mV\n"));
    else xprintf(PSTR(" V\n"));
}

void SendNumber(int32_t Data) {
	uint8_t D[4]={0,0,0,0},point=0,i;
	uint16_t n;
	if(Data<0) {
        uart_put('-');
        Data = -Data;
    }
    else uart_put('+');
	if(Data>=999900000L) n = 9999;
	else if(Data>=100000000L)  n = (uint16_t)(Data/100000);
	else if(Data>=10000000L) {
		n = (uint16_t)(Data/10000);
		point = 1;
	}
	else if(Data>=1000000L) {
		n = (uint16_t)(Data/1000);
		point = 2;
	}
	else {
		n = (uint16_t)(Data/100);
		point = 3;
	}

	while (n>=1000) { D[3]++; n-=1000; }
	while (n>=100)	{ D[2]++; n-=100; }
	while (n>=10)	{ D[1]++; n-=10; }
	while (n>=1)	{ D[0]++; n-=1; }
	for(i=3; i!=255; i--) {
        uart_put(D[i]+'0');
		if(point==i) {
            uart_put('.');
        }      
	}
}
