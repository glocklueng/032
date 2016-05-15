/******************************************************************************
* Project Name		: Bootloadable Blinking LED
* File Name			: main.c
* Version 			: 1.0
* Device Used		: CY8C4245AXI-483
* Software Used		: PSoC Creator 3.0
* Compiler    		: ARMGCC 4.7.3, ARM RVDS Generic, ARM MDK Generic
* Related Hardware	: CY8CKIT-049-42xx PSoC 4 Prototyping Kit 
*
********************************************************************************
* Copyright (2014), Cypress Semiconductor Corporation. All Rights Reserved.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*******************************************************************************/

/******************************************************************************
*                           THEORY OF OPERATION
* This is a blinking LED project. A PWM component drives the pin to blink the 
* LED at regular intervals. This project contains a bootloadable component so 
* that it can be bootloaded into PSoC 4 which has a bootloader already programmed 
* into it.
* Default UART Port Configuration for bootloading the PSoC 4 in CY8CKIT-049-42xx
* Baud Rate : 115200 bps
* Data Bits : 8
* Stop Bits : 1
* Parity    : None
******************************************************************************/

#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <device.h>
#include <math.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <effects.h>

//#define REVERSE_DIRECTION ( 1 )


/***************************************
*Global variables
***************************************/

extern const uint32 StripLights_CLUT[ ];
extern uint32  StripLights_ledArray[StripLights_ROWS][StripLights_COLUMNS];
void echo_uart(void);

void run_server(void);

uint32_t GetRand(int size)
{
	return getColor(rand() % size);

}

void Select(unsigned char ch)
{
    int a;
    
//	srand(clock());

	//printf("%c - ", ch);
	switch (ch) {
	case 'a':
		//printf("Icicle\n");
		Icicle(2, 9, 6);
		break;

	case 'b':
		//printf("Sparkler RGB\n");
		Sparkler(1000, 10, 18, 0);
		break;

	case 'c':
		//printf("Sparkler White\n");
		Sparkler(1000, 1, 18, 1);
		break;

	case 'd':
        {

		//printf("ColorFader Loop\n");
		StripLights_DisplayClear(getColor(rand() % StripLights_COLOR_WHEEL_SIZE));

		for (a = 0; a < 5; a++) {
			ColorFader(1, getColor(rand() % StripLights_COLOR_WHEEL_SIZE));
		}
        }
        
		break;

	case 'e':
	    //printf("SingleLEDPingPong\n");
		SingleLEDPing(1, 5, getColor(rand() % 74));
		break;

	case 'f':
		//printf("StripLights_BLUE\n");
		StripLights_DisplayClear(StripLights_BLUE);
		break;

	case 'g':
		//printf("SingleLEDPingPong\n");
		SingleLEDPingPong(1, 15, getColor(rand() % 74));

		break;

	case 'h':
		//printf("Tween1\n");
		Tween1();

		break;

	case 'i':
		//printf("ColorFader\n");
		ColorFader(1, getColor(rand() % StripLights_COLOR_WHEEL_SIZE));

		break;

	case 'j':
		//printf("CandyCane\n");
		CandyCane(40, StripLights_RED, StripLights_WHITE);

		break;

	case 'k':
		//printf("Snake\n");
		Snake(50);
		break;

	case 'l':
		//printf("ColorWheel\n");

		for (a = 0; a < 10; a++) {
			ColorWheel(100);
		}

		break;

	case 'm':
		//printf("CandyCane R\n");

		CandyCane(5, getColor(rand() % 74), StripLights_WHITE);
		
		break;

	case 'n':
        {
		led_color c1, c2;

		//printf("CandyCaneSmooth\n");

		c1.rgb = 0; c2.rgb = 0;

		while (c1.rgb == c2.rgb) {
			c1.rgb = getColor(rand() % StripLights_COLOR_WHEEL_SIZE);
			c2.rgb = getColor(rand() % StripLights_COLOR_WHEEL_SIZE);
		}

		CandyCaneSmooth(5, c1, c2);
        }
        
		break;

	case 'o':
		//printf("Fire2012 h\n");
		break;
		for (a = 0; a < 550; a++) {
			Fire2012(0);
			CyDelay(1000 / 60);
		}

		break;

	case 'p':
		//printf("Fire2012 c\n");

		for (a = 0; a < 200; a++) {
			Fire2012(1);
			CyDelay(1000 / 60);
		}

		break;

	case 'q':
		//printf("Fire2012a c\n");

		for (a = 0; a < 200; a++) {
			Fire2012a(0);
			CyDelay(1000 / 60);
		}

		break;

	case 'r':
		//printf("Meet(5)\n");
		Meet(5);
		break;

	case 's':
		//printf("Larson1\n");
		Larson1(StripLights_MAX_X);
		break;
	case 't':
		//printf("TweenerALLHSV\n");
		TweenerALLHSV(6, GetRand(72), GetRand(72), 20);
		break;

	case 'u':
		//printf("TweenerHSV\n");
		TweenerHSV(0, StripLights_MAX_X, GetRand(72), GetRand(72), 20, 1);
		break;

	case 'v':
		//printf("Sparky\n");
		Sparky(200);
		break;

	case 'w':
		//printf("Falloff\n");
		Falloff(100);
		break;


	case 'x': {
    float ii,i;
		//printf("ping pong fader\n");
		for (ii = 1; ii < 128.0f; ii+=15)
			for ( i = 0; i < 3; i+=0.1f) {
				float val;
				val = (sin(i)*ii);
				StripLights_DisplayClear(RGB(val,( val), val));
                CyDelay(25);
			}

		for (a = 0; a < 100; a++) {
			FadeStrip(StripLights_MIN_X, StripLights_MAX_X+1, a);
			StripLights_Trigger(1);
              CyDelay(25);
		}
		break;
	}

	case 'y':
    {
		float y,x,val;
		for (y = 0; y < StripLights_MAX_X; y++) {
			for (x = 0; x < StripLights_MAX_X+1; x += 1.0f) {
				val = (1 + sin((45.0f + (x +y) ) / 10))*128.0f;
				StripLights_Pixel(x, 0, RGB(val, (val), val));
			}
			StripLights_Trigger(1);
                CyDelay(25);
		}
		for (a = 0; a < 100; a++) {
			FadeStrip(StripLights_MIN_X, StripLights_MAX_X+1, a);
			StripLights_Trigger(1);
                CyDelay(25);
		}

		break;
    }
	case ' ':
		//printf ( "StripLights_BLACK\n" );
		StripLights_DisplayClear ( 0 );
		break;


	case '1':
		//printf("StripLights_RED\n");
		StripLights_DisplayClear(StripLights_RED);
		break;

	case '2':
		//printf("StripLights_GREEN\n");
		StripLights_DisplayClear(StripLights_GREEN);
		break;

	case '3':

		//printf("StripLights_BLUE\n");
		StripLights_DisplayClear(StripLights_BLUE);
		break;

	case '0':
		StripLights_Trigger(1);
		break;

		//brg
	case '4':
		//printf("StripLights_RED\n");
		StripLights_DisplayClear(RGB(0,0xff,0));
		break;

	case '5':
		//printf("StripLights_GREEN\n");
		StripLights_DisplayClear(RGB(0,0,0xff));
		break;
	case '6':
		//printf("StripLights_BLUE\n");
		StripLights_DisplayClear(RGB(0xff, 0, 0));
		break;

	case '9':
		CySoftwareReset();
		break;
    }
}

int main()
{

    StripLights_Start();
	

	/* Start UART component and clear the TX,RX buffers */
	UART_Start();
	UART_SpiUartClearRxBuffer();

    // Setting for ESP8266 enable */
	CH_PD_SetDriveMode(CH_PD_DM_STRONG ) ;
        
    // ESP8266 off 
    CH_PD_Write(0);

	// start wifi for esp wifi
	uWIFI_Start();
	uWIFI_SpiUartClearRxBuffer();
   
	CyGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */

    // LED
	P1_6_SetDriveMode(P1_6_DM_STRONG ) ;
    
    // LED
	P1_6_Write(1);

    // diode is not bypassed for LED's
    // dim 2   40 - 60mA @ 3V
    // dim 3   30 - 40mA @ 3V
    // dim 4   20mA      @ 3V
    // dim 4   20 - 40mA @ 5V
    
    StripLights_Dim( 2 );
    
    // detect ESP8266, seems to work.
    if(GPIO2_Read() ){
        // detected
        StripLights_DisplayClear( StripLights_ORANGE );	CyDelay( 500 ); 
    	run_server();
    }else{
       StripLights_DisplayClear( StripLights_RED_MASK );	CyDelay( 500 ); 
	}
    
    StripLights_DisplayClear( StripLights_GREEN_MASK );	CyDelay( 500 ); 
	
    StripLights_DisplayClear( StripLights_BLUE_MASK );	CyDelay( 500 ); 
    
	StripLights_DisplayClear( 0 );

	// test uarts , use to talk directly to the esp from the psoc UART pins add usb to serial adapter.
	//echo_uart();
    
    // if gets to here WiFi didn't connect, or isn't present so turn it off.
    CH_PD_Write(0);
    
	while(1) {
		static int selected, lastselected;
		{	
			do {
				selected = 'a' + (rand() % ('z'-'a'));

			} while (selected == lastselected);

	
			Select (selected);

			lastselected = selected;
		}
	}

       //reset board
    CySoftwareReset();
    
    return 0;
}
/*
	AT+CIFSR
	AT+CIPMUX=1
	AT+CIPSERVER=1,40002
	AT+CIPSTO=9000
	AT+CIPMUX=0
*/

#define MAX_BUFFER		( (StripLights_COLUMNS * 3) + 20 )

char rx_buffer[ MAX_BUFFER ];

uint8_t send_command( const char *const text, const char *const command, const char *const wait, uint16_t timeout) 
{
	int length;

	if( text ){
		UART_UartPutString( text );
	}
	if( command) { 
		UART_UartPutString( command);
		uWIFI_UartPutString( command);
	}
	
	
	// if wait supplied, scan buffer for command
	if ( wait ) {
	
		while( 1 ) {
			
			// attempt to get a string (needs timeout)
			length = at_getstr(rx_buffer,MAX_BUFFER,50);
			
			timeout -- ;
			
			// return 0 on timed out
			if( timeout == 0 ) {
				if( 0 ) UART_UartPutString("\nTimed out waiting for correct reply\n");
				return 0;
			}
			// got a return
			if( length ) {
				
				if ( 0 ) {
					
					UART_UartPutString( "buff = [\n");
					UART_UartPutString( rx_buffer);		
					UART_UartPutString( "]\n");
				}
				
				// look for string
				if(strstr( rx_buffer, wait ) != NULL ) {
					if( 0 ) UART_UartPutString("\nfound - [");
					if( 0 ) UART_UartPutString( rx_buffer ) ;
					// found
					break;
				}
			} else {
				
				if ( 0 ) {
					UART_UartPutString("\nEmpty buffer\n");
				}
			}
		}
	}
	
	while( uWIFI_SpiUartGetRxBufferSize() ) {
		
		UART_UartPutChar( uWIFI_UartGetChar() );	
	}
	
	
	while( uWIFI_SpiUartGetRxBufferSize() ) {
		
		UART_UartPutChar( uWIFI_UartGetChar() );
	}
	
	return 1;
	
}

#define	DEFAULT_TIMEOUT	( 50 )

// stream mode
// AT+CIPMODE=1 and then AT+CIPSEND,

void StripLights_SetXToColour ( uint32 from, unsigned short length ) 
{
	int x;
	
	length = MIN(StripLights_MAX_X,length);
	
	 for(x = StripLights_MIN_X; x <= length ; x++) {
		StripLights_Pixel(x,0,from);
	}
	
    //send to LED strip
    while( StripLights_Ready() == 0);
	
    StripLights_Trigger(1);
	
}

/**
 * @brief Does all the work of getting data, simple parse and sending to LED's
 *
 * @return none
 */
void run_server(void)
{
	uint8_t *buf_ptr;
	
	uint8_t ret = 0;
    
    uint8_t x = 0, error_count;
    
    // esp8266 on
    CH_PD_Write(1);
	
	//Set to black
	StripLights_DisplayClear(0);
	
	// LED off
	P1_6_Write(0);

	// this caused all sorts of issues, so i removed it
#if 0
	while( ret == 0 ) {
		
		ret = send_command("resetting\r\n", "AT+RST\r\n\n","ready",5000); //.com on later firmware, ready on others
	}
	P1_6_Write(0);
#endif
	

	// Simple progress meter
	StripLights_SetXToColour( getColor(1) ,2  );	
		
	// returns "no change" , 1 CONNECT TO AP, 2 BE AN AP, 3 BOTH
	if( send_command("cwmode=3\r\n", "AT+CWMODE=1\r\n",NULL,DEFAULT_TIMEOUT) == 0 ) {
        return;
    }
    x = 1;
	
    error_count = 5;
    
	do {
    	// Simple progress meter, stage 2
    	StripLights_SetXToColour( getColor(5) ,x  );		
        x++;
        
		// LED On
		P1_6_Write(1);
		
		// Not really used, can be used to see if already connected
		send_command("get ip\r\n","AT+CIFSR\r\n",NULL,0);
		CyDelay(400);
		
		// wireless AP settings, first param is ap name, second password
		ret =send_command("connecting\r\n","AT+CWJAP=\"badgenet\",\"badgenet\"\r\n","OK",1000);

		// LED Off		
		P1_6_Write(0);
    
        error_count--;
        if (error_count == 0 ) 
            return;
        
        
	}while( ret == 0 );

    x = 1;

    error_count = 10;
    
	do {
    	// Simple progress meter, stage 3
    	StripLights_SetXToColour( getColor(6) ,x  );		
        x++;
        
		CyDelay(400);
		ret= send_command("check connection\r\n","AT+CWJAP?\r\n","OK",DEFAULT_TIMEOUT); 
        
        error_count--;
        if (error_count == 0 ) 
            return;

    } while( ret == 0 );

    x = 1;
    error_count = 10;
    
	//GET LOCAL IP ADDRESS
	do {
    	// Simple progress meter, stage 4
    	StripLights_SetXToColour( getColor(9) ,x  );		
        x++;
		CyDelay(400);
		ret= send_command("get ip\r\n","AT+CIFSR\r\n",NULL,0); 
        
        
        error_count--;
        if (error_count == 0 ) 
            return;

        
	} while( ret == 0 );

    x = 1;
    
	//START UP MULTI-IP CONNECTION
	// 	0 Single IP connection
	// 	1 Multi IP connection
	do {
    	// Simple progress meter, stage 4
    	StripLights_SetXToColour( getColor(11) ,x  );		
        x++;

        CyDelay(400);
		ret= send_command("multip\r\n","AT+CIPMUX=1\r\n","OK",DEFAULT_TIMEOUT);
	} while( ret == 0 );

    x = 1;
    
	do {
    	// Simple progress meter, stage 4
    	StripLights_SetXToColour( getColor(6) ,x  );		
        x++;

        CyDelay(400);
		ret= send_command("cipserver\r\n","AT+CIPSERVER=1,40002\r\n","OK",DEFAULT_TIMEOUT);
	} while( ret == 0 );
	
	// progress meter, stage 7
    x = 1;
	
	// switch into UDP listen/receive mode, all data passed in will be of +IDT,0,length:data format

	do {
    	// Simple progress meter, stage 7
    	StripLights_SetXToColour( getColor(14) ,x  );		
        x++;
		CyDelay(400);
		ret= send_command("cipsto\r\n","AT+CIPSTO=9000\r\n","OK",DEFAULT_TIMEOUT);
	} while( ret == 0 );

	// progress meter, stage 8
    x = 1;

	do {
    	// Simple progress meter, stage 4
    	StripLights_SetXToColour( getColor(16) ,x  );		
        x++;
        
		CyDelay(400);
		ret= send_command("cipmux\r\n","AT+CIPMUX=0\r\n","OK",DEFAULT_TIMEOUT);
	} while( ret == 0 );

		
	// progress meter, stage 9

    // Simple progress meter, stage 9
    StripLights_SetXToColour( getColor(20) , StripLights_MAX_X/2 );		
		
	// setup done, tell host (if connected)
	if( 0 ) UART_UartPutString("\nSetup and ready!\n");
	
	// progress meter, stage 10, done
	StripLights_SetXToColour( getColor(21) ,StripLights_MAX_X );	

	CyDelay(200);

	// all off
	StripLights_DisplayClear(0);

	while(1) { 
		int i ;
		uint8_t ch;
		
		// if switch is pressed, run into bootloader , mostly for dev
		BOOT_CHECK();

		//led off
		P1_6_Write(0);

		// wait for data from ESP UART
		while ( uWIFI_SpiUartGetRxBufferSize() == 0 );
		
		
		// fetch one byte of data
		ch = uWIFI_UartGetChar();

		
		// find start of +IPD,0,450:
		if( ch == '+' ) {

			//wait, this could be set to < 4 instead and then can drop the other checks.
			while ( uWIFI_SpiUartGetRxBufferSize() == 0 );
			
			ch = uWIFI_UartGetChar();
			
			if( ch == 'I' ) {
				
				while ( uWIFI_SpiUartGetRxBufferSize() == 0 );
				ch = uWIFI_UartGetChar();			
			
				if( ch == 'P' ) {	
					
					while ( uWIFI_SpiUartGetRxBufferSize() == 0 );
					ch = uWIFI_UartGetChar();
					
					if( ch == 'D' ) {	
						
						while ( uWIFI_SpiUartGetRxBufferSize() == 0 );
						ch = uWIFI_UartGetChar();

						//UART_UartPutString("Found +IPD\n");
						
						// illformatted
						if( ch != ',' )  {
							if( 0 ) UART_UartPutString("Unexpected char #1\n");
							break;
						}
						
                        // LED on
						P1_6_Write(1);
						
						// scan for end of descriptive
						// 10 will be enough 0,450:
						i = 10 ;
						do {
							while ( uWIFI_SpiUartGetRxBufferSize() == 0 );
							ch = uWIFI_UartGetChar();
							i--;
							if( i ==0 ) {
								if( 0 ) UART_UartPutString("couldn't find : marker\n");
								break;
							}								
						} while( ch != ':' );
						
						//UART_UartPutString("Found Start of data block\n");

						// point to start or end of LED buffer
#if defined(REVERSE_DIRECTION)	
						buf_ptr = (uint8_t*)&StripLights_ledArray[0][StripLights_MAX_X-1];
						for( i = 0 ; i <= StripLights_MAX_X ; i++  ) {
#else
						buf_ptr = (uint8_t*)&StripLights_ledArray[0][0];
						for( i = 0 ; i <= StripLights_MAX_X ; i++  ) {
#endif

// fill in rx_buffer from ESP UART
						//gbr
	
							while ( uWIFI_SpiUartGetRxBufferSize() < 3 );
	
							buf_ptr[1] = uWIFI_UartGetChar();
							buf_ptr[2] = uWIFI_UartGetChar();	
							buf_ptr[0] = uWIFI_UartGetChar();
							
#if defined(REVERSE_DIRECTION)
							buf_ptr -= sizeof(uint32_t);
#else
							buf_ptr += sizeof(uint32_t);
#endif
						}


						//end of buffer
						while ( uWIFI_SpiUartGetRxBufferSize() == 0 );
						ch = uWIFI_UartGetChar();
						
						// check this char for sanity if wanted
						/*
						UART_UartPutChar( ch) ;
						UART_UartPutString(" - Buffer filled\n");
						UART_UartPutString( rx_buffer );
						UART_UartPutString("END\n");
						*/

						//send to LED strip
		   				while( StripLights_Ready() == 0);
						StripLights_Trigger(1);
						
						//CyDelay(4);
						BOOT_CHECK();		
					}
				}
			}
		}
	}
}
void echo_uart(void)
{
	while(1) {
		
		
		BOOT_CHECK();

		
		int ret = 0;
		
		while( ret == 0 ) {
		
		ret = send_command("resetting\r\n", "AT+RST\r\n\n","ready",5000); //.com on later firmware, ready on others
    	}

			
		// echo from usb uart to wifi uart
		if( UART_SpiUartGetRxBufferSize() ) {
			
			uWIFI_UartPutChar( UART_UartGetChar() );
		}
		
		
		//echo from wifi uart to usb uart
		if( uWIFI_SpiUartGetRxBufferSize() ) {
			
			UART_UartPutChar( uWIFI_UartGetChar() );
		}
	}
}

void ColorFader( int count , uint32 color) 
{
	while(count--){
		FadeToColor( 0,StripLights_COLUMNS, color, 50,1 );
	}
}

void Tween1( void )
{
	hsv_color tween;
	static led_color src;
	static hsv_color result ;
	
	src.c.r = rand()%255;
    src.c.g = rand()%255;
    src.c.b = rand()%255;

	tween = rgb_to_hsv((led_color)getColor(rand()%StripLights_COLOR_WHEEL_SIZE));
	
	result.hsv = TweenerHSV(
		0,
		StripLights_COLUMNS,
		src.rgb,
		tween.hsv,
		10
		,1);
	
	// Tweener( 100,src.rgb );

	src.c.r += 5-(rand()%10);
    src.c.g += 5-(rand()%10);
    src.c.b += 5-(rand()%10);

	result.hsv = TweenerHSV(
		StripLights_COLUMNS,
		StripLights_COLUMNS,
		result.hsv,
		tween.hsv,
		10
		,-1
	);
		
}

void CandyCane ( uint16_t count , uint32 c1, uint32 c2 )
{
    int i,x;
    uint8_t flip =0;
	
    // Candy cane
	
	// loop effect for this many times
    for( i=0; i < count ; i++ )
    {   
		
        // all strip, for every other led
        for(x = StripLights_MIN_X; x <= StripLights_MAX_X; x+=2)
        {
			// if flipped. draw c1,c2 otherwise c2,c1
            if( flip ) {
                StripLights_Pixel(x, 0, c1);
                StripLights_Pixel(x+1, 0, c2);
            } else {
                StripLights_Pixel(x+1, 0, c1);
                StripLights_Pixel(x, 0, c2);
            }
        }

		// toggle flip
        flip = 1 - flip;
        
		// wait and trigger
        while( StripLights_Ready() == 0);
		StripLights_Trigger(1);
		
		// delay between transitions 
       	CyDelay( 100 );        
		
        BOOT_CHECK();
    }   
}

/*
 * FadeLED - Tween one LED to a specified colour
 *
 */
void FadeLED( uint16 i, uint32 target, int percentage)
{
		led_color trgb;

		trgb.rgb = StripLights_GetPixel(i,0);
		
		trgb.rgb = TweenC1toC2( trgb, (led_color)target, percentage ) ;

		StripLights_Pixel( i, 0, trgb.rgb );
}
		
void CandyCaneSmooth ( uint16_t count , led_color c1, led_color c2 )
{
    int i,x,percentage;
    uint8_t flip =0;
	uint32 t1,t2;
	
    // Candy cane
	if (0 ) {
		char buffer[256];
		//printf(buffer,"c1 = %02x %02x %02x\n",c1.c.r,c1.c.g,c1.c.b);
		UART_UartPutString( buffer );
		//printf(buffer,"c2 = %02x %02x %02x\n",c2.c.r,c2.c.g,c2.c.b);
		UART_UartPutString( buffer );
	}
	
			
	// loop effect for this many times
    for( i=0; i < count ; i++ )
    {   
		
		for( percentage = 0 ; percentage <= 100 ; percentage+=5 ) { 
			
			//  calculate target colours
			t1 = TweenC1toC2( c1, c2, percentage ) ;
			t2 = TweenC1toC2( c2, c1, percentage ) ;
						
	        // all strip, for every other led
	        for(x = StripLights_MIN_X; x <= StripLights_MAX_X; x+=2)
	        {
				// if flipped. draw c1,c2 otherwise c2,c1
	            if( flip ) {
	                StripLights_Pixel(x, 0, t1);
	                StripLights_Pixel(x+1, 0, t2);
	            } else {
	                StripLights_Pixel(x, 0, t2);
	                StripLights_Pixel(x+1, 0, t1);
	            }
	        }

			// toggle flip
	        flip = 1 - flip;
	        
			// wait and trigger
	        while( StripLights_Ready() == 0);
			StripLights_Trigger(1);
			
			
			// delay between transitions 
	       	CyDelay( 120 );        
			
	        BOOT_CHECK();
		}
    }   
}
void SingleLEDPing( uint16_t count , uint8 fade_amount, uint32 color) 
{
 
    int i;
    static int x;
	
    i = 0;
    
    	while(1)
        {
			if(fade_amount ) {
				// Fade  strip	
				FadeStrip( StripLights_MIN_X, StripLights_MAX_X , fade_amount );
			} else { 
				StripLights_MemClear(0);
			}
		
            
            StripLights_Pixel(x%(StripLights_MAX_X+1), 0, color);
            
            while( StripLights_Ready() == 0);
	    	StripLights_Trigger(1);
            
            CyDelay( 20 );        
            
            BOOT_CHECK();
            i++ ;
            x++;
            if ( i > count*(StripLights_MAX_X+1) ) break;

        }
}

void SingleLEDPingPong( uint16_t count , uint8 fade_amount, uint32 color) 
{
	int i,x;
	
    for( i=0; i < count ; i++ )
    {   
    	for(x = StripLights_MIN_X; x <= StripLights_MAX_X; x++)
        {
			if(fade_amount ) {
				// Fade  strip	
				FadeStrip( StripLights_MIN_X, StripLights_MAX_X , fade_amount );
			} else { 
				StripLights_MemClear(0);
			}
		
            
            StripLights_Pixel(x, 0, color);
            
            while( StripLights_Ready() == 0);
	    	StripLights_Trigger(1);
            CyDelay( 50 );        
            BOOT_CHECK();
        }
        
        for(x = StripLights_MIN_X; x <= StripLights_MAX_X; x++)
        {
			if(fade_amount ) {
				// Fade  strip	
				FadeStrip( StripLights_MIN_X, StripLights_MAX_X , fade_amount );
			} else { 
				StripLights_MemClear(0);
			}
			
            StripLights_Pixel(StripLights_MAX_X-x, 0, color);
            
            while( StripLights_Ready() == 0);
	    	StripLights_Trigger(1);
			
            CyDelay( 50 );        
			
            BOOT_CHECK();
        }
    }
}

// snake tail chaser
void Snake( uint16_t count )
{
	int i,x;        
    uint32 startColor;
     
    startColor = StripLights_RED;

    for(x = StripLights_MIN_X+1; x <= StripLights_MAX_X; x++)
    {            
        
        if( x & 6)
	        for(i = StripLights_MIN_X; i <= StripLights_MAX_X; i++)
	        {            
	            uint32_t colour = StripLights_GetPixel(i, 0);
	            StripLights_Pixel(i, 0, colour/2);
	        }        
        
        StripLights_Pixel(x, 0, startColor);
        
        while( StripLights_Ready() == 0);
    	StripLights_Trigger(1);
        CyDelay( 50 );        

        if( x % 10 == 5 ) startColor+=0x010101;
        
        BOOT_CHECK();
    }
}

void Twinkle( uint16_t count ) 
{
        int i,x;
        led_color col;
        uint32 startColor;
        startColor = StripLights_WHITE;
            
        
        for(x = 0; x <= count; x++)
        {            

			col.c.r = rand();
			col.c.g = rand();
			col.c.b = rand();

			startColor = col.rgb;
                       
			StripLights_Pixel(rand()%StripLights_MAX_X, 0, startColor);
			
            for(i = StripLights_MIN_X; i <= StripLights_MAX_X; i++)
            {            
                col.rgb = StripLights_GetPixel(i, 0);
                
                if ( col.c.r > 0 ) col.c.r -= col.c.r/2; 
                if ( col.c.g > 0 ) col.c.g -= col.c.g/2; 
                if ( col.c.b > 0 ) col.c.b -= col.c.b/2; 
                
                StripLights_Pixel(i, 0, col.rgb );
            }        
      
            while( StripLights_Ready() == 0);
	    	StripLights_Trigger(1);
            CyDelay( 15 );        
            
            BOOT_CHECK();
        }
        
}

void FadeStrip(  uint16 start, int16 length ,int percentage )
{
	led_color trgb;
	int i;
	
    for(i = start; i <= start+length; i++) {
    
		// get pixel
		trgb.rgb = StripLights_GetPixel(i,0);
		
		trgb.rgb = TweenC1toC2( trgb,(led_color) StripLights_BLACK, percentage ) ;

		StripLights_Pixel( i, 0, trgb.rgb );
	}
}

//shortcut for generate a sparkle
inline uint8 calculate_sparkle( register uint8 i ) 
{
	register uint8 rnd;

	// every so often return very dark, to make it sparkle
	if ( rand() % 20 == 10 ) {
		return 3;
	}
	
	// pick value from hole range
	rnd = ( rand() % 255 );

	// scale down by level
	rnd /= ( i + 1 ) ;
	
	// scale down again if near end
	if ( i > 4 ) rnd /= 2;
	if ( i > 6 ) rnd /= 2;
	
	return rnd;
}

void Icicle (uint8 redraw, uint8 length, int fade_amount )           
 {
    int x,j,i;
	led_color temp;

	// for entire length of strip, plus engough to move it off the display)
	for(x = StripLights_MIN_X; x <= StripLights_MAX_X + ( length * 2 ); x++)
	{
		
		if(fade_amount ) {
			// Fade  strip	
			FadeStrip( StripLights_MIN_X, StripLights_MAX_X , fade_amount );
		} else { 
			StripLights_MemClear(0);
		}
		
		
		// draw in same place 8 times
		for ( j = 0 ; j < redraw ;j++ ){
			
			// length of icicle
			for(i=0; i < length; i++)
			{
				// caculate a randow twink based on current position in length
				temp.c.r =
				temp.c.g =
				temp.c.b = calculate_sparkle( i  );

				// draw a pixel at x+i
			    StripLights_Pixel(x+i, 0, temp.rgb );
				
				CyDelay( 1 );  
		 	}    
			    
		    // strip ready?
			while( StripLights_Ready() == 0);
			
			//push current data to led strip
		    StripLights_Trigger(1);
		    CyDelay( 15 );        
		}
		
		// check if firmware load requested
	    if( Boot_P0_7_Read ( ) == 0 )   CySoftwareReset();
	}
}



void Sparkler ( uint16 runtime, int fade_amount , int num_sparkles ,char white ) 
{
    int x,j;
	led_color temp;

	// length of time to run
	for(x = 0; x <= runtime ; x++)
	{
		if(fade_amount ) {
			// Fade  strip	
			FadeStrip( StripLights_MIN_X, StripLights_MAX_X , fade_amount );
		} else { 
			StripLights_MemClear(0);
		}
		 
		
		// draw in same place 8 times
		for ( j = 0 ; j < num_sparkles ;j++ ){
						
			temp.c.r = calculate_sparkle( j );
			
			if (white ) { 
				temp.c.g = temp.c.b = temp.c.r;
			} else {
				temp.c.g = calculate_sparkle( j );
				temp.c.b = calculate_sparkle( j );
			}
				
			// draw a pixel 
			StripLights_Pixel(rand()%StripLights_MAX_X, 0, temp.rgb );
		}
		
	    // strip ready?
		while( StripLights_Ready() == 0);
		
		//push current data to led strip
	    StripLights_Trigger(1);
	    CyDelay( 15 );     
	}
	
	if( fade_amount ) {
		// fade at end
		for(x = 0; x <= 200 ; x++)
		{
			// Fade  strip	
			FadeStrip( StripLights_MIN_X, StripLights_MAX_X , fade_amount );

			// strip ready?
			while( StripLights_Ready() == 0);

			//push current data to led strip
			StripLights_Trigger(1);
			CyDelay( 3 );     
		}
	}		
}

void ColorWheel( uint16_t count )
{
	static int i =0xAAA ,x;
	static uint32 color;
	static uint32 startColor ;   

    
	if (i >= count ) {
		i = 0;
	}
	
	if ( i == 0xaaa ) {
		i = 0;
		color = 0;
		startColor = 0;
	}
	
    for( ; i < count ; i++ )
    {   
        color = startColor;
        for(x = StripLights_MIN_X; x <= StripLights_MAX_X; x++)
        {
            StripLights_Pixel(x, 0, getColor( color ));

        	 color++;
            
			if(color >= StripLights_COLOR_WHEEL_SIZE) color = 0;
       	}
		
		startColor++;
    
		if(startColor >= StripLights_COLOR_WHEEL_SIZE) startColor = 0;
    
       while( StripLights_Ready() == 0);
    
		StripLights_Trigger(1);
	
       CyDelay( 50 );

        BOOT_CHECK();
    }
}

void Tweener( uint16_t count, uint32 from) 
{
    int i,newx;
    led_color src, target;

	src.rgb  = from;

	newx = 0;
    
    for( i = 0 ; i < count; i ++ )
    {
       //StripLights_MemClear( 0 );
            
        target.c.r += 5-(rand()%10);
        target.c.g += 5-(rand()%10);
        target.c.b += 5-(rand()%10);

        newx = TweenC1toC2Range(StripLights_COLUMNS/2,newx,src.rgb,target.rgb);
		newx = TweenC1toC2Range(StripLights_COLUMNS/2,newx,target.rgb,src.rgb);
        
       while( StripLights_Ready() == 0); 
   	   StripLights_Trigger(1); CyDelay( 50 );
    
    }
	
}

void FadeToColor ( uint16_t startx, uint16_t count, uint32_t target, uint32_t delay, int direction )
{
	int j,i;
	int offset,oldoffset;

	led_color frgb,trgb;
	hsv_color  src,target_hsv,result;

	frgb.rgb = target;

	src = rgb_to_hsv ( frgb ) ;

	offset = startx;

	for ( j = 0 ; j < 100 ; j ++ ) {

		oldoffset = offset;

		for ( i = 0 ; i < count; i ++ ) {

			// get colour of current LED at offset
			trgb.rgb = StripLights_GetPixel ( offset,0 );

			// convert current led color to hsv
			target_hsv = rgb_to_hsv ( trgb );

			result = target_hsv;
			// tween, what we want to  what it is at percentage i
			result.h.h = TweenU8toU8(target_hsv.h.h, src.h.h, j);
			result.h.s = TweenU8toU8(target_hsv.h.s, src.h.s, j);
			result.h.v = TweenU8toU8(target_hsv.h.v, src.h.v, j);

			// update pixel
			StripLights_PixelHSV ( offset, 0, result );

			// handle travel direction of pixel
			offset += direction;

			if ( offset < ( int ) StripLights_MIN_X ) {
				offset = startx ;
			}

			if ( offset > startx+count ) {
				offset = StripLights_MIN_X ;
			}

		}

		// check bootloader mode
		BOOT_CHECK();

		// if wants a delay, update led strip and delay
		if ( delay ) {
			while ( StripLights_Ready() == 0 );

			StripLights_Trigger ( 1 );
			CyDelay ( delay );
		}

		offset = oldoffset;

	}
}

uint32 TweenerHSV( uint16_t startx, uint16_t count, uint32 from,uint32 to,uint32 delay,int direction) 
{
    int i;
	int offset;
	led_color frgb,trgb;
    hsv_color  src, target,result;

	trgb.rgb = to;
	frgb.rgb = from;
	
	src = rgb_to_hsv( frgb ) ;
	target = rgb_to_hsv( trgb );

	result = src;
	
	offset = startx;
	
    for( i = 1 ; i <= count+1; i ++ )
    {
        result.h.h = TweenU8toU8(src.h.h, target.h.h, i);
        
      	StripLights_PixelHSV(offset, 0, result );
		
		offset += direction;
		
		if (offset < (int)StripLights_MIN_X ) offset = startx ;
		if (offset > startx+count ) offset = StripLights_MIN_X ;
      	
        BOOT_CHECK();    
		
		if(delay) {
   			while( StripLights_Ready() == 0);

			StripLights_Trigger(1);
			CyDelay( delay );
		}
		
    }
		    
   while( StripLights_Ready() == 0);

	StripLights_Trigger(1);

   CyDelay( 100 );


	return result.hsv;
}

uint32 TweenerALLHSV( uint16_t count, uint32 from,uint32 to,uint32 delay) 
{
    int i;
	
	led_color frgb,trgb;
    hsv_color  src, target,result;

	trgb.rgb = to;
	frgb.rgb = from;
	
	src = rgb_to_hsv( frgb ) ;
	target = rgb_to_hsv( trgb );

	result = src;
	
    for( i = 1 ; i < count; i ++ )
    {
        result.h.h = TweenU8toU8(src.h.h, target.h.h, i);
        
      	StripLights_DisplayClearHSV( result );
		
		if(delay){
			CyDelay( delay );
		}
		
        BOOT_CHECK();    
    }
	return result.hsv;
}

// quick helper function for testing hsv/rgb.
void StripLights_PixelHSV(int32 x,int32 y,hsv_color hsv )
{
	led_color rgb;
	
	rgb = hsv_to_rgb( hsv ) ;
	
	StripLights_Pixel( x,y,rgb.rgb);
}
// quick helper function for testing hsv/rgb.
void StripLights_DisplayClearHSV(hsv_color hsv )
{
	led_color rgb;
	
	rgb = hsv_to_rgb( hsv ) ;
	
	StripLights_DisplayClear( rgb.rgb);
}

void Falloff(uint16_t count)
{
	int x, y, k;
	uint32_t startColor = 0;
	uint32_t startColor2 = 0;


	for (k = 0; k < 5; k++) {
		y = 1;

		startColor = 0;
		startColor2 = 0;

		while (startColor == startColor2) {
			startColor = GetRand(24);// StripLights_RED;
			startColor2 = GetRand(24);// StripLights_RED;
		}

		for (x = y - 1; x <= y; x++) {


			StripLights_Pixel(x, 0, startColor);
			FadeStrip(x - 5, 10, 10);

			StripLights_Pixel(StripLights_MAX_X - x, 0, startColor2);
			FadeStrip((StripLights_MAX_X - (x - 5)), 10, 10);

			while (StripLights_Ready() == 0);

			StripLights_Trigger(1);
			CyDelay(25);

			y++;

			if (y > StripLights_MAX_X + 20) {
				break;
			}
		}
	}
}
// Fire2012 by Mark Kriegsman, July 2012
// as pat of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


uint8_t qadd8 ( uint8_t i, uint8_t j )
{
	int t = i + j;

	if ( t > 255 ) {
		t = 255;
	}

	return t;
}


// qsub8: subtract one byte from another, saturating at 0x00
uint8_t qsub8 ( uint8_t i, uint8_t j )
{
	int t = i - j;

	if ( t < 0 ) {
		t = 0;
	}

	return t;
}

// scale8: scale one byte by a second one, which is treated as
//         the numerator of a fraction whose denominator is 256
//         In other words, it computes i * (scale / 256)
//         4 clocks AVR, 2 clocks ARM
uint8_t scale8 ( uint8_t i, fract8 scale )
{
	return
	    ( ( int ) i * ( int ) ( scale ) ) >> 8;
}


//  The "video" version of scale8 guarantees that the output will
//  be only be zero if one or both of the inputs are zero.  If both
//  inputs are non-zero, the output is guaranteed to be non-zero.
//  This makes for better 'video'/LED dimming, at the cost of
//  several additional cycles.
uint8_t scale8_video ( uint8_t i, fract8 scale )
{
	uint8_t j = ( ( ( int ) i * ( int ) scale ) >> 8 ) + ( ( i&&scale ) ?1:0 );
	// uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
	// uint8_t j = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
	return j;
}

#define RAND16_2053  2053
#define RAND16_13849 13849
#define RAND16_SEED  1337
uint16_t rand16seed = RAND16_SEED;


uint8_t random8l ( void )
{
	rand16seed = ( rand16seed * RAND16_2053 ) + RAND16_13849;
	return ( uint8_t ) rand16seed;
}

uint16_t random16()
{
	rand16seed = ( rand16seed * RAND16_2053 ) + RAND16_13849;
	return rand16seed;
}


uint8_t random8 ( uint8_t lim )
{
	uint8_t r = random8l();
	r = scale8 ( r, lim );
	return r;
}

uint8_t random8a ( uint8_t min, uint8_t lim )
{
	uint8_t delta = lim - min;
	uint8_t r = random8 ( delta ) + min;
	return r;
}

#define NUM_LEDS    (StripLights_MAX_X+1)

// CRGB HeatColor( uint8_t temperature)
//
// Approximates a 'black body radiation' spectrum for
// a given 'heat' level.  This is useful for animations of 'fire'.
// Heat is specified as an arbitrary scale from 0 (cool) to 255 (hot).
// This is NOT a chromatically correct 'black body radiation'
// spectrum, but it's surprisingly close, and it's fast and small.
//
// On AVR/Arduino, this typically takes around 70 bytes of program memory,
// versus 768 bytes for a full 256-entry RGB lookup table.

led_color HeatColor ( uint8_t temperature )
{
	led_color heatcolor;

	// Scale 'heat' down from 0-255 to 0-191,
	// which can then be easily divided into three
	// equal 'thirds' of 64 units each.
	uint8_t t192 = scale8_video ( temperature, 192 );

	// calculate a value that ramps up from
	// zero to 255 in each 'third' of the scale.
	uint8_t heatramp = t192 & 0x3F; // 0..63
	heatramp <<= 2; // scale up to 0..252

	// now figure out which third of the spectrum we're in:
	if ( t192 & 0x80 ) {
		// we're in the hottest third
		heatcolor.c.r = 255; // full red
		heatcolor.c.g = 255; // full green
		heatcolor.c.b = heatramp; // ramp up blue

	}

	else if ( t192 & 0x40 ) {
		// we're in the middle third
		heatcolor.c.r = 255; // full red
		heatcolor.c.g = heatramp; // ramp up green
		heatcolor.c.b = 0; // no blue

	}

	else {
		// we're in the coolest third
		heatcolor.c.r = heatramp; // ramp up red
		heatcolor.c.g = 0; // no green
		heatcolor.c.b = 0; // no blue
	}

	return heatcolor;
}

led_color ColdColor ( uint8_t temperature )
{
	led_color heatcolor;

	// Scale 'heat' down from 0-255 to 0-191,
	// which can then be easily divided into three
	// equal 'thirds' of 64 units each.
	uint8_t t192 = scale8_video ( temperature, 192 );

	// calculate a value that ramps up from
	// zero to 255 in each 'third' of the scale.
	uint8_t heatramp = t192 & 0x3F; // 0..63
	heatramp <<= 2; // scale up to 0..252

	// now figure out which third of the spectrum we're in:
	if ( t192 & 0x80 ) {
		// we're in the hottest third
		heatcolor.c.r = 255; // full red
		heatcolor.c.g = 255; // full green
		heatcolor.c.b = heatramp; // ramp up blue

	}

	else if ( t192 & 0x40 ) {
		// we're in the middle third
		heatcolor.c.r = 0; // full red
		heatcolor.c.g = 128; // ramp up green
		heatcolor.c.b = heatramp; // no blue

	}

	else {
		// we're in the coolest third
		heatcolor.c.r = 0; // ramp up red
		heatcolor.c.g = 0; // no green
		heatcolor.c.b = heatramp; // no blue
	}

	return heatcolor;
}


void Fire2012 ( int cold )
{
    int i,j,k;
    
	// Array of temperature readings at each simulation cell
	static uint8_t heat[NUM_LEDS];

	// Step 1.  Cool down every cell a little
	for (  i = 0; i < NUM_LEDS; i++ ) {
		heat[i] = qsub8 ( heat[i],  random8a ( 0, ( ( COOLING * 10 ) / NUM_LEDS ) + 2 ) );
	}

	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (  k= NUM_LEDS - 1; k >= 2; k-- ) {
		heat[k] = ( heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if ( random8l() < SPARKING ) {
		int y = random8 ( 7 );
		heat[y] = qadd8 ( heat[y], random8a ( 160,255 ) );
	}

	// Step 4.  Map from heat cells to LED colors
	for (  j = 0; j < NUM_LEDS; j++ ) {
		if ( cold ) {
			StripLights_Pixel ( j,0, ColdColor ( heat[j] ).rgb );
		}

		else {
			StripLights_Pixel ( j,0, HeatColor ( heat[j] ).rgb );
		}
	}

	while ( StripLights_Ready() == 0 );

	StripLights_Trigger ( 1 );
	CyDelay ( 16 );
}

void Fire2012a ( int cold )
{
	// Array of temperature readings at each simulation cell
	static uint8_t heat[NUM_LEDS];

    int i,j,k;
    
	// Step 1.  Cool down every cell a little
	for (  i = 0; i < NUM_LEDS; i++ ) {
		heat[i] = qsub8 ( heat[i],  random8a ( 0, ( ( COOLING * 10 ) / NUM_LEDS ) + 2 ) );
	}

	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (  k= NUM_LEDS - 3; k > 0; k-- ) {
		heat[k] = ( heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if ( random8l() < SPARKING ) {
		int y = random8 ( 7 );
		heat[y] = qadd8 ( heat[y], random8a ( 160,255 ) );
	}


	// Step 4.  Map from heat cells to LED colors
	for (  j = 0; j < NUM_LEDS; j++ ) {
		if ( cold ) {
			StripLights_Pixel ( j,0, ColdColor ( heat[j] ).rgb );
		}

		else {
			StripLights_Pixel ( j,0, HeatColor ( heat[j] ).rgb );
		}
	}

	while ( StripLights_Ready() == 0 );

	StripLights_Trigger ( 1 );
	CyDelay ( 16 );
}

static int pos = 0, dir = 1; // Position, direction of "eye"

void Larson1(int count  )
{
    int j;
    
	for ( j = 0; j < count; j++) {
		// Draw 5 pixels centered on pos.  setPixelColor() will clip any
		// pixels off the ends of the strip, we don't need to watch for that.
		StripLights_Pixel(pos - 2, 0,	RGB(0x10, 0x00, 0x00)); // Dark red
		StripLights_Pixel(pos - 1, 0,	RGB(0x80, 0x00, 0x00)); // Medium red
		StripLights_Pixel(pos, 0,		RGB(0xFF, 0x30, 0x00)); // Center pixel is brightest
		StripLights_Pixel(pos + 1, 0,	RGB(0x80, 0x00, 0x00)); // Medium red
		StripLights_Pixel(pos + 2, 0,	RGB(0x10, 0x00, 0x00)); // Dark red

		while (StripLights_Ready() == 0);

		StripLights_Trigger(1);

		CyDelay(50);

		// Rather than being sneaky and erasing just the tail pixel,
		// it's easier to erase it all and draw a new one next time.
		StripLights_MemClear(0);


		// Using this instead of the memclear would leave a trail behind it.
		//FadeStrip(StripLights_MIN_X, StripLights_MAX_X, 6);

		// Bounce off ends of strip
		pos += dir;
		if (pos < 0) {
			pos = 1;
			dir = -dir;
		} else if (pos >= StripLights_MAX_X) {
			pos = StripLights_MAX_X - 2;
			dir = -dir;
		}
	}
}


// snake tail chaser
void Meet(uint16_t count)
{
	int x;
	uint32_t startColor;
	uint32_t startColor2;

	startColor = getColor(rand() % 73);// StripLights_RED;
	startColor2 = getColor(rand() % 73);// StripLights_RED;

	for (x = StripLights_MIN_X; x <= StripLights_MAX_X+50; x++) {


		StripLights_Pixel(x, 0, startColor);
		if (rand() % 10 == 5) {
			startColor += 0x010101;
		}

		StripLights_Pixel(StripLights_MAX_X-x, 0, startColor2);
		if (rand() % 10 == 5) {
			startColor += 0x010101;
		}
		if (rand() % 10 == 5) {
			startColor2 += 0x010101;
		}

		FadeStrip(StripLights_MIN_X, StripLights_MAX_X, 5);

		while (StripLights_Ready() == 0);

		StripLights_Trigger(1);
		CyDelay(50);

	}
}
void twinks(int xbase, int length)
{
    int x;
    
	for ( x = 0; x < length; x++) {

		uint32_t r = calculate_sparkle(rand() % 10);

		StripLights_Pixel(xbase+x , 0, RGB(r,r,r));

	}
}
void Sparky(uint16_t count)
{
	int x1,x;

	for ( x = 0; x < count; x++) {
		// pick a starting spot
		x1 = rand() % StripLights_MAX_X;

		twinks(x1, 5 + (rand() % 5));

		StripLights_Trigger(1);
        CyDelay(5);
	}


}
