;-------------------------------------------------------------------------------------------------------------------
;	AVRSYN.ASM
;	Synthesiser on the ATmega64A
;   Created: 27/02/2002 - AT90S8535 code release
;       V01: 16/04/2005 - Adjusted code to support ATMEGA16 and AVR-Studio 4 Build 408
;       V02: 19/04/2005 - Added code to reduce the Keyboard Scan period from 32uS to 100mS
;       V03: 21/04/2005 - Extended wait time between `set & read' in Keyboard Scan
;       V04: 05/05/2005 - Added support for MIDI Address
;                         0 = Recognise all MIDI addresses, 1-15 = Recognise specified MIDI Address
;       V05: 14/06/2005 - Added software `JTAG Disable' (Gabriel Lindeborg)
;       V06: 21/06/2005 - Adjusted Timer2 initialisation to calibrate fundamental DCO frequency (Jarek)
;       V07: 23/06/2005 - Replaced ADCSR with ADCSRA for AVR Studio 4 Build 411
;                         Replaced UBRR with UBRRL for AVR Studio 4 Build 411
;       V08: 29/09/2005 - Removed `skip' on WAIT_20US
;                         Activated MODWHEEL control
;		V10: 07/12/2006 - Upgraded to ATMEGA32 @ 16MHz
;		V11: 03/15/2011 - Upgraded to ATMEGA64a @ 16MHz
;
;	PF0..7		A/D
;	PB0-PB4		Control Panel Switches - ROWS
;	PB5-PB7		In Circuit Programming (ISP)
;	PC0-PC7		DATA port for 2 x HC574
;	PD0		    RxD (MIDI IN)
;	PD1		    MIDI LED
;	PD2		    WRL\
;	PD3		    WRH\
;	PD4-PD7		Control Panel Switches - COLUMNS
;
;	Timer0		not used
;	Timer1		Time counter: CK/256      --> TCNT1 (16uS)
;	Timer2		Sample timer: (CK/8) / 32 --> 31.25kHz
;
;   OSCILLATOR:
;   This design uses the "direct frequency synthesis". A three-byte counter (= phase) is being
;   incremented by a value which is proportional to the sound frequency (= phase delta). The
;   increment takes place every sampling period, i.e. each 32uS. The most significant byte of
;   the counter is the sawtooth wave.
;   The square wave is a result of comparing the sawtooth wave to 128.
;   Each oscillator has its own "phase" and "phase delta" registers.
;   The contents of each "phase delta" register depends on the frequency being generated:
;
;                   PHASE DELTA = 2 ^ 24 * Freq / SamplingFreq
;
;   where:
;       SamplingFreq = 31250 Hz
;       Freq = 440 * 2 ^ ((n - 69 + d) / 12)
;       where in turn:
;           n = MIDI note number. Range limited to 36 to 96 (5 octaves)
;           d = transpose/detune (in halftones)
;
;   FILTER:
;   This is probably the simplest low pass digital filter ever invented, the formula is:
;
;                   y[i] = y[i - 1] + m * (x[i] - y[i - 1])
;
;   The greater the m, the higher the cutoff frequency. For m=0 there is no
;   AC signal on the output, and for m=1 the output is a copy of the input.
;-------------------------------------------------------------------------------------------------------------------

                    .NOLIST
                    .INCLUDE "m128def.inc"
                    .LIST
                    .LISTMAC

                    .SET cpu_frequency = 16000000
                    .SET baud_rate     = 31250
		            .SET KBDSCAN       = 6250		;Key scan period: 6250 * 16탎 = 100ms

;-------------------------------------------------------------------------------------------------------------------
;			V A R I A B L E S
;-------------------------------------------------------------------------------------------------------------------
;registry:
;			            R0	reserved for the "lpm" instruction
.DEF ZERO           =   R1

;current phase of DCO A:
.DEF PHASEA_0	    = 	R2
.DEF PHASEA_1	    = 	R3
.DEF PHASEA_2	    = 	R4

;current phase of DCO B:
.DEF PHASEB_0	    = 	R5
.DEF PHASEB_1	    = 	R6
.DEF PHASEB_2	    = 	R7

;increase phase for DCO A:
.DEF DELTAA_0	    = 	R8
.DEF DELTAA_1	    = 	R9
.DEF DELTAA_2	    = 	R10

;increase phase for DCO B:
.DEF DELTAB_0	    = 	R11
.DEF DELTAB_1	    = 	R12
.DEF DELTAB_2	    = 	R13

;DCF:
.DEF YN1_L		    = 	R14
.DEF YN1_H		    = 	R15

;RAM (0060h...025Fh):

                    .DSEG
					.org 0x100

;MIDI:
MIDIPHASE:          .BYTE 1
MIDICHANNEL:        .BYTE 1
MIDIDATA0:	        .BYTE 1
MIDIVELOCITY:	    .BYTE 1
MIDINOTE:	        .BYTE 1
MIDINOTEPREV:	    .BYTE 1		        ; buffer for MIDI note
MIDIPBEND_L:        .BYTE 1		        ;\
MIDIPBEND_H:        .BYTE 1		        ;/ -32768..+32766
MIDIMODWHEEL:	    .BYTE 1		        ; 0..254

;current sound parameters:
LFOFREQ:	        .BYTE 1	            ; 0..255
LFOLEVEL:	        .BYTE 1	            ; 0..255
SWITCH1:	        .BYTE 1	            ; bit meanings for switch-bank 1:
					                    ; b0: SW16 DCO Distortion on/off
					                    ; b1: SW15 MODWHEEL disable/enable
					                    ; b2: SW14 LFO KBD SYNC off/on
    					                ; b3: SW13 LFO norm/rand
					                    ; b4: SW12 ENV LOOP off/on
					                    ; b5: SW11 transpose down/up
    					                ; b6: SW10 octave A down/up
					                    ; b7: SW9  octave B down/up
SWITCH2:	        .BYTE 1	            ; bit meanings for switch-bank 2:
					                    ; b0: SW8  wave A saw/squ
					                    ; b1: SW7  wave B saw/squ
    					                ; b2: SW6  osc B off/on
					                    ; b3: SW5  DCA gate/env
    					                ; b4: SW4  ENV AR/ASR
					                    ; b5: SW3  LFO DCO/DCF
					                    ; b6: SW2  LFO tri/squ
					                    ; b7: SW1  DCF KBD TRACK
SWITCH3:	        .BYTE	1		    ; b0: MIDI SWITCH 1
					                    ; b1: MIDI SWITCH 2
					                    ; b2: MIDI SWITCH 3
					                    ; b3: MIDI SWITCH 4
MODEFLAGS1:	        .BYTE 1	            ; b0 = DCO DIST: 0=off, 1=on
					                    ; b1 = wave A: 0=saw, 1=squ
					                    ; b2 = wave B: 0=saw, 1=squ
					                    ; b3 = osc B: 0=off, 1=on
					                    ; b4 = DCA mode: 0=gate, 1=env
					                    ; b5 = transpose: 0=down, 1=up
					                    ; b6 = octave A: 0=down, 1=up
					                    ; b7 = octave B: 0=down, 1=up
MODEFLAGS2:	        .BYTE 1	            ; b0 = LFO MODE: 0=DCO, 1=DCF
					                    ; b1 = LFO WAVE: 0=tri, 1=squ
					                    ; b2 = DCF KBD TRACK: 0=off, 1=on
    					                ; b3 = ENV LOOP: 0=off, 1=on
					                    ; b4 = ENV MODE: 0 = A-R, 1 = A-S-R
					                    ; b5 =
					                    ; b6 = LFO KBD SYNC: 0=off, 1=on
					                    ; b7 = LFO: 0=norm, 1=rand
SETMIDICHANNEL:	    .BYTE 1             ; selected MIDI channel: 0 for OMNI or 1..15
DETUNEB_FRAC:	    .BYTE 1	            ;\
DETUNEB_INTG:	    .BYTE 1	            ;/ -128,000..+127,996
CUTOFF:		        .BYTE 1	            ; 0..255
VCFENVMOD:	        .BYTE 1	            ; 0..255
PORTAMENTO:	        .BYTE 1	            ; 0..255
ATTACKTIME:	        .BYTE 1	            ; 0..255
RELEASETIME:        .BYTE 1	            ; 0..255
NOTE_L:		        .BYTE 1
NOTE_H:		        .BYTE 1
NOTE_INTG:	        .BYTE 1
PORTACNT:	        .BYTE 1		        ; 2 / 1 / 0
LPF_I:		        .BYTE 1
LEVEL:		        .BYTE 1		        ; 0..255
PITCH:		        .BYTE 1		        ; 0..96
ADC_CHAN:	        .BYTE 1		        ; 0..7
ADC_0:		        .BYTE 1
ADC_1:		        .BYTE 1
ADC_2:		        .BYTE 1
ADC_3:		        .BYTE 1
ADC_4:		        .BYTE 1
ADC_5:		        .BYTE 1
ADC_6:		        .BYTE 1
ADC_7:		        .BYTE 1
GATE:		        .BYTE 1		        ; 0 / 1
GATEEDGE:	        .BYTE 1		        ; 0 / 1
TPREV_KBD_L:	    .BYTE 1
TPREV_KBD_H:	    .BYTE 1
TPREV_L:	        .BYTE 1
TPREV_H:	        .BYTE 1
DELTAT_L:	        .BYTE 1		        ;\ Time from former course
DELTAT_H:	        .BYTE 1		        ;/ of the main loop (1 bit = 32 탎)
ENVPHASE:	        .BYTE 1		        ; 0=stop 1=attack 2=sustain 3=release
ENV_FRAC_L:	        .BYTE 1
ENV_FRAC_H:	        .BYTE 1
ENV_INTEGR:	        .BYTE 1
LFOPHASE:	        .BYTE 1		        ; 0=up 1=down
LFO_FRAC_L:	        .BYTE 1		        ;\
LFO_FRAC_H:	        .BYTE 1		        ; > -128,000..+127,999
LFO_INTEGR:	        .BYTE 1		        ;/
LFOVALUE:	        .BYTE 1		        ; -128..+127
OLDWAVEA:	        .BYTE 1
OLDWAVEB:	        .BYTE 1
SHIFTREG_0:	        .BYTE 1		        ;\
SHIFTREG_1:	        .BYTE 1		        ; > shift register for
SHIFTREG_2:	        .BYTE 1		        ;/  pseudo-random generator
LFOBOTTOM_0:        .BYTE 1		        ;\
LFOBOTTOM_1:        .BYTE 1		        ; > bottom level of LFO
LFOBOTTOM_2:        .BYTE 1		        ;/
LFOTOP_0:	        .BYTE 1		        ;\
LFOTOP_1:	        .BYTE 1		        ; > top level of LFO
LFOTOP_2:	        .BYTE 1		        ;/

OLDSWITCH1:	   .BYTE 1
OLDSWITCH2:	   .BYTE 1	        

;stack: 0x0A3..0x25F
            .ESEG

;-------------------------------------------------------------------------------------------------------------------
;			V E C T O R   T A B L E
;-------------------------------------------------------------------------------------------------------------------
            .CSEG
			.ORG 0x0000

		    jmp	RESET		            ; RESET

;; IRQ 0 -7
		    jmp	IRQ_NONE	            ; INT0
		    jmp	IRQ_NONE	            ; INT1
		    jmp	IRQ_NONE	            ; INT2
		    jmp	IRQ_NONE	            ; INT3

		    jmp	IRQ_NONE	            ; INT4
		    jmp	IRQ_NONE	            ; INT5
		    jmp	IRQ_NONE	            ; INT6
		    jmp	IRQ_NONE	            ; INT7


		    jmp	TIM2_CMP				; TIMER2 COMPARE
		    jmp	IRQ_NONE	            ; TIMER2 OVERFLOW

			
		    jmp	IRQ_NONE	            ; TIMER1 CAPTURE
		    jmp	IRQ_NONE	            ; TIMER1 COMPA
		    jmp	IRQ_NONE	            ; TIMER1 COMPB
    		jmp	IRQ_NONE	            ; TIMER1 OVF

		    jmp	IRQ_NONE	            ; TIMER0 COMPA
		    jmp	IRQ_NONE	            ; TIMER0 OVF

		    jmp	IRQ_NONE	            ; SPI,STC Tranfer complete

		    jmp	UART_RXC	            ; UART, RX COMPLETE
		    jmp	IRQ_NONE	            ; UART,UDRE
		    jmp	IRQ_NONE	            ; UART, TX COMPLETE

		    jmp	IRQ_NONE	            ; ADC CONVERSION COMPLETE

		    jmp	IRQ_NONE	            ; EEPROM READY

		    jmp	IRQ_NONE	            ; ANALOG COMPARATOR

			jmp IRQ_NONE				; TIMER1 COMPAREC
			jmp IRQ_NONE				; TIMER3 CAPTURE
			jmp IRQ_NONE				; TIMER3 COMPAREA
			jmp IRQ_NONE				; TIMER3 COMPAREB
			jmp IRQ_NONE				; TIMER3 COMPAREC
			jmp IRQ_NONE				; TIMER3 OVERFLOW

			jmp IRQ_NONE				; USART1 RX, COMPLETE
			jmp IRQ_NONE				; USART1 UDR EMPTY
			jmp IRQ_NONE				; USART1 TX COMPLETE

            jmp IRQ_NONE                ; 2-Wire Serial Interface

            jmp IRQ_NONE                ; STORE PROGRAM MEMORY READY



;-------------------------------------------------------------------------------------------------------------------
;			R O M   T A B L E S
;-------------------------------------------------------------------------------------------------------------------
DELTA_C:
            .DW	0x2559		            ;\
		    .DW	0x0011		            ;/ note  0 ( 8.175799 Hz)

DELTA_CIS:
            .DW	0x2A5A		            ;\
		    .DW	0x0012		            ;/ note  1 ( 8.661957 Hz)

DELTA_D:
            .DW	0x3EE1		            ;\
		    .DW	0x0013		            ;/ note  2 ( 9.177024 Hz)

DELTA_DIS:
            .DW	0x63D8		            ;\
		    .DW	0x0014		            ;/ note  3 ( 9.722718 Hz)

DELTA_E:
            .DW	0x9A3C		            ;\
		    .DW	0x0015		            ;/ note  4 (10.300861 Hz)

DELTA_F:
            .DW	0xE314		            ;\
		    .DW	0x0016		            ;/ note  5 (10.913382 Hz)

DELTA_FIS:
            .DW	0x3F7A		            ;\
		    .DW	0x0018		            ;/ note  6 (11.562326 Hz)

DELTA_G:
            .DW	0xB098		            ;\
		    .DW	0x0019		            ;/ note  7 (12.249857 Hz)

DELTA_GIS:
            .DW	0x37A8		            ;\
		    .DW	0x001B		            ;/ note  8 (12.978272 Hz)

DELTA_A:
            .DW	0xD5FA		            ;\
		    .DW	0x001C		            ;/ note  9 (13.750000 Hz)

DELTA_AIS:
            .DW	0x8CEE		            ;\
		    .DW	0x001E		            ;/ note 10 (14.567618 Hz)

DELTA_H:
            .DW	0x5DFD		            ;\
		    .DW	0x0020		            ;/ note 11 (15.433853 Hz)

DELTA_C1:
            .DW	0x4AB2		            ;\
		    .DW	0x0022		            ;/ note 12 (16.351598 Hz)

;-----------------------------------------------------------------------------
TAB_VCF:
            .DW	0x0101
		    .DW	0x0101
		    .DW	0x0201
		    .DW	0x0202
    		.DW	0x0202
		    .DW	0x0202
		    .DW	0x0202
		    .DW	0x0402
		    .DW	0x0404
	    	.DW	0x0505
    		.DW	0x0606
    		.DW	0x0806
		    .DW	0x0908
		    .DW	0x0A0A
		    .DW	0x0C0C
		    .DW	0x100C
		    .DW	0x1211
		    .DW	0x1614
		    .DW	0x1A18
		    .DW	0x201C
		    .DW	0x2422
		    .DW	0x2C28
		    .DW	0x3430
		    .DW	0x4038
		    .DW	0x4C44
		    .DW	0x5852
		    .DW	0x6861
		    .DW	0x8070
		    .DW	0x968A
		    .DW	0xB0A4
		    .DW	0xD8C4
		    .DW	0xFFE8

;-----------------------------------------------------------------------------
TAB_VCA:
            .DW	0x0100		            ; -48.2dB, -oodB
		    .DW	0x0302		            ; -38.6dB, -42.1dB
		    .DW	0x0504		            ; -34.2dB, -36.1dB
		    .DW	0x0706		            ; -31.3dB, -32.6dB
		    .DW	0x0908		            ; -29.1dB, -30.1dB
    		.DW	0x0C0A		            ; -26.6dB, -28.2dB
		    .DW	0x110E		            ; -23.6dB, -25.2dB
		    .DW	0x1814		            ; -20.6dB, -22.1dB
		    .DW	0x201C		            ; -18.1dB, -19.2dB
		    .DW	0x2824		            ; -16.1dB, -17.0dB
		    .DW	0x302C		            ; -14.5dB, -15.3dB
    		.DW	0x3834		            ; -13.2dB, -13.8dB
		    .DW	0x403C		            ; -12.0dB, -12.6dB
    		.DW	0x4A46		            ; -10.8dB, -11.3dB
		    .DW	0x5450		            ;  -9.7dB, -10.1dB
		    .DW	0x615A		            ;  -8.4dB,  -9.1dB
		    .DW	0x7068		            ;  -7.2dB,  -7.8dB
		    .DW	0x8078		            ;  -6.0dB,  -6.6dB
		    .DW	0x9489		            ;  -4.8dB,  -5.4dB
    		.DW	0xA8A0		            ;  -3.7dB,  -4.1dB
		    .DW	0xC2B8		            ;  -2.4dB,  -2.9dB
		    .DW	0xE0D2		            ;  -1.2dB,  -1.7dB
		    .DW	0xFFF0		            ;  -0.0dB,  -0.6dB

;-----------------------------------------------------------------------------
;rate = 536.870912 / time (both in seconds)
TIMETORATE:
            .DW	65535		            ; 8.192 mS
		    .DW	50957		            ; 10.54 mS
		    .DW	39621		            ; 13.55 mS
		    .DW	30807		            ; 17.43 mS
		    .DW	23953		            ; 22.41 mS
		    .DW	18625		            ; 28.83 mS
		    .DW	14481		            ; 37.07 mS
		    .DW	11260		            ; 47.68 mS
		    .DW	 8755		            ; 61.32 mS
    		.DW	 6807		            ; 78.87 mS
		    .DW	 5293		            ; 101.4 mS
		    .DW	 4115		            ; 130.5 mS
		    .DW	 3200		            ; 167.8 mS
		    .DW	 2488		            ; 215.8 mS
		    .DW	 1935		            ; 277.5 mS
    		.DW	 1504		            ; 356.9 mS
		    .DW	 1170		            ; 459.0 mS
		    .DW	  909		            ; 590.4 mS
		    .DW	  707		            ; 759.3 mS
		    .DW	  550		            ; 976.5 mS
		    .DW	  427		            ; 1.256 S
    		.DW	  332		            ; 1.615 S
    		.DW   258		            ; 2.077 S
		    .DW	  201		            ; 2.672 S
		    .DW	  156		            ; 3.436 S
		    .DW	  121		            ; 4.419 S
		    .DW	   94		            ; 5.684 S
		    .DW	   73		            ; 7.310 S
		    .DW	   57		            ; 9.401 S
		    .DW	   44		            ; 12.09 S
		    .DW	   35		            ; 15.55 S
		    .DW	   27		            ; 20.00 S

reversebits:
	.db	0x00,0x80,0x40,0xC0,0x20,0xA0,0x60,0xE0,0x10,0x90,0x50,0xD0,0x30,0xB0,0x70,0xF0,0x08,0x88,0x48,0xC8,0x28,0xA8,0x68,0xE8,0x18,0x98,0x58,0xD8,0x38,0xB8,0x78,0xF8,0x04,0x84,0x44,0xC4,0x24,0xA4,0x64,0xE4,0x14,0x94,0x54,0xD4,0x34,0xB4,0x74,0xF4 ,0x0C,0x8C,0x4C,0xCC,0x2C,0xAC,0x6C,0xEC,0x1C,0x9C,0x5C,0xDC,0x3C,0xBC,0x7C,0xFC,0x02,0x82,0x42,0xC2,0x22,0xA2,0x62,0xE2,0x12,0x92,0x52,0xD2,0x32,0xB2,0x72,0xF2,0x0A,0x8A,0x4A,0xCA,0x2A,0xAA,0x6A,0xEA,0x1A,0x9A,0x5A,0xDA,0x3A,0xBA,0x7A,0xFA,0x06,0x86,0x46,0xC6,0x26,0xA6,0x66,0xE6,0x16,0x96,0x56,0xD6,0x36,0xB6,0x76,0xF6,0x0E,0x8E,0x4E,0xCE,0x2E,0xAE,0x6E,0xEE,0x1E,0x9E,0x5E,0xDE,0x3E,0xBE,0x7E,0xFE,0x01,0x81,0x41,0xC1,0x21,0xA1,0x61,0xE1,0x11,0x91,0x51,0xD1,0x31,0xB1,0x71,0xF1,0x09,0x89,0x49,0xC9,0x29,0xA9,0x69,0xE9,0x19,0x99,0x59,0xD9,0x39,0xB9,0x79,0xF9,0x05,0x85,0x45,0xC5,0x25,0xA5,0x65,0xE5,0x15,0x95,0x55,0xD5,0x35,0xB5,0x75,0xF5,0x0D,0x8D,0x4D,0xCD,0x2D,0xAD,0x6D,0xED,0x1D,0x9D,0x5D,0xDD,0x3D,0xBD,0x7D,0xFD,0x03,0x83,0x43,0xC3,0x23,0xA3,0x63,0xE3,0x13,0x93,0x53,0xD3,0x33,0xB3,0x73,0xF3,0x0B,0x8B,0x4B,0xCB,0x2B,0xAB,0x6B,0xEB,0x1B,0x9B,0x5B,0xDB,0x3B,0xBB,0x7B,0xFB,0x07,0x87,0x47,0xC7,0x27,0xA7,0x67,0xE7,0x17,0x97,0x57,0xD7,0x37,0xB7,0x77,0xF7,0x0F,0x8F,0x4F,0xCF,0x2F,0xAF,0x6F,0xEF,0x1F,0x9F,0x5F,0xDF,0x3F,0xBF,0x7F,0xFF
;-------------------------------------------------------------------------------------------------------------------
;		I N T E R R U P T   S U B R O U T I N E S
;-------------------------------------------------------------------------------------------------------------------

IRQ_NONE:
            reti


; Timer 0 compare interrupt (playback)
;-------------------------------------
TIM0_CMP:
		    push	R16
		    in	    R16, SREG		    ;\
    		push	R16			        ;/ push SREG



		    pop	    R16		            ;\
		    out	    SREG, R16	        ;/ pop SREG
		    pop	    R16
	reti


; Timer 2 compare interrupt (sampling)
;-------------------------------------
TIM2_CMP:

		    push	R16
		    in	    R16, SREG		    ;\
    		push	R16			        ;/ push SREG

			push	R0
		    push	R17
		    push	R28
		    push	R29

;DCO A, DCO B:
		    lds	R28, MODEFLAGS1

;DCO A:
		    mov	    R17, PHASEA_2
		    rol	    R17			        ; R17.7 --> Cy
		    sbc	    R17, R17	        ; R17 = 0 or 255 (square wave)
		    sbrs	R28, 1			    ; 0/1 (DCO A = saw/squ)
		    mov	    R17, PHASEA_2	    ; only when sawtooth
		    lds	    R29, OLDWAVEA
		    sts	    OLDWAVEA, R17
		    add	    R17, R29	        ;\ R17 = mean value of former
		    ror	    R17			        ;/ and present values

;DCO B:
		    mov	    R16, PHASEB_2
		    rol	    R16			        ; R16.7 --> Cy
		    sbc	    R16, R16	        ; R16 = 0 or 255 (square wave)
		    sbrs	R28, 2			    ; 0/1 (DCO B = saw/squ)
		    mov	    R16, PHASEB_2	    ; only when sawtooth
		    lds	    R29, OLDWAVEB
		    sts	    OLDWAVEB, R16
		    add	    R16, R29	        ;\ R16 = mean value of former
		    ror	    R16		            ;/ and present values
		    sbrc	R28, 0			    ; 0/1 (OSC DIST = off/on)
    		eor	    R17, R16
		    sbrs	R28, 3
		    ldi	    R16, 128	        ; when DCO B = off

;sum of DCO A and DCO B:
		    add	    R17, R16	        ; Cy:R17 = A + B
		    ror	    R17		            ; R17 = 0,5 * (A + B)
		    lsr	    R17		            ; R17 = 0,25 * (A + B)
		    ldi	    R16, 0		        ;\ R17:R16 = A + B
		    subi	R17, 64			    ;/ -16384..+16383

;DCF:
		    lds	    R28, LPF_I
		    cpi	    R28, 255
		    breq	T2_SKIPVCF
		    sub	    R16, YN1_L	        ;\
		    sbc	    R17, YN1_H	        ;/ R17:R16 = x[n] - y[n-1]

T2_FLOOP:
            asr	    R17		            ;\
		    ror	    R16		            ;/ R17:R16 = R17:R16 asr 1
		    lsl	    R28		            ; R28.7 --> Carry
		    brcc	T2_FCHECK
		    add	    YN1_L, R16	        ;\
		    adc	    YN1_H, R17	        ;/ y[n-1] + 1/k*(x[n]-y[n-1])

T2_FCHECK:
            tst	    R28
		    brne	T2_FLOOP
    		mov	    R16, YN1_L	        ;\
		    mov	    R17, YN1_H	        ;/ y[n-1] --> y[n]

T2_SKIPVCF:

;DCA:
		    ldi	    R28, 0
		    ldi	    R29, 0
		    push	R31
		    lds	    R31, LEVEL
		    cpi	    R31, 255
		    brne	T2_ACHECK		    ; multiply when LEVEL!=255
		    mov	    R28, R16
		    mov	    R29, R17
		    rjmp	T2_AEXIT

T2_ALOOP:
            asr	    R17		            ;\
		    ror	    R16		            ;/ R17:R16 = R17:R16 asr 1
		    lsl	    R31		            ; Cy <-- R31 <-- 0
		    brcc	T2_ACHECK
    		add	    R28, R16
		    adc	    R29, R17

T2_ACHECK:
            tst	    R31
		    brne	T2_ALOOP

T2_AEXIT:
            pop	    R31

;write sample (R29:R28) in DAC:
		    ;subi	R29, 128		    ; U2 --> PB


; flicker LED's using 16 bit signed value (not very power efficient since some values  will conflict)

			out		DDRA,R29
			out		PORTA,R28
		    
; oops

			push 	r30
			push 	r31

			;//	lsl		r28						; index=(index*2)

				ldi		ZL, low(reversebits<<1)	; AVR's are odd with their two byte memory layout
				ldi		ZH, high(reversebits<<1)

				add		ZL,r29					; add on the value to reverse, index into the lookup table

				adc 	ZH,zero					; add carry if it exists

			   	;lpm								; -> r0  (uses Z) with lsb as index into byte
	    
				out	    PORTC, r29
					        	; MSByte

			pop		r31
			pop		r30


;increment phase:
		    add	    PHASEA_0, DELTAA_0
		    adc	    PHASEA_1, DELTAA_1
		    adc	    PHASEA_2, DELTAA_2
		    add	    PHASEB_0, DELTAB_0
		    adc	    PHASEB_1, DELTAB_1
		    adc	    PHASEB_2, DELTAB_2

;end:
		    pop	    R29
		    pop	    R28
		    pop	    R17
			pop		R0

		    pop	    R16		            ;\
		    out	    SREG, R16	        ;/ pop ESREG
		    pop	    R16
		    ;;cbi	PORTB,0			    ; PB0 = 0 (DIAG)
		    reti

;------------------------
; UART receiver (MIDI IN)
;------------------------
UART_RXC:
            push	R16
		    in	    R16, SREG	        ;\
		    push	R16			        ;/ push SREG

		    in	    R16, UDR0	        ; read received byte in R16
		    cbi	    UCSR0B, 7		    ; RXCIE=0 (disable UART interrupts)
		    sei				            ; enable other interrupts
		    push	R17

		    tst	    R16		            ;\ jump when
		    brpl	INTRX_DATA		    ;/ R16.7 == 0 (MIDI data byte)

;MIDI status byte (1xxxxxxx):
		    mov	    R17, R16
		    andi	R17, 0xF0
		    cpi	    R17, 0x80
		    breq	INTRX_ACCEPT	    ; 8x note off
		    cpi	    R17, 0x90
		    breq	INTRX_ACCEPT	    ; 9x note on
		    cpi	    R17, 0xB0
		    breq	INTRX_ACCEPT	    ; Bx control change
		    cpi	    R17, 0xE0
		    breq	INTRX_ACCEPT	    ; Ex pitch bend
		    ldi	    R17, 0		        ;\
		    sts	    MIDIPHASE, R17	    ;/ MIDIPHASE = 0
		    rjmp	INTRX_EXIT		    ; Ax polyphonic aftertouch
						                ; Cx program change
						                ; Dx channel aftertouch
						                ; Fx system

INTRX_ACCEPT:
            sts	    MIDIPHASE, R17	    ; phase = 80 90 B0 E0
		    andi	R16, 0x0F		    ;\
		    inc	    R16			        ; > store MIDI channel 1..16
		    sts	    MIDICHANNEL, R16	;/
		    lds	    R17, SETMIDICHANNEL	;0 for OMNI or 1..15
		    tst	    R17
		    breq	INTRX_ACPT_X		; end when OMNI
		    cp	    R17, R16			; compare set channel to the incoming channel
		    breq	INTRX_ACPT_X		; end when right channel
		    ldi	    R17, 0			    ;\ otherwise:
		    sts	    MIDIPHASE, R17		;/ MIDIPHASE = 0 (no data service)

INTRX_ACPT_X:
            rjmp	INTRX_EXIT

;MIDI data byte (0xxxxxxx):
INTRX_DATA:
            lds	    R17, MIDIPHASE
		    cpi	    R17, 0x80		    ;\
		    breq	INTRX_NOFF1		    ; \
		    cpi	    R17, 0x81		    ; / note off
		    breq	INTRX_NOFF2		    ;/
		    rjmp	INTRX_NOTEON

INTRX_NOFF1:
            inc	    R17			        ;\
		    sts	    MIDIPHASE, R17	    ;/ MIDIPHASE = 0x81
		    sts	    MIDIDATA0, R16	    ; MIDIDATA0 = d
		    rjmp	INTRX_EXIT

INTRX_NOFF2:
            dec	    R17			        ;\
		    sts	    MIDIPHASE, R17	    ;/ MIDIPHASE = 0x80
		    rjmp	INTRXNON2_OFF

;9x note on:
INTRX_NOTEON:
            cpi	    R17, 0x90		    ;\
		    breq	INTRX_NON1		    ; \
		    cpi	    R17, 0x91		    ; / note on
		    breq	INTRX_NON2		    ;/
		    rjmp	INTRX_CTRL

INTRX_NON1:
            inc     R17			        ;\
		    sts	    MIDIPHASE, R17	    ;/ MIDIPHASE = 0x91
		    sts	    MIDIDATA0, R16	    ; MIDIDATA0 = d
		    rjmp	INTRX_EXIT

INTRX_NON2:
            dec	    R17			        ;\
		    sts	    MIDIPHASE, R17	    ;/ MIDIPHASE = 0x90
		    tst	    R16			        ;\
		    brne	INTRXNON2_ON	    ;/ jump when velocity != 0

;turn note off:
INTRXNON2_OFF:
            lds	    R16, MIDIDATA0
		    lds	    R17, MIDINOTEPREV
		    cp	    R16, R17
		    brne	INTRXNON2_OFF1
		    ldi	    R17, 255		    ;\ remove previous note
		    sts	    MIDINOTEPREV, R17	;/ from buffer

INTRXNON2_OFF1:
            lds	    R17, MIDINOTE
		    cp	    R16, R17		    ;\
		    brne	INTRXNON2_OFF3	    ;/ exit when not the same note
		    lds	    R17, MIDINOTEPREV
		    cpi	    R17, 255
		    breq	INTRXNON2_OFF2
		    sts	    MIDINOTE, R17		; previous note is valid
		    ldi	    R17, 255		    ;\ remove previous note
		    sts	    MIDINOTEPREV, R17	;/ from buffer

INTRXNON2_OFF3:
            rjmp	INTRX_EXIT

INTRXNON2_OFF2:
            ldi	    R17, 255		    ;\ remove last note
		    sts	    MIDINOTE, R17		;/
		    ldi	    R17, 0			    ;\
		    sts	    GATE, R17		    ;/ GATE = 0
			sbi	    PORTE, 1		    ; LED on
		    rjmp	INTRX_EXIT

;turn note on:
INTRXNON2_ON:
            sts	    MIDIVELOCITY, R16	; store velocity
		    lds	    R17, MIDINOTE		;\ move previous note
		    sts	    MIDINOTEPREV, R17	;/ into buffer
		    lds	    R17, MIDIDATA0		;\
		    sts	    MIDINOTE, R17		;/ MIDINOTE = note#
		    ldi	    R17, 1
		    sts	    GATE, R17		    ; GATE = 1
		    sts	    GATEEDGE, R17		; GATEEDGE = 1
			cbi	    PORTE, 1		    ; LED off
		    rjmp	INTRX_EXIT

;Bx control change:
INTRX_CTRL:
            cpi	    R17, 0xB0		    ;\
		    breq	INTRX_CC1		    ; \
		    cpi	    R17, 0xB1		    ; / control change
		    breq	INTRX_CC2		    ;/
		    rjmp	INTRX_PBEND

INTRX_CC1:
            inc     R17			        ;\
		    sts	    MIDIPHASE, R17		;/ MIDIPHASE = 0xB1
		    sts	    MIDIDATA0, R16		; MIDIDATA0 = controller#
		    rjmp	INTRX_EXIT

INTRX_CC2:
            dec     R17			        ;\
		    sts	    MIDIPHASE, R17		;/ MIDIPHASE = 0xB0
		    lds	    R17, MIDIDATA0
		    cpi	    R17, 1
		    brne	INTRX_EXIT		    ; skip if not MODWHEEL
		    lsl	    R16
		    sts	    MIDIMODWHEEL, R16
		    rjmp	INTRX_EXIT

;Ex pitch bender:
INTRX_PBEND:
            cpi	    R17, 0xE0		    ;\
		    breq	INTRX_PB1		    ; \
		    cpi	    R17, 0xE1		    ; / pitch bend
		    breq	INTRX_PB2		    ;/
		    rjmp	INTRX_EXIT

INTRX_PB1:
            inc     R17			        ;\
		    sts	    MIDIPHASE, R17		;/ MIDIPHASE = 0xE1
		    sts	    MIDIDATA0, R16		; MIDIDATA0 = dFine	0..127
		    rjmp	INTRX_EXIT

INTRX_PB2:
            dec	    R17			        ;\
		    sts	    MIDIPHASE, R17		;/ MIDIPHASE = 0xE0
		    lds	    R17,MIDIDATA0		;\
		    lsl	    R17			        ;/ R17 = dFine*2	0..254
		    lsl	    R17			        ;\ R16,R17 = P.B.data
		    rol	    R16			        ;/ 0..255,996
		    subi	R16, 128		    ; R16,R17 = -128,000..+127,996
		    sts	    MIDIPBEND_L, R17	;\
		    sts	    MIDIPBEND_H, R16	;/ store P.BEND value
		    rjmp	INTRX_EXIT

INTRX_EXIT:
            pop	    R17
		    pop	    R16			        ;\
		    out	    SREG, R16		    ;/ pop SREG
		    pop	    R16
		    sbi	    UCSR0B, 7			    ; RXCIE=1
		    reti

;-------------------------------------------------------------------------------------------------------------------
;		M A I N   L E V E L   S U B R O U T I N E S
;-------------------------------------------------------------------------------------------------------------------

;=============================================================================
;			Delay subroutines
;=============================================================================

WAIT_10US:
            push	R16		            ; 3+2
		    ldi	    R16, 50		        ; 1

W10U_LOOP:
            dec	    R16		            ; 1\
		    brne	W10U_LOOP	        ; 2/1	/ 49*3 + 2
		    pop	    R16		            ; 2
		    ret			                ; 4

;=============================================================================
;			I/O subroutines
;=============================================================================

;-----------------------------------------------------------------------------
;A/D conversion (start)
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R18 = channel #	        0..7
;Out:	-
;Used:	-
;-----------------------------------------------------------------------------
ADC_START:
            out	    ADMUX, R18	        ; set multiplexer
		    sbi	    ADCSRA, 6	        ; ADSC=1
		    ret

;-----------------------------------------------------------------------------
;A/D conversion (end)
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	-
;Out:	    R16 = result		            0..255
;Used:	    SREG,R17
;-----------------------------------------------------------------------------
ADC_END:
ADCE_LOOP:
            sbis	ADCSRA, 4 	        ;\
		    rjmp	ADCE_LOOP	        ;/ wait for ADIF==1
		    sbi	    ADCSRA, 4 		    ; clear ADIF
		    in	    R16, ADCL	        ;\
		    in	    R17, ADCH	        ;/ R17:R16 = 000000Dd:dddddddd
		    lsr	    R17		            ;\
		    ror	    R16		            ;/ R17:R16 = 0000000D:dddddddd
		    lsr	    R17		            ;\
		    ror	    R16		            ;/ R16 = Dddddddd
		    ret

;=============================================================================
;			arithmetic subroutines
;=============================================================================

;-----------------------------------------------------------------------------
; 16 bit arithmetical shift right (division by 2^n)
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R17:R16 = x
;	        R18 = n (shift count)		0..16
;Out:	    R17:R16 = x asr n
;Used:	    SREG
;-----------------------------------------------------------------------------
ASR16:
            tst	    R18
		    breq	ASR16_EXIT
		    push	R18

ASR16_LOOP:
            asr	    R17		            ;\
		    ror	    R16		            ;/ R17,R16 = R17,R16 asr 1
		    dec	    R18
		    brne	ASR16_LOOP
		    pop	    R18

ASR16_EXIT:
            ret

;-----------------------------------------------------------------------------
; 32 bit logical shift right
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R19:R18:R17:R16 = x
;	        R20 = n (shift count)
;Out:	    R19:R18:R17:R16 = x >> n
;Used:	    SREG
;-----------------------------------------------------------------------------
SHR32:
            tst	    R20
		    breq	SHR32_EXIT
		    push	R20

SHR32_LOOP:
            lsr	    R19
		    ror	    R18
		    ror	    R17
		    ror	    R16
		    dec	    R20
		    brne	SHR32_LOOP
		    pop	    R20

SHR32_EXIT:
            ret

;-----------------------------------------------------------------------------
; 32 bit logical shift left
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R19:R18:R17:R16 = x
;	        R20 = n (shift count)
;Out:	    R19:R18:R17:R16 = x << n
;Used:	    SREG
;-----------------------------------------------------------------------------
SHL32:
            tst	    R20
		    breq	SHL32_EXIT
		    push	R20

SHL32_LOOP:
            lsl	    R16
		    rol	    R17
		    rol	    R18
		    rol	    R19
		    dec	    R20
		    brne	SHL32_LOOP
		    pop	    R20

SHL32_EXIT:
            ret

;-----------------------------------------------------------------------------
;8 bit x 8 bit multiplication (unsigned)
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R16 = x					    0..255
;	        R17 = y					    0,000..0,996
;Out:	    R17,R16 = x * y				0,000..254,004
;Used:	    SREG,R18-R20
;-----------------------------------------------------------------------------
MUL8X8U:
            ldi	    R18, 0		        ;\
		    mov	    R19, R16	        ;/ R19,R18 = x,0
		    mov	    R20, R17
		    clr	    R16
		    clr	    R17
		    rjmp	M88_CHECK

M88_LOOP:
            lsr	    R19		            ;\
		    ror	    R18		            ;/ R19,R18 = R19,R18 shr 1
		    lsl	    R20
		    brcc	M88_CHECK
		    add	    R16, R18
		    adc	    R17, R19

M88_CHECK:
            tst	    R20
		    brne	M88_LOOP
		    ret

;-----------------------------------------------------------------------------
;8 bit x 8 bit multiplication (signed)
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R16 = x					    -128..+127
;	        R17 = y					    0,000..0,996
;Out:	    R17,R16 = x * y				-127,500..+126,504
;Used:	    SREG,R18-R20
;-----------------------------------------------------------------------------
MUL8X8S:
            bst	    R16, 7			    ; T = sign: 0=plus, 1=minus
		    sbrc	R16, 7			    ;\
		    neg	    R16			        ;/ R16 = abs(R16)	0..128
		    rcall	MUL8X8U			    ; R17,R16 = LFO * LFOMOD
		    brtc	M8X8S_EXIT		    ; exit if x >= 0
		    com	    R16			        ;\
		    com	    R17			        ; \
		    sec				            ;  > R17:R16 = -R17:R16
		    adc	    R16, ZERO	        ; /
		    adc	    R17, ZERO	        ;/

M8X8S_EXIT:
            ret

;-----------------------------------------------------------------------------
;32 bit x 16 bit multiplication (unsigned)
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R19:R18:R17:R16 = x			0..2^32-1
;	        R23:R22 = y			        0,yyyyyyyyyyyyyyyy	0..0,9999847
;Out:	    R19:R18:R17:R16 = x * y		0..2^32-1
;Used:	    SREG,R20-R29
;-----------------------------------------------------------------------------
MUL32X16:
            push	R30
		    clr	    R20		            ;\
		    clr	    R21		            ;/ XX = x
		    clr	    R24		            ;\
		    clr	    R25		            ; \
		    clr	    R26		            ;  \
		    clr	    R27		            ;  / ZZ = 0
		    clr	    R28		            ; /
		    clr	    R29		            ;/
		    rjmp	M3216_CHECK

M3216_LOOP:
            lsr	    R23		            ;\
		    ror	    R22		            ;/ y:Carry = y >> 1
		    brcc	M3216_SKIP
		    add	    R24,R16		        ;\
		    adc	    R25,R17		        ; \
		    adc	    R26,R18		        ;  \
		    adc	    R27,R19		        ;  / ZZ = ZZ + XX
		    adc	    R28,R20		        ; /
		    adc	    R29,R21		        ;/

M3216_SKIP:
            lsl	    R16		            ;\
		    rol	    R17		            ; \
		    rol	    R18		            ;  \
		    rol	    R19		            ;  / YY = YY << 1
		    rol	    R20		            ; /
		    rol	    R21		            ;/

M3216_CHECK:
            mov	    R30,R22		        ;\
		    or	    R30,R23		        ;/ check if y == 0
		    brne	M3216_LOOP
		    mov	    R16,R26		        ;\
    		mov	    R17,R27		        ; \
		    mov	    R18,R28		        ; / x * y
		    mov	    R19,R29		        ;/
		    pop	    R30
		    ret

;-----------------------------------------------------------------------------
; Load 32 bit phase value from ROM
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R30 = index
;Out:	    R19:R18:R17:R16 = value
;Used:	    SREG,R0,R30,R31
;-----------------------------------------------------------------------------
LOAD_32BIT:
            lsl	    R30			        ; R30 *= 2
			mov		R16,R30				; R16 = R30 ( index * 2  )

		    ldi	    ZH, high(DELTA_C)
		    ldi		ZL, low(DELTA_C)	; Z = ROM address

			add		R30,R16				; add (index*2)
		    add	    R30, R30
    		adc	    R31, R31
		    lpm
		    mov	    R16, R0
		    adiw	R30, 1
		    lpm
		    mov	    R17, R0
		    adiw	R30, 1
		    lpm
		    mov	    R18, R0
		    adiw	R30, 1
		    lpm
		    mov	    R19, R0
		    ret

;-----------------------------------------------------------------------------
; Load phase delta from ROM
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R23,R22 = indeks = 0,0..12,0 = n,f
;Out:	    R19:R18:R17:R16 = delta
;Used:	    SREG,R0,R21,R24-R31
;-----------------------------------------------------------------------------
LOAD_DELTA:
            push	R22
		    push	R23
		    mov	    R30, R23
    		rcall	LOAD_32BIT
		    mov	    R24, R16
		    mov	    R25, R17
		    mov	    R26, R18
		    mov	    R27, R19		    ; R27-R24 = delta[n]
		    mov	    R30, R23
		    inc	    R30
		    rcall	LOAD_32BIT
		    sub	    R16, R24
		    sbc	    R17, R25
		    sbc	    R18, R26
		    sbc	    R19, R27
		    push	R24
		    push	R25
		    push	R26
		    push	R27
		    mov	    R23, R22
		    ldi	    R22, 0
		    push	R20
		    rcall	MUL32X16
		    pop	    R20
		    pop	    R27
		    pop	    R26
		    pop	    R25
		    pop	    R24
    		add	    R16, R24
		    adc	    R17, R25
    		adc	    R18, R26
		    adc	    R19, R27
		    pop	    R23
		    pop	    R22
		    ret

;-----------------------------------------------------------------------------
;note number recalculation
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R23 = n	                    0..139 = m12 + 12*n12
;Out:	    R23 = m12                   0..11
;	        R20 = n12                   0..11
;Used:	    SREG
;-----------------------------------------------------------------------------
NOTERECALC:
            ldi	R20,0			        ; n12 = 0
		    rjmp	NRC_2

NRC_1:
            subi	R23, 12			    ; m12 -= 12
		    inc	    R20			        ; n12++

NRC_2:
            cpi	    R23, 12
		    brsh	NRC_1			    ; repeat while m12 >= 12
		    ret

;-----------------------------------------------------------------------------
;read a byte from a table
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R16 = i		                0..255
;	        R31:R30 = &Tab
;Out:	    R0 = Tab[i]	                0..255
;Used:	    SREG,R30,R31
;-----------------------------------------------------------------------------
TAB_BYTE:
            add	    R30, R30			;\
		    adc	    R31, R31		    ;/ Z = 2 * &Tab
		    add	    R30, R16
		    adc	    R31, ZERO
		    lpm
		    ret

;-----------------------------------------------------------------------------
;read a word from a table
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R16 = i			            0..255
;	        R31:R30 = &Tab
;Out:	    R19:R18 = Tab[i]            0..65535
;Used:	    SREG,R0,R30,R31
;-----------------------------------------------------------------------------
TAB_WORD:
            add	    R30, R16
		    adc	    R31, ZERO
		    add	    R30, R30		    ;\
		    adc	    R31, R31		    ;/ Z = 2 * &Tab
		    lpm
		    mov	    R18, R0			    ; LSByte
		    adiw	R30, 1			    ; Z++
		    lpm
		    mov	    R19, R0			    ; MSByte
		    ret

;-----------------------------------------------------------------------------
;"time" --> "rate" conversion
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R16 = time			        0..255
;Out:	    R19:R18:R17:R16 = rate		0x001B0000..0xFFFF0000
;Used:	    SREG,R0,R30,R31
;-----------------------------------------------------------------------------
ADCTORATE:
            lsr	    R16
		    lsr	    R16
		    lsr	    R16			        ;0..31
		    ldi	    R30, TIMETORATE
		    ldi	    R31, 0
		    rcall	TAB_WORD		    ;R19:R18 = rate
		    clr	    R16
		    clr	    R17
		    ret

;-----------------------------------------------------------------------------
;conversion of the "detune B" potentiometer function
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R16 = x		                0..255
;Out:	    R17,R16 = y	                0,000..255,996
;Used:	    SREG,R18-R30
;-----------------------------------------------------------------------------
NONLINPOT:
            ldi	    R22, 0
		    mov	    R23, R16
    		cpi	    R23, 112
		    brlo	NLP_I
		    cpi	    R23, 144
		    brlo	NLP_II
		    rjmp	NLP_III

NLP_I:
            ldi	    R16, 0			    ;\  R18,R17:R16 = m =
		    ldi	    R17, 32			    ; > = 126/112 =
		    ldi	    R18, 1			    ;/  = 1,125
    		ldi	    R30, 0			    ;\ R31,R30 = n =
		    ldi	    R31, 0			    ;/ = 0,0
		    rjmp	NLP_CONT

NLP_II:
            ldi	    R16, 8			    ;\  R18,R17:R16 = m =
		    ldi	    R17, 33			    ; > = (130-126)/(143-112) =
    		ldi	    R18, 0			    ;/  = 0,129032258
		    ldi	    R30, 140		    ;\ R31,R30 = n =
		    ldi	    R31, 111		    ;/ = 126 - m*112 = 111,5483871
		    rjmp	NLP_CONT

NLP_III:
            ldi	    R16, 183		    ;\  R18,R17:R16 = m =
		    ldi	    R17, 29			    ; > = (255-130)/(255-143) =
		    ldi	    R18, 1			    ;/  = 1,116071429
    		ldi	    R30, 103		    ;\ R31,R30 = n =
		    ldi	    R31, 226		    ;/ 255 - m*255 = -29,59821429

NLP_CONT:
            ldi	    R19, 0
		    rcall	MUL32X16
		    add	    R16, R30
		    adc	    R17, R31
		    ret


	

;-------------------------------------------------------------------------------------------------------------------
;			M A I N   P R O G R A M
;-------------------------------------------------------------------------------------------------------------------
RESET:
            cli				            ; disable interrupts

;JTAG Disable - Set JTD in MCSCSR
            lds     R16, MCUCSR         ; Read MCUCSR
            sbr     R16, 1 << JTD       ; Set jtag disable flag
            out     MCUCSR, R16         ; Write MCUCSR
            out     MCUCSR, R16         ; and again as per datasheet

;initialise stack:
			ldi 	R17, high(RAMEND)
		    out	    SPH, R17
  			ldi 	R16, low(RAMEND)
		    out	    SPL, R16


;initialise variables:
		    clr	    ZERO
		    clr	    PHASEA_0
    		clr	    PHASEA_1
		    clr	    PHASEA_2
		    clr	    PHASEB_0
		    clr	    PHASEB_1
		    clr	    PHASEB_2
		    clr	    YN1_L			    ;\
		    clr	    YN1_H			    ;/ y[n-1] = 0,0
		    ldi	    R16, 0
		    sts	    GATE, R16		    ; GATE = 0
		    sts	    GATEEDGE, R16	    ; GATEEDGE = 0
		    sts	    LEVEL, R16		    ; LEVEL = 0
		    sts	    ENV_FRAC_L, R16	    ;\
		    sts	    ENV_FRAC_H, R16	    ; > ENV = 0
		    sts	    ENV_INTEGR, R16	    ;/
		    sts	    ADC_CHAN, R16	    ;ADC_CHAN = 0
		    sts	    NOTE_L, R16		    ;\
		    sts	    NOTE_H, R16		    ; >
		    sts	    NOTE_INTG, R16	    ;/
		    sts	    MIDIPBEND_L, R16    ;\
		    sts	    MIDIPBEND_H, R16    ;/ P.BEND = 0
		    sts	    MIDIMODWHEEL, R16   ; MOD.WHEEL = 0
		    ldi	    R16, 2
		    sts	    PORTACNT, R16	    ; PORTACNT = 2
		    ldi	    R16, 255
		    sts	    LPF_I, R16		    ; no DCF
		    sts	    MIDINOTE, R16	    ; note# = 255
		    sts	    MIDINOTEPREV, R16   ; note# = 255
		    ldi	    R16, 0x5E		    ;\
		    ldi	    R17, 0xB4		    ; \
		    ldi	    R18, 0x76		    ;  \ initialising of
		    sts	    SHIFTREG_0, R16		;  / shift register
		    sts	    SHIFTREG_1, R17		; /
		    sts	    SHIFTREG_2, R18		;/
		    ldi	    R16, 0			    ;\
    		ldi	    R17, 0			    ; > Amin = 0
		    ldi	    R18, 0			    ;/
		    sts	    LFOBOTTOM_0, R16	;\
		    sts	    LFOBOTTOM_1, R17	; > store Amin
		    sts	    LFOBOTTOM_2, R18	;/
		    ldi	    R16, 255		    ;\
		    ldi	    R17, 255		    ; > Amax = 255,999
		    ldi	    R18, 255		    ;/
		    sts	    LFOTOP_0, R16		;\
		    sts	    LFOTOP_1, R17		; > store Amax
		    sts	    LFOTOP_2, R18		;/

;initialise sound parameters:
		    ldi	    R16,0
		    sts	    LFOPHASE, R16		;
		    sts	    ENVPHASE, R16		;
		    sts	    DETUNEB_FRAC, R16	;\
		    sts	    DETUNEB_INTG, R16	;/ detune = 0
		    sts	    LFOLEVEL, R16		;
		    sts	    VCFENVMOD, R16		;
		    ldi	    R16, 84			    ;\
		    sts	    LFOFREQ, R16	    ;/
		    ldi	    R16, 0x18    		;\
		    sts	    MODEFLAGS1, R16		;/ DCO B = on, DCA = env
		    ldi	    R16, 0x10    		;\ LFO = DCO
		    sts	    MODEFLAGS2, R16		;/ ENV mode: A-S-R
		    ldi	    R16, 128		    ;\
		    sts	    ATTACKTIME, R16		; >
		    sts	    RELEASETIME, R16	;/

;initialise port A:
		    ldi	    R16, 0x00    		;\
		    out	    PORTA, R16		    ;/ PA = zzzzzzzz
		    ldi	    R16, 0x00    		;\
		    out	    DDRA, R16		    ;/ PA = iiiiiiii    all inputs (panel pots)

;initialise port B:
		    ldi	    R16, 0xFF    		;\
		    out	    PORTB, R16		    ;/ PB = pppppppp
		    ldi	    R16, 0x00    	    ;\
		    out	    DDRB, R16		    ;/ PB = iiiiiiii    all inputs

;initialise port C:
		    ldi	    R16, 0x00     	    ;\
		    out	    PORTC, R16		    ;/ PC = 00000000
    		ldi	    R16, 0xFF    		;\
		    out	    DDRC, R16		    ;/ PC = oooooooo    all outputs (DAC)

;initialise port D:
		    ldi	    R16, 0xFC    		;\
		    out	    PORTD, R16		    ;/ PD = 1111110z
		    ldi	    R16, 0xFE    		;\
		    out	    DDRD, R16		    ;/ PD = oooooooi    all outputs except PD0 (MIDI-IN)

			ldi		R16,0xff
		    out	    DDRE, R16		    ;/ PC = oooooooo    all outputs (DAC)
			sbi	    PORTE, 1		    ; LED on
;initialise Timer0:
;		    ldi	    R16, 0x0A    		;\
;		    out	    TCCR0, R16		    ;/ stop Timer 0

;initialise Timer1:
		    ldi	    R16, 0x04    		;\ prescaler = CK/256
		    out	    TCCR1B, R16		    ;/ (clock = 32탎)

;initialise Timer2:
            ldi     R16, 31             ;\
            out     OCR2, R16           ;/ OCR2 = 31
            ldi     R16, 0x0A           ;\ clear timer on compare,
            out     TCCR2, R16          ;/ set prescaler = CK/8

;initialise UART:
		    ldi	    R16, high((cpu_frequency / (baud_rate * 16)) - 1)
		    sts	    UBRR0H, R16
    		ldi	    R16, low((cpu_frequency / (baud_rate * 16)) - 1)
            out     UBRR0L, R16

; enable receiver and receiver interrupt
    		ldi	    R16, (1<<RXCIE0)|(1<<RXEN0)   ;\
		    out	    UCSR0B, R16		            ;/ RXCIE=1, RXEN=1

;initialise ADC:
		    ldi	    R16, 0x86    		;\
		    out	    ADCSRA, R16		    ;/ ADEN=1, clk = 125 kHz

;initialise interrupts:
		    ldi	    R16, 0x80    		;\
		    out	    TIMSK, R16		    ;/ OCIE2=1

    		sei				            ; Interrupt Enable

;start conversion of the first A/D channel:
		    lds	    R18, ADC_CHAN
		    rcall	ADC_START

;initialize the keyboard scan time 050419:
		    in	R16, TCNT1L		        ;\
		    in	R17, TCNT1H		        ;/ R17:R16 = TCNT1 = t
		    sts	TPREV_KBD_L, R16
		    sts	TPREV_KBD_H, R17

;main loop:

MAINLOOP:
            ;-------------
            ;switches:
            ;-------------
;begin:
		    in	    R16, TCNT1L		    ;\
		    in	    R17, TCNT1H		    ;/ R17:R16 = t
		    lds	    R18, TPREV_KBD_L	;\
		    lds	    R19, TPREV_KBD_H	;/ R19:R18 = t0
		    sub	    R16, R18			;\
		    sbc	    R17, R19			;/ R17:R16 = t - t0
		    subi	R16, LOW(KBDSCAN)	;\
		    sbci	R17, HIGH(KBDSCAN)	;/ R17:R16 = (t-t0) - 100ms
		    brsh	MLP_SCAN		    ;\
		    rjmp	MLP_SKIPSCAN		;/ skip scanning if (t-t0) < 100ms

MLP_SCAN:
            in	    R16, TCNT1L
		    in	    R17, TCNT1H
		    sts	    TPREV_KBD_L, R16	;\
		    sts	    TPREV_KBD_H, R17	;/ t0 = t

;reading:
    		ldi	    R16, 0x10    		; inverted state of PD outputs
		    ldi	    R17, 0x01    		; mask
		    ldi	    R18, 0x10    		; mask
		    ldi	    R19, 0x00    		; bits of SWITCH1
		    ldi	    R20, 0x00    		; bits of SWITCH2
		    ldi	    R21, 0x00			; bits of SWITCH3

MLP_SWLOOP:
            in	    R30, PORTD
		    ori	    R30, 0xF0
		    eor	    R30, R16
		    out	    PORTD, R30          ; `set' keyboard ROW to scan
		    rcall	WAIT_10US
		    in	    R30, PINB           ; `read' keyboard COL for key status
		    sbrs	R30, 0			    ;\
		    or	    R19, R17		    ;/ set bit when PB0==0
		    sbrs	R30, 1			    ;\
		    or	    R19, R18		    ;/ set bit when PB1==0
		    sbrs	R30, 2			    ;\
		    or	    R20, R17		    ;/ set bit when PB2==0
		    sbrs	R30, 3			    ;\
		    or	    R20, R18		    ;/ set bit when PB3==0
		    sbrs	R30, 4				;\
		    or	    R21, R17	        ;/ set bit when PB4==0
		    lsl	    R17
		    lsl	    R18
		    lsl 	R16
		    brne	MLP_SWLOOP
		    ldi	    R16, 0xFC    		;\
            out     PORTD, R16          ;/ just resets the ROW selector bits
		    sts	    SWITCH1, R19
		    sts	    SWITCH2, R20
    		sts	    SWITCH3, R21		; V04
#if 1

;service:
		    lds	    R18, SWITCH1
		    lds	    R16, OLDSWITCH1

		    lds	    R19, SWITCH2
		    lds	    R17, OLDSWITCH2
			
			cp		r18,r16				; if same , dont process.
			brne	scan1

scan:
			
			cp		r19,r17				; if same , dont process.
			brne	scan1


		    lds	    R18, MODEFLAGS1
		    lds	    R19, MODEFLAGS2
		    lds	    R16, SWITCH1
		    lds	    R17, SWITCH2

			rjmp	skip_keyscan


scan1:

			sts		OLDSWITCH1,R18
			sts		OLDSWITCH2,R19

		    lds	    R18, MODEFLAGS1
		    lds	    R19, MODEFLAGS2
		    lds	    R16, SWITCH1
		    lds	    R17, SWITCH2

			push r20

		    bst	    R16, 0	 			;\ BIT 0 from R16 into T, T into bit 7 of R19
		    brtc	next1				; Is T set?

			; invert T
			ldi		R20,1<<7
			eor		R19, R20

next1:
;			bld	    R19, 7	 			;/ PD4.PB0. SW16 LFO normal/random

  		    bst	    R16, 1		        ;\
		    brtc	next2				; Is T set?

			; invert T
			ldi		R20,1<<1
			eor		R19, R20



next2:
;		    bld	    R19, 1		        ;/ PD5.PB0. SW15 LFO Wave tri/squ

		    bst	    R16, 2		        ;\
		    brtc	next3				; Is T set?
			; invert T

			ldi		R20,1<<4
			eor		R19, R20


next3:

;		    bld	    R19, 4		        ;/ PD6.PB0. SW14 Control knob shift

		    bst	    R16, 3		        ;\
		    brtc	next4				; Is T set?

			ldi		R20,1<<0
			eor		R18, R20




next4:
;		    bld	    R18, 0		        ;/ PD7.PB0. SW13 DCO Distortion off/on
		    bst	    R16, 4		        ;\
		    brtc	next5				; Is T set?

			ldi		R20,1<<6
			eor		R19, R20




next5:
 ;   		bld	    R19, 6		        ;/ PD4.PB1. SW12 LFO keyboard sync off/on

		    bst	    R16, 5		        ;\
		    brtc	next6				; Is T set?

			ldi		R20,1<<0
			eor		R19, R20



next6:
;		    bld	    R19, 0		        ;/ PD5.PB1. SW11 LFO Mode 0=DCF, 1 = DCO


		    bst	    R16, 6		        ;\
		    brtc	next7				; Is T set?

			ldi		R20,1<<3
			eor		R19, R20



next7:
;		    bld	    R19, 3		        ;/ PD6.PB1. SW10 DCF mode 0=LP, 1=HP

    		bst	    R16, 7		        ;\
		    brtc	next8				; Is T set?

			ldi		R20,1<<2
;			eor		R19, R20

next8:

;		    bld	    R19, 2		        ;/ PD7.PB1. SW9  DCF key track 0=off, 1=on


		    bst	    R17, 0		        ;\
		    brtc	next9				; Is T set?

			ldi		R20,1<<4
			eor		R18, R20



next9:
;		    bld	    R18, 4		        ;/ PD4.PB2. SW8  DCA gate/env

		    bst	    R17, 1		        ;\
		    brtc	next10				; Is T set?

			ldi		R20,1<<6
			eor		R18, R20



next10:
;		    bld	    R18, 6		        ;/ PD5.PB2. SW7  Osc A Noise
		    bst	    R17, 2		        ;\
		    brtc	next11				; Is T set?

			ldi		R20,1<<7
			eor		R18, R20



next11:
;		    bld	    R18, 7		        ;/ PD6.PB2. SW6  Octave B down/up
		    bst	    R17, 3		        ;\
		    brtc	next12				; Is T set?

			ldi		R20,1<<2
			eor		R18, R20



next12:
;		    bld	    R18, 2		        ;/ PD7.PB2. SW5  Osc B wave saw/square

		    bst	    R17, 4		        ;\
		    brtc	next13				; Is T set?

			ldi		R20,1<<5
			eor		R18, R20


next13:

;		    bld	    R18, 5		        ;/ PD4.PB3. SW4  Transpose down/up

		    bst	    R17, 5		        ;\
		    brtc	next14				; Is T set?

			ldi		R20,1<<5
			eor		R19, R20



next14:
;		    bld	    R19, 5		        ;/ PD5.PB3. SW3  Modwheel disable/enable
		    bst	    R17, 6		        ;\
		    brtc	next15				; Is T set?

			ldi		R20,1<<3
			eor		R18, R20



next15:
;		    bld	    R18, 3		        ;/ PD6.PB3. SW2  Osc B on/off
		    bst	    R17, 7		        ;\
		    brtc	next16				; Is T set?

			ldi		R20,1<<1
			eor		R18, R20



next16:
;		    bld	    R18, 1		        ;/ PD7.PB3. SW1  Osc A wave saw/square


			pop		r20

		    sts	    MODEFLAGS1, R18
		    sts	    MODEFLAGS2, R19



#else
;service:
		    lds	    R16, SWITCH1
		    lds	    R17, SWITCH2
		    lds	    R18, MODEFLAGS1
		    lds	    R19, MODEFLAGS2

		    bst	    R16, 0		        ;\
		    bld	    R18, 0		        ;/ PD4.PB0. SW16 DCO Distortion off/on

   		    bst	    R16, 1		        ;\
		    bld	    R19, 5		        ;/ PD5.PB0. SW15 MODWHEEL disable/enable

		    bst	    R16, 2		        ;\
		    bld	    R19, 6		        ;/ PD6.PB0. SW14 LFO KBD SYNC off/on

		    bst	    R16, 3		        ;\
		    bld	    R19, 7		        ;/ PD7.PB0. SW13 LFO norm/rand

		    bst	    R16, 4		        ;\
    		bld	    R19, 3		        ;/ PD4.PB1. SW12 ENV LOOP off/on

		    bst	    R16, 5		        ;\
		    bld	    R18, 5		        ;/ PD5.PB1. SW11 transpose down/up

		    bst	    R16, 6		        ;\
		    bld	    R18, 6		        ;/ PD6.PB1. SW10 octave A down/up

    		bst	    R16, 7		        ;\
		    bld	    R18, 7		        ;/ PD7.PB1. SW9  octave B down/up

		    bst	    R17, 0		        ;\
		    bld	    R18, 1		        ;/ PD4.PB2. SW8  DCA gate/env

		    bst	    R17, 1		        ;\
		    bld	    R18, 2		        ;/ PD5.PB2. SW7  DCO B off/on

		    bst	    R17, 2		        ;\
		    bld	    R18, 3		        ;/ PD6.PB2. SW6  Wave B saw/squ

		    bst	    R17, 3		        ;\
		    bld	    R18, 4		        ;/ PD7.PB2. SW5  Wave A saw/squ

		    bst	    R17, 4		        ;\
		    bld	    R19, 4		        ;/ PD4.PB3. SW4  ENV AR/ASR

		    bst	    R17, 5		        ;\
		    bld	    R19, 0		        ;/ PD5.PB3. SW3  LFO DCO/DCF

		    bst	    R17, 6		        ;\
		    bld	    R19, 1		        ;/ PD6.PB3. SW2  LFO TRI/SQU

		    bst	    R17, 7		        ;\
		    bld	    R19, 2		        ;/ PD7.PB3. SW1  DCF KBD TRACK

		    sts	    MODEFLAGS1, R18
		    sts	    MODEFLAGS2, R19
#endif

skip_keyscan:

; MIDI channel will be set:
		    lds	    R16, SWITCH3
		    sts	    SETMIDICHANNEL, R16	; 0 for OMNI or 1..15

            ;---------------
            ;potentiometers:
            ;---------------
MLP_SKIPSCAN:
;reading
		    rcall	ADC_END			    ; R16 = AD(i)
		    lds	    R18, ADC_CHAN		;\
		    ldi	    R28, low(ADC_0)		    ; \  ADC_0 is outside range for LDI
			ldi		R29, high( ADC_0)


		    add	    R28, R18		    ; / Y = &ADC_i
			adc		R29,zero

		    st	    Y, R16			    ; AD(i) --> ADC_i

		    ldi	    R29, 0			    ;/

;next channel:
		    inc	    R18
		    andi	R18, 0x07
		    sts	    ADC_CHAN,R18
		    rcall	ADC_START	        ; start conversion of next channel

;service:
		    lds	    R16, ADC_0		    ;\
		    sts	    LFOFREQ, R16	    ;/ AD0 --> LFO rate

		    lds	    R16, ADC_1		    ;\
		    sts	    LFOLEVEL, R16		;/ AD1 --> LFO level

		    lds	    R16, ADC_2		    ;\
		    rcall	NONLINPOT		    ; \
		    subi	R17, 128		    ;  > AD2 --> tune B
		    sts	    DETUNEB_FRAC, R16	; /  -128.000..+127.996
		    sts	    DETUNEB_INTG, R17	;/

		    lds	    R16, ADC_3		    ;\
		    sts	    CUTOFF,R16		    ;/ AD3 --> DCF cutoff

		    lds	    R16, ADC_4		    ;\
		    sts	    ATTACKTIME, R16		;/ AD4 --> ATTACK TIME

		    lds	    R16, ADC_5		    ;\
		    sts	    RELEASETIME, R16	;/ AD5 --> RELEASE TIME

		    lds	    R16, ADC_6		    ;\
		    sts	    PORTAMENTO, R16		;/ AD6 --> PORTAMENTO

		    lds	    R16, ADC_7		    ;\
		    sts	    VCFENVMOD, R16		;/ AD7 --> DCF ENV MOD

            ;-------------
            ;calculate dT:
            ;-------------
		    in	    R22, TCNT1L		    ;\
		    in	    R23, TCNT1H		    ;/ R23:R22 = TCNT1 = t
		    mov	    R18, R22		    ;\
    		mov	    R19, R23		    ;/ R19:R18 = t
		    lds	    R16, TPREV_L	    ;\
		    lds	    R17, TPREV_H	    ;/ R17:R16 = t0
		    sub	    R22, R16		    ;\ R23:R22 = t - t0 = dt
		    sbc	    R23, R17		    ;/ (1 bit = 32 탎)
		    sts	    TPREV_L, R18	    ;\
		    sts	    TPREV_H, R19	    ;/ t0 = t
    		sts	    DELTAT_L, R22		;\
		    sts	    DELTAT_H, R23		;/ R23:R22 = dT

            ;----
            ;LFO:
            ;----
;calculate dA:
		    lds	    R16, LFOFREQ	    ;\
		    com	    R16			        ;/ R16 = 255 - ADC0
		    rcall	ADCTORATE           ; R19:R18:R17:R16 = rate of rise/fall
		    lds	    R22, DELTAT_L		;\
    		lds	    R23, DELTAT_H		;/ R23:R22 = dT
		    rcall	MUL32X16		    ; R18:R17:R16 = dA
		    lds	    R19, LFO_FRAC_L
		    lds	    R20, LFO_FRAC_H
    		lds	    R21, LFO_INTEGR
		    subi    R21, 128
		    ldi	    R31, 0			    ; flag = 0
		    lds	    R30, LFOPHASE
		    tst	    R30
		    brne	MLP_LFOFALL

;rising phase:

MLP_LFORISE:
            lds	    R22, LFOTOP_0		;\
		    lds	    R23, LFOTOP_1		; > R24:R23:R22 = Amax
		    lds	    R24, LFOTOP_2		;/
		    add	    R19, R16		    ;\
    		adc	    R20, R17		    ; > A += dA
		    adc	    R21, R18		    ;/
		    brcs	MLP_LFOTOP
		    cp	    R19, R22		    ;\
		    cpc	    R20, R23		    ; > A - Amax
		    cpc	    R21, R24		    ;/
		    brlo	MLP_LFOX		    ; skip when A < Amax

;A reached top limit:

MLP_LFOTOP:
            mov	    R19, R22		    ;\
		    mov	    R20, R23		    ; > A = Amax
		    mov	    R21, R24		   	;/
		    ldi	    R30, 1			    ; begin of falling
		    ldi	    R31, 1			    ; flag = 1
		    rjmp	MLP_LFOX

;falling phase:

MLP_LFOFALL:
            lds	    R22, LFOBOTTOM_0	;\
		    lds	    R23, LFOBOTTOM_1	; > R24:R23:R22 = Amin
		    lds	    R24, LFOBOTTOM_2	;/
    		sub	    R19, R16		    ;\
		    sbc	    R20, R17		    ; > A -= dA
		    sbc	    R21, R18		    ;/
		    brcs	MLP_LFOBOTTOM
		    cp	    R22, R19		    ;\
		    cpc	    R23, R20		    ; > Amin - A
		    cpc 	R24, R21		    ;/
		    brlo	MLP_LFOX		    ; skip when A > Amin

;A reached bottom limit:

MLP_LFOBOTTOM:
            mov	    R19, R22		    ;\
		    mov	    R20, R23		    ; > A = Amin
		    mov	    R21, R24		    ;/
		    ldi	    R30, 0			    ; begin of rising
		    ldi	    R31, 1			    ; flag = 1

MLP_LFOX:
            sts	    LFOPHASE, R30
		    subi	R21, 128		    ; R21,R20:R19 = LFO tri wave
		    sts	    LFO_FRAC_L, R19		;\
		    sts	    LFO_FRAC_H, R20		; > store LFO value
    		sts	    LFO_INTEGR, R21		;/

;switch norm/rand:

;determine Amin i Amax:
		    ldi	    R16, 0			    ;\
		    ldi	    R17, 0			    ; > Amin when not LFO==tri
    		ldi	    R18, 0			    ;/  and not LFO==rand
		    lds	    R30, MODEFLAGS2
		    andi	R30, 0x82
		    cpi	    R30, 0x80    		; Z = ((LFO==tri)&&(LFO==rand))
    		brne	MLP_LFOAWR
		    tst	    R31
    		breq	MLP_LFOAX
		    lds	    R16, SHIFTREG_0		;\
		    lds	    R17, SHIFTREG_1		; \ Amin = pseudo-random number
		    lds	    R18, SHIFTREG_2		; /	0,000..127,999
		    andi	R18, 0x7F		    ;/

MLP_LFOAWR:
            sts	    LFOBOTTOM_0, R16	;\
		    sts	    LFOBOTTOM_1, R17	; > store Amin
		    sts	    LFOBOTTOM_2, R18	;/
		    com	    R16			        ;\
		    com	    R17			        ; > Amax = 255,999 - Amin
		    com	    R18			        ;/	128,000..255,999
		    sts	    LFOTOP_0, R16		;\
		    sts	    LFOTOP_1, R17		; > store Amax
		    sts	    LFOTOP_2, R18		;/

MLP_LFOAX:
		    lds	    R16, MODEFLAGS2
		    andi	R16, 0x82
		    cpi	    R16, 0x82    		; Z = ((LFO==squ)&&(LFO==rand))
		    brne	MLP_LFONORM
		    tst	    R31			        ; flag == 1 ?
		    breq	MLP_LFONWR		    ; jump when not
		    lds	    R21, SHIFTREG_2
		    rjmp	MLP_LFOWR

MLP_LFONORM:

;switch tri/squ:
		    lds	    R16, MODEFLAGS2		;\ Z=0: squ
		    andi	R16, 0x02    		;/ Z=1: tri
    		breq	MLP_LFOWR
		    lsl	    R21			        ; Cy = (LFO < 0)
		    ldi	    R21, 127		    ;\
		    adc	    R21, ZERO		    ;/ R21 = -128 or +127

MLP_LFOWR:
            sts	    LFOVALUE, R21

MLP_LFONWR:
            lds	    R21, LFOVALUE

;determine LFO level:
            lds	    R16, MODEFLAGS2
            andi	R16, 0x20
            breq	MLP_LFOMWX		    ; skip when MOD.WHEEL = off
		    lds	    R16, LFOLEVEL
		    lds	    R17,MIDIMODWHEEL
		    cp	    R16, R17
    		brsh	MLP_LFOLWR
		    mov	    R16, R17		    ; MOD.WHEEL is greater

MLP_LFOLWR:
            sts	    LFOLEVEL, R16

MLP_LFOMWX:
            ;----
            ;ENV:
            ;----
;check envelope phase:
		    lds	    R17, ENVPHASE
		    lds	    R16, ATTACKTIME
    		cpi	    R17, 1
		    breq    MLP_ENVAR		    ; when "attack"
		    lds	    R16, RELEASETIME
		    cpi	    R17, 3
		    breq	MLP_ENVAR		    ; when "release"
		    rjmp	MLP_EEXIT		    ; when "stop" or "sustain"

;calculate dL:

MLP_ENVAR:
            rcall	ADCTORATE           ; R19:R18:R17:R16 = rate of rise/fall
		    lds	    R22, DELTAT_L		;\
		    lds	    R23, DELTAT_H		;/ R23:R22 = dT
		    rcall	MUL32X16		    ; R18:R17:R16 = dL

;add/subtract dL to/from L:
		    lds	    R19, ENV_FRAC_L		;\
		    lds	    R20, ENV_FRAC_H		; > R21:R20:R19 = L
    		lds	    R21, ENV_INTEGR		;/
		    lds	    R22, ENVPHASE
		    cpi	    R22, 3
		    breq    MLP_ERELEASE

MLP_EATTACK:
		    add	    R19, R16		    ;\
		    adc	    R20, R17		    ; > R21:R20:R19 = L + dL
		    adc	    R21, R18		    ;/
		    brcc	MLP_ESTORE

;L reached top limit:
		    ldi	    R19, 255		    ;\
		    ldi	    R20, 255		    ; > L = Lmax
		    ldi	    R21, 255		    ;/
		    ldi	    R16, 3			    ; when A-R or loop: release
		    lds	    R17, MODEFLAGS2		;\
		    bst	    R17, 3			    ;/ T = 0 (norm) / 1 (loop)
		    brts	MLP_ESTOREP
		    lds	    R17, MODEFLAGS2		;\
		    bst	    R17, 4			    ;/ T = 0 (AR) / 1 (ASR)
		    brtc	MLP_ESTOREP
		    ldi	    R16, 2			    ; when A-S-R and norm: sustain
		    rjmp	MLP_ESTOREP

MLP_ERELEASE:
            sub	    R19, R16		    ;\
		    sbc	    R20, R17		    ; > R21:R20:R19 = L - dL
		    sbc	    R21, R18		    ;/
		    brcc	MLP_ESTORE

;L reached bottom limit:
		    ldi	    R19, 0			    ;\
		    ldi	    R20, 0			    ; > L = 0
		    ldi	    R21, 0			    ;/
		    ldi	    R16, 1			    ; when loop: attack
		    lds	    R17, MODEFLAGS2		; R17.3 = 0 (norm) / 1 (loop)
		    sbrs	R17, 3			    ; skip when loop
		    ldi	    R16, 0			    ; stop

MLP_ESTOREP:
            sts	ENVPHASE, R16		    ; store phase

MLP_ESTORE:
            sts	    ENV_FRAC_L, R19		;\
		    sts	    ENV_FRAC_H, R20		; > store L
		    sts	    ENV_INTEGR, R21		;/

MLP_EEXIT:
            ;-----
            ;GATE:
            ;-----
		    lds	    R16, GATE
		    tst	    R16			        ; check GATE
		    brne	MLP_KEYON

;no key is pressed:

MLP_KEYOFF:
            ldi	    R16,3			    ;\
		    sts	    ENVPHASE, R16		;/ "release"
		    rjmp	MLP_NOTEON

;key is pressed:

MLP_KEYON:
            lds	    R16, GATEEDGE
		    tst	    R16		            ; Z=0 when key has just been pressed
		    breq	MLP_NOTEON

;key has just been pressed:
		    ldi	    R16, 0			    ;\
		    sts	    GATEEDGE, R16		;/ GATEEDGE = 0
		    lds	    R16, PORTACNT		;\
		    tst	    R16			        ; \
		    breq	MLP_KEYON1		    ;  > if ( PORTACNT != 0 )
		    dec	    R16			        ; /    PORTACNT--
		    sts	    PORTACNT, R16		;/

MLP_KEYON1:

;envelope starts:
		    ldi	    R16, 1			    ;\
		    sts	    ENVPHASE, R16		;/ attack
		    ldi	    R16, 0
		    sts	    ENV_FRAC_L, R16		;\
		    sts	    ENV_FRAC_H, R16		; > ENV = 0
		    sts	    ENV_INTEGR, R16		;/

; LFO starts (only when LFO KBD SYNC = on):
		    lds	    R16, MODEFLAGS2
		    andi	R16, 0x40
		    breq	MLP_NOTEON		    ; skip when LFO KBD SYNC = off
		    ldi	    R16, 255		    ;\
		    ldi	    R17, 255		    ; > A = Amax
		    ldi	    R18, 127		    ;/
		    sts	    LFO_FRAC_L, R16		;\
		    sts	    LFO_FRAC_H, R17		; > store A
		    sts	    LFO_INTEGR, R18		;/
		    ldi	    R16, 1			    ;\
		    sts	    LFOPHASE, R16		;/ begin of falling

MLP_NOTEON:
            ;-------------
            ;DCO A, DCO B:
            ;-------------
		    ldi	    R25, 0			    ;\
		    ldi	    R22, 0			    ; > R23,R22:R25 = note# 0..127
		    lds	    R23, MIDINOTE		;/
		    cpi	    R23, 255
		    brne	MLP_NLIM2
		    rjmp	MLP_VCOX

;note# limited to 36..96:

MLP_NLIM1:
            subi	R23, 12

MLP_NLIM2:
            cpi	    R23, 97
		    brsh	MLP_NLIM1
		    rjmp	MLP_NLIM4

MLP_NLIM3:
            subi	R23, 244

MLP_NLIM4:
            cpi	    R23, 36
		    brlo	MLP_NLIM3

;transpose 1 octave down:
		    subi	R23, 12			    ; n -= 12		24..84

;portamento:
		    lds	    R25, NOTE_L		    ;\
		    lds	    R26, NOTE_H		    ; > R27,R26:R25 = nCurr
		    lds	    R27, NOTE_INTG		;/
		    lds	    R16, PORTACNT		;\
    		tst	    R16			        ; > jump when it's the first note
		    brne	MLP_PORTAWR	        ;/  (PORTACNT != 0)
		    lds	    R16, PORTAMENTO
    		rcall	ADCTORATE
		    push    R22
		    push	R23
		    mov	    R22, R18		    ;\ R23:R22 = portamento rate
		    mov	    R23, R19		    ;/ 65535..27
		    ldi	    R16, 0
		    ldi	    R17, 0
		    lds	    R18, DELTAT_L
		    lds	    R19, DELTAT_H
		    ldi	    R20, 3
		    rcall	SHR32
		    rcall	MUL32X16		    ; R18,R17:R16 = nDelta
		    pop	    R23
		    pop	    R22
		    mov	    R19, R16		    ;\
		    mov	    R20, R17		    ; > R21,R20:R19 = nDelta
		    mov	    R21, R18		    ;/
		    lds	    R25, NOTE_L		    ;\
		    lds	    R26, NOTE_H		    ; > R27,R26:R25 = nCurr
		    lds	    R27, NOTE_INTG		;/
		    cp	    R22, R26		    ;\ nEnd - nCurr
		    cpc	    R23, R27		    ;/ Cy = (nEnd < nCurr)
		    brsh	MLP_PORTAADD

MLP_PORTAMIN:
            sub	    R25, R19			;\
		    sbc	    R26, R20			; > nCurr -= nDelta
		    sbc	    R27, R21			;/
		    cp	    R22, R26			;\ nEnd - nCurr;
		    cpc	    R23, R27		    ;/ Cy = (nEnd < nCurr)
		    brlo	MLP_PORTA1
		    rjmp	MLP_PORTAEND

MLP_PORTAADD:
            add	    R25, R19		    ;\
		    adc	    R26, R20		    ; > nCurr += nDelta
		    adc	    R27, R21		    ;/
		    cp	    R22, R26		    ;\ nEnd - nCurr;
		    cpc	    R23, R27		    ;/ Cy = (nEnd < nCurr)
		    brsh	MLP_PORTA1

MLP_PORTAEND:
            ldi	    R25, 0			    ;\
		    mov	    R26, R22			; > nCurr = nEnd
    		mov	    R27, R23			;/

MLP_PORTA1:
            mov	    R22, R26
		    mov	    R23, R27

MLP_PORTAWR:
        	sts	NOTE_L, R25
		    sts	    NOTE_H, R22
		    sts	    NOTE_INTG, R23

;"transpose" switch:
		    lds	    R16, MODEFLAGS1		; b5 = transpose: 0=down, 1=up
		    sbrs	R16, 5			    ;\			24..96
		    subi	R23, 12		        ;/ n -= 12	12..84

;pitch bender (-12..+12):
;;          lds	    R16, MODEFLAGS2
;;          andi	R16, 0x10
;;          breq	MLP_PBX			    ; skip when P.BEND = off
		    lds	    R16, MIDIPBEND_L	;\ R17,R16 = P.BEND
    		lds	    R17, MIDIPBEND_H	;/	-128,000..+127,996
		    ldi	    R18, 5			    ;\ R17,R16 = P.BEND/32
		    rcall	ASR16			    ;/	-4,000..+3,999
		    mov	    R18, R16		    ;\ R19,R18 = P.BEND/32
		    mov	    R19, R17		    ;/	-4,000..+3,999
		    add	    R16, R18		    ;\ R17,R16 = 2/32*P.BEND
		    adc	    R17, R19		    ;/	-8,000..+7,999
		    add	    R16, R18		    ;\ R17,R16 = 3/32*P.BEND
		    adc	    R17, R19		    ;/	-12,000..+11,999
		    add	    R22, R16		    ;\
		    adc	    R23, R17		    ;/ add P.BEND

MLP_PBX:
;for "DCF KBD TRACK":
		    sts	    PITCH, R23		    ; n = 0..108

;LFO modulation:
		    lds	    R16, MODEFLAGS2		;\
		    andi	R16, 0x01    		;/ Z=1 when LFO=DCO
		    brne	MLP_VCOLFOX		    ; skip when LFO=DCF
		    lds	    R16, LFOVALUE		; R16 = LFO	    -128..+127
    		lds	    R17, LFOLEVEL		; R17 = LFO level	0..255

;nonlinear potentiometer function:
		    mov	    R18, R17		    ; R18 = LL
		    lsr	    R17			        ; R17 = LL/2
		    cpi	    R18, 128
		    brlo	MLP_OM1			    ; skip if LL = 0..127
		    subi	R17, 128		    ; R17 = 0,5*LL-128    -64..-1
		    add	    R17, R18		    ; R17 = 1,5*LL-128    +64..254

MLP_OM1:
            rcall	MUL8X8S			    ; R17,R16 = LFO*mod
		    ldi	    R18, 4			    ;\
		    rcall	ASR16			    ;/ R17,R16 = LFO*mod / 16
		    add	    R22, R16		    ;\
		    adc	    R23, R17		    ;/ add LFO to note #

;limiting to 0..108
		    tst	    R23
		    brpl	MLP_VCOLFO1
		    ldi	    R22, 0
		    ldi	    R23, 0
		    rjmp	MLP_VCOLFOX

MLP_VCOLFO1:
            cpi	    R23, 109
		    brlo	MLP_VCOLFOX
		    ldi	    R22, 0
		    ldi	    R23, 108

MLP_VCOLFOX:
            push	R22			        ;\ note# = 0..108
		    push	R23			        ;/ store for phase delta B

;phase delta A:
;octave A:
		    lds	    R16, MODEFLAGS1		; b6 = octave A: 0=down, 1=up
		    sbrc	R16, 6
		    subi	R23, 244		    ; n += 12
		    rcall	NOTERECALC		    ; R23,R22 = m12 (0,0..11,996),
						                ; R20 = n12 (0..11)
		    rcall	LOAD_DELTA		    ; R19:R18:R17:R16 = delta
		    rcall	SHL32			    ; R19:R18:R17:R16 = delta*(2^exp)

;           Enabling these 3 lines will lock DCO A to 440Hz (?)
;           ldi     R17, 0xBF
;           ldi     R18, 0x9A
;           ldi     R19, 0x03

		    mov	    DELTAA_0, R17
		    mov	    DELTAA_1, R18
		    mov	    DELTAA_2, R19

;phase delta B:
		    pop	    R23			        ;\
		    pop	    R22			        ;/ n

;detune B:
		    lds	    R16, DETUNEB_FRAC	;\ R17,R16 = detuneB
		    lds	    R17, DETUNEB_INTG	;/ -128,000..+127,996
		    ldi	    R18, 4			    ;\ R17,R16 = detuneB / 16
    		rcall	ASR16			    ;/ -8,0000..+7,9998
		    add	    R22, R16		    ;\
		    adc	    R23, R17		    ;/

;octave B:
            lds	    R16, MODEFLAGS1		; b7 = octave B: 0=down, 1=up
		    sbrc	R16, 7
		    subi	R23, 244		    ; n += 12
		    rcall	NOTERECALC		    ; R23,R22 = m12 (0,0..11,996),
						                ; R20 = n12 (0..11)
		    rcall	LOAD_DELTA		    ; R19:R18:R17:R16 = delta
		    rcall	SHL32			    ; R19:R18:R17:R16 = delta*(2^exp)
		    mov	    DELTAB_0, R17
		    mov	    DELTAB_1, R18
		    mov	    DELTAB_2, R19

MLP_VCOX:
            ;----
            ;DCF:
            ;----
	        ;LFO mod:
		    ldi	    R30, 0			    ;\
		    ldi	    R31, 0			    ;/ sum = 0
		    lds	    R16, MODEFLAGS2		;\
		    andi	R16, 0x01    		;/ Z=1 when LFO=DCO
		    breq	MLP_DCF0		    ; skip when LFO=DCO
		    lds	    R16, LFOVALUE		; R16 = LFO	    -128..+127
		    lds	    R17, LFOLEVEL		; R17 = DCF LFO MOD	0..255
		    rcall	MUL8X8S			    ; R17,R16 = LFO * VCFLFOMOD
		    mov	    R30, R17
		    ldi	    R31, 0
		    rol	    R17			        ; R17.7 --> Cy (sign)
		    sbc	    R31, R31		    ; sign extension to R31

MLP_DCF0:

;ENV mod:
            lds	    R16, ENV_INTEGR
		    lds	    R17, VCFENVMOD
		    rcall	MUL8X8U			    ; R17,R16 = ENV * ENVMOD
    		rol	    R16			        ; Cy = R16.7 (for rounding)
		    adc	    R30, R17
		    adc	    R31, ZERO

;KBD TRACK:
            lds	    R16, MODEFLAGS2		;\ Z=0: KBD TRACK on
		    andi	R16, 0x04    		;/ Z=1: KBD TRACK off
		    breq	MLP_DCF3
		    lds	    R16, PITCH		    ; R16 = n (12/octave)	0..96
		    lsl	    R16			        ; R16 = 2*n (24/octave)	0..192
		    subi	R16, 96	        	; R16 = 2*(n-48) (24/octave)   -96..+96
		    ldi	    R17, 171
		    rcall	MUL8X8S		        ; R17 = 1,5*(n-48) (16/octave) -64..+64
		    ldi	    R18, 0			    ;\
		    sbrc	R17, 7			    ; > R18 = sign extension
		    ldi	    R18, 255		    ;/  of R17
		    add	    R30, R17
		    adc	    R31, R18

MLP_DCF3:
;CUTOFF:
		    lds	    R16, CUTOFF
		    clr	    R17
		    add	    R16, R30
    		adc	    R17, R31
		    tst	    R17
		    brpl	MLP_DCF1
		    ldi	    R16, 0
		    rjmp	MLP_DCF2

MLP_DCF1:
            breq	MLP_DCF2
		    ldi	    R16, 255

MLP_DCF2:
            lsr	    R16
		    lsr	    R16			        ; 0..63
		    ldi	    R30, TAB_VCF	    ;\
    		ldi	    R31, 0			    ;/ Z = &Tab
		    rcall	TAB_BYTE		    ; R0 = 1.. 255
		    sts	    LPF_I, R0

            ;---------------
            ;sound level:
            ;---------------
		    lds	    R17, MODEFLAGS1		;\ check DCA mode:
		    andi	R17, 0x10    		;/ Z=1 (gate), Z=0 (env)
		    brne	MLP_VCAENV		    ; jump when mode==env
		    lds	    R16, GATE		    ;\
		    ror	    R16			        ;/ GATE --> Cy
		    ldi	    R16, 0			    ;\ R16 =   0 (when GATE == 0),
		    sbc	    R16, R16		    ;/ R16 = 255 (when GATE == 1)
		    rjmp	MLP_VCAOK

MLP_VCAENV:
            lds	    R16,ENV_INTEGR
		    ldi	    R17, 46
    		rcall	MUL8X8U
		    mov	    R16, R17		    ; R16 = 0..45
		    ldi	    R30, TAB_VCA	    ;\
		    ldi	    R31, 0			    ;/ Z = &Tab
		    rcall	TAB_BYTE		    ; R0 = 2..255
		    mov	    R16, R0

MLP_VCAOK:
            sts	LEVEL,R16
            ;-----------------------------------
            ;pseudo-random shift register:
            ;-----------------------------------
	        ;BIT = SHIFTREG.23 xor SHIFTREG.18
	        ;SHIFTREG = (SHIFTREG << 1) + BIT
		    lds	    R16, SHIFTREG_0
		    lds	    R17, SHIFTREG_1
		    lds	    R18, SHIFTREG_2
    		bst	    R18, 7			    ;\
		    bld	    R19, 0			    ;/ R19.0 = SHIFTREG.23
		    bst	    R18, 2			    ;\
		    bld	    R20, 0			    ;/ R20.0 = SHIFTREG.18
		    eor	    R19, R20			    ;R19.0 = BIT
		    lsr	    R19			        ; Cy = BIT
		    rol	    R16			        ;\
		    rol	    R17			        ; > R18:R17:R16 =
		    rol	    R18			        ;/  = (SHIFTREG << 1) + BIT
		    sts	    SHIFTREG_0, R16
		    sts	    SHIFTREG_1, R17
		    sts	    SHIFTREG_2, R18

            ;------------------------
            ;back to the main loop:
            ;------------------------
		    rjmp	MAINLOOP

;-------------------------------------------------------------------------------------------------------------------
            .EXIT
