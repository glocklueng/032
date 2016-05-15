/*******************************************************************************
* File Name: uWIFI_SPI_UART.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_uWIFI_H)
#define CY_SCB_SPI_UART_uWIFI_H

#include "uWIFI.h"


/***************************************
*   SPI Initial Parameter Constants
****************************************/

#define uWIFI_SPI_MODE                   (0u)
#define uWIFI_SPI_SUB_MODE               (0u)
#define uWIFI_SPI_CLOCK_MODE             (0u)
#define uWIFI_SPI_OVS_FACTOR             (16u)
#define uWIFI_SPI_MEDIAN_FILTER_ENABLE   (0u)
#define uWIFI_SPI_LATE_MISO_SAMPLE_ENABLE (0u)
#define uWIFI_SPI_RX_DATA_BITS_NUM       (8u)
#define uWIFI_SPI_TX_DATA_BITS_NUM       (8u)
#define uWIFI_SPI_WAKE_ENABLE            (0u)
#define uWIFI_SPI_BITS_ORDER             (1u)
#define uWIFI_SPI_TRANSFER_SEPARATION    (1u)
#define uWIFI_SPI_NUMBER_OF_SS_LINES     (1u)
#define uWIFI_SPI_RX_BUFFER_SIZE         (8u)
#define uWIFI_SPI_TX_BUFFER_SIZE         (8u)

#define uWIFI_SPI_INTERRUPT_MODE         (0u)

#define uWIFI_SPI_INTR_RX_MASK           (0u)
#define uWIFI_SPI_INTR_TX_MASK           (0u)

#define uWIFI_SPI_RX_TRIGGER_LEVEL       (7u)
#define uWIFI_SPI_TX_TRIGGER_LEVEL       (0u)

#define uWIFI_SPI_BYTE_MODE_ENABLE       (0u)
#define uWIFI_SPI_FREE_RUN_SCLK_ENABLE   (0u)
#define uWIFI_SPI_SS0_POLARITY           (0u)
#define uWIFI_SPI_SS1_POLARITY           (0u)
#define uWIFI_SPI_SS2_POLARITY           (0u)
#define uWIFI_SPI_SS3_POLARITY           (0u)


/***************************************
*   UART Initial Parameter Constants
****************************************/

#define uWIFI_UART_SUB_MODE              (0u)
#define uWIFI_UART_DIRECTION             (3u)
#define uWIFI_UART_DATA_BITS_NUM         (8u)
#define uWIFI_UART_PARITY_TYPE           (2u)
#define uWIFI_UART_STOP_BITS_NUM         (2u)
#define uWIFI_UART_OVS_FACTOR            (9u)
#define uWIFI_UART_IRDA_LOW_POWER        (0u)
#define uWIFI_UART_MEDIAN_FILTER_ENABLE  (1u)
#define uWIFI_UART_RETRY_ON_NACK         (0u)
#define uWIFI_UART_IRDA_POLARITY         (0u)
#define uWIFI_UART_DROP_ON_FRAME_ERR     (0u)
#define uWIFI_UART_DROP_ON_PARITY_ERR    (0u)
#define uWIFI_UART_WAKE_ENABLE           (0u)
#define uWIFI_UART_RX_BUFFER_SIZE        (512u)
#define uWIFI_UART_TX_BUFFER_SIZE        (32u)
#define uWIFI_UART_MP_MODE_ENABLE        (0u)
#define uWIFI_UART_MP_ACCEPT_ADDRESS     (0u)
#define uWIFI_UART_MP_RX_ADDRESS         (2u)
#define uWIFI_UART_MP_RX_ADDRESS_MASK    (255u)

#define uWIFI_UART_INTERRUPT_MODE        (1u)

#define uWIFI_UART_INTR_RX_MASK          (4u)
#define uWIFI_UART_INTR_TX_MASK          (0u)

#define uWIFI_UART_RX_TRIGGER_LEVEL      (7u)
#define uWIFI_UART_TX_TRIGGER_LEVEL      (0u)

#define uWIFI_UART_BYTE_MODE_ENABLE      (0u)
#define uWIFI_UART_CTS_ENABLE            (0u)
#define uWIFI_UART_CTS_POLARITY          (0u)
#define uWIFI_UART_RTS_POLARITY          (0u)
#define uWIFI_UART_RTS_FIFO_LEVEL        (4u)

/* SPI mode enum */
#define uWIFI_SPI_SLAVE  (0u)
#define uWIFI_SPI_MASTER (1u)

/* UART direction enum */
#define uWIFI_UART_RX    (1u)
#define uWIFI_UART_TX    (2u)
#define uWIFI_UART_TX_RX (3u)


/***************************************
*   Conditional Compilation Parameters
****************************************/

#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)

    /* Mode */
    #define uWIFI_SPI_MASTER_CONST       (1u)

    /* Direction */
    #define uWIFI_RX_DIRECTION           (1u)
    #define uWIFI_TX_DIRECTION           (1u)
    #define uWIFI_UART_RX_DIRECTION      (1u)
    #define uWIFI_UART_TX_DIRECTION      (1u)

    /* Only external RX and TX buffer for Uncofigured mode */
    #define uWIFI_INTERNAL_RX_SW_BUFFER   (0u)
    #define uWIFI_INTERNAL_TX_SW_BUFFER   (0u)

    /* Get RX and TX buffer size */
    #define uWIFI_RX_BUFFER_SIZE (uWIFI_rxBufferSize)
    #define uWIFI_TX_BUFFER_SIZE (uWIFI_txBufferSize)

    /* Return true if buffer is provided */
    #define uWIFI_CHECK_RX_SW_BUFFER (NULL != uWIFI_rxBuffer)
    #define uWIFI_CHECK_TX_SW_BUFFER (NULL != uWIFI_txBuffer)

    /* Always provide global variables to support RX and TX buffers */
    #define uWIFI_INTERNAL_RX_SW_BUFFER_CONST    (1u)
    #define uWIFI_INTERNAL_TX_SW_BUFFER_CONST    (1u)

    /* Get wakeup enable option */
    #define uWIFI_SPI_WAKE_ENABLE_CONST  (1u)
    #define uWIFI_CHECK_SPI_WAKE_ENABLE  (0u != uWIFI_scbEnableWake)
    #define uWIFI_UART_WAKE_ENABLE_CONST (1u)

    /* SPI/UART: TX or RX FIFO size */
    #if (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
        #define uWIFI_SPI_UART_FIFO_SIZE (uWIFI_FIFO_SIZE)
    #else
        #define uWIFI_SPI_UART_FIFO_SIZE (uWIFI_GET_FIFO_SIZE(uWIFI_CTRL_REG & \
                                                                                    uWIFI_CTRL_BYTE_MODE))
    #endif /* (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

#else

    /* Internal RX and TX buffer: for SPI or UART */
    #if (uWIFI_SCB_MODE_SPI_CONST_CFG)

        /* SPI Direction */
        #define uWIFI_SPI_RX_DIRECTION (1u)
        #define uWIFI_SPI_TX_DIRECTION (1u)

        /* Get FIFO size */
        #if (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
            #define uWIFI_SPI_UART_FIFO_SIZE    (uWIFI_FIFO_SIZE)
        #else
            #define uWIFI_SPI_UART_FIFO_SIZE \
                                           uWIFI_GET_FIFO_SIZE(uWIFI_SPI_BYTE_MODE_ENABLE)

        #endif /* (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

        /* SPI internal RX and TX buffers */
        #define uWIFI_INTERNAL_SPI_RX_SW_BUFFER  (uWIFI_SPI_RX_BUFFER_SIZE > \
                                                                uWIFI_SPI_UART_FIFO_SIZE)
        #define uWIFI_INTERNAL_SPI_TX_SW_BUFFER  (uWIFI_SPI_TX_BUFFER_SIZE > \
                                                                uWIFI_SPI_UART_FIFO_SIZE)

        /* Internal SPI RX and TX buffer */
        #define uWIFI_INTERNAL_RX_SW_BUFFER  (uWIFI_INTERNAL_SPI_RX_SW_BUFFER)
        #define uWIFI_INTERNAL_TX_SW_BUFFER  (uWIFI_INTERNAL_SPI_TX_SW_BUFFER)

        /* Internal SPI RX and TX buffer size */
        #define uWIFI_RX_BUFFER_SIZE         (uWIFI_SPI_RX_BUFFER_SIZE + 1u)
        #define uWIFI_TX_BUFFER_SIZE         (uWIFI_SPI_TX_BUFFER_SIZE)

        /* Get wakeup enable option */
        #define uWIFI_SPI_WAKE_ENABLE_CONST  (0u != uWIFI_SPI_WAKE_ENABLE)
        #define uWIFI_UART_WAKE_ENABLE_CONST (0u)

    #else

        /* UART Direction */
        #define uWIFI_UART_RX_DIRECTION (0u != (uWIFI_UART_DIRECTION & uWIFI_UART_RX))
        #define uWIFI_UART_TX_DIRECTION (0u != (uWIFI_UART_DIRECTION & uWIFI_UART_TX))

        /* Get FIFO size */
        #if (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
            #define uWIFI_SPI_UART_FIFO_SIZE    (uWIFI_FIFO_SIZE)
        #else
            #define uWIFI_SPI_UART_FIFO_SIZE \
                                           uWIFI_GET_FIFO_SIZE(uWIFI_UART_BYTE_MODE_ENABLE)
        #endif /* (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

        /* UART internal RX and TX buffers */
        #define uWIFI_INTERNAL_UART_RX_SW_BUFFER  (uWIFI_UART_RX_BUFFER_SIZE > \
                                                                uWIFI_SPI_UART_FIFO_SIZE)
        #define uWIFI_INTERNAL_UART_TX_SW_BUFFER  (uWIFI_UART_TX_BUFFER_SIZE > \
                                                                    uWIFI_SPI_UART_FIFO_SIZE)

        /* Internal UART RX and TX buffer */
        #define uWIFI_INTERNAL_RX_SW_BUFFER  (uWIFI_INTERNAL_UART_RX_SW_BUFFER)
        #define uWIFI_INTERNAL_TX_SW_BUFFER  (uWIFI_INTERNAL_UART_TX_SW_BUFFER)

        /* Internal UART RX and TX buffer size */
        #define uWIFI_RX_BUFFER_SIZE         (uWIFI_UART_RX_BUFFER_SIZE + 1u)
        #define uWIFI_TX_BUFFER_SIZE         (uWIFI_UART_TX_BUFFER_SIZE)

        /* Get wakeup enable option */
        #define uWIFI_SPI_WAKE_ENABLE_CONST  (0u)
        #define uWIFI_UART_WAKE_ENABLE_CONST (0u != uWIFI_UART_WAKE_ENABLE)

    #endif /* (uWIFI_SCB_MODE_SPI_CONST_CFG) */

    /* Mode */
    #define uWIFI_SPI_MASTER_CONST   (uWIFI_SPI_MODE == uWIFI_SPI_MASTER)

    /* Direction */
    #define uWIFI_RX_DIRECTION ((uWIFI_SCB_MODE_SPI_CONST_CFG) ? \
                                            (uWIFI_SPI_RX_DIRECTION) : (uWIFI_UART_RX_DIRECTION))

    #define uWIFI_TX_DIRECTION ((uWIFI_SCB_MODE_SPI_CONST_CFG) ? \
                                            (uWIFI_SPI_TX_DIRECTION) : (uWIFI_UART_TX_DIRECTION))

    /* Internal RX and TX buffer: for SPI or UART. Used in conditional compilation check */
    #define uWIFI_CHECK_RX_SW_BUFFER (uWIFI_INTERNAL_RX_SW_BUFFER)
    #define uWIFI_CHECK_TX_SW_BUFFER (uWIFI_INTERNAL_TX_SW_BUFFER)

    /* Provide global variables to support RX and TX buffers */
    #define uWIFI_INTERNAL_RX_SW_BUFFER_CONST    (uWIFI_INTERNAL_RX_SW_BUFFER)
    #define uWIFI_INTERNAL_TX_SW_BUFFER_CONST    (uWIFI_INTERNAL_TX_SW_BUFFER)

    /* Wakeup for SPI */
    #define uWIFI_CHECK_SPI_WAKE_ENABLE  (uWIFI_SPI_WAKE_ENABLE_CONST)

#endif /* End (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*       Type Definitions
***************************************/

/* uWIFI_SPI_INIT_STRUCT */
typedef struct
{
    uint32 mode;
    uint32 submode;
    uint32 sclkMode;
    uint32 oversample;
    uint32 enableMedianFilter;
    uint32 enableLateSampling;
    uint32 enableWake;
    uint32 rxDataBits;
    uint32 txDataBits;
    uint32 bitOrder;
    uint32 transferSeperation;
    uint32 rxBufferSize;
    uint8* rxBuffer;
    uint32 txBufferSize;
    uint8* txBuffer;
    uint32 enableInterrupt;
    uint32 rxInterruptMask;
    uint32 rxTriggerLevel;
    uint32 txInterruptMask;
    uint32 txTriggerLevel;
    uint8 enableByteMode;
    uint8 enableFreeRunSclk;
    uint8 polaritySs;
} uWIFI_SPI_INIT_STRUCT;

/* uWIFI_UART_INIT_STRUCT */
typedef struct
{
    uint32 mode;
    uint32 direction;
    uint32 dataBits;
    uint32 parity;
    uint32 stopBits;
    uint32 oversample;
    uint32 enableIrdaLowPower;
    uint32 enableMedianFilter;
    uint32 enableRetryNack;
    uint32 enableInvertedRx;
    uint32 dropOnParityErr;
    uint32 dropOnFrameErr;
    uint32 enableWake;
    uint32 rxBufferSize;
    uint8* rxBuffer;
    uint32 txBufferSize;
    uint8* txBuffer;
    uint32 enableMultiproc;
    uint32 multiprocAcceptAddr;
    uint32 multiprocAddr;
    uint32 multiprocAddrMask;
    uint32 enableInterrupt;
    uint32 rxInterruptMask;
    uint32 rxTriggerLevel;
    uint32 txInterruptMask;
    uint32 txTriggerLevel;
    uint8 enableByteMode;
    uint8 enableCts;
    uint8 ctsPolarity;
    uint8 rtsRxFifoLevel;
    uint8 rtsPolarity;
} uWIFI_UART_INIT_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

/* SPI specific functions */
#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    void uWIFI_SpiInit(const uWIFI_SPI_INIT_STRUCT *config);
#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(uWIFI_SCB_MODE_SPI_INC)
    #define uWIFI_SpiIsBusBusy() ((uint32) (0u != (uWIFI_SPI_STATUS_REG & \
                                                              uWIFI_SPI_STATUS_BUS_BUSY)))

    #if (uWIFI_SPI_MASTER_CONST)
        void uWIFI_SpiSetActiveSlaveSelect(uint32 slaveSelect);
    #endif /*(uWIFI_SPI_MASTER_CONST) */

    #if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
        void uWIFI_SpiSetSlaveSelectPolarity(uint32 slaveSelect, uint32 polarity);
    #endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */
#endif /* (uWIFI_SCB_MODE_SPI_INC) */

/* UART specific functions */
#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    void uWIFI_UartInit(const uWIFI_UART_INIT_STRUCT *config);
#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(uWIFI_SCB_MODE_UART_INC)
    void uWIFI_UartSetRxAddress(uint32 address);
    void uWIFI_UartSetRxAddressMask(uint32 addressMask);

    /* UART RX direction APIs */
    #if(uWIFI_UART_RX_DIRECTION)
        uint32 uWIFI_UartGetChar(void);
        uint32 uWIFI_UartGetByte(void);

        #if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
            /* UART APIs for Flow Control */
            void uWIFI_UartSetRtsPolarity(uint32 polarity);
            void uWIFI_UartSetRtsFifoLevel(uint32 level);
        #endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */
    #endif /* (uWIFI_UART_RX_DIRECTION) */

    /* UART TX direction APIs */
    #if(uWIFI_UART_TX_DIRECTION)
        #define uWIFI_UartPutChar(ch)    uWIFI_SpiUartWriteTxData((uint32)(ch))
        void uWIFI_UartPutString(const char8 string[]);
        void uWIFI_UartPutCRLF(uint32 txDataByte);

        #if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
            /* UART APIs for Flow Control */
            void uWIFI_UartEnableCts(void);
            void uWIFI_UartDisableCts(void);
            void uWIFI_UartSetCtsPolarity(uint32 polarity);
        #endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */
    #endif /* (uWIFI_UART_TX_DIRECTION) */
#endif /* (uWIFI_SCB_MODE_UART_INC) */

/* Common APIs RX direction */
#if(uWIFI_RX_DIRECTION)
    uint32 uWIFI_SpiUartReadRxData(void);
    uint32 uWIFI_SpiUartGetRxBufferSize(void);
    void   uWIFI_SpiUartClearRxBuffer(void);
#endif /* (uWIFI_RX_DIRECTION) */

/* Common APIs TX direction */
#if(uWIFI_TX_DIRECTION)
    void   uWIFI_SpiUartWriteTxData(uint32 txData);
    void   uWIFI_SpiUartPutArray(const uint8 wrBuf[], uint32 count);
    void   uWIFI_SpiUartClearTxBuffer(void);
    uint32 uWIFI_SpiUartGetTxBufferSize(void);
#endif /* (uWIFI_TX_DIRECTION) */

CY_ISR_PROTO(uWIFI_SPI_UART_ISR);

#if(uWIFI_UART_RX_WAKEUP_IRQ)
    CY_ISR_PROTO(uWIFI_UART_WAKEUP_ISR);
#endif /* (uWIFI_UART_RX_WAKEUP_IRQ) */


/***************************************
*     Buffer Access Macro Definitions
***************************************/

#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* RX direction */
    void   uWIFI_PutWordInRxBuffer  (uint32 idx, uint32 rxDataByte);
    uint32 uWIFI_GetWordFromRxBuffer(uint32 idx);

    /* TX direction */
    void   uWIFI_PutWordInTxBuffer  (uint32 idx, uint32 txDataByte);
    uint32 uWIFI_GetWordFromTxBuffer(uint32 idx);

#else
    /* RX direction */
    #if(uWIFI_INTERNAL_RX_SW_BUFFER_CONST)
        #define uWIFI_PutWordInRxBuffer(idx, rxDataByte) \
                do{                                                 \
                    uWIFI_rxBufferInternal[(idx)] = ((uint8) (rxDataByte)); \
                }while(0)

        #define uWIFI_GetWordFromRxBuffer(idx) uWIFI_rxBufferInternal[(idx)]

    #endif /* (uWIFI_INTERNAL_RX_SW_BUFFER_CONST) */

    /* TX direction */
    #if(uWIFI_INTERNAL_TX_SW_BUFFER_CONST)
        #define uWIFI_PutWordInTxBuffer(idx, txDataByte) \
                    do{                                             \
                        uWIFI_txBufferInternal[(idx)] = ((uint8) (txDataByte)); \
                    }while(0)

        #define uWIFI_GetWordFromTxBuffer(idx) uWIFI_txBufferInternal[(idx)]

    #endif /* (uWIFI_INTERNAL_TX_SW_BUFFER_CONST) */

#endif /* (uWIFI_TX_SW_BUFFER_ENABLE) */


/***************************************
*         SPI API Constants
***************************************/

/* SPI sub mode enum */
#define uWIFI_SPI_MODE_MOTOROLA      (0x00u)
#define uWIFI_SPI_MODE_TI_COINCIDES  (0x01u)
#define uWIFI_SPI_MODE_TI_PRECEDES   (0x11u)
#define uWIFI_SPI_MODE_NATIONAL      (0x02u)
#define uWIFI_SPI_MODE_MASK          (0x03u)
#define uWIFI_SPI_MODE_TI_PRECEDES_MASK  (0x10u)
#define uWIFI_SPI_MODE_NS_MICROWIRE  (uWIFI_SPI_MODE_NATIONAL)

/* SPI phase and polarity mode enum */
#define uWIFI_SPI_SCLK_CPHA0_CPOL0   (0x00u)
#define uWIFI_SPI_SCLK_CPHA0_CPOL1   (0x02u)
#define uWIFI_SPI_SCLK_CPHA1_CPOL0   (0x01u)
#define uWIFI_SPI_SCLK_CPHA1_CPOL1   (0x03u)

/* SPI bits order enum */
#define uWIFI_BITS_ORDER_LSB_FIRST   (0u)
#define uWIFI_BITS_ORDER_MSB_FIRST   (1u)

/* SPI transfer separation enum */
#define uWIFI_SPI_TRANSFER_SEPARATED     (0u)
#define uWIFI_SPI_TRANSFER_CONTINUOUS    (1u)

/* SPI slave select constants */
#define uWIFI_SPI_SLAVE_SELECT0    (uWIFI_SCB__SS0_POSISTION)
#define uWIFI_SPI_SLAVE_SELECT1    (uWIFI_SCB__SS1_POSISTION)
#define uWIFI_SPI_SLAVE_SELECT2    (uWIFI_SCB__SS2_POSISTION)
#define uWIFI_SPI_SLAVE_SELECT3    (uWIFI_SCB__SS3_POSISTION)

/* SPI slave select polarity settings */
#define uWIFI_SPI_SS_ACTIVE_LOW  (0u)
#define uWIFI_SPI_SS_ACTIVE_HIGH (1u)


/***************************************
*         UART API Constants
***************************************/

/* UART sub-modes enum */
#define uWIFI_UART_MODE_STD          (0u)
#define uWIFI_UART_MODE_SMARTCARD    (1u)
#define uWIFI_UART_MODE_IRDA         (2u)

/* UART direction enum */
#define uWIFI_UART_RX    (1u)
#define uWIFI_UART_TX    (2u)
#define uWIFI_UART_TX_RX (3u)

/* UART parity enum */
#define uWIFI_UART_PARITY_EVEN   (0u)
#define uWIFI_UART_PARITY_ODD    (1u)
#define uWIFI_UART_PARITY_NONE   (2u)

/* UART stop bits enum */
#define uWIFI_UART_STOP_BITS_1   (2u)
#define uWIFI_UART_STOP_BITS_1_5 (3u)
#define uWIFI_UART_STOP_BITS_2   (4u)

/* UART IrDA low power OVS enum */
#define uWIFI_UART_IRDA_LP_OVS16     (16u)
#define uWIFI_UART_IRDA_LP_OVS32     (32u)
#define uWIFI_UART_IRDA_LP_OVS48     (48u)
#define uWIFI_UART_IRDA_LP_OVS96     (96u)
#define uWIFI_UART_IRDA_LP_OVS192    (192u)
#define uWIFI_UART_IRDA_LP_OVS768    (768u)
#define uWIFI_UART_IRDA_LP_OVS1536   (1536u)

/* Uart MP: mark (address) and space (data) bit definitions */
#define uWIFI_UART_MP_MARK       (0x100u)
#define uWIFI_UART_MP_SPACE      (0x000u)

/* UART CTS/RTS polarity settings */
#define uWIFI_UART_CTS_ACTIVE_LOW    (0u)
#define uWIFI_UART_CTS_ACTIVE_HIGH   (1u)
#define uWIFI_UART_RTS_ACTIVE_LOW    (0u)
#define uWIFI_UART_RTS_ACTIVE_HIGH   (1u)

/* Sources of RX errors */
#define uWIFI_INTR_RX_ERR        (uWIFI_INTR_RX_OVERFLOW    | \
                                             uWIFI_INTR_RX_UNDERFLOW   | \
                                             uWIFI_INTR_RX_FRAME_ERROR | \
                                             uWIFI_INTR_RX_PARITY_ERROR)

/* Shifted INTR_RX_ERR defines ONLY for uWIFI_UartGetByte() */
#define uWIFI_UART_RX_OVERFLOW       (uWIFI_INTR_RX_OVERFLOW << 8u)
#define uWIFI_UART_RX_UNDERFLOW      (uWIFI_INTR_RX_UNDERFLOW << 8u)
#define uWIFI_UART_RX_FRAME_ERROR    (uWIFI_INTR_RX_FRAME_ERROR << 8u)
#define uWIFI_UART_RX_PARITY_ERROR   (uWIFI_INTR_RX_PARITY_ERROR << 8u)
#define uWIFI_UART_RX_ERROR_MASK     (uWIFI_UART_RX_OVERFLOW    | \
                                                 uWIFI_UART_RX_UNDERFLOW   | \
                                                 uWIFI_UART_RX_FRAME_ERROR | \
                                                 uWIFI_UART_RX_PARITY_ERROR)


/***************************************
*     Vars with External Linkage
***************************************/

#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    extern const uWIFI_SPI_INIT_STRUCT  uWIFI_configSpi;
    extern const uWIFI_UART_INIT_STRUCT uWIFI_configUart;
#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*    Specific SPI Macro Definitions
***************************************/

#define uWIFI_GET_SPI_INTR_SLAVE_MASK(sourceMask)  ((sourceMask) & uWIFI_INTR_SLAVE_SPI_BUS_ERROR)
#define uWIFI_GET_SPI_INTR_MASTER_MASK(sourceMask) ((sourceMask) & uWIFI_INTR_MASTER_SPI_DONE)
#define uWIFI_GET_SPI_INTR_RX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~uWIFI_INTR_SLAVE_SPI_BUS_ERROR)

#define uWIFI_GET_SPI_INTR_TX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~uWIFI_INTR_MASTER_SPI_DONE)


/***************************************
*    Specific UART Macro Definitions
***************************************/

#define uWIFI_UART_GET_CTRL_OVS_IRDA_LP(oversample) \
        ((uWIFI_UART_IRDA_LP_OVS16   == (oversample)) ? uWIFI_CTRL_OVS_IRDA_LP_OVS16 : \
         ((uWIFI_UART_IRDA_LP_OVS32   == (oversample)) ? uWIFI_CTRL_OVS_IRDA_LP_OVS32 : \
          ((uWIFI_UART_IRDA_LP_OVS48   == (oversample)) ? uWIFI_CTRL_OVS_IRDA_LP_OVS48 : \
           ((uWIFI_UART_IRDA_LP_OVS96   == (oversample)) ? uWIFI_CTRL_OVS_IRDA_LP_OVS96 : \
            ((uWIFI_UART_IRDA_LP_OVS192  == (oversample)) ? uWIFI_CTRL_OVS_IRDA_LP_OVS192 : \
             ((uWIFI_UART_IRDA_LP_OVS768  == (oversample)) ? uWIFI_CTRL_OVS_IRDA_LP_OVS768 : \
              ((uWIFI_UART_IRDA_LP_OVS1536 == (oversample)) ? uWIFI_CTRL_OVS_IRDA_LP_OVS1536 : \
                                                                          uWIFI_CTRL_OVS_IRDA_LP_OVS16)))))))

#define uWIFI_GET_UART_RX_CTRL_ENABLED(direction) ((0u != (uWIFI_UART_RX & (direction))) ? \
                                                                     (uWIFI_RX_CTRL_ENABLED) : (0u))

#define uWIFI_GET_UART_TX_CTRL_ENABLED(direction) ((0u != (uWIFI_UART_TX & (direction))) ? \
                                                                     (uWIFI_TX_CTRL_ENABLED) : (0u))


/***************************************
*        SPI Register Settings
***************************************/

#define uWIFI_CTRL_SPI      (uWIFI_CTRL_MODE_SPI)
#define uWIFI_SPI_RX_CTRL   (uWIFI_RX_CTRL_ENABLED)
#define uWIFI_SPI_TX_CTRL   (uWIFI_TX_CTRL_ENABLED)


/***************************************
*       SPI Init Register Settings
***************************************/

#define uWIFI_SPI_SS_POLARITY \
             (((uint32) uWIFI_SPI_SS0_POLARITY << uWIFI_SPI_SLAVE_SELECT0) | \
              ((uint32) uWIFI_SPI_SS1_POLARITY << uWIFI_SPI_SLAVE_SELECT1) | \
              ((uint32) uWIFI_SPI_SS2_POLARITY << uWIFI_SPI_SLAVE_SELECT2) | \
              ((uint32) uWIFI_SPI_SS3_POLARITY << uWIFI_SPI_SLAVE_SELECT3))

#if(uWIFI_SCB_MODE_SPI_CONST_CFG)

    /* SPI Configuration */
    #define uWIFI_SPI_DEFAULT_CTRL \
                    (uWIFI_GET_CTRL_OVS(uWIFI_SPI_OVS_FACTOR) | \
                     uWIFI_GET_CTRL_BYTE_MODE (uWIFI_SPI_BYTE_MODE_ENABLE) | \
                     uWIFI_GET_CTRL_EC_AM_MODE(uWIFI_SPI_WAKE_ENABLE)      | \
                     uWIFI_CTRL_SPI)

    #define uWIFI_SPI_DEFAULT_SPI_CTRL \
                    (uWIFI_GET_SPI_CTRL_CONTINUOUS    (uWIFI_SPI_TRANSFER_SEPARATION)       | \
                     uWIFI_GET_SPI_CTRL_SELECT_PRECEDE(uWIFI_SPI_SUB_MODE &                   \
                                                                  uWIFI_SPI_MODE_TI_PRECEDES_MASK)     | \
                     uWIFI_GET_SPI_CTRL_SCLK_MODE     (uWIFI_SPI_CLOCK_MODE)                | \
                     uWIFI_GET_SPI_CTRL_LATE_MISO_SAMPLE(uWIFI_SPI_LATE_MISO_SAMPLE_ENABLE) | \
                     uWIFI_GET_SPI_CTRL_SCLK_CONTINUOUS(uWIFI_SPI_FREE_RUN_SCLK_ENABLE)     | \
                     uWIFI_GET_SPI_CTRL_SSEL_POLARITY (uWIFI_SPI_SS_POLARITY)               | \
                     uWIFI_GET_SPI_CTRL_SUB_MODE      (uWIFI_SPI_SUB_MODE)                  | \
                     uWIFI_GET_SPI_CTRL_MASTER_MODE   (uWIFI_SPI_MODE))

    /* RX direction */
    #define uWIFI_SPI_DEFAULT_RX_CTRL \
                    (uWIFI_GET_RX_CTRL_DATA_WIDTH(uWIFI_SPI_RX_DATA_BITS_NUM)     | \
                     uWIFI_GET_RX_CTRL_BIT_ORDER (uWIFI_SPI_BITS_ORDER)           | \
                     uWIFI_GET_RX_CTRL_MEDIAN    (uWIFI_SPI_MEDIAN_FILTER_ENABLE) | \
                     uWIFI_SPI_RX_CTRL)

    #define uWIFI_SPI_DEFAULT_RX_FIFO_CTRL \
                    uWIFI_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(uWIFI_SPI_RX_TRIGGER_LEVEL)

    /* TX direction */
    #define uWIFI_SPI_DEFAULT_TX_CTRL \
                    (uWIFI_GET_TX_CTRL_DATA_WIDTH(uWIFI_SPI_TX_DATA_BITS_NUM) | \
                     uWIFI_GET_TX_CTRL_BIT_ORDER (uWIFI_SPI_BITS_ORDER)       | \
                     uWIFI_SPI_TX_CTRL)

    #define uWIFI_SPI_DEFAULT_TX_FIFO_CTRL \
                    uWIFI_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(uWIFI_SPI_TX_TRIGGER_LEVEL)

    /* Interrupt sources */
    #define uWIFI_SPI_DEFAULT_INTR_SPI_EC_MASK   (uWIFI_NO_INTR_SOURCES)

    #define uWIFI_SPI_DEFAULT_INTR_I2C_EC_MASK   (uWIFI_NO_INTR_SOURCES)
    #define uWIFI_SPI_DEFAULT_INTR_SLAVE_MASK \
                    (uWIFI_SPI_INTR_RX_MASK & uWIFI_INTR_SLAVE_SPI_BUS_ERROR)

    #define uWIFI_SPI_DEFAULT_INTR_MASTER_MASK \
                    (uWIFI_SPI_INTR_TX_MASK & uWIFI_INTR_MASTER_SPI_DONE)

    #define uWIFI_SPI_DEFAULT_INTR_RX_MASK \
                    (uWIFI_SPI_INTR_RX_MASK & (uint32) ~uWIFI_INTR_SLAVE_SPI_BUS_ERROR)

    #define uWIFI_SPI_DEFAULT_INTR_TX_MASK \
                    (uWIFI_SPI_INTR_TX_MASK & (uint32) ~uWIFI_INTR_MASTER_SPI_DONE)

#endif /* (uWIFI_SCB_MODE_SPI_CONST_CFG) */


/***************************************
*        UART Register Settings
***************************************/

#define uWIFI_CTRL_UART      (uWIFI_CTRL_MODE_UART)
#define uWIFI_UART_RX_CTRL   (uWIFI_RX_CTRL_LSB_FIRST) /* LSB for UART goes first */
#define uWIFI_UART_TX_CTRL   (uWIFI_TX_CTRL_LSB_FIRST) /* LSB for UART goes first */


/***************************************
*      UART Init Register Settings
***************************************/

#if(uWIFI_SCB_MODE_UART_CONST_CFG)

    /* UART configuration */
    #if(uWIFI_UART_MODE_IRDA == uWIFI_UART_SUB_MODE)

        #define uWIFI_DEFAULT_CTRL_OVS   ((0u != uWIFI_UART_IRDA_LOW_POWER) ?              \
                                (uWIFI_UART_GET_CTRL_OVS_IRDA_LP(uWIFI_UART_OVS_FACTOR)) : \
                                (uWIFI_CTRL_OVS_IRDA_OVS16))

    #else

        #define uWIFI_DEFAULT_CTRL_OVS   uWIFI_GET_CTRL_OVS(uWIFI_UART_OVS_FACTOR)

    #endif /* (uWIFI_UART_MODE_IRDA == uWIFI_UART_SUB_MODE) */

    #define uWIFI_UART_DEFAULT_CTRL \
                                (uWIFI_GET_CTRL_BYTE_MODE  (uWIFI_UART_BYTE_MODE_ENABLE)  | \
                                 uWIFI_GET_CTRL_ADDR_ACCEPT(uWIFI_UART_MP_ACCEPT_ADDRESS) | \
                                 uWIFI_DEFAULT_CTRL_OVS                                              | \
                                 uWIFI_CTRL_UART)

    #define uWIFI_UART_DEFAULT_UART_CTRL \
                                    (uWIFI_GET_UART_CTRL_MODE(uWIFI_UART_SUB_MODE))

    /* RX direction */
    #define uWIFI_UART_DEFAULT_RX_CTRL_PARITY \
                                ((uWIFI_UART_PARITY_NONE != uWIFI_UART_PARITY_TYPE) ?      \
                                  (uWIFI_GET_UART_RX_CTRL_PARITY(uWIFI_UART_PARITY_TYPE) | \
                                   uWIFI_UART_RX_CTRL_PARITY_ENABLED) : (0u))

    #define uWIFI_UART_DEFAULT_UART_RX_CTRL \
                    (uWIFI_GET_UART_RX_CTRL_MODE(uWIFI_UART_STOP_BITS_NUM)                    | \
                     uWIFI_GET_UART_RX_CTRL_POLARITY(uWIFI_UART_IRDA_POLARITY)                | \
                     uWIFI_GET_UART_RX_CTRL_MP_MODE(uWIFI_UART_MP_MODE_ENABLE)                | \
                     uWIFI_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(uWIFI_UART_DROP_ON_PARITY_ERR) | \
                     uWIFI_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(uWIFI_UART_DROP_ON_FRAME_ERR)   | \
                     uWIFI_UART_DEFAULT_RX_CTRL_PARITY)

    #define uWIFI_UART_DEFAULT_RX_CTRL \
                                (uWIFI_GET_RX_CTRL_DATA_WIDTH(uWIFI_UART_DATA_BITS_NUM)        | \
                                 uWIFI_GET_RX_CTRL_MEDIAN    (uWIFI_UART_MEDIAN_FILTER_ENABLE) | \
                                 uWIFI_GET_UART_RX_CTRL_ENABLED(uWIFI_UART_DIRECTION))

    #define uWIFI_UART_DEFAULT_RX_FIFO_CTRL \
                                uWIFI_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(uWIFI_UART_RX_TRIGGER_LEVEL)

    #define uWIFI_UART_DEFAULT_RX_MATCH_REG  ((0u != uWIFI_UART_MP_MODE_ENABLE) ?          \
                                (uWIFI_GET_RX_MATCH_ADDR(uWIFI_UART_MP_RX_ADDRESS) | \
                                 uWIFI_GET_RX_MATCH_MASK(uWIFI_UART_MP_RX_ADDRESS_MASK)) : (0u))

    /* TX direction */
    #define uWIFI_UART_DEFAULT_TX_CTRL_PARITY (uWIFI_UART_DEFAULT_RX_CTRL_PARITY)

    #define uWIFI_UART_DEFAULT_UART_TX_CTRL \
                                (uWIFI_GET_UART_TX_CTRL_MODE(uWIFI_UART_STOP_BITS_NUM)       | \
                                 uWIFI_GET_UART_TX_CTRL_RETRY_NACK(uWIFI_UART_RETRY_ON_NACK) | \
                                 uWIFI_UART_DEFAULT_TX_CTRL_PARITY)

    #define uWIFI_UART_DEFAULT_TX_CTRL \
                                (uWIFI_GET_TX_CTRL_DATA_WIDTH(uWIFI_UART_DATA_BITS_NUM) | \
                                 uWIFI_GET_UART_TX_CTRL_ENABLED(uWIFI_UART_DIRECTION))

    #define uWIFI_UART_DEFAULT_TX_FIFO_CTRL \
                                uWIFI_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(uWIFI_UART_TX_TRIGGER_LEVEL)

    #define uWIFI_UART_DEFAULT_FLOW_CTRL \
                        (uWIFI_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(uWIFI_UART_RTS_FIFO_LEVEL) | \
                         uWIFI_GET_UART_FLOW_CTRL_RTS_POLARITY (uWIFI_UART_RTS_POLARITY)   | \
                         uWIFI_GET_UART_FLOW_CTRL_CTS_POLARITY (uWIFI_UART_CTS_POLARITY)   | \
                         uWIFI_GET_UART_FLOW_CTRL_CTS_ENABLE   (uWIFI_UART_CTS_ENABLE))

    /* Interrupt sources */
    #define uWIFI_UART_DEFAULT_INTR_I2C_EC_MASK  (uWIFI_NO_INTR_SOURCES)
    #define uWIFI_UART_DEFAULT_INTR_SPI_EC_MASK  (uWIFI_NO_INTR_SOURCES)
    #define uWIFI_UART_DEFAULT_INTR_SLAVE_MASK   (uWIFI_NO_INTR_SOURCES)
    #define uWIFI_UART_DEFAULT_INTR_MASTER_MASK  (uWIFI_NO_INTR_SOURCES)
    #define uWIFI_UART_DEFAULT_INTR_RX_MASK      (uWIFI_UART_INTR_RX_MASK)
    #define uWIFI_UART_DEFAULT_INTR_TX_MASK      (uWIFI_UART_INTR_TX_MASK)

#endif /* (uWIFI_SCB_MODE_UART_CONST_CFG) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

#define uWIFI_SPIM_ACTIVE_SS0    (uWIFI_SPI_SLAVE_SELECT0)
#define uWIFI_SPIM_ACTIVE_SS1    (uWIFI_SPI_SLAVE_SELECT1)
#define uWIFI_SPIM_ACTIVE_SS2    (uWIFI_SPI_SLAVE_SELECT2)
#define uWIFI_SPIM_ACTIVE_SS3    (uWIFI_SPI_SLAVE_SELECT3)

#endif /* CY_SCB_SPI_UART_uWIFI_H */


/* [] END OF FILE */
