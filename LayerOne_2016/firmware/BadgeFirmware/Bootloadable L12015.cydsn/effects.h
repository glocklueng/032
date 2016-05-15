/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef _EFFECTS_H_
	#define _EFFECTS_H_ 1
	
#include <project.h>

typedef union rgb_t { 
    
	uint32_t rgb;
    
	struct rgb_tag { 
        uint8_t g,r,b,pad;
    } c;
	
} led_color;

typedef union hsv_t { 
    
	uint32_t hsv;
    
	struct hsv_tag { 
        uint8_t h,s,v,pad;
    } h;
	
} hsv_color;

typedef uint8_t   fract8;   // ANSI: unsigned short _Fract
typedef int8_t    sfract7;  // ANSI: signed   short _Fract
typedef uint16_t  fract16;  // ANSI: unsigned       _Fract
typedef int16_t   sfract15; // ANSI: signed         _Fract


#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t
typedef DWORD   COLORREF;

#undef RGB

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(g)|((WORD)((BYTE)(r))<<8))|(((DWORD)(BYTE)(b))<<16)))

#define BOOT_CHECK() if( Boot_P0_7_Read ( ) == 0 )   CySoftwareReset();
led_color HeatColor ( uint8_t temperature );
uint8_t scale8_video ( uint8_t i, fract8 scale );
led_color ColdColor ( uint8_t temperature );

uint32 TweenerALLHSV( uint16_t count, uint32 from,uint32 to,uint32 delay);
uint32 TweenerHSV( uint16_t startx,  uint16_t count, uint32 from,uint32 to,uint32 delay,int direction);
void Stripe(void);
void SingleLEDPingPong( uint16_t count , uint8 fade_amount, uint32 c1);
void CandyCane ( uint16_t count , uint32 c1, uint32 c2 );
void Snake( uint16_t count );
void Twinkle( uint16_t count );
void ColorWheel( uint16_t count );
void Tweener( uint16_t count ,uint32 source);
void ColorFader( int count  , uint32 color);
void Tween1( void );
void Icicle (uint8 redraw, uint8 length, int fade_amount );           
void Sparkler ( uint16 runtime, int fade_amount , int num_sparkles ,char white );
void CandyCaneSmooth ( uint16_t count , led_color c1, led_color c2 );
void SingleLEDPing( uint16_t count , uint8 fade_amount, uint32 color);

uint32 TweenC1toC2(led_color c1, led_color c2,int amount);
int TweenC1toC2Range( uint16_t count, uint16_t x, uint32 source, uint32 target);
hsv_color RgbToHsv(led_color rgb);
led_color HsvToRgb(hsv_color hsv);
void StripLights_PixelHSV(int32 x,int32 y,hsv_color hsv );
void StripLights_DisplayClearHSV(hsv_color hsv );
led_color hsv_to_rgb(hsv_color hsv);
hsv_color rgb_to_hsv(led_color rgb);
uint8 TweenU8toU8(uint8 source, uint8 target,int amount);
void FadeToColor( uint16_t startx, uint16_t count, uint32 target, uint32 delay, int direction) ;
void FadeStrip(  uint16 start, int16 length ,int percentage );
uint32 AddColor( led_color c1, led_color c2 );
void Falloff(uint16_t count);
void Sparky(uint16_t count);
void Fire2012 ( int cold );
void Fire2012a ( int cold );
void Larson1(int count  );
void Meet(uint16_t count);

uint8_t at_parse(const char* data, const char* data_expected);
uint16_t at_getstr( char *const buffer ,uint16_t length,uint16_t timeout);
/* [] END OF FILE */

#endif
