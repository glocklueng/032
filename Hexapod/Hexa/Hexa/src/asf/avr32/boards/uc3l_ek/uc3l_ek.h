/*****************************************************************************
 *
 * \file
 *
 * \brief AT32UC3L-EK board header file.
 *
 * This file contains definitions and services related to the features of the
 * AT32UC3L-EK board.
 *
 * To use this board, define BOARD=UC3L_EK.
 *
 * Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 ******************************************************************************/


#ifndef _UC3L_EK_H_
#define _UC3L_EK_H_

#include "compiler.h"

#ifdef __AVR32_ABI_COMPILER__ // Automatically defined when compiling for AVR32, not when assembling.
#  include "led.h"
#endif  // __AVR32_ABI_COMPILER__


/*! \name Oscillator Definitions
 */
//! @{

#define FOSC32          32768                                   //!< Osc32 frequency: Hz.
#define OSC32_STARTUP   AVR32_SCIF_OSCCTRL32_STARTUP_8192_RCOSC //!< Osc32 startup time: RCOsc periods.

// No Osc0 crystal the AT32UC3L-EK
#define FOSC0           ((int)"NONE")                                //!< Osc0 frequency: Hz.
//#define OSC0_STARTUP    AVR32_SCIF_OSCCTRL0_STARTUP_2048_RCOSC  //!< Osc0 startup time: RCOsc periods.

//! @}

#define BOARD_OSC32_HZ          32768
#define BOARD_OSC32_STARTUP_US  71000
#define BOARD_OSC32_IS_XTAL     true
#define BOARD_OSC32_PINSEL      1



/*! \name GPIO Connections of PWMs
 */
//! @{
#define PWMA09_GPIO   AVR32_PIN_PA09
#define PWMA11_GPIO   AVR32_PIN_PA11
#define PWMA12_GPIO   AVR32_PIN_PA12
#define PWMA13_GPIO   AVR32_PIN_PA13
#define PWMA15_GPIO   AVR32_PIN_PA15
#define PWMA16_GPIO   AVR32_PIN_PA16
#define PWMA17_GPIO   AVR32_PIN_PA17
#define PWMA18_GPIO   AVR32_PIN_PA18
#define PWMA19_GPIO   AVR32_PIN_PA19
#define PWMA20_GPIO   AVR32_PIN_PA20
#define PWMA21_GPIO   AVR32_PIN_PA21
#define PWMA22_GPIO   AVR32_PIN_PA22

#define PWMA23_GPIO   AVR32_PIN_PB00
#define PWMA25_GPIO   AVR32_PIN_PB02
#define PWMA26_GPIO   AVR32_PIN_PB03
#define PWMA27_GPIO   AVR32_PIN_PB04
#define PWMA28_GPIO   AVR32_PIN_PB05
#define PWMA29_GPIO   AVR32_PIN_PB06
#define PWMA30_GPIO   AVR32_PIN_PB07
#define PWMA31_GPIO   AVR32_PIN_PB08
#define PWMA32_GPIO   AVR32_PIN_PB09
#define PWMA33_GPIO   AVR32_PIN_PB10
#define PWMA34_GPIO   AVR32_PIN_PB11
#define PWMA35_GPIO   AVR32_PIN_PB12
//! @}


/*! \name PWM Functions
 */
//! @{
#define PWM09_PWM_FUNCTION	AVR32_PWMA_9_FUNCTION
#define PWM11_PWM_FUNCTION	AVR32_PWMA_11_FUNCTION
#define PWM12_PWM_FUNCTION	AVR32_PWMA_12_FUNCTION
#define PWM13_PWM_FUNCTION	AVR32_PWMA_13_FUNCTION
#define PWM15_PWM_FUNCTION	AVR32_PWMA_15_FUNCTION
#define PWM16_PWM_FUNCTION	AVR32_PWMA_16_FUNCTION
#define PWM17_PWM_FUNCTION	AVR32_PWMA_17_FUNCTION
#define PWM18_PWM_FUNCTION	AVR32_PWMA_18_FUNCTION
#define PWM19_PWM_FUNCTION	AVR32_PWMA_19_FUNCTION
#define PWM20_PWM_FUNCTION	AVR32_PWMA_20_FUNCTION
#define PWM21_PWM_FUNCTION	AVR32_PWMA_21_FUNCTION
#define PWM22_PWM_FUNCTION	AVR32_PWMA_22_FUNCTION

#define PWM23_PWM_FUNCTION	AVR32_PWMA_23_FUNCTION
#define PWM25_PWM_FUNCTION	AVR32_PWMA_25_FUNCTION
#define PWM26_PWM_FUNCTION	AVR32_PWMA_26_FUNCTION
#define PWM27_PWM_FUNCTION	AVR32_PWMA_27_FUNCTION
#define PWM28_PWM_FUNCTION	AVR32_PWMA_28_FUNCTION
#define PWM29_PWM_FUNCTION	AVR32_PWMA_29_FUNCTION
#define PWM30_PWM_FUNCTION	AVR32_PWMA_30_FUNCTION
#define PWM31_PWM_FUNCTION	AVR32_PWMA_31_FUNCTION
#define PWM32_PWM_FUNCTION	AVR32_PWMA_32_FUNCTION
#define PWM33_PWM_FUNCTION	AVR32_PWMA_33_FUNCTION
#define PWM34_PWM_FUNCTION	AVR32_PWMA_34_FUNCTION
#define PWM35_PWM_FUNCTION	AVR32_PWMA_35_FUNCTION
//! @}


/*! \name PWMs of PWMA Connections
 */
//! @{
#define PWM_CH1		(0x400000ULL) | 0x1ULL		//A22 // 22
#define PWM_CH2		(0x10000000ULL) | 0x1ULL	//B05 // 28
#define PWM_CH3		(0x8000000ULL) | 0x1ULL		//B04 // 27
#define PWM_CH4		(0x200000000ULL) | 0x1ULL	//B10 // 33
#define PWM_CH5		(0x200000ULL) | 0x1ULL		//A21 // 21
#define PWM_CH6		(0x100000ULL) | 0x1ULL		//A20 // 20
#define PWM_CH7		(0x2000ULL) | 0x1ULL		//A13 // 13
#define PWM_CH8		(0x800ULL) | 0x1ULL			//A11 // 11
#define PWM_CH9		(0x100000000ULL) | 0x1ULL	//B09 // 32
#define PWM_CH10	(0x20000000ULL) | 0x1ULL	//B06 // 29
#define PWM_CH11	(0x40000000ULL) | 0x1ULL	//B07 // 30
#define PWM_CH12	(0x80000000ULL) | 0x1ULL	//B08 // 31

#define PWM_CH13	(0x4000000ULL) | 0x1ULL		//B03 // 26
#define PWM_CH14	(0x2000000ULL) | 0x1ULL		//B02 // 25
#define PWM_CH15	(0x800000ULL) | 0x1ULL		//B00 // 23
#define PWM_CH16	(0x800000000ULL) | 0x1ULL	//B12 // 35
#define PWM_CH17	(0x200ULL)  | 0x1ULL		//A09 // 9
#define PWM_CH18	(0x1000ULL) | 0x1ULL		//A12 // 12
#define PWM_CH19	(0x400000000ULL) | 0x1ULL	//B11 // 34
#define PWM_CH20	(0x40000ULL) | 0x1ULL		//A18 // 18
#define PWM_CH21	(0x80000ULL) | 0x1ULL		//A19 // 19
#define PWM_CH22	(0x20000ULL) | 0x1ULL		//A17 // 17
#define PWM_CH23	(0x10000ULL) | 0x1ULL		//A16 // 16
#define PWM_CH24	(0x8000ULL) | 0x1ULL		//A15 // 15
//! @}


/*! \name PWMs of GPIO Connections
 */
//! @{
#define PWM_GPIO_CH1	22	//A22
#define PWM_GPIO_CH2	37	//B05
#define PWM_GPIO_CH3	36	//B04
#define PWM_GPIO_CH4	42	//B10
#define PWM_GPIO_CH5	21	//A21
#define PWM_GPIO_CH6	20	//A20
#define PWM_GPIO_CH7	13	//A13
#define PWM_GPIO_CH8	11	//A11
#define PWM_GPIO_CH9	41	//B09
#define PWM_GPIO_CH10	38	//B06
#define PWM_GPIO_CH11	39	//B07
#define PWM_GPIO_CH12	40	//B08

#define PWM_GPIO_CH13	35	//B03
#define PWM_GPIO_CH14	34	//B02
#define PWM_GPIO_CH15	32	//B00
#define PWM_GPIO_CH16	44	//B12
#define PWM_GPIO_CH17	9	//A09
#define PWM_GPIO_CH18	12	//A12
#define PWM_GPIO_CH19	43	//B11
#define PWM_GPIO_CH20	18	//A18
#define PWM_GPIO_CH21	19	//A19
#define PWM_GPIO_CH22	17	//A17
#define PWM_GPIO_CH23	16	//A16
#define PWM_GPIO_CH24	15	//A15
//! @}







/*! \name Miscellaneous data
 */
//! @{

//! The ADVREF value in Volt == VDDANA
#define AT32UC3L_EK_ADVREF_V      (1.8F)

//! @}


//! Number of LEDs.
#define LED_COUNT   4

/*! \name GPIO Connections of LEDs
 */
//! @{
#  define LED0_GPIO   AVR32_PIN_PA21
#  define LED1_GPIO   AVR32_PIN_PA20
#  define LED3_GPIO   AVR32_PIN_PA13
#if defined(UC3L_EK_REV1)
#  define LED2_GPIO   AVR32_PIN_PA15
#else // UC3L_EK_REV2
#  define LED2_GPIO   AVR32_PIN_PB02
#endif
//! @}


/*! \name PWM Channels of LEDs
 */
//! @{
#define LED0_PWM      21
#define LED1_PWM      20
#define LED3_PWM      13
#if defined(UC3L_EK_REV1)
#define LED2_PWM      15
#else // UC3L_EK_REV2
#define LED2_PWM      25
#endif
//! @}

/*! \name PWM Functions of LEDs
 */
//! @{

//! Bit mask for the PWMA channels connected to the LEDs
#if defined(UC3L_EK_REV1)
#define LED_PWMA_CHANNELS_MASK  0x204208000ULL
#else // UC3L_EK_REV2
#define LED_PWMA_CHANNELS_MASK  0x206200000ULL
#endif


#define LED0_PWM_FUNCTION   AVR32_PWMA_21_FUNCTION
#define LED1_PWM_FUNCTION   AVR32_PWMA_20_FUNCTION
#define LED3_PWM_FUNCTION   AVR32_PWMA_13_FUNCTION
#if defined(UC3L_EK_REV1)
#define LED2_PWM_FUNCTION   AVR32_PWMA_15_FUNCTION
#else
#define LED2_PWM_FUNCTION   AVR32_PWMA_25_FUNCTION
#endif



//! @}

/*! \name GPIO Connections of Push Buttons
 */
//! @{
#define GPIO_WAKE_BUTTON          AVR32_PIN_PA11
#define GPIO_WAKE_BUTTON_PRESSED  0
//! @}

/*! \name SPI Connections of the AT45DBX Data Flash Memory
 */
//! @{
#define AT45DBX_SPI                 (&AVR32_SPI)
#define AT45DBX_SPI_NPCS            0
#define AT45DBX_SPI_SCK_PIN         AVR32_SPI_SCK_0_0_PIN
#define AT45DBX_SPI_SCK_FUNCTION    AVR32_SPI_SCK_0_0_FUNCTION
#define AT45DBX_SPI_MISO_PIN        AVR32_SPI_MISO_0_0_PIN
#define AT45DBX_SPI_MISO_FUNCTION   AVR32_SPI_MISO_0_0_FUNCTION
#define AT45DBX_SPI_MOSI_PIN        AVR32_SPI_MOSI_0_0_PIN
#define AT45DBX_SPI_MOSI_FUNCTION   AVR32_SPI_MOSI_0_0_FUNCTION
#define AT45DBX_SPI_NPCS0_PIN       AVR32_SPI_NPCS_0_0_PIN
#define AT45DBX_SPI_NPCS0_FUNCTION  AVR32_SPI_NPCS_0_0_FUNCTION
//! @}

/*! \name USART connection to the UC3B board controller
 */
//! @{
#define USART                       (&AVR32_USART3)
#define USART_RXD_PIN               AVR32_USART3_RXD_0_0_PIN
#define USART_RXD_FUNCTION          AVR32_USART3_RXD_0_0_FUNCTION
#define USART_TXD_PIN               AVR32_USART3_TXD_0_0_PIN
#define USART_TXD_FUNCTION          AVR32_USART3_TXD_0_0_FUNCTION
#define USART_IRQ                   AVR32_USART3_IRQ
#define USART_IRQ_GROUP             AVR32_USART3_IRQ_GROUP
#define USART_SYSCLK                SYSCLK_USART3
//! @}


/*! \name ADC Connections of the Accelerometer
 */
//! @{
#define ADC_ACC_X_CHANNEL           6
#define ADC_ACC_X_CHANNEL_MASK      AVR32_ADCIFB_CH6_MASK
#define ADC_ACC_X_PIN               AVR32_ADCIFB_AD_6_PIN
#define ADC_ACC_X_FUNCTION          AVR32_ADCIFB_AD_6_FUNCTION
#define ADC_ACC_Y_CHANNEL           7
#define ADC_ACC_Y_CHANNEL_MASK      AVR32_ADCIFB_CH7_MASK
#define ADC_ACC_Y_PIN               AVR32_ADCIFB_AD_7_PIN
#define ADC_ACC_Y_FUNCTION          AVR32_ADCIFB_AD_7_FUNCTION
#define ADC_ACC_Z_CHANNEL           8
#define ADC_ACC_Z_CHANNEL_MASK      AVR32_ADCIFB_CH8_MASK
#define ADC_ACC_Z_PIN               AVR32_ADCIFB_AD_8_PIN
#define ADC_ACC_Z_FUNCTION          AVR32_ADCIFB_AD_8_FUNCTION
//! @}

/*! \name QMatrix connections of the touch sensors
 */
//! @{

//! Wheel X connections
#if defined(UC3L_EK_REV1)
#define QMATRIX_WHEEL_X8_UPRIGHT_PIN          AVR32_CAT_CSA_8_PIN       // X1 on schematics, DISCONNECTED ON AT32UC3L-EK REV1
#define QMATRIX_WHEEL_X8_UPRIGHT_FUNCTION     AVR32_CAT_CSA_8_FUNCTION
#define QMATRIX_WHEEL_X9_RIGHT_PIN            AVR32_CAT_CSB_8_PIN       // X2 on schematics
#define QMATRIX_WHEEL_X9_RIGHT_FUNCTION       AVR32_CAT_CSB_8_FUNCTION
#else // UC3L_EK_REV2
#define QMATRIX_WHEEL_X6_UPRIGHT_PIN          AVR32_CAT_CSA_6_PIN       // X1 on schematics
#define QMATRIX_WHEEL_X6_UPRIGHT_FUNCTION     AVR32_CAT_CSA_6_FUNCTION
#define QMATRIX_WHEEL_X7_RIGHT_PIN            AVR32_CAT_CSB_6_PIN       // X2 on schematics
#define QMATRIX_WHEEL_X7_RIGHT_FUNCTION       AVR32_CAT_CSB_6_FUNCTION
#endif
#define QMATRIX_WHEEL_X10_DOWNRIGHT_PIN       AVR32_CAT_CSA_10_PIN      // X3 on schematics
#define QMATRIX_WHEEL_X10_DOWNRIGHT_FUNCTION  AVR32_CAT_CSA_10_FUNCTION
#define QMATRIX_WHEEL_X11_DOWNLEFT_PIN        AVR32_CAT_CSB_10_PIN      // X4 on schematics
#define QMATRIX_WHEEL_X11_DOWNLEFT_FUNCTION   AVR32_CAT_CSB_10_FUNCTION
#define QMATRIX_WHEEL_X14_LEFT_PIN            AVR32_CAT_CSA_14_PIN      // X5 on schematics
#define QMATRIX_WHEEL_X14_LEFT_FUNCTION       AVR32_CAT_CSA_14_FUNCTION
#define QMATRIX_WHEEL_X15_UPLEFT_PIN          AVR32_CAT_CSB_14_PIN      // X6 on schematics
#define QMATRIX_WHEEL_X15_UPLEFT_FUNCTION     AVR32_CAT_CSB_14_FUNCTION

//! Wheel Y connections
#define QMATRIX_WHEEL_Y2_PIN                  AVR32_CAT_CSA_5_PIN       // Y0 on schematics
#define QMATRIX_WHEEL_Y2_FUNCTION             AVR32_CAT_CSA_5_FUNCTION
#define QMATRIX_WHEEL_YK2_PIN                 AVR32_CAT_CSB_5_PIN       // YK0 on schematics
#define QMATRIX_WHEEL_YK2_FUNCTION            AVR32_CAT_CSB_5_FUNCTION

#if defined(UC3L_EK_REV1)
#define QMATRIX_SMP_PIN                       AVR32_CAT_SMP_0_1_PIN
#define QMATRIX_SMP_FUNCTION                  AVR32_CAT_SMP_0_1_FUNCTION
#else // UC3L_EK_REV2
#define QMATRIX_SMP_PIN                       AVR32_CAT_SMP_0_4_PIN
#define QMATRIX_SMP_FUNCTION                  AVR32_CAT_SMP_0_4_FUNCTION
#endif

#if !defined(UC3L_EK_REV1)

#if (defined (__GNUC__) && ( defined (__AVR32_UC3L016__)     || \
                            defined (__AVR32_UC3L032__)     || \
                            defined (__AVR32_UC3L064__))) \
  ||(defined(__ICCAVR32__) || defined(__AAVR32__)) && \
                          ( defined (__AT32UC3L016__)     || \
                            defined (__AT32UC3L032__)     || \
                            defined (__AT32UC3L064__))
#define AVR32_CAT_VDIVEN_0_PIN                43
#define AVR32_CAT_VDIVEN_0_FUNCTION           5
// These defines are missing from or wrong in the toolchain header file uc3l064.h
// #11171
#endif


#define QMATRIX_VDIVEN_PIN                    AVR32_CAT_VDIVEN_0_PIN
#define QMATRIX_VDIVEN_FUNCTION               AVR32_CAT_VDIVEN_0_FUNCTION
#endif

//! Buttons X connections
#if defined(UC3L_EK_REV1)
#define QMATRIX_BTNS_DOWN_X8_PIN              AVR32_CAT_CSA_8_PIN       // X1 on schematics, DISCONNECTED ON AT32UC3L-EK REV1
#define QMATRIX_BTNS_DOWN_X8_FUNCTION         AVR32_CAT_CSA_8_FUNCTION
#define QMATRIX_BTNS_UP_X9_PIN                AVR32_CAT_CSB_8_PIN       // X2 on schematics
#define QMATRIX_BTNS_UP_X9_FUNCTION           AVR32_CAT_CSB_8_FUNCTION
#else // UC3L_EK_REV2
#define QMATRIX_BTNS_DOWN_X6_PIN              AVR32_CAT_CSA_6_PIN       // X1 on schematics
#define QMATRIX_BTNS_DOWN_X6_FUNCTION         AVR32_CAT_CSA_6_FUNCTION
#define QMATRIX_BTNS_UP_X7_PIN                AVR32_CAT_CSB_6_PIN       // X2 on schematics
#define QMATRIX_BTNS_UP_X7_FUNCTION           AVR32_CAT_CSB_6_FUNCTION
#endif
#define QMATRIX_BTNS_LEFT_X10_PIN             AVR32_CAT_CSA_10_PIN      // X3 on schematics
#define QMATRIX_BTNS_LEFT_X10_FUNCTION        AVR32_CAT_CSA_10_FUNCTION
#define QMATRIX_BTNS_PLAYPAUSE_X14_PIN        AVR32_CAT_CSA_14_PIN      // X5 on schematics
#define QMATRIX_BTNS_PLAYPAUSE_X14_FUNCTION   AVR32_CAT_CSA_14_FUNCTION
#define QMATRIX_BTNS_RIGHT_X15_PIN            AVR32_CAT_CSB_14_PIN      // X6 on schematics
#define QMATRIX_BTNS_RIGHT_X15_FUNCTION       AVR32_CAT_CSB_14_FUNCTION

//! Buttons Y connections
#define QMATRIX_BTNS_Y7_PIN                   AVR32_CAT_CSA_15_PIN      // Y1 on schematics
#define QMATRIX_BTNS_Y7_FUNCTION              AVR32_CAT_CSA_15_FUNCTION
#define QMATRIX_BTNS_YK7_PIN                  AVR32_CAT_CSB_15_PIN      // YK1 on schematics
#define QMATRIX_BTNS_YK7_FUNCTION             AVR32_CAT_CSB_15_FUNCTION

//! Number of X pins
#if defined( UC3L_EK_REV1 )
#define QMATRIX_NB_X                          6 // X8, X9, X10, X11, X14, X15
#else // UC3L_EK_REV2
#define QMATRIX_NB_X                          6 // X6, X7, X10, X11, X14, X15
#endif

//! Number of Y pins
#define QMATRIX_NB_Y                          2 // Y2, Y7

//! Y pins id for buttons and for wheel
#define Y_BUTTONS                             7 // Y7
#define Y_WHEEL                               2 // Y2

//! @}

#if defined(UC3L_EK_REV1)
/*! \name Connections of the AT86RF231 chip
 */
//! @{
#define AT86RF231_SPI                 (&AVR32_SPI)
#define AT86RF231_SPI_NPCS            3
#define AT86RF231_SPI_SCK_PIN         AVR32_SPI_SCK_0_0_PIN
#define AT86RF231_SPI_SCK_FUNCTION    AVR32_SPI_SCK_0_0_FUNCTION
#define AT86RF231_SPI_MISO_PIN        AVR32_SPI_MISO_0_0_PIN
#define AT86RF231_SPI_MISO_FUNCTION   AVR32_SPI_MISO_0_0_FUNCTION
#define AT86RF231_SPI_MOSI_PIN        AVR32_SPI_MOSI_0_0_PIN
#define AT86RF231_SPI_MOSI_FUNCTION   AVR32_SPI_MOSI_0_0_FUNCTION
#define AT86RF231_SPI_NPCS3_PIN       AVR32_SPI_NPCS_3_0_PIN
#define AT86RF231_SPI_NPCS3_FUNCTION  AVR32_SPI_NPCS_3_0_FUNCTION
#define AT86RF231_NPRST               AVR32_PIN_PA01
#define AT86RF231_SLPTR               AVR32_PIN_PA00
#define AT86RF231_OB_IRQ              AVR32_PIN_PB11
#define AT86RF231_OB_OSCIN            AVR32_PIN_PA08
//! @}
#else // UC3L_EK_REV2
/*! \name Signals on the WLESS connector.
 */
//! @{
#define WLESS_SPI                       (&AVR32_SPI)
#define WLESS_SPI_MISO_PIN              AVR32_SPI_MISO_0_0_PIN          // J8.7
#define WLESS_SPI_MISO_FUNCTION         AVR32_SPI_MISO_0_0_FUNCTION
#define WLESS_SPI_MOSI_PIN              AVR32_SPI_MOSI_0_0_PIN          // J8.6
#define WLESS_SPI_MOSI_FUNCTION         AVR32_SPI_MOSI_0_0_FUNCTION
#define WLESS_SPI_SCK_PIN               AVR32_SPI_SCK_0_0_PIN           // J8.8
#define WLESS_SPI_SCK_FUNCTION          AVR32_SPI_SCK_0_0_FUNCTION
#define WLESS_SPI_NPCS1_PIN             AVR32_SPI_NPCS_1_PIN            // J8.5
#define WLESS_SPI_NPCS1_FUNCTION        AVR32_SPI_NPCS_1_FUNCTION

#define WLESS_UART1_TX_PIN              AVR32_USART1_TXD_0_0_PIN        // J8.4
#define WLESS_UART1_TX_FUNCTION         AVR32_USART1_TXD_0_0_FUNCTION
#define WLESS_UART1_RX_PIN              AVR32_USART1_RXD_0_0_PIN        // J8.3
#define WLESS_UART1_RX_FUNCTION         AVR32_USART1_RXD_0_0_FUNCTION
#define WLESS_UART1_RTS_PIN             AVR32_USART1_RTS_0_0_PIN        // J8.1 & J44.3-5 (jumper)
#define WLESS_UART1_RTS_FUNCTION        AVR32_USART1_RTS_0_0_FUNCTION
#define WLESS_UART1_CTS_PIN             AVR32_USART1_CTS_0_0_PIN        // J8.2 & J44.4-6 (jumper)
#define WLESS_UART1_CTS_FUNCTION        AVR32_USART1_CTS_0_0_FUNCTION

#define WLESS_TWIMS0_TWD_PIN            AVR32_TWIMS0_TWD_0_2_PIN        // J8.1 & J44.1-3 (jumper)
#ifndef AVR32_TWIMS0_TWD_0_2_FUNCTION
#define AVR32_TWIMS0_TWD_0_2_FUNCTION   6
// These defines are missing from or wrong in the toolchain header file uc3l064.h
#endif
#define WLESS_TWIMS0_TWD_FUNCTION       AVR32_TWIMS0_TWD_0_2_FUNCTION
#define WLESS_TWIMS0_TWCK_PIN           AVR32_TWIMS0_TWCK_0_0_PIN       // J8.1 & J44.2-4 (jumper)
#define WLESS_TWIMS0_TWCK_FUNCTION      AVR32_TWIMS0_TWCK_0_0_FUNCTION

#define WLESS_RZ600_RST_PIN             AVR32_PIN_PA00                  // J8.1 & J44.3-5 (jumper)
#define WLESS_RZ600_MISC_PIN            AVR32_PIN_PA01                  // J8.2 & J44.4-6 (jumper)
#define WLESS_RZ600_SLPTR_PIN           AVR32_PIN_PA08                  // J8.4
#define WLESS_RZ600_IRQ_PIN             AVR32_PIN_PA09                  // J8.3

#define WLESS_RZ600_SPI                 WLESS_SPI
#define WLESS_RZ600_SPI_MISO_PIN        WLESS_SPI_MISO_PIN              // J8.7
#define WLESS_RZ600_SPI_MISO_FUNCTION   WLESS_SPI_MISO_FUNCTION
#define WLESS_RZ600_SPI_MOSI_PIN        WLESS_SPI_MOSI_PIN              // J8.6
#define WLESS_RZ600_SPI_MOSI_FUNCTION   WLESS_SPI_MOSI_FUNCTION
#define WLESS_RZ600_SPI_SCK_PIN         WLESS_SPI_SCK_PIN               // J8.8
#define WLESS_RZ600_SPI_SCK_FUNCTION    WLESS_SPI_SCK_FUNCTION
#define WLESS_RZ600_SPI_NPCS1_PIN       WLESS_SPI_NPCS1_PIN             // J8.5
#define WLESS_RZ600_SPI_NPCS1_FUNCTION  WLESS_SPI_NPCS1_FUNCTION
//! @}
#endif

/*! \name Battery connection
 */
//! @{
#define AT32UC3L_EK_VBAT_PIN          AVR32_ADCIFB_AD_4_PIN
#define AT32UC3L_EK_VBAT_FUNCTION     AVR32_ADCIFB_AD_4_FUNCTION
#define AT32UC3L_EK_VBAT_ADC_CHANNEL  AVR32_ADCIFB_CH4_MASK
//! @}

/*! \name USB mode button (connected to UC3B1256)
 */
//! @{
#define UC3B_GPIO_USB_MODE_BUTTON     AVR32_PIN_PA03
//! @}
#endif  // _UC3L_EK_H_
