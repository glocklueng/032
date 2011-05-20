;;;; avrsyn (2009.09.05)
;;;;
;;;; original from http://jarek.synth.net/
;;;; this version lives at http://krue.net/avrsyn/

;;; hardware
;;;
;;; pa7..pa0: a/d
;;; pb0-pb4:  control panel switches - rows
;;; pb5-pb7:  isp
;;; pc0-pc7:  data port for 2 x hc574
;;; pd0:      rxd (midi in)
;;; pd1:      txd (midi out)
;;; pd2:      low byte latch
;;; pd3:      high byte latch
;;; pd4-pd7:  control panel switches - columns
;;; 
;;; timer0:   sample rate timer: (ck/1) --> 62.5kHz
;;; timer1:   time counting: (CK/256) --> TCNT1 (16 uS)
;;; timer2:   not used

; configuration

.set cpu_frequency = 16000000
.set baud_rate = 31250

.set CONFNOTES = 8 ; number of midi notes to remember
.set CONFWAVE = 1 ; wavetable set

.set CONFCONTROLTICKS = $40
.set CONFBACKGROUNDTICKS = $10

.equ CONFFILTERIIR = 0
.equ CONFFILTERSVF = 1
.equ CONFFILTERMOOG = 2
.set CONFFILTER = CONFFILTERMOOG

;;; atmega32 defines

.equ iosize = $40
.equ ramsize = $800
.equ flashsize = $4000
.equ eepromsize = $400
.equ pagesize = $40

;;; memory boundaries

.set regsize = $20
.set regstart = $0
.set regend = $1f
.set iostart = $20
.set ioend = iostart + iosize - 1
.set ramstart = ioend + 1
.set ramend = ioend + ramsize
.set flashstart = $0
.set flashend = flashsize - 1
.set pagemask =  ~ ( pagesize - 1 )

;;; t bit boolean

.macro true
  set
.endmacro

.macro false
  clt
.endmacro

.macro branch_true
  brts @0
.endmacro

.macro branch_false
  brtc @0
.endmacro

;;; alignment

.macro align
  .org (pc | (exp2(@0) - 1)) + 1
.endmacro
 
;;; io registers

.equ TWBR = $00
.equ TWSR = $01
  .equ TWS7 = 7
  .equ TWS6 = 6
  .equ TWS5 = 5
  .equ TWS4 = 4
  .equ TWS3 = 3
  .equ TWPS1 = 1
  .equ TWPS0 = 0
.equ TWAR = $02
  .equ TWA6 = 7
  .equ TWA5 = 6
  .equ TWA4 = 5
  .equ TWA3 = 4
  .equ TWA2 = 3
  .equ TWA1 = 2
  .equ TWA0 = 1
  .equ TWGCE = 0
.equ TWDR = $03
.equ ADCL = $04
.equ ADCH = $05
.equ ADCSRA = $06
  .equ ADEN = 7
  .equ ADSC = 6
  .equ ADATE = 5
  .equ ADIF = 4
  .equ ADIE = 3
  .equ ADPS2 = 2
  .equ ADPS1 = 1
  .equ ADPS0 = 0
.equ ADMUX = $07
  .equ REFS1 = 7
  .equ REFS0 = 6
  .equ ADLAR = 5
  .equ MUX4 = 4
  .equ MUX3 = 3
  .equ MUX2 = 2
  .equ MUX1 = 1
  .equ MUX0 = 0
.equ ACSR = $08
  .equ ACD = 7
  .equ ACBG = 6
  .equ ACO = 5
  .equ ACI = 4
  .equ ACIE = 3
  .equ ACIC = 2
  .equ ACIS1 = 1
  .equ ACIS0 = 0
.equ UBRRL = $09
.equ UCSRB = $0a
  .equ RXCIE = 7
  .equ TXCIE = 6
  .equ UDRIE = 5
  .equ RXEN = 4
  .equ TXEN = 3
  .equ UCSZ2 = 2
  .equ RXB8 = 1
  .equ TXB8 = 0
.equ UCSRA = $0b
  .equ RXC = 7
  .equ TXC = 6
  .equ UDRE = 5
  .equ FE = 4
  .equ DOR = 3
  .equ PE = 2
  .equ U2X = 1
  .equ MPCM = 0
.equ UDR = $0c
.equ SPCR = $0d
  .equ SPIE = 7
  .equ SPE = 6
  .equ DORD = 5
  .equ MSTR = 4
  .equ CPOL = 3
  .equ CPHA = 2
  .equ SPR1 = 1
  .equ SPR0 = 0
.equ SPSR = $0e
  .equ SPIF = 7
  .equ WCOL = 6
  .equ SPI2X = 0
.equ SPDR = $0f
.equ PIND = $10
.equ DDRD = $11
.equ PORTD = $12
.equ PINC = $13
.equ DDRC = $14
.equ PORTC = $15
.equ PINB = $16
.equ DDRB = $17
.equ PORTB = $18
.equ PINA = $19
.equ DDRA = $1a
.equ PORTA = $1b
.equ EECR = $1c
  .equ EERIE = 3
  .equ EEMWE = 2
  .equ EEWE = 1
  .equ EERE = 0
.equ EEDR = $1d
.equ EEARL = $1e
.equ EEARH = $1f
.equ UBRRH = $20
.equ UCSRC = $20
  .equ URSEL = 7
  .equ UMSEL = 6
  .equ UPM1 = 5
  .equ UPM0 = 4
  .equ USBS = 3
  .equ UCSZ1 = 2
  .equ UCSZ0 = 1
  .equ UCPOL = 0
.equ WDTCR = $21
  .equ WDTOE = 4
  .equ WDE = 3
  .equ WDP2 = 2
  .equ WDP1 = 1
  .equ WDP0 = 0
.equ ASSR = $22
  .equ AS2 = 3
  .equ TCN2UB = 2
  .equ OCR2UB = 1
  .equ TCR2UB = 0
.equ OCR2 = $23
.equ TCNT2 = $24
.equ TCCR2 = $25
  .equ FOC2 = 7
  .equ WGM20 = 6
  .equ COM21 = 5
  .equ COM20 = 4
  .equ WGM21 = 3
  .equ CS22 = 2
  .equ CS21 = 1
  .equ CS20 = 0
.equ ICR1L = $26
.equ ICR1H = $27
.equ OCR1BL = $28
.equ OCR1BH = $29
.equ OCR1AL = $2a
.equ OCR1AH = $2b
.equ TCNT1L = $2c
.equ TCNT1H = $2d
.equ TCCR1B = $2e
  .equ ICNC1 = 7
  .equ ICES1 = 6
  .equ WGM13 = 4
  .equ WGM12 = 3
  .equ CS12 = 2
  .equ CS11 = 1
  .equ CS10 = 0
.equ TCCR1A = $2f
  .equ COM1A1 = 7
  .equ COM1A0 = 6
  .equ COM1B1 = 5
  .equ COM1B0 = 4
  .equ FOC1A = 3
  .equ FOC1B = 2
  .equ WGM11 = 1
  .equ WGM10 = 0
.equ SFIOR = $30
  .equ ADTS2 = 7
  .equ ADTS1 = 6
  .equ ADTS0 = 5
  .equ ACME = 3
  .equ PUD = 2
  .equ PSR2 = 1
  .equ PSR10 = 0
.equ OSCCA = $31
.equ OCDR = $31
.equ TCNT0 = $32
.equ TCCR0 = $33
  .equ FOC0 = 7
  .equ WGM00 = 6
  .equ COM01 = 5
  .equ COM00 = 4
  .equ WGM01 = 3
  .equ CS02 = 2
  .equ CS01 = 1
  .equ CS00 = 0
.equ MCUCSR = $34
  .equ JTD = 7
  .equ ISC2 = 6
  .equ JTRF = 4
  .equ WDRF = 3
  .equ BORF = 2
  .equ EXTRF = 1
  .equ PORF = 0
.equ MCUCR = $35
  .equ SE = 7
  .equ SM2 = 6
  .equ SM1 = 5
  .equ SM0 = 4
  .equ ISC11 = 3
  .equ ISC10 = 2
  .equ ISC01 = 1
  .equ ISC00 = 0
.equ TWCR = $36
  .equ TWINT = 7
  .equ TWEA = 6
  .equ TWSTA = 5
  .equ TWSTO = 4
  .equ TWWC = 3
  .equ TWEN = 2
  .equ TWIE = 0
.equ SPMCR = $37
  .equ SPMIE = 7
  .equ RWWSB = 6
  .equ RWWSRE = 4
  .equ BLBSET = 3
  .equ PGWRT = 2
  .equ PGERS = 1
  .equ SPMEN = 0
.equ TIFR = $38
  .equ OCF2 = 7
  .equ TOV2 = 6
  .equ ICF1 = 5
  .equ OCF1A = 4
  .equ OCF1B = 3
  .equ TOV1 = 2
  .equ OCF0 = 1
  .equ TOV0 = 0
.equ TIMSK = $39
  .equ OCIE2 = 7
  .equ TOIE2 = 6
  .equ TICIE1 = 5
  .equ OCIE1A = 4
  .equ OCIE1B = 3
  .equ TOIE1 = 2
  .equ OCIE0 = 1
  .equ TOIE0 = 0
.equ GIFR = $3a
  .equ INTF1 = 7
  .equ INTF0 = 6
  .equ INTF2 = 5
.equ GIRC = $3b
  .equ INT1 = 7
  .equ INT0 = 6
  .equ INT2 = 5
  .equ IVSEL = 1
  .equ IVCE = 0
.equ OCR0 = $3c
.equ SPL = $3d
.equ SPH = $3e
.equ SREG = $3f

;;; registers

; current phase of DCO A
.def PHASEA_0 = r2
.def PHASEA_1 = r3
.def PHASEA_2 = r4

; current phase of DCO B
.def PHASEB_0 = r5
.def PHASEB_1 = r6
.def PHASEB_2 = r7

.def zero = r8
  
; increase phase for DCO A
;.def DELTAA_0 = r8
;.def DELTAA_1 = r9
;.def DELTAA_2 = r10

; increase phase for DCO B
;.def DELTAB_0 = r11
;.def DELTAB_1 = r12
;.def DELTAB_2 = r13

; DCF
.def XN1_L = r12
.def XN1_H = r13
.def YN1_L = r14
.def YN1_H = r15

.def svf_low = r12
.def svf_high = r13
.def svf_band = r14
.def svf_notch = r15

.def moog_wa = r9
.def moog_wb = r10
.def moog_wc = r11
.def moog_ya = r12
.def moog_yb = r13
.def moog_yc = r14
.def moog_yd = r15

;;; ram
.dseg

; midi
MIDIPHASE:	.BYTE	1
MIDICHANNEL:	.BYTE	1
MIDIDATA0:	.BYTE	1
MIDIVELOCITY:	.BYTE	1
MIDINOTE:	.BYTE	1
MIDINOTEPREV:	.BYTE	1		; buffer for MIDI note
MIDIPBEND_L:	.BYTE	1		;\
MIDIPBEND_H:	.BYTE	1		;/ -32768..+32766
MIDIPRESSURE:   .byte 1   ; channel pressure
MIDICC:         .byte $80 ; continuous controllers
  .equ MIDIMODWHEEL = MIDICC + $01
;  .equ MIDIEXPRESSION = MIDICC + $0b
  .equ MIDIEXPRESSION = MIDICC + $40

ENV0: .byte 4
ENV1: .byte 4

LFO0: .byte $b
LFOLEVEL: .byte 1
LFO1: .byte $b

  ;current sound parameters:
;;;LFOFREQ:	.BYTE	1	    ;0..255
;;;LFOLEVEL:	.BYTE	1	    ;0..255

SETMIDICHANNEL:	.BYTE	1		;selected MIDI channel: 0 for OMNI or 1..15
;;;DETUNEB_FRAC:	.BYTE	1	    ;\
;;;DETUNEB_INTG:	.BYTE	1	    ;/ -128,000..+127,996
  ;other variables:
SWITCH1: .byte 1
SWITCH2: .byte 1
SWITCH3: .byte 1

NOTE_L:		.BYTE	1
NOTE_H:		.BYTE	1
NOTE_INTG:	.BYTE	1
PORTACNT:	.BYTE	1		;2 / 1 / 0
LPF_I:		.BYTE	1
LEVEL:		.BYTE	1		;0..255
PITCH:		.BYTE	1		;0..96

; potentiometers
ADCDATA: .byte 8

GATE:		.BYTE	1		;0 / 1

OLDWAVEA:	.BYTE	1
OLDWAVEB:	.BYTE	1
WAVEB: .byte 1
SHIFTREG_0:	.BYTE	1		;\
SHIFTREG_1:	.BYTE	1		; > shift register for
SHIFTREG_2:	.BYTE	1		;/  pseudo-random generator

; increase phase for DCO A
DELTAA_0: .byte 1
DELTAA_1: .byte 1
DELTAA_2: .byte 1

; increase phase for DCO B
DELTAB_0: .byte 1
DELTAB_1: .byte 1
DELTAB_2: .byte 1

; timer ticks
ticks: .byte 1
control_ticks: .byte 1

; key scan state
KEYSTATE: .byte 1

; note array
NOTES: .byte CONFNOTES
NOTESACTIVE: .byte 1

; arpeggiator
ARPPHASE: .byte 3
ARPNOTE: .byte 1
ARPSTATE: .byte 1

;; input configuration

; boolean inputs
.equ DCO_DISTORTION_REG = SWITCH1
.equ DCO_DISTORTION_BIT = 0

.equ DCA_ENV_ENABLE_REG = SWITCH1
.equ DCA_ENV_ENABLE_BIT = 1

.equ TRANSPOSE_REG = SWITCH1
.equ TRANSPOSE_BIT = 2

.equ DCOB_TRANSPOSE_REG = SWITCH1
.equ DCOB_TRANSPOSE_BIT = 3

.equ DCOA_TRANSPOSE_REG = SWITCH1
.equ DCOA_TRANSPOSE_BIT = 4

.equ LFO_MODE_REG = SWITCH1
.equ LFO_MODE_BIT = 5

.equ LFO_WAVE_REG = SWITCH1
.equ LFO_WAVE_BIT = 6

.equ LFO_RANDOM_REG = SWITCH1
.equ LFO_RANDOM_BIT = 7

.equ LFO_SYNC_REG = SWITCH2
.equ LFO_SYNC_BIT = 0

.equ DCF_TRACK_REG = SWITCH2
.equ DCF_TRACK_BIT = 1

.equ DCOA_NOISE_REG = SWITCH2
.equ DCOA_NOISE_BIT = 2

.equ MOD_WHEEL_REG = SWITCH2
.equ MOD_WHEEL_BIT = 3

.equ ARP_ENABLE_REG = SWITCH2
.equ ARP_ENABLE_BIT = 4

.equ ARP_SYNC_REG = SWITCH2
.equ ARP_SYNC_BIT = 5

; continuous inputs
.equ LFOFREQ = ADCDATA + $0
.equ LFOLEVELPANEL = ADCDATA + $1
.equ DETUNEB = ADCDATA + $2
.equ CUTOFF = ADCDATA + $3
.equ ARPTHRESHOLD = ADCDATA + $4
.equ ARPRATE = ADCDATA + $5
.equ PORTAMENTO = ADCDATA + $6
.equ VCFENVMOD = ADCDATA + $7

.equ ENV0PARAM = MIDICC + $30
.equ ENV1PARAM = MIDICC + $34
.equ DCOALEVEL = MIDICC + $38
.equ DCOBLEVEL = MIDICC + $39
.equ DCOAWAVE = MIDICC + $3a
.equ DCOBWAVE = MIDICC + $3b
.equ RESONANCE = MIDICC + $3e
.equ FMDEPTH = MIDICC + $3f

  .eseg

;;; vectors

  .cseg
;  .org	$0000

		    jmp	reset		            ; RESET

;; IRQ 0 -7
		    jmp	IRQ_NONE	            ; INT0
		    jmp	IRQ_NONE	            ; INT1
		    jmp	IRQ_NONE	            ; INT2
		    jmp	IRQ_NONE	            ; INT3

		    jmp	IRQ_NONE	            ; INT4
		    jmp	IRQ_NONE	            ; INT5
		    jmp	IRQ_NONE	            ; INT6
		    jmp	IRQ_NONE	            ; INT7


		    jmp	IRQ_NONE				; TIMER2 COMPARE
		    jmp	IRQ_NONE	            ; TIMER2 OVERFLOW

			
		    jmp	IRQ_NONE	            ; TIMER1 CAPTURE
		    jmp	IRQ_NONE	            ; TIMER1 COMPA
		    jmp	IRQ_NONE	            ; TIMER1 COMPB
    		jmp	IRQ_NONE	            ; TIMER1 OVF

		    jmp	IRQ_NONE	            ; TIMER0 COMPA
		    jmp	timer0_overflow	        ; TIMER0 OVF

		    jmp	IRQ_NONE	            ; SPI,STC Tranfer complete

		    jmp	usart_rxc	            ; UART, RX COMPLETE
		    jmp	IRQ_NONE	            ; UART,UDRE
		    jmp	IRQ_NONE	            ; UART, TX COMPLETE

		    jmp	adc_complete	        ; ADC CONVERSION COMPLETE

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

IRQ_NONE:
  reti

;;; rom tables

phase_table:
.db $ac,$92,$08,$00
.db $2d,$15,$09,$00
.db $70,$9f,$09,$00
.db $ec,$31,$0a,$00
.db $1d,$cd,$0a,$00
.db $89,$71,$0b,$00
.db $bc,$1f,$0c,$00
.db $4b,$d8,$0c,$00
.db $d4,$9b,$0d,$00
.db $fc,$6a,$0e,$00
.db $77,$46,$0f,$00
.db $fe,$2e,$10,$00
.db $59,$25,$11,$00
.db $5a,$2a,$12,$00
.db $e0,$3e,$13,$00
.db $d8,$63,$14,$00
.db $3b,$9a,$15,$00
.db $13,$e3,$16,$00
.db $79,$3f,$18,$00
.db $97,$b0,$19,$00
.db $a8,$37,$1b,$00
.db $f9,$d5,$1c,$00
.db $ee,$8c,$1e,$00
.db $fc,$5d,$20,$00
.db $b2,$4a,$22,$00
.db $b4,$54,$24,$00
.db $c1,$7d,$26,$00
.db $b0,$c7,$28,$00
.db $77,$34,$2b,$00
.db $27,$c6,$2d,$00
.db $f3,$7e,$30,$00
.db $2f,$61,$33,$00
.db $50,$6f,$36,$00
.db $f3,$ab,$39,$00
.db $dc,$19,$3d,$00
.db $f9,$bb,$40,$00
.db $64,$95,$44,$00
.db $69,$a9,$48,$00
.db $82,$fb,$4c,$00
.db $60,$8f,$51,$00
.db $ee,$68,$56,$00
.db $4f,$8c,$5b,$00
.db $e7,$fd,$60,$00
.db $5e,$c2,$66,$00
.db $a0,$de,$6c,$00
.db $e6,$57,$73,$00
.db $b8,$33,$7a,$00
.db $f2,$77,$81,$00
.db $c9,$2a,$89,$00
.db $d2,$52,$91,$00
.db $04,$f7,$99,$00
.db $c1,$1e,$a3,$00
.db $dc,$d1,$ac,$00
.db $9e,$18,$b7,$00
.db $cf,$fb,$c1,$00
.db $bc,$84,$cd,$00
.db $40,$bd,$d9,$00
.db $cc,$af,$e6,$00
.db $70,$67,$f4,$00
.db $e5,$ef,$02,$01
.db $93,$55,$12,$01
.db $a5,$a5,$22,$01
.db $09,$ee,$33,$01
.db $83,$3d,$46,$01
.db $b9,$a3,$59,$01
.db $3d,$31,$6e,$01
.db $9f,$f7,$83,$01
.db $79,$09,$9b,$01
.db $81,$7a,$b3,$01
.db $99,$5f,$cd,$01
.db $e1,$ce,$e8,$01
.db $ca,$df,$05,$02
.db $27,$ab,$24,$02
.db $4a,$4b,$45,$02
.db $12,$dc,$67,$02
.db $07,$7b,$8c,$02
.db $72,$47,$b3,$02
.db $7a,$62,$dc,$02
.db $3e,$ef,$07,$03
.db $f2,$12,$36,$03
.db $03,$f5,$66,$03
.db $33,$bf,$9a,$03
.db $c3,$9d,$d1,$03
.db $94,$bf,$0b,$04
.db $4f,$56,$49,$04
.db $95,$96,$8a,$04
.db $24,$b8,$cf,$04
.db $0e,$f6,$18,$05
.db $e4,$8e,$66,$05
.db $f5,$c4,$b8,$05
.db $7c,$de,$0f,$06
.db $e4,$25,$6c,$06
.db $06,$ea,$cd,$06
.db $67,$7e,$35,$07
.db $87,$3b,$a3,$07
.db $28,$7f,$17,$08
.db $9f,$ac,$92,$08
.db $2a,$2d,$15,$09
.db $49,$70,$9f,$09
.db $1c,$ec,$31,$0a
.db $c9,$1d,$cd,$0a
.db $ea,$89,$71,$0b
.db $f8,$bc,$1f,$0c
.db $c9,$4b,$d8,$0c
.db $0c,$d4,$9b,$0d
.db $ce,$fc,$6a,$0e
.db $0e,$77,$46,$0f
.db $50,$fe,$2e,$10
.db $3f,$59,$25,$11
.db $55,$5a,$2a,$12
.db $92,$e0,$3e,$13
.db $38,$d8,$63,$14
.db $93,$3b,$9a,$15
.db $d5,$13,$e3,$16
.db $f1,$79,$3f,$18
.db $93,$97,$b0,$19
.db $18,$a8,$37,$1b
.db $9c,$f9,$d5,$1c
.db $1c,$ee,$8c,$1e
.db $a0,$fc,$5d,$20
.db $7e,$b2,$4a,$22
.db $ab,$b4,$54,$24
.db $25,$c1,$7d,$26
.db $70,$b0,$c7,$28
.db $27,$77,$34,$2b
.db $aa,$27,$c6,$2d
.db $e3,$f3,$7e,$30
.db $27,$2f,$61,$33

;-----------------------------------------------------------------------------
TAB_VCF:	.DW	0x0101
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
TAB_VCA:	.DW	0x0100		    ;-48.2dB,-oodB
		.DW	0x0302		    ;-38.6dB,-42.1dB
		.DW	0x0504		    ;-34.2dB,-36.1dB
		.DW	0x0706		    ;-31.3dB,-32.6dB
		.DW	0x0908		    ;-29.1dB,-30.1dB
    		.DW	0x0C0A		    ;-26.6dB,-28.2dB
		.DW	0x110E		    ;-23.6dB,-25.2dB
		.DW	0x1814		    ;-20.6dB,-22.1dB
		.DW	0x201C		    ;-18.1dB,-19.2dB
		.DW	0x2824		    ;-16.1dB,-17.0dB
		.DW	0x302C		    ;-14.5dB,-15.3dB
    		.DW	0x3834		    ;-13.2dB,-13.8dB
		.DW	0x403C		    ;-12.0dB,-12.6dB
    		.DW	0x4A46		    ;-10.8dB,-11.3dB
		.DW	0x5450		    ; -9.7dB,-10.1dB
		.DW	0x615A		    ; -8.4dB, -9.1dB
		.DW	0x7068		    ; -7.2dB, -7.8dB
		.DW	0x8078		    ; -6.0dB, -6.6dB
		.DW	0x9489		    ; -4.8dB, -5.4dB
    		.DW	0xA8A0		    ; -3.7dB, -4.1dB
		.DW	0xC2B8		    ; -2.4dB, -2.9dB
		.DW	0xE0D2		    ; -1.2dB, -1.7dB
		.DW	0xFFF0		    ; -0.0dB, -0.6dB

; y = 0x10 * 27 * pow(1.286,(0xff-x)/8.0)
rate_table:
.db $59,$00,$14,$00
.db $dc,$61,$13,$00
.db $46,$c8,$12,$00
.db $72,$33,$12,$00
.db $38,$a3,$11,$00
.db $76,$17,$11,$00
.db $07,$90,$10,$00
.db $c9,$0c,$10,$00
.db $9c,$8d,$0f,$00
.db $5e,$12,$0f,$00
.db $f0,$9a,$0e,$00
.db $35,$27,$0e,$00
.db $0f,$b7,$0d,$00
.db $61,$4a,$0d,$00
.db $11,$e1,$0c,$00
.db $03,$7b,$0c,$00
.db $1e,$18,$0c,$00
.db $49,$b8,$0b,$00
.db $6b,$5b,$0b,$00
.db $6c,$01,$0b,$00
.db $37,$aa,$0a,$00
.db $b5,$55,$0a,$00
.db $d1,$03,$0a,$00
.db $75,$b4,$09,$00
.db $8e,$67,$09,$00
.db $09,$1d,$09,$00
.db $d2,$d4,$08,$00
.db $d7,$8e,$08,$00
.db $07,$4b,$08,$00
.db $51,$09,$08,$00
.db $a2,$c9,$07,$00
.db $ed,$8b,$07,$00
.db $21,$50,$07,$00
.db $2e,$16,$07,$00
.db $06,$de,$06,$00
.db $9c,$a7,$06,$00
.db $e1,$72,$06,$00
.db $c7,$3f,$06,$00
.db $43,$0e,$06,$00
.db $46,$de,$05,$00
.db $c6,$af,$05,$00
.db $b7,$82,$05,$00
.db $0c,$57,$05,$00
.db $bc,$2c,$05,$00
.db $bb,$03,$05,$00
.db $ff,$db,$04,$00
.db $7d,$b5,$04,$00
.db $2d,$90,$04,$00
.db $04,$6c,$04,$00
.db $fa,$48,$04,$00
.db $06,$27,$04,$00
.db $1f,$06,$04,$00
.db $3c,$e6,$03,$00
.db $56,$c7,$03,$00
.db $65,$a9,$03,$00
.db $61,$8c,$03,$00
.db $43,$70,$03,$00
.db $04,$55,$03,$00
.db $9d,$3a,$03,$00
.db $07,$21,$03,$00
.db $3b,$08,$03,$00
.db $34,$f0,$02,$00
.db $ec,$d8,$02,$00
.db $5c,$c2,$02,$00
.db $7f,$ac,$02,$00
.db $4f,$97,$02,$00
.db $c7,$82,$02,$00
.db $e2,$6e,$02,$00
.db $9a,$5b,$02,$00
.db $eb,$48,$02,$00
.db $d0,$36,$02,$00
.db $45,$25,$02,$00
.db $44,$14,$02,$00
.db $cb,$03,$02,$00
.db $d4,$f3,$01,$00
.db $5b,$e4,$01,$00
.db $5d,$d5,$01,$00
.db $d6,$c6,$01,$00
.db $c2,$b8,$01,$00
.db $1d,$ab,$01,$00
.db $e5,$9d,$01,$00
.db $15,$91,$01,$00
.db $ab,$84,$01,$00
.db $a3,$78,$01,$00
.db $fa,$6c,$01,$00
.db $ae,$61,$01,$00
.db $bc,$56,$01,$00
.db $20,$4c,$01,$00
.db $d8,$41,$01,$00
.db $e2,$37,$01,$00
.db $3b,$2e,$01,$00
.db $e0,$24,$01,$00
.db $cf,$1b,$01,$00
.db $06,$13,$01,$00
.db $83,$0a,$01,$00
.db $43,$02,$01,$00
.db $45,$fa,$00,$00
.db $85,$f2,$00,$00
.db $04,$eb,$00,$00
.db $bd,$e3,$00,$00
.db $b1,$dc,$00,$00
.db $dc,$d5,$00,$00
.db $3d,$cf,$00,$00
.db $d3,$c8,$00,$00
.db $9c,$c2,$00,$00
.db $96,$bc,$00,$00
.db $bf,$b6,$00,$00
.db $17,$b1,$00,$00
.db $9c,$ab,$00,$00
.db $4c,$a6,$00,$00
.db $26,$a1,$00,$00
.db $2a,$9c,$00,$00
.db $54,$97,$00,$00
.db $a5,$92,$00,$00
.db $1b,$8e,$00,$00
.db $b5,$89,$00,$00
.db $72,$85,$00,$00
.db $50,$81,$00,$00
.db $50,$7d,$00,$00
.db $6f,$79,$00,$00
.db $ac,$75,$00,$00
.db $08,$72,$00,$00
.db $80,$6e,$00,$00
.db $15,$6b,$00,$00
.db $c4,$67,$00,$00
.db $8e,$64,$00,$00
.db $71,$61,$00,$00
.db $6d,$5e,$00,$00
.db $81,$5b,$00,$00
.db $ac,$58,$00,$00
.db $ed,$55,$00,$00
.db $44,$53,$00,$00
.db $b0,$50,$00,$00
.db $31,$4e,$00,$00
.db $c5,$4b,$00,$00
.db $6d,$49,$00,$00
.db $27,$47,$00,$00
.db $f3,$44,$00,$00
.db $d1,$42,$00,$00
.db $bf,$40,$00,$00
.db $be,$3e,$00,$00
.db $cd,$3c,$00,$00
.db $eb,$3a,$00,$00
.db $18,$39,$00,$00
.db $54,$37,$00,$00
.db $9e,$35,$00,$00
.db $f5,$33,$00,$00
.db $59,$32,$00,$00
.db $ca,$30,$00,$00
.db $47,$2f,$00,$00
.db $d1,$2d,$00,$00
.db $66,$2c,$00,$00
.db $06,$2b,$00,$00
.db $b1,$29,$00,$00
.db $67,$28,$00,$00
.db $26,$27,$00,$00
.db $f0,$25,$00,$00
.db $c4,$24,$00,$00
.db $a0,$23,$00,$00
.db $86,$22,$00,$00
.db $74,$21,$00,$00
.db $6b,$20,$00,$00
.db $6a,$1f,$00,$00
.db $71,$1e,$00,$00
.db $80,$1d,$00,$00
.db $96,$1c,$00,$00
.db $b4,$1b,$00,$00
.db $d8,$1a,$00,$00
.db $04,$1a,$00,$00
.db $35,$19,$00,$00
.db $6e,$18,$00,$00
.db $ac,$17,$00,$00
.db $f0,$16,$00,$00
.db $3b,$16,$00,$00
.db $8b,$15,$00,$00
.db $e0,$14,$00,$00
.db $3a,$14,$00,$00
.db $9a,$13,$00,$00
.db $ff,$12,$00,$00
.db $68,$12,$00,$00
.db $d6,$11,$00,$00
.db $49,$11,$00,$00
.db $c0,$10,$00,$00
.db $3b,$10,$00,$00
.db $bb,$0f,$00,$00
.db $3e,$0f,$00,$00
.db $c5,$0e,$00,$00
.db $50,$0e,$00,$00
.db $df,$0d,$00,$00
.db $71,$0d,$00,$00
.db $06,$0d,$00,$00
.db $9f,$0c,$00,$00
.db $3b,$0c,$00,$00
.db $da,$0b,$00,$00
.db $7c,$0b,$00,$00
.db $21,$0b,$00,$00
.db $c9,$0a,$00,$00
.db $73,$0a,$00,$00
.db $21,$0a,$00,$00
.db $d0,$09,$00,$00
.db $83,$09,$00,$00
.db $37,$09,$00,$00
.db $ee,$08,$00,$00
.db $a7,$08,$00,$00
.db $63,$08,$00,$00
.db $20,$08,$00,$00
.db $e0,$07,$00,$00
.db $a2,$07,$00,$00
.db $65,$07,$00,$00
.db $2a,$07,$00,$00
.db $f2,$06,$00,$00
.db $bb,$06,$00,$00
.db $85,$06,$00,$00
.db $52,$06,$00,$00
.db $1f,$06,$00,$00
.db $ef,$05,$00,$00
.db $c0,$05,$00,$00
.db $92,$05,$00,$00
.db $66,$05,$00,$00
.db $3b,$05,$00,$00
.db $12,$05,$00,$00
.db $ea,$04,$00,$00
.db $c3,$04,$00,$00
.db $9d,$04,$00,$00
.db $78,$04,$00,$00
.db $55,$04,$00,$00
.db $33,$04,$00,$00
.db $11,$04,$00,$00
.db $f1,$03,$00,$00
.db $d2,$03,$00,$00
.db $b4,$03,$00,$00
.db $96,$03,$00,$00
.db $7a,$03,$00,$00
.db $5e,$03,$00,$00
.db $44,$03,$00,$00
.db $2a,$03,$00,$00
.db $11,$03,$00,$00
.db $f8,$02,$00,$00
.db $e1,$02,$00,$00
.db $ca,$02,$00,$00
.db $b4,$02,$00,$00
.db $9e,$02,$00,$00
.db $8a,$02,$00,$00
.db $76,$02,$00,$00
.db $62,$02,$00,$00
.db $4f,$02,$00,$00
.db $3d,$02,$00,$00
.db $2b,$02,$00,$00
.db $1a,$02,$00,$00
.db $09,$02,$00,$00
.db $f9,$01,$00,$00
.db $e9,$01,$00,$00
.db $da,$01,$00,$00
.db $cc,$01,$00,$00
.db $bd,$01,$00,$00
.db $b0,$01,$00,$00


; time to rate conversion
;   r16         -> time
;   r18:r17:r16 <- rate
;   r31,r30     xx
rate:
  push r31
  push r30
  ldi r30,low(rate_table<<1)
  ldi r31,high(rate_table<<1)
  clr r17
  lsl r16
  rol r17
  lsl r16
  rol r17
  add r30,r16
  adc r31,r17
  lpm r16,z+
  lpm r17,z+
  lpm r18,z+
  pop r30
  pop r31
  ret

;; lfo

; r29:r28 -> lfo data structure
; r16     -> frequency
; r25     -> flags

.equ LFOSTATE = $0
  .equ LFOSTATERISE = 0
  .equ LFOSTATEFALL = 1
.equ LFOPHASE0 = $1
.equ LFOPHASE1 = $2
.equ LFOPHASE2 = $3
.equ LFOTOP0 = $4
.equ LFOTOP1 = $5
.equ LFOTOP2 = $6
.equ LFOBOTTOM0 = $7
.equ LFOBOTTOM1 = $8
.equ LFOBOTTOM2 = $9
.equ LFOVALUE = $a

.equ LFOFLAGSQUARE = 1
.equ LFOFLAGRANDOM = 0

lfo:
  ; get phase increment
  com r16
  rcall rate

  ; get current phase
  ldd r19,y+LFOPHASE0
  ldd r20,y+LFOPHASE1
  ldd r21,y+LFOPHASE2
  subi r21,$80

  ; flag: did lfo hit a limit?
  ldi r31,0

  ldd r30,y+LFOSTATE
  tst r30
  brne lfo_fall

lfo_rise:
  ; get top limit
  ldd r22,y+LFOTOP0
  ldd r23,y+LFOTOP1
  ldd r24,y+LFOTOP2

  ; add increment
  add r19,r16
  adc r20,r17
  adc r21,r18

  ; if overflow, we must have passed the
  ; top limit as well
  brcs lfo_top

  ; check if we passed the top limit
  cp r19,r22
  cpc r20,r23
  cpc r21,r24
  brlo lfo_done

lfo_top:
  ; set lfo value to top limit
  mov r19,r22
  mov r20,r23
  mov r21,r24

  ; change to falling state
  ldi r30,LFOSTATEFALL

  ; set "limit hit" flag
  ldi r31,1
  rjmp lfo_done

lfo_fall:
  ; get bottom limit
  ldd r22,y+LFOBOTTOM0
  ldd r23,y+LFOBOTTOM1
  ldd r24,y+LFOBOTTOM2

  ; subtract increment
  sub R19,R16
  sbc R20,R17
  sbc R21,R18
  
  ; if overflow, we must have passed the
  ; bottom limit as well
  brcs lfo_bottom
  
  ; check if we passed the bottom limit
  cp r22,r19
  cpc r23,r20
  cpc r24,r21
  brlo lfo_done

lfo_bottom:
  ; set lfo value to bottom limit
  mov r19,r22
  mov r20,r23
  mov r21,r24

  ; change to rising state
  ldi r30,LFOSTATERISE

  ; set "limit hit" flag
  ldi r31,1

lfo_done:
  ; store lfo state
  std y+LFOSTATE,r30

  ; store lfo value
  subi r21,$80
  std y+LFOPHASE0,r19
  std y+LFOPHASE1,r20
  std y+LFOPHASE2,r21

lfo_limits:
  ; only update limits if "limit hit" flag is set
  tst r31
  breq lfo_value
  
  ; calculate top and bottom
  bst r25,LFOFLAGRANDOM
  brts lfo_wave_random

lfo_wave_normal:
  ldi r16,0
  ldi r17,0
  ldi r18,0
  rjmp lfo_store_limits

lfo_wave_random:
  lds r16,SHIFTREG_0
  lds r17,SHIFTREG_1
  lds r18,SHIFTREG_2
  ; make sure bottom is lower than $800000
  andi r18,$7f

lfo_store_limits:
  ; store bottom
  std y+LFOBOTTOM0,r16
  std y+LFOBOTTOM1,r17
  std y+LFOBOTTOM2,r18

  ; top := $ffffff - bottom
  com r16
  com r17
  com r18

  ; store bottom
  std y+LFOTOP0,r16
  std y+LFOTOP1,r17
  std y+LFOTOP2,r18

lfo_value:
  ; test for square
  bst r25,LFOFLAGSQUARE
  brtc lfo_store_value

lfo_square:
  lsl r21
  ldi r21,$7f
  adc r21,zero

lfo_store_value:
  std y+LFOVALUE,r21
  ret

;; mod wheel
modwheel:
  ; determine lfo level
  lds r16,LFOLEVELPANEL
  lds r17,MIDIMODWHEEL

  ; check if mod wheel enabled
  lds r30,MOD_WHEEL_REG
  bst r30,MOD_WHEEL_BIT
  brtc modwheel_store

lfo_mod_wheel:
  ; get greater of LFOLEVELPANEL and MIDIMODWHEEL
  cp r16,r17
  brsh modwheel_store
  mov r16,r17

modwheel_store:
  sts LFOLEVEL,r16
  ret

;; adsr envelope

; r31:r30  -> envelope state
; r29:r28  -> envelope parameters
; r21..r16 xx

.equ ENVSTATE = 0
  .equ ENVSTATESTOP = 0
  .equ ENVSTATEATTACK = 1
  .equ ENVSTATEDECAY = 2
  .equ ENVSTATESUSTAIN = 3
  .equ ENVSTATERELEASE = 4
.equ ENVL = 1
.equ ENVM = 2
.equ ENVH = 3

.equ ENVATTACK = 0
.equ ENVDECAY = 1
.equ ENVSUSTAIN = 2
.equ ENVRELEASE = 3

env:
  ldd r16,z+ENVSTATE
  ldd r19,z+ENVL
  ldd r20,z+ENVM
  ldd r21,z+ENVH

  cpi r16,ENVSTATEATTACK
  breq env_attack
  cpi r16,ENVSTATEDECAY
  breq env_decay
  cpi r16,ENVSTATERELEASE
  breq env_release
  
  rjmp env_done
  
env_attack:
  ldd r16,y+ENVATTACK
  rcall rate
  
  add r19,r16
  adc r20,r17
  adc r21,r18
  brcc env_store

  ldi r19,$ff
  ldi r20,$ff
  ldi r21,$ff

  ldi r16,ENVSTATEDECAY
  rjmp env_store_state

env_decay:
  ldd r16,y+ENVDECAY
  rcall rate

  sub r19,r16
  sbc r20,r17
  sbc r21,r18
  
  ldd r16,y+ENVSUSTAIN
  brcs env_decay_limit

  cp r21,r16
  brsh env_store

env_decay_limit:
  mov r21,r16
  clr r20
  clr r19

  ldi r16,ENVSTATESUSTAIN
  rjmp env_store_state

env_release:
  ldd r16,y+ENVRELEASE
  rcall rate

  sub r19,r16
  sbc r20,r17
  sbc r21,r18
  brcc env_store

  clr r19
  clr r20
  clr r21

  ldi r16,ENVSTATESTOP

env_store_state:
  std z+ENVSTATE,r16

env_store:
  std z+ENVL,r19
  std z+ENVM,r20
  std z+ENVH,r21

env_done:
  ret


;; dco modulation
dcomod:

  ; start with midi note in r23.r22:r25
  ldi r25,0
  ldi r22,0
  lds r23,MIDINOTE

  ; note value of $ff means no note
  cpi r23,$ff
  brne dcomod_limit_max
;  rjmp MLP_VCOX
  ret

  ; limit maximum note number to 96
dcomod_limit_max_loop:
  subi r23,12

dcomod_limit_max:
  cpi r23,97
  brsh dcomod_limit_max_loop
  rjmp dcomod_limit_min

  ; limit minimum note number to 36
dcomod_limit_min_loop:
  subi r23,-12

dcomod_limit_min:
  cpi r23,36
  brlo dcomod_limit_min_loop

  ; transpose 1 octave down (24..84)
  subi r23,12

;; portamento

  ; load current pitch into r27.r26:r25
  lds r25,NOTE_L
  lds r26,NOTE_H
  lds r27,NOTE_INTG

  ; skip portamento for first note
  lds r16,PORTACNT
  tst r16
  brne portamento_write

  ; get pitch delta
  lds r16,PORTAMENTO
  rcall rate

  ; move to r21.r20:r19
  mov r19,r16
  mov r20,r17
  mov r21,r18

  ; is target pitch higher or lower?
  cp r22,r26
  cpc r23,r27
  brsh portamento_add

portamento_sub:
  ; subtract delta
  sub r25,r19
  sbc r26,r20
  sbc r27,r21

  ; check if target pitch was reached
  cp r22,r26
  cpc r23,r27
  brlo portamento_done
  
  rjmp portamento_limit

portamento_add:
  ; add delta
  add r25,r19
  adc r26,r20
  adc r27,r21

  ; check if target pitch was reached
  cp r22,r26
  cpc r23,r27
  brsh portamento_done

portamento_limit:
  ; copy limit to current
  ldi r25,0
  mov r26,r22
  mov r27,r23

portamento_done:
  ; make 'portamentoed' pitch the current pitch
  mov r22,r26
  mov r23,r27

portamento_write:
  ; save portamento pitch for next time
  sts NOTE_L,r25
  sts NOTE_H,r22
  sts NOTE_INTG,r23

;; transpose switch
  lds r16,TRANSPOSE_REG
  sbrs r16,TRANSPOSE_BIT
  subi r23,12

;; pitch bend

  ; get controller value
  lds r16,MIDIPBEND_L
  lds r17,MIDIPBEND_H

  ; scale by 3/32
  ldi r18,5       ;\ R17,R16 = P.BEND/32
  rcall ASR16     ;/    -4,000..+3,999
  mov r18,r16     ;\ R19,R18 = P.BEND/32
  mov r19,r17     ;/    -4,000..+3,999
  add r16,r18     ;\ R17,R16 = 2/32*P.BEND
  adc r17,r19     ;/    -8,000..+7,999
  add r16,r18     ;\ R17,R16 = 3/32*P.BEND
  adc r17,r19     ;/    -12,000..+11,999

  ; add to pitch
  add r22,r16
  adc r23,r17
 
  ; store pitch for dcf keyboard tracking
  sts PITCH,r23

;; lfo modulation

  ; check lfo mode
  lds r16,LFO_MODE_REG
  andi r16,(1<<LFO_MODE_BIT)

  ; skip when lfo mode = dcf
  brne dcomod_lfo_done

  ; get lfo parameters
  lds r16,LFO0+LFOVALUE
  lds r17,LFOLEVEL

  ; nonlinear potentiometer function
  ;  $00..$7f -> x/2
  ;  $80..$ff -> 3x/2 - $80

  mov r18,r17 ; x
  lsr r17     ; x/2

  cpi r18,$80
  brlo dcomod_lfo_delta
  subi r17,$80
  add r17,r18

dcomod_lfo_delta:
  ; calculate lfo modulation amount
  mulsu r16,r17
  movw r16,r0
  ldi r18,4
  rcall ASR16

  ; add to pitch
  add r22,r16
  adc r23,r17

  ; confine pitch to 0..108
  ; check lower bound
  tst r23
  brpl dcomod_lfo_limit

  ldi r22,0
  ldi r23,0
  rjmp dcomod_lfo_done

dcomod_lfo_limit:
  ; check upper bound
  cpi r23,109
  brlo dcomod_lfo_done

  ldi r22,0
  ldi r23,108

dcomod_lfo_done:
  ; save a copy of pitch for dcob
  push r22
  push r23

;; phase delta a

  ; octave transpose
  lds r16,DCOA_TRANSPOSE_REG
  sbrc r16,DCOA_TRANSPOSE_BIT
  subi r23,-12

  ; calculate delta
  rcall LOAD_DELTA

  ; store delta
  sts DELTAA_0,r17
  sts DELTAA_1,r18
  sts DELTAA_2,r19

;; phase delta b

;  pop r23
;  pop r22

  ; calculate detune
  lds r16,DETUNEB
  rcall NONLINPOT
  subi r17,$80
  
  ldi r18,4
  rcall ASR16

  ; apply detune to phase delta
  pop r23
  pop r22
  add r22,r16
  adc r23,r17

  ; octave transpose
  lds r16,DCOB_TRANSPOSE_REG
  sbrc r16,DCOB_TRANSPOSE_BIT
  subi r23,-12

  ; calculate delta 
  rcall LOAD_DELTA

  ; store delta
  sts DELTAB_0,r17
  sts DELTAB_1,r18
  sts DELTAB_2,r19
  ret


;; dcf modulation
dcfmod:

  ; initial modulation amount
  ldi r30,0
  ldi r31,0

;; lfo
dcfmod_lfo:

  ; skip when lfo mode = dco
  lds r16,LFO_MODE_REG
  andi r16,(1<<LFO_MODE_BIT)
  breq dcfmod_env

  ; get lfo parameters
  lds r16,LFOVALUE
  lds r17,LFOLEVEL

  ; calculate modulation amount
  mulsu r16,r17
  mov r30,r1
  ldi r31,0

  ; sign extend high byte
  rol r1
  sbc r31,r31

;; env
dcfmod_env:

  ; get envelope parameters
  lds r16,ENV1+ENVH
  lds r17,VCFENVMOD

  ; calculate modulation amount
  mul r16,r17

  ; add high byte to modulation total
  rol r0
  adc r30,r1
  adc r31,ZERO

;; keyboard tracking

  ; skip when keyboard tracking is off
  lds r16,DCF_TRACK_REG
  andi r16,(1<<DCF_TRACK_BIT)
  breq dcfmod_cutoff
  
  lds r16,PITCH   ;R16 = n (12/octave)	0..96
  lsl r16         ;R16 = 2*n (24/octave)	0..192
  subi r16,96     ;R16 = 2*(n-48) (24/octave)   -96..+96
  ldi r17,171
  mulsu r16,r17   ;R1 = 1,5*(n-48) (16/octave) -64..+64

  ; sign extension
  ldi r18,0
  sbrc r1,7
  ldi r18,$ff
  add r30,r1
  adc r31,r18

;; cutoff
dcfmod_cutoff:

  lds r16,CUTOFF
  clr r17
  add r16,r30
  adc r17,r31
  tst r17
  brpl dcfmod_cutoff_max

dcfmod_cutoff_min:
  ldi r16,0
  rjmp dcfmod_done

dcfmod_cutoff_max:
  breq dcfmod_done
  ldi r16,255

dcfmod_done:

  ; confine to 0..63 for table lookup
  lsr r16
  lsr r16

  ; get table adjusted value and save it
  ldi r30,low(TAB_VCF)
  ldi r31,high(TAB_VCF)
  rcall TAB_BYTE
  sts LPF_I,r0
  ret
  

;; level
dca:

  ; check dca mode
  lds r17,DCA_ENV_ENABLE_REG
  bst r17,DCA_ENV_ENABLE_BIT
  brts dca_env

dca_gate:
  ; GATE={0,1} -> r16={0,$ff}
  lds r16,GATE
  ror r16
  ldi r16,0
  sbc r16,r16
  rjmp dca_done

dca_env:
  ; get envelope
;  lds r16,ENV_INTEGR
  lds r16,ENV0+ENVH

  ; apply velocity
  lds r17,MIDIVELOCITY
  lsl r17
  mul r16,r17
  mov r16,r1

  ; confine r16 to 0..45 for table lookup
  ldi r17,46
  mul r16,r17
  mov r16,r1

  ; lookup dca value in table
  ldi r30,low(TAB_VCA<<1)
  ldi r31,high(TAB_VCA<<1)
  add r30,r16
  sbci r31,0
  lpm r16,z

dca_done:
  sts LEVEL,r16
  ret


;; pseudo-random shift register
random:

  ; reg := (reg << 1) | (reg[23] xor reg[18])

  ; load shift register
  lds r16,SHIFTREG_0
  lds r17,SHIFTREG_1
  lds r18,SHIFTREG_2

  ; r19 get bit 23
  bst r18,7
  bld r19,0

  ; r20 gets bit 18
  bst r18,2
  bld r20,0

  ; xor and shift result to carry
  eor r19,r20
  lsr r19

  ; rotate shift register
  rol r16
  rol r17
  rol r18

  ; save shift register
  sts SHIFTREG_0,r16
  sts SHIFTREG_1,r17
  sts SHIFTREG_2,r18
  ret
  

;; arpeggiator

.equ ARPSTATEREST = 0
.equ ARPSTATENOTEON = 1
.equ ARPSTATENOTEOFF = 2

arp:
  ; get state
  lds r16,ARPSTATE
  cpi r16,ARPSTATEREST
  breq arp_rest
  cpi r16,ARPSTATENOTEON
  breq arp_noteon
  cpi r16,ARPSTATENOTEOFF
  breq arp_noteoff
  ret

arp_rest:
  ; wait for active note
  lds r16,NOTESACTIVE
  tst r16
  breq arp_rest_done

  ; reset parameters
  lds r17,ARP_SYNC_REG
  bst r17,ARP_SYNC_BIT
  brts arp_rest_start

  sts ARPPHASE+0,zero
  sts ARPPHASE+1,zero
  sts ARPPHASE+2,zero
  ldi r16,CONFNOTES-1
  sts ARPNOTE,r16

arp_rest_start:
  ; start first note
  rcall note_start

  ; next state
  ldi r16,ARPSTATENOTEON
  sts ARPSTATE,r16

arp_rest_done:
  ret

arp_noteon:
  ; check for active notes
  lds r16,NOTESACTIVE
  tst r16
  brne arp_noteon_active

arp_noteon_notactive:
  ; no more active notes
  rcall note_stop

  ldi r16,ARPSTATEREST
  sts ARPSTATE,r16
  ret

arp_noteon_active:
  ; increment phase
  rcall arp_increment

  ; check for end of note
  lds r19,ARPTHRESHOLD
  cp r18,r19
  brsh arp_noteon_stop
  ret

arp_noteon_stop:
  rcall note_stop

  ldi r16,ARPSTATENOTEOFF
  sts ARPSTATE,r16
  ret

arp_noteoff:
  ; check for active notes
  lds r16,NOTESACTIVE
  tst r16
  brne arp_noteoff_active

arp_noteoff_notactive:
  ; no more active notes
  ldi r16,ARPSTATEREST
  sts ARPSTATE,r16
  ret

arp_noteoff_active:
  ; increment phase
  rcall arp_increment

  ; check for start of new note
  lds r19,ARPTHRESHOLD
  cp r18,r19
  brlo arp_noteoff_start
  ret

arp_noteoff_start:
  rcall note_next
  rcall note_start

  ldi r16,ARPSTATENOTEON
  sts ARPSTATE,r16
  ret

arp_increment:
  ; get phase increment
  lds r16,ARPRATE
  rcall rate

  ; get current phase
  lds r19,ARPPHASE+0
  lds r20,ARPPHASE+1
  lds r21,ARPPHASE+2

  ; add increment
  add r16,r19
  adc r17,r20
  adc r18,r21

  ; store new phase
  sts ARPPHASE+0,r16
  sts ARPPHASE+1,r17
  sts ARPPHASE+2,r18
  ret


note:
  ; check if arppegiator is enabled
  lds r16,ARP_ENABLE_REG
  bst r16,ARP_ENABLE_BIT
  brtc note_normal
  rjmp arp

note_normal:
  ; get note state
  lds r30,GATE
  cpi r30,0
  brne note_normal_on

note_normal_off:
  ; wait for active note
  lds r16,NOTESACTIVE
  tst r16
  breq note_normal_off_done

  ; start most recent note
  rcall note_current
  rcall note_start

note_normal_off_done:
  ret

note_normal_on:
  ; check if no more notes
  lds r16,NOTESACTIVE
  tst r16
  brne note_normal_on_update

  ; stop note
  rcall note_stop
  ret

note_normal_on_update:
  ; update current note
  rcall note_current
  ret


;; switch matrix scanning
matrix:

  ; get switch variables
  lds r19,SWITCH1
  lds r20,SWITCH2
  lds r21,SWITCH3

  ; get state
  lds r16,KEYSTATE
  mov r17,r16
  swap r17

  ; set all bits that can be affected during this state
  or r19,r16
  or r19,r17
  or r20,r16
  or r20,r17
  or r21,r16
  or r21,r17

  ; clear the bits not asserted
  in r30,PINB

  sbrc r30,0
    eor r19,r17
  sbrc r30,1
    eor r19,r16
  sbrc r30,2
    eor r20,r17
  sbrc r30,3
    eor r20,r16
  sbrc r30,4
    eor r21,r17

  ; update switch variables
  sts SWITCH1,r19
  sts SWITCH2,r20
  sts SWITCH3,r21

  ; update state
  sbrc r16,7
  swap r16
  lsl r16
  sts KEYSTATE,r16

  ; output columns
  in r30,PORTD
  ori r30,$f0
  eor r30,r16
  out PORTD,r30
  ret

;; main timer interrupt
timer0_overflow:
  push r16

  ; push sreg
  in r16,SREG
  push r16

  push r17
  push r18
  push r19
  push r30
  push r31
  push r0
  push r1

;; dcos

;; dco a

  ; waveform selection
  lds r31,DCOAWAVE
  lsr r31
  lsr r31
  ori r31,$40

  ; accumulator sets index in bank
  mov r30,PHASEA_2

  ; get value from wavetable
  lpm r17,z

  ; average with previous value, save new previous
;  lds r19,OLDWAVEA
;  sts OLDWAVEA,r17
;  add r17,r19
;  ror r17

;; dco b  

  ; waveform selection
  lds r31,DCOBWAVE
  lsr r31
  lsr r31
  ori r31,$40

  ; accumulator sets index in bank
  mov r30,PHASEB_2

  ; get value from wavetable
  lpm r16,z

  ; average with previous value, save new previous
;  lds r19,OLDWAVEB
;  sts OLDWAVEB,r16
;  add r16,r19
;  ror r16

  ; apply XOR distortion if desired		    
  lds r18,DCO_DISTORTION_REG
  sbrc r18,DCO_DISTORTION_BIT
  eor r17,r16

;; mixer
  lds r19,DCOALEVEL
  subi r17,$80
  mulsu r17,r19
  asr r1
  ror r0
  movw r30,r0

  lds r19,DCOBLEVEL
  subi r16,$80
  mulsu r16,r19
  asr r1
  ror r0
  add r30,r0
  adc r31,r1

  asr r31
  ror r30

  ; store dco b wave in signed form for fm
  sts WAVEB,r16

  movw r16,r30

;; dcf (moog)
.if CONFFILTER == CONFFILTERMOOG

  mov r16,r17

  ; tanh table high address byte
  ldi r31,high(tanh_table<<1)

  ; x(n) - r * yd(n-1)
  lds r18,RESONANCE
  mov r17,moog_yd
  mulsu r17,r18
  sub r16,r1

  ; tanh(x(n) - r * yd(n-1)) - wa(n-1)
  mov r30,r16
  lpm r16,z
  sub r16,moog_wa

  ; ya(n) := ya(n-1) + g * (tanh(x(n) - r * yd(n-1)) - wa(n-1))
  lds r17,LPF_I
  mulsu r16,r17
  add moog_ya,r1

  ; wa(n) := tanh(ya(n))
  mov r30,moog_ya
  lpm r16,z
  mov moog_wa,r16

  ; yb(n) := yb(n-1) + g * (wa(n) - wb(n-1))
  sub r16,moog_wb
  mulsu r16,r17
  add moog_yb,r1

  ; wb(n) := tanh(yb(n))
  mov r30,moog_yb
  lpm r16,z
  mov moog_wb,r16

  ; yc(n) := yc(n-1) + g * (wb(n) - wc(n-1))
  sub r16,moog_wc
  mulsu r16,r17
  add moog_yc,r1

  ; wc(n) := tanh(yc(n))
  mov r30,moog_yc
  lpm r16,z
  mov moog_wc,r16

  ; wc(n) - tanh(yd(n-1))
  mov r30,moog_yd
  lpm r18,z
  sub r16,r18

  ; yd(n) := yd(n-1) + g * (wc(n) - tanh(yd(n-1)))
  mulsu r16,r17
  add moog_yd,r1

  ; dcf output
  mov r17,moog_yd
  clr r16

.endif

;; dcf (svf)
.if CONFFILTER == CONFFILTERSVF

  lds r18,LPF_I
;  lsr r18
;  ldi r30,low(svf_table<<1)
;  ldi r31,high(svf_table<<1)
;  add r30,r18
;  adc r31,zero
;  lpm r18,z

  lds r19,CUTOFF

  ; low := low + f * band
  mov r16,svf_band
  mulsu r16,r18
  add r1,svf_low
  mov svf_low,r1

  ; high := q * (in - band) - low
  sub r17,svf_band
  mulsu r17,r19
  sub r1,svf_low
  mov svf_high,r1

  ; band := f * high + band
  mov r17,svf_high
  mulsu r17,r18
  add r1,svf_band
  mov svf_band,r1

  ; notch := high + low
  mov r17,svf_high
  add r17,svf_low
  mov svf_notch,r17

  ; select filter type
  lds r18,SWITCH3
  andi r18,$03

dcf_svf_lp:
  cpi r18,0
  brne dcf_svf_hp
  mov r17,svf_low
  rjmp dcf_svf_done

dcf_svf_hp:
  cpi r18,1
  brne dcf_svf_band
  mov r17, svf_high
  rjmp dcf_svf_done
  
dcf_svf_band:
  cpi r18,2
  brne dcf_svf_notch
  mov r17,svf_band
  rjmp dcf_svf_done

dcf_svf_notch:
  mov r17,svf_band

dcf_svf_done:
  clr r16

.endif

;; dcf (iir)
.if CONFFILTER == CONFFILTERIIR
  ; select filter type
  lds r18,MODEFLAGS2
  bst r18,sw_dcf_mode
  brts dcf_iir_hp

dcf_iir_lp:
  lds r18,LPF_I

  ; x[n] - y[n-1]
  sub r16,YN1_L
  sbc r17,YN1_H

  ; m * (x[n] - y[n-1])
  mulsu r17,r18
  movw r16,r0

  ; y[n-1] + m * (x[n] - y[n-1])
  add r16,YN1_L
  adc r17,YN1_H

  movw YN1_L,r16
  rjmp dcf_iir_done
  
dcf_iir_hp:
  lds r18,LPF_I

  ; x[n] - x[n-1]
  movw r30,r16
  sub r30,XN1_L
  sbc r31,XN1_H

  movw XN1_L,r16

  ; m * y[n-1]
  movw r16,YN1_L
  mulsu r17,r18
  movw r16,r0
  
  ; x[n] - x[n-1] + m * y[n-1]
  add r16,r30
  adc r17,r31

  movw YN1_L,r16

dcf_iir_done:
.endif

;; dca

  lds r18,LEVEL

  mul r16,r18
  mov r30,r1
  clr r31

  mulsu r17,r18
  sbc r31,r31
  add r30,r0
  adc r31,r1
  
;; write sample to dac

  ; convert two's comp to offset binary
  subi r31,$80

  ; reset latch clocks
  cbi PORTD,2
  cbi PORTD,3

  ; output low byte
  out PORTC,r30
  sbi PORTD,2

  ; output high byte
  out PORTC,r31
  sbi PORTD,3

;; increment phase

  ldi r30,low(DELTAA_0)
  ldi r31,high(DELTAA_0)
  ld r16,z+
  add PHASEA_0,r16
  ld r16,z+
  adc PHASEA_1,r16
  ld r16,z+
  adc PHASEA_2,r16
  ld r16,z+
  add PHASEB_0,r16
  ld r16,z+
  adc PHASEB_1,r16
  ld r16,z+
  adc PHASEB_2,r16

  lds r16,DCOA_NOISE_REG
  bst r16,DCOA_NOISE_BIT
  brts dco_noise

;; fm
dco_fm:
  ; mod * depth
  lds r16,WAVEB
  lds r17,FMDEPTH
  mulsu r16,r17
  movw r18,r0

  ; delta * mod * depth
  lds r16,DELTAA_0
  clr r17
  mulsu r19,r16
  sbc r17,r17
  add PHASEA_0,r1
  adc PHASEA_1,r17
  adc PHASEA_2,r17

  lds r16,DELTAA_1
  mulsu r19,r16
  add PHASEA_0,r0
  adc PHASEA_1,r1
  adc PHASEA_2,r17

  lds r16,DELTAA_2
  mulsu r19,r16
  add PHASEA_1,r0
  adc PHASEA_2,r1
  rjmp control_signals

;; noise
dco_noise:
  ser r16
  sbrc PHASEA_2,3
  com r16
  sbrc PHASEA_2,4
  com r16
  sbrc PHASEA_2,6
  com r16
  sbrc PHASEA_2,7
  com r16

  lsl r16
  rol PHASEA_0
  rol PHASEA_1
  rol PHASEA_2

;; evaluate control signals every CONFCONTROLTICKS timer ticks
control_signals:
  lds r16,ticks
  inc r16
  andi r16,(CONFCONTROLTICKS-1)
  sts ticks,r16
  brne timer0_overflow_done

  sei
  push r29
  push r28
  push r27
  push r26
  push r25
  push r24
  push r23
  push r22
  push r21
  push r20

  rcall note
  rcall modwheel

  ldi r28,low(LFO0)
  ldi r29,high(LFO0)
  lds r16,LFOFREQ

  lds r17,LFO_RANDOM_REG
  bst r17,LFO_RANDOM_BIT
  bld r25,LFOFLAGRANDOM

  lds r17,LFO_WAVE_REG
  bst r17,LFO_WAVE_BIT
  bld r25,LFOFLAGSQUARE
  rcall lfo

  ldi r28,low(ENV0PARAM)
  ldi r29,high(ENV0PARAM)
  ldi r30,low(ENV0)
  ldi r31,high(ENV0)
  rcall env

  ldi r28,low(ENV1PARAM)
  ldi r29,high(ENV1PARAM)
  ldi r30,low(ENV1)
  ldi r31,high(ENV1)
  rcall env

  rcall dcomod
  rcall dcfmod
  rcall dca
  rcall random

;; run background tasks every CONFBACKGROUNDTICKS control ticks
  lds r16,control_ticks
  inc r16
  andi r16,(CONFBACKGROUNDTICKS-1)
  sts control_ticks,r16
  brne timer0_overflow_control_done

  rcall matrix

timer0_overflow_control_done:
  pop r20
  pop r21
  pop r22
  pop r23
  pop r24
  pop r25
  pop r26
  pop r27
  pop r28
  pop r29

timer0_overflow_done:
;; end sample isr
  pop r1
  pop r0
  pop r31
  pop r30
  pop r19
  pop r18
  pop r17

  ; pop sreg
  pop r16
  out SREG,r16

  pop r16
  reti


;; note management

note_stop:
  ; set envelopes to release
  ldi r16,ENVSTATERELEASE
  sts ENV0+ENVSTATE,r16
  sts ENV1+ENVSTATE,r16
  
  ; turn off gate
  ldi r17,0
  sts GATE,r17

  ; turn off led
  cbi PORTD,1
  ret


note_start:
  ; envelopes set to attack
  ldi r16,ENVSTATEATTACK
  sts ENV0+ENVSTATE,r16
  sts ENV1+ENVSTATE,r16

  ; lfo reset if sync is activated
  lds r16,LFO_SYNC_REG
  bst r16,LFO_SYNC_BIT
  brtc note_start_nolfosync

note_start_lfosync:
  ldi r16,$ff
  sts LFO0+LFOPHASE0,r16
  sts LFO0+LFOPHASE1,r16
  ldi r16,$7f
  sts LFO0+LFOPHASE2,r16
  ldi r16,LFOSTATEFALL
  sts LFO0+LFOSTATE,r16
  
note_start_nolfosync:
  ; decrement PORTACNT if != 0
  lds r16,PORTACNT
  cpse r16,zero
  dec r16
  sts PORTACNT,r16

  ; turn gate on
  ldi r17,1
  sts GATE,r17

  ; turn on led
  sbi PORTD,1
  ret


; r16     -> note to search for
; r17     <- note number
; r27:r26 <- note address
; t       <- note found?
; r18     xx

note_find:
  ldi r26,low(NOTES+CONFNOTES)
  ldi r27,high(NOTES+CONFNOTES)
  ldi r17,CONFNOTES

note_find_loop:
  ld r18,-x
  cp r18,r16
  breq note_find_true
  dec r17
  brne note_find_loop

note_find_false:
  false
  ret

note_find_true:
  dec r17
  true
  ret

note_allocate:
  ldi r16,$ff
  rjmp note_find

; r16 -> note

note_add:
  push r16
  
  ; any notes free?
  rcall note_allocate
  branch_false note_add_done

  ; store note
  pop r16
  st x,r16

  ; increment active note count
  lds r16,NOTESACTIVE
  inc r16
  sts NOTESACTIVE,r16
  
;  ; first note requires gate
;  cpi r16,1
;  brne note_add_done
;
;note_add_first:
;  rcall note_start

note_add_done:
  ret


note_delete:
  ; search for active note
  rcall note_find
  branch_false note_delete_done

  ; free note
  ldi r16,$ff
  st x,r16

  ; in normal mode, shift other notes to fill in gap
  lds r18,ARP_ENABLE_REG
  sbrs r18,ARP_ENABLE_BIT
  rcall note_shift

  ; decrement active note count
  lds r16,NOTESACTIVE
  dec r16
  sts NOTESACTIVE,r16
  
;  ; last note released
;  cpi r16,0
;  brne note_delete_done
;
;note_delete_last:
;  rcall note_stop

note_delete_done:
  ret


note_shift:
  tst r17

note_shift_loop:
  breq note_shift_done
  sbiw r26,1
  ld r16,x+
  st x,r16
  sbiw r26,1
  dec r17
  rjmp note_shift_loop

note_shift_done:
  ret


note_current:
  ldi r26,low(NOTES)
  ldi r27,high(NOTES)
  ldi r17,CONFNOTES

note_current_loop:
  ld r18,x+
  cpi r18,$ff
  brne note_current_true
  dec r17
  brne note_current_loop

note_current_false:
  ldi r16,$ff
  sts MIDINOTE,r16
  ret

note_current_true:
  ld r16,-x
  sts MIDINOTE,r16
  ret
  

note_next:
  ldi r26,low(NOTES)
  ldi r27,high(NOTES)
  lds r17,ARPNOTE
  add r26,r17
  adc r27,zero
  mov r18,r17

note_next_loop:
  tst r18
  brne note_next_check

  subi r18,-CONFNOTES
  adiw r26,CONFNOTES

note_next_check:
  dec r18
  cp r18,r17
  breq note_next_done

  ld r16,-x
  cpi r16,$ff
  breq note_next_loop

note_next_valid:
  ; store new note
  sts MIDINOTE,r16

note_next_done:
  sts ARPNOTE,r18
  ret


;; midi in

usart_rxc:
  push r16
  in r16,SREG
  push r16

  ; read received byte
  in r16,UDR

  ; disable further usart_rxc interrupts
  cbi UCSRB,RXCIE

  ; enable other interrupts
  sei

  push r17
  push r18
  push r26
  push r27
  
  ; check for status byte (bit7)
  tst r16
  brpl midi_data

;; status byte (1xxxxxxx)
midi_status:
  mov r17,r16
  andi r17,$f0
  cpi r17,$80
  breq midi_accept ;8x note off
  cpi r17,$90
  breq midi_accept ;9x note on
  cpi r17,$b0
  breq midi_accept ;Bx control change
  cpi r17,$d0
  breq midi_accept ;Dx channel pressure
  cpi r17,$e0
  breq midi_accept ;Ex pitch bend

midi_dont_accept:
  sts MIDIPHASE,zero
  rjmp midi_done		    ;Ax polyphonic aftertouch
						                ;Cx program change
						                ;Fx system

midi_accept:
  ; set phase to x0
  sts MIDIPHASE,r17

  ; store midi channel (1..16)
  andi r16,$0f
  inc r16
  sts MIDICHANNEL,r16

  ; check for OMNI mode
  lds r17,SETMIDICHANNEL	;0 for OMNI or 1..15   050505
  tst r17
  breq midi_accept_done

  ; check for correct channel
  cp r17,r16
  breq midi_accept_done

  ; phase = 0 if incorrect channel
  ldi r17,0
  sts MIDIPHASE,r17

midi_accept_done:
  rjmp midi_done

;; data byte (0xxxxxxx)
midi_data:
  lds r17,MIDIPHASE

midi_data_80:
  cpi r17,$80
  brne midi_data_81
  inc r17
  sts MIDIPHASE,r17
  sts MIDIDATA0,r16
  rjmp midi_done

midi_data_81:
  cpi r17,$81
  brne midi_data_90
  dec r17
  sts MIDIPHASE,r17
  rjmp midi_noteoff

midi_data_90:
  cpi r17,$90
  brne midi_data_91
  inc r17
  sts MIDIPHASE,r17
  sts MIDIDATA0,r16
  rjmp midi_done

midi_data_91:
  cpi r17,$91
  brne midi_data_b0
  dec r17
  sts MIDIPHASE,r17
  cpse r16,zero
  rjmp midi_noteon
  rjmp midi_noteoff

midi_data_b0:
  cpi r17,$b0
  brne midi_data_b1
  inc r17
  sts MIDIPHASE,r17
  sts MIDIDATA0,r16
  rjmp midi_done

midi_data_b1:
  cpi r17,$b1
  brne midi_data_d0
  dec r17
  sts MIDIPHASE,r17

  ; get address from controller number
  lds r17,MIDIDATA0
  ldi r26,low(MIDICC)
  ldi r27,high(MIDICC)
  add r26,r17
  adc r27,zero

  ; shift midi data to 0..254 to match values from adc
  lsl r16

  ; store in controller data table
  st x,r16
  rjmp midi_done

midi_data_d0:
  cpi r17,$d0
  brne midi_data_e0
  lsl r16
  sts MIDIPRESSURE,r16
  rjmp midi_done

midi_data_e0: 
  cpi r17,$e0
  brne midi_data_e1
  inc r17
  sts MIDIPHASE,r17
  sts MIDIDATA0,r16
  rjmp midi_done

midi_data_e1:
  cpi r17,$e1
  brne midi_done
  dec r17
  sts MIDIPHASE,r17
  
  lds r17,MIDIDATA0
  lsl r17      ;/ R17 = dFine*2	0..254
  lsl r17      ;\ R16,R17 = P.B.data
  rol r16      ;/ 0..255,996
  subi r16,$80 ;R16,R17 = -128,000..+127,996

  ; store pitch bend value
  sts MIDIPBEND_L,r17
  sts MIDIPBEND_H,r16
  rjmp midi_done

midi_done:
  pop r27
  pop r26
  pop r18
  pop r17
  pop r16
  out SREG,r16
  pop r16

  ; reenable usart_rxc interrupt
  sbi UCSRB,RXCIE
  reti


midi_noteoff:
  lds r16,MIDIDATA0
  rcall note_delete
  rcall note_current
  rjmp midi_done


midi_noteon:
  ; store velocity
  sts MIDIVELOCITY,r16

  lds r16,MIDIDATA0
  rcall note_add
  rcall note_current
  rjmp midi_done


;-------------------------------------------------------------------------------------------------------------------
;		M A I N   L E V E L   S U B R O U T I N E S
;-------------------------------------------------------------------------------------------------------------------

;=============================================================================
;			I/O subroutines
;=============================================================================

;; adc isr
adc_complete:
  push r16
  in r16,SREG
  push r16
  push r26
  push r27
  
  ; get address for current channel
  ldi r26,low(ADCDATA)
  ldi r27,high(ADCDATA)
  in r16,ADMUX
  andi r16,$07
  add r26,r16
  adc r27,zero

  ; save adc value in ram
  in r16,ADCH
  st x,r16

  ; select next channel
  in r16,ADMUX
  inc r16
  andi r16,(1<<ADLAR)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)
  out ADMUX,r16

  ; start next conversion
  sbi ADCSRA,ADSC
  
  pop r27
  pop r26
  pop r16
  out SREG,r16
  pop r16
  reti
  
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
ASR16:		tst	    R18
		    breq	ASR16_EXIT
		    push	R18

ASR16_LOOP:	asr	    R17		            ;\
		    ror	    R16		            ;/ R17,R16 = R17,R16 asr 1
		    dec	    R18
		    brne	ASR16_LOOP
		    pop	    R18

ASR16_EXIT:	ret

;-----------------------------------------------------------------------------
;32 bit x 16 bit multiplication (unsigned)
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R19:R18:R17:R16 = x			0..2^32-1
;	        R23:R22 = y			0,yyyyyyyyyyyyyyyy	0..0,9999847
;Out:	    R19:R18:R17:R16 = x * y		0..2^32-1
;Used:	    SREG,R24-R29
;-----------------------------------------------------------------------------
MUL32X16:
  mul r22,r16
  movw r24,r0
  clr r26
  clr r27
  clr r28
  clr r29

  mul r22,r17
  add r25,r0
  adc r26,r1
  adc r27,zero
  adc r28,zero
  adc r29,zero

  mul r22,r18
  add r26,r0
  adc r27,r1
  adc r28,zero
  adc r29,zero

  mul r22,r19
  add r27,r0
  adc r28,r1
  adc r29,zero

  mul r23,r16
  add r25,r0
  adc r26,r1
  adc r27,zero
  adc r28,zero
  adc r29,zero

  mul r23,r17
  add r26,r0
  adc r27,r1
  adc r28,zero
  adc r29,zero

  mul r23,r18
  add r27,r0
  adc r28,r1
  adc r29,zero

  mul r23,r19
  add r28,r0
  adc r29,r1

  movw r16,r26
  movw r18,r28
  ret


;-----------------------------------------------------------------------------
; Load phase delta from ROM
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R23,R22 = indeks = 0,0..12,0 = n,f
;Out:	    R19:R18:R17:R16 = delta
;Used:	    SREG,R0,R21,R24-R31
;-----------------------------------------------------------------------------
LOAD_DELTA:
  ; preserve index
  push r22
  push r23

  ; calculate table index
  mov r30,r23
  clr r31
  lsl r30
  rol r31
  lsl r30
  rol r31
  subi r30,low(-(phase_table<<1))
  sbci r31,high(-(phase_table<<1))

  ; table[i]
  lpm r24,z+
  lpm r25,z+
  lpm r26,z+
  lpm r27,z+

  ; table[i+1]
  lpm r16,z+
  lpm r17,z+
  lpm r18,z+
  lpm r19,z+

  ; table[i+1] - table[i]
  sub r16,r24
  sbc r17,r25
  sbc r18,r26
  sbc r19,r27

  ; calculate fractional table value
  push r24
  push r25
  push r26
  push r27
  mov r23,r22
  ldi r22,0
  rcall MUL32X16
  pop r27
  pop r26
  pop r25
  pop r24

  ; add fractional to base table value
  add r16,r24
  adc r17,r25
  adc r18,r26
  adc r19,r27

  ; recall index
  pop r23
  pop r22
  ret


;-----------------------------------------------------------------------------
;note number recalculation
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R23 = n	                    0..139 = m12 + 12*n12
;Out:	    R23 = m12                   0..11
;	        R20 = n12                   0..11
;Used:	    SREG
;-----------------------------------------------------------------------------
NOTERECALC:	ldi	R20,0			        ;n12 = 0
		    rjmp	NRC_2

NRC_1:		subi	R23,12			    ;m12 -= 12
		    inc	    R20			        ;n12++

NRC_2:		cpi	    R23,12
		    brsh	NRC_1			    ;repeat while m12 >= 12
		    ret

;-----------------------------------------------------------------------------
;read a byte from a table
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R16 = i		                0..255
;	        R31:R30 = &Tab
;Out:	    R0 = Tab[i]	                0..255
;Used:	    SREG,R30,R31
;-----------------------------------------------------------------------------
TAB_BYTE:	add	    R30,R30			    ;\
		    adc	    R31,R31			    ;/ Z = 2 * &Tab
		    add	    R30,R16
		    adc	    R31,ZERO
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
TAB_WORD:	add	    R30,R16
		    adc	    R31,ZERO
		    add	    R30,R30			    ;\
		    adc	    R31,R31			    ;/ Z = 2 * &Tab
		    lpm
		    mov	    R18,R0			    ;LSByte
		    adiw	R30,1			    ;Z++
		    lpm
		    mov	    R19,R0			    ;MSByte
		    ret

;-----------------------------------------------------------------------------
;conversion of the "detune B" potentiometer function
;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;In:	    R16 = x		                0..255
;Out:	    R17,R16 = y	                0,000..255,996
;Used:	    SREG,R18-R30
;-----------------------------------------------------------------------------
NONLINPOT:	ldi	    R22,0
		    mov	    R23,R16
    		cpi	    R23,112
		    brlo	NLP_I
		    cpi	    R23,144
		    brlo	NLP_II
		    rjmp	NLP_III

NLP_I:		ldi	    R16,0			    ;\  R18,R17:R16 = m =
		    ldi	    R17,32			    ; > = 126/112 =
		    ldi	    R18,1			    ;/  = 1,125
    		ldi	    R30,0			    ;\ R31,R30 = n =
		    ldi	    R31,0			    ;/ = 0,0
		    rjmp	NLP_CONT

NLP_II:		ldi	    R16,8			    ;\  R18,R17:R16 = m =
		    ldi	    R17,33			    ; > = (130-126)/(143-112) =
    		ldi	    R18,0			    ;/  = 0,129032258
		    ldi	    R30,140			    ;\ R31,R30 = n =
		    ldi	    R31,111			    ;/ = 126 - m*112 = 111,5483871
		    rjmp	NLP_CONT

NLP_III:	ldi	    R16,183			    ;\  R18,R17:R16 = m =
		    ldi	    R17,29			    ; > = (255-130)/(255-143) =
		    ldi	    R18,1			    ;/  = 1,116071429
    		ldi	    R30,103			    ;\ R31,R30 = n =
		    ldi	    R31,226			    ;/ 255 - m*255 = -29,59821429

NLP_CONT:	ldi	    R19,0
		    rcall	MUL32X16
		    add	    R16,R30
		    adc	    R17,R31
		    ret

;-------------------------------------------------------------------------------------------------------------------
;			M A I N   P R O G R A M
;-------------------------------------------------------------------------------------------------------------------
reset:
  ; disable interrupts
  cli

  ; zero register
  clr zero

  ; clear ram
  ldi r30,low(ramstart)
  ldi r31,high(ramstart)
  ldi r28,low(ramsize)
  ldi r29,high(ramsize)

reset_ram_loop:
  st z+,zero
  sbiw r28,1
  brne reset_ram_loop

  ; initialize stack
  ldi r16,low(ramend)
  ldi r17,high(ramend)
  out SPL,r16
  out SPH,r17

  ; initialize variables
  clr PHASEA_0
  clr PHASEA_1
  clr PHASEA_2
  clr PHASEB_0
  clr PHASEB_1
  clr PHASEB_2

  clr XN1_L
  clr XN1_H
  clr YN1_L
  clr YN1_H

		    ldi	    R16,2
		    sts	    PORTACNT,R16	    ;PORTACNT = 2
		    ldi	    R16,255
		    sts	    LPF_I,R16		    ;no DCF
		    sts	    MIDINOTE,R16	    ;note# = 255
		    sts	    MIDINOTEPREV,R16    ;note# = 255
		    ldi	    R16,0x5E		    ;\
		    ldi	    R17,0xB4		    ; \
		    ldi	    R18,0x76		    ;  \ initialising of
		    sts	    SHIFTREG_0,R16		;  / shift register
		    sts	    SHIFTREG_1,R17		; /
		    sts	    SHIFTREG_2,R18		;/

  ldi r16,$10
  sts KEYSTATE,r16

  ; initialize lfos
  ldi r16,LFOSTATERISE
  sts LFO0+LFOSTATE,r16
  sts LFO1+LFOSTATE,r16

  sts LFO0+LFOBOTTOM0,zero
  sts LFO0+LFOBOTTOM1,zero
  sts LFO1+LFOBOTTOM0,zero
  sts LFO1+LFOBOTTOM1,zero
  ldi r16,$80
  sts LFO0+LFOBOTTOM2,r16
  sts LFO1+LFOBOTTOM2,r16

  ldi r16,$ff
  sts LFO0+LFOTOP0,r16
  sts LFO0+LFOTOP1,r16
  sts LFO1+LFOTOP0,r16
  sts LFO1+LFOTOP1,r16
  ldi r16,$7f
  sts LFO0+LFOTOP2,r16
  sts LFO1+LFOTOP2,r16
  
  ; initialize envelopes
  ldi r16,ENVSTATESTOP
  sts ENV0+ENVSTATE,r16
  sts ENV1+ENVSTATE,r16

  sts ENV0+ENVL,zero
  sts ENV0+ENVM,zero
  sts ENV0+ENVH,zero
  sts ENV1+ENVL,zero
  sts ENV1+ENVM,zero
  sts ENV1+ENVH,zero

  ; initialize notes
  ldi r30,low(NOTES)
  ldi r31,high(NOTES)
  ldi r16,$ff
  ldi r17,CONFNOTES
reset_notes_init:
  st z+,r16
  dec r17
  brne reset_notes_init

;initialise sound parameters:
		    ldi	    R16,0
;		    sts	    LFOPHASE,R16		;
;		    sts	    ENVPHASE,R16		;
;		    sts	    DETUNEB_FRAC,R16	;\
;		    sts	    DETUNEB_INTG,R16	;/ detune = 0
		    sts	    LFOLEVEL,R16		;
		    sts	    VCFENVMOD,R16		;
		    ldi	    R16,84			    ;\
		    sts	    LFOFREQ,R16		    ;/
		    ldi	    R16,128			    ;\
;		    sts	    ATTACKTIME,R16		; >
;		    sts	    RELEASETIME,R16		;/
		    ldi r16,$7f<<1
		    sts MIDIEXPRESSION,r16

  ; initialize porta (zzzzzzzz/iiiiiiii)
  ldi r16,$00
  out PORTA,r16
  ldi r16,$00
  out DDRA,r16

  ; initialize portb (pppppppp/iiiiiiii)
  ldi r16,$ff
  out PORTB,r16
  ldi r16,$00
  out DDRB,r16

  ; initialize portc (00000000/oooooooo)
  ldi r16,$00
  out PORTC,r16
  ldi r16,$ff
  out DDRC,r16

  ; initialize portd (1111110z/oooooooi)
  ldi r16,$fc
  out PORTD,r16
  ldi r16,$fe
  out DDRD,r16

  ; enable timer0
  ; set prescaler to ck/1
  ldi r16,(1<<CS00)
  out TCCR0,r16

  ; enable timer1
  ; set prescaler to ck/256
  ldi r16,(1<<CS12)
  out TCCR1B,r16

  ; disable timer2
;  ldi r16,(1<<WGM21)|(1<<CS21)
;  out TCCR2,r16
;;  ldi r16,31
;  ldi r16,62
;  out OCR2,r16
  ldi r16,$00
  out TCCR2,r16 

  ; set baud rate of uart
  ldi r16,high((cpu_frequency/(baud_rate*16))-1)
  out UBRRH,r16
  ldi r16,low((cpu_frequency/(baud_rate*16))-1)
  out UBRRL,r16

  ; enable receiver and receiver interrupt
  ldi r16,(1<<RXCIE)|(1<<RXEN)
  out UCSRB,r16

  ; start ADC on channel 0, left justify result
  ldi r16,(1<<ADLAR)
  out ADMUX,r16

  ; enable ADC, ADC interrupt, set prescaler to 128, and start first conversion
  ldi r16,(1<<ADEN)|(1<<ADSC)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
  out ADCSRA,r16

  ; timer interrupts
  ldi r16,(1<<TOIE0)
  out TIMSK,r16

  ; let's go
  sei

main:
  rjmp main

;; tanh table
align 7
tanh_table:
.db $00,$00,$01,$02,$03,$04,$05,$06,$07,$08,$09,$0a,$0b,$0c,$0d,$0e
.db $0f,$10,$11,$12,$13,$14,$15,$16,$16,$17,$18,$19,$1a,$1b,$1b,$1c
.db $1d,$1e,$1f,$1f,$20,$21,$22,$22,$23,$24,$24,$25,$26,$26,$27,$28
.db $28,$29,$29,$2a,$2a,$2b,$2c,$2c,$2d,$2d,$2e,$2e,$2e,$2f,$2f,$30
.db $30,$31,$31,$31,$32,$32,$33,$33,$33,$34,$34,$34,$35,$35,$35,$36
.db $36,$36,$36,$37,$37,$37,$37,$38,$38,$38,$38,$38,$39,$39,$39,$39
.db $39,$3a,$3a,$3a,$3a,$3a,$3a,$3b,$3b,$3b,$3b,$3b,$3b,$3b,$3c,$3c
.db $3c,$3c,$3c,$3c,$3c,$3c,$3c,$3c,$3d,$3d,$3d,$3d,$3d,$3d,$3d,$3d
.db $c3,$c3,$c3,$c3,$c3,$c3,$c3,$c3,$c3,$c4,$c4,$c4,$c4,$c4,$c4,$c4
.db $c4,$c4,$c4,$c5,$c5,$c5,$c5,$c5,$c5,$c5,$c6,$c6,$c6,$c6,$c6,$c6
.db $c7,$c7,$c7,$c7,$c7,$c8,$c8,$c8,$c8,$c8,$c9,$c9,$c9,$c9,$ca,$ca
.db $ca,$ca,$cb,$cb,$cb,$cc,$cc,$cc,$cd,$cd,$cd,$ce,$ce,$cf,$cf,$cf
.db $d0,$d0,$d1,$d1,$d2,$d2,$d2,$d3,$d3,$d4,$d4,$d5,$d6,$d6,$d7,$d7
.db $d8,$d8,$d9,$da,$da,$db,$dc,$dc,$dd,$de,$de,$df,$e0,$e1,$e1,$e2
.db $e3,$e4,$e5,$e5,$e6,$e7,$e8,$e9,$ea,$ea,$eb,$ec,$ed,$ee,$ef,$f0
.db $f1,$f2,$f3,$f4,$f5,$f6,$f7,$f8,$f9,$fa,$fb,$fc,$fd,$fe,$ff,$00

;; waveform data

.org $2000

.if CONFWAVE == 1

.macro saw
.db $00, $01, $02, $03, $04, $05, $06, $07
.db $08, $09, $0a, $0b, $0c, $0d, $0e, $0f
.db $10, $11, $12, $13, $14, $15, $16, $17
.db $18, $19, $1a, $1b, $1c, $1d, $1e, $1f
.db $20, $21, $22, $23, $24, $25, $26, $27
.db $28, $29, $2a, $2b, $2c, $2d, $2e, $2f
.db $30, $31, $32, $33, $34, $35, $36, $37
.db $38, $39, $3a, $3b, $3c, $3d, $3e, $3f
.db $40, $41, $42, $43, $44, $45, $46, $47
.db $48, $49, $4a, $4b, $4c, $4d, $4e, $4f
.db $50, $51, $52, $53, $54, $55, $56, $57
.db $58, $59, $5a, $5b, $5c, $5d, $5e, $5f
.db $60, $61, $62, $63, $64, $65, $66, $67
.db $68, $69, $6a, $6b, $6c, $6d, $6e, $6f
.db $70, $71, $72, $73, $74, $75, $76, $77
.db $78, $79, $7a, $7b, $7c, $7d, $7e, $7f
.db $80, $81, $82, $83, $84, $85, $86, $87
.db $88, $89, $8a, $8b, $8c, $8d, $8e, $8f
.db $90, $91, $92, $93, $94, $95, $96, $97
.db $98, $99, $9a, $9b, $9c, $9d, $9e, $9f
.db $a0, $a1, $a2, $a3, $a4, $a5, $a6, $a7
.db $a8, $a9, $aa, $ab, $ac, $ad, $ae, $af
.db $b0, $b1, $b2, $b3, $b4, $b5, $b6, $b7
.db $b8, $b9, $ba, $bb, $bc, $bd, $be, $bf
.db $c0, $c1, $c2, $c3, $c4, $c5, $c6, $c7
.db $c8, $c9, $ca, $cb, $cc, $cd, $ce, $cf
.db $d0, $d1, $d2, $d3, $d4, $d5, $d6, $d7
.db $d8, $d9, $da, $db, $dc, $dd, $de, $df
.db $e0, $e1, $e2, $e3, $e4, $e5, $e6, $e7
.db $e8, $e9, $ea, $eb, $ec, $ed, $ee, $ef
.db $f0, $f1, $f2, $f3, $f4, $f5, $f6, $f7
.db $f8, $f9, $fa, $fb, $fc, $fd, $fe, $ff
.endmacro

.macro sine
.db $80, $83, $86, $89, $8c, $8f, $92, $95
.db $98, $9c, $9f, $a2, $a5, $a8, $ab, $ae
.db $b0, $b3, $b6, $b9, $bc, $bf, $c1, $c4
.db $c7, $c9, $cc, $ce, $d1, $d3, $d5, $d8
.db $da, $dc, $de, $e0, $e2, $e4, $e6, $e8
.db $ea, $ec, $ed, $ef, $f0, $f2, $f3, $f5
.db $f6, $f7, $f8, $f9, $fa, $fb, $fc, $fc
.db $fd, $fe, $fe, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $fe, $fe
.db $fd, $fc, $fc, $fb, $fa, $f9, $f8, $f7
.db $f6, $f5, $f3, $f2, $f0, $ef, $ed, $ec
.db $ea, $e8, $e6, $e4, $e2, $e0, $de, $dc
.db $da, $d8, $d5, $d3, $d1, $ce, $cc, $c9
.db $c7, $c4, $c1, $bf, $bc, $b9, $b6, $b3
.db $b0, $ae, $ab, $a8, $a5, $a2, $9f, $9c
.db $98, $95, $92, $8f, $8c, $89, $86, $83
.db $80, $7c, $79, $76, $73, $70, $6d, $6a
.db $67, $63, $60, $5d, $5a, $57, $54, $51
.db $4f, $4c, $49, $46, $43, $40, $3e, $3b
.db $38, $36, $33, $31, $2e, $2c, $2a, $27
.db $25, $23, $21, $1f, $1d, $1b, $19, $17
.db $15, $13, $12, $10, $0f, $0d, $0c, $0a
.db $09, $08, $07, $06, $05, $04, $03, $03
.db $02, $01, $01, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $01, $01
.db $02, $03, $03, $04, $05, $06, $07, $08
.db $09, $0a, $0c, $0d, $0f, $10, $12, $13
.db $15, $17, $19, $1b, $1d, $1f, $21, $23
.db $25, $27, $2a, $2c, $2e, $31, $33, $36
.db $38, $3b, $3e, $40, $43, $46, $49, $4c
.db $4f, $51, $54, $57, $5a, $5d, $60, $63
.db $67, $6a, $6d, $70, $73, $76, $79, $7c
.endmacro

.macro square
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $00, $00, $00, $00, $00, $00, $00, $00
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $ff, $ff
.endmacro

.macro triangle
.db $80, $82, $84, $86, $88, $8a, $8c, $8e
.db $90, $92, $94, $96, $98, $9a, $9c, $9e
.db $a0, $a2, $a4, $a6, $a8, $aa, $ac, $ae
.db $b0, $b2, $b4, $b6, $b8, $ba, $bc, $be
.db $c0, $c2, $c4, $c6, $c8, $ca, $cc, $ce
.db $d0, $d2, $d4, $d6, $d8, $da, $dc, $de
.db $e0, $e2, $e4, $e6, $e8, $ea, $ec, $ee
.db $f0, $f2, $f4, $f6, $f8, $fa, $fc, $fe
.db $ff, $fd, $fb, $f9, $f7, $f5, $f3, $f1
.db $ef, $ed, $eb, $e9, $e7, $e5, $e3, $e1
.db $df, $dd, $db, $d9, $d7, $d5, $d3, $d1
.db $cf, $cd, $cb, $c9, $c7, $c5, $c3, $c1
.db $bf, $bd, $bb, $b9, $b7, $b5, $b3, $b1
.db $af, $ad, $ab, $a9, $a7, $a5, $a3, $a1
.db $9f, $9d, $9b, $99, $97, $95, $93, $91
.db $8f, $8d, $8b, $89, $87, $85, $83, $81
.db $7f, $7d, $7b, $79, $77, $75, $73, $71
.db $6f, $6d, $6b, $69, $67, $65, $63, $61
.db $5f, $5d, $5b, $59, $57, $55, $53, $51
.db $4f, $4d, $4b, $49, $47, $45, $43, $41
.db $3f, $3d, $3b, $39, $37, $35, $33, $31
.db $2f, $2d, $2b, $29, $27, $25, $23, $21
.db $1f, $1d, $1b, $19, $17, $15, $13, $11
.db $0f, $0d, $0b, $09, $07, $05, $03, $01
.db $00, $02, $04, $06, $08, $0a, $0c, $0e
.db $10, $12, $14, $16, $18, $1a, $1c, $1e
.db $20, $22, $24, $26, $28, $2a, $2c, $2e
.db $30, $32, $34, $36, $38, $3a, $3c, $3e
.db $40, $42, $44, $46, $48, $4a, $4c, $4e
.db $50, $52, $54, $56, $58, $5a, $5c, $5e
.db $60, $62, $64, $66, $68, $6a, $6c, $6e
.db $70, $72, $74, $76, $78, $7a, $7c, $7e
.endmacro

  saw
  saw
  saw
  saw
  saw
  saw
  saw
  saw
  saw
  saw
  saw
  saw
  saw
  saw
  saw
  saw

  sine
  sine
  sine
  sine
  sine
  sine
  sine
  sine
  sine
  sine
  sine
  sine
  sine
  sine
  sine
  sine

  square
  square
  square
  square
  square
  square
  square
  square
  square
  square
  square
  square
  square
  square
  square
  square

  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle
  triangle

.elif CONFWAVE == 2

;; sine to saw morph

.db $80, $7d, $7a, $77, $74, $71, $6e, $6b
.db $68, $64, $61, $5e, $5b, $58, $55, $52
.db $50, $4d, $4a, $47, $44, $41, $3f, $3c
.db $39, $37, $34, $32, $2f, $2d, $2b, $28
.db $26, $24, $22, $20, $1e, $1c, $1a, $18
.db $16, $14, $13, $11, $10, $0e, $0d, $0b
.db $0a, $09, $08, $07, $06, $05, $04, $04
.db $03, $02, $02, $01, $01, $01, $01, $01
.db $00, $01, $01, $01, $01, $01, $02, $02
.db $03, $04, $04, $05, $06, $07, $08, $09
.db $0a, $0b, $0d, $0e, $10, $11, $13, $14
.db $16, $18, $1a, $1c, $1e, $20, $22, $24
.db $26, $28, $2b, $2d, $2f, $32, $34, $37
.db $39, $3c, $3f, $41, $44, $47, $4a, $4d
.db $50, $52, $55, $58, $5b, $5e, $61, $64
.db $68, $6b, $6e, $71, $74, $77, $7a, $7d
.db $80, $83, $86, $89, $8c, $8f, $92, $95
.db $98, $9c, $9f, $a2, $a5, $a8, $ab, $ae
.db $b0, $b3, $b6, $b9, $bc, $bf, $c1, $c4
.db $c7, $c9, $cc, $ce, $d1, $d3, $d5, $d8
.db $da, $dc, $de, $e0, $e2, $e4, $e6, $e8
.db $ea, $ec, $ed, $ef, $f0, $f2, $f3, $f5
.db $f6, $f7, $f8, $f9, $fa, $fb, $fc, $fc
.db $fd, $fe, $fe, $ff, $ff, $ff, $ff, $ff
.db $ff, $ff, $ff, $ff, $ff, $ff, $fe, $fe
.db $fd, $fc, $fc, $fb, $fa, $f9, $f8, $f7
.db $f6, $f5, $f3, $f2, $f0, $ef, $ed, $ec
.db $ea, $e8, $e6, $e4, $e2, $e0, $de, $dc
.db $da, $d8, $d5, $d3, $d1, $ce, $cc, $c9
.db $c7, $c4, $c1, $bf, $bc, $b9, $b6, $b3
.db $b0, $ae, $ab, $a8, $a5, $a2, $9f, $9c
.db $98, $95, $92, $8f, $8c, $89, $86, $83
.db $7c, $7a, $77, $74, $71, $6e, $6b, $68
.db $65, $62, $5f, $5c, $59, $56, $53, $50
.db $4e, $4c, $49, $46, $43, $40, $3e, $3b
.db $38, $37, $34, $32, $2f, $2d, $2b, $28
.db $26, $24, $22, $20, $1e, $1c, $1a, $18
.db $16, $14, $13, $11, $10, $0e, $0e, $0c
.db $0b, $0a, $09, $08, $07, $06, $05, $05
.db $04, $03, $03, $02, $02, $02, $02, $02
.db $02, $03, $03, $03, $03, $03, $04, $04
.db $05, $06, $06, $07, $08, $09, $0a, $0b
.db $0c, $0d, $0f, $10, $12, $13, $15, $16
.db $18, $1a, $1c, $1d, $1f, $21, $23, $25
.db $27, $29, $2c, $2e, $30, $33, $35, $38
.db $3a, $3d, $40, $42, $45, $48, $4b, $4e
.db $51, $52, $55, $58, $5b, $5e, $61, $64
.db $68, $6b, $6e, $71, $74, $77, $7a, $7d
.db $80, $83, $86, $89, $8c, $8f, $92, $95
.db $98, $9c, $9f, $a2, $a5, $a8, $ab, $ae
.db $af, $b2, $b5, $b8, $bb, $be, $c0, $c3
.db $c6, $c8, $cb, $cd, $d0, $d2, $d4, $d7
.db $d9, $db, $dd, $df, $e1, $e3, $e4, $e6
.db $e8, $ea, $eb, $ed, $ee, $f0, $f1, $f3
.db $f4, $f5, $f6, $f7, $f8, $f9, $fa, $fa
.db $fb, $fc, $fc, $fd, $fd, $fd, $fd, $fd
.db $fe, $fe, $fe, $fe, $fe, $fe, $fd, $fd
.db $fc, $fb, $fb, $fa, $f9, $f8, $f7, $f6
.db $f5, $f4, $f2, $f2, $f0, $ef, $ed, $ec
.db $ea, $e8, $e6, $e4, $e2, $e0, $de, $dc
.db $da, $d8, $d5, $d3, $d1, $ce, $cc, $c9
.db $c8, $c5, $c2, $c0, $bd, $ba, $b7, $b4
.db $b2, $b0, $ad, $aa, $a7, $a4, $a1, $9e
.db $9b, $98, $95, $92, $8f, $8c, $89, $86
.db $7a, $78, $75, $72, $6f, $6c, $6a, $67
.db $64, $60, $5d, $5b, $58, $55, $52, $4f
.db $4d, $4b, $48, $45, $42, $3f, $3e, $3b
.db $38, $36, $33, $31, $2f, $2d, $2b, $28
.db $26, $24, $22, $20, $1e, $1c, $1a, $18 
.db $16, $14, $14, $12, $11, $0f, $0e, $0c
.db $0b, $0a, $09, $09, $08, $07, $06, $06
.db $05, $04, $04, $03, $03, $03, $03, $03
.db $03, $04, $04, $04, $04, $04, $05, $05
.db $06, $07, $07, $08, $09, $0a, $0b, $0c
.db $0d, $0e, $10, $11, $13, $14, $16, $17
.db $19, $1b, $1d, $1e, $20, $22, $24, $26
.db $28, $2a, $2d, $2f, $31, $34, $36, $39
.db $3b, $3e, $41, $42, $45, $48, $4b, $4e
.db $51, $53, $56, $59, $5c, $5f, $61, $64
.db $68, $6b, $6e, $71, $74, $77, $7a, $7d
.db $80, $83, $86, $89, $8c, $8f, $92, $95
.db $98, $9c, $9f, $a1, $a4, $a7, $aa, $ad
.db $af, $b2, $b5, $b8, $bb, $be, $bf, $c2
.db $c5, $c7, $ca, $cc, $cf, $d1, $d3, $d6
.db $d8, $da, $dc, $de, $e0, $e2, $e3, $e5
.db $e7, $e9, $ea, $ec, $ed, $ef, $f0, $f2
.db $f3, $f4, $f5, $f6, $f7, $f8, $f9, $f9
.db $fa, $fb, $fb, $fc, $fc, $fc, $fc, $fc
.db $fd, $fd, $fd, $fd, $fd, $fd, $fc, $fc
.db $fb, $fa, $fa, $f9, $f8, $f7, $f7, $f6
.db $f5, $f4, $f2, $f1, $ef, $ee, $ec, $ec
.db $ea, $e8, $e6, $e4, $e2, $e0, $de, $dc
.db $da, $d8, $d5, $d3, $d1, $cf, $cd, $ca
.db $c8, $c5, $c2, $c1, $be, $bb, $b8, $b5
.db $b3, $b1, $ae, $ab, $a8, $a5, $a3, $a0
.db $9c, $99, $96, $94, $91, $8e, $8b, $88
.db $78, $76, $73, $70, $6d, $6b, $68, $65
.db $62, $5f, $5c, $59, $57, $54, $51, $4e
.db $4c, $4a, $47, $44, $41, $3f, $3d, $3a
.db $37, $36, $33, $31, $2e, $2c, $2b, $28
.db $26, $24, $22, $20, $1e, $1c, $1a, $18
.db $17, $15, $14, $12, $11, $0f, $0f, $0d
.db $0c, $0b, $0a, $09, $08, $08, $07, $07
.db $06, $05, $05, $04, $04, $04, $04, $04
.db $04, $05, $05, $05, $05, $05, $06, $06
.db $07, $08, $08, $09, $0a, $0b, $0c, $0d
.db $0e, $0f, $11, $12, $14, $15, $17, $18
.db $1a, $1c, $1e, $1f, $21, $23, $25, $27
.db $29, $2b, $2e, $30, $32, $35, $37, $3a
.db $3b, $3e, $41, $43, $46, $49, $4c, $4f
.db $52, $53, $56, $59, $5c, $5f, $62, $65
.db $69, $6b, $6e, $71, $74, $77, $7a, $7d
.db $80, $83, $86, $89, $8c, $8f, $92, $95
.db $97, $9b, $9e, $a1, $a4, $a7, $aa, $ad
.db $ae, $b1, $b4, $b7, $ba, $bd, $bf, $c2
.db $c5, $c6, $c9, $cb, $ce, $d0, $d2, $d5
.db $d7, $d9, $db, $dd, $df, $e1, $e2, $e4
.db $e6, $e8, $e9, $eb, $ec, $ee, $ef, $f1
.db $f2, $f3, $f4, $f5, $f6, $f7, $f8, $f8
.db $f9, $fa, $fa, $fb, $fb, $fb, $fb, $fb
.db $fc, $fc, $fc, $fc, $fc, $fc, $fb, $fb
.db $fa, $f9, $f9, $f8, $f8, $f7, $f6, $f5
.db $f4, $f3, $f1, $f1, $ef, $ee, $ec, $eb
.db $e9, $e8, $e6, $e4, $e2, $e0, $de, $dc
.db $da, $d8, $d5, $d4, $d2, $cf, $cd, $ca
.db $c9, $c6, $c3, $c1, $bf, $bc, $b9, $b6
.db $b4, $b2, $af, $ac, $a9, $a7, $a4, $a1
.db $9e, $9b, $98, $95, $93, $90, $8d, $8a
.db $76, $74, $71, $6e, $6c, $69, $66, $64
.db $61, $5d, $5b, $58, $55, $53, $50, $4d
.db $4b, $49, $46, $43, $41, $3e, $3c, $3a
.db $37, $35, $32, $31, $2e, $2c, $2a, $28
.db $26, $24, $22, $20, $1e, $1c, $1a, $19
.db $17, $15, $14, $13, $12, $10, $0f, $0d
.db $0c, $0c, $0b, $0a, $09, $08, $07, $07
.db $07, $06, $06, $05, $05, $05, $05, $05
.db $05, $06, $06, $06, $06, $06, $07, $07
.db $08, $09, $09, $0a, $0b, $0c, $0d, $0e
.db $0f, $10, $12, $13, $15, $16, $18, $19
.db $1b, $1d, $1f, $20, $22, $24, $26, $28
.db $2a, $2c, $2f, $31, $33, $35, $37, $3a
.db $3c, $3f, $42, $44, $47, $49, $4c, $4f
.db $52, $54, $57, $5a, $5c, $5f, $62, $65
.db $69, $6c, $6e, $71, $74, $77, $7a, $7d
.db $80, $83, $86, $89, $8c, $8f, $92, $94
.db $97, $9b, $9e, $a1, $a4, $a6, $a9, $ac
.db $ae, $b1, $b4, $b7, $b9, $bc, $be, $c1
.db $c4, $c6, $c9, $cb, $cd, $cf, $d1, $d4
.db $d6, $d8, $da, $dc, $de, $e0, $e1, $e3
.db $e5, $e7, $e8, $ea, $eb, $ed, $ee, $f0
.db $f1, $f2, $f3, $f4, $f5, $f6, $f7, $f7
.db $f8, $f9, $f9, $fa, $fa, $fa, $fa, $fa
.db $fb, $fb, $fb, $fb, $fb, $fb, $fa, $fa
.db $f9, $f9, $f9, $f8, $f7, $f6, $f5, $f4
.db $f4, $f3, $f1, $f0, $ee, $ed, $ec, $eb
.db $e9, $e7, $e6, $e4, $e2, $e0, $de, $dc
.db $da, $d8, $d6, $d4, $d2, $cf, $ce, $cb
.db $c9, $c6, $c4, $c2, $bf, $bd, $ba, $b7
.db $b5, $b3, $b0, $ad, $ab, $a8, $a5, $a3
.db $9f, $9c, $9a, $97, $94, $92, $8f, $8c
.db $74, $72, $6f, $6d, $6a, $67, $65, $62
.db $5f, $5c, $59, $57, $54, $51, $4f, $4c
.db $4a, $48, $45, $43, $40, $3d, $3c, $39
.db $36, $35, $32, $30, $2e, $2c, $2a, $28
.db $26, $24, $22, $20, $1e, $1c, $1b, $19
.db $17, $15, $15, $13, $12, $10, $10, $0e
.db $0d, $0c, $0b, $0b, $0a, $09, $08, $08
.db $07, $07, $07, $06, $06, $06, $06, $06
.db $06, $07, $07, $07, $07, $07, $08, $08
.db $09, $0a, $0a, $0b, $0c, $0d, $0e, $0f
.db $10, $11, $13, $14, $16, $17, $19, $1a
.db $1c, $1e, $20, $21, $23, $25, $27, $29
.db $2b, $2d, $30, $32, $33, $36, $38, $3b
.db $3d, $40, $43, $44, $47, $4a, $4d, $50
.db $53, $54, $57, $5a, $5d, $60, $62, $65
.db $69, $6c, $6f, $71, $74, $77, $7a, $7d
.db $80, $83, $86, $89, $8c, $8f, $91, $94
.db $97, $9b, $9e, $a0, $a3, $a6, $a9, $ac
.db $ad, $b0, $b3, $b6, $b9, $bc, $bd, $c0
.db $c3, $c5, $c8, $ca, $cd, $ce, $d0, $d3
.db $d5, $d7, $d9, $db, $dd, $df, $e0, $e2
.db $e4, $e6, $e7, $e9, $ea, $ec, $ed, $ef
.db $f0, $f1, $f2, $f3, $f4, $f5, $f6, $f6
.db $f7, $f8, $f8, $f9, $f9, $f9, $f9, $f9
.db $fa, $fa, $fa, $fa, $fa, $fa, $f9, $f9
.db $f9, $f8, $f8, $f7, $f6, $f5, $f5, $f4
.db $f3, $f2, $f0, $f0, $ee, $ed, $eb, $eb
.db $e9, $e7, $e5, $e4, $e2, $e0, $de, $dc
.db $da, $d8, $d6, $d4, $d2, $d0, $ce, $cb
.db $ca, $c7, $c4, $c3, $c0, $bd, $bb, $b8
.db $b6, $b4, $b1, $af, $ac, $a9, $a7, $a4
.db $a1, $9e, $9b, $99, $96, $93, $91, $8e
.db $72, $70, $6d, $6b, $68, $66, $63, $61
.db $5e, $5b, $58, $55, $53, $50, $4e, $4b
.db $49, $47, $44, $42, $3f, $3d, $3b, $38
.db $36, $34, $32, $30, $2d, $2c, $2a, $28
.db $26, $24, $22, $20, $1e, $1c, $1b, $19
.db $17, $16, $15, $13, $13, $11, $10, $0e
.db $0e, $0d, $0c, $0b, $0b, $0a, $09, $09
.db $08, $08, $08, $07, $07, $07, $07, $07
.db $07, $08, $08, $08, $08, $08, $09, $09
.db $0a, $0b, $0b, $0c, $0d, $0e, $0f, $10
.db $11, $12, $14, $15, $17, $18, $1a, $1b
.db $1d, $1f, $21, $22, $24, $26, $28, $2a
.db $2c, $2e, $31, $32, $34, $37, $39, $3c
.db $3e, $40, $43, $45, $48, $4b, $4d, $50
.db $53, $55, $58, $5a, $5d, $60, $63, $66
.db $69, $6c, $6f, $72, $74, $77, $7a, $7d
.db $80, $83, $86, $89, $8c, $8e, $91, $94
.db $97, $9a, $9d, $a0, $a3, $a6, $a8, $ab
.db $ad, $b0, $b3, $b5, $b8, $bb, $bd, $c0
.db $c2, $c4, $c7, $c9, $cc, $ce, $cf, $d2
.db $d4, $d6, $d8, $da, $dc, $de, $df, $e1
.db $e3, $e5, $e6, $e8, $e9, $eb, $ec, $ee
.db $ef, $f0, $f1, $f2, $f3, $f4, $f5, $f5
.db $f6, $f7, $f7, $f8, $f8, $f8, $f8, $f8
.db $f9, $f9, $f9, $f9, $f9, $f9, $f8, $f8
.db $f8, $f7, $f7, $f6, $f5, $f5, $f4, $f3
.db $f2, $f2, $f0, $ef, $ed, $ed, $eb, $ea
.db $e9, $e7, $e5, $e4, $e2, $e0, $de, $dc
.db $da, $d8, $d6, $d4, $d3, $d0, $ce, $cc
.db $ca, $c8, $c5, $c3, $c1, $be, $bc, $b9
.db $b7, $b5, $b2, $b0, $ad, $ab, $a8, $a5
.db $a2, $9f, $9d, $9a, $98, $95, $93, $90
.db $70, $6e, $6b, $69, $66, $64, $61, $5f
.db $5c, $59, $57, $54, $52, $4f, $4d, $4a
.db $48, $46, $43, $41, $3e, $3c, $3a, $38
.db $35, $34, $31, $30, $2d, $2b, $2a, $27
.db $26, $24, $22, $20, $1e, $1d, $1b, $19
.db $18, $16, $15, $14, $13, $11, $11, $0f
.db $0e, $0e, $0d, $0c, $0b, $0b, $0a, $0a
.db $09, $08, $09, $08, $08, $08, $08, $08
.db $08, $09, $09, $09, $09, $09, $0a, $0a
.db $0b, $0c, $0c, $0d, $0e, $0f, $10, $11
.db $12, $13, $15, $16, $18, $19, $1b, $1c
.db $1e, $20, $22, $23, $25, $27, $29, $2b
.db $2d, $2f, $31, $33, $35, $38, $3a, $3d
.db $3e, $41, $44, $46, $49, $4b, $4e, $51
.db $54, $55, $58, $5b, $5e, $60, $63, $66
.db $6a, $6c, $6f, $72, $75, $77, $7a, $7d
.db $80, $83, $86, $89, $8b, $8e, $91, $94
.db $96, $9a, $9d, $a0, $a2, $a5, $a8, $ab
.db $ac, $af, $b2, $b5, $b7, $ba, $bc, $bf
.db $c2, $c3, $c6, $c8, $cb, $cd, $cf, $d1
.db $d3, $d5, $d7, $d9, $db, $dd, $de, $e0
.db $e2, $e4, $e5, $e7, $e8, $ea, $eb, $ed
.db $ee, $ef, $f0, $f1, $f2, $f3, $f4, $f4
.db $f5, $f6, $f6, $f7, $f7, $f7, $f7, $f7
.db $f8, $f8, $f8, $f8, $f8, $f8, $f7, $f8
.db $f7, $f6, $f6, $f5, $f5, $f4, $f3, $f2
.db $f2, $f1, $ef, $ef, $ed, $ec, $eb, $ea
.db $e8, $e7, $e5, $e3, $e2, $e0, $de, $dc
.db $da, $d9, $d6, $d5, $d3, $d0, $cf, $cc
.db $cb, $c8, $c6, $c4, $c2, $bf, $bd, $ba
.db $b8, $b6, $b3, $b1, $ae, $ac, $a9, $a7
.db $a4, $a1, $9f, $9c, $9a, $97, $95, $92
.db $6e, $6c, $6a, $67, $65, $62, $60, $5d
.db $5b, $58, $55, $53, $50, $4e, $4c, $49
.db $47, $45, $43, $40, $3e, $3b, $3a, $37
.db $35, $33, $31, $2f, $2d, $2b, $2a, $27
.db $26, $24, $22, $20, $1e, $1d, $1b, $1a
.db $18, $16, $16, $14, $13, $12, $11, $10
.db $0f, $0e, $0d, $0d, $0c, $0b, $0b, $0b
.db $0a, $09, $09, $09, $09, $09, $09, $09
.db $09, $0a, $0a, $0a, $0a, $0a, $0b, $0b
.db $0c, $0d, $0d, $0e, $0f, $10, $11, $12
.db $13, $14, $16, $17, $19, $1a, $1c, $1d
.db $1f, $21, $23, $24, $26, $28, $2a, $2c
.db $2e, $30, $32, $34, $36, $39, $3b, $3d
.db $3f, $42, $45, $46, $49, $4c, $4f, $51
.db $54, $56, $59, $5b, $5e, $61, $63, $66
.db $6a, $6c, $6f, $72, $75, $77, $7a, $7d
.db $80, $83, $86, $89, $8b, $8e, $91, $94
.db $96, $9a, $9d, $9f, $a2, $a5, $a7, $aa
.db $ac, $af, $b1, $b4, $b7, $ba, $bb, $be
.db $c1, $c3, $c5, $c7, $ca, $cc, $ce, $d0
.db $d2, $d4, $d6, $d8, $da, $dc, $dd, $df
.db $e1, $e3, $e4, $e6, $e7, $e9, $ea, $ec
.db $ed, $ee, $ef, $f0, $f1, $f2, $f3, $f3
.db $f4, $f5, $f5, $f6, $f6, $f6, $f6, $f6
.db $f7, $f7, $f7, $f7, $f7, $f7, $f7, $f7
.db $f6, $f5, $f5, $f5, $f4, $f3, $f3, $f2
.db $f1, $f0, $ef, $ee, $ed, $ec, $ea, $ea
.db $e8, $e6, $e5, $e3, $e2, $e0, $de, $dc
.db $da, $d9, $d6, $d5, $d3, $d1, $cf, $cd
.db $cb, $c9, $c6, $c5, $c2, $c0, $bd, $bb
.db $b9, $b7, $b4, $b2, $b0, $ad, $ab, $a8
.db $a5, $a3, $a0, $9e, $9b, $99, $96, $94
.db $6c, $6a, $68, $65, $63, $61, $5e, $5c
.db $59, $56, $54, $52, $4f, $4d, $4a, $48
.db $46, $44, $42, $3f, $3d, $3b, $39, $37
.db $34, $33, $30, $2f, $2d, $2b, $29, $27
.db $26, $24, $22, $20, $1e, $1d, $1b, $1a
.db $18, $17, $16, $15, $14, $12, $12, $10
.db $0f, $0f, $0e, $0d, $0d, $0c, $0b, $0b
.db $0b, $0a, $0a, $0a, $0a, $0a, $0a, $0a
.db $0a, $0b, $0b, $0b, $0b, $0b, $0c, $0c
.db $0d, $0e, $0e, $0f, $10, $11, $12, $13
.db $14, $15, $17, $18, $1a, $1b, $1d, $1e
.db $20, $22, $24, $25, $27, $29, $2b, $2d
.db $2f, $30, $33, $35, $37, $39, $3b, $3e
.db $40, $43, $45, $47, $4a, $4c, $4f, $52
.db $55, $56, $59, $5c, $5e, $61, $64, $66
.db $6a, $6d, $6f, $72, $75, $77, $7a, $7d
.db $80, $83, $86, $89, $8b, $8e, $91, $93
.db $96, $9a, $9c, $9f, $a2, $a4, $a7, $aa
.db $ab, $ae, $b1, $b4, $b6, $b9, $bb, $bd
.db $c0, $c2, $c5, $c7, $c9, $cb, $cd, $d0
.db $d1, $d3, $d5, $d7, $d9, $db, $dc, $de
.db $e0, $e2, $e3, $e5, $e6, $e8, $e9, $eb
.db $ec, $ed, $ee, $ef, $f0, $f1, $f2, $f2
.db $f3, $f4, $f4, $f5, $f5, $f5, $f5, $f5
.db $f6, $f6, $f6, $f6, $f6, $f6, $f6, $f6
.db $f5, $f5, $f5, $f4, $f3, $f3, $f2, $f1
.db $f1, $f0, $ee, $ee, $ec, $eb, $ea, $e9
.db $e8, $e6, $e5, $e3, $e2, $e0, $de, $dc
.db $da, $d9, $d7, $d5, $d3, $d1, $d0, $cd
.db $cc, $c9, $c7, $c5, $c3, $c1, $be, $bc
.db $ba, $b8, $b6, $b3, $b1, $ae, $ac, $aa
.db $a7, $a4, $a2, $9f, $9d, $9b, $98, $96
.db $6a, $68, $66, $64, $61, $5f, $5d, $5a
.db $58, $55, $53, $50, $4e, $4c, $49, $47
.db $45, $43, $41, $3f, $3c, $3a, $38, $36
.db $34, $32, $30, $2f, $2c, $2b, $29, $27
.db $25, $24, $22, $20, $1f, $1d, $1c, $1a
.db $19, $17, $16, $15, $14, $13, $12, $11
.db $10, $0f, $0f, $0e, $0d, $0d, $0c, $0c
.db $0c, $0b, $0b, $0a, $0b, $0b, $0b, $0b
.db $0b, $0c, $0c, $0c, $0c, $0c, $0d, $0d
.db $0e, $0f, $10, $11, $12, $13, $14, $15
.db $16, $17, $18, $19, $1b, $1c, $1e, $1f
.db $21, $23, $25, $26, $28, $2a, $2c, $2e
.db $2f, $31, $34, $36, $38, $3a, $3c, $3f
.db $41, $43, $46, $48, $4a, $4d, $50, $52
.db $55, $57, $59, $5c, $5f, $61, $64, $67
.db $6a, $6d, $70, $72, $75, $78, $7a, $7d
.db $80, $83, $86, $88, $8b, $8e, $90, $93
.db $96, $99, $9c, $9f, $a1, $a4, $a7, $a9
.db $ab, $ae, $b0, $b3, $b6, $b8, $ba, $bd
.db $bf, $c1, $c4, $c6, $c8, $ca, $cc, $cf
.db $d1, $d2, $d4, $d6, $d8, $da, $db, $dd
.db $df, $e1, $e2, $e4, $e5, $e7, $e8, $e9
.db $ea, $eb, $ec, $ed, $ee, $ef, $f0, $f1
.db $f2, $f3, $f3, $f4, $f4, $f4, $f4, $f4
.db $f5, $f5, $f5, $f5, $f5, $f6, $f5, $f5
.db $f4, $f4, $f4, $f3, $f3, $f2, $f1, $f1
.db $f0, $ef, $ee, $ed, $ec, $eb, $ea, $e9
.db $e7, $e6, $e4, $e3, $e1, $e0, $de, $dc
.db $db, $d9, $d7, $d5, $d4, $d1, $d0, $ce
.db $cc, $ca, $c8, $c6, $c4, $c1, $bf, $bd
.db $bb, $b9, $b7, $b4, $b2, $b0, $ad, $ab
.db $a8, $a6, $a3, $a1, $9f, $9c, $9a, $98
.db $68, $66, $64, $62, $5f, $5d, $5b, $59
.db $56, $53, $51, $4f, $4d, $4a, $48, $46
.db $44, $42, $40, $3e, $3b, $39, $38, $36
.db $33, $32, $30, $2e, $2c, $2a, $29, $27
.db $25, $24, $22, $20, $1f, $1d, $1c, $1a
.db $19, $17, $17, $15, $15, $13, $13, $11
.db $11, $10, $0f, $0f, $0e, $0e, $0d, $0d
.db $0c, $0c, $0c, $0b, $0c, $0c, $0c, $0c
.db $0c, $0d, $0d, $0d, $0d, $0d, $0e, $0e
.db $0f, $10, $11, $12, $13, $14, $15, $16
.db $17, $18, $19, $1a, $1c, $1d, $1f, $20
.db $22, $24, $26, $27, $29, $2b, $2d, $2f
.db $30, $32, $35, $37, $38, $3b, $3d, $40
.db $41, $44, $47, $48, $4b, $4e, $50, $53
.db $56, $57, $5a, $5d, $5f, $62, $64, $67
.db $6b, $6d, $70, $72, $75, $78, $7a, $7d
.db $80, $83, $86, $88, $8b, $8e, $90, $93
.db $95, $99, $9c, $9e, $a1, $a3, $a6, $a9
.db $aa, $ad, $b0, $b2, $b5, $b8, $b9, $bc
.db $bf, $c0, $c3, $c5, $c8, $c9, $cb, $ce
.db $d0, $d1, $d3, $d5, $d7, $d9, $da, $dc
.db $de, $e0, $e1, $e3, $e4, $e6, $e7, $e8
.db $e9, $ea, $eb, $ec, $ed, $ee, $ef, $f0
.db $f1, $f2, $f2, $f3, $f3, $f3, $f3, $f3
.db $f4, $f4, $f4, $f4, $f4, $f5, $f4, $f4
.db $f4, $f3, $f3, $f2, $f2, $f1, $f1, $f0
.db $ef, $ef, $ed, $ed, $eb, $eb, $e9, $e9
.db $e7, $e6, $e4, $e3, $e1, $e0, $de, $dc
.db $db, $d9, $d7, $d6, $d4, $d2, $d0, $ce
.db $cd, $ca, $c8, $c7, $c5, $c2, $c0, $be
.db $bc, $ba, $b8, $b6, $b3, $b1, $af, $ad
.db $aa, $a7, $a5, $a3, $a1, $9e, $9c, $9a
.db $66, $64, $62, $60, $5e, $5c, $59, $57
.db $55, $52, $50, $4e, $4b, $49, $47, $45
.db $43, $41, $3f, $3d, $3b, $39, $37, $35
.db $33, $31, $2f, $2e, $2c, $2a, $29, $27
.db $25, $24, $22, $20, $1f, $1d, $1c, $1b
.db $19, $18, $17, $16, $15, $14, $13, $12
.db $11, $11, $10, $0f, $0f, $0e, $0e, $0e
.db $0d, $0d, $0d, $0c, $0c, $0d, $0d, $0d
.db $0d, $0e, $0e, $0e, $0e, $0e, $0f, $10
.db $11, $12, $12, $13, $14, $15, $16, $17
.db $18, $19, $1b, $1c, $1d, $1e, $20, $21
.db $23, $25, $27, $28, $2a, $2c, $2e, $2f
.db $31, $33, $36, $37, $39, $3c, $3e, $40
.db $42, $45, $47, $49, $4c, $4e, $51, $53
.db $56, $58, $5a, $5d, $60, $62, $65, $67
.db $6b, $6d, $70, $73, $75, $78, $7a, $7d
.db $80, $83, $86, $88, $8b, $8d, $90, $93
.db $95, $99, $9b, $9e, $a0, $a3, $a6, $a8
.db $aa, $ad, $af, $b2, $b4, $b7, $b9, $bb
.db $be, $c0, $c2, $c4, $c7, $c9, $ca, $cd
.db $cf, $d1, $d2, $d4, $d6, $d8, $d9, $db
.db $dd, $df, $e0, $e2, $e3, $e4, $e5, $e7
.db $e8, $e9, $ea, $eb, $ec, $ed, $ee, $ee
.db $ef, $f0, $f1, $f2, $f2, $f2, $f2, $f2
.db $f3, $f3, $f3, $f3, $f4, $f4, $f3, $f3
.db $f3, $f2, $f2, $f2, $f1, $f1, $f0, $ef
.db $ef, $ee, $ed, $ec, $eb, $ea, $e9, $e8
.db $e7, $e5, $e4, $e3, $e1, $e0, $de, $dc
.db $db, $d9, $d7, $d6, $d4, $d2, $d1, $cf
.db $cd, $cb, $c9, $c7, $c5, $c3, $c1, $bf
.db $bd, $bb, $b9, $b7, $b5, $b2, $b0, $ae
.db $ab, $a9, $a7, $a4, $a2, $a0, $9e, $9c
.db $64, $62, $60, $5e, $5c, $5a, $58, $56
.db $53, $51, $4e, $4c, $4a, $48, $46, $44
.db $42, $40, $3e, $3c, $3a, $38, $37, $34
.db $32, $31, $2f, $2d, $2b, $2a, $29, $27
.db $25, $24, $22, $20, $1f, $1d, $1c, $1b
.db $19, $18, $18, $16, $16, $14, $14, $12
.db $12, $11, $11, $10, $10, $0f, $0e, $0f
.db $0e, $0e, $0e, $0d, $0d, $0e, $0e, $0e
.db $0e, $0f, $0f, $0f, $0f, $0f, $10, $11
.db $12, $13, $13, $14, $15, $16, $17, $18
.db $19, $1a, $1c, $1d, $1e, $1f, $21, $22
.db $24, $26, $28, $29, $2b, $2d, $2f, $30
.db $32, $34, $37, $38, $3a, $3d, $3e, $41
.db $43, $45, $48, $4a, $4c, $4f, $51, $54
.db $57, $58, $5b, $5d, $60, $63, $65, $68
.db $6b, $6e, $70, $73, $75, $78, $7a, $7d
.db $80, $83, $86, $88, $8b, $8d, $90, $92
.db $95, $98, $9b, $9d, $a0, $a3, $a5, $a8
.db $a9, $ac, $af, $b1, $b4, $b6, $b8, $bb
.db $bd, $bf, $c2, $c3, $c6, $c8, $c9, $cc
.db $ce, $d0, $d1, $d3, $d5, $d7, $d8, $da
.db $dc, $de, $df, $e1, $e2, $e3, $e4, $e6
.db $e7, $e8, $e9, $ea, $eb, $ec, $ed, $ed
.db $ee, $ef, $f0, $f1, $f1, $f1, $f1, $f1
.db $f2, $f2, $f2, $f2, $f3, $f3, $f2, $f2
.db $f2, $f1, $f2, $f1, $f0, $f0, $ef, $ef
.db $ee, $ee, $ec, $ec, $ea, $ea, $e8, $e8
.db $e7, $e5, $e4, $e3, $e1, $e0, $de, $dc
.db $db, $d9, $d7, $d6, $d5, $d3, $d1, $cf
.db $ce, $cc, $c9, $c8, $c6, $c4, $c2, $c0
.db $be, $bc, $ba, $b8, $b6, $b4, $b2, $af
.db $ad, $aa, $a8, $a6, $a4, $a2, $a0, $9e
.db $62, $60, $5e, $5c, $5a, $58, $56, $54
.db $52, $4f, $4d, $4b, $49, $47, $45, $43
.db $41, $3f, $3d, $3b, $39, $37, $36, $34
.db $32, $30, $2e, $2d, $2b, $2a, $28, $26
.db $25, $24, $22, $20, $1f, $1e, $1c, $1b
.db $1a, $18, $18, $17, $16, $15, $14, $13
.db $12, $12, $11, $11, $10, $10, $0f, $0f
.db $0f, $0e, $0f, $0e, $0e, $0f, $0f, $0f
.db $0f, $10, $10, $10, $10, $10, $11, $12
.db $13, $14, $14, $15, $16, $17, $18, $19
.db $1a, $1b, $1d, $1e, $1f, $20, $22, $23
.db $25, $27, $29, $2a, $2c, $2e, $30, $31
.db $33, $35, $37, $39, $3b, $3d, $3f, $42
.db $44, $46, $49, $4a, $4d, $4f, $52, $54
.db $57, $59, $5b, $5e, $60, $63, $65, $68
.db $6b, $6e, $70, $73, $75, $78, $7a, $7d
.db $80, $83, $86, $88, $8b, $8d, $90, $92
.db $95, $98, $9b, $9d, $a0, $a2, $a5, $a7
.db $a9, $ac, $ae, $b1, $b3, $b6, $b7, $ba
.db $bc, $be, $c1, $c3, $c5, $c7, $c9, $cb
.db $cd, $cf, $d0, $d2, $d4, $d6, $d7, $d9
.db $db, $dd, $de, $e0, $e1, $e2, $e3, $e5
.db $e6, $e7, $e8, $e9, $ea, $eb, $ec, $ec
.db $ed, $ee, $ef, $f0, $f0, $f0, $f0, $f0
.db $f1, $f1, $f1, $f1, $f2, $f2, $f1, $f2
.db $f1, $f1, $f1, $f0, $f0, $ef, $ef, $ee
.db $ee, $ed, $ec, $eb, $ea, $e9, $e8, $e8
.db $e6, $e5, $e4, $e2, $e1, $e0, $de, $dc
.db $db, $da, $d8, $d6, $d5, $d3, $d2, $d0
.db $ce, $cc, $ca, $c9, $c7, $c5, $c3, $c1
.db $bf, $bd, $bb, $b9, $b7, $b5, $b3, $b1
.db $ae, $ac, $aa, $a8, $a6, $a4, $a2, $a0
.db $60, $5e, $5c, $5a, $58, $56, $54, $52
.db $50, $4e, $4c, $4a, $48, $46, $44, $42
.db $40, $3e, $3c, $3a, $38, $36, $35, $33
.db $31, $30, $2e, $2d, $2b, $29, $28, $26
.db $25, $24, $22, $20, $1f, $1e, $1d, $1b
.db $1a, $19, $18, $17, $17, $15, $15, $14
.db $13, $13, $12, $12, $11, $11, $10, $10
.db $10, $0f, $10, $0f, $0f, $10, $10, $10
.db $10, $11, $11, $11, $11, $12, $13, $13
.db $14, $15, $15, $16, $17, $18, $19, $1a
.db $1b, $1c, $1e, $1f, $21, $22, $23, $24
.db $26, $28, $2a, $2b, $2d, $2f, $31, $32
.db $34, $36, $38, $3a, $3c, $3e, $40, $43
.db $44, $47, $49, $4b, $4e, $50, $53, $55
.db $58, $59, $5c, $5e, $61, $63, $66, $68
.db $6c, $6e, $71, $73, $76, $78, $7b, $7d
.db $80, $83, $85, $88, $8a, $8d, $8f, $92
.db $94, $98, $9a, $9d, $9f, $a2, $a4, $a7
.db $a8, $ab, $ad, $b0, $b2, $b5, $b7, $b9
.db $bc, $bd, $c0, $c2, $c4, $c6, $c8, $ca
.db $cc, $ce, $cf, $d1, $d3, $d5, $d6, $d8
.db $da, $dc, $dd, $de, $df, $e1, $e2, $e4
.db $e5, $e6, $e7, $e8, $e9, $ea, $eb, $eb
.db $ec, $ed, $ed, $ee, $ef, $ef, $ef, $ef
.db $f0, $f0, $f0, $f0, $f1, $f1, $f0, $f1
.db $f0, $f0, $f0, $ef, $ef, $ee, $ee, $ed
.db $ed, $ec, $eb, $eb, $e9, $e9, $e8, $e7
.db $e6, $e5, $e3, $e2, $e1, $e0, $de, $dc
.db $db, $da, $d8, $d7, $d5, $d3, $d2, $d0
.db $cf, $cd, $cb, $ca, $c8, $c6, $c4, $c2
.db $c0, $be, $bc, $ba, $b8, $b6, $b4, $b2
.db $b0, $ae, $ac, $aa, $a8, $a6, $a4, $a2
.db $5e, $5d, $5b, $59, $57, $55, $53, $51
.db $4f, $4c, $4a, $48, $47, $45, $43, $41
.db $3f, $3e, $3c, $3a, $38, $36, $35, $33
.db $31, $30, $2e, $2c, $2a, $29, $28, $26
.db $25, $24, $22, $20, $1f, $1e, $1d, $1b
.db $1a, $19, $19, $17, $17, $16, $15, $14
.db $14, $13, $13, $12, $12, $11, $11, $11
.db $11, $10, $10, $10, $10, $10, $11, $11
.db $11, $12, $12, $12, $12, $13, $14, $14
.db $15, $16, $16, $17, $18, $19, $1a, $1b
.db $1c, $1d, $1f, $20, $22, $23, $24, $25
.db $27, $29, $2b, $2c, $2e, $30, $31, $33
.db $35, $37, $39, $3b, $3d, $3f, $41, $43
.db $45, $47, $4a, $4c, $4e, $51, $53, $56
.db $58, $5a, $5c, $5f, $61, $64, $66, $69
.db $6c, $6e, $71, $73, $76, $78, $7b, $7d
.db $80, $83, $85, $88, $8a, $8d, $8f, $92
.db $94, $97, $9a, $9c, $9f, $a1, $a4, $a6
.db $a8, $aa, $ad, $af, $b2, $b4, $b6, $b9
.db $bb, $bd, $bf, $c1, $c3, $c5, $c7, $c9
.db $cb, $cd, $cf, $d0, $d2, $d4, $d5, $d7
.db $d9, $db, $dc, $dd, $de, $e0, $e1, $e3
.db $e4, $e5, $e6, $e7, $e8, $e9, $ea, $ea
.db $eb, $ec, $ec, $ed, $ee, $ee, $ee, $ee
.db $ef, $ef, $ef, $f0, $f0, $f0, $f0, $f0
.db $ef, $ef, $ef, $ef, $ee, $ee, $ed, $ed
.db $ec, $ec, $eb, $ea, $e9, $e9, $e7, $e7
.db $e6, $e5, $e3, $e2, $e1, $e0, $de, $dc
.db $db, $da, $d8, $d7, $d6, $d4, $d2, $d0
.db $cf, $cd, $cb, $ca, $c8, $c6, $c4, $c2
.db $c1, $bf, $bd, $bb, $b9, $b8, $b6, $b4
.db $b1, $af, $ad, $ab, $a9, $a7, $a5, $a3
.db $5c, $5b, $59, $57, $55, $53, $51, $4f
.db $4d, $4b, $49, $47, $45, $43, $42, $40
.db $3e, $3d, $3b, $39, $37, $35, $34, $32
.db $30, $2f, $2d, $2c, $2a, $29, $28, $26
.db $25, $24, $22, $20, $1f, $1e, $1d, $1c
.db $1b, $19, $19, $18, $17, $16, $16, $15
.db $14, $14, $13, $13, $12, $12, $12, $12
.db $11, $11, $11, $11, $11, $11, $12, $12
.db $12, $13, $13, $13, $13, $14, $15, $15
.db $16, $17, $17, $18, $19, $1a, $1b, $1c
.db $1d, $1e, $20, $21, $23, $24, $25, $26
.db $28, $2a, $2c, $2d, $2f, $31, $32, $34
.db $36, $38, $3a, $3c, $3d, $40, $42, $44
.db $46, $48, $4b, $4c, $4f, $51, $54, $56
.db $59, $5a, $5d, $5f, $62, $64, $66, $69
.db $6c, $6e, $71, $73, $76, $78, $7b, $7d
.db $80, $83, $85, $88, $8a, $8d, $8f, $92
.db $94, $97, $9a, $9c, $9e, $a1, $a3, $a6
.db $a7, $aa, $ac, $af, $b1, $b4, $b5, $b8
.db $ba, $bc, $be, $c0, $c3, $c4, $c6, $c8
.db $ca, $cc, $ce, $cf, $d1, $d3, $d4, $d6
.db $d8, $da, $db, $dc, $dd, $df, $e0, $e2
.db $e3, $e4, $e5, $e6, $e7, $e8, $e9, $e9
.db $ea, $eb, $eb, $ec, $ed, $ed, $ed, $ed
.db $ee, $ee, $ee, $ef, $ef, $ef, $ef, $ef
.db $ef, $ee, $ee, $ee, $ee, $ed, $ed, $ec
.db $ec, $eb, $ea, $ea, $e9, $e8, $e7, $e7
.db $e5, $e4, $e3, $e2, $e1, $e0, $de, $dc
.db $db, $da, $d8, $d7, $d6, $d4, $d3, $d1
.db $d0, $ce, $cc, $cb, $c9, $c7, $c5, $c3
.db $c2, $c0, $be, $bd, $bb, $b9, $b7, $b5
.db $b3, $b1, $af, $ad, $ab, $a9, $a7, $a5
.db $5a, $59, $57, $55, $53, $51, $50, $4e
.db $4c, $49, $48, $46, $44, $42, $40, $3f
.db $3d, $3c, $3a, $38, $36, $34, $33, $32
.db $30, $2f, $2d, $2c, $2a, $29, $28, $26
.db $25, $24, $22, $20, $1f, $1e, $1d, $1c
.db $1b, $1a, $19, $18, $18, $17, $16, $15
.db $15, $14, $14, $14, $13, $13, $12, $13
.db $12, $12, $12, $12, $12, $12, $13, $13
.db $13, $14, $14, $14, $14, $15, $16, $16
.db $17, $18, $18, $19, $1a, $1b, $1c, $1d
.db $1e, $1f, $21, $22, $24, $25, $26, $27
.db $29, $2b, $2d, $2e, $30, $32, $33, $35
.db $37, $38, $3b, $3d, $3e, $41, $42, $45
.db $46, $49, $4b, $4d, $4f, $52, $54, $57
.db $59, $5b, $5d, $60, $62, $64, $67, $69
.db $6c, $6f, $71, $73, $76, $78, $7b, $7d
.db $80, $83, $85, $88, $8a, $8d, $8f, $91
.db $94, $97, $99, $9c, $9e, $a0, $a3, $a5
.db $a7, $a9, $ac, $ae, $b1, $b3, $b5, $b7
.db $ba, $bb, $be, $bf, $c2, $c3, $c5, $c8
.db $c9, $cb, $cd, $ce, $d0, $d2, $d3, $d5
.db $d7, $d9, $da, $db, $dc, $de, $df, $e1
.db $e2, $e3, $e4, $e5, $e6, $e7, $e8, $e8
.db $e9, $ea, $ea, $eb, $ec, $ec, $ec, $ec
.db $ed, $ed, $ed, $ee, $ee, $ee, $ee, $ee
.db $ee, $ed, $ee, $ed, $ed, $ec, $ec, $ec
.db $eb, $eb, $ea, $e9, $e8, $e8, $e7, $e6
.db $e5, $e4, $e3, $e2, $e1, $e0, $de, $dc
.db $db, $da, $d8, $d7, $d6, $d4, $d3, $d1
.db $d0, $ce, $cd, $cc, $ca, $c8, $c6, $c4
.db $c3, $c1, $c0, $be, $bc, $ba, $b8, $b7
.db $b4, $b2, $b0, $af, $ad, $ab, $a9, $a7
.db $58, $57, $55, $53, $51, $50, $4e, $4c
.db $4a, $48, $46, $45, $43, $41, $3f, $3e
.db $3c, $3b, $39, $37, $35, $34, $33, $31
.db $2f, $2e, $2c, $2b, $2a, $28, $27, $26
.db $25, $24, $22, $20, $1f, $1e, $1d, $1c
.db $1b, $1a, $1a, $19, $18, $17, $17, $16
.db $15, $15, $15, $14, $14, $14, $13, $13
.db $13, $13, $13, $13, $13, $13, $14, $14
.db $14, $15, $15, $15, $15, $16, $17, $17
.db $18, $19, $19, $1a, $1b, $1c, $1d, $1e
.db $1f, $20, $22, $23, $25, $26, $27, $28
.db $2a, $2c, $2e, $2f, $31, $33, $34, $36
.db $38, $39, $3c, $3d, $3f, $41, $43, $46
.db $47, $4a, $4c, $4e, $50, $52, $55, $57
.db $5a, $5b, $5e, $60, $62, $65, $67, $69
.db $6d, $6f, $71, $74, $76, $78, $7b, $7d
.db $80, $83, $85, $88, $8a, $8c, $8f, $91
.db $93, $97, $99, $9b, $9e, $a0, $a2, $a5
.db $a6, $a9, $ab, $ae, $b0, $b2, $b4, $b6
.db $b9, $ba, $bd, $bf, $c1, $c3, $c4, $c7
.db $c8, $ca, $cc, $cd, $cf, $d1, $d2, $d4
.db $d6, $d8, $d9, $da, $db, $dd, $de, $e0
.db $e1, $e2, $e3, $e4, $e5, $e6, $e7, $e7
.db $e8, $e9, $e9, $ea, $eb, $eb, $eb, $eb
.db $ec, $ec, $ec, $ed, $ed, $ed, $ed, $ed
.db $ed, $ed, $ed, $ec, $ec, $ec, $eb, $eb
.db $eb, $ea, $e9, $e9, $e8, $e7, $e6, $e6
.db $e5, $e4, $e3, $e2, $e1, $e0, $de, $dc
.db $db, $da, $d9, $d8, $d6, $d5, $d4, $d2
.db $d1, $cf, $cd, $cc, $cb, $c9, $c7, $c5
.db $c4, $c2, $c1, $bf, $bd, $bb, $ba, $b8
.db $b6, $b4, $b2, $b0, $af, $ad, $ab, $a9
.db $56, $55, $53, $51, $50, $4e, $4c, $4b
.db $49, $47, $45, $43, $42, $40, $3e, $3d
.db $3b, $3a, $38, $36, $35, $33, $32, $30
.db $2f, $2e, $2c, $2b, $29, $28, $27, $26
.db $25, $24, $22, $20, $1f, $1e, $1d, $1c
.db $1b, $1a, $1a, $19, $19, $18, $17, $16
.db $16, $16, $15, $15, $15, $14, $14, $14
.db $14, $14, $14, $14, $14, $14, $15, $15
.db $15, $16, $16, $16, $16, $17, $18, $18
.db $19, $1a, $1a, $1b, $1c, $1d, $1e, $1f
.db $20, $21, $23, $24, $26, $27, $28, $29
.db $2b, $2d, $2f, $30, $32, $34, $35, $37
.db $39, $3a, $3d, $3e, $40, $42, $44, $46
.db $48, $4a, $4d, $4e, $51, $53, $55, $58
.db $5a, $5c, $5e, $60, $63, $65, $67, $6a
.db $6d, $6f, $71, $74, $76, $78, $7b, $7d
.db $80, $83, $85, $88, $8a, $8c, $8f, $91
.db $93, $96, $99, $9b, $9d, $a0, $a2, $a4
.db $a6, $a8, $ab, $ad, $af, $b2, $b3, $b6
.db $b8, $ba, $bc, $be, $c0, $c2, $c3, $c6
.db $c7, $c9, $cb, $cc, $ce, $d0, $d1, $d3
.db $d5, $d7, $d8, $d9, $da, $dc, $dd, $df
.db $e0, $e1, $e2, $e3, $e4, $e5, $e6, $e6
.db $e7, $e8, $e8, $e9, $ea, $ea, $ea, $ea
.db $eb, $eb, $eb, $ec, $ec, $ec, $ec, $ec
.db $ec, $ec, $ec, $ec, $eb, $eb, $eb, $ea
.db $ea, $ea, $e9, $e8, $e7, $e7, $e6, $e6
.db $e5, $e4, $e3, $e2, $e1, $e0, $de, $dc
.db $db, $da, $d9, $d8, $d7, $d5, $d4, $d2
.db $d1, $d0, $ce, $cd, $cb, $ca, $c8, $c6
.db $c5, $c3, $c2, $c0, $be, $bd, $bb, $b9
.db $b7, $b5, $b4, $b2, $b0, $af, $ad, $ab
.db $54, $53, $51, $50, $4e, $4c, $4b, $49
.db $47, $45, $44, $42, $40, $3f, $3d, $3b
.db $3a, $39, $37, $36, $34, $32, $31, $30
.db $2e, $2d, $2c, $2b, $29, $28, $27, $25
.db $24, $23, $22, $21, $20, $1f, $1e, $1d
.db $1c, $1b, $1a, $19, $19, $18, $18, $17
.db $17, $16, $16, $16, $15, $15, $15, $15
.db $15, $14, $15, $14, $15, $15, $15, $16
.db $16, $17, $17, $17, $18, $18, $19, $19
.db $1a, $1b, $1c, $1d, $1e, $1f, $20, $21
.db $22, $23, $24, $25, $27, $28, $2a, $2b
.db $2c, $2e, $30, $31, $33, $34, $36, $38
.db $39, $3b, $3d, $3f, $41, $43, $45, $47
.db $49, $4b, $4d, $4f, $51, $54, $56, $58
.db $5b, $5c, $5e, $61, $63, $65, $68, $6a
.db $6d, $6f, $72, $74, $76, $79, $7b, $7d
.db $80, $83, $85, $87, $8a, $8c, $8e, $91
.db $93, $96, $98, $9b, $9d, $9f, $a2, $a4
.db $a5, $a8, $aa, $ac, $af, $b1, $b3, $b5
.db $b7, $b9, $bb, $bd, $bf, $c1, $c3, $c5
.db $c7, $c8, $ca, $cc, $cd, $cf, $d0, $d2
.db $d4, $d5, $d6, $d8, $d9, $db, $dc, $dd
.db $de, $df, $e0, $e1, $e2, $e3, $e4, $e5
.db $e6, $e7, $e7, $e8, $e8, $e9, $e9, $e9
.db $ea, $ea, $eb, $eb, $eb, $ec, $eb, $ec
.db $eb, $eb, $eb, $eb, $eb, $ea, $ea, $ea
.db $e9, $e9, $e8, $e8, $e7, $e7, $e6, $e5
.db $e4, $e3, $e2, $e1, $e0, $df, $de, $dd
.db $dc, $db, $d9, $d8, $d7, $d5, $d4, $d3
.db $d2, $d0, $cf, $ce, $cc, $ca, $c9, $c7
.db $c6, $c5, $c3, $c1, $c0, $be, $bc, $bb
.db $b9, $b7, $b5, $b4, $b2, $b0, $af, $ad
.db $52, $51, $4f, $4e, $4c, $4b, $49, $48
.db $46, $44, $42, $41, $3f, $3e, $3c, $3a
.db $39, $38, $36, $35, $33, $32, $31, $2f
.db $2e, $2d, $2b, $2a, $29, $28, $27, $25
.db $24, $23, $22, $21, $20, $1f, $1e, $1d
.db $1c, $1b, $1b, $1a, $1a, $19, $18, $17
.db $17, $17, $17, $16, $16, $16, $15, $16
.db $16, $15, $16, $15, $16, $16, $16, $17
.db $17, $18, $18, $18, $19, $19, $1a, $1a
.db $1b, $1c, $1d, $1e, $1f, $20, $21, $22
.db $23, $24, $25, $26, $28, $29, $2b, $2c
.db $2d, $2f, $31, $32, $34, $35, $37, $39
.db $3a, $3c, $3e, $40, $42, $44, $45, $48
.db $49, $4c, $4e, $50, $52, $54, $56, $59
.db $5b, $5d, $5f, $61, $63, $66, $68, $6a
.db $6d, $70, $72, $74, $76, $79, $7b, $7d
.db $80, $83, $85, $87, $8a, $8c, $8e, $90
.db $93, $96, $98, $9a, $9d, $9f, $a1, $a3
.db $a5, $a7, $aa, $ac, $ae, $b0, $b2, $b4
.db $b7, $b8, $bb, $bc, $be, $c0, $c2, $c4
.db $c6, $c7, $c9, $cb, $cc, $ce, $cf, $d1
.db $d3, $d4, $d5, $d7, $d8, $da, $db, $dc
.db $dd, $de, $df, $e0, $e1, $e2, $e3, $e4
.db $e5, $e6, $e6, $e7, $e7, $e8, $e8, $e8
.db $e9, $e9, $ea, $ea, $ea, $eb, $ea, $eb
.db $ea, $ea, $eb, $ea, $ea, $ea, $e9, $e9
.db $e9, $e9, $e8, $e7, $e6, $e6, $e5, $e5
.db $e4, $e3, $e2, $e1, $e0, $df, $de, $dd
.db $dc, $db, $d9, $d8, $d7, $d6, $d5, $d3
.db $d2, $d1, $cf, $ce, $cd, $cb, $ca, $c8
.db $c7, $c6, $c4, $c2, $c1, $bf, $be, $bc
.db $ba, $b8, $b7, $b5, $b4, $b2, $b1, $af
.db $50, $4f, $4d, $4c, $4a, $49, $47, $46
.db $44, $42, $41, $3f, $3e, $3c, $3b, $39
.db $38, $37, $35, $34, $32, $31, $30, $2f
.db $2d, $2c, $2b, $2a, $28, $27, $27, $25
.db $24, $23, $22, $21, $20, $1f, $1e, $1d
.db $1c, $1b, $1b, $1a, $1a, $19, $19, $18
.db $18, $18, $17, $17, $17, $17, $16, $17
.db $16, $16, $17, $16, $17, $17, $17, $18
.db $18, $19, $19, $19, $1a, $1a, $1b, $1b
.db $1c, $1d, $1e, $1f, $20, $21, $22, $23
.db $24, $25, $26, $27, $29, $2a, $2c, $2d
.db $2e, $30, $32, $33, $35, $36, $38, $3a
.db $3b, $3d, $3f, $41, $42, $45, $46, $49
.db $4a, $4c, $4f, $50, $53, $55, $57, $59
.db $5c, $5d, $5f, $62, $64, $66, $68, $6b
.db $6e, $70, $72, $74, $77, $79, $7b, $7d
.db $80, $83, $85, $87, $89, $8c, $8e, $90
.db $92, $95, $98, $9a, $9c, $9e, $a1, $a3
.db $a4, $a7, $a9, $ab, $ad, $b0, $b1, $b4
.db $b6, $b7, $ba, $bb, $be, $bf, $c1, $c3
.db $c5, $c6, $c8, $ca, $cb, $cd, $ce, $d0
.db $d2, $d3, $d4, $d6, $d7, $d9, $da, $db
.db $dc, $dd, $de, $df, $e0, $e1, $e2, $e3
.db $e4, $e5, $e5, $e6, $e6, $e7, $e7, $e7
.db $e8, $e8, $e9, $e9, $e9, $ea, $e9, $ea
.db $ea, $e9, $ea, $e9, $e9, $e9, $e9, $e8
.db $e8, $e8, $e7, $e7, $e6, $e6, $e5, $e5
.db $e4, $e3, $e2, $e1, $e0, $df, $de, $dd
.db $dc, $db, $d9, $d9, $d8, $d6, $d5, $d4
.db $d3, $d1, $d0, $cf, $ce, $cc, $cb, $c9
.db $c8, $c7, $c5, $c4, $c2, $c1, $bf, $be
.db $bc, $ba, $b9, $b7, $b6, $b4, $b3, $b1
.db $4e, $4d, $4c, $4a, $49, $47, $46, $44
.db $43, $41, $40, $3e, $3d, $3b, $3a, $38
.db $37, $36, $35, $33, $32, $30, $2f, $2e
.db $2d, $2c, $2a, $2a, $28, $27, $26, $25
.db $24, $23, $22, $21, $20, $1f, $1e, $1d
.db $1d, $1c, $1b, $1b, $1a, $1a, $19, $19
.db $18, $18, $18, $18, $17, $17, $17, $17
.db $17, $17, $17, $17, $18, $18, $18, $19
.db $19, $1a, $1a, $1a, $1b, $1b, $1c, $1c
.db $1d, $1e, $1f, $20, $21, $22, $23, $24
.db $25, $26, $27, $28, $2a, $2b, $2d, $2e
.db $2f, $31, $33, $34, $36, $37, $39, $3b
.db $3c, $3e, $40, $42, $43, $45, $47, $49
.db $4b, $4d, $4f, $51, $53, $55, $58, $5a
.db $5c, $5e, $60, $62, $64, $66, $69, $6b
.db $6e, $70, $72, $74, $77, $79, $7b, $7d
.db $80, $83, $85, $87, $89, $8c, $8e, $90
.db $92, $95, $97, $9a, $9c, $9e, $a0, $a2
.db $a4, $a6, $a8, $ab, $ad, $af, $b1, $b3
.db $b5, $b7, $b9, $bb, $bd, $be, $c0, $c2
.db $c4, $c5, $c7, $c9, $ca, $cc, $cd, $cf
.db $d1, $d2, $d3, $d5, $d6, $d8, $d9, $da
.db $db, $dc, $dd, $de, $df, $e0, $e1, $e2
.db $e3, $e4, $e4, $e5, $e5, $e6, $e6, $e6
.db $e7, $e7, $e8, $e8, $e8, $e9, $e9, $e9
.db $e9, $e9, $e9, $e9, $e9, $e8, $e8, $e8
.db $e8, $e7, $e7, $e6, $e6, $e5, $e5, $e4
.db $e3, $e3, $e2, $e1, $e0, $df, $de, $dd
.db $dc, $db, $da, $d9, $d8, $d6, $d6, $d4
.db $d3, $d2, $d1, $d0, $ce, $cd, $cb, $ca
.db $c9, $c8, $c6, $c5, $c3, $c2, $c0, $bf
.db $bd, $bc, $ba, $b9, $b7, $b6, $b4, $b3
.db $4c, $4b, $4a, $48, $47, $46, $44, $43
.db $41, $40, $3e, $3d, $3b, $3a, $39, $37
.db $36, $35, $34, $32, $31, $30, $2f, $2d
.db $2c, $2b, $2a, $29, $28, $27, $26, $25
.db $24, $23, $22, $21, $20, $1f, $1e, $1e
.db $1d, $1c, $1c, $1b, $1b, $1a, $1a, $19
.db $19, $19, $19, $18, $18, $18, $18, $18
.db $18, $18, $18, $18, $18, $19, $19, $1a
.db $1a, $1b, $1b, $1b, $1c, $1c, $1d, $1e
.db $1f, $20, $20, $21, $22, $23, $24, $25
.db $26, $27, $29, $2a, $2b, $2c, $2e, $2f
.db $30, $32, $34, $35, $37, $38, $3a, $3b
.db $3d, $3f, $41, $42, $44, $46, $48, $4a
.db $4c, $4e, $50, $52, $54, $56, $58, $5a
.db $5d, $5e, $60, $62, $65, $67, $69, $6b
.db $6e, $70, $72, $75, $77, $79, $7b, $7d
.db $80, $83, $85, $87, $89, $8b, $8e, $90
.db $92, $95, $97, $99, $9b, $9e, $a0, $a2
.db $a3, $a6, $a8, $aa, $ac, $ae, $b0, $b2
.db $b4, $b6, $b8, $ba, $bc, $be, $bf, $c1
.db $c3, $c5, $c6, $c8, $c9, $cb, $cc, $ce
.db $d0, $d1, $d2, $d4, $d5, $d6, $d7, $d9
.db $da, $db, $dc, $dd, $de, $df, $e0, $e0
.db $e1, $e2, $e3, $e4, $e4, $e5, $e5, $e5
.db $e6, $e6, $e7, $e7, $e8, $e8, $e8, $e8
.db $e8, $e8, $e8, $e8, $e8, $e8, $e7, $e7
.db $e7, $e7, $e6, $e6, $e5, $e5, $e4, $e4
.db $e3, $e2, $e2, $e1, $e0, $df, $de, $dd
.db $dc, $db, $da, $d9, $d8, $d7, $d6, $d5
.db $d4, $d3, $d1, $d0, $cf, $ce, $cc, $cb
.db $ca, $c9, $c7, $c6, $c5, $c3, $c2, $c0
.db $bf, $bd, $bc, $ba, $b9, $b8, $b6, $b5
.db $4a, $49, $48, $47, $45, $44, $43, $41
.db $40, $3e, $3d, $3b, $3a, $39, $38, $36
.db $35, $34, $33, $32, $30, $2f, $2e, $2d
.db $2c, $2b, $2a, $29, $27, $27, $26, $25
.db $24, $23, $22, $21, $20, $1f, $1f, $1e
.db $1d, $1c, $1c, $1b, $1b, $1b, $1a, $1a
.db $1a, $19, $19, $19, $19, $19, $19, $19
.db $19, $19, $19, $19, $19, $1a, $1a, $1b
.db $1b, $1c, $1c, $1c, $1d, $1d, $1e, $1f
.db $20, $21, $21, $22, $23, $24, $25, $26
.db $27, $28, $2a, $2b, $2c, $2d, $2f, $30
.db $31, $33, $35, $36, $38, $39, $3b, $3c
.db $3e, $40, $42, $43, $45, $47, $49, $4b
.db $4c, $4e, $51, $52, $54, $57, $59, $5b
.db $5d, $5f, $61, $63, $65, $67, $69, $6c
.db $6e, $70, $73, $75, $77, $79, $7b, $7d
.db $80, $83, $85, $87, $89, $8b, $8d, $90
.db $92, $94, $97, $99, $9b, $9d, $9f, $a1
.db $a3, $a5, $a7, $a9, $ac, $ae, $af, $b2
.db $b4, $b5, $b7, $b9, $bb, $bd, $be, $c0
.db $c2, $c4, $c5, $c7, $c8, $ca, $cb, $cd
.db $cf, $d0, $d1, $d3, $d4, $d5, $d6, $d8
.db $d9, $da, $db, $dc, $dd, $de, $df, $df
.db $e0, $e1, $e2, $e3, $e3, $e4, $e4, $e4
.db $e5, $e5, $e6, $e6, $e7, $e7, $e7, $e7
.db $e7, $e7, $e7, $e7, $e7, $e7, $e7, $e7
.db $e6, $e6, $e6, $e5, $e5, $e5, $e4, $e4
.db $e3, $e2, $e1, $e1, $e0, $df, $de, $dd
.db $dc, $db, $da, $d9, $d9, $d7, $d6, $d5
.db $d4, $d3, $d2, $d1, $d0, $ce, $cd, $cc
.db $cb, $ca, $c8, $c7, $c6, $c5, $c3, $c2
.db $c0, $bf, $bd, $bc, $bb, $b9, $b8, $b7
.db $48, $47, $46, $45, $43, $42, $41, $40
.db $3e, $3d, $3b, $3a, $39, $38, $36, $35
.db $34, $33, $32, $31, $2f, $2e, $2e, $2c
.db $2b, $2a, $29, $28, $27, $26, $26, $25
.db $24, $23, $22, $21, $20, $1f, $1f, $1e
.db $1d, $1d, $1d, $1c, $1c, $1b, $1b, $1a
.db $1a, $1a, $1a, $1a, $1a, $1a, $19, $1a
.db $1a, $1a, $1a, $1a, $1a, $1b, $1b, $1c
.db $1c, $1d, $1d, $1d, $1e, $1e, $1f, $20
.db $21, $22, $22, $23, $24, $25, $26, $27
.db $28, $29, $2b, $2c, $2d, $2e, $30, $31
.db $32, $34, $36, $37, $39, $3a, $3c, $3d
.db $3f, $40, $43, $44, $46, $48, $49, $4c
.db $4d, $4f, $51, $53, $55, $57, $59, $5b
.db $5e, $5f, $61, $63, $65, $68, $6a, $6c
.db $6f, $71, $73, $75, $77, $79, $7b, $7d
.db $80, $83, $85, $87, $89, $8b, $8d, $8f
.db $91, $94, $96, $98, $9b, $9d, $9f, $a1
.db $a2, $a5, $a7, $a9, $ab, $ad, $af, $b1
.db $b3, $b4, $b7, $b8, $ba, $bc, $bd, $c0
.db $c1, $c3, $c4, $c6, $c7, $c9, $ca, $cc
.db $ce, $cf, $d0, $d2, $d3, $d4, $d5, $d7
.db $d8, $d9, $da, $db, $dc, $dd, $de, $de
.db $df, $e0, $e1, $e2, $e2, $e3, $e3, $e3
.db $e4, $e4, $e5, $e5, $e6, $e6, $e6, $e6
.db $e6, $e6, $e7, $e6, $e6, $e6, $e6, $e6
.db $e6, $e6, $e5, $e5, $e4, $e4, $e3, $e3
.db $e3, $e2, $e1, $e1, $e0, $df, $de, $dd
.db $dc, $db, $da, $da, $d9, $d8, $d7, $d6
.db $d5, $d4, $d2, $d2, $d1, $cf, $ce, $cd
.db $cc, $cb, $ca, $c8, $c7, $c6, $c5, $c3
.db $c2, $c0, $bf, $be, $bd, $bb, $ba, $b9
.db $46, $45, $44, $43, $42, $41, $3f, $3e
.db $3d, $3b, $3a, $39, $38, $37, $35, $34
.db $33, $32, $31, $30, $2f, $2e, $2d, $2c
.db $2b, $2a, $29, $28, $27, $26, $26, $24
.db $24, $23, $22, $21, $20, $20, $1f, $1e
.db $1e, $1d, $1d, $1c, $1c, $1c, $1b, $1b
.db $1b, $1b, $1b, $1a, $1a, $1a, $1a, $1b
.db $1b, $1a, $1b, $1b, $1b, $1c, $1c, $1d
.db $1d, $1e, $1e, $1e, $1f, $1f, $20, $21
.db $22, $23, $23, $24, $25, $26, $27, $28
.db $29, $2a, $2c, $2d, $2e, $2f, $31, $32
.db $33, $35, $37, $38, $3a, $3b, $3d, $3e
.db $40, $41, $43, $45, $47, $49, $4a, $4c
.db $4e, $50, $52, $54, $56, $58, $5a, $5c
.db $5e, $60, $62, $64, $66, $68, $6a, $6c
.db $6f, $71, $73, $75, $77, $79, $7b, $7d
.db $80, $83, $85, $87, $89, $8b, $8d, $8f
.db $91, $94, $96, $98, $9a, $9c, $9e, $a0
.db $a2, $a4, $a6, $a8, $aa, $ac, $ae, $b0
.db $b2, $b4, $b6, $b7, $b9, $bb, $bd, $bf
.db $c0, $c2, $c3, $c5, $c6, $c8, $c9, $cb
.db $cd, $ce, $cf, $d1, $d2, $d3, $d4, $d6
.db $d7, $d8, $d9, $da, $db, $dc, $dd, $dd
.db $de, $df, $e0, $e1, $e1, $e2, $e2, $e2
.db $e3, $e3, $e4, $e4, $e5, $e5, $e5, $e6
.db $e5, $e5, $e6, $e6, $e6, $e6, $e5, $e5
.db $e5, $e5, $e5, $e4, $e4, $e4, $e3, $e3
.db $e2, $e2, $e1, $e0, $e0, $df, $de, $dd
.db $dc, $dc, $da, $da, $d9, $d8, $d7, $d6
.db $d5, $d4, $d3, $d2, $d1, $d0, $cf, $ce
.db $cd, $cc, $cb, $c9, $c8, $c7, $c6, $c5
.db $c3, $c2, $c1, $bf, $be, $bd, $bc, $bb
.db $44, $43, $42, $41, $40, $3f, $3e, $3d
.db $3b, $3a, $39, $38, $36, $35, $34, $33
.db $32, $31, $30, $2f, $2e, $2d, $2c, $2b
.db $2a, $29, $28, $28, $27, $26, $25, $24
.db $24, $23, $22, $21, $20, $20, $1f, $1f
.db $1e, $1d, $1d, $1d, $1d, $1c, $1c, $1b
.db $1b, $1b, $1b, $1b, $1b, $1b, $1b, $1b
.db $1b, $1b, $1c, $1c, $1c, $1d, $1d, $1e
.db $1e, $1f, $1f, $1f, $20, $20, $21, $22
.db $23, $24, $24, $25, $26, $27, $28, $29
.db $2a, $2b, $2d, $2e, $2f, $30, $32, $33
.db $34, $36, $38, $39, $3b, $3c, $3e, $3f
.db $41, $42, $44, $46, $47, $49, $4b, $4d
.db $4f, $51, $53, $54, $56, $58, $5a, $5c
.db $5f, $60, $62, $64, $66, $68, $6a, $6c
.db $6f, $71, $73, $75, $77, $79, $7b, $7d
.db $80, $83, $85, $87, $89, $8b, $8d, $8f
.db $91, $94, $96, $98, $9a, $9c, $9e, $a0
.db $a1, $a4, $a6, $a8, $aa, $ac, $ad, $af
.db $b1, $b3, $b5, $b7, $b9, $ba, $bc, $be
.db $bf, $c1, $c2, $c4, $c5, $c7, $c8, $ca
.db $cc, $cd, $ce, $d0, $d1, $d2, $d3, $d5
.db $d6, $d7, $d8, $d9, $da, $db, $dc, $dc
.db $dd, $de, $df, $e0, $e0, $e1, $e1, $e1
.db $e2, $e2, $e3, $e3, $e4, $e4, $e4, $e5
.db $e5, $e5, $e5, $e5, $e5, $e5, $e5, $e5
.db $e5, $e5, $e4, $e4, $e3, $e3, $e3, $e3
.db $e2, $e1, $e1, $e0, $e0, $df, $de, $dd
.db $dc, $dc, $db, $da, $d9, $d8, $d8, $d7
.db $d6, $d5, $d4, $d3, $d2, $d1, $d0, $cf
.db $ce, $cd, $cc, $cb, $ca, $c8, $c7, $c6
.db $c5, $c3, $c2, $c1, $c0, $bf, $be, $bd
.db $42, $41, $40, $3f, $3e, $3d, $3c, $3b
.db $3a, $38, $37, $36, $35, $34, $33, $32
.db $31, $30, $2f, $2e, $2d, $2c, $2c, $2b
.db $2a, $29, $28, $27, $26, $26, $25, $24
.db $24, $23, $22, $21, $20, $20, $1f, $1f
.db $1e, $1e, $1e, $1d, $1d, $1d, $1c, $1c
.db $1c, $1c, $1c, $1c, $1c, $1c, $1c, $1c
.db $1c, $1c, $1d, $1d, $1d, $1e, $1e, $1f
.db $1f, $20, $20, $20, $21, $21, $22, $23
.db $24, $25, $25, $26, $27, $28, $29, $2a
.db $2b, $2c, $2e, $2f, $30, $31, $33, $34
.db $35, $37, $39, $3a, $3c, $3d, $3f, $40
.db $42, $43, $45, $47, $48, $4a, $4c, $4e
.db $4f, $51, $53, $55, $57, $59, $5b, $5d
.db $5f, $61, $63, $65, $67, $69, $6b, $6d
.db $6f, $71, $73, $75, $77, $79, $7b, $7d
.db $80, $83, $85, $87, $89, $8b, $8d, $8f
.db $91, $93, $95, $97, $99, $9b, $9d, $9f
.db $a1, $a3, $a5, $a7, $a9, $ab, $ad, $af
.db $b1, $b2, $b4, $b6, $b8, $b9, $bb, $bd
.db $be, $c0, $c1, $c3, $c4, $c6, $c7, $c9
.db $cb, $cc, $cd, $cf, $d0, $d1, $d2, $d4
.db $d5, $d6, $d7, $d8, $d9, $da, $db, $db
.db $dc, $dd, $de, $df, $df, $e0, $e0, $e0
.db $e1, $e1, $e2, $e2, $e3, $e3, $e3, $e4
.db $e4, $e4, $e4, $e4, $e4, $e4, $e4, $e4
.db $e4, $e4, $e4, $e3, $e3, $e3, $e2, $e2
.db $e2, $e1, $e1, $e0, $e0, $df, $de, $dd
.db $dc, $dc, $db, $da, $da, $d9, $d8, $d7
.db $d6, $d5, $d4, $d4, $d3, $d2, $d1, $d0
.db $cf, $ce, $cd, $cc, $cb, $ca, $c9, $c8
.db $c6, $c5, $c4, $c3, $c2, $c1, $c0, $bf
.db $40, $3f, $3e, $3d, $3c, $3b, $3a, $39
.db $38, $37, $36, $35, $34, $33, $32, $31
.db $30, $2f, $2e, $2d, $2c, $2b, $2b, $2a
.db $29, $28, $27, $27, $26, $25, $25, $24
.db $23, $23, $22, $21, $21, $20, $20, $1f
.db $1f, $1e, $1e, $1e, $1e, $1d, $1d, $1d
.db $1d, $1d, $1d, $1d, $1d, $1d, $1d, $1d
.db $1d, $1d, $1e, $1e, $1e, $1f, $1f, $20
.db $20, $21, $21, $22, $22, $23, $24, $24
.db $25, $26, $27, $28, $29, $2a, $2b, $2c
.db $2d, $2e, $2f, $30, $32, $33, $34, $35
.db $37, $38, $3a, $3b, $3d, $3e, $40, $41
.db $43, $44, $46, $48, $49, $4b, $4d, $4f
.db $50, $52, $54, $56, $58, $5a, $5c, $5e
.db $60, $61, $63, $65, $67, $69, $6b, $6d
.db $70, $72, $74, $76, $78, $7a, $7c, $7e
.db $80, $82, $84, $86, $88, $8a, $8c, $8e
.db $90, $93, $95, $97, $99, $9b, $9d, $9f
.db $a0, $a2, $a4, $a6, $a8, $aa, $ac, $ae
.db $b0, $b1, $b3, $b5, $b7, $b8, $ba, $bc
.db $bd, $bf, $c0, $c2, $c3, $c5, $c6, $c8
.db $c9, $cb, $cc, $cd, $ce, $d0, $d1, $d2
.db $d3, $d4, $d5, $d6, $d7, $d8, $d9, $da
.db $db, $dc, $dc, $dd, $de, $de, $df, $df
.db $e0, $e0, $e1, $e1, $e2, $e2, $e2, $e3
.db $e3, $e3, $e3, $e3, $e3, $e3, $e3, $e3
.db $e3, $e3, $e3, $e3, $e2, $e2, $e2, $e2
.db $e1, $e1, $e0, $e0, $df, $df, $de, $dd
.db $dd, $dc, $db, $db, $da, $d9, $d9, $d8
.db $d7, $d6, $d5, $d5, $d4, $d3, $d2, $d1
.db $d0, $cf, $ce, $cd, $cc, $cb, $ca, $c9
.db $c8, $c7, $c6, $c5, $c4, $c3, $c2, $c1
.db $3e, $3e, $3d, $3c, $3b, $3a, $39, $38
.db $37, $36, $35, $34, $33, $32, $31, $30
.db $2f, $2f, $2e, $2d, $2c, $2b, $2a, $29
.db $28, $28, $27, $27, $26, $25, $25, $24
.db $23, $23, $22, $21, $21, $20, $20, $1f
.db $1f, $1e, $1e, $1e, $1e, $1d, $1e, $1d
.db $1d, $1d, $1d, $1d, $1d, $1d, $1d, $1e
.db $1e, $1e, $1e, $1e, $1f, $1f, $20, $20
.db $21, $22, $22, $23, $23, $24, $25, $25
.db $26, $27, $28, $29, $2a, $2b, $2c, $2d
.db $2e, $2f, $30, $31, $33, $34, $35, $36
.db $38, $39, $3b, $3c, $3d, $3f, $40, $42
.db $43, $45, $47, $48, $4a, $4c, $4d, $4f
.db $51, $53, $55, $56, $58, $5a, $5c, $5e
.db $60, $61, $63, $65, $67, $69, $6b, $6d
.db $70, $72, $74, $76, $78, $7a, $7c, $7e
.db $80, $82, $84, $86, $88, $8a, $8c, $8e
.db $90, $93, $95, $97, $99, $9b, $9d, $9f
.db $a0, $a2, $a4, $a6, $a8, $aa, $ab, $ad
.db $af, $b1, $b3, $b4, $b6, $b8, $b9, $bb
.db $bd, $be, $c0, $c1, $c3, $c4, $c5, $c7
.db $c8, $ca, $cb, $cc, $cd, $cf, $d0, $d1
.db $d2, $d3, $d4, $d5, $d6, $d7, $d8, $d9
.db $da, $db, $db, $dc, $dd, $dd, $de, $de
.db $df, $e0, $e0, $e1, $e1, $e2, $e2, $e2
.db $e2, $e2, $e3, $e3, $e3, $e3, $e3, $e3
.db $e3, $e3, $e2, $e3, $e2, $e2, $e2, $e2
.db $e1, $e1, $e0, $e0, $df, $df, $de, $dd
.db $dd, $dc, $db, $db, $da, $d9, $d9, $d8
.db $d8, $d7, $d6, $d5, $d4, $d3, $d2, $d1
.db $d1, $d0, $cf, $ce, $cd, $cc, $cb, $ca
.db $c9, $c8, $c7, $c6, $c5, $c4, $c3, $c2
.db $3c, $3c, $3b, $3a, $39, $38, $37, $36
.db $35, $34, $33, $32, $32, $31, $30, $2f
.db $2e, $2e, $2d, $2c, $2b, $2a, $2a, $29
.db $28, $28, $27, $26, $25, $25, $25, $24
.db $23, $23, $22, $21, $21, $20, $20, $1f
.db $1f, $1f, $1f, $1e, $1e, $1e, $1e, $1e
.db $1e, $1e, $1e, $1e, $1e, $1e, $1e, $1f
.db $1f, $1f, $1f, $1f, $20, $20, $21, $21
.db $22, $23, $23, $24, $24, $25, $26, $26
.db $27, $28, $29, $2a, $2b, $2c, $2d, $2e
.db $2f, $30, $31, $32, $34, $35, $36, $37
.db $39, $3a, $3c, $3d, $3e, $40, $41, $43
.db $44, $46, $48, $49, $4b, $4d, $4e, $50
.db $51, $53, $55, $57, $59, $5b, $5d, $5f
.db $61, $62, $64, $66, $68, $6a, $6c, $6e
.db $70, $72, $74, $76, $78, $7a, $7c, $7e
.db $80, $82, $84, $86, $88, $8a, $8c, $8e
.db $90, $92, $94, $96, $98, $9a, $9c, $9e
.db $9f, $a1, $a3, $a5, $a7, $a9, $ab, $ad
.db $af, $b0, $b2, $b3, $b5, $b7, $b8, $ba
.db $bc, $bd, $bf, $c0, $c2, $c3, $c4, $c6
.db $c7, $c9, $ca, $cb, $cc, $ce, $cf, $d0
.db $d1, $d2, $d3, $d4, $d5, $d6, $d7, $d8
.db $d9, $da, $da, $db, $dc, $dc, $dd, $dd
.db $de, $df, $df, $e0, $e0, $e1, $e1, $e1
.db $e1, $e1, $e2, $e2, $e2, $e2, $e2, $e2
.db $e2, $e2, $e2, $e2, $e2, $e2, $e1, $e1
.db $e1, $e1, $e0, $e0, $df, $df, $de, $dd
.db $dd, $dc, $db, $db, $db, $da, $d9, $d8
.db $d8, $d7, $d6, $d6, $d5, $d4, $d3, $d2
.db $d2, $d1, $d0, $cf, $ce, $ce, $cd, $cc
.db $cb, $ca, $c9, $c8, $c7, $c6, $c5, $c4
.db $3a, $3a, $39, $38, $37, $36, $36, $35
.db $34, $33, $32, $31, $30, $2f, $2f, $2e
.db $2d, $2d, $2c, $2b, $2a, $29, $29, $28
.db $27, $27, $26, $26, $25, $25, $24, $24
.db $23, $23, $22, $21, $21, $20, $20, $20
.db $1f, $1f, $1f, $1f, $1f, $1e, $1f, $1e
.db $1e, $1e, $1e, $1f, $1f, $1f, $1f, $1f
.db $1f, $20, $20, $20, $21, $21, $22, $22
.db $23, $24, $24, $25, $25, $26, $27, $27
.db $28, $29, $2a, $2b, $2c, $2d, $2e, $2f
.db $30, $31, $32, $33, $35, $36, $37, $38
.db $3a, $3b, $3d, $3e, $3f, $41, $42, $44
.db $45, $47, $49, $4a, $4b, $4d, $4f, $51
.db $52, $54, $56, $57, $59, $5b, $5d, $5f
.db $61, $62, $64, $66, $68, $6a, $6c, $6e
.db $70, $72, $74, $76, $78, $7a, $7c, $7e
.db $80, $82, $84, $86, $88, $8a, $8c, $8e
.db $90, $92, $94, $96, $98, $9a, $9c, $9e
.db $9f, $a1, $a3, $a5, $a7, $a9, $aa, $ac
.db $ae, $af, $b1, $b3, $b5, $b6, $b7, $b9
.db $bb, $bc, $be, $bf, $c1, $c2, $c3, $c5
.db $c6, $c8, $c9, $ca, $cb, $cd, $ce, $cf
.db $d0, $d1, $d2, $d3, $d4, $d5, $d6, $d7
.db $d8, $d9, $d9, $da, $db, $db, $dc, $dc
.db $dd, $de, $de, $df, $df, $e0, $e0, $e0
.db $e1, $e1, $e1, $e1, $e1, $e1, $e2, $e2
.db $e2, $e2, $e1, $e2, $e1, $e1, $e1, $e1
.db $e1, $e0, $e0, $e0, $df, $df, $de, $dd
.db $dd, $dc, $dc, $db, $db, $da, $da, $d9
.db $d9, $d8, $d7, $d7, $d6, $d5, $d4, $d3
.db $d3, $d2, $d1, $d1, $d0, $cf, $ce, $cd
.db $cc, $cb, $ca, $ca, $c9, $c8, $c7, $c6
.db $38, $38, $37, $36, $35, $35, $34, $33
.db $32, $31, $31, $30, $2f, $2e, $2e, $2d
.db $2c, $2c, $2b, $2a, $29, $29, $28, $28
.db $27, $27, $26, $26, $25, $24, $24, $23
.db $23, $23, $22, $21, $21, $21, $20, $20
.db $20, $1f, $1f, $1f, $1f, $1f, $1f, $1f
.db $1f, $1f, $1f, $1f, $1f, $20, $20, $20
.db $20, $20, $21, $21, $22, $22, $23, $23
.db $24, $25, $25, $26, $26, $27, $28, $28
.db $29, $2a, $2b, $2c, $2d, $2e, $2f, $30
.db $31, $32, $33, $34, $36, $37, $38, $39
.db $3b, $3c, $3e, $3f, $40, $42, $43, $45
.db $46, $48, $49, $4b, $4c, $4e, $50, $52
.db $53, $55, $57, $58, $5a, $5c, $5e, $60
.db $62, $63, $65, $67, $69, $6a, $6c, $6e
.db $71, $72, $74, $76, $78, $7a, $7c, $7e
.db $80, $82, $84, $86, $88, $8a, $8c, $8e
.db $8f, $92, $94, $96, $97, $99, $9b, $9d
.db $9e, $a0, $a2, $a4, $a6, $a8, $a9, $ab
.db $ad, $ae, $b0, $b2, $b4, $b5, $b7, $b8
.db $ba, $bb, $bd, $be, $c0, $c1, $c2, $c4
.db $c5, $c7, $c8, $c9, $ca, $cc, $cd, $ce
.db $cf, $d0, $d1, $d2, $d3, $d4, $d5, $d6
.db $d7, $d8, $d8, $d9, $da, $da, $db, $db
.db $dc, $dd, $dd, $de, $de, $df, $df, $e0
.db $e0, $e0, $e0, $e0, $e1, $e1, $e1, $e1
.db $e1, $e1, $e1, $e1, $e1, $e1, $e1, $e1
.db $e0, $e0, $e0, $df, $df, $df, $de, $dd
.db $dd, $dd, $dc, $dc, $db, $da, $da, $d9
.db $d9, $d8, $d8, $d7, $d7, $d6, $d5, $d4
.db $d4, $d3, $d2, $d2, $d1, $d0, $cf, $cf
.db $ce, $cd, $cc, $cb, $cb, $ca, $c9, $c8
.db $36, $36, $35, $34, $34, $33, $32, $32
.db $31, $30, $2f, $2f, $2e, $2d, $2c, $2c
.db $2b, $2b, $2a, $29, $29, $28, $28, $27
.db $26, $26, $25, $25, $25, $24, $24, $23
.db $23, $23, $22, $21, $21, $21, $20, $20
.db $20, $20, $20, $20, $20, $1f, $20, $1f
.db $1f, $20, $20, $20, $20, $20, $20, $21
.db $21, $21, $22, $22, $23, $23, $24, $24
.db $25, $26, $26, $27, $27, $28, $29, $29
.db $2a, $2b, $2c, $2d, $2e, $2f, $30, $31
.db $32, $33, $34, $35, $37, $38, $39, $3a
.db $3c, $3d, $3f, $40, $41, $43, $44, $46
.db $47, $48, $4a, $4c, $4d, $4f, $50, $52
.db $54, $56, $57, $59, $5b, $5c, $5e, $60
.db $62, $63, $65, $67, $69, $6b, $6d, $6e
.db $71, $73, $74, $76, $78, $7a, $7c, $7e
.db $80, $82, $84, $86, $88, $8a, $8c, $8d
.db $8f, $92, $93, $95, $97, $99, $9b, $9d
.db $9e, $a0, $a2, $a4, $a5, $a7, $a9, $aa
.db $ac, $ae, $b0, $b1, $b3, $b4, $b6, $b8
.db $b9, $ba, $bc, $bd, $bf, $c0, $c1, $c3
.db $c4, $c6, $c7, $c8, $c9, $cb, $cc, $cd
.db $ce, $cf, $d0, $d1, $d2, $d3, $d4, $d5
.db $d6, $d7, $d7, $d8, $d9, $d9, $da, $da
.db $db, $dc, $dc, $dd, $dd, $de, $de, $df
.db $df, $df, $e0, $e0, $e0, $e0, $e0, $e0
.db $e1, $e1, $e0, $e1, $e0, $e0, $e0, $e0
.db $e0, $e0, $e0, $df, $df, $df, $de, $dd
.db $dd, $dd, $dc, $dc, $db, $db, $db, $da
.db $da, $d9, $d8, $d8, $d7, $d7, $d6, $d5
.db $d5, $d4, $d4, $d3, $d2, $d1, $d1, $d0
.db $cf, $ce, $ce, $cd, $cc, $cc, $cb, $ca
.db $34, $34, $33, $33, $32, $31, $31, $30
.db $2f, $2e, $2e, $2d, $2d, $2c, $2b, $2b
.db $2a, $2a, $29, $29, $28, $27, $27, $27
.db $26, $26, $25, $25, $24, $24, $24, $23
.db $23, $23, $22, $21, $21, $21, $21, $20
.db $20, $20, $20, $20, $20, $20, $20, $20
.db $20, $20, $20, $21, $21, $21, $21, $22
.db $22, $22, $23, $23, $24, $24, $25, $25
.db $26, $27, $27, $28, $28, $29, $2a, $2a
.db $2b, $2c, $2d, $2e, $2f, $30, $31, $32
.db $33, $34, $35, $36, $38, $39, $3a, $3b
.db $3d, $3e, $40, $41, $42, $44, $45, $47
.db $48, $49, $4b, $4d, $4e, $50, $51, $53
.db $54, $56, $58, $59, $5b, $5d, $5f, $61
.db $63, $64, $66, $68, $69, $6b, $6d, $6f
.db $71, $73, $75, $76, $78, $7a, $7c, $7e
.db $80, $82, $84, $86, $88, $8a, $8b, $8d
.db $8f, $91, $93, $95, $97, $98, $9a, $9c
.db $9d, $9f, $a1, $a3, $a5, $a7, $a8, $aa
.db $ac, $ad, $af, $b0, $b2, $b3, $b5, $b7
.db $b8, $b9, $bb, $bc, $be, $bf, $c0, $c2
.db $c3, $c5, $c6, $c7, $c8, $ca, $cb, $cc
.db $cd, $ce, $cf, $d0, $d1, $d2, $d3, $d4
.db $d5, $d6, $d6, $d7, $d8, $d8, $d9, $d9
.db $da, $db, $db, $dc, $dc, $dd, $dd, $de
.db $de, $de, $df, $df, $df, $df, $e0, $e0
.db $e0, $e0, $e0, $e0, $e0, $e0, $e0, $e0
.db $e0, $e0, $df, $df, $df, $df, $de, $dd
.db $dd, $dd, $dc, $dc, $dc, $db, $db, $da
.db $da, $d9, $d9, $d9, $d8, $d7, $d7, $d6
.db $d6, $d5, $d5, $d4, $d3, $d3, $d2, $d2
.db $d1, $d0, $cf, $cf, $ce, $cd, $cd, $cc
.db $32, $32, $31, $31, $30, $30, $2f, $2f
.db $2e, $2d, $2c, $2c, $2b, $2b, $2a, $2a
.db $29, $29, $28, $28, $27, $27, $27, $26
.db $25, $25, $25, $24, $24, $24, $24, $23
.db $23, $23, $22, $21, $21, $21, $21, $21
.db $20, $20, $21, $20, $21, $20, $21, $20
.db $21, $21, $21, $21, $22, $22, $22, $23
.db $23, $23, $24, $24, $24, $25, $26, $26
.db $27, $28, $28, $29, $29, $2a, $2b, $2c
.db $2d, $2e, $2e, $2f, $30, $31, $32, $33
.db $34, $35, $37, $38, $39, $3a, $3b, $3c
.db $3e, $3f, $41, $42, $43, $45, $46, $47
.db $49, $4a, $4c, $4d, $4f, $51, $52, $54
.db $55, $57, $59, $5a, $5c, $5e, $5f, $61
.db $63, $64, $66, $68, $6a, $6c, $6d, $6f
.db $71, $73, $75, $77, $78, $7a, $7c, $7e
.db $80, $82, $84, $86, $88, $89, $8b, $8d
.db $8f, $91, $93, $94, $96, $98, $9a, $9c
.db $9d, $9f, $a1, $a2, $a4, $a6, $a7, $a9
.db $ab, $ac, $ae, $af, $b1, $b3, $b4, $b6
.db $b7, $b9, $ba, $bb, $bd, $be, $bf, $c1
.db $c2, $c4, $c5, $c6, $c7, $c8, $c9, $cb
.db $cc, $cd, $ce, $cf, $d0, $d1, $d2, $d2
.db $d3, $d4, $d5, $d6, $d7, $d7, $d8, $d8
.db $d9, $da, $da, $db, $dc, $dc, $dc, $dd
.db $dd, $dd, $de, $de, $de, $df, $df, $df
.db $df, $e0, $df, $e0, $df, $e0, $df, $e0
.db $e0, $df, $df, $df, $df, $df, $de, $dd
.db $dd, $dd, $dc, $dc, $dc, $dc, $db, $db
.db $db, $da, $d9, $d9, $d9, $d8, $d8, $d7
.db $d7, $d6, $d6, $d5, $d5, $d4, $d4, $d3
.db $d2, $d1, $d1, $d0, $d0, $cf, $cf, $ce
.db $30, $30, $2f, $2f, $2e, $2e, $2d, $2d
.db $2c, $2c, $2b, $2b, $2a, $2a, $29, $29
.db $28, $28, $27, $27, $26, $26, $26, $25
.db $25, $25, $24, $24, $24, $23, $23, $23
.db $23, $23, $22, $21, $21, $21, $21, $21
.db $21, $21, $21, $21, $21, $21, $21, $21
.db $21, $22, $22, $22, $22, $23, $23, $23
.db $24, $24, $25, $25, $25, $26, $27, $27
.db $28, $29, $29, $2a, $2a, $2b, $2c, $2d
.db $2e, $2f, $2f, $30, $31, $32, $33, $34
.db $35, $36, $38, $39, $3a, $3b, $3c, $3d
.db $3f, $40, $42, $43, $44, $46, $47, $48
.db $4a, $4b, $4d, $4e, $50, $51, $53, $55
.db $56, $58, $59, $5b, $5d, $5e, $60, $62
.db $64, $65, $67, $68, $6a, $6c, $6e, $6f
.db $72, $73, $75, $77, $79, $7a, $7c, $7e
.db $80, $82, $84, $86, $87, $89, $8b, $8d
.db $8e, $91, $92, $94, $96, $98, $99, $9b
.db $9c, $9e, $a0, $a2, $a3, $a5, $a7, $a8
.db $aa, $ab, $ad, $af, $b0, $b2, $b3, $b5
.db $b6, $b8, $b9, $ba, $bc, $bd, $be, $c0
.db $c1, $c3, $c4, $c5, $c6, $c7, $c8, $ca
.db $cb, $cc, $cd, $ce, $cf, $d0, $d1, $d1
.db $d2, $d3, $d4, $d5, $d6, $d6, $d7, $d7
.db $d8, $d9, $d9, $da, $db, $db, $db, $dc
.db $dc, $dd, $dd, $dd, $de, $de, $de, $de
.db $df, $df, $df, $df, $df, $df, $df, $df
.db $df, $df, $df, $df, $df, $df, $de, $dd
.db $dd, $dd, $dd, $dd, $dc, $dc, $dc, $db
.db $db, $db, $da, $da, $da, $d9, $d9, $d8
.db $d8, $d7, $d7, $d6, $d6, $d5, $d5, $d4
.db $d4, $d3, $d3, $d2, $d2, $d1, $d1, $d0
.db $2e, $2e, $2e, $2d, $2d, $2c, $2c, $2b
.db $2b, $2a, $2a, $29, $29, $28, $28, $28
.db $27, $27, $27, $26, $26, $25, $25, $25
.db $24, $24, $24, $24, $23, $23, $23, $23
.db $23, $23, $22, $21, $21, $21, $21, $21
.db $21, $21, $21, $21, $21, $21, $22, $22
.db $22, $22, $22, $23, $23, $23, $24, $24
.db $24, $25, $25, $26, $26, $27, $28, $28
.db $29, $2a, $2a, $2b, $2b, $2c, $2d, $2e
.db $2f, $30, $30, $31, $32, $33, $34, $35
.db $36, $37, $39, $3a, $3b, $3c, $3d, $3e
.db $40, $41, $43, $44, $45, $47, $48, $49
.db $4b, $4c, $4e, $4f, $50, $52, $54, $55
.db $57, $58, $5a, $5b, $5d, $5f, $61, $62
.db $64, $65, $67, $69, $6b, $6c, $6e, $70
.db $72, $73, $75, $77, $79, $7a, $7c, $7e
.db $80, $82, $84, $86, $87, $89, $8b, $8d
.db $8e, $90, $92, $94, $95, $97, $99, $9b
.db $9c, $9e, $9f, $a1, $a3, $a5, $a6, $a8
.db $a9, $ab, $ac, $ae, $b0, $b1, $b2, $b4
.db $b5, $b7, $b8, $b9, $bb, $bc, $bd, $bf
.db $c0, $c2, $c3, $c4, $c5, $c6, $c7, $c9
.db $ca, $cb, $cc, $cd, $ce, $cf, $d0, $d0
.db $d1, $d2, $d3, $d4, $d5, $d5, $d6, $d6
.db $d7, $d8, $d8, $d9, $da, $da, $db, $db
.db $dc, $dc, $dc, $dd, $dd, $dd, $de, $de
.db $de, $de, $de, $df, $df, $df, $df, $df
.db $df, $df, $df, $df, $df, $df, $de, $dd
.db $dd, $dd, $dd, $dd, $dd, $dc, $dc, $dc
.db $dc, $db, $db, $db, $da, $da, $d9, $d9
.db $d9, $d8, $d8, $d8, $d7, $d7, $d6, $d6
.db $d5, $d5, $d4, $d4, $d3, $d3, $d2, $d2
.db $2c, $2c, $2c, $2b, $2b, $2b, $2a, $2a
.db $29, $29, $28, $28, $28, $27, $27, $27
.db $26, $26, $26, $25, $25, $25, $25, $24
.db $24, $24, $23, $23, $23, $23, $23, $23
.db $23, $23, $22, $21, $21, $21, $21, $21
.db $21, $21, $22, $22, $22, $22, $22, $22
.db $22, $23, $23, $23, $24, $24, $24, $25
.db $25, $26, $26, $27, $27, $28, $29, $29
.db $2a, $2b, $2b, $2c, $2c, $2d, $2e, $2f
.db $30, $31, $31, $32, $33, $34, $35, $36
.db $37, $38, $3a, $3b, $3c, $3d, $3e, $3f
.db $41, $42, $44, $45, $46, $48, $49, $4a
.db $4c, $4d, $4f, $50, $51, $53, $54, $56
.db $57, $59, $5b, $5c, $5e, $5f, $61, $63
.db $65, $66, $68, $69, $6b, $6d, $6e, $70
.db $72, $74, $75, $77, $79, $7a, $7c, $7e
.db $80, $82, $84, $86, $87, $89, $8b, $8c
.db $8e, $90, $92, $93, $95, $97, $98, $9a
.db $9b, $9d, $9f, $a1, $a2, $a4, $a5, $a7
.db $a9, $aa, $ac, $ad, $af, $b0, $b1, $b3
.db $b4, $b6, $b7, $b8, $ba, $bb, $bc, $be
.db $bf, $c1, $c2, $c3, $c4, $c5, $c6, $c8
.db $c9, $ca, $cb, $cc, $cd, $ce, $cf, $cf
.db $d0, $d1, $d2, $d3, $d4, $d4, $d5, $d5
.db $d6, $d7, $d7, $d8, $d9, $d9, $da, $da
.db $db, $db, $dc, $dc, $dc, $dd, $dd, $dd
.db $de, $de, $de, $de, $de, $de, $de, $df
.db $df, $df, $df, $df, $df, $df, $de, $dd
.db $dd, $dd, $dd, $dd, $dd, $dd, $dd, $dc
.db $dc, $dc, $db, $db, $db, $db, $da, $da
.db $da, $d9, $d9, $d9, $d8, $d8, $d8, $d7
.db $d7, $d6, $d6, $d5, $d5, $d5, $d4, $d4
.db $2a, $2a, $2a, $2a, $29, $29, $29, $28
.db $28, $27, $27, $27, $26, $26, $26, $25
.db $25, $25, $25, $25, $24, $24, $24, $24
.db $23, $23, $23, $23, $23, $23, $23, $22
.db $22, $22, $22, $22, $22, $22, $22, $22
.db $22, $22, $22, $22, $22, $22, $23, $23
.db $23, $23, $24, $24, $24, $25, $25, $26
.db $26, $26, $27, $27, $28, $29, $29, $2a
.db $2b, $2c, $2c, $2d, $2e, $2e, $2f, $30
.db $31, $32, $33, $34, $35, $36, $37, $38
.db $39, $3a, $3b, $3c, $3d, $3e, $40, $41
.db $42, $43, $45, $46, $47, $48, $4a, $4b
.db $4c, $4e, $4f, $51, $52, $54, $55, $57
.db $58, $5a, $5b, $5d, $5e, $60, $62, $63
.db $65, $66, $68, $6a, $6b, $6d, $6f, $70
.db $72, $74, $76, $77, $79, $7b, $7c, $7e
.db $80, $82, $84, $85, $87, $89, $8a, $8c
.db $8e, $90, $91, $93, $95, $96, $98, $9a
.db $9b, $9d, $9e, $a0, $a2, $a3, $a5, $a6
.db $a8, $a9, $ab, $ac, $ae, $af, $b1, $b2
.db $b4, $b5, $b6, $b8, $b9, $ba, $bb, $bd
.db $be, $bf, $c0, $c2, $c3, $c4, $c5, $c6
.db $c7, $c8, $c9, $ca, $cb, $cc, $cd, $ce
.db $cf, $d0, $d1, $d2, $d2, $d3, $d4, $d4
.db $d5, $d6, $d7, $d7, $d8, $d9, $d9, $da
.db $da, $da, $db, $db, $dc, $dc, $dc, $dd
.db $dd, $dd, $dd, $de, $de, $de, $de, $de
.db $de, $de, $de, $de, $de, $de, $de, $de
.db $de, $de, $dd, $dd, $dd, $dd, $dd, $dd
.db $dd, $dc, $dc, $dc, $dc, $db, $db, $db
.db $db, $db, $da, $da, $da, $d9, $d9, $d9
.db $d8, $d8, $d7, $d7, $d7, $d6, $d6, $d6
.db $28, $28, $28, $28, $27, $27, $27, $27
.db $26, $26, $26, $25, $25, $25, $25, $24
.db $24, $24, $24, $24, $23, $23, $23, $23
.db $23, $23, $23, $23, $22, $22, $23, $22
.db $22, $22, $22, $22, $22, $22, $22, $22
.db $22, $22, $22, $22, $23, $23, $23, $23
.db $24, $24, $24, $25, $25, $26, $26, $27
.db $27, $27, $28, $28, $29, $2a, $2a, $2b
.db $2c, $2d, $2d, $2e, $2f, $2f, $30, $31
.db $32, $33, $34, $35, $36, $37, $38, $39
.db $3a, $3b, $3c, $3d, $3e, $3f, $41, $42
.db $43, $44, $46, $47, $48, $49, $4b, $4c
.db $4d, $4f, $50, $52, $53, $55, $56, $58
.db $59, $5a, $5c, $5d, $5f, $61, $62, $64
.db $66, $67, $68, $6a, $6c, $6d, $6f, $71
.db $73, $74, $76, $77, $79, $7b, $7c, $7e
.db $80, $82, $84, $85, $87, $89, $8a, $8c
.db $8d, $8f, $91, $93, $94, $96, $98, $99
.db $9a, $9c, $9e, $9f, $a1, $a3, $a4, $a6
.db $a7, $a8, $aa, $ab, $ad, $ae, $b0, $b1
.db $b3, $b4, $b5, $b7, $b8, $b9, $ba, $bc
.db $bd, $be, $bf, $c1, $c2, $c3, $c4, $c5
.db $c6, $c7, $c8, $c9, $ca, $cb, $cc, $cd
.db $ce, $cf, $d0, $d1, $d1, $d2, $d3, $d3
.db $d4, $d5, $d6, $d6, $d7, $d8, $d8, $d9
.db $d9, $d9, $da, $da, $db, $db, $dc, $dc
.db $dc, $dd, $dd, $dd, $dd, $de, $de, $de
.db $de, $de, $de, $de, $de, $de, $de, $de
.db $de, $de, $dd, $de, $de, $dd, $dd, $dd
.db $dd, $dd, $dd, $dd, $dd, $dc, $dc, $dc
.db $dc, $dc, $db, $db, $db, $db, $da, $da
.db $da, $d9, $d9, $d9, $d9, $d8, $d8, $d8
.db $26, $26, $26, $26, $26, $26, $25, $25
.db $25, $25, $24, $24, $24, $24, $24, $23
.db $23, $23, $23, $23, $23, $23, $23, $22
.db $22, $22, $22, $22, $22, $22, $22, $22
.db $22, $22, $22, $22, $22, $22, $22, $22
.db $22, $22, $23, $23, $23, $23, $24, $24
.db $24, $25, $25, $25, $26, $26, $27, $27
.db $28, $28, $29, $29, $2a, $2b, $2b, $2c
.db $2d, $2e, $2e, $2f, $30, $30, $31, $32
.db $33, $34, $35, $36, $37, $38, $39, $3a
.db $3b, $3c, $3d, $3e, $3f, $40, $42, $43
.db $44, $45, $47, $48, $49, $4a, $4c, $4d
.db $4e, $50, $51, $52, $54, $55, $57, $58
.db $5a, $5b, $5d, $5e, $60, $61, $63, $64
.db $66, $67, $69, $6a, $6c, $6e, $6f, $71
.db $73, $74, $76, $78, $79, $7b, $7c, $7e
.db $80, $82, $84, $85, $87, $88, $8a, $8c
.db $8d, $8f, $91, $92, $94, $96, $97, $99
.db $9a, $9c, $9d, $9f, $a0, $a2, $a3, $a5
.db $a6, $a8, $a9, $ab, $ac, $ae, $af, $b0
.db $b2, $b3, $b4, $b6, $b7, $b8, $b9, $bb
.db $bc, $bd, $be, $c0, $c1, $c2, $c3, $c4
.db $c5, $c6, $c7, $c8, $c9, $ca, $cb, $cc
.db $cd, $ce, $cf, $d0, $d0, $d1, $d2, $d2
.db $d3, $d4, $d5, $d5, $d6, $d7, $d7, $d8
.db $d8, $d9, $d9, $da, $da, $db, $db, $db
.db $dc, $dc, $dc, $dd, $dd, $dd, $dd, $de
.db $de, $de, $de, $de, $de, $de, $de, $de
.db $de, $de, $de, $de, $de, $de, $de, $de
.db $de, $de, $dd, $dd, $dd, $dd, $dd, $dd
.db $dd, $dd, $dc, $dc, $dc, $dc, $dc, $db
.db $db, $db, $db, $da, $da, $da, $da, $da
.db $24, $24, $24, $24, $24, $24, $24, $24
.db $23, $23, $23, $23, $23, $23, $22, $22
.db $22, $22, $22, $22, $22, $22, $22, $22
.db $22, $22, $22, $22, $22, $22, $22, $22
.db $22, $22, $22, $22, $22, $22, $22, $22
.db $22, $23, $23, $23, $24, $24, $24, $24
.db $25, $25, $26, $26, $27, $27, $27, $28
.db $29, $29, $2a, $2a, $2b, $2c, $2c, $2d
.db $2e, $2f, $2f, $30, $31, $31, $32, $33
.db $34, $35, $36, $37, $38, $39, $3a, $3b
.db $3c, $3d, $3e, $3f, $40, $41, $43, $44
.db $45, $46, $48, $49, $4a, $4b, $4d, $4e
.db $4f, $50, $52, $53, $55, $56, $57, $59
.db $5a, $5c, $5d, $5f, $60, $62, $63, $65
.db $67, $68, $69, $6b, $6c, $6e, $70, $71
.db $73, $75, $76, $78, $79, $7b, $7c, $7e
.db $80, $82, $84, $85, $87, $88, $8a, $8b
.db $8d, $8f, $90, $92, $94, $95, $97, $98
.db $99, $9b, $9d, $9e, $a0, $a1, $a3, $a4
.db $a6, $a7, $a9, $aa, $ab, $ad, $ae, $b0
.db $b1, $b2, $b3, $b5, $b6, $b7, $b8, $ba
.db $bb, $bc, $bd, $bf, $c0, $c1, $c2, $c3
.db $c4, $c5, $c6, $c7, $c8, $c9, $ca, $cb
.db $cc, $cd, $ce, $cf, $cf, $d0, $d1, $d1
.db $d2, $d3, $d4, $d4, $d5, $d6, $d6, $d7
.db $d7, $d8, $d9, $d9, $d9, $da, $da, $db
.db $db, $dc, $dc, $dc, $dc, $dd, $dd, $dd
.db $de, $de, $de, $de, $de, $de, $de, $de
.db $de, $de, $de, $de, $de, $de, $de, $de
.db $de, $de, $de, $de, $de, $de, $de, $de
.db $de, $de, $de, $dd, $dd, $dd, $dd, $dd
.db $dd, $dc, $dc, $dc, $dc, $dc, $dc, $dc
.db $22, $22, $22, $22, $22, $22, $22, $22
.db $22, $22, $22, $22, $21, $21, $21, $21
.db $21, $21, $21, $21, $21, $21, $21, $21
.db $21, $21, $21, $22, $22, $22, $22, $22
.db $22, $22, $22, $22, $22, $22, $22, $23
.db $23, $23, $23, $24, $24, $24, $25, $25
.db $25, $26, $26, $27, $27, $28, $28, $29
.db $29, $2a, $2b, $2b, $2c, $2d, $2d, $2e
.db $2f, $30, $30, $31, $32, $32, $33, $34
.db $35, $36, $37, $38, $39, $3a, $3b, $3c
.db $3d, $3e, $3f, $40, $41, $42, $44, $45
.db $46, $47, $49, $4a, $4b, $4c, $4e, $4f
.db $50, $51, $53, $54, $55, $57, $58, $5a
.db $5b, $5d, $5e, $5f, $61, $62, $64, $65
.db $67, $68, $6a, $6b, $6d, $6e, $70, $71
.db $73, $75, $76, $78, $79, $7b, $7c, $7e
.db $80, $82, $84, $85, $87, $88, $8a, $8b
.db $8d, $8f, $90, $92, $93, $95, $96, $98
.db $99, $9b, $9c, $9e, $9f, $a1, $a2, $a3
.db $a5, $a6, $a8, $a9, $ab, $ac, $ad, $af
.db $b0, $b1, $b2, $b4, $b5, $b6, $b7, $b9
.db $ba, $bb, $bc, $be, $bf, $c0, $c1, $c2
.db $c3, $c4, $c5, $c6, $c7, $c8, $c9, $ca
.db $cb, $cc, $cd, $ce, $ce, $cf, $d0, $d0
.db $d1, $d2, $d3, $d3, $d4, $d5, $d5, $d6
.db $d7, $d7, $d8, $d8, $d9, $d9, $da, $da
.db $db, $db, $db, $dc, $dc, $dc, $dd, $dd
.db $dd, $dd, $de, $de, $de, $de, $de, $de
.db $de, $de, $de, $de, $de, $de, $df, $df
.db $df, $df, $df, $df, $df, $df, $df, $df
.db $df, $df, $df, $df, $df, $de, $de, $de
.db $de, $de, $de, $de, $de, $de, $de, $de
.db $20, $20, $20, $20, $20, $20, $20, $20
.db $20, $20, $20, $20, $20, $20, $20, $20
.db $20, $20, $20, $20, $20, $20, $21, $21
.db $21, $21, $21, $21, $21, $21, $22, $22
.db $22, $22, $22, $22, $22, $22, $23, $23
.db $23, $23, $24, $24, $25, $25, $25, $26
.db $26, $27, $27, $28, $28, $29, $29, $2a
.db $2a, $2b, $2c, $2c, $2d, $2e, $2e, $2f
.db $30, $31, $31, $32, $33, $34, $35, $35
.db $36, $37, $38, $39, $3a, $3b, $3c, $3d
.db $3e, $3f, $40, $41, $43, $44, $45, $46
.db $47, $48, $4a, $4b, $4c, $4d, $4f, $50
.db $51, $52, $54, $55, $56, $58, $59, $5b
.db $5c, $5d, $5f, $60, $62, $63, $65, $66
.db $68, $69, $6a, $6c, $6d, $6f, $70, $72
.db $74, $75, $77, $78, $7a, $7b, $7d, $7e
.db $80, $82, $83, $85, $86, $88, $89, $8b
.db $8c, $8e, $90, $91, $93, $94, $96, $97
.db $98, $9a, $9b, $9d, $9e, $a0, $a1, $a3
.db $a4, $a5, $a7, $a8, $aa, $ab, $ac, $ae
.db $af, $b0, $b1, $b3, $b4, $b5, $b6, $b8
.db $b9, $ba, $bb, $bc, $bd, $bf, $c0, $c1
.db $c2, $c3, $c4, $c5, $c6, $c7, $c8, $c9
.db $ca, $cb, $cb, $cc, $cd, $ce, $cf, $cf
.db $d0, $d1, $d2, $d2, $d3, $d4, $d4, $d5
.db $d6, $d6, $d7, $d7, $d8, $d8, $d9, $d9
.db $da, $da, $db, $db, $db, $dc, $dc, $dd
.db $dd, $dd, $dd, $de, $de, $de, $de, $de
.db $de, $de, $de, $df, $df, $df, $df, $df
.db $df, $df, $df, $e0, $e0, $e0, $e0, $e0
.db $e0, $e0, $e0, $e0, $e0, $e0, $e0, $e0
.db $e0, $e0, $e0, $e0, $e0, $e0, $e0, $e0
.db $1e, $1f, $1f, $1f, $1f, $1f, $1f, $1f
.db $1f, $1f, $1f, $1f, $1f, $1f, $1f, $1f
.db $1f, $20, $20, $20, $20, $20, $20, $20
.db $20, $21, $21, $21, $21, $21, $22, $22
.db $22, $22, $22, $22, $22, $22, $23, $23
.db $23, $24, $24, $24, $25, $25, $26, $26
.db $27, $27, $28, $28, $29, $29, $2a, $2b
.db $2b, $2c, $2c, $2d, $2e, $2e, $2f, $30
.db $31, $32, $32, $33, $34, $35, $36, $36
.db $37, $38, $39, $3a, $3b, $3c, $3d, $3e
.db $3f, $40, $41, $42, $44, $45, $46, $47
.db $48, $49, $4b, $4c, $4d, $4e, $4f, $51
.db $52, $53, $55, $56, $57, $59, $5a, $5b
.db $5c, $5e, $5f, $61, $62, $64, $65, $67
.db $68, $69, $6b, $6c, $6e, $6f, $71, $72
.db $74, $75, $77, $78, $7a, $7b, $7d, $7e
.db $80, $82, $83, $85, $86, $88, $89, $8b
.db $8c, $8e, $8f, $91, $92, $94, $95, $97
.db $98, $99, $9b, $9c, $9e, $9f, $a1, $a2
.db $a4, $a5, $a6, $a7, $a9, $aa, $ab, $ad
.db $ae, $af, $b1, $b2, $b3, $b4, $b5, $b7
.db $b8, $b9, $ba, $bb, $bc, $be, $bf, $c0
.db $c1, $c2, $c3, $c4, $c5, $c6, $c7, $c8
.db $c9, $ca, $ca, $cb, $cc, $cd, $ce, $ce
.db $cf, $d0, $d1, $d2, $d2, $d3, $d4, $d4
.db $d5, $d5, $d6, $d7, $d7, $d8, $d8, $d9
.db $d9, $da, $da, $db, $db, $dc, $dc, $dc
.db $dd, $dd, $dd, $de, $de, $de, $de, $de
.db $de, $de, $de, $df, $df, $df, $df, $df
.db $e0, $e0, $e0, $e0, $e0, $e0, $e0, $e0
.db $e1, $e1, $e1, $e1, $e1, $e1, $e1, $e1
.db $e1, $e1, $e1, $e1, $e1, $e1, $e1, $e1
.db $1c, $1d, $1d, $1d, $1d, $1d, $1d, $1d
.db $1d, $1d, $1e, $1e, $1e, $1e, $1e, $1e
.db $1e, $1f, $1f, $1f, $1f, $1f, $1f, $20
.db $20, $20, $20, $21, $21, $21, $21, $21
.db $22, $22, $22, $22, $22, $23, $23, $23
.db $24, $24, $24, $25, $25, $26, $26, $27
.db $27, $28, $28, $29, $29, $2a, $2b, $2b
.db $2c, $2c, $2d, $2e, $2f, $2f, $30, $31
.db $32, $33, $33, $34, $35, $36, $37, $37
.db $38, $39, $3a, $3b, $3c, $3d, $3e, $3f
.db $40, $41, $42, $43, $45, $46, $47, $48
.db $49, $4a, $4c, $4d, $4e, $4f, $50, $52
.db $53, $54, $55, $57, $58, $59, $5b, $5c
.db $5d, $5f, $60, $61, $63, $64, $66, $67
.db $69, $6a, $6b, $6d, $6e, $6f, $71, $72
.db $74, $75, $77, $78, $7a, $7b, $7d, $7e
.db $80, $82, $83, $85, $86, $88, $89, $8b
.db $8c, $8e, $8f, $91, $92, $93, $95, $96
.db $97, $99, $9a, $9c, $9d, $9f, $a0, $a1
.db $a3, $a4, $a5, $a7, $a8, $a9, $ab, $ac
.db $ad, $ae, $b0, $b1, $b2, $b3, $b4, $b6
.db $b7, $b8, $b9, $ba, $bb, $bd, $be, $bf
.db $c0, $c1, $c2, $c3, $c4, $c5, $c6, $c7
.db $c8, $c9, $c9, $ca, $cb, $cc, $cd, $cd
.db $ce, $cf, $d0, $d1, $d1, $d2, $d3, $d4
.db $d4, $d5, $d5, $d6, $d7, $d7, $d8, $d8
.db $d9, $d9, $da, $da, $db, $db, $dc, $dc
.db $dc, $dd, $dd, $dd, $de, $de, $de, $de
.db $de, $df, $df, $df, $df, $df, $e0, $e0
.db $e0, $e0, $e1, $e1, $e1, $e1, $e1, $e1
.db $e2, $e2, $e2, $e2, $e2, $e2, $e2, $e3
.db $e3, $e3, $e3, $e3, $e3, $e3, $e3, $e3
.db $1a, $1b, $1b, $1b, $1b, $1b, $1c, $1c
.db $1c, $1c, $1c, $1c, $1d, $1d, $1d, $1d
.db $1d, $1e, $1e, $1e, $1e, $1e, $1f, $1f
.db $1f, $20, $20, $20, $20, $21, $21, $21
.db $22, $22, $22, $22, $22, $23, $23, $23
.db $24, $24, $25, $25, $26, $26, $27, $27
.db $28, $28, $29, $2a, $2a, $2b, $2b, $2c
.db $2d, $2d, $2e, $2f, $30, $30, $31, $32
.db $33, $34, $34, $35, $36, $37, $38, $38
.db $39, $3a, $3b, $3c, $3d, $3e, $3f, $40
.db $41, $42, $43, $44, $46, $47, $48, $49
.db $4a, $4b, $4d, $4e, $4f, $50, $51, $53
.db $54, $55, $56, $58, $59, $5a, $5b, $5d
.db $5e, $5f, $61, $62, $63, $65, $66, $68
.db $69, $6a, $6c, $6d, $6e, $70, $71, $73
.db $74, $76, $77, $78, $7a, $7b, $7d, $7e
.db $80, $82, $83, $85, $86, $88, $89, $8a
.db $8c, $8d, $8f, $90, $92, $93, $94, $96
.db $97, $98, $9a, $9b, $9d, $9e, $9f, $a1
.db $a2, $a3, $a5, $a6, $a7, $a8, $aa, $ab
.db $ac, $ad, $af, $b0, $b1, $b2, $b3, $b5
.db $b6, $b7, $b8, $b9, $ba, $bc, $bd, $be
.db $bf, $c0, $c1, $c2, $c3, $c4, $c5, $c6
.db $c7, $c8, $c8, $c9, $ca, $cb, $cc, $cc
.db $cd, $ce, $cf, $d0, $d0, $d1, $d2, $d3
.db $d3, $d4, $d5, $d5, $d6, $d6, $d7, $d8
.db $d8, $d9, $d9, $da, $da, $db, $db, $dc
.db $dc, $dd, $dd, $dd, $de, $de, $de, $de
.db $de, $df, $df, $df, $e0, $e0, $e0, $e0
.db $e1, $e1, $e1, $e2, $e2, $e2, $e2, $e2
.db $e3, $e3, $e3, $e3, $e3, $e4, $e4, $e4
.db $e4, $e4, $e4, $e5, $e5, $e5, $e5, $e5
.db $18, $19, $19, $19, $19, $1a, $1a, $1a
.db $1a, $1b, $1b, $1b, $1b, $1c, $1c, $1c
.db $1c, $1d, $1d, $1d, $1d, $1e, $1e, $1e
.db $1f, $1f, $1f, $20, $20, $20, $21, $21
.db $22, $22, $22, $22, $22, $23, $23, $24
.db $24, $25, $25, $26, $26, $27, $27, $28
.db $28, $29, $2a, $2a, $2b, $2c, $2c, $2d
.db $2e, $2e, $2f, $30, $30, $31, $32, $33
.db $34, $35, $35, $36, $37, $38, $39, $3a
.db $3b, $3c, $3c, $3d, $3e, $3f, $40, $41
.db $42, $43, $45, $46, $47, $48, $49, $4a
.db $4b, $4c, $4e, $4f, $50, $51, $52, $53
.db $55, $56, $57, $58, $5a, $5b, $5c, $5e
.db $5f, $60, $61, $63, $64, $65, $67, $68
.db $6a, $6b, $6c, $6d, $6f, $70, $72, $73
.db $75, $76, $77, $79, $7a, $7b, $7d, $7e
.db $80, $82, $83, $85, $86, $87, $89, $8a
.db $8b, $8d, $8e, $90, $91, $93, $94, $95
.db $96, $98, $99, $9b, $9c, $9d, $9f, $a0
.db $a1, $a2, $a4, $a5, $a6, $a8, $a9, $aa
.db $ab, $ad, $ae, $af, $b0, $b1, $b2, $b4
.db $b5, $b6, $b7, $b8, $b9, $ba, $bb, $bd
.db $be, $bf, $c0, $c1, $c2, $c3, $c4, $c4
.db $c5, $c6, $c7, $c8, $c9, $ca, $cb, $cb
.db $cc, $cd, $ce, $cf, $d0, $d0, $d1, $d2
.db $d2, $d3, $d4, $d4, $d5, $d6, $d6, $d7
.db $d8, $d8, $d9, $d9, $da, $da, $db, $db
.db $dc, $dc, $dd, $dd, $de, $de, $de, $de
.db $de, $df, $df, $e0, $e0, $e0, $e1, $e1
.db $e1, $e2, $e2, $e2, $e3, $e3, $e3, $e3
.db $e4, $e4, $e4, $e4, $e5, $e5, $e5, $e5
.db $e6, $e6, $e6, $e6, $e7, $e7, $e7, $e7
.db $16, $17, $17, $17, $18, $18, $18, $19
.db $19, $19, $19, $1a, $1a, $1a, $1b, $1b
.db $1b, $1c, $1c, $1c, $1d, $1d, $1e, $1e
.db $1e, $1f, $1f, $1f, $20, $20, $21, $21
.db $22, $22, $22, $22, $22, $23, $23, $24
.db $24, $25, $26, $26, $27, $27, $28, $28
.db $29, $2a, $2a, $2b, $2c, $2c, $2d, $2e
.db $2e, $2f, $30, $31, $31, $32, $33, $34
.db $35, $36, $36, $37, $38, $39, $3a, $3b
.db $3c, $3d, $3d, $3e, $3f, $40, $41, $42
.db $43, $44, $46, $47, $48, $49, $4a, $4b
.db $4c, $4d, $4f, $50, $51, $52, $53, $54
.db $56, $57, $58, $59, $5a, $5c, $5d, $5e
.db $5f, $61, $62, $63, $65, $66, $67, $69
.db $6a, $6b, $6d, $6e, $6f, $71, $72, $73
.db $75, $76, $77, $79, $7a, $7b, $7d, $7e
.db $80, $82, $83, $85, $86, $87, $89, $8a
.db $8b, $8d, $8e, $8f, $91, $92, $93, $95
.db $96, $97, $99, $9a, $9b, $9d, $9e, $9f
.db $a1, $a2, $a3, $a4, $a6, $a7, $a8, $a9
.db $aa, $ac, $ad, $ae, $af, $b0, $b1, $b3
.db $b4, $b5, $b6, $b7, $b8, $b9, $ba, $bc
.db $bd, $be, $bf, $c0, $c1, $c2, $c3, $c3
.db $c4, $c5, $c6, $c7, $c8, $c9, $ca, $ca
.db $cb, $cc, $cd, $ce, $cf, $cf, $d0, $d1
.db $d2, $d2, $d3, $d4, $d4, $d5, $d6, $d6
.db $d7, $d8, $d8, $d9, $d9, $da, $da, $db
.db $dc, $dc, $dd, $dd, $de, $de, $de, $de
.db $de, $df, $df, $e0, $e0, $e1, $e1, $e1
.db $e2, $e2, $e2, $e3, $e3, $e4, $e4, $e4
.db $e5, $e5, $e5, $e6, $e6, $e6, $e7, $e7
.db $e7, $e7, $e8, $e8, $e8, $e9, $e9, $e9
.db $14, $15, $15, $16, $16, $16, $17, $17
.db $17, $18, $18, $18, $19, $19, $1a, $1a
.db $1a, $1b, $1b, $1c, $1c, $1c, $1d, $1d
.db $1e, $1e, $1f, $1f, $1f, $20, $21, $21
.db $21, $22, $22, $22, $23, $23, $24, $24
.db $25, $25, $26, $26, $27, $28, $28, $29
.db $2a, $2a, $2b, $2c, $2c, $2d, $2e, $2f
.db $2f, $30, $31, $31, $32, $33, $34, $35
.db $36, $37, $37, $38, $39, $3a, $3b, $3c
.db $3d, $3e, $3f, $40, $41, $42, $43, $44
.db $45, $46, $47, $48, $49, $4a, $4b, $4c
.db $4d, $4e, $50, $51, $52, $53, $54, $55
.db $56, $58, $59, $5a, $5b, $5d, $5e, $5f
.db $60, $61, $63, $64, $65, $67, $68, $69
.db $6b, $6c, $6d, $6e, $70, $71, $72, $74
.db $75, $76, $78, $79, $7a, $7c, $7d, $7e
.db $80, $82, $83, $84, $86, $87, $88, $8a
.db $8b, $8c, $8e, $8f, $90, $92, $93, $94
.db $95, $97, $98, $99, $9b, $9c, $9d, $9f
.db $a0, $a1, $a2, $a3, $a5, $a6, $a7, $a8
.db $aa, $ab, $ac, $ad, $ae, $af, $b0, $b2
.db $b3, $b4, $b5, $b6, $b7, $b8, $b9, $ba
.db $bb, $bc, $bd, $be, $bf, $c0, $c1, $c2
.db $c3, $c4, $c5, $c6, $c7, $c8, $c9, $c9
.db $ca, $cb, $cc, $cd, $ce, $cf, $cf, $d0
.db $d1, $d1, $d2, $d3, $d4, $d4, $d5, $d6
.db $d6, $d7, $d8, $d8, $d9, $da, $da, $db
.db $db, $dc, $dc, $dd, $dd, $de, $de, $de
.db $df, $df, $df, $e0, $e1, $e1, $e1, $e2
.db $e2, $e3, $e3, $e4, $e4, $e4, $e5, $e5
.db $e6, $e6, $e6, $e7, $e7, $e8, $e8, $e8
.db $e9, $e9, $e9, $ea, $ea, $ea, $eb, $eb
.db $12, $13, $13, $14, $14, $15, $15, $16
.db $16, $16, $17, $17, $18, $18, $18, $19
.db $19, $1a, $1a, $1b, $1b, $1c, $1c, $1d
.db $1d, $1e, $1e, $1f, $1f, $20, $20, $21
.db $21, $22, $22, $22, $23, $23, $24, $24
.db $25, $26, $26, $27, $28, $28, $29, $29
.db $2a, $2b, $2c, $2c, $2d, $2e, $2e, $2f
.db $30, $31, $32, $32, $33, $34, $35, $36
.db $37, $38, $38, $39, $3a, $3b, $3c, $3d
.db $3e, $3f, $40, $41, $42, $43, $44, $45
.db $46, $47, $48, $49, $4a, $4b, $4c, $4d
.db $4e, $4f, $51, $52, $53, $54, $55, $56
.db $57, $58, $5a, $5b, $5c, $5d, $5e, $60
.db $61, $62, $63, $65, $66, $67, $68, $6a
.db $6b, $6c, $6d, $6f, $70, $71, $73, $74
.db $75, $77, $78, $79, $7a, $7c, $7d, $7e
.db $80, $82, $83, $84, $86, $87, $88, $89
.db $8b, $8c, $8d, $8f, $90, $91, $93, $94
.db $95, $96, $98, $99, $9a, $9b, $9d, $9e
.db $9f, $a0, $a2, $a3, $a4, $a5, $a6, $a8
.db $a9, $aa, $ab, $ac, $ad, $ae, $af, $b1
.db $b2, $b3, $b4, $b5, $b6, $b7, $b8, $b9
.db $ba, $bb, $bc, $bd, $be, $bf, $c0, $c1
.db $c2, $c3, $c4, $c5, $c6, $c7, $c8, $c8
.db $c9, $ca, $cb, $cc, $cd, $ce, $ce, $cf
.db $d0, $d1, $d2, $d2, $d3, $d4, $d4, $d5
.db $d6, $d7, $d7, $d8, $d8, $d9, $da, $da
.db $db, $dc, $dc, $dd, $dd, $de, $de, $de
.db $df, $df, $e0, $e0, $e1, $e1, $e2, $e2
.db $e3, $e3, $e4, $e4, $e5, $e5, $e6, $e6
.db $e7, $e7, $e8, $e8, $e8, $e9, $e9, $ea
.db $ea, $ea, $eb, $eb, $ec, $ec, $ed, $ed
.db $10, $11, $11, $12, $12, $13, $13, $14
.db $14, $15, $15, $16, $16, $17, $17, $18
.db $18, $19, $19, $1a, $1a, $1b, $1c, $1c
.db $1d, $1d, $1e, $1e, $1f, $1f, $20, $21
.db $21, $22, $22, $22, $23, $23, $24, $25
.db $25, $26, $27, $27, $28, $29, $29, $2a
.db $2b, $2c, $2c, $2d, $2e, $2f, $2f, $30
.db $31, $32, $33, $33, $34, $35, $36, $37
.db $38, $39, $39, $3a, $3b, $3c, $3d, $3e
.db $3f, $40, $41, $42, $43, $44, $45, $46
.db $47, $48, $49, $4a, $4b, $4c, $4d, $4e
.db $4f, $50, $52, $53, $54, $55, $56, $57
.db $58, $59, $5b, $5c, $5d, $5e, $5f, $61
.db $62, $63, $64, $65, $67, $68, $69, $6a
.db $6c, $6d, $6e, $6f, $70, $72, $73, $74
.db $76, $77, $78, $79, $7b, $7c, $7d, $7e
.db $80, $82, $83, $84, $85, $87, $88, $89
.db $8a, $8c, $8d, $8e, $90, $91, $92, $93
.db $94, $96, $97, $98, $99, $9b, $9c, $9d
.db $9e, $9f, $a1, $a2, $a3, $a4, $a5, $a7
.db $a8, $a9, $aa, $ab, $ac, $ad, $ae, $b0
.db $b1, $b2, $b3, $b4, $b5, $b6, $b7, $b8
.db $b9, $ba, $bb, $bc, $bd, $be, $bf, $c0
.db $c1, $c2, $c3, $c4, $c5, $c6, $c7, $c7
.db $c8, $c9, $ca, $cb, $cc, $cd, $cd, $ce
.db $cf, $d0, $d1, $d1, $d2, $d3, $d4, $d4
.db $d5, $d6, $d7, $d7, $d8, $d9, $d9, $da
.db $db, $db, $dc, $dd, $dd, $de, $de, $de
.db $df, $df, $e0, $e1, $e1, $e2, $e2, $e3
.db $e3, $e4, $e4, $e5, $e6, $e6, $e7, $e7
.db $e8, $e8, $e9, $e9, $ea, $ea, $eb, $eb
.db $ec, $ec, $ed, $ed, $ee, $ee, $ef, $ef
.db $0e, $0f, $10, $10, $11, $11, $12, $12
.db $13, $13, $14, $15, $15, $16, $16, $17
.db $17, $18, $19, $19, $1a, $1a, $1b, $1c
.db $1c, $1d, $1d, $1e, $1f, $1f, $20, $20
.db $21, $22, $22, $22, $23, $24, $24, $25
.db $26, $26, $27, $28, $28, $29, $2a, $2b
.db $2b, $2c, $2d, $2e, $2e, $2f, $30, $31
.db $32, $32, $33, $34, $35, $36, $37, $38
.db $39, $3a, $3a, $3b, $3c, $3d, $3e, $3f
.db $40, $41, $42, $43, $44, $45, $46, $47
.db $48, $49, $4a, $4b, $4c, $4d, $4e, $4f
.db $50, $51, $53, $54, $55, $56, $57, $58
.db $59, $5a, $5b, $5d, $5e, $5f, $60, $61
.db $62, $64, $65, $66, $67, $68, $6a, $6b
.db $6c, $6d, $6e, $70, $71, $72, $73, $74
.db $76, $77, $78, $79, $7b, $7c, $7d, $7e
.db $80, $82, $83, $84, $85, $87, $88, $89
.db $8a, $8c, $8d, $8e, $8f, $90, $92, $93
.db $94, $95, $96, $98, $99, $9a, $9b, $9c
.db $9e, $9f, $a0, $a1, $a2, $a3, $a5, $a6
.db $a7, $a8, $a9, $aa, $ab, $ac, $ad, $af
.db $b0, $b1, $b2, $b3, $b4, $b5, $b6, $b7
.db $b8, $b9, $ba, $bb, $bc, $bd, $be, $bf
.db $c0, $c1, $c2, $c3, $c4, $c5, $c6, $c6
.db $c7, $c8, $c9, $ca, $cb, $cc, $cd, $ce
.db $ce, $cf, $d0, $d1, $d2, $d2, $d3, $d4
.db $d5, $d5, $d6, $d7, $d8, $d8, $d9, $da
.db $da, $db, $dc, $dc, $dd, $de, $de, $de
.db $df, $e0, $e0, $e1, $e1, $e2, $e3, $e3
.db $e4, $e4, $e5, $e6, $e6, $e7, $e7, $e8
.db $e9, $e9, $ea, $ea, $eb, $eb, $ec, $ed
.db $ed, $ee, $ee, $ef, $ef, $f0, $f0, $f1
.db $0c, $0d, $0e, $0e, $0f, $10, $10, $11
.db $11, $12, $13, $13, $14, $15, $15, $16
.db $16, $17, $18, $18, $19, $1a, $1a, $1b
.db $1c, $1c, $1d, $1e, $1e, $1f, $20, $20
.db $21, $22, $22, $22, $23, $24, $24, $25
.db $26, $27, $27, $28, $29, $2a, $2a, $2b
.db $2c, $2d, $2e, $2e, $2f, $30, $31, $32
.db $33, $33, $34, $35, $36, $37, $38, $39
.db $3a, $3b, $3b, $3c, $3d, $3e, $3f, $40
.db $41, $42, $43, $44, $45, $46, $47, $48
.db $49, $4a, $4b, $4c, $4d, $4e, $4f, $50
.db $51, $52, $54, $55, $56, $57, $58, $59
.db $5a, $5b, $5c, $5d, $5f, $60, $61, $62
.db $63, $64, $65, $67, $68, $69, $6a, $6b
.db $6d, $6e, $6f, $70, $71, $72, $74, $75
.db $76, $77, $78, $7a, $7b, $7c, $7d, $7e
.db $80, $82, $83, $84, $85, $86, $88, $89
.db $8a, $8b, $8c, $8e, $8f, $90, $91, $92
.db $93, $95, $96, $97, $98, $99, $9b, $9c
.db $9d, $9e, $9f, $a0, $a1, $a3, $a4, $a5
.db $a6, $a7, $a8, $a9, $aa, $ab, $ac, $ae
.db $af, $b0, $b1, $b2, $b3, $b4, $b5, $b6
.db $b7, $b8, $b9, $ba, $bb, $bc, $bd, $be
.db $bf, $c0, $c1, $c2, $c3, $c4, $c5, $c5
.db $c6, $c7, $c8, $c9, $ca, $cb, $cc, $cd
.db $cd, $ce, $cf, $d0, $d1, $d2, $d2, $d3
.db $d4, $d5, $d6, $d6, $d7, $d8, $d9, $d9
.db $da, $db, $dc, $dc, $dd, $de, $de, $de
.db $df, $e0, $e0, $e1, $e2, $e2, $e3, $e4
.db $e4, $e5, $e6, $e6, $e7, $e8, $e8, $e9
.db $ea, $ea, $eb, $eb, $ec, $ed, $ed, $ee
.db $ef, $ef, $f0, $f0, $f1, $f2, $f2, $f3
.db $0a, $0b, $0c, $0d, $0d, $0e, $0f, $0f
.db $10, $11, $11, $12, $13, $13, $14, $15
.db $15, $16, $17, $18, $18, $19, $1a, $1a
.db $1b, $1c, $1d, $1d, $1e, $1f, $20, $20
.db $21, $22, $22, $22, $23, $24, $25, $25
.db $26, $27, $28, $28, $29, $2a, $2b, $2c
.db $2d, $2d, $2e, $2f, $30, $31, $32, $33
.db $33, $34, $35, $36, $37, $38, $39, $3a
.db $3b, $3c, $3c, $3d, $3e, $3f, $40, $41
.db $42, $43, $44, $45, $46, $47, $48, $49
.db $4a, $4b, $4c, $4d, $4e, $4f, $50, $51
.db $52, $53, $55, $56, $57, $58, $59, $5a
.db $5b, $5c, $5d, $5e, $5f, $61, $62, $63
.db $64, $65, $66, $67, $68, $6a, $6b, $6c
.db $6d, $6e, $6f, $70, $72, $73, $74, $75
.db $76, $77, $79, $7a, $7b, $7c, $7d, $7e
.db $80, $82, $83, $84, $85, $86, $87, $89
.db $8a, $8b, $8c, $8d, $8e, $90, $91, $92
.db $93, $94, $95, $96, $98, $99, $9a, $9b
.db $9c, $9d, $9e, $9f, $a1, $a2, $a3, $a4
.db $a5, $a6, $a7, $a8, $a9, $aa, $ab, $ad
.db $ae, $af, $b0, $b1, $b2, $b3, $b4, $b5
.db $b6, $b7, $b8, $b9, $ba, $bb, $bc, $bd
.db $be, $bf, $c0, $c1, $c2, $c3, $c4, $c4
.db $c5, $c6, $c7, $c8, $c9, $ca, $cb, $cc
.db $cd, $cd, $ce, $cf, $d0, $d1, $d2, $d3
.db $d3, $d4, $d5, $d6, $d7, $d8, $d8, $d9
.db $da, $db, $db, $dc, $dd, $de, $de, $de
.db $df, $e0, $e0, $e1, $e2, $e3, $e3, $e4
.db $e5, $e6, $e6, $e7, $e8, $e8, $e9, $ea
.db $eb, $eb, $ec, $ed, $ed, $ee, $ef, $ef
.db $f0, $f1, $f1, $f2, $f3, $f3, $f4, $f5
.db $08, $09, $0a, $0b, $0b, $0c, $0d, $0e
.db $0e, $0f, $10, $11, $11, $12, $13, $14
.db $14, $15, $16, $17, $17, $18, $19, $1a
.db $1b, $1b, $1c, $1d, $1e, $1e, $1f, $20
.db $21, $22, $22, $22, $23, $24, $25, $26
.db $26, $27, $28, $29, $2a, $2b, $2b, $2c
.db $2d, $2e, $2f, $30, $31, $32, $32, $33
.db $34, $35, $36, $37, $38, $39, $3a, $3b
.db $3c, $3d, $3d, $3e, $3f, $40, $41, $42
.db $43, $44, $45, $46, $47, $48, $49, $4a
.db $4b, $4c, $4d, $4e, $4f, $50, $51, $52
.db $53, $54, $56, $57, $58, $59, $5a, $5b
.db $5c, $5d, $5e, $5f, $60, $61, $62, $64
.db $65, $66, $67, $68, $69, $6a, $6b, $6c
.db $6e, $6f, $70, $71, $72, $73, $74, $75
.db $77, $78, $79, $7a, $7b, $7c, $7d, $7e
.db $80, $82, $83, $84, $85, $86, $87, $88
.db $89, $8b, $8c, $8d, $8e, $8f, $90, $91
.db $92, $94, $95, $96, $97, $98, $99, $9a
.db $9b, $9c, $9e, $9f, $a0, $a1, $a2, $a3
.db $a4, $a5, $a6, $a7, $a8, $a9, $aa, $ac
.db $ad, $ae, $af, $b0, $b1, $b2, $b3, $b4
.db $b5, $b6, $b7, $b8, $b9, $ba, $bb, $bc
.db $bd, $be, $bf, $c0, $c1, $c2, $c3, $c3
.db $c4, $c5, $c6, $c7, $c8, $c9, $ca, $cb
.db $cc, $cd, $ce, $ce, $cf, $d0, $d1, $d2
.db $d3, $d4, $d5, $d5, $d6, $d7, $d8, $d9
.db $da, $da, $db, $dc, $dd, $de, $de, $de
.db $df, $e0, $e1, $e2, $e2, $e3, $e4, $e5
.db $e5, $e6, $e7, $e8, $e9, $e9, $ea, $eb
.db $ec, $ec, $ed, $ee, $ef, $ef, $f0, $f1
.db $f2, $f2, $f3, $f4, $f5, $f5, $f6, $f7
.db $06, $07, $08, $09, $0a, $0b, $0b, $0c
.db $0d, $0e, $0f, $0f, $10, $11, $12, $13
.db $13, $14, $15, $16, $17, $18, $18, $19
.db $1a, $1b, $1c, $1d, $1d, $1e, $1f, $20
.db $21, $22, $22, $22, $23, $24, $25, $26
.db $27, $28, $28, $29, $2a, $2b, $2c, $2d
.db $2e, $2f, $30, $30, $31, $32, $33, $34
.db $35, $36, $37, $38, $39, $3a, $3b, $3c
.db $3d, $3e, $3e, $3f, $40, $41, $42, $43
.db $44, $45, $46, $47, $48, $49, $4a, $4b
.db $4c, $4d, $4e, $4f, $50, $51, $52, $53
.db $54, $55, $57, $58, $59, $5a, $5b, $5c
.db $5d, $5e, $5f, $60, $61, $62, $63, $64
.db $65, $66, $67, $69, $6a, $6b, $6c, $6d
.db $6e, $6f, $70, $71, $72, $73, $75, $76
.db $77, $78, $79, $7a, $7b, $7c, $7d, $7e
.db $80, $82, $83, $84, $85, $86, $87, $88
.db $89, $8a, $8b, $8d, $8e, $8f, $90, $91
.db $92, $93, $94, $95, $96, $97, $99, $9a
.db $9b, $9c, $9d, $9e, $9f, $a0, $a1, $a2
.db $a3, $a4, $a5, $a6, $a7, $a8, $a9, $ab
.db $ac, $ad, $ae, $af, $b0, $b1, $b2, $b3
.db $b4, $b5, $b6, $b7, $b8, $b9, $ba, $bb
.db $bc, $bd, $be, $bf, $c0, $c1, $c2, $c2
.db $c3, $c4, $c5, $c6, $c7, $c8, $c9, $ca
.db $cb, $cc, $cd, $ce, $cf, $d0, $d0, $d1
.db $d2, $d3, $d4, $d5, $d6, $d7, $d8, $d8
.db $d9, $da, $db, $dc, $dd, $de, $de, $de
.db $df, $e0, $e1, $e2, $e3, $e3, $e4, $e5
.db $e6, $e7, $e8, $e8, $e9, $ea, $eb, $ec
.db $ed, $ed, $ee, $ef, $f0, $f1, $f1, $f2
.db $f3, $f4, $f5, $f5, $f6, $f7, $f8, $f9
.db $04, $05, $06, $07, $08, $09, $0a, $0b
.db $0b, $0c, $0d, $0e, $0f, $10, $11, $12
.db $12, $13, $14, $15, $16, $17, $18, $19
.db $1a, $1a, $1b, $1c, $1d, $1e, $1f, $20
.db $21, $22, $22, $22, $23, $24, $25, $26
.db $27, $28, $29, $2a, $2b, $2c, $2c, $2d
.db $2e, $2f, $30, $31, $32, $33, $34, $35
.db $36, $37, $38, $39, $3a, $3b, $3c, $3d
.db $3e, $3f, $3f, $40, $41, $42, $43, $44
.db $45, $46, $47, $48, $49, $4a, $4b, $4c
.db $4d, $4e, $4f, $50, $51, $52, $53, $54
.db $55, $56, $58, $59, $5a, $5b, $5c, $5d
.db $5e, $5f, $60, $61, $62, $63, $64, $65
.db $66, $67, $68, $69, $6a, $6b, $6c, $6d
.db $6f, $70, $71, $72, $73, $74, $75, $76
.db $77, $78, $79, $7a, $7b, $7c, $7d, $7e
.db $80, $82, $83, $84, $85, $86, $87, $88
.db $89, $8a, $8b, $8c, $8d, $8e, $8f, $90
.db $91, $93, $94, $95, $96, $97, $98, $99
.db $9a, $9b, $9c, $9d, $9e, $9f, $a0, $a1
.db $a2, $a3, $a4, $a5, $a6, $a7, $a8, $aa
.db $ab, $ac, $ad, $ae, $af, $b0, $b1, $b2
.db $b3, $b4, $b5, $b6, $b7, $b8, $b9, $ba
.db $bb, $bc, $bd, $be, $bf, $c0, $c1, $c1
.db $c2, $c3, $c4, $c5, $c6, $c7, $c8, $c9
.db $ca, $cb, $cc, $cd, $ce, $cf, $d0, $d1
.db $d2, $d3, $d4, $d4, $d5, $d6, $d7, $d8
.db $d9, $da, $db, $dc, $dd, $de, $de, $de
.db $df, $e0, $e1, $e2, $e3, $e4, $e5, $e6
.db $e6, $e7, $e8, $e9, $ea, $eb, $ec, $ed
.db $ee, $ee, $ef, $f0, $f1, $f2, $f3, $f4
.db $f5, $f5, $f6, $f7, $f8, $f9, $fa, $fb
.db $02, $03, $04, $05, $06, $07, $08, $09
.db $0a, $0b, $0c, $0d, $0e, $0f, $10, $11
.db $11, $12, $13, $14, $15, $16, $17, $18
.db $19, $1a, $1b, $1c, $1d, $1e, $1f, $20
.db $21, $22, $22, $22, $23, $24, $25, $26
.db $27, $28, $29, $2a, $2b, $2c, $2d, $2e
.db $2f, $30, $31, $32, $33, $34, $35, $36
.db $37, $38, $39, $3a, $3b, $3c, $3d, $3e
.db $3f, $40, $40, $41, $42, $43, $44, $45
.db $46, $47, $48, $49, $4a, $4b, $4c, $4d
.db $4e, $4f, $50, $51, $52, $53, $54, $55
.db $56, $57, $59, $5a, $5b, $5c, $5d, $5e
.db $5f, $60, $61, $62, $63, $64, $65, $66
.db $67, $68, $69, $6a, $6b, $6c, $6d, $6e
.db $6f, $70, $71, $72, $73, $74, $75, $76
.db $77, $78, $79, $7a, $7b, $7c, $7d, $7e
.db $80, $82, $83, $84, $85, $86, $87, $88
.db $89, $8a, $8b, $8c, $8d, $8e, $8f, $90
.db $91, $92, $93, $94, $95, $96, $97, $98
.db $99, $9a, $9b, $9c, $9d, $9e, $9f, $a0
.db $a1, $a2, $a3, $a4, $a5, $a6, $a7, $a9
.db $aa, $ab, $ac, $ad, $ae, $af, $b0, $b1
.db $b2, $b3, $b4, $b5, $b6, $b7, $b8, $b9
.db $ba, $bb, $bc, $bd, $be, $bf, $c0, $c0
.db $c1, $c2, $c3, $c4, $c5, $c6, $c7, $c8
.db $c9, $ca, $cb, $cc, $cd, $ce, $cf, $d0
.db $d1, $d2, $d3, $d4, $d5, $d6, $d7, $d8
.db $d9, $da, $db, $dc, $dd, $de, $de, $de
.db $df, $e0, $e1, $e2, $e3, $e4, $e5, $e6
.db $e7, $e8, $e9, $ea, $eb, $ec, $ed, $ee
.db $ef, $ef, $f0, $f1, $f2, $f3, $f4, $f5
.db $f6, $f7, $f8, $f9, $fa, $fb, $fc, $fd
.db $00, $01, $02, $03, $04, $05, $06, $07
.db $08, $09, $0a, $0b, $0c, $0d, $0e, $0f
.db $10, $11, $12, $13, $14, $15, $16, $17
.db $18, $19, $1a, $1b, $1c, $1d, $1e, $1f
.db $20, $21, $22, $23, $24, $25, $26, $27
.db $28, $29, $2a, $2b, $2c, $2d, $2e, $2f
.db $30, $31, $32, $33, $34, $35, $36, $37
.db $38, $39, $3a, $3b, $3c, $3d, $3e, $3f
.db $40, $41, $42, $43, $44, $45, $46, $47
.db $48, $49, $4a, $4b, $4c, $4d, $4e, $4f
.db $50, $51, $52, $53, $54, $55, $56, $57
.db $58, $59, $5a, $5b, $5c, $5d, $5e, $5f
.db $60, $61, $62, $63, $64, $65, $66, $67
.db $68, $69, $6a, $6b, $6c, $6d, $6e, $6f
.db $70, $71, $72, $73, $74, $75, $76, $77
.db $78, $79, $7a, $7b, $7c, $7d, $7e, $7f
.db $80, $81, $82, $83, $84, $85, $86, $87
.db $88, $89, $8a, $8b, $8c, $8d, $8e, $8f
.db $90, $91, $92, $93, $94, $95, $96, $97
.db $98, $99, $9a, $9b, $9c, $9d, $9e, $9f
.db $a0, $a1, $a2, $a3, $a4, $a5, $a6, $a7
.db $a8, $a9, $aa, $ab, $ac, $ad, $ae, $af
.db $b0, $b1, $b2, $b3, $b4, $b5, $b6, $b7
.db $b8, $b9, $ba, $bb, $bc, $bd, $be, $bf
.db $c0, $c1, $c2, $c3, $c4, $c5, $c6, $c7
.db $c8, $c9, $ca, $cb, $cc, $cd, $ce, $cf
.db $d0, $d1, $d2, $d3, $d4, $d5, $d6, $d7
.db $d8, $d9, $da, $db, $dc, $dd, $de, $df
.db $e0, $e1, $e2, $e3, $e4, $e5, $e6, $e7
.db $e8, $e9, $ea, $eb, $ec, $ed, $ee, $ef
.db $f0, $f1, $f2, $f3, $f4, $f5, $f6, $f7
.db $f8, $f9, $fa, $fb, $fc, $fd, $fe, $ff

.endif

