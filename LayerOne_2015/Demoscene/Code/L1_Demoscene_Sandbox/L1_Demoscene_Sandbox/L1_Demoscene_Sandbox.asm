;*
; L1_Demoscene_Sandbox.asm
;
;  Created: 8/2/2014 4:30:12 PM
;  Author: arko
;/ 

; FUSE SETTINGS
; -------------------------------
; EXTENDED = 0xF9
; LOW = 0xDF
; HIGH = 0xE7

; Peripherals:
; -------------------------------
; LED1	  -	Pin 12 - PB0 (ICP)
; SWITCH1 - Pin 29 - PC6 (RESET)
; SWITCH2 - Pin 30 - PD0 (RXD)
;
; Audio DAC:
; -------------------------------
; 6-bit DAC
;
; Pin 11 - PD7 - a5 (MSB)
; Pin 10 - PD6 - a4
; Pin 09 - PD5 - a3
; Pin 02 - PD4 - a2
; Pin 31 - PD1 - a1
; Pin 32 - PD2 - a0 (LSB)
;
; VGA Port:
; -------------------------------
; 2-bit DAC for each VGA channel (BLUE, GREEN, RED)
;
; Pin 24 - PC1 - b1 (BLUE MSB)
; Pin 23 - PC0 - b0 (BLUE LSB) 
; Pin 26 - PC3 - g1 (GREEN MSB)
; Pin 25 - PC2 - g0 (GREEN LSB)
; Pin 28 - PC5 - r1 (RED MSB)
; Pin 27 - PC4 - r0 (RED LSB)
; Pin 01 - PD3 - Brightness
; Pin 16 - PB4 - MOSI pin during programming, but used for text-hack

; Atmel Studio environment is already configured to include this
; .include m88PAdef.inc  
;


start: 
	;ldi r17, 0xF6 
	;out DDRD, r17 

	ldi r17, 0xF6 
	out DDRB, r17 

	;ldi r16, 0xFF
	;out PORTD, r16 

	ldi r16, 0xFF
	out PORTB, r16
loop:  
	;in r16, PORTD
	;dec r16
	;out PORTD, r16 

	in r16, PORTB
	dec r16
	out PORTB, r16 
	
	rcall Delay_20ms

	rjmp loop

Delay_Sound:                                                        
	ldi     R18,byte3(2 * 1000 * 20 / 5)
	ldi     R17,high(2 * 1000 * 20 / 5)
	ldi     R16,low(2 * 1000 * 20 / 5)

	subi    R16,1
	sbci    R17,0
	sbci    R18,0
	brcc    pc-3

	ret

Delay_20ms:                                                        
	ldi     R18,byte3(20000000 * 1000 * 20 / 5)
	ldi     R17,high(20000000 * 1000 * 20 / 5)
	ldi     R16,low(20000000 * 1000 * 20 / 5)

	subi    R16,1
	sbci    R17,0
	sbci    R18,0
	brcc    pc-3

	ret