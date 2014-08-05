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

.equ clockmhz = 20		; CPU frequency is 20Mhz
.equ f_cpu = 20000000

start: 
	ldi r17, 0x00	; PB0 and PB4 are set to outputs, other PORTB pins are set to inputs
	out DDRB, r17 

	ldi r16, 0xFF	; Set all PORTB outputs high, and inputs to pull-up
	out PORTB, r16

	ldi r17, 0xF6	; Audio DAC - PD7, PD6, PD5, PD4, PD2, and PD1 are set to outputs
	out DDRD, r17   ; PD0 and PD3 pins are set to inputs

	ldi r16, 0x09	; Set all PORTD outputs low, and inputs to pull-up
	out PORTD, r16

	ldi r16, 0x00

sawup:  
	rcall audio_playbyte
	inc r16
	rcall delay_1ms	
	brcc sawup

sawdown:	
	rcall audio_playbyte
	dec r16
	rcall delay_1ms	
	brcc sawdown
	rjmp sawup

delay_1ms:                                                        
	ldi     R27,byte3(clockmhz * 1000 * 5 / 5)	; Waste clock cycles 
	ldi     R26,high(clockmhz * 1000 * 5 / 5)	; in a clever way
	ldi     R25,low(clockmhz * 1000 * 5 / 5)

	subi    R25,1
	sbci    R26,0
	sbci    R27,0
	brcc    pc-3

	ret

audio_playbyte:
	; This subroute takes the byte in r16 and correctly maps 
	; it to PORTD which is connected to the DAC R2R
	; Is this really inefficient code? Yep!
	ldi r18, 0x00

	mov r17, r16
	ldi r22, 0x01
	and r17, r22
	lsl r17
	lsl r17
	or  r18, r17

	mov r17, r16
	ldi r22, 0x02
	and r17, r22
	or  r18, r17

	mov r17, r16
	ldi r22, 0x3C
	and r17, r22
	lsl r17
	lsl r17
	or  r18, r17

	ldi r22, 0x09
	or r18, r22
	out PORTD, r18

	ret
