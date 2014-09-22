/* 
 * File:   main.c
 * Author: arko
 *
 * Created on September 8, 2014, 7:25 PM
 */
#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#define  FCY    16000000UL    // Instruction cycle frequency, Hz
#include <libpic30.h>

_CONFIG1(FWDTEN_OFF)
_CONFIG2(FNOSC_FRCPLL & PLL96MHZ_ON & PLLDIV_DIV2)

#define HOR_RES 192           //320      //640
#define VER_RES 480           //240      //480
#define HOR_FRONT_PORCH 355   //10       //20
#define HOR_BACK_PORCH 51     //22       //51
#define HOR_PULSE_WIDTH  80   //46       //96
#define VER_FRONT_PORCH 12    //6        //12
#define VER_BACK_PORCH 21     //9        //36
#define VER_PULSE_WIDTH 2     //1        //2

#define GFX_DISPLAY_PIXEL_COUNT    90160

#define GFX_DISPLAY_BUFFER_START_ADDRESS        0x00000001ul
#define GFX_DISPLAY_BUFFER_LENGTH               0x00012C00ul

//__eds__ unsigned char GFXDisplayBuffer[GFX_DISPLAY_PIXEL_COUNT] __attribute__((eds));
//__eds__ unsigned char GFXDisplayBufferBottom[GFX_DISPLAY_PIXEL_COUNT] __attribute__((eds));


__eds__ unsigned char  __attribute__((far,section("eds1b"),space(eds) ,address(0x01000))) GFXDisplayBuffer[ GFX_DISPLAY_PIXEL_COUNT ]; // Sample data buffer
//__eds__ unsigned char  __attribute__((far,section("eds2b"),space(eds) ,address(0x09000))) GFXDisplayBufferBottom[ GFX_DISPLAY_PIXEL_COUNT ]; // Sample data buffer




void config_graphics(void) {
    
    /*
    CLKDIVbits.G1CLKSEL = 1;    // Use 96Mhz Clk
    CLKDIV2bits.GCLKDIV = 56;  // 16 for 30Hz, 6 for 60Hz

    CLKDIVbits.PLLEN = 1;

    G1CON2bits.DPMODE = 1;      // TFT
    G1DBENbits.GDBEN = 0xFFFF;
    G1DPWbits.DPW = 320;
    G1DPHbits.DPH = 240;
    G1DPWTbits.DPWT = 99 + 320;
    G1DPHTbits.DPHT = 22 + 240;
    G1CON3bits.DPCLKPOL = 1;    // Sample on falling edge
    G1CON3bits.DPENOE = 1;
    G1CON3bits.DPENPOL = 1;
    
    G1CON3bits.DPVSOE = 1;      
    G1CON3bits.DPHSOE = 1;     
    G1CON3bits.DPVSPOL = 0;
    G1CON3bits.DPHSPOL = 0;
    G1ACTDAbits.ACTLINE = 22;    // ver sum
    G1ACTDAbits.ACTPIX = 88;    // hor sum
    G1VSYNCbits.VSST = 4;   // front
    G1HSYNCbits.HSST = 20;
    G1VSYNCbits.VSLEN = 3;  //pulse
    G1HSYNCbits.HSLEN = 30;
    G1DBLCONbits.VENST = 4;
    G1DBLCONbits.HENST = 20;

    G1DPADRL = (unsigned long)(0x0000) & 0xFFFF;
    G1DPADRH = 0;

    G1CON2bits.DPBPP = 3;     // 8bpp mode
    G1CON1bits.PUBPP = 3;

    G1CON3bits.DPPINOE = 1;
    
    G1CON3bits.DPPWROE = 1;

    G1CON2bits.DPTEST = 2;
    */
    
    CLKDIVbits.RCDIV = 0;

    CLKDIVbits.G1CLKSEL = 1;                         /* Use 96Mhz Clk */
    CLKDIV2bits.GCLKDIV = 11;  //56      //11        /* 56 = divide by 15 = 6.5Mhz */

    G1CON2bits.DPMODE = 1;      /* TFT */
    G1DBENbits.GDBEN = 0xFFFF;
    G1DPWbits.DPW = HOR_RES;
    G1DPHbits.DPH = VER_RES;
    G1DPWTbits.DPWT = HOR_FRONT_PORCH + HOR_PULSE_WIDTH + HOR_BACK_PORCH + HOR_RES;
    G1DPHTbits.DPHT = VER_FRONT_PORCH + VER_PULSE_WIDTH + VER_BACK_PORCH + VER_RES;
    G1CON3bits.DPCLKPOL = 1;    /* Sample on rising edge*/
    G1CON3bits.DPENOE = 0;
    G1CON3bits.DPPWROE = 0;
    G1CON3bits.DPVSOE = 1;      /* use VSYNC */
    G1CON3bits.DPHSOE = 1;      /* use HSYNC */
    G1CON3bits.DPVSPOL = 0;     /* VSYNC negative polarity */
    G1CON3bits.DPHSPOL = 0;     /* HSYNC negative polarity */
    G1ACTDAbits.ACTLINE = VER_FRONT_PORCH + VER_PULSE_WIDTH + VER_BACK_PORCH;
    G1ACTDAbits.ACTPIX = HOR_FRONT_PORCH + HOR_PULSE_WIDTH + HOR_BACK_PORCH;
    G1VSYNCbits.VSST = VER_FRONT_PORCH;
    G1HSYNCbits.HSST = HOR_FRONT_PORCH;
    G1VSYNCbits.VSLEN = VER_PULSE_WIDTH;
    G1HSYNCbits.HSLEN = HOR_PULSE_WIDTH;
    G1DBLCONbits.VENST = VER_FRONT_PORCH + VER_PULSE_WIDTH + VER_BACK_PORCH;
    G1DBLCONbits.HENST = HOR_FRONT_PORCH + HOR_PULSE_WIDTH + HOR_BACK_PORCH;
    
    G1DPADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
    G1DPADRH = 0;

    G1CON2bits.DPBPP = 3;     /* 8bpp mode */
    G1CON1bits.PUBPP = 3;

    G1CON3bits.DPPINOE = 1;
    G1CON3bits.DPPOWER = 1;

    //G1CON2bits.DPTEST = 2;
    
    G1CON1bits.G1EN = 1;        /* Enable module */
    G1CON3bits.DPPOWER = 1;
    
    /*
    G1CLUTbits.CLUTEN = 1;

    G1CLUTbits.CLUTRWEN = 1;
    G1CLUTbits.CLUTADR = 0x00;
    G1CLUTWR = 0x001F;
    G1CLUTbits.CLUTADR = 0x01;
    G1CLUTWR = 0xF800;
    G1CLUTbits.CLUTADR = 0x02;
    G1CLUTWR = 0x07E0;

    G1CLUTbits.CLUTRWEN = 0;
    */
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

    uint32_t y;
    for (y = 1; y < GFX_DISPLAY_PIXEL_COUNT+4000; y++)
    {
        GFXDisplayBuffer[(unsigned long)(y)] = 0x00;
    }

    config_graphics();

    uint32_t x = 0;
    y=0;

    while (1) {
        y=0;

        for (y = 6528; y < 43008; y++)
        {
            y = y + 191;
                   //if(((y-6528)/192) % 4)
                   //{
                   //    GFXDisplayBuffer[(unsigned long)(y)] = 0x00;
                   // }
                   //else
                   //{

                   GFXDisplayBuffer[y+1] = (unsigned char)x;
                   GFXDisplayBuffer[y+2] = (unsigned char)x;
                   GFXDisplayBuffer[y+3] = (unsigned char)x;
                   GFXDisplayBuffer[y+4] = (unsigned char)x;
                   GFXDisplayBuffer[y+5] = (unsigned char)x;
                   GFXDisplayBuffer[y+6] = (unsigned char)x;
                   GFXDisplayBuffer[y+7] = (unsigned char)x;
                   GFXDisplayBuffer[y+8] = (unsigned char)x;
                   GFXDisplayBuffer[y+9] = (unsigned char)x;
                   GFXDisplayBuffer[y+10] = (unsigned char)x;
                   GFXDisplayBuffer[y+11] = (unsigned char)x;
                   GFXDisplayBuffer[y+11] = (unsigned char)x;
                   GFXDisplayBuffer[y+12] = (unsigned char)x;
                   GFXDisplayBuffer[y+13] = (unsigned char)x;
                   GFXDisplayBuffer[y+14] = (unsigned char)x;
                   GFXDisplayBuffer[y+15] = (unsigned char)x;
                   GFXDisplayBuffer[y+16] = (unsigned char)x;
                   GFXDisplayBuffer[y+17] = (unsigned char)x;
                   GFXDisplayBuffer[y+18] = (unsigned char)x;
                   GFXDisplayBuffer[y+19] = (unsigned char)x;
                   GFXDisplayBuffer[y+20] = (unsigned char)x;
                   GFXDisplayBuffer[y+21] = (unsigned char)x;
                   GFXDisplayBuffer[y+22] = (unsigned char)x;
                   GFXDisplayBuffer[y+23] = (unsigned char)x;
                   GFXDisplayBuffer[y+24] = (unsigned char)x;
                   GFXDisplayBuffer[y+25] = (unsigned char)x;
                   GFXDisplayBuffer[y+26] = (unsigned char)x;
                   GFXDisplayBuffer[y+27] = (unsigned char)x;
                   GFXDisplayBuffer[y+28] = (unsigned char)x;
                   GFXDisplayBuffer[y+29] = (unsigned char)x;
                   GFXDisplayBuffer[y+30] = (unsigned char)x;
                   GFXDisplayBuffer[y+31] = (unsigned char)x;
                   GFXDisplayBuffer[y+32] = (unsigned char)x;
                   GFXDisplayBuffer[y+33] = (unsigned char)x;
                   GFXDisplayBuffer[y+34] = (unsigned char)x;
                   GFXDisplayBuffer[y+35] = (unsigned char)x;
                   GFXDisplayBuffer[y+36] = (unsigned char)x;
                   GFXDisplayBuffer[y+37] = (unsigned char)x;
                   GFXDisplayBuffer[y+38] = (unsigned char)x;
                   GFXDisplayBuffer[y+39] = (unsigned char)x;
                   GFXDisplayBuffer[y+40] = (unsigned char)x;
                   GFXDisplayBuffer[y+41] = (unsigned char)x;
                   GFXDisplayBuffer[y+42] = (unsigned char)x;
                   GFXDisplayBuffer[y+43] = (unsigned char)x;
                   GFXDisplayBuffer[y+44] = (unsigned char)x;
                   GFXDisplayBuffer[y+45] = (unsigned char)x;
                   GFXDisplayBuffer[y+46] = (unsigned char)x;
                   GFXDisplayBuffer[y+47] = (unsigned char)x;
                   GFXDisplayBuffer[y+48] = (unsigned char)x;
                   GFXDisplayBuffer[y+49] = (unsigned char)x;
                   GFXDisplayBuffer[y+50] = (unsigned char)x;
                   GFXDisplayBuffer[y+51] = (unsigned char)x;
                   GFXDisplayBuffer[y+52] = (unsigned char)x;
                   GFXDisplayBuffer[y+53] = (unsigned char)x;
                   GFXDisplayBuffer[y+54] = (unsigned char)x;
                   GFXDisplayBuffer[y+55] = (unsigned char)x;
                   GFXDisplayBuffer[y+56] = (unsigned char)x;
                   GFXDisplayBuffer[y+57] = (unsigned char)x;
                   GFXDisplayBuffer[y+58] = (unsigned char)x;
                   GFXDisplayBuffer[y+59] = (unsigned char)x;
                   GFXDisplayBuffer[y+60] = (unsigned char)x;
                   GFXDisplayBuffer[y+61] = (unsigned char)x;
                   GFXDisplayBuffer[y+62] = (unsigned char)x;
                   GFXDisplayBuffer[y+63] = (unsigned char)x;
                   GFXDisplayBuffer[y+64] = (unsigned char)x;
                   GFXDisplayBuffer[y+65] = (unsigned char)x;
                   GFXDisplayBuffer[y+66] = (unsigned char)x;
                   GFXDisplayBuffer[y+67] = (unsigned char)x;
                   GFXDisplayBuffer[y+68] = (unsigned char)x;
                   GFXDisplayBuffer[y+69] = (unsigned char)x;
                   GFXDisplayBuffer[y+70] = (unsigned char)x;
                   GFXDisplayBuffer[y+71] = (unsigned char)x;
                   GFXDisplayBuffer[y+72] = (unsigned char)x;
                   GFXDisplayBuffer[y+73] = (unsigned char)x;
                   GFXDisplayBuffer[y+74] = (unsigned char)x;
                   GFXDisplayBuffer[y+75] = (unsigned char)x;
                   GFXDisplayBuffer[y+76] = (unsigned char)x;
                   GFXDisplayBuffer[y+77] = (unsigned char)x;
                   GFXDisplayBuffer[y+78] = (unsigned char)x;
                   GFXDisplayBuffer[y+79] = (unsigned char)x;
                   GFXDisplayBuffer[y+80] = (unsigned char)x;
                   GFXDisplayBuffer[y+81] = (unsigned char)x;
                   GFXDisplayBuffer[y+82] = (unsigned char)x;
                   GFXDisplayBuffer[y+83] = (unsigned char)x;
                   GFXDisplayBuffer[y+84] = (unsigned char)x;
                   GFXDisplayBuffer[y+85] = (unsigned char)x;
                   GFXDisplayBuffer[y+86] = (unsigned char)x;
                   GFXDisplayBuffer[y+87] = (unsigned char)x;
                   GFXDisplayBuffer[y+88] = (unsigned char)x;
                   GFXDisplayBuffer[y+89] = (unsigned char)x;
                   GFXDisplayBuffer[y+90] = (unsigned char)x;
                   GFXDisplayBuffer[y+91] = (unsigned char)x;
                   GFXDisplayBuffer[y+92] = (unsigned char)x;
                   GFXDisplayBuffer[y+93] = (unsigned char)x;
                   GFXDisplayBuffer[y+94] = (unsigned char)x;
                   GFXDisplayBuffer[y+95] = (unsigned char)x;
                   GFXDisplayBuffer[y+96] = (unsigned char)x;
                   GFXDisplayBuffer[y+97] = (unsigned char)x;
                   GFXDisplayBuffer[y+98] = (unsigned char)x;
                   GFXDisplayBuffer[y+99] = (unsigned char)x;

                   GFXDisplayBuffer[y+100] = (unsigned char)x;
                   GFXDisplayBuffer[y+101] = (unsigned char)x;
                   GFXDisplayBuffer[y+102] = (unsigned char)x;
                   GFXDisplayBuffer[y+103] = (unsigned char)x;
                   GFXDisplayBuffer[y+104] = (unsigned char)x;
                   GFXDisplayBuffer[y+105] = (unsigned char)x;
                   GFXDisplayBuffer[y+106] = (unsigned char)x;
                   GFXDisplayBuffer[y+107] = (unsigned char)x;
                   GFXDisplayBuffer[y+108] = (unsigned char)x;
                   GFXDisplayBuffer[y+109] = (unsigned char)x;
                   GFXDisplayBuffer[y+110] = (unsigned char)x;
                   GFXDisplayBuffer[y+111] = (unsigned char)x;
                   GFXDisplayBuffer[y+111] = (unsigned char)x;
                   GFXDisplayBuffer[y+112] = (unsigned char)x;
                   GFXDisplayBuffer[y+113] = (unsigned char)x;
                   GFXDisplayBuffer[y+114] = (unsigned char)x;
                   GFXDisplayBuffer[y+115] = (unsigned char)x;
                   GFXDisplayBuffer[y+116] = (unsigned char)x;
                   GFXDisplayBuffer[y+117] = (unsigned char)x;
                   GFXDisplayBuffer[y+118] = (unsigned char)x;
                   GFXDisplayBuffer[y+119] = (unsigned char)x;
                   GFXDisplayBuffer[y+120] = (unsigned char)x;
                   GFXDisplayBuffer[y+121] = (unsigned char)x;
                   GFXDisplayBuffer[y+122] = (unsigned char)x;
                   GFXDisplayBuffer[y+123] = (unsigned char)x;
                   GFXDisplayBuffer[y+124] = (unsigned char)x;
                   GFXDisplayBuffer[y+125] = (unsigned char)x;
                   GFXDisplayBuffer[y+126] = (unsigned char)x;
                   GFXDisplayBuffer[y+127] = (unsigned char)x;
                   GFXDisplayBuffer[y+128] = (unsigned char)x;
                   GFXDisplayBuffer[y+129] = (unsigned char)x;
                   GFXDisplayBuffer[y+130] = (unsigned char)x;
                   GFXDisplayBuffer[y+131] = (unsigned char)x;
                   GFXDisplayBuffer[y+132] = (unsigned char)x;
                   GFXDisplayBuffer[y+133] = (unsigned char)x;
                   GFXDisplayBuffer[y+134] = (unsigned char)x;
                   GFXDisplayBuffer[y+135] = (unsigned char)x;
                   GFXDisplayBuffer[y+136] = (unsigned char)x;
                   GFXDisplayBuffer[y+137] = (unsigned char)x;
                   GFXDisplayBuffer[y+138] = (unsigned char)x;
                   GFXDisplayBuffer[y+139] = (unsigned char)x;
                   GFXDisplayBuffer[y+140] = (unsigned char)x;
                   GFXDisplayBuffer[y+141] = (unsigned char)x;
                   GFXDisplayBuffer[y+142] = (unsigned char)x;
                   GFXDisplayBuffer[y+143] = (unsigned char)x;
                   GFXDisplayBuffer[y+144] = (unsigned char)x;
                   GFXDisplayBuffer[y+145] = (unsigned char)x;
                   GFXDisplayBuffer[y+146] = (unsigned char)x;
                   GFXDisplayBuffer[y+147] = (unsigned char)x;
                   GFXDisplayBuffer[y+148] = (unsigned char)x;
                   GFXDisplayBuffer[y+149] = (unsigned char)x;
                   GFXDisplayBuffer[y+150] = (unsigned char)x;
                   GFXDisplayBuffer[y+151] = (unsigned char)x;
                   GFXDisplayBuffer[y+152] = (unsigned char)x;
                   GFXDisplayBuffer[y+153] = (unsigned char)x;
                   GFXDisplayBuffer[y+154] = (unsigned char)x;
                   GFXDisplayBuffer[y+155] = (unsigned char)x;
                   GFXDisplayBuffer[y+156] = (unsigned char)x;
                   GFXDisplayBuffer[y+157] = (unsigned char)x;
                   GFXDisplayBuffer[y+158] = (unsigned char)x;
                   GFXDisplayBuffer[y+159] = (unsigned char)x;
                   GFXDisplayBuffer[y+160] = (unsigned char)x;
                   GFXDisplayBuffer[y+161] = (unsigned char)x;
                   GFXDisplayBuffer[y+162] = (unsigned char)x;
                   GFXDisplayBuffer[y+163] = (unsigned char)x;
                   GFXDisplayBuffer[y+164] = (unsigned char)x;
                   GFXDisplayBuffer[y+165] = (unsigned char)x;
                   GFXDisplayBuffer[y+166] = (unsigned char)x;
                   GFXDisplayBuffer[y+167] = (unsigned char)x;
                   GFXDisplayBuffer[y+168] = (unsigned char)x;
                   GFXDisplayBuffer[y+169] = (unsigned char)x;
                   GFXDisplayBuffer[y+170] = (unsigned char)x;
                   GFXDisplayBuffer[y+171] = (unsigned char)x;
                   GFXDisplayBuffer[y+172] = (unsigned char)x;
                   GFXDisplayBuffer[y+173] = (unsigned char)x;
                   GFXDisplayBuffer[y+174] = (unsigned char)x;
                   GFXDisplayBuffer[y+175] = (unsigned char)x;
                   GFXDisplayBuffer[y+176] = (unsigned char)x;
                   GFXDisplayBuffer[y+177] = (unsigned char)x;
                   GFXDisplayBuffer[y+178] = (unsigned char)x;
                   GFXDisplayBuffer[y+179] = (unsigned char)x;
                   GFXDisplayBuffer[y+180] = (unsigned char)x;
                   GFXDisplayBuffer[y+181] = (unsigned char)x;
                   GFXDisplayBuffer[y+182] = (unsigned char)x;
                   GFXDisplayBuffer[y+183] = (unsigned char)x;
                   GFXDisplayBuffer[y+184] = (unsigned char)x;
                   GFXDisplayBuffer[y+185] = (unsigned char)x;
                   GFXDisplayBuffer[y+186] = (unsigned char)x;
                   GFXDisplayBuffer[y+187] = (unsigned char)x;
                   GFXDisplayBuffer[y+188] = (unsigned char)x;
                   GFXDisplayBuffer[y+189] = (unsigned char)x;
                   GFXDisplayBuffer[y+190] = (unsigned char)x;
                   GFXDisplayBuffer[y+191] = (unsigned char)x;
                   //}
                   __delay_ms(1);
        }

        __delay_ms(10);
        x++;
        //G1CON2bits.DPTEST = 2;
    }

    return 0;
}







