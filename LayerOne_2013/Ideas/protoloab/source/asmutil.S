; Xegalab / XMultiKit: AVR XMEGA Oscilloscope and Development Kit
;
; Gabotronics C.A.
; July 2010
;
; Copyright 2010 Gabriel Anzziani
;
; This program is distributed under the terms of the GNU General Public License 
;
; www.gabotronics.com
; email me at: gabriel@gabotronics.com

#include <avr/io.h>

; 8:8 Fixed Point Multiplication
.global fixmult
.func fixmult
fixmult:
    push r20
    push r21
    movw  r20,r24
    
    muls	r23, r21	; (signed)ah * (signed)bh
    mov		r31, r0		;
    mul		r22, r20	; al * bl
    mov 	r30, r1		;
    ;mov	r16, r0
    mulsu	r23, r20	; (signed)ah * bl
    add		r30, r0     ;
    adc		r31, r1     ;
    mulsu	r21, r22	; (signed)bh * al
    add		r30, r0     ;
    adc		r31, r1     ;
	
    movw    r24,r30
    clr r1
    pop r21
    pop r20
    ret
.endfunc


; trigger on comparator a
;----------------------------------------------------------------------------;
;
.global trigcompa
.func trigcompa
trigcompa:
    lds     R0, 0x0387  ; ACA Status
    sts     0x0387, R0  ; Clear interrupt flag
trigcompa1:
	sbic	0x0000,0	; 2 cycle ; Status update? (GPIO0 bit 0)
	rjmp	digicanceltriga
    lds     R0, 0x0387  ; ACA Status
    sbrs    R0,0
	rjmp	trigcompa1
	sbi		0x0001, 4  ; Set trigger (GPIO1 bit 4)

digicanceltriga:
    ret
.endfunc

; trigger on comparator b
;----------------------------------------------------------------------------;
;
.global trigcompb
.func trigcompb
trigcompb:
    lds     R0, 0x0397  ; ACB Status
    sts     0x0397, R0  ; Clear interrupt flag
trigcompb1:
	sbic	0x0000,0	; 2 cycle ; Status update? (GPIO0 bit 0)
	rjmp	digicanceltrigb
    lds     R0, 0x0397  ; ACB Status
    sbrs    R0,0
	rjmp	trigcompb1
	sbi		0x0001, 4  ; Set trigger (GPIO1 bit 4)

digicanceltrigb:
    ret
.endfunc

; trigger on ext
;----------------------------------------------------------------------------;
;
.global trigext
.func trigext
trigext:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigext
    sbic    0x0016,1        ; Check if bit VPORT1.1 is low
	rjmp	trigext
trigextb:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigext
    sbis    0x0016,1        ; Check if bit VPORT1.1 is high
	rjmp	trigextb

	sbi		0x0001, 4       ; Set trigger (GPIO1)
canceltrigext:
    ret
.endfunc

; trigger on PB4
;----------------------------------------------------------------------------;
;
.global trigpb4
.func trigpb4
trigpb4:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigpb4
    sbic    0x0016,4        ; Check if bit VPORT1.4 is low
	rjmp	trigpb4
trigpb4b:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigpb4
    sbis    0x0016,4        ; Check if bit VPORT1.4 is high
	rjmp	trigpb4b

	sbi		0x0001, 4       ; Set trigger (GPIO1)
canceltrigpb4:
    ret
.endfunc

; trigger on PB5
;----------------------------------------------------------------------------;
;
.global trigpb5
.func trigpb5
trigpb5:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigpb5
    sbic    0x0016,5        ; Check if bit VPORT1.4 is low
	rjmp	trigpb5
trigpb5b:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigpb5
    sbis    0x0016,5        ; Check if bit VPORT1.4 is high
	rjmp	trigpb5b

	sbi		0x0001, 4       ; Set trigger (GPIO1)
canceltrigpb5:
    ret
.endfunc

; trigger on PB6
;----------------------------------------------------------------------------;
;
.global trigpb6
.func trigpb6
trigpb6:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigpb6
    sbic    0x0016,6        ; Check if bit VPORT1.4 is low
	rjmp	trigpb6
trigpb6b:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigpb4
    sbis    0x0016,6        ; Check if bit VPORT1.4 is high
	rjmp	trigpb6b

	sbi		0x0001, 4       ; Set trigger (GPIO1)
canceltrigpb6:
    ret
.endfunc

; trigger on PB7
;----------------------------------------------------------------------------;
;
.global trigpb7
.func trigpb7
trigpb7:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigpb7
    sbic    0x0016,7        ; Check if bit VPORT1.4 is low
	rjmp	trigpb7
trigpb7b:
	sbic	0x0000,0	    ; 2 cycle ; Status update? (GPIO0)
	rjmp	canceltrigpb7
    sbis    0x0016,7        ; Check if bit VPORT1.4 is high
	rjmp	trigpb7b

	sbi		0x0001, 4       ; Set trigger (GPIO1)
canceltrigpb7:
    ret
.endfunc

; falling edge trigger detect
;----------------------------------------------------------------------------;
;
;                 data1  ___.
;                            \
;  triggervalue - - - - - - - \
;                              \.___ data2
;
.global trigdown
.func trigdown
trigdown:
    lds     R1, 0x0007 ; Load trigger level from GPIO7

trigcheck1d:
	in		r0,0x0012	; 1 cycle ; data1= Read VPO input
	cp		r1,r0		; 1 cycle ; compare triggervalue with data1
	brlo    trigcheck2d	; 1 cycle  ; triggervalue < data1 ?
	sbis	0x0000,0	; 1 cycle ; Status update? (GPIO0)
	rjmp	trigcheck1d	; 2 cycles
						; 6 cycles
	rjmp	canceltrigd

trigcheck2d:
	in		r0,0x0012	; 1 cycle ; data2= Read VPO input
	cp		r0,r1		; 1 cycle ; compare triggervalue with data2
	brlo    triggeredd	; 1 cycle  ; data2 < triggervalue ?
	sbis	0x0000,0	; 1 cycle ; Status update? (GPIO0)
	rjmp	trigcheck2d	; 2 cycles
						; 6 cycles	

	rjmp	canceltrigd

triggeredd:
	sbi		0x0001, 4  ; Set trigger (GPIO1)

canceltrigd:
    clr r1
    ret
.endfunc

; rising edge trigger detect
;----------------------------------------------------------------------------;
;
;                              .___ data2 
;                              /
;  triggervalue - - - - - - - /
;                  data1 ___./   
;
.global trigup
.func trigup
trigup:
    lds     R1, 0x0007 ; Load trigger level from GPIO7
    
trigcheck1u:
	in		r0,0x0012	; 1 cycle ; data1= Read VPO input
	cp		r0,r1		; 1 cycle ; compare triggervalue with data1
	brlo    trigcheck2u	; 1 cycle  ; triggervalue < data1 ?
	sbis	0x0000,0	; 2 cycle ; Status update? (GPIO0)
	rjmp	trigcheck1u	; 2 cycles
						; 7 cycles
	rjmp	canceltrigu

trigcheck2u:
	in		r0,0x0012	; 1 cycle ; data2= Read VPO input
	cp		r1,r0		; 1 cycle ; compare triggervalue with data2
	brlo    triggeredu	; 1 cycle  ; data2 < triggervalue ?
	sbis	0x0000,0	; 2 cycle ; Status update? (GPIO0)
	rjmp	trigcheck2u	; 2 cycles
						; 7 cycles	

	rjmp	canceltrigu

triggeredu:
	sbi		0x0001, 4  ; Set trigger (GPIO1)

canceltrigu:
    clr r1
    ret
.endfunc

;----------------------------------------------------------------------------;
.global acquire1    ; Acquires 256 samples on only one analog channel
.func acquire1      ; MSO set at 1uS/div  ->  62.5nS / pixel ->  16MS/s
acquire1:           ; input: r24:r25 (pointer to analog data buffer)
    push YL         ; Save YL
    push YH         ; Save YH
    movw YL, R24    ; Load pointer to Y, R24 can now be used to read the port

ac1:in   R24,0x0012   ; 0 0     Virtual Port 0 input (Mapped to PORTJ)
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 1
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 2
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 3
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 4
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 5
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 6
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 7
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 8
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 9
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 A
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 B
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 C
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 D
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 E
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R24,0x0012   ; 0 F
    ST   Y+,R24
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R24,0x0012   ; F
    ST   Y+,R24

	sbi	0x0001, 7  ; Set data acquired (GPIO1)
    pop YH
    pop YL
	ret
.endfunc

;----------------------------------------------------------------------------;
.global acquire2    ; Acquires 256 samples on only one analog channel and the digital
.func acquire2      ; MSO set at 2uS/div  ->  125nS / pixel ->  8 MS/s
acquire2:           ; input: r22:r23, r24:r25 (pointers to Analog and Digital buffers)
    push R28        ; Save YL
    push R29        ; Save YH
    push R30        ; Save ZL
    push R31        ; Save ZH
    movw YL, R24    ; Load pointer to Y, R24 can now be used to read the port
    movw ZL, R22    ; Load pointer to Z, R22 can now be used to read the port

    ; ADC is delayed 5 samples, so read 5 digital inputs first to be in sync
ac2:in   R22,0x0016  ; Virtual Port 1 input (Mapped to PORTB)
    ST   Z+,R22
    rjmp 1f		    ; 2 cycle delay
1:  in   R22,0x0016
    ST   Z+,R22
    rjmp 2f		    ; 2 cycle delay
2:  in   R22,0x0016
    ST   Z+,R22
    rjmp 3f		    ; 2 cycle delay
3:  in   R22,0x0016
    ST   Z+,R22
    rjmp 4f		    ; 2 cycle delay
4:  in   R22,0x0016
    ST   Z+,R22
	; cycles
    in   R24,0x0012   ; 0 0     1 cycle
    ST   Y+,R24		  ;         1 cycle
    in   R22,0x0016	  ;         1 cycle
    ST   Z+,R22		  ;         1 cycle
					  ;         4 cycles total for one set of samples (analog+digital)
					  ;         32Mhz / 4 cycles = 8MHz
    in   R24,0x0012   ; 1       Virtual Port 0 input (Mapped to PORTJ)
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; B
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; C
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; D
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; E
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 0 F
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 1
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 2
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 3
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 4
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 5
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 6
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 7
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 8
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; 9
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22
    in   R24,0x0012   ; A
    ST   Y+,R24
    in   R22,0x0016
    ST   Z+,R22

    ; Last 5 analog inputs
    in   R24,0x0012   ; B
    ST   Y+,R24
    rjmp 5f
5:  in   R24,0x0012   ; C
    ST   Y+,R24
    rjmp 6f
6:  in   R24,0x0012   ; D
    ST   Y+,R24
    rjmp 7f
7:  in   R24,0x0012   ; E
    ST   Y+,R24
    rjmp 8f
8:  in   R24,0x0012   ; F
    ST   Y+,R24

	sbi	0x0001, 7  ; Set data acquired (GPIO1)
    pop r31
    pop r30
    pop r29
    pop r28
	ret
.endfunc

;----------------------------------------------------------------------------;
.global acquire3    ; Acquires 256 samples on one analog channel and the digital
.func acquire3      ; MSO set at 5uS/div  ->  312.5nS / pixel ->  3.2 MS/s
acquire3:           ; input: r22:r23, r24:r25 (pointers to Analog and Digital buffers)
    push R28        ; Save YL
    push R29        ; Save YH
    push R30        ; Save ZL
    push R31        ; Save ZH
    movw YL, R24    ; Load pointer to Y, R24 can now be used to read the port
    movw ZL, R22    ; Load pointer to Z, R22 can now be used to read the port
    ldi  R25, 251   ; R25 will be used as the counter

    ; ADC is delayed 5 samples, so read 5 digital inputs first to be in sync
    in   R22,0x0016 ; 1 cycle
    ST   Z+,R22     ; 1 cycle
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    rjmp 9f 		; 2 cycle delay
                    ; 10 cycles total
9:  in   R22,0x0016
    ST   Z+,R22
    lpm
    lpm
    rjmp 10f
10: in   R22,0x0016
    ST   Z+,R22
    lpm
    lpm
    rjmp 11f
11: in   R22,0x0016
    ST   Z+,R22
    lpm
    lpm
    rjmp 12f
12: in   R22,0x0016
    ST   Z+,R22
; remaining 251 samples:
more:                   ; cycles:
    in   R24,0x0012     ; 1
    ST   Y+,R24         ; 1
    in   R22,0x0016     ; 1
    ST   Z+,R22         ; 1
    lpm                 ; 3
    dec  R25            ; 1
    brne more           ; 2
                        ; = 10 cycles total for one set of samples (analog+digital)
						; 32Mhz / 10 cycles = 3.2MHz

    nop
    ; Last 5 analog inputs
    in   R24,0x0012   ; B
    ST   Y+,R24
    lpm
    lpm
    rjmp 13f
13: in   R24,0x0012   ; C
    ST   Y+,R24
    lpm
    lpm
    rjmp 14f
14: in   R24,0x0012   ; D
    ST   Y+,R24
    lpm
    lpm
    rjmp 15f
15: in   R24,0x0012   ; E
    ST   Y+,R24
    lpm
    lpm
    rjmp 16f
16: in   R24,0x0012   ; F
    ST   Y+,R24

	sbi	0x0001, 7  ; Set data acquired (GPIO1)
    pop R31
    pop R30
    pop R29
    pop R28
	ret
.endfunc

;----------------------------------------------------------------------------;
.global acquire4    ; Acquires 256 samples on two analog channels and the digital
.func acquire4      ; MSO set at 10uS/div  -> (312.5nS / pixel) /2 channels ->  3.2 MS/s
acquire4:           ; input: R20:R21, R22:R23, R24:R25 (pointers to CH1, Digital and CH2 buffers)
    push R26        ; Save XL
    push R27        ; Save XH
    push R28        ; Save YL
    push R29        ; Save YH
    push R30        ; Save ZL
    push R31        ; Save ZH
    movw YL, R24    ; Load pointer to Y, R24 can now be used to read the port
    movw ZL, R22    ; Load pointer to Z, R22 can now be used to read the port
    movw XL, R20    ; Load pointer to X, R20 can now be used to read the port
    ldi  R25, 251   ; R25 will be used as the counter

wh1:sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp    wh1
wl1:sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp    wl1

    ; ADC is delayed 5 samples, so read 5 digital inputs first to be in sync
    in   R22,0x0016 ; 1 cycle
    ST   Z+,R22     ; 1 cycle
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
                    ; 20 cycles total
    in   R22,0x0016
    ST   Z+,R22
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    in   R22,0x0016
    ST   Z+,R22
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    in   R22,0x0016
    ST   Z+,R22
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    lpm				; 3 cycle delay
    in   R22,0x0016
    ST   Z+,R22
    lpm				; 3 cycle delay
; remaining 251 samples:
more4:                  ; cycles:
    in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
    lpm                 ; 3
    in   R22,0x0016     ; 1     Digital
    ST   Z+,R22         ; 1
    lpm                 ; 3
    in   R20,0x0012     ; 1     CH2
    ST   X+,R20         ; 1
    lpm                 ; 3
    rjmp 17f            ; 2
17: dec  R25            ; 1
    brne more4          ; 2
                        ; = 20 cycles total for one set of samples (2 analog + 1 digital)
						; 32Mhz / 10 cycles = 3.2MHz

    nop
    ; Last 5 analog inputs
    in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
    lpm                 ; 3
    lpm                 ; 3
    rjmp 18f
18: in   R24,0x0012     ;       CH2
    ST   X+,R24
    lpm
    lpm
    rjmp 19f

    ; Last 5 analog inputs
19: in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
    lpm                 ; 3
    lpm                 ; 3
    rjmp 20f
20: in   R24,0x0012     ;       CH2
    ST   X+,R24
    lpm
    lpm
    rjmp 21f

    ; Last 5 analog inputs
21: in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
    lpm                 ; 3
    lpm                 ; 3
    rjmp 22f
22: in   R24,0x0012     ;       CH2
    ST   X+,R24
    lpm
    lpm
    rjmp 23f

    ; Last 5 analog inputs
23: in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
    lpm                 ; 3
    lpm                 ; 3
    rjmp 24f
24: in   R24,0x0012     ;       CH2
    ST   X+,R24
    lpm
    lpm
    rjmp 25f

    ; Last 5 analog inputs
25: in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
    lpm                 ; 3
    lpm                 ; 3
    rjmp 26f
26: in   R24,0x0012     ;       CH2
    ST   X+,R24

	sbi	0x0001, 7  ; Set data acquired (GPIO1)
    pop R31
    pop R30
    pop R29
    pop R28
    pop R27
    pop R26
	ret
.endfunc

;----------------------------------------------------------------------------;
.global acquire5    ; Acquires 256 samples on two analog channels and the digital
.func acquire5      ; MSO set at 20uS/div thru 1mS/div
acquire5:           ; input: R20:R21, R22:R23, R24:R25 (pointers to CH1, Digital and CH2 buffers)
    push R26        ; Save XL
    push R27        ; Save XH
    push R28        ; Save YL
    push R29        ; Save YH
    push R30        ; Save ZL
    push R31        ; Save ZH
    movw YL, R24    ; Load pointer to Y, R24 can now be used to read the port
    movw ZL, R22    ; Load pointer to Z, R22 can now be used to read the port
    movw XL, R20    ; Load pointer to X, R20 can now be used to read the port
    ldi  R25, 251   ; R25 will be used as the counter

// Flush three samples
fl3:sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp    fl3
fh3:sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp    fh3

fl4:sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp    fl4
fh4:sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp    fh4

fl2:sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp    fl2
fh2:sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp    fh2

wl2:sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp    wl2
wh2:sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp    wh2

    ; ADC is delayed 5 samples (but there are 2 an CH), so read 2 digital inputs first to be in sync
    in   R22,0x0016 ; 1 cycle
    ST   Z+,R22     ; 1 cycle
w1: sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp w1         ; 2 cycles

    in   R22,0x0016 ; 1 cycle
    ST   Z+,R22     ; 1 cycle
w2: sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp w2         ; 2 cycles

    in   R22,0x0016 ; 1 cycle
    ST   Z+,R22     ; 1 cycle
w3: sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp w3         ; 2 cycles

; remaining 253 samples:
more5:                  ; cycles:
    in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
    in   R22,0x0016     ; 1     Digital
    ST   Z+,R22         ; 1
w4: sbic	0x001E, 3	; 1 Wait until CH Sel signal is low
    rjmp w4             ; 2 cycles
    in   R20,0x0012     ; 1     CH2
    ST   X+,R20         ; 1
w5: sbis	0x001E, 3	; 1 Wait until CH Sel signal is high
    rjmp w5             ; 2 cycles

    dec  R25            ; 1
    brne more5          ; 2

     ; Last 4 analog inputs
    in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
w6: sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp w6             ; 2 cycles
    in   R24,0x0012     ;       CH2
    ST   X+,R24
w7: sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp w7             ; 2 cycles

    ; Last 4 analog inputs
    in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
w8: sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp w8             ; 2 cycles
    in   R24,0x0012     ;       CH2
    ST   X+,R24
w9: sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp w9             ; 2 cycles

    ; Last 3 analog inputs
    in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
w10:sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp w10            ; 2 cycles
    in   R24,0x0012     ;       CH2
    ST   X+,R24
w11:sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp w11            ; 2 cycles

    ; Last 3 analog inputs
    in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
w12:sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp w12            ; 2 cycles
    in   R24,0x0012     ;       CH2
    ST   X+,R24
w13:sbis	0x001E, 3	; Wait until CH Sel signal is high
    rjmp w13            ; 2 cycles

    ; Last 3 analog inputs
    in   R24,0x0012     ; 1     CH1
    ST   Y+,R24         ; 1
w14:sbic	0x001E, 3	; Wait until CH Sel signal is low
    rjmp w14            ; 2 cycles
    in   R24,0x0012     ;       CH2
    ST   X+,R24

	sbi	0x0001, 7  ; Set data acquired (GPIO1)
    pop R31
    pop R30
    pop R29
    pop R28
    pop R27
    pop R26
	ret
.endfunc
