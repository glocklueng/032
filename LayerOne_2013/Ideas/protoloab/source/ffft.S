;-----------------------------------------------------------------------------;
; Fixed-point FFT routines for megaAVRs                        (C)ChaN, 2005
;-----------------------------------------------------------------------------;
;
; void fft_input (const int16_t *array_src, complex_t *array_bfly);
; void fft_execute (complex_t *array_bfly);
; void fft_output (complex_t *array_bfly, uint16_t *array_dst);
;
;  <array_src>: Wave form to be processed.
;  <array_bfly>: Complex array for butterfly operations.
;  <array_dst>: Spectrum output buffer.
;
; These functions must be called in sequence to do a DFT in FFT algorithm.
; fft_input() fills the complex array with a wave form to prepare butterfly
; operations. A hamming window is applied at the same time.
; fft_execute() executes the butterfly operations.
; fft_output() re-orders the results, converts the complex spectrum into
; scalar spectrum and output it in linear scale.
;
; The number of points FFT_N is defined in "ffft.h" and the value can be 64,
; 128, 256 or 512.
;
;----------------------------------------------------------------------------;
; 16bit fixed-point FFT performance with a MegaAVR @16MHz (measured)
;
;  Points:   Input, Execute,  Output,    Total:  Throughput
;   64pts:   .17ms,   2.0ms,   1.2ms,    3.4ms:   19.0kpps
;  128pts:   .33ms,   4.6ms,   2.4ms,    7.3ms:   17.5kpps
;  256pts:   .66ms,  10.4ms,   4.9ms,   15.9ms:   16.1kpps
;  512pts:   1.3ms,  23.2ms,   9.7ms,   34.2ms:   14.9kpps
; 1024pts:   <undefined>
;----------------------------------------------------------------------------;


.nolist
#define FFFT_ASM
#include "ffft.h"
.list

#define FFT_B 8

;----------------------------------------------------------------------------;
; Constant Tables
tbl_cos_sin:	; Table of {cos(x),sin(x)}, (0 <= x < pi, in FFT_N/2 steps)
	.dc.w	32767, 0, 32757, 804, 32727, 1607, 32678, 2410, 32609, 3211, 32520, 4011, 32412, 4807, 32284, 5601
	.dc.w	32137, 6392, 31970, 7179, 31785, 7961, 31580, 8739, 31356, 9511, 31113, 10278, 30851, 11038, 30571, 11792
	.dc.w	30272, 12539, 29955, 13278, 29621, 14009, 29268, 14732, 28897, 15446, 28510, 16150, 28105, 16845, 27683, 17530
	.dc.w	27244, 18204, 26789, 18867, 26318, 19519, 25831, 20159, 25329, 20787, 24811, 21402, 24278, 22004, 23731, 22594
	.dc.w	23169, 23169, 22594, 23731, 22004, 24278, 21402, 24811, 20787, 25329, 20159, 25831, 19519, 26318, 18867, 26789
	.dc.w	18204, 27244, 17530, 27683, 16845, 28105, 16150, 28510, 15446, 28897, 14732, 29268, 14009, 29621, 13278, 29955
	.dc.w	12539, 30272, 11792, 30571, 11038, 30851, 10278, 31113, 9511, 31356, 8739, 31580, 7961, 31785, 7179, 31970
	.dc.w	6392, 32137, 5601, 32284, 4807, 32412, 4011, 32520, 3211, 32609, 2410, 32678, 1607, 32727, 804, 32757
	.dc.w	0, 32766, -804, 32757, -1607, 32727, -2410, 32678, -3211, 32609, -4010, 32520, -4807, 32412, -5601, 32284
	.dc.w	-6392, 32137, -7179, 31970, -7961, 31785, -8739, 31580, -9511, 31356, -10278, 31113, -11038, 30851, -11792, 30571
	.dc.w	-12539, 30272, -13278, 29955, -14009, 29621, -14732, 29268, -15446, 28897, -16150, 28510, -16845, 28105, -17530, 27683
	.dc.w	-18204, 27244, -18867, 26789, -19519, 26318, -20159, 25831, -20787, 25329, -21402, 24811, -22004, 24278, -22594, 23731
	.dc.w	-23169, 23169, -23731, 22594, -24278, 22005, -24811, 21402, -25329, 20787, -25831, 20159, -26318, 19519, -26789, 18867
	.dc.w	-27244, 18204, -27683, 17530, -28105, 16845, -28510, 16150, -28897, 15446, -29268, 14732, -29620, 14009, -29955, 13278
	.dc.w	-30272, 12539, -30571, 11792, -30851, 11038, -31113, 10278, -31356, 9511, -31580, 8739, -31784, 7961, -31970, 7179
	.dc.w	-32137, 6392, -32284, 5601, -32412, 4807, -32520, 4011, -32609, 3211, -32678, 2410, -32727, 1607, -32757, 804

;----------------------------------------------------------------------------;
.global fft_execute
.func fft_execute
fft_execute:
	pushw	T2H,T2L
	pushw	T4H,T4L
	pushw	T6H,T6L
	pushw	T8H,T8L
	pushw	T10H,T10L
	pushw	T12H,T12L
	pushw	T14H,T14L
	pushw	AH,AL
	pushw	YH,YL

	movw	ZL, EL				;Z = array_bfly;
	ldiw	EH,EL, 1			;E = 1;
	ldiw	XH,XL, FFT_N/2		;X = FFT_N/2;
1:	ldi	    AL, 4				;T12 = E; (angular speed)
	mul	    EL, AL				;
	movw	T12L, r0			;
	mul	    EH, AL				;
	add	    T12H, r0			;/
	movw	T14L, EL			;T14 = E;
	pushw	EH,EL
	movw	YL, ZL				;Z = &array_bfly[0];
	mul	    XL, AL				;Y = &array_bfly[X];
	addw	YH,YL, r1,r0		;
	mul	    XH, AL				;
	add	    YH, r0				;/
	pushw	ZH,ZL
2:	clrw	T10H,T10L			;T10 = 0 (angle)
	clr	EH				;Zero reg.
3:	lddw	AH,AL, Z+0			;A = *Z - *Y; *Z++ += *Y;
	asrw	AH,AL				;
	lddw	DH,DL, Y+0			;
	asrw	DH,DL				;
	movw	CL, AL				;
	subw	AH,AL, DH,DL			;
	addw	CH,CL, DH,DL			;
	stw	Z+, CH,CL			;/
	lddw	BH,BL, Z+0			;B = *Z - *Y; *Z++ += *Y;
	asrw	BH,BL				;
	lddw	DH,DL, Y+2			;
	asrw	DH,DL				;
	movw	CL, BL				;
	subw	BH,BL, DH,DL		;
	addw	CH,CL, DH,DL		;
	stw	    Z+, CH,CL			;/
	movw	r0, ZL
	ldiw	ZH,ZL, tbl_cos_sin	;C = cos(T10); D = sin(T10);
	addw	ZH,ZL, T10H,T10L	;
	lpmw	CH,CL, Z+			;
	lpmw	DH,DL, Z+			;/
	movw	ZL, r0
	FMULS16	T4H,T4L,T2H,T2L, AH,AL, CH,CL	;*Y++ = A * C + B * D;
	FMULS16	T8H,T8L,T6H,T6L, BH,BL, DH,DL	;
	addd	T4H,T4L,T2H,T2L, T8H,T8L,T6H,T6L;
	stw	Y+, T4H,T4L			;/
	FMULS16	T4H,T4L,T2H,T2L, BH,BL, CH,CL 	;*Y++ = B * C - A * D;
	FMULS16	T8H,T8L,T6H,T6L, AH,AL, DH,DL 	;
	subd	T4H,T4L,T2H,T2L, T8H,T8L,T6H,T6L;
	stw	Y+, T4H,T4L			;/
	addw	T10H,T10L, T12H,T12L		;T10 += T12; (next angle)
#if FFT_N >= 128
	sbrs	T10H, FFT_B - 7			;while(T10 < pi)
#else
	sbrs	T10L, FFT_B + 1
#endif
	rjmp	3b				;/
	ldi	AL, 4				;Y += X; Z += X; (skip split segment)
	mul	XL, AL
	addw	YH,YL, r1,r0		;
	addw	ZH,ZL, r1,r0		;
	mul	XH, AL				    ;
	add	YH, r0				    ;
	add	ZH, r0				    ;
	ldi	EL, 1				    ;while(--T14)
	subw	T14H,T14L, EH,EL	;
	rjne	2b				    ;/
	popw	ZH,ZL
	popw	EH,EL
	lslw	EH,EL				;E *= 2;
	lsrw	XH,XL				;while(X /= 2)
	adiw	XL, 0				;
	rjne	1b				    ;

	popw	YH,YL
	popw	AH,AL
	popw	T14H,T14L
	popw	T12H,T12L
	popw	T10H,T10L
	popw	T8H,T8L
	popw	T6H,T6L
	popw	T4H,T4L
	popw	T2H,T2L
;	clr	r1
	ret
.endfunc

; Constant Tables
Log2Table:	; Table 16*(Log2(1) thru Log2(2)), 16 values
    .dc.b   0,2,3,4,5,6,7,8,9,10,11,12,13,14,14,15

;----------------------------------------------------------------------------;
.global fft_output
.func fft_output
fft_output:
	pushw	T2H,T2L
	pushw	T4H,T4L
	pushw	T6H,T6L
	pushw	T8H,T8L
	pushw	T10H,T10L
	pushw	AH,AL
	pushw	YH,YL

	movw	T10L, EL			;T10 = array_bfly;
	movw	YL, DL				;Y = array_output;
;	ldiw	ZH,ZL, tbl_bitrev		;Z = tbl_bitrev;
	clr	EH				;Zero
	ldiw	AH,AL, FFT_N / 2		;A = FFT_N / 2; (pluse only)
    movw    XL, T10L
1:;	lpmw	XH,XL, Z+			;X = *Z++;
	;addw	XH,XL, T10H,T10L		;X += array_bfly;
	ldw	BH,BL, X+			;B = *X++;
	ldw	CH,CL, X+			;C = *X++;
	ldw	ZH,ZL, X+			;B = *X++; //ignore, just to advance X
	ldw	ZH,ZL, X+			;C = *X++; //ignore, just to advance X
	FMULS16	T4H,T4L,T2H,T2L, BH,BL, BH,BL	;T4:T2 = B * B;
	FMULS16	T8H,T8L,T6H,T6L, CH,CL, CH,CL	;T8:T6 = C * C;
	addd	T4H,T4L,T2H,T2L, T8H,T8L,T6H,T6L;T4:T2 += T8:T6;
	SQRT32					;B = sqrt(T4:T2);
    clr     r1
	sbis	0x0003,4	    ; Use log? (bit 4 in GPIO3)
	rjmp	nolog	        ; 2 cycles
    ; ****** Apply log ******
    LSR     BH              ; remove LSB, they are too noisy
    ROR     BL
    LSR     BH              ; remove LSB, they are too noisy
    ROR     BL
    LSR     BH              ; remove LSB, they are too noisy
    ROR     BL
    LSR     BH              ; remove LSB, they are too noisy
    ROR     BL
    ; Check if zero
    tst     BH
    breq    checklo
    rjmp    nonzero
checklo:
    tst     BL
    breq    gotit           ; data is zero

nonzero:
    ldiw	ZH,ZL, Log2Table
    ldi     CL, 0xF0        ; load with 240 (16*log2(2^15)=240)
    ; shift until most significant bit is set, each bit-shift results in log += 16
doshift:
    sbrc    BH,7
    rjmp    doneshift
    ROL     BL
    ROL     BH
    subi    CL,0x10 ; Substract 16
    rjmp    doshift
doneshift:
    ; BH has now form 1iii ixxx, get the next 4 bits (iiii) and address table with it
    lsl     BH      ; remove msb (has a 1 on it)
    lsr     BH
    lsr     BH
    lsr     BH
    lsr     BH      ; now BH is in the form 0000iiii
    add     ZL,BH   ; add offset to pointer
    adc     ZH,R1   ; R1 is zero
    lpm     CH,Z
    add     CL,CH
    mov     BH, CL
    rjmp    gotit
nolog:
    ROL     BL
    ROL     BH
    ;LSR     BH
    ;ROR     BL
    ;LSR     BH
    ;ROR     BL
    ;LSR     BH
    ;ROR     BL
    ;LSR     BH
    ;ROR     BL
    ;LSR     BH
    ;ROR     BL
    ;LSR     BH
    ;ROR     BL
    ;TST     BH
    ;BREQ    gotit
    ;ldi     BL, 255     ; clip
gotit:
    st Y+, BH           ; Only take high byte

	subiw	AH,AL, 1			;while(--A)
	rjne	1b				;/

	popw	YH,YL
	popw	AH,AL
	popw	T10H,T10L
	popw	T8H,T8L
	popw	T6H,T6L
	popw	T4H,T4L
	popw	T2H,T2L
	clr	r1
	ret
.endfunc



;----------------------------------------------------------------------------;
.global fmuls_f
.func fmuls_f
fmuls_f:
	movw	CL, EL				;C = E;
	clr	EH	;Zero
	FMULS16	ZH,ZL,XH,XL, CH,CL, DH,DL	;Z:X = C * D;
	movw	EL, ZL
	clr	r1
	ret
.endfunc

