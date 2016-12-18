/*===================================================================*/
/*                                                                   */
/*  tft.h : TFT(ILI9341) function header                             */
/*                                                                   */
/*  2016/1/20  Racoon                                                */
/*                                                                   */
/*===================================================================*/

#ifndef TFT_H
#define TFT_H

#include "stm32f4xx_hal.h"
#include "spidma.h"

#define TFT_WIDTH   320
#define TFT_HEIGHT  240

#define TFT_WHITE 0xFFFF
#define TFT_WHITESMOKE 0xBFFF
#define TFT_GHOSTWHITE 0xDFFF
#define TFT_ALICEBLUE 0xDFF7
#define TFT_LAVENDAR 0x5FEF
#define TFT_AZURE 0xFFF7
#define TFT_LIGHTCYAN 0xFFE7
#define TFT_MINTCREAM 0xFFFF
#define TFT_HONEYDEW 0xFEF7
#define TFT_IVORY 0xFEFF
#define TFT_BEIGE 0xBCFF
#define TFT_LIGHTYELLOW 0xFCFF
#define TFT_LIGHTGOLDENRODYELLOW 0xFAFF
#define TFT_LEMONCHIFFON 0xFAFF
#define TFT_FLORALWHITE 0xFEFF
#define TFT_OLDLACE 0xBDFF
#define TFT_CORNSILK 0xDCFF
#define TFT_PAPAYAWHITE 0x9BFF
#define TFT_BLANCHEDALMOND 0x7AFF
#define TFT_BISQUE 0x39FF
#define TFT_SNOW 0xFFFF
#define TFT_LINEN 0x9DFF
#define TFT_ANTIQUEWHITE 0x7BFF
#define TFT_SEASHELL 0xBEFF
#define TFT_LAVENDERBLUSH 0x9FFF
#define TFT_MISTYROSE 0x3CFF
#define TFT_GAINSBORO 0xFCE6
#define TFT_LIGHTGRAY 0xBAD6
#define TFT_LIGHTSTEELBLUE 0x3CB6
#define TFT_LIGHTBLUE 0xDDB6
#define TFT_LIGHTSKYBLUE 0x9F8E
#define TFT_POWDERBLUE 0x1DB7
#define TFT_PALETURQUOISE 0x9EB7
#define TFT_SKYBLUE 0x9D8E
#define TFT_MEDIUMAQUAMARINE 0x756E
#define TFT_AQUAMARINE 0xFB87
#define TFT_PALEGREEN 0xF39F
#define TFT_LIGHTGREEN 0x9297
#define TFT_KHAKI 0x52F7
#define TFT_PALEGOLDENROD 0x55F7
#define TFT_MOCCASIN 0x37FF
#define TFT_NAVAJOWHITE 0x16FF
#define TFT_PEACHPUFF 0xF7FE
#define TFT_WHEAT 0x16FF
#define TFT_PINK 0x19FE
#define TFT_LIGHTPINK 0xD8FD
#define TFT_THISTLE 0x1BDE
#define TFT_PLUM 0x1CE5
#define TFT_SILVER 0x18C6
#define TFT_DARKGRAY 0x55AD
#define TFT_LIGHTSLATEGRAY 0x537C
#define TFT_SLATEGRAY 0x1274
#define TFT_SLATEBLUE 0xFA6A
#define TFT_STEELBLUE 0x374C
#define TFT_MEDIUMSLATEBLUE 0x5E7B
#define TFT_ROYALBLUE 0x5C43
#define TFT_BLUE 0x1F00
#define TFT_DODGERBLUE 0x9F24
#define TFT_CORNFLOWERBLUE 0xBE6C
#define TFT_DEEPSKYBLUE 0x1F06
#define TFT_CYAN 0xFF07
#define TFT_AQUA 0xFF07
#define TFT_TURQUOISE 0x1A47
#define TFT_MEDIUMTURQUOISE 0x9A4E
#define TFT_DARKTURQUOISE 0x9A06
#define TFT_LIGHTSEAGREEN 0xB525
#define TFT_MEDIUMSPRINGGREEN 0xF307
#define TFT_SPRINGGREEN 0xF007
#define TFT_LIME 0xE007
#define TFT_LIMEGREEN 0x6636
#define TFT_YELLOWGREEN 0x669E
#define TFT_LAWNGREEN 0xE087
#define TFT_CHARTREUSE 0xE087
#define TFT_GREENYELLOW 0xE6B7
#define TFT_YELLOW 0xE0FF
#define TFT_GOLD 0xC0FE
#define TFT_ORANGE 0x20FD
#define TFT_DARKORANGE 0x60FC
#define TFT_GOLDENROD 0x24DD
#define TFT_BURLYWOOD 0xD1E5
#define TFT_TAN 0xB2D5
#define TFT_SANDYBROWN 0x2CFD
#define TFT_DARKSALMON 0xCFEC
#define TFT_LIGHTCORAL 0x10F4
#define TFT_SALMON 0xEFC
#define TFT_LIGHTSALMON 0xFFD
#define TFT_CORAL 0xAFC
#define TFT_TOMATO 0x29FB
#define TFT_ORANGERED 0x20FA
#define TFT_RED 0xF8
#define TFT_DEEPPINK 0xB2F8
#define TFT_HOTPINK 0x57FB
#define TFT_PALEVIOLETRED 0x92DB
#define TFT_VIOLET 0x3EF4
#define TFT_ORCHID 0x9BDB
#define TFT_MAGENTA 0x1FF8
#define TFT_FUCHSIA 0x1FF8
#define TFT_MEDIUMORCHID 0xBABA
#define TFT_DARKORCHID 0xBA99
#define TFT_DARKVIOLET 0x1A98
#define TFT_BLUEVIOLET 0x7C89
#define TFT_MEDIUMPURPLE 0x9B93
#define TFT_GRAY 0x1084
#define TFT_MEDIUMBLUE 0x1A00
#define TFT_DARKCYAN 0x7104
#define TFT_CADETBLUE 0x1465
#define TFT_DARKSEAGREEN 0xF295
#define TFT_MEDIUMSEAGREEN 0xAE45
#define TFT_TEAL 0x1004
#define TFT_FORESTGREEN 0x6424
#define TFT_SEAGREEN 0x6B34
#define TFT_DARKKHAKI 0xCDC5
#define TFT_PERU 0x28D4
#define TFT_CRIMSON 0xA8E0
#define TFT_INDIANRED 0xECD2
#define TFT_ROSYBROWN 0x92C4
#define TFT_MEDIUMVIOLETRED 0xB1C8
#define TFT_DIMGRAY 0x4D6B
#define TFT_BLACK 0x0
#define TFT_MIDNIGHTBLUE 0xCE18
#define TFT_DARKSLATEBLUE 0xF149
#define TFT_DARKBLUE 0x1100
#define TFT_NAVY 0x1000
#define TFT_DARKSLATEGRAY 0x8A32
#define TFT_GREEN 0x4
#define TFT_DARKGREEN 0x2003
#define TFT_DARKOLIVEGREEN 0x665B
#define TFT_OLIVEDRAB 0x846C
#define TFT_OLIVE 0x84
#define TFT_DARKGOLDENROD 0x41BC
#define TFT_CHOCOLATE 0x44D3
#define TFT_SIENNA 0xA6A2
#define TFT_SADDLEBROWN 0x228A
#define TFT_FIREBRICK 0x24B1
#define TFT_BROWN 0x65A9
#define TFT_MAROON 0x80
#define TFT_DARKRED 0x88
#define TFT_DARKMAGENTA 0x1188
#define TFT_PURPLE 0x1080
#define TFT_INDIGO 0x1048

const unsigned char chrimg[][7] = {
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

#pragma pack(2)
typedef struct tagBITMAPFILEHEADER {
    unsigned short bfType;
    unsigned long  bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long  bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    unsigned long  biSize;
    long           biWidth;
    long           biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long  biCompression;
    unsigned long  biSizeImage;
    long           biXPixPerMeter;
    long           biYPixPerMeter;
    unsigned long  biClrUsed;
    unsigned long  biClrImporant;
} BITMAPINFOHEADER;
#pragma pack()

#pragma pack(1)
typedef struct tagBMP24 {
    unsigned char B;
    unsigned char G;
    unsigned char R;
} BMP24;
#pragma pack()

void tft_init();
void tft_set_window ( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 );
void tft_clear ( uint16_t color );
void tft_put_char ( int x, int y, char chr, uint16_t color, uint16_t bgcolor );
void tft_text ( int x, int y, char *text, uint16_t color, uint16_t bgcolor );
void tft_hline ( int x1, int y, int x2, uint16_t color );
void tft_vline ( int x, int y1, int y2, uint16_t color );
void tft_box ( int x1, int y1, int x2, int y2, uint16_t color );
void tft_boxfill ( int x1, int y1, int x2, int y2, uint16_t color );
bool draw_bmp_4bpp ( const unsigned char *imgdata, int x, int y );
void backlight ( int status );

uint8_t read_diag(void);

#endif

