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
#include "stm32f4xx_hal_spi.h"

#define dc(x) HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, (GPIO_PinState)x)
#define cs(x) HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin,(GPIO_PinState) x)
#define rst(x) HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, (GPIO_PinState)x)
#define back(x) HAL_GPIO_WritePin(TFT_BACKLIGHT_GPIO_Port, TFT_BACKLIGHT_Pin, (GPIO_PinState)x)

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


#pragma pack(2)
typedef struct tagBITMAPFILEHEADER {
  unsigned short bfType;
  unsigned long  bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned long  bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
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
typedef struct tagBMP24{
    unsigned char B;
    unsigned char G;
    unsigned char R;
} BMP24;
#pragma pack()

void tft_init(void);
void tft_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void tft_clear(uint16_t color);
void tft_put_char(int x, int y, char chr, uint16_t color, uint16_t bgcolor);
void tft_text(int x, int y, char *text, uint16_t color, uint16_t bgcolor);
void tft_hline(int x1, int y, int x2, uint16_t color);
void tft_vline(int x, int y1, int y2, uint16_t color);
void tft_box(int x1, int y1, int x2, int y2, uint16_t color);
void tft_boxfill(int x1, int y1, int x2, int y2, uint16_t color);
uint8_t draw_bmp_4bpp(const unsigned char *imgdata, int x, int y);

#endif

