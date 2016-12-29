/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "tft.h"
#include "usbd_cdc_if.h"
#include "bipbuffer.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

DAC_HandleTypeDef hdac;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_tx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
void *bip;
extern uint8_t dataReady;
extern uint8_t buf2[256];
int snoop = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config ( void );
void Error_Handler ( void );
static void MX_GPIO_Init ( void );
static void MX_DMA_Init ( void );
static void MX_CAN1_Init ( void );
static void MX_CAN2_Init ( void );
static void MX_SPI1_Init ( void );
static void MX_SPI2_Init ( void );
static void MX_DAC_Init ( void );

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/** \brief  System Reset
    The function initiates a system reset request to reset the MCU.
 */
__STATIC_INLINE void NVIC_SystemResetX ( void )
{
    __DSB();                                                     /* Ensure all outstanding memory accesses included
                                                              buffered write are completed before reset */
    SCB->AIRCR  = ( ( 0x5FA << SCB_AIRCR_VECTKEY_Pos )      |
                    ( SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk ) |
                    SCB_AIRCR_SYSRESETREQ_Msk );                  /* Keep priority group unchanged */
    __DSB();                                                     /* Ensure completion of memory access */

    while ( 1 );                                                 /* wait until reset */
}

int SetupCANFilter ( CAN_HandleTypeDef *obj, uint32_t id, uint32_t mask, uint32_t format, uint32_t handle )
{
    int retval = -1;

    if ( ( format == CAN_ID_STD ) || ( format == CAN_ID_EXT ) ) {

        CAN_FilterConfTypeDef  sFilterConfig;

        sFilterConfig.FilterNumber = handle;
        sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
        sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

        if ( format == CAN_ID_STD ) {
            sFilterConfig.FilterIdHigh = id << 5;
            sFilterConfig.FilterIdLow =  0x0;
            sFilterConfig.FilterMaskIdHigh = mask << 5;
            sFilterConfig.FilterMaskIdLow = 0x0;    // allows both remote and data frames

        } else
            if ( format == CAN_ID_EXT ) {
                sFilterConfig.FilterIdHigh = id >> 13;  // EXTID[28:13]
                sFilterConfig.FilterIdLow = ( 0x00FF & ( id << 3 ) ) | ( 1 << 2 ); // EXTID[12:0]
                sFilterConfig.FilterMaskIdHigh = mask >> 13;
                sFilterConfig.FilterMaskIdLow = ( 0x00FF & ( mask << 3 ) ) | ( 1 << 2 );
            }

        sFilterConfig.FilterFIFOAssignment = 0;
        sFilterConfig.FilterActivation = ENABLE;
        sFilterConfig.BankNumber = 14 + handle;

        HAL_CAN_ConfigFilter ( obj, &sFilterConfig );
        retval = handle;
    }

    return retval;
}

/* USER CODE END 0 */

int main ( void )
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_CAN1_Init();
    MX_CAN2_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_FATFS_Init();
    MX_USB_DEVICE_Init();
    MX_DAC_Init();

    /* USER CODE BEGIN 2 */

    CAN_FilterConfTypeDef  sFilterConfig;

    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.BankNumber = 14;

    if ( HAL_CAN_ConfigFilter ( &hcan1, &sFilterConfig ) != HAL_OK ) {
        /* Filter configuration Error */
        Error_Handler();
    }

    tft_init();
    tft_clear ( TFT_RED );


    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while ( 1 ) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */



        {
            uint32_t status;

#if 0


            if ( HAL_CAN_GetState ( &hcan1 ) != HAL_CAN_STATE_READY ) {
                uint8_t buffer[64];
                sprintf ( ( char* ) buffer, "can not ready 0x%x\n", HAL_CAN_GetState ( &hcan1 ) );
                CDC_Transmit_HS ( &buffer[0], strlen ( ( ( const char * ) buffer ) ) );
                HAL_Delay ( 100 );
            }

#endif

            if ( 1 == snoop ) {

                status = HAL_CAN_Receive ( &hcan1, CAN_FIFO0, 100 );

                if ( HAL_OK == status ) {
                    uint8_t buffer[100];

                    sprintf ( ( char* ) buffer, "0x%03x(0x%03x) %d %d (%d) 0x%02x 0x%02x 0x%02x 0x%02x - 0x%02x 0x%02x 0x%02x 0x%02x\n",
                              hcan1.pRxMsg->StdId ,
                              hcan1.pRxMsg->ExtId ,
                              hcan1.pRxMsg->IDE,
                              hcan1.pRxMsg->RTR,
                              hcan1.pRxMsg->DLC,
                              hcan1.pRxMsg->Data[0], hcan1.pRxMsg->Data[1], hcan1.pRxMsg->Data[2], hcan1.pRxMsg->Data[3],
                              hcan1.pRxMsg->Data[4], hcan1.pRxMsg->Data[5], hcan1.pRxMsg->Data[6], hcan1.pRxMsg->Data[7] );

                    CDC_Transmit_HS ( &buffer[0], strlen ( ( ( const char * ) buffer ) )  );

                } else
                    if ( HAL_CAN_STATE_TIMEOUT != status ) {
                        uint8_t buffer[64];
                        sprintf ( ( char* ) buffer, "rx err 0x%x\n", hcan1.State );
                        CDC_Transmit_HS ( buffer, strlen ( ( ( const char * ) buffer ) ) );
                    }
            }
        }


        //. unsigned char *data = bipbuf_poll ( bip, 1 ) ;

        if ( dataReady ) {
            CDC_Transmit_HS ( &buf2[0], dataReady );
            dataReady = 0;

            switch ( buf2[0] ) {
                case 'l':
                    snoop = 1 - snoop;
                    break;

                case 'r':
                    tft_clear ( TFT_RED );
                    break;

                case 'g':
                    tft_clear ( TFT_GREEN );
                    break;

                case 'b':
                    tft_clear ( TFT_BLUE );
                    break;

                case ' ':
                    tft_clear ( TFT_BLACK );
                    break;

                case 'R':
                    NVIC_SystemResetX();
                    break;

                case 's': {
                        uint8_t buffer[64];
                        sprintf ( ( char* ) buffer, "=TSR=0x%x (%x %x %x)\n",

                                  hcan1.Instance->TSR,
                                  __HAL_CAN_TRANSMIT_STATUS ( &hcan1, CAN_TXMAILBOX_0 ),
                                  __HAL_CAN_TRANSMIT_STATUS ( &hcan1, CAN_TXMAILBOX_1 ),
                                  __HAL_CAN_TRANSMIT_STATUS ( &hcan1, CAN_TXMAILBOX_2 )
                                );
                        CDC_Transmit_HS ( buffer, strlen ( ( ( const char * ) buffer ) ) );
                    }
                    break;

                case 't':
                    hcan1.pTxMsg->StdId = 0x7df;
                    hcan1.pTxMsg->ExtId = 0;
                    hcan1.pTxMsg->RTR = CAN_RTR_DATA;
                    hcan1.pTxMsg->IDE = CAN_ID_STD;
                    hcan1.pTxMsg->DLC = 2;
                    hcan1.pTxMsg->Data[0] = 0x3e;
                    hcan1.pTxMsg->Data[1] = 0x80;
                    hcan1.pTxMsg->Data[2] = 0xaa;
                    hcan1.pTxMsg->Data[3] = 0xff;
                    hcan1.pTxMsg->Data[4] = 0xaa;
                    hcan1.pTxMsg->Data[5] = 0xff;
                    hcan1.pTxMsg->Data[6] = 0xaa;
                    hcan1.pTxMsg->Data[7] = 0xff;


                    if ( HAL_OK != HAL_CAN_Transmit ( &hcan1, 1 ) ) {
                        uint8_t buffer[64];
                        sprintf ( ( char* ) buffer, "tx err 0x%x 0x%x\n", hcan1.State , HAL_CAN_GetError ( &hcan1 ) );
                        CDC_Transmit_HS ( buffer, strlen ( ( ( const char * ) buffer ) ) );

                    }

                    break;
            }
        }
    }


    /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config ( void )
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG ( PWR_REGULATOR_VOLTAGE_SCALE1 );

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    RCC_OscInitStruct.PLL.PLLR = 2;

    if ( HAL_RCC_OscConfig ( &RCC_OscInitStruct ) != HAL_OK ) {
        Error_Handler();
    }

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if ( HAL_RCC_ClockConfig ( &RCC_ClkInitStruct, FLASH_LATENCY_5 ) != HAL_OK ) {
        Error_Handler();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;

    if ( HAL_RCCEx_PeriphCLKConfig ( &PeriphClkInitStruct ) != HAL_OK ) {
        Error_Handler();
    }

    /**Configure the Systick interrupt time
    */
    HAL_SYSTICK_Config ( HAL_RCC_GetHCLKFreq() / 1000 );

    /**Configure the Systick
    */
    HAL_SYSTICK_CLKSourceConfig ( SYSTICK_CLKSOURCE_HCLK );

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority ( SysTick_IRQn, 0, 0 );
}
/* quanta 1+14+6 = 21, 21 * 4 = 84, 42000000 / 84 = 5000000 */
/* CAN Baudrate = 500Kbps (CAN clocked at 42 MHz) Prescale = 4 */
/* 42*1000*1000/(((1+14+6)*4))*/
/* Requires a clock with integer division into APB clock */

// APB1=42000000/7/500000 = 12
// prescaler = 12
// sj1 = 1
// bs1= 1 bs2 = 5
// 42*1000*1000/(((1+5+1)*12))

// 6/1/11/2 = nope 85.7%

// APB1=42000000/7/500000 = 12
// 7/1/10/1 = yes 91.7%
// 7/3/ 8/1 = yes 75%

//500	0.0000	4	21	15	5	76.2	 0x004e0003
//500	0.0000	6	14	10	3	78.6	 0x00290005

/* CAN1 init function */
static void MX_CAN1_Init ( void )
{
    static CanRxMsgTypeDef rx_msg;
    static CanTxMsgTypeDef tx_msg;

    hcan1.pRxMsg = &rx_msg;
    hcan1.pTxMsg = &tx_msg;

    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 7;
    hcan1.Init.Mode = CAN_MODE_NORMAL;
    hcan1.Init.SJW = CAN_SJW_1TQ;
    hcan1.Init.BS1 = CAN_BS1_8TQ;
    hcan1.Init.BS2 = CAN_BS2_3TQ;
    hcan1.Init.TTCM = DISABLE;
    hcan1.Init.ABOM = DISABLE;
    hcan1.Init.AWUM = DISABLE;
    hcan1.Init.NART = DISABLE;
    hcan1.Init.RFLM = DISABLE;
    hcan1.Init.TXFP = DISABLE;

    if ( HAL_CAN_Init ( &hcan1 ) != HAL_OK ) {
        Error_Handler();
    }


}

/* CAN2 init function */
static void MX_CAN2_Init ( void )
{

    hcan2.Instance = CAN2;
    hcan2.Init.Prescaler = 7;
    hcan2.Init.Mode = CAN_MODE_NORMAL;
    hcan2.Init.SJW = CAN_SJW_1TQ;
    hcan2.Init.BS1 = CAN_BS1_8TQ;
    hcan2.Init.BS2 = CAN_BS2_3TQ;
    hcan2.Init.TTCM = DISABLE;
    hcan2.Init.ABOM = DISABLE;
    hcan2.Init.AWUM = DISABLE;
    hcan2.Init.NART = DISABLE;
    hcan2.Init.RFLM = DISABLE;
    hcan2.Init.TXFP = DISABLE;

    if ( HAL_CAN_Init ( &hcan2 ) != HAL_OK ) {
        Error_Handler();
    }

}

/* DAC init function */
static void MX_DAC_Init ( void )
{

    DAC_ChannelConfTypeDef sConfig;

    /**DAC Initialization
    */
    hdac.Instance = DAC;

    if ( HAL_DAC_Init ( &hdac ) != HAL_OK ) {
        Error_Handler();
    }

    /**DAC channel OUT1 config
    */
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    if ( HAL_DAC_ConfigChannel ( &hdac, &sConfig, DAC_CHANNEL_1 ) != HAL_OK ) {
        Error_Handler();
    }

}

/* SPI1 init function */
static void MX_SPI1_Init ( void )
{

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;

    if ( HAL_SPI_Init ( &hspi1 ) != HAL_OK ) {
        Error_Handler();
    }

}

/* SPI2 init function */
static void MX_SPI2_Init ( void )
{

    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 10;

    if ( HAL_SPI_Init ( &hspi2 ) != HAL_OK ) {
        Error_Handler();
    }

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init ( void )
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA2_Stream3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority ( DMA2_Stream3_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ ( DMA2_Stream3_IRQn );

}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init ( void )
{

    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pins : SW0_Pin SW1_Pin */
    GPIO_InitStruct.Pin = SW0_Pin | SW1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init ( GPIOB, &GPIO_InitStruct );

    /*Configure GPIO pin : SD_CS_Pin */
    GPIO_InitStruct.Pin = SD_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init ( SD_CS_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pins : TFT_CS_Pin TFT_RESET_Pin TFT_BACKLIGHT_Pin */
    GPIO_InitStruct.Pin = TFT_CS_Pin | TFT_RESET_Pin | TFT_BACKLIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init ( GPIOC, &GPIO_InitStruct );

    /*Configure GPIO pin : TFT_DC_Pin */
    GPIO_InitStruct.Pin = TFT_DC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init ( TFT_DC_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin ( SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin ( GPIOC, TFT_CS_Pin | TFT_RESET_Pin | TFT_BACKLIGHT_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin ( TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET );

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler ( void )
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while ( 1 ) {
    }

    /* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed ( uint8_t* file, uint32_t line )
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
