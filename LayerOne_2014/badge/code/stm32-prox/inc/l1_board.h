#ifndef _L1_2014_DEF_
#define _L1_2014_DEF_   ( 1 )

// latest board

#define BOARD_REVISION	( 3 )  

// defines

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// VDD voltage in MV
#define VDD_MV		( 33400 )

#define DMA_ADC		(  0 )

// Ports and pin mapping

/* GPIOA Section */


#define OLED2_PORT                	GPIOA
#define SW_PORT                         GPIOA
#define AMPL_PORT                       GPIOA

#define	USB_DISCONNECT_PORT		GPIOA

#define SDIN_DB1_PIN                    GPIO_Pin_6				//output						
#define SCLK_DB0_PIN                    GPIO_Pin_5				//output

// SPI PINS ARM->FPGA (PROGRAMS & CONFIGURES FPGA)
#define FPGA_DOUT_PORT                  GPIOC
#define FPGA_DOUT_PIN                   GPIO_Pin_11				//output ( reconfigure this to be PCK0)

#define	FPGA_DIN_PORT			GPIOA
#define FPGA_DIN_PIN                    GPIO_Pin_9				//output

#define	FPGA_CCLK_PORT			GPIOA
#define FPGA_CCLK_PIN                   GPIO_Pin_8				//output

#define	FPGA_NPROGRAM_PORT		GPIOA
#define FPGA_NPROGRAM_PIN               GPIO_Pin_7				//output


//OLED
#if BOARD_REVISION == 3
#define OLED_CS_PORT                	GPIOA
#define CS_PIN                          GPIO_Pin_4               		// inverted output
#endif

// Switches

#if BOARD_REVISION == 3
#define SW_K1_PORT                     GPIOB
#else
#define SW_K1_PORT                     GPIOA
#endif

#define SW_K2_PORT                     GPIOA
#define SW_K3_PORT                     GPIOA

#define	RCC_APB2Periph_GPIO_SW		   RCC_APB2Periph_GPIOA
#if BOARD_REVISION == 3
#define SW_K1_PIN                      GPIO_Pin_9				// input, pulldown
#else
#define SW_K1_PIN                      GPIO_Pin_4				// input, pulldown
#endif

#define SW_K2_PIN                      GPIO_Pin_3				// input, pulldown
#define SW_K3_PIN                      GPIO_Pin_2				// input, pulldown

// USB
#if BOARD_REVISION == 3
#define USB_DISCONNECT_PIN		GPIO_Pin_10				// output:
#else
#define USB_DISCONNECT_PIN		GPIO_Pin_2				// output:
#endif

#define GPIO_SW_PORTSOURCE            GPIO_PortSourceGPIOA      		// PA

#if BOARD_REVISION == 3
#define GPIO_SW_K1_PINSOURCE          GPIO_PinSource9     			// PB9
#define GPIO_SW_K2_PINSOURCE   	      GPIO_PinSource3     			// PA4
#else
#define GPIO_SW_K1_PINSOURCE          GPIO_PinSource4     			// PA4
#define GPIO_SW_K2_PINSOURCE   	      GPIO_PinSource2     			// PA4
#endif

#define GPIO_SW_K1_EXTI_Line          EXTI_Line0          

// Ampl Lo/Hi
#define AMPL_HI_PIN                    GPIO_Pin_1				// input analog
#define AMPL_LO_PIN                    GPIO_Pin_0				// input analog

#define ADC_AIN_CHANNEL                ADC_Channel_1             		// PA1

#ifdef CS_PIN
#define GPIOA_OUTPUTS_2MHZ_PP_MASK		( CS_PIN | FPGA_NPROGRAM_PIN |SDIN_DB1_PIN | SCLK_DB0_PIN )
#else
#define GPIOA_OUTPUTS_2MHZ_PP_MASK		( FPGA_NPROGRAM_PIN |SDIN_DB1_PIN | SCLK_DB0_PIN )
#endif

#if BOARD_REVISION == 3
#define GPIOA_OUTPUTS_50MHZ_PP_MASK		( FPGA_DOUT_PIN  | FPGA_DIN_PIN | FPGA_CCLK_PIN | SDIN_DB1_PIN | SCLK_DB0_PIN )
#else
#define GPIOA_OUTPUTS_50MHZ_PP_MASK		( FPGA_DOUT_PIN  | FPGA_DIN_PIN | FPGA_CCLK_PIN | USB_DISCONNECT_PIN   | SDIN_DB1_PIN | SCLK_DB0_PIN )
#endif

//#define GPIOA_OUTPUTS_50MHZ_OD_MASK		( USB_DISCONNECT_PIN )
#define GPIOA_INPUT_FLOAT_MASK			( SW_K3_PIN  )
#if BOARD_REVISION == 3
#define GPIOA_INPUT_PULLDOWN_MASK		(  SW_K2_PIN  )
#else
#define GPIOA_INPUT_PULLDOWN_MASK		( SW_K1_PIN | SW_K2_PIN )
#endif
#define GPIOA_ANALOG_MASK			( AMPL_HI_PIN | AMPL_LO_PIN  )
#define GPIOA_DEFAULT_LOW_MASK			( GPIOA_OUTPUTS_2MHZ_PP_MASK | GPIOA_OUTPUTS_50MHZ_PP_MASK )

/* End Section GPIOA */

/* GPIOB Section */

// MUXSEL's
#if BOARD_REVISION == 3
#	define MUXSEL_PORT              GPIOC
#else
#	define MUXSEL_PORT              GPIOB
#endif

#define	MUXSEL_HIPKD_PORT		MUXSEL_PORT
#define	MUXSEL_LOPKD_PORT		MUXSEL_PORT

#define	MUXSEL_HIRAW_PORT		GPIOB
#define	MUXSEL_LORAW_PORT		GPIOB



#if BOARD_REVISION == 3
#define MUXSEL_HIPKD_PIN                GPIO_Pin_9			        //output
#define MUXSEL_LOPKD_PIN                GPIO_Pin_8			        //output
#else
#define MUXSEL_HIPKD_PIN                GPIO_Pin_15			        //output
#define MUXSEL_LOPKD_PIN                GPIO_Pin_14			        //output
#endif


#if BOARD_REVISION == 3
//PORTB
#define MUXSEL_HIRAW_PIN                GPIO_Pin_2			        //output
#define MUXSEL_LORAW_PIN                GPIO_Pin_1			        //output
#else
#define MUXSEL_HIRAW_PIN                GPIO_Pin_13			        //output
#define MUXSEL_LORAW_PIN                GPIO_Pin_12			        //output
#endif

// FPGA Power On
#define FPGAON_PORT                     GPIOB
#define FPGAON_PIN                      GPIO_Pin_11				//output

// USB Power On
#define	NVDD_ON_PORT			GPIOB
#define NVDD_ON_PIN                     GPIO_Pin_10				//output

// FPGA DONE
#define FPGA_DONE_PORT                  GPIOB
#define FPGA_DONE_PIN                   GPIO_Pin_0				// input


// OLED
#if BOARD_REVISION == 3
#define OLED_PORT                	GPIOB
#define RES_PIN                         GPIO_Pin_8               		// inverted output
#define DC_PIN                          GPIO_Pin_7				// output
#else
#define OLED_PORT                	GPIOB
#define OLED_CS_PORT                	GPIOB
#define CS_PIN                          GPIO_Pin_9               		// inverted output
#define RES_PIN                         GPIO_Pin_8               		// inverted output
#define DC_PIN                          GPIO_Pin_7				// output
#endif

// Synchronous Serial Port FPGA<>ARM
#if BOARD_REVISION == 3

#define SSP_PORT	                GPIOB
#define SSP_DIN_PORT			GPIOB
#define SSP_DIN_PIN                     GPIO_Pin_15				// input(OUTPUT)	
#define SSP_DOUT_PORT			GPIOB
#define SSP_DOUT_PIN                    GPIO_Pin_14				// output
#define SSP_CLK_PORT			GPIOB
#define SSP_CLK_PIN                     GPIO_Pin_13			        // input
#define SSP_FRAME_PORT			GPIOB
#define SSP_FRAME_PIN                   GPIO_Pin_12				// input

#else

#define SSP_PORT	                GPIOB
#define SSP_DIN_PORT			GPIOB

#define SSP_DIN_PIN                     GPIO_Pin_6				// input(OUTPUT)	
#define SSP_DOUT_PORT			GPIOB
#define SSP_DOUT_PIN                    GPIO_Pin_5				// output
#define SSP_CLK_PORT			GPIOB
#define SSP_CLK_PIN                     GPIO_Pin_2			        // input
#define SSP_FRAME_PORT			GPIOB
#define SSP_FRAME_PIN                   GPIO_Pin_1				// input

#endif

#define GPIO_SSP_PORTSOURCE             GPIO_PortSourceGPIOB      		// PB

// Frame ___||_________
#define GPIO_SSP_FRAME_PINSOURCE	GPIO_PinSource1

// _||_||_||_||
#define GPIO_SSP_CLK_PINSOURCE		GPIO_PinSource2

//@todo: USB_DISCONNECT_PIN
#if BOARD_REVISION == 3

#define GPIOB_OUTPUTS_2MHZ_PP_MASK		( DC_PIN | RES_PIN | USB_DISCONNECT_PIN |				\
						CS_PIN | FPGAON_PIN  | 	\
						MUXSEL_LOPKD_PIN |MUXSEL_HIRAW_PIN | MUXSEL_LORAW_PIN )
#else
#define GPIOB_OUTPUTS_2MHZ_PP_MASK		( DC_PIN | RES_PIN | 				\
						CS_PIN | FPGAON_PIN  | MUXSEL_HIPKD_PIN | 	\
						MUXSEL_LOPKD_PIN |MUXSEL_HIRAW_PIN | MUXSEL_LORAW_PIN )
#endif

//#define GPIOB_OUTPUTS_10MHZ_PP_MASK		( )
#define GPIOB_OUTPUTS_50MHZ_PP_MASK		( NVDD_ON_PIN  )
//#define GPIOB_OUTPUTS_50MHZ_OD_MASK		( USB_DISCONNECT_PIN )

//#define GPIOB_INPUT_FLOAT_MASK		( SSP_CLK_PIN )
#define GPIOB_INPUT_PULLUP_MASK			( FPGA_DONE_PIN ) 

#if BOARD_REVISION == 3
#define GPIOB_INPUT_PULLDOWN_MASK		( SW_K1_PIN | SSP_FRAME_PIN | SSP_DIN_PIN |SSP_DOUT_PIN | SSP_CLK_PIN ) // Might want to make these float
#else
#define GPIOB_INPUT_PULLDOWN_MASK		( SSP_FRAME_PIN | SSP_DIN_PIN |SSP_DOUT_PIN | SSP_CLK_PIN ) // Might want to make these float
#endif

//#define GPIOB_ANALOG_MASK			( )
#define	GPIOB_DEFAULT_LOW_MASK			( GPIOB_OUTPUTS_2MHZ_PP_MASK )
//#define	GPIOB_DEFAULT_HIGH_MASK		( )
/* End Section GPIOB */

/* GPIOC Section */
#define MISO_PORT                      GPIOC
#define MISO_PIN                       GPIO_Pin_15				// input from fpga

#define MOSI_PORT                      GPIOC
#define MOSI_PIN                       GPIO_Pin_14				// output to fpga

#define SPCK_PORT                      GPIOC
#define SPCK_PIN                       GPIO_Pin_13				// output to fpga

#define NCS_PORT                       GPIOC
#define NCS_PIN                        GPIO_Pin_12				// output to fpga

#if BOARD_REVISION != 3
// Output Enables (these aren't supposed to be on the ARM) !!
#define OE_PORT                         GPIOC
#define OE1_PORT            GPIOC
#define OE2_PORT            GPIOC
#define OE3_PORT            GPIOC
#define OE4_PORT            GPIOC

#define OE4_PIN                         GPIO_Pin_11                             // input float
#define OE3_PIN                         GPIO_Pin_10                             // input float
#define OE2_PIN                         GPIO_Pin_9                              // input float
#define OE1_PIN                         GPIO_Pin_8                              // input float

#endif

// FPGA_NINIT
#define FPGA_NINIT_PORT                 GPIOC
#define FPGA_NINIT_PIN                  GPIO_Pin_7			        // input, pullup

// Speaker
#define SPEAKER_PORT                    GPIOC
#define SPEAKER_PIN                     GPIO_Pin_6                              // output

// Relay control
#define RELAY_PORT                      GPIOC
#define RELAY_PIN                       GPIO_Pin_5

// PCKO
#define PCK0_PORT			GPIOC	
#define PCK0_PIN			GPIO_Pin_4				// output clock for FPGA/SSP 24Mhz clock

// SD-CARD
#define SDCARD_PORT                    GPIOC
#define SDDI_PIN                       GPIO_Pin_3				// input
#define SDDO_PIN                       GPIO_Pin_2				// output
#define SDSCK_PIN                      GPIO_Pin_1   				// inverted output
#define SDCS_PIN                       GPIO_Pin_0				// output

#if BOARD_REVISION == 3
#define GPIOC_OUTPUTS_2MHZ_PP_MASK		(MUXSEL_HIPKD_PIN |SPEAKER_PIN | RELAY_PIN |SDDO_PIN | SDCS_PIN)
#else
#define GPIOC_OUTPUTS_2MHZ_PP_MASK		( SPEAKER_PIN | RELAY_PIN |SDDO_PIN | SDCS_PIN)

#endif

#define GPIOC_OUTPUTS_10MHZ_PP_MASK		( SDSCK_PIN | NCS_PIN | MOSI_PIN |SPCK_PIN )
#define GPIOC_OUTPUTS_50MHZ_PP_MASK		( PCK0_PIN   )

#if BOARD_REVISION == 3
#define GPIOC_INPUT_FLOAT_MASK          ( SDDI_PIN | MISO_PIN )
#else
#define GPIOC_INPUT_FLOAT_MASK          ( OE4_PIN | OE3_PIN | OE2_PIN | OE1_PIN | SDDI_PIN | MISO_PIN )
#endif

#define GPIOC_INPUT_PULLUP_MASK			( FPGA_NINIT_PIN )

#if BOARD_REVISION == 3
#define GPIOC_ANALOG_MASK				( GPIO_Pin_4 | GPIO_Pin_3 )
#endif

#define	GPIOC_DEFAULT_LOW_MASK			( GPIOC_OUTPUTS_2MHZ_PP_MASK | GPIOC_OUTPUTS_10MHZ_PP_MASK )
//#define	GPIOC_DEFAULT_HIGH_MASK		( )
/* End Section GPIOC */


/* GPIOD Section */
// LED connected to CPU
#define LED_PORT                        GPIOD
#define LED_PIN                         GPIO_Pin_2                              // output
#define RCC_APB2Periph_GPIO_LED         RCC_APB2Periph_GPIOD      		// PD
// PD0/PD1 XTAL



#define GPIOD_OUTPUTS_2MHZ_PP_MASK			( LED_PIN )
//#define GPIOD_OUTPUTS_10MHZ_PP_MASK		( )
//#define GPIOD_OUTPUTS_50MHZ_PP_MASK		( )
//#define GPIOD_INPUT_FLOAT_MASK			( )
//#define GPIOD_ANALOG_MASK					( )
#define	GPIOD_DEFAULT_LOW_MASK				( LED_PIN )
/* End Section GPIOD */

// aliases for proxmark3

#define				GPIO_MUXSEL_HIPKD_PIN	MUXSEL_HIPKD_PIN
#define				GPIO_MUXSEL_LOPKD_PIN	MUXSEL_LOPKD_PIN
#define				GPIO_MUXSEL_HIRAW_PIN	MUXSEL_HIRAW_PIN
#define				GPIO_MUXSEL_LORAW_PIN	MUXSEL_LORAW_PIN

#define				GPIO_MUXSEL_HIPKD_PORT	MUXSEL_HIPKD_PORT
#define				GPIO_MUXSEL_LOPKD_PORT	MUXSEL_LOPKD_PORT
#define				GPIO_MUXSEL_HIRAW_PORT	MUXSEL_HIRAW_PORT
#define				GPIO_MUXSEL_LORAW_PORT	MUXSEL_LORAW_PORT

// only one led
#define				GPIO_LED_A_PIN			LED_PIN
#define				GPIO_LED_B_PIN			LED_PIN
#define				GPIO_LED_C_PIN			LED_PIN
#define				GPIO_LED_D_PIN			LED_PIN

#define				GPIO_LED_A_PORT			LED_PORT
#define				GPIO_LED_B_PORT			LED_PORT
#define				GPIO_LED_C_PORT			LED_PORT
#define				GPIO_LED_D_PORT			LED_PORT

#define				GPIO_FPGA_NPROGRAM_PIN		FPGA_NPROGRAM_PIN
#define				GPIO_FPGA_NPROGRAM_PORT		FPGA_NPROGRAM_PORT
#define				GPIO_FPGA_CCLK_PIN		FPGA_CCLK_PIN
#define				GPIO_FPGA_CCLK_PORT		FPGA_CCLK_PORT
#define				GPIO_FPGA_DIN_PIN		FPGA_DIN_PIN
#define				GPIO_FPGA_DIN_PORT		FPGA_DIN_PORT

#define				GPIO_FPGA_NINIT_PIN		FPGA_NINIT_PIN
#define				GPIO_FPGA_NINIT_PORT		FPGA_NINIT_PORT

#define				GPIO_FPGA_DONE_PIN		FPGA_DONE_PIN
#define				GPIO_FPGA_DONE_PORT		FPGA_DONE_PORT

#define				GPIO_SSC_CLK_PIN		SSP_CLK_PIN
#define				GPIO_SSC_CLK_PORT		SSP_CLK_PORT

#define				GPIO_SSC_DIN_PIN		SSP_DIN_PIN
#define				GPIO_SSC_DIN_PORT		SSP_DIN_PORT
				
				// PA17				//PB5
#define				GPIO_SSC_DOUT_PIN		SSP_DOUT_PIN
#define				GPIO_SSC_DOUT_PORT		SSP_DOUT_PORT

#define				GPIO_OE1_PORT			OE1_PORT
#define				GPIO_OE2_PORT			OE2_PORT
#define				GPIO_OE3_PORT			OE3_PORT
#define				GPIO_OE4_PORT			OE4_PORT

#define				GPIO_OE1_PIN			OE1_PIN
#define				GPIO_OE2_PIN			OE2_PIN
#define				GPIO_OE3_PIN			OE3_PIN
#define				GPIO_OE4_PIN			OE4_PIN

#define				SpinDelay			DelaymS
#define				SpinDelayUs			DelayuS

//Define all SPI Pins

#define SPI_OUT_PORT            	SSP_DOUT_PORT
#define SPI_OUT_PIN			SSP_DOUT_PIN			        // SSP_DOUT
#define SPI_IN_PORT      		SSP_DIN_PORT
#define SPI_IN_PIN			SSP_DIN_PIN			        // SSP_DIN
#define SPI_CLK_PORT			SSP_CLK_PORT
#define SPI_CLK_PIN			SSP_CLK_PIN			        // SSP_CLK
#define SPI_CS_PORT			SSP_FRAME_PORT
#define SPI_CS_PIN			SSP_FRAME_PIN			        // SSP_FRAME

						  
// JTAG
// PORTB/PORTA
// PB3/PB4/PA14/PA14/PA15
// BOOT0

#endif
