/*===================================================================*/
/*                                                                   */
/*  tft.cpp : TFT(ILI9341) function                                  */
/*                                                                   */
/*  2016/1/20  Racoon                                                */
/*                                                                   */
/*===================================================================*/

#include "mbed.h"
#include "tft.h"
// rev 1
//#define OLD_BOARD
// shield
// #define MOPHO_BOARD

//standalone
#define STANDALONE_BOARD

#ifdef OLD_BOARD
DigitalOut cs ( PB_5, PullUp ); // TFT chipselect pin
#else
DigitalOut cs ( PC_7, PullUp ); // TFT chipselect pin
#endif

DigitalOut dc ( PA_8, PullUp ); // TFT data command select pin
//mopho
#if defined( MOPHO_BOARD) 
DigitalOut rst ( PA_2, PullUp ); // TFT reset pin
#endif
// standalone
#if defined( STANDALONE_BOARD) 
DigitalOut rst ( PC_8, PullUp ); // TFT reset pin
#endif

static DigitalOut back ( PC_9 );

/*-------------------------------------------------------------------*/
/*  Write command                                                    */
/*-------------------------------------------------------------------*/
void write_cmd ( uint8_t cmd )
{
    dc = 0;
    spi_write ( cmd );
}

/*-------------------------------------------------------------------*/
/*  Write data                                                       */
/*-------------------------------------------------------------------*/
void write_data ( uint8_t data )
{
    dc = 1;
    spi_write ( data );
}


uint8_t read_diag(void)
{
	uint8_t status;
	
	write_cmd ( 0xf );
	// dummy read
	spi_read();
	status = spi_read();
	if( status == 0 ) {
		return 0;
	} 
	return 1;
	
}


/*-------------------------------------------------------------------*/
/*  TFT reset                                                        */
/*-------------------------------------------------------------------*/
void tft_reset()
{
    // reset
    rst = 0;
    wait_ms ( 200 );
    rst = 1;
    // delay for RST
    wait_ms ( 200 );


    // reset ili9341
    write_cmd ( 0x1 );
    wait_ms ( 500 );

    cs = 1;
    dc = 1;

    wait_ms ( 200 );
    rst = 0;
    wait_us ( 10 );
    rst = 1;
    wait_ms ( 120 );
    cs = 0;
    wait_ms ( 10 );

    write_cmd ( 0x3A ); // Pixel Format
    write_data ( 0x55 ); // 16bit Color

    write_cmd ( 0xB1 ); // Frame Control
    write_data ( 0 );
    write_data ( 0x1f );
    // 11101000b e8
    // 00101000b 28
    unsigned char mac = 0x28;//(0xe8 + (1 << 7));
    write_cmd ( 0x36 ); // Memory Access Control
    write_data ( mac ); // MY MX MV BGR

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

    dc = 1;
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
bool draw_bmp_4bpp ( const unsigned char *imgdata, int x, int y )
{
    BITMAPFILEHEADER *bf = ( BITMAPFILEHEADER * ) imgdata;
    BITMAPINFOHEADER *bi = ( BITMAPINFOHEADER * ) ( imgdata + sizeof ( BITMAPFILEHEADER ) );

    if ( bi->biBitCount != 4 ) {
        return false;
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

        HAL_SPI_Transmit ( &SpiHandle, ( uint8_t * ) HLine, bi->biWidth * 2, 100 );
    }

    return true;
}


void backlight ( int status )
{
    back = 1;
}
/*-------------------------------------------------------------------*/
/*  Initialize TFT                                                   */
/*-------------------------------------------------------------------*/
void tft_init ( void )
{
    backlight ( 1 );

    spi_init();

    tft_reset();
	
	//read_diag();
	
}




