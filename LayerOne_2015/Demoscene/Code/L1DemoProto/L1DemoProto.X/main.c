/* 
 * File:   main.c
 * Author: arko
 *
 * Created on September 8, 2014, 7:25 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

//#define  FCY    16000000UL    // Instruction cycle frequency, Hz
//#include <libpic30.h>

_CONFIG1(FWDTEN_OFF)
_CONFIG2(FNOSC_FRCPLL & PLL96MHZ_ON)

int HOR_RES = 320;
int VER_RES = 240;
int HOR_FRONT_PORCH = 20;
int HOR_BACK_PORCH = 38;
int HOR_PULSE_WIDTH  = 30;
int VER_FRONT_PORCH = 4;
int VER_BACK_PORCH = 15;
int VER_PULSE_WIDTH = 3;

void config_graphics(void) {
    CLKDIVbits.RCDIV = 0;
    CLKDIVbits.G1CLKSEL = 1;    /* Use 96Mhz Clk */
    CLKDIV2bits.GCLKDIV = 4;  // 16 for 30Hz, 6 for 60Hz

    CLKDIVbits.PLLEN = 1;

    G1CON2bits.DPMODE = 1;      /* TFT */
    G1DBENbits.GDBEN = 0xFFFF;
    G1DPWbits.DPW = 320;
    G1DPHbits.DPH = 240;
    G1DPWTbits.DPWT = 20 + 30 + 38 + 320;
    G1DPHTbits.DPHT = 4 + 3 + 15 + 240;
    G1CON3bits.DPCLKPOL = 1;    /* Sample on falling edge*/
    G1CON3bits.DPENOE = 1;
    G1CON3bits.DPENPOL = 1;
    
    G1CON3bits.DPVSOE = 1;      
    G1CON3bits.DPHSOE = 1;     
    G1CON3bits.DPVSPOL = 0;
    G1CON3bits.DPHSPOL = 0;
    G1ACTDAbits.ACTLINE = 4 + 3 + 15;    // ver sum
    G1ACTDAbits.ACTPIX = 20 + 30 + 38;    // hor sum
    G1VSYNCbits.VSST = 4;   // front
    G1HSYNCbits.HSST = 20;
    G1VSYNCbits.VSLEN = 3;  //pulse
    G1HSYNCbits.HSLEN = 30;
    G1DBLCONbits.VENST = 4 + 3 + 15;
    G1DBLCONbits.HENST = 20 + 30 + 38;

    G1DPADRL = (unsigned long)(0x0000) & 0xFFFF;
    G1DPADRH = 0;

    G1CON2bits.DPBPP = 3;     /* 8bpp mode */
    G1CON1bits.PUBPP = 3;

    G1CON3bits.DPPINOE = 1;
    
    G1CON3bits.DPPWROE = 1;

    G1CON2bits.DPTEST = 2;

    G1CON1bits.G1EN = 1;        /* Enable module */
    G1CON3bits.DPPOWER = 1;

}


int main(void) {
    //OSCCON = 0x0000;

    CLKDIVbits.RCDIV = 0;      // 8 MHz
    CLKDIVbits.CPDIV = 0;

    ANSB = 0x0000;
    ANSC = 0x0000;
    ANSD = 0x0000;
    ANSF = 0x0000;
    ANSG = 0x0000;
    //TRISE = 0x0000;
    config_graphics();

    while (1) {
        G1CON2bits.DPTEST = 2;
    }

    return 0;
}







