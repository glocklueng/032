#ifndef _L1_2014_DEF_
#define _L1_2014_DEF_   ( 1 )

// charliex - nullspacelabs.com


// defines

// Ports and pin mapping

// LED connected to CPU
#define LED_PORT                        GPIOD
#define LED_PIN                         GPIO_Pin_2                              // output
#define RCC_APB2Periph_GPIO_LED         RCC_APB2Periph_GPIOD      // PD

// Speaker
#define SPEAKER_PORT                    GPIOC
#define SPEAKER_PIN                     GPIO_Pin_6                              // output

// FPGA

// Output Enables
#define OE_PORT                         GPIOC
#define OE1_PIN                         GPIO_Pin_8                              // output
#define OE2_PIN                         GPIO_Pin_9                              // output
#define OE3_PIN                         GPIO_Pin_10                             // output
#define OE4_PIN                         GPIO_Pin_11                             // output

// FPGA_NINIT
#define FPGA_NINIT_PORT                 GPIOC
#define FPGA_NINIT_PIN                  GPIO_Pin_7								// input

#define FPGAON_PORT                    GPIOB

#define FPGAON_PIN                     GPIO_Pin_11
#define NVDD_ON_PIN                    GPIO_Pin_10

#define FPGA_DONE_PORT                 GPIOB
#define FPGA_DONE_PIN                  GPIO_Pin_0


#define FPGA_DOUT_PORT                 GPIOA
#define FPGA_DOUT_PIN                  GPIO_Pin_10
#define FPGA_DIN_PIN                   GPIO_Pin_9
#define FPGA_CCLK_PIN                  GPIO_Pin_8
#define FPGA_NPROGRAM_PIN              GPIO_Pin_7

#define MISO_PORT                      GPIOC
#define MISO_PIN                       GPIO_Pin_15

#define MOSI_PORT                      GPIOC
#define MOSI_PIN                       GPIO_Pin_14

#define SPCK_PORT                      GPIOC
#define SPCK_PIN                       GPIO_Pin_13

#define NCS_PORT                       GPIOC
#define NCS_PIN                        GPIO_Pin_12

// Relay control
#define RELAY_PORT                      GPIOC
#define RELAY_PIN                       GPIO_Pin_5

// PCKO
#define PCK0_PORT						GPIOC	
#define PCK0_PIN						GPIO_Pin_4				// output clock for FPGA/SSP

// FPGA ENDS


// SD-CARD
#define SDCARD_PORT                    GPIOC
#define SDDI_PIN                       GPIO_Pin_3
#define SDDO_PIN                       GPIO_Pin_2
#define SDSCK_PIN                      GPIO_Pin_1               // inverted
#define SDCS_PIN                       GPIO_Pin_0

// MUXSEL's
#define MUXSEL_PORT                    GPIOB
#define MUXSEL_HIPKD_PIN               GPIO_Pin_15
#define MUXSEL_LOPKD_PIN               GPIO_Pin_14
#define MUXSEL_HIRAW_PIN               GPIO_Pin_13
#define MUXSEL_LOWRAW_PIN              GPIO_Pin_12
#define NVDDON_PIN                     GPIO_Pin_10

// OLED
#define OLED_PORT                	    GPIOB
#define CS_PIN                         GPIO_Pin_9               // inverted
#define RES_PIN                        GPIO_Pin_8               // inverted
#define DC_PIN                         GPIO_Pin_7

#define OLED2_PORT                	   GPIOA
#define SDIN_DB1_PIN                   GPIO_Pin_6
#define SCLK_DB0_PIN                   GPIO_Pin_5

// Switches
#define	RCC_APB2Periph_GPIO_SW		   RCC_APB2Periph_GPIOA
#define SW_PORT                        GPIOA
#define SW_K1_PIN                      GPIO_Pin_4								// input
#define SW_K2_PIN                      GPIO_Pin_2								// input
#define SW_K3_PIN                      GPIO_Pin_3								// input

#define GPIO_SW_PORTSOURCE            GPIO_PortSourceGPIOA      				// PA
#define GPIO_SW_K1_PINSOURCE          GPIO_PinSource4     						// PA4
#define GPIO_SW_K1_EXTI_Line          EXTI_Line0          
#define GPIO_SW_K2_PINSOURCE   		  GPIO_PinSource2     						// PA4
#define GPIO_SW_K2_EXTI_Line          EXTI_Line2          

// Ampl Lo/Hi
#define AMPL_PORT                      GPIOA
#define AMPL_HI_PIN                    GPIO_Pin_1								// input analog
#define AMPL_LO_PIN                    GPIO_Pin_0								// input analog

#define ADC_AIN_CHANNEL                ADC_Channel_1             				// PA1

#define SSP_PORT						GPIOB
#define SSP_DIN_PIN                    GPIO_Pin_6
#define SSP_DOUT_PIN                   GPIO_Pin_5
#define SSP_CLK_PIN                    GPIO_Pin_2
#define SSP_FRAME                      GPIO_Pin_1


// JTAG
// PORTB/PORTA
// PB3/PB4/PA14/PA14/PA15
// BOOT0

#endif
