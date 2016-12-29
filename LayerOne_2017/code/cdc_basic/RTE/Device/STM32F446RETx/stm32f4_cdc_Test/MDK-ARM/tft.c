#include "main.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#include "tft.h"

static const unsigned char chrimg[][7] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 20h SP
    0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, // 21h !
    0x00, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, // 22h "
    0x00, 0x28, 0x7c, 0x28, 0x7c, 0x28, 0x00, // 23h #
    0x10, 0x3c, 0x50, 0x38, 0x14, 0x78, 0x10, // 24h $
    0x00, 0x32, 0x54, 0x68, 0x16, 0x2a, 0x4c, // 25h %
    0x18, 0x24, 0x14, 0x38, 0x4e, 0x44, 0x3a, // 26h &
    0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, // 27h '
    0x04, 0x08, 0x10, 0x10, 0x10, 0x08, 0x04, // 28h (
    0x40, 0x20, 0x10, 0x10, 0x10, 0x20, 0x40, // 29h )
    0x10, 0x54, 0x38, 0xfe, 0x38, 0x54, 0x10, // 2Ah *
    0x10, 0x10, 0x10, 0xfe, 0x10, 0x10, 0x10, // 2Bh +
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, // 2Ch ,
    0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, // 2Dh -
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, // 2Eh .
    0x00, 0x08, 0x08, 0x10, 0x20, 0x20, 0x00, // 2Fh /

    0x38, 0x44, 0x4c, 0x54, 0x64, 0x44, 0x38, // 30h 0
    0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x38, // 31h 1
    0x38, 0x44, 0x04, 0x18, 0x20, 0x40, 0x7c, // 32h 2
    0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38, // 33h 3
    0x08, 0x18, 0x28, 0x48, 0x7c, 0x08, 0x08, // 34h 4
    0x7c, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38, // 35h 5
    0x38, 0x44, 0x40, 0x78, 0x44, 0x44, 0x38, // 36h 6
    0x7c, 0x44, 0x04, 0x08, 0x08, 0x10, 0x10, // 37h 7
    0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38, // 38h 8
    0x38, 0x44, 0x44, 0x3c, 0x04, 0x44, 0x38, // 39h 9
    0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, // 3Ah :
    0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x20, // 3Bh ;
    0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, // 3Ch <
    0x00, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x00, // 3Dh =
    0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, // 3Eh >
    0x38, 0x44, 0x44, 0x08, 0x10, 0x00, 0x10, // 3Fh >

    0x7c, 0x82, 0xba, 0xa6, 0xbc, 0x80, 0x7c, // 40h @
    0x38, 0x44, 0x44, 0x44, 0x7c, 0x44, 0x44, // 41h A
    0x78, 0x44, 0x44, 0x78, 0x44, 0x44, 0x78, // 42h B
    0x38, 0x44, 0x40, 0x40, 0x40, 0x44, 0x38, // 43h C
    0x70, 0x48, 0x44, 0x44, 0x44, 0x48, 0x70, // 44h D
    0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7c, // 45h E
    0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, // 46h F
    0x38, 0x44, 0x40, 0x4c, 0x44, 0x44, 0x38, // 47h G
    0x44, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x44, // 48h H
    0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, // 49h I
    0x3c, 0x08, 0x08, 0x08, 0x08, 0x48, 0x30, // 4Ah J
    0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, // 4Bh K
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c, // 4CH L
    0x44, 0x6c, 0x54, 0x54, 0x44, 0x44, 0x44, // 4DH M
    0x44, 0x64, 0x54, 0x54, 0x4c, 0x44, 0x44, // 4EH N
    0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, // 4FH O

    0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x40, // 50h P
    0x38, 0x44, 0x44, 0x44, 0x54, 0x48, 0x34, // 51h Q
    0x78, 0x44, 0x44, 0x78, 0x44, 0x44, 0x44, // 52h R
    0x38, 0x44, 0x40, 0x38, 0x04, 0x44, 0x38, // 53h S
    0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, // 54h T
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, // 55h U
    0x44, 0x44, 0x44, 0x44, 0x28, 0x28, 0x10, // 56h V
    0x44, 0x44, 0x54, 0x54, 0x54, 0x54, 0x28, // 57h W
    0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44, // 58h X
    0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, // 59h Y
    0x7c, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7c, // 5Ah Z
    0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1c, // 5Bh [
    0x00, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, // 5Ch back slash
    0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70, // 5Dh ]
    0x00, 0x10, 0x28, 0x00, 0x00, 0x00, 0x00, // 5Eh ^
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c  // 5Fh _
};

extern SPI_HandleTypeDef hspi1;

void spi_init ( void )
{
	/*
    SpiHandle.Instance               = SPI1;
    SpiHandle.Init.Mode              = SPI_MODE_MASTER;
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;

    if ( HAL_SPI_Init ( &SpiHandle ) != HAL_OK )
        while ( 1 );
	*/
}

/*-------------------------------------------------------------------*/
/*  Write a byte data                                                */
/*-------------------------------------------------------------------*/
void spi_write ( uint8_t data )
{
    HAL_SPI_Transmit ( &hspi1, &data, 1, 100 );
}

/*-------------------------------------------------------------------*/
/*  Write a word data                                                */
/*-------------------------------------------------------------------*/
void spi_writew ( uint16_t data )
{
    HAL_SPI_Transmit ( &hspi1, ( uint8_t * ) &data, 2, 100 );
}

void wait_ms ( uint16_t ms )
{
    HAL_Delay ( ms ) ;
}

void wait_us ( uint16_t us )
{
    while ( us-- );
}


/*-------------------------------------------------------------------*/
/*  Write command                                                    */
/*-------------------------------------------------------------------*/
void write_cmd ( uint8_t cmd )
{
    dc ( 0 );
    spi_write ( cmd );
}

/*-------------------------------------------------------------------*/
/*  Write data                                                       */
/*-------------------------------------------------------------------*/
void write_data ( uint8_t data )
{
    dc ( 1 );
    spi_write ( data );
}

/*-------------------------------------------------------------------*/
/*  TFT reset                                                        */
/*-------------------------------------------------------------------*/
void tft_reset()
{
    wait_ms ( 200 );
    cs ( 1 );
    dc ( 1 );
    rst ( 1 );
    wait_ms ( 200 );
    rst ( 0 );
    wait_us ( 10 );
    rst ( 1 );
    wait_ms ( 120 );
    cs ( 0 );
    wait_ms ( 10 );

    write_cmd ( 0x3A ); // Pixel Format
    write_data ( 0x55 ); // 16bit Color

    write_cmd ( 0xB1 ); // Frame Control
    write_data ( 0 );
    write_data ( 0x1f );

    write_cmd ( 0x36 ); // Memory Access Control
#ifdef OLD_BOARD
    write_data ( 0xE8 ); // MY MX MV BGR
#else
    write_data ( 0x28 ); // MY MX MV BGR
#endif

    write_cmd ( 0x11 ); // Sleep Out
    wait_ms ( 5 );

    write_cmd ( 0x29 ); // Display On
}

/*-------------------------------------------------------------------*/
/*  Set windows size, start memory write                             */
/*-------------------------------------------------------------------*/
void tft_set_window ( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 )
{
    write_cmd ( 0x2A ); // Column Address Set
    write_data ( x0 >> 8 );
    write_data ( x0 );
    write_data ( x1 >> 8 );
    write_data ( x1 );

    write_cmd ( 0x2B ); // Page Address Set
    write_data ( y0 >> 8 );
    write_data ( y0 );
    write_data ( y1 >> 8 );
    write_data ( y1 );

    write_cmd ( 0x2C ); // Memory Write

    wait_us ( 20 );

    dc ( 1 );
}

/*-------------------------------------------------------------------*/
/*  Clear screen                                                     */
/*-------------------------------------------------------------------*/
void tft_clear ( uint16_t color )
{
    tft_set_window ( 0, 0, TFT_WIDTH, TFT_HEIGHT );

    for ( int i = 0; i < TFT_WIDTH * TFT_HEIGHT; ++i ) {
        spi_writew ( color );
    }
}

/*-------------------------------------------------------------------*/
/*  Put char                                                         */
/*-------------------------------------------------------------------*/
void tft_put_char ( int x, int y, char chr, uint16_t color, uint16_t bgcolor )
{
    if ( chr < 0x20 || chr > 0x7f ) {
        chr = 0x3f;

    } else {
        chr = ( chr < 0x60 ) ? chr - 0x20 : chr - 0x40;
    }

    tft_set_window ( x, y, x + 7, y + 6 );

    for ( int dy = 0; dy < 7; ++dy ) {
        unsigned char img = chrimg[chr][dy];

        for ( int dx = 0; dx < 8; ++dx ) {
            if ( img & 0x80 ) {
                spi_writew ( color );

            } else {
                spi_writew ( bgcolor );
            }

            img <<= 1;
        }
    }

}

/*-------------------------------------------------------------------*/
/*  Text out                                                         */
/*-------------------------------------------------------------------*/
void tft_text ( int x, int y, char *text, uint16_t color, uint16_t bgcolor )
{
    while ( *text != 0 ) {
        tft_put_char ( x, y, *text, color, bgcolor );
        x += 8;
        text++;
    }
}

/*-------------------------------------------------------------------*/
/*  Horizontal Line                                                        */
/*-------------------------------------------------------------------*/
void tft_hline ( int x1, int y, int x2, uint16_t color )
{
    tft_set_window ( x1, y, x2, y );

    for ( ; x1 < x2; ++x1 ) {
        spi_writew ( color );
    }
}

/*-------------------------------------------------------------------*/
/*  Vertical Line                                                        */
/*-------------------------------------------------------------------*/
void tft_vline ( int x, int y1, int y2, uint16_t color )
{
    tft_set_window ( x, y1, x, y2 );

    for ( ; y1 < y2; ++y1 ) {
        spi_writew ( color );
    }
}

/*-------------------------------------------------------------------*/
/*  Box                                                              */
/*-------------------------------------------------------------------*/
void tft_box ( int x1, int y1, int x2, int y2, uint16_t color )
{
    tft_hline ( x1, y1, x2, color );
    tft_vline ( x1, y1, y2, color );
    tft_vline ( x2, y1, y2, color );
    tft_hline ( x1, y2, x2, color );
}

/*-------------------------------------------------------------------*/
/*  Box Fill                                                        */
/*-------------------------------------------------------------------*/
void tft_boxfill ( int x1, int y1, int x2, int y2, uint16_t color )
{
    tft_set_window ( x1, y1, x2, y2 );

    for ( int i = 0; i < ( x2 - x1 + 1 ) * ( y2 - y1 + 1 ); ++i ) {
        spi_writew ( color );
    }
}

/*-------------------------------------------------------------------*/
/*  Draw 4bit BMP                                                    */
/*-------------------------------------------------------------------*/
uint8_t draw_bmp_4bpp ( const unsigned char *imgdata, int x, int y )
{
    BITMAPFILEHEADER *bf = ( BITMAPFILEHEADER * ) imgdata;
    BITMAPINFOHEADER *bi = ( BITMAPINFOHEADER * ) ( imgdata + sizeof ( BITMAPFILEHEADER ) );

    if ( bi->biBitCount != 4 ) {
        return 0;
    }

    unsigned char *pRGBPal = ( unsigned char* ) imgdata + sizeof ( BITMAPFILEHEADER ) + bi->biSize;
    unsigned short palette[16];

    for ( int i = 0; pRGBPal < imgdata + bf->bfOffBits && i < 16; ++i ) {
        unsigned short r, g, b;
        b = *pRGBPal++ >> 3;
        g = *pRGBPal++ >> 2;
        r = *pRGBPal++ >> 3;
        pRGBPal++;
        palette[i] = ( ( g & 7 ) << 13 ) | ( b << 8 ) | ( r << 3 ) | ( g >> 3 );
    }

    unsigned short HLine[320];
    int linesize = ( bi->biWidth / 2 + 3 ) & 0xfffc;

    tft_set_window ( x, y, x + bi->biWidth - 1, y + bi->biHeight - 1 );

    unsigned char *bmp;

    for ( int y = bi->biHeight - 1; y >= 0; --y ) {
        bmp = ( unsigned char * ) imgdata + bf->bfOffBits + y * linesize;

        for ( int x = 0; x < bi->biWidth; ++x ) {
            char pal;

            if ( x & 1 ) {
                pal = *bmp & 0xf;
                bmp++;

            } else {
                pal = *bmp >> 4;
            }

            HLine[x] = palette[pal];
        }

        HAL_SPI_Transmit ( &hspi1, ( uint8_t * ) HLine, bi->biWidth * 2, 100 );
    }

    return 1;
}

/*-------------------------------------------------------------------*/
/*  Initialize TFT                                                   */
/*-------------------------------------------------------------------*/
void tft_init ( void )
{
    back ( 1 );
    spi_init();
    tft_reset();
}

///////////////////////////////////////////////////////
// TFT 9341 + touchscreen 8 bit version



