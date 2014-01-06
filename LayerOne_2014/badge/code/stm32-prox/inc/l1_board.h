#ifndef _L1_2014_DEF_
#define _L1_2014_DEF_   ( 1 )

// charliex - nullspacelabs.com

// defines

// Ports and pin mapping

/* GPIOA Section */

#define	FPGA_NPROGRAM_PORT				GPIOA
#define FPGA_DOUT_PORT                 GPIOA
#define OLED2_PORT                	   GPIOA
#define SW_PORT                        GPIOA
#define AMPL_PORT                      GPIOA
#define	FPGA_CCLK_PORT					GPIOA
#define	FPGA_DIN_PORT					GPIOA
#define	USB_DISCONNECT_PORT				GPIOA

#define FPGA_DOUT_PIN                  GPIO_Pin_10								//output ( reconfigure this to be PCK0)
#define FPGA_DIN_PIN                   GPIO_Pin_9								//output
#define FPGA_CCLK_PIN                  GPIO_Pin_8								//output
#define FPGA_NPROGRAM_PIN              GPIO_Pin_7								//output
#define SDIN_DB1_PIN                   GPIO_Pin_6								//output						
#define SCLK_DB0_PIN                   GPIO_Pin_5								//output

// Switches

#define SW_K1_PORT                     GPIOA
#define SW_K2_PORT                     GPIOA
//#define SW_K3_PORT                     GPIOA

#define	RCC_APB2Periph_GPIO_SW		   RCC_APB2Periph_GPIOA
#define SW_K1_PIN                      GPIO_Pin_4								// input, pulldown
#define SW_K2_PIN                      GPIO_Pin_3								// input, pulldown
//#define SW_K3_PIN                    GPIO_Pin_2								// input, pulldown
#define USB_DISCONNECT_PIN			   GPIO_Pin_2								// output:

#define GPIO_SW_PORTSOURCE            GPIO_PortSourceGPIOA      				// PA
#define GPIO_SW_K1_PINSOURCE          GPIO_PinSource4     						// PA4
#define GPIO_SW_K1_EXTI_Line          EXTI_Line0          
#define GPIO_SW_K2_PINSOURCE   		  GPIO_PinSource2     						// PA4
#define GPIO_SW_K2_EXTI_Line          EXTI_Line2          

// Ampl Lo/Hi
#define AMPL_HI_PIN                    GPIO_Pin_1								// input analog
#define AMPL_LO_PIN                    GPIO_Pin_0								// input analog

#define ADC_AIN_CHANNEL                ADC_Channel_1             				// PA1

#define GPIOA_OUTPUTS_2MHZ_PP_MASK		( FPGA_DOUT_PIN  | FPGA_NPROGRAM_PIN |SDIN_DB1_PIN | SCLK_DB0_PIN )
#define GPIOA_OUTPUTS_50MHZ_PP_MASK		( FPGA_DIN_PIN | FPGA_CCLK_PIN | USB_DISCONNECT_PIN | FPGA_CCLK_PIN  | SDIN_DB1_PIN | SCLK_DB0_PIN )
//#define GPIOA_OUTPUTS_50MHZ_OD_MASK		( USB_DISCONNECT_PIN )
//#define GPIOA_INPUT_FLOAT_MASK			(  )
#define GPIOA_INPUT_PULLDOWN_MASK		( SW_K1_PIN | SW_K2_PIN  )
#define GPIOA_ANALOG_MASK				( AMPL_HI_PIN | AMPL_LO_PIN  )
#define GPIOA_DEFAULT_LOW_MASK			( GPIOA_OUTPUTS_2MHZ_PP_MASK | GPIOA_OUTPUTS_50MHZ_PP_MASK )

/* End Section GPIOA */

/* GPIOB Section */

#define FPGAON_PORT                    GPIOB
#define FPGA_DONE_PORT                 GPIOB
#define	NVDD_ON_PORT					GPIOB

// MUXSEL's
#define MUXSEL_PORT                   	GPIOB
#define	MUXSEL_HIPKD_PORT				GPIOB
#define	MUXSEL_LOPKD_PORT				GPIOB
#define	MUXSEL_HIRAW_PORT				GPIOB
#define	MUXSEL_LORAW_PORT				GPIOB
	
#define MUXSEL_HIPKD_PIN               GPIO_Pin_15								//output
#define MUXSEL_LOPKD_PIN               GPIO_Pin_14								//output
#define MUXSEL_HIRAW_PIN               GPIO_Pin_13								//output
#define MUXSEL_LORAW_PIN               GPIO_Pin_12								//output

// FPGA Power On
#define FPGAON_PIN                     GPIO_Pin_11								//output
#define NVDD_ON_PIN                    GPIO_Pin_10								//output

// OLED
#define OLED_PORT                	    GPIOB
#define CS_PIN                         GPIO_Pin_9               				// inverted output
#define RES_PIN                        GPIO_Pin_8               				// inverted output
#define DC_PIN                         GPIO_Pin_7								// output

#define SSP_PORT						GPIOB
#define SSP_DIN_PIN                    GPIO_Pin_6								// input(OUTPUT)	
#define SSP_DOUT_PIN                   GPIO_Pin_5								// output
// PB3/4 JTAG
#define SSP_CLK_PIN                    GPIO_Pin_2								// input
#define SSP_FRAME                      GPIO_Pin_1								// input

#define FPGA_DONE_PIN                  GPIO_Pin_0								// input

#define GPIOB_OUTPUTS_2MHZ_PP_MASK		(SSP_DIN_PIN | SSP_FRAME  | SSP_DOUT_PIN | DC_PIN | RES_PIN | CS_PIN | FPGAON_PIN  | NVDD_ON_PIN | MUXSEL_HIPKD_PIN | \
											MUXSEL_LOPKD_PIN |MUXSEL_HIRAW_PIN | MUXSEL_LORAW_PIN )

//#define GPIOB_OUTPUTS_10MHZ_PP_MASK		( )
//#define GPIOB_OUTPUTS_50MHZ_PP_MASK		( USB_DISCONNECT_PIN )
//#define GPIOB_OUTPUTS_50MHZ_OD_MASK		( USB_DISCONNECT_PIN )
#define GPIOB_INPUT_FLOAT_MASK			( SSP_FRAME  | SSP_CLK_PIN )
#define GPIOB_INPUT_PULLUP_MASK			( FPGA_DONE_PIN ) 
//#define GPIOB_ANALOG_MASK				( )
#define	GPIOB_DEFAULT_LOW_MASK			( GPIOB_OUTPUTS_2MHZ_PP_MASK )
//#define	GPIOB_DEFAULT_HIGH_MASK		( )
/* End Section GPIOB */

/* GPIOC Section */
#define MISO_PORT                      GPIOC
#define MISO_PIN                       GPIO_Pin_15

#define MOSI_PORT                      GPIOC
#define MOSI_PIN                       GPIO_Pin_14

#define SPCK_PORT                      GPIOC
#define SPCK_PIN                       GPIO_Pin_13

#define NCS_PORT                       GPIOC
#define NCS_PIN                        GPIO_Pin_12

// Output Enables
#define OE_PORT                         GPIOC
#define OE4_PIN                         GPIO_Pin_11                             // output
#define OE3_PIN                         GPIO_Pin_10                             // output
#define OE2_PIN                         GPIO_Pin_9                              // output
#define OE1_PIN                         GPIO_Pin_8                              // output

// FPGA_NINIT
#define FPGA_NINIT_PORT                 GPIOC
#define FPGA_NINIT_PIN                  GPIO_Pin_7								// input, pullup

// Speaker
#define SPEAKER_PORT                    GPIOC
#define SPEAKER_PIN                     GPIO_Pin_6                              // output

// Relay control
#define RELAY_PORT                      GPIOC
#define RELAY_PIN                       GPIO_Pin_5

// PCKO
#define PCK0_PORT						GPIOC	
#define PCK0_PIN						GPIO_Pin_4								// output clock for FPGA/SSP 24Mhz clock

// SD-CARD
#define SDCARD_PORT                    GPIOC
#define SDDI_PIN                       GPIO_Pin_3								// input
#define SDDO_PIN                       GPIO_Pin_2								// output
#define SDSCK_PIN                      GPIO_Pin_1   				            // inverted output
#define SDCS_PIN                       GPIO_Pin_0								// output

#define GPIOC_OUTPUTS_2MHZ_PP_MASK		( OE4_PIN | OE3_PIN | OE2_PIN | OE1_PIN | SPEAKER_PIN | RELAY_PIN |SDDO_PIN | SDCS_PIN)
#define GPIOC_OUTPUTS_10MHZ_PP_MASK		( SDSCK_PIN )
#define GPIOC_OUTPUTS_50MHZ_PP_MASK		( PCK0_PIN  )
//#define GPIOC_OUTPUTS_50MHZ_PP_MASK	( )
#define GPIOC_INPUT_FLOAT_MASK			(  SDDI_PIN)
#define GPIOC_INPUT_PULLUP_MASK			( FPGA_NINIT_PIN )
//#define GPIOC_ANALOG_MASK				( )
#define	GPIOC_DEFAULT_LOW_MASK			( GPIOC_OUTPUTS_2MHZ_PP_MASK | GPIOC_OUTPUTS_10MHZ_PP_MASK )
//#define	GPIOC_DEFAULT_HIGH_MASK		( )
/* End Section GPIOC */


/* GPIOD Section */
// LED connected to CPU
#define LED_PORT                        GPIOD
#define LED_PIN                         GPIO_Pin_2                              // output
#define RCC_APB2Periph_GPIO_LED         RCC_APB2Periph_GPIOD      				// PD
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
#define				GPIO_FPGA_CCLK_PIN			FPGA_CCLK_PIN
#define				GPIO_FPGA_CCLK_PORT			FPGA_CCLK_PORT
#define				GPIO_FPGA_DIN_PIN			FPGA_DIN_PIN
#define				GPIO_FPGA_DIN_PORT			FPGA_DIN_PORT

#define				GPIO_FPGA_NINIT_PIN			FPGA_NINIT_PIN
#define				GPIO_FPGA_NINIT_PORT		FPGA_NINIT_PORT

#define				GPIO_FPGA_DONE_PIN			FPGA_DONE_PIN
#define				GPIO_FPGA_DONE_PORT			FPGA_DONE_PORT


#define				SpinDelay					DelaymS
#define				SpinDelayUs					DelayuS

// JTAG
// PORTB/PORTA
// PB3/PB4/PA14/PA14/PA15
// BOOT0

#endif
