;*********************************************************** 
;* TP-18A                                                  *
;* Mai.2007
;* http://home.versanet.de/~b-konze/                       *
;*********************************************************** 

#define Typ "6a_pwm"

; activate Motor brake 

;.equ MOT_BRAKE      = 0

;Select Brake Power
;	1 = min Brake
;	2 = medium Brake
;  	3 = hard Brake

.equ BrakeSelect    = 2

.equ CALIBRATE			= 1      
.equ OSCAL_VALUE		= 0x01ff 	; eeprom position

.equ FORCE_LIPO 	= 1			; 1=Lipo und 0=NC wenn keine Buecke
.equ ActiveFreeRun	= 0

;**** **** **** **** ****
; PORT  definitions
;**** **** **** **** ****

;*********************
; PORT D 
;*********************  
;.equ    		= 7     ;i
.equ	c_comp		= 6	;i common comparator input (AIN0)
.equ	ApFET		= 5	;o
.equ	CpFET		= 4	;o
.equ	BpFET		= 3	;o
.equ	BnFET		= 2	;o


.equ	INIT_PD		= 0
.equ	DIR_PD		= (1<<ApFET)+(1<<BpFET)+(1<<CpFET)+(1<<BnFET)
.equ	BRAKE_PD	= 0

#define ApFET_on	sbi	PORTD,5
#define ApFET_off	cbi	PORTD,5
#define BpFET_on	sbi	PORTD,3
#define BpFET_off	cbi	PORTD,3
#define CpFET_on	sbi	PORTD,4
#define CpFET_off	cbi	PORTD,4
#define BnFET_on	sbi	PORTD,2
#define BnFET_off	cbi	PORTD,2

;****
;Move BnFET_* to PB6
;  -mmca
;****



;*********************
; PORT C definitions *
;*********************
;.equ			= 5	; phase input
;.equ    	  	= 4	; ADC4
.equ	mux_c		= 3	; phase input
.equ	mux_b		= 2	; phase input
.equ 	mux_a		= 1 	; ADC1 temperature control input 
.equ 	accu_adc	= 0 	; ADC0 voltage control input 


;*********************
; PORT C definitions *
;*********************
;.equ	mux_a		= 5	; phase input
;.equ	mux_c		= 4	; phase input
;.equ    	  	= 3	; ADC3
;.equ	mux_b		= 2	; phase input
;.equ 	temp_adc	= 1 	; ADC1 temperature control input 
;.equ 	accu_adc	= 0 	; ADC0 voltage control input 

; Above block removed by mmca


;*********************
; ADC only
;*********************
;.equ    	  	= 7	; ADC7
;.equ    	  	= 6	; ADC6

.equ	INIT_PC		= 0
.equ	DIR_PC		= 0
.equ	BRAKE_PC	= 0


;*********************
; PORT B definitions *
;*********************
;.equ		= 7	
;.equ		= 6	
;.equ		= 5 (sck stk200 interface) 
;.equ		= 4	(miso stk200 interface)
;.equ		= 3	(mosi stk200 interface) 
;.equ		= 2	
;.equ		= 1	
;.equ		= 0

.equ	AnFET		= 2	;o
.equ	CnFET		= 1	;o
.equ	PWM_icp		= 0	;i r/c pulse input


.equ	INIT_PB		= 0
.equ	DIR_PB		= (1<<AnFET)+(1<<CnFET)
.equ	BRAKE_PB	= (1<<AnFET)+(1<<CnFET)

#define AnFET_on	sbi	PORTB,2
#define AnFET_off	cbi	PORTB,2
#define CnFET_on	sbi	PORTB,1
#define CnFET_off	cbi	PORTB,1



