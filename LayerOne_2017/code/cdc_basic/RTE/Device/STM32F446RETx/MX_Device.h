/******************************************************************************
 * File Name   : MX_Device.h
 * Date        : 23/12/2016 13:27:17
 * Description : STM32Cube MX parameter definitions
 * Note        : This file is generated by STM32CubeMX (DO NOT EDIT!)
 ******************************************************************************/

#ifndef __MX_DEVICE_H
#define __MX_DEVICE_H

/*---------------------------- Clock Configuration ---------------------------*/

#define MX_LSI_VALUE                            32000
#define MX_LSE_VALUE                            32768
#define MX_HSI_VALUE                            16000000
#define MX_HSE_VALUE                            8000000
#define MX_EXTERNAL_CLOCK_VALUE                 12288000
#define MX_PLLCLKFreq_Value                     72000000
#define MX_PLLQCLKFreq_Value                    48000000
#define MX_PLLRCLKFreq_Value                    72000000
#define MX_PLLSAIPCLKFreq_Value                 48000000
#define MX_PLLSAIQCLKFreq_Value                 48000000
#define MX_PLLI2SPCLKFreq_Value                 48000000
#define MX_PLLI2SQCLKFreq_Value                 48000000
#define MX_PLLI2SRCLKFreq_Value                 48000000
#define MX_SYSCLKFreq_VALUE                     72000000
#define MX_HCLKFreq_Value                       72000000
#define MX_FCLKCortexFreq_Value                 72000000
#define MX_CortexFreq_Value                     72000000
#define MX_AHBFreq_Value                        72000000
#define MX_APB1Freq_Value                       36000000
#define MX_APB2Freq_Value                       72000000
#define MX_APB1TimFreq_Value                    72000000
#define MX_APB2TimFreq_Value                    72000000
#define MX_CECFreq_Value                        32786
#define MX_I2S1Freq_Value                       48000000
#define MX_I2S2Freq_Value                       48000000
#define MX_SAIAFreq_Value                       48000000
#define MX_SAIBFreq_Value                       48000000
#define MX_SDIOFreq_Value                       48000000
#define MX_PWRFreq_Value                        72000000
#define MX_RTCFreq_Value                        32000
#define MX_USBFreq_Value                        48000000
#define MX_WatchDogFreq_Value                   32000
#define MX_FMPI2C1Freq_Value                    36000000
#define MX_SPDIFRXFreq_Value                    72000000
#define MX_MCO1PinFreq_Value                    16000000
#define MX_MCO2PinFreq_Value                    72000000

/*-------------------------------- CAN1       --------------------------------*/

#define MX_CAN1                                 1

/* GPIO Configuration */

/* Pin PB8 */
#define MX_CAN1_RX_GPIO_Speed                   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_CAN1_RX_Pin                          PB8
#define MX_CAN1_RX_GPIOx                        GPIOB
#define MX_CAN1_RX_GPIO_PuPd                    GPIO_NOPULL
#define MX_CAN1_RX_GPIO_Pin                     GPIO_PIN_8
#define MX_CAN1_RX_GPIO_AF                      GPIO_AF9_CAN1
#define MX_CAN1_RX_GPIO_Mode                    GPIO_MODE_AF_PP

/* Pin PB9 */
#define MX_CAN1_TX_GPIO_Speed                   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_CAN1_TX_Pin                          PB9
#define MX_CAN1_TX_GPIOx                        GPIOB
#define MX_CAN1_TX_GPIO_PuPd                    GPIO_NOPULL
#define MX_CAN1_TX_GPIO_Pin                     GPIO_PIN_9
#define MX_CAN1_TX_GPIO_AF                      GPIO_AF9_CAN1
#define MX_CAN1_TX_GPIO_Mode                    GPIO_MODE_AF_PP

/*-------------------------------- CAN2       --------------------------------*/

#define MX_CAN2                                 1

/* GPIO Configuration */

/* Pin PB5 */
#define MX_CAN2_RX_GPIO_Speed                   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_CAN2_RX_Pin                          PB5
#define MX_CAN2_RX_GPIOx                        GPIOB
#define MX_CAN2_RX_GPIO_PuPd                    GPIO_NOPULL
#define MX_CAN2_RX_GPIO_Pin                     GPIO_PIN_5
#define MX_CAN2_RX_GPIO_AF                      GPIO_AF9_CAN2
#define MX_CAN2_RX_GPIO_Mode                    GPIO_MODE_AF_PP

/* Pin PB6 */
#define MX_CAN2_TX_GPIO_Speed                   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_CAN2_TX_Pin                          PB6
#define MX_CAN2_TX_GPIOx                        GPIOB
#define MX_CAN2_TX_GPIO_PuPd                    GPIO_NOPULL
#define MX_CAN2_TX_GPIO_Pin                     GPIO_PIN_6
#define MX_CAN2_TX_GPIO_AF                      GPIO_AF9_CAN2
#define MX_CAN2_TX_GPIO_Mode                    GPIO_MODE_AF_PP

/*-------------------------------- SPI1       --------------------------------*/

#define MX_SPI1                                 1

/* GPIO Configuration */

/* Pin PA6 */
#define MX_SPI1_MISO_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SPI1_MISO_Pin                        PA6
#define MX_SPI1_MISO_GPIOx                      GPIOA
#define MX_SPI1_MISO_GPIO_PuPd                  GPIO_NOPULL
#define MX_SPI1_MISO_GPIO_Pin                   GPIO_PIN_6
#define MX_SPI1_MISO_GPIO_AF                    GPIO_AF5_SPI1
#define MX_SPI1_MISO_GPIO_Mode                  GPIO_MODE_AF_PP

/* Pin PA7 */
#define MX_SPI1_MOSI_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SPI1_MOSI_Pin                        PA7
#define MX_SPI1_MOSI_GPIOx                      GPIOA
#define MX_SPI1_MOSI_GPIO_PuPd                  GPIO_NOPULL
#define MX_SPI1_MOSI_GPIO_Pin                   GPIO_PIN_7
#define MX_SPI1_MOSI_GPIO_AF                    GPIO_AF5_SPI1
#define MX_SPI1_MOSI_GPIO_Mode                  GPIO_MODE_AF_PP

/* Pin PA5 */
#define MX_SPI1_SCK_GPIO_Speed                  GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SPI1_SCK_Pin                         PA5
#define MX_SPI1_SCK_GPIOx                       GPIOA
#define MX_SPI1_SCK_GPIO_PuPd                   GPIO_NOPULL
#define MX_SPI1_SCK_GPIO_Pin                    GPIO_PIN_5
#define MX_SPI1_SCK_GPIO_AF                     GPIO_AF5_SPI1
#define MX_SPI1_SCK_GPIO_Mode                   GPIO_MODE_AF_PP

/*-------------------------------- SPI2       --------------------------------*/

#define MX_SPI2                                 1

/* GPIO Configuration */

/* Pin PB10 */
#define MX_SPI2_SCK_GPIO_Speed                  GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SPI2_SCK_Pin                         PB10
#define MX_SPI2_SCK_GPIOx                       GPIOB
#define MX_SPI2_SCK_GPIO_PuPd                   GPIO_NOPULL
#define MX_SPI2_SCK_GPIO_Pin                    GPIO_PIN_10
#define MX_SPI2_SCK_GPIO_AF                     GPIO_AF5_SPI2
#define MX_SPI2_SCK_GPIO_Mode                   GPIO_MODE_AF_PP

/* Pin PC1 */
#define MX_SPI2_MOSI_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SPI2_MOSI_Pin                        PC1
#define MX_SPI2_MOSI_GPIOx                      GPIOC
#define MX_SPI2_MOSI_GPIO_PuPd                  GPIO_NOPULL
#define MX_SPI2_MOSI_GPIO_Pin                   GPIO_PIN_1
#define MX_SPI2_MOSI_GPIO_AF                    GPIO_AF7_SPI2
#define MX_SPI2_MOSI_GPIO_Mode                  GPIO_MODE_AF_PP

/* Pin PC2 */
#define MX_SPI2_MISO_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SPI2_MISO_Pin                        PC2
#define MX_SPI2_MISO_GPIOx                      GPIOC
#define MX_SPI2_MISO_GPIO_PuPd                  GPIO_NOPULL
#define MX_SPI2_MISO_GPIO_Pin                   GPIO_PIN_2
#define MX_SPI2_MISO_GPIO_AF                    GPIO_AF5_SPI2
#define MX_SPI2_MISO_GPIO_Mode                  GPIO_MODE_AF_PP

/*-------------------------------- SYS        --------------------------------*/

#define MX_SYS                                  1

/* GPIO Configuration */

/* Pin PA13 */
#define MX_SYS_JTMS-SWDIO_Pin                   PA13

/* Pin PA14 */
#define MX_SYS_JTCK-SWCLK_Pin                   PA14

/*-------------------------------- USB_DEVICE --------------------------------*/

#define MX_USB_DEVICE                           1

/* GPIO Configuration */

/*-------------------------------- NVIC       --------------------------------*/

#define MX_NVIC                                 1

/*-------------------------------- GPIO       --------------------------------*/

#define MX_GPIO                                 1

/* GPIO Configuration */

/* Pin PA8 */
#define MX_PA8_GPIO_Speed                       GPIO_SPEED_FREQ_LOW
#define MX_PA8_Pin                              PA8
#define MX_PA8_GPIOx                            GPIOA
#define MX_PA8_PinState                         GPIO_PIN_RESET
#define MX_PA8_GPIO_PuPd                        GPIO_NOPULL
#define MX_PA8_GPIO_Pin                         GPIO_PIN_8
#define MX_PA8_GPIO_ModeDefaultOutputPP         GPIO_MODE_OUTPUT_PP

/* Pin PC9 */
#define MX_PC9_GPIO_Speed                       GPIO_SPEED_FREQ_LOW
#define MX_PC9_Pin                              PC9
#define MX_PC9_GPIOx                            GPIOC
#define MX_PC9_PinState                         GPIO_PIN_RESET
#define MX_PC9_GPIO_PuPd                        GPIO_NOPULL
#define MX_PC9_GPIO_Pin                         GPIO_PIN_9
#define MX_PC9_GPIO_ModeDefaultOutputPP         GPIO_MODE_OUTPUT_PP

#endif  /* __MX_DEVICE_H */
