/*******************************************************************************
* File Name: `$INSTANCE_NAME`_SPI_UART.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides constants and parameter values for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_`$INSTANCE_NAME`_H)
#define CY_SCB_SPI_UART_`$INSTANCE_NAME`_H

#include "`$INSTANCE_NAME`.h"


/***************************************
*   SPI Initial Parameter Constants
****************************************/

#define `$INSTANCE_NAME`_SPI_MODE                   (`$SpiMode`u)
#define `$INSTANCE_NAME`_SPI_SUB_MODE               (`$SpiSubMode`u)
#define `$INSTANCE_NAME`_SPI_CLOCK_MODE             (`$SpiSclkMode`u)
#define `$INSTANCE_NAME`_SPI_OVS_FACTOR             (`$SpiOvsFactor`u)
#define `$INSTANCE_NAME`_SPI_MEDIAN_FILTER_ENABLE   (`$SpiMedianFilterEnable`u)
#define `$INSTANCE_NAME`_SPI_LATE_MISO_SAMPLE_ENABLE (`$SpiLateMisoSampleEnable`u)
#define `$INSTANCE_NAME`_SPI_RX_DATA_BITS_NUM       (`$SpiNumberOfRxDataBits`u)
#define `$INSTANCE_NAME`_SPI_TX_DATA_BITS_NUM       (`$SpiNumberOfTxDataBits`u)
#define `$INSTANCE_NAME`_SPI_WAKE_ENABLE            (`$SpiWakeEnable`u)
#define `$INSTANCE_NAME`_SPI_BITS_ORDER             (`$SpiBitsOrder`u)
#define `$INSTANCE_NAME`_SPI_TRANSFER_SEPARATION    (`$SpiTransferSeparation`u)
#define `$INSTANCE_NAME`_SPI_NUMBER_OF_SS_LINES     (`$SpiNumberOfSelectLines`u)
#define `$INSTANCE_NAME`_SPI_RX_BUFFER_SIZE         (`$SpiRxBufferSize`u)
#define `$INSTANCE_NAME`_SPI_TX_BUFFER_SIZE         (`$SpiTxBufferSize`u)

#define `$INSTANCE_NAME`_SPI_INTERRUPT_MODE         (`$SpiInterruptMode`u)

#define `$INSTANCE_NAME`_SPI_INTR_RX_MASK           (`$SpiRxIntrMask`u)
#define `$INSTANCE_NAME`_SPI_INTR_TX_MASK           (`$SpiTxIntrMask`u)

#define `$INSTANCE_NAME`_SPI_RX_TRIGGER_LEVEL       (`$SpiRxTriggerLevel`u)
#define `$INSTANCE_NAME`_SPI_TX_TRIGGER_LEVEL       (`$SpiTxTriggerLevel`u)


/***************************************
*   UART Initial Parameter Constants
****************************************/

#define `$INSTANCE_NAME`_UART_SUB_MODE              (`$UartSubMode`u)
#define `$INSTANCE_NAME`_UART_DIRECTION             (`$UartDirection`u)
#define `$INSTANCE_NAME`_UART_DATA_BITS_NUM         (`$UartNumberOfDataBits`u)
#define `$INSTANCE_NAME`_UART_PARITY_TYPE           (`$UartParityType`u)
#define `$INSTANCE_NAME`_UART_STOP_BITS_NUM         (`$UartNumberOfStopBits`u)
#define `$INSTANCE_NAME`_UART_OVS_FACTOR            (`$UartOvsFactor`u)
#define `$INSTANCE_NAME`_UART_IRDA_LOW_POWER        (`$UartIrdaLowPower`u)
#define `$INSTANCE_NAME`_UART_MEDIAN_FILTER_ENABLE  (`$UartMedianFilterEnable`u)
#define `$INSTANCE_NAME`_UART_RETRY_ON_NACK         (`$UartSmCardRetryOnNack`u)
#define `$INSTANCE_NAME`_UART_IRDA_POLARITY         (`$UartIrdaPolarity`u)
#define `$INSTANCE_NAME`_UART_DROP_ON_FRAME_ERR     (`$UartDropOnFrameErr`u)
#define `$INSTANCE_NAME`_UART_DROP_ON_PARITY_ERR    (`$UartDropOnParityErr`u)
#define `$INSTANCE_NAME`_UART_WAKE_ENABLE           (`$UartWakeEnable`u)
#define `$INSTANCE_NAME`_UART_RX_BUFFER_SIZE        (`$UartRxBufferSize`u)
#define `$INSTANCE_NAME`_UART_TX_BUFFER_SIZE        (`$UartTxBufferSize`u)
#define `$INSTANCE_NAME`_UART_MP_MODE_ENABLE        (`$UartMpEnable`u)
#define `$INSTANCE_NAME`_UART_MP_ACCEPT_ADDRESS     (`$UartMpRxAcceptAddress`u)
#define `$INSTANCE_NAME`_UART_MP_RX_ADDRESS         (`$UartMpRxAddress`u)
#define `$INSTANCE_NAME`_UART_MP_RX_ADDRESS_MASK    (`$UartMpRxAddressMask`u)

#define `$INSTANCE_NAME`_UART_INTERRUPT_MODE        (`$UartInterruptMode`u)

#define `$INSTANCE_NAME`_UART_INTR_RX_MASK          (`$UartRxIntrMask`u)
#define `$INSTANCE_NAME`_UART_INTR_TX_MASK          (`$UartTxIntrMask`u)

#define `$INSTANCE_NAME`_UART_RX_TRIGGER_LEVEL      (`$UartRxTriggerLevel`u)
#define `$INSTANCE_NAME`_UART_TX_TRIGGER_LEVEL      (`$UartTxTriggerLevel`u)

/* Sources of RX errors */
#define `$INSTANCE_NAME`_INTR_RX_ERR        (`$INSTANCE_NAME`_INTR_RX_OVERFLOW    | \
                                             `$INSTANCE_NAME`_INTR_RX_UNDERFLOW   | \
                                             `$INSTANCE_NAME`_INTR_RX_FRAME_ERROR | \
                                             `$INSTANCE_NAME`_INTR_RX_PARITY_ERROR)

/* UART direction enum */
#define `$INSTANCE_NAME`_UART_RX    (1u)
#define `$INSTANCE_NAME`_UART_TX    (2u)
#define `$INSTANCE_NAME`_UART_TX_RX (3u)


/***************************************
*   Conditional Compilation Parameters
****************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)

    /* Direction */
    #define `$INSTANCE_NAME`_RX_DIRECTION           (1u)
    #define `$INSTANCE_NAME`_TX_DIRECTION           (1u)
    #define `$INSTANCE_NAME`_UART_RX_DIRECTION      (1u)
    #define `$INSTANCE_NAME`_UART_TX_DIRECTION      (1u)

    /* Only external RX and TX buffer for uncofigured mode */
    #define `$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER   (0u)
    #define `$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER   (0u)

    /* Get RX and TX buffer size */
    #define `$INSTANCE_NAME`_RX_BUFFER_SIZE (`$INSTANCE_NAME`_rxBufferSize)
    #define `$INSTANCE_NAME`_TX_BUFFER_SIZE (`$INSTANCE_NAME`_txBufferSize)

    /* Return true if buffer is provided */
    #define `$INSTANCE_NAME`_CHECK_RX_SW_BUFFER (NULL != `$INSTANCE_NAME`_rxBuffer)
    #define `$INSTANCE_NAME`_CHECK_TX_SW_BUFFER (NULL != `$INSTANCE_NAME`_txBuffer)

    /* Alwasy provde global variables to support RX and TX buffers */
    #define `$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST    (1u)
    #define `$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST    (1u)

    /* Get wakeup enable option */
    #define `$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST  (1u)
    #define `$INSTANCE_NAME`_CHECK_SPI_WAKE_ENABLE  (0u != `$INSTANCE_NAME`_scbEnableWake)
    #define `$INSTANCE_NAME`_UART_WAKE_ENABLE_CONST (1u)

#else

    /* SPI internal RX and TX buffers */
    #define `$INSTANCE_NAME`_INTERNAL_SPI_RX_SW_BUFFER  (`$INSTANCE_NAME`_SPI_RX_BUFFER_SIZE > \
                                                                                            `$INSTANCE_NAME`_FIFO_SIZE)
    #define `$INSTANCE_NAME`_INTERNAL_SPI_TX_SW_BUFFER  (`$INSTANCE_NAME`_SPI_TX_BUFFER_SIZE > \
                                                                                            `$INSTANCE_NAME`_FIFO_SIZE)

    /* UART internal RX and TX buffers */
    #define `$INSTANCE_NAME`_INTERNAL_UART_RX_SW_BUFFER  (`$INSTANCE_NAME`_UART_RX_BUFFER_SIZE > \
                                                                                            `$INSTANCE_NAME`_FIFO_SIZE)
    #define `$INSTANCE_NAME`_INTERNAL_UART_TX_SW_BUFFER  (`$INSTANCE_NAME`_UART_TX_BUFFER_SIZE > \
                                                                                            `$INSTANCE_NAME`_FIFO_SIZE)

    /* SPI Direction */
    #define `$INSTANCE_NAME`_SPI_RX_DIRECTION (1u)
    #define `$INSTANCE_NAME`_SPI_TX_DIRECTION (1u)

    /* UART Direction */
    #define `$INSTANCE_NAME`_UART_RX_DIRECTION (0u != (`$INSTANCE_NAME`_UART_DIRECTION & `$INSTANCE_NAME`_UART_RX))
    #define `$INSTANCE_NAME`_UART_TX_DIRECTION (0u != (`$INSTANCE_NAME`_UART_DIRECTION & `$INSTANCE_NAME`_UART_TX))

    /* Direction */
    #define `$INSTANCE_NAME`_RX_DIRECTION ((`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG) ? \
                                            (`$INSTANCE_NAME`_SPI_RX_DIRECTION) : (`$INSTANCE_NAME`_UART_RX_DIRECTION))

    #define `$INSTANCE_NAME`_TX_DIRECTION ((`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG) ? \
                                            (`$INSTANCE_NAME`_SPI_TX_DIRECTION) : (`$INSTANCE_NAME`_UART_TX_DIRECTION))

    /* Internal RX and TX buffer: for SPI or UART */
    #if(`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG)

        /* Internal SPI RX and TX buffer */
        #define `$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER  (`$INSTANCE_NAME`_INTERNAL_SPI_RX_SW_BUFFER)
        #define `$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER  (`$INSTANCE_NAME`_INTERNAL_SPI_TX_SW_BUFFER)

        /* Internal SPI RX and TX buffer size */
        #define `$INSTANCE_NAME`_RX_BUFFER_SIZE         (`$INSTANCE_NAME`_SPI_RX_BUFFER_SIZE + 1u)
        #define `$INSTANCE_NAME`_TX_BUFFER_SIZE         (`$INSTANCE_NAME`_SPI_TX_BUFFER_SIZE)
        
        /* Get wakeup enable option */
        #define `$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST  (0u != `$INSTANCE_NAME`_SPI_WAKE_ENABLE)
        #define `$INSTANCE_NAME`_UART_WAKE_ENABLE_CONST (0u)

    #else

        /* Internal UART RX and TX buffer */
        #define `$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER  (`$INSTANCE_NAME`_INTERNAL_UART_RX_SW_BUFFER)
        #define `$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER  (`$INSTANCE_NAME`_INTERNAL_UART_TX_SW_BUFFER)

        /* Internal UART RX and TX buffer size */
        #define `$INSTANCE_NAME`_RX_BUFFER_SIZE         (`$INSTANCE_NAME`_UART_RX_BUFFER_SIZE + 1u)
        #define `$INSTANCE_NAME`_TX_BUFFER_SIZE         (`$INSTANCE_NAME`_UART_TX_BUFFER_SIZE)
        
        /* Get wakeup enable option */
        #define `$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST  (0u)
        #define `$INSTANCE_NAME`_UART_WAKE_ENABLE_CONST (0u != `$INSTANCE_NAME`_UART_WAKE_ENABLE)
    #endif /* (`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG) */

    /* Internal RX and TX buffer: for SPI or UART. Used in conditional compilation check */
    #define `$INSTANCE_NAME`_CHECK_RX_SW_BUFFER (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER)
    #define `$INSTANCE_NAME`_CHECK_TX_SW_BUFFER (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER)

    /* Provide global variables to support RX and TX buffers */
    #define `$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST    (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER)
    #define `$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST    (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER)
    
    /* Wakeup for SPI */
    #define `$INSTANCE_NAME`_CHECK_SPI_WAKE_ENABLE  (`$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST)

#endif /* End (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Bootloader communication interface enable: NOT supported yet */
#define `$INSTANCE_NAME`_SPI_BTLDR_COMM_ENABLED   ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_`$INSTANCE_NAME`) || \
                                                    (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

#define `$INSTANCE_NAME`_UART_BTLDR_COMM_ENABLED   ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_`$INSTANCE_NAME`) || \
                                                    (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))


/***************************************
*       Type Definitions
***************************************/

/* `$INSTANCE_NAME`_SPI_INIT_STRUCT */
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
} `$INSTANCE_NAME`_SPI_INIT_STRUCT;

/* `$INSTANCE_NAME`_UART_INIT_STRUCT */
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
} `$INSTANCE_NAME`_UART_INIT_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

/* SPI specific functions */
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    void `$INSTANCE_NAME`_SpiInit(const `$INSTANCE_NAME`_SPI_INIT_STRUCT *config);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(`$INSTANCE_NAME`_SCB_MODE_SPI_INC)
    void `$INSTANCE_NAME`_SpiSetActiveSlaveSelect(uint32 activeSelect);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_SPI_INC) */

/* UART specific functions */
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    void `$INSTANCE_NAME`_UartInit(const `$INSTANCE_NAME`_UART_INIT_STRUCT *config);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(`$INSTANCE_NAME`_SCB_MODE_UART_INC)
    void `$INSTANCE_NAME`_UartSetRxAddress(uint32 address);
    void `$INSTANCE_NAME`_UartSetRxAddressMask(uint32 addressMask);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UART_INC) */

/* UART RX direction APIs */
#if(`$INSTANCE_NAME`_UART_RX_DIRECTION)
    uint32 `$INSTANCE_NAME`_UartGetChar(void);
    uint32 `$INSTANCE_NAME`_UartGetByte(void);
#endif /* (`$INSTANCE_NAME`_UART_RX_DIRECTION) */

/* UART TX direction APIs */
#if(`$INSTANCE_NAME`_UART_TX_DIRECTION)
    #define `$INSTANCE_NAME`_UartPutChar(ch)    `$INSTANCE_NAME`_SpiUartWriteTxData((uint32)(ch))
    void `$INSTANCE_NAME`_UartPutString(const char8 string[]);
    void `$INSTANCE_NAME`_UartPutCRLF(uint32 txDataByte);
#endif /* (`$INSTANCE_NAME`_UART_TX_DIRECTION) */

/* Common APIs Rx direction */
#if(`$INSTANCE_NAME`_RX_DIRECTION)
    uint32 `$INSTANCE_NAME`_SpiUartReadRxData(void);
    uint32 `$INSTANCE_NAME`_SpiUartGetRxBufferSize(void);
    void   `$INSTANCE_NAME`_SpiUartClearRxBuffer(void);
#endif /* (`$INSTANCE_NAME`_RX_DIRECTION) */

/* Common APIs Tx direction */
#if(`$INSTANCE_NAME`_TX_DIRECTION)
    void   `$INSTANCE_NAME`_SpiUartWriteTxData(uint32 txDataByte);
    void   `$INSTANCE_NAME`_SpiUartPutArray(const `$BitWidthReplacementStringTx` wrBuf[], uint32 count);
    void   `$INSTANCE_NAME`_SpiUartClearTxBuffer(void);
    uint32 `$INSTANCE_NAME`_SpiUartGetTxBufferSize(void);
#endif /* (`$INSTANCE_NAME`_TX_DIRECTION) */

CY_ISR_PROTO(`$INSTANCE_NAME`_SPI_UART_ISR);

#if(`$INSTANCE_NAME`_UART_RX_WAKEUP_IRQ)
    CY_ISR_PROTO(`$INSTANCE_NAME`_UART_WAKEUP_ISR);
#endif /* (`$INSTANCE_NAME`_UART_RX_WAKEUP_IRQ) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void `$INSTANCE_NAME`_SpiCyBtldrCommStart(void);
    void `$INSTANCE_NAME`_SpiCyBtldrCommStop (void);
    void `$INSTANCE_NAME`_SpiCyBtldrCommReset(void);
    cystatus `$INSTANCE_NAME`_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus `$INSTANCE_NAME`_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void `$INSTANCE_NAME`_UartCyBtldrCommStart(void);
    void `$INSTANCE_NAME`_UartCyBtldrCommStop (void);
    void `$INSTANCE_NAME`_UartCyBtldrCommReset(void);
    cystatus `$INSTANCE_NAME`_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus `$INSTANCE_NAME`_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_UART_BTLDR_COMM_ENABLED) */


/***************************************
*     Buffer Access Macro Definitions
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    /* RX direction */
    void   `$INSTANCE_NAME`_PutWordInRxBuffer  (uint32 idx, uint32 rxDataByte);
    uint32 `$INSTANCE_NAME`_GetWordFromRxBuffer(uint32 idx);

    /* TX direction */
    void   `$INSTANCE_NAME`_PutWordInTxBuffer  (uint32 idx, uint32 txDataByte);
    uint32 `$INSTANCE_NAME`_GetWordFromTxBuffer(uint32 idx);

#else

    /* RX direction */
    #if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST)
        #define `$INSTANCE_NAME`_PutWordInRxBuffer(idx, rxDataByte) \
                do{                                                 \
                    `$INSTANCE_NAME`_rxBufferInternal[(idx)] = ((`$BitWidthReplacementStringRx`) (rxDataByte)); \
                }while(0)

        #define `$INSTANCE_NAME`_GetWordFromRxBuffer(idx) `$INSTANCE_NAME`_rxBufferInternal[(idx)]

    #endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST) */

    /* TX direction */
    #if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST)
        #define `$INSTANCE_NAME`_PutWordInTxBuffer(idx, txDataByte) \
                    do{                                             \
                        `$INSTANCE_NAME`_txBufferInternal[(idx)] = ((`$BitWidthReplacementStringTx`) (txDataByte)); \
                    }while(0)

        #define `$INSTANCE_NAME`_GetWordFromTxBuffer(idx) `$INSTANCE_NAME`_txBufferInternal[(idx)]

    #endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST) */

#endif /* (`$INSTANCE_NAME`_TX_SW_BUFFER_ENABLE) */


/***************************************
*         SPI API Constants
***************************************/

/* SPI mode enum */
#define `$INSTANCE_NAME`_SPI_SLAVE  (0u)
#define `$INSTANCE_NAME`_SPI_MASTER (1u)

/* SPI sub mode enum */
#define `$INSTANCE_NAME`_SPI_MODE_MOTOROLA      (0x00u)
#define `$INSTANCE_NAME`_SPI_MODE_TI_COINCIDES  (0x01u)
#define `$INSTANCE_NAME`_SPI_MODE_TI_PRECEDES   (0x11u)
#define `$INSTANCE_NAME`_SPI_MODE_NATIONAL      (0x02u)
#define `$INSTANCE_NAME`_SPI_MODE_MASK          (0x03u)
#define `$INSTANCE_NAME`_SPI_MODE_TI_PRECEDES_MASK  (0x10u)

/* SPI phase and polarity mode enum */
#define `$INSTANCE_NAME`_SPI_SCLK_CPHA0_CPOL0   (0x00u)
#define `$INSTANCE_NAME`_SPI_SCLK_CPHA0_CPOL1   (0x02u)
#define `$INSTANCE_NAME`_SPI_SCLK_CPHA1_CPOL0   (0x01u)
#define `$INSTANCE_NAME`_SPI_SCLK_CPHA1_CPOL1   (0x03u)

/* SPI bits order enum */
#define `$INSTANCE_NAME`_BITS_ORDER_LSB_FIRST   (0u)
#define `$INSTANCE_NAME`_BITS_ORDER_MSB_FIRST   (1u)

/* SPI transfer separation enum */
#define `$INSTANCE_NAME`_SPI_TRANSFER_SEPARATED     (0u)
#define `$INSTANCE_NAME`_SPI_TRANSFER_CONTINUOUS    (1u)

/* "activeSS" constants for SpiSetActiveSlaveSelect() function */
#define `$INSTANCE_NAME`_SPIM_ACTIVE_SS0    (0x00u)
#define `$INSTANCE_NAME`_SPIM_ACTIVE_SS1    (0x01u)
#define `$INSTANCE_NAME`_SPIM_ACTIVE_SS2    (0x02u)
#define `$INSTANCE_NAME`_SPIM_ACTIVE_SS3    (0x03u)


/***************************************
*         UART API Constants
***************************************/

/* UART sub-modes enum */
#define `$INSTANCE_NAME`_UART_MODE_STD          (0u)
#define `$INSTANCE_NAME`_UART_MODE_SMARTCARD    (1u)
#define `$INSTANCE_NAME`_UART_MODE_IRDA         (2u)

/* UART direction enum */
#define `$INSTANCE_NAME`_UART_RX    (1u)
#define `$INSTANCE_NAME`_UART_TX    (2u)
#define `$INSTANCE_NAME`_UART_TX_RX (3u)

/* UART parity enum */
#define `$INSTANCE_NAME`_UART_PARITY_EVEN   (0u)
#define `$INSTANCE_NAME`_UART_PARITY_ODD    (1u)
#define `$INSTANCE_NAME`_UART_PARITY_NONE   (2u)

/* UART stop bits enum */
#define `$INSTANCE_NAME`_UART_STOP_BITS_1   (1u)
#define `$INSTANCE_NAME`_UART_STOP_BITS_1_5 (2u)
#define `$INSTANCE_NAME`_UART_STOP_BITS_2   (3u)

/* UART IrDA low power OVS enum */
#define `$INSTANCE_NAME`_UART_IRDA_LP_OVS16     (16u)
#define `$INSTANCE_NAME`_UART_IRDA_LP_OVS32     (32u)
#define `$INSTANCE_NAME`_UART_IRDA_LP_OVS48     (48u)
#define `$INSTANCE_NAME`_UART_IRDA_LP_OVS96     (96u)
#define `$INSTANCE_NAME`_UART_IRDA_LP_OVS192    (192u)
#define `$INSTANCE_NAME`_UART_IRDA_LP_OVS768    (768u)
#define `$INSTANCE_NAME`_UART_IRDA_LP_OVS1536   (1536u)

/* Uart MP: mark (address) and space (data) bit definitions */
#define `$INSTANCE_NAME`_UART_MP_MARK       (0x100u)
#define `$INSTANCE_NAME`_UART_MP_SPACE      (0x000u)


/***************************************
*     Vars with External Linkage
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    extern const `$INSTANCE_NAME`_SPI_INIT_STRUCT  `$INSTANCE_NAME`_configSpi;
    extern const `$INSTANCE_NAME`_UART_INIT_STRUCT `$INSTANCE_NAME`_configUart;
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*    Specific SPI Macro Definitions
***************************************/

#define `$INSTANCE_NAME`_GET_SPI_INTR_SLAVE_MASK(sourceMask)  ((sourceMask) & `$INSTANCE_NAME`_INTR_SLAVE_SPI_BUS_ERROR)
#define `$INSTANCE_NAME`_GET_SPI_INTR_MASTER_MASK(sourceMask) ((sourceMask) & `$INSTANCE_NAME`_INTR_MASTER_SPI_DONE)
#define `$INSTANCE_NAME`_GET_SPI_INTR_RX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~`$INSTANCE_NAME`_INTR_SLAVE_SPI_BUS_ERROR)

#define `$INSTANCE_NAME`_GET_SPI_INTR_TX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~`$INSTANCE_NAME`_INTR_MASTER_SPI_DONE)


/***************************************
*    Specific UART Macro Definitions
***************************************/

#define `$INSTANCE_NAME`_UART_GET_CTRL_OVS_IRDA_LP(oversample) \
        ((`$INSTANCE_NAME`_UART_IRDA_LP_OVS16   == (oversample)) ? `$INSTANCE_NAME`_CTRL_OVS_IRDA_LP_OVS16 : \
         ((`$INSTANCE_NAME`_UART_IRDA_LP_OVS32   == (oversample)) ? `$INSTANCE_NAME`_CTRL_OVS_IRDA_LP_OVS32 : \
          ((`$INSTANCE_NAME`_UART_IRDA_LP_OVS48   == (oversample)) ? `$INSTANCE_NAME`_CTRL_OVS_IRDA_LP_OVS48 : \
           ((`$INSTANCE_NAME`_UART_IRDA_LP_OVS96   == (oversample)) ? `$INSTANCE_NAME`_CTRL_OVS_IRDA_LP_OVS96 : \
            ((`$INSTANCE_NAME`_UART_IRDA_LP_OVS192  == (oversample)) ? `$INSTANCE_NAME`_CTRL_OVS_IRDA_LP_OVS192 : \
             ((`$INSTANCE_NAME`_UART_IRDA_LP_OVS768  == (oversample)) ? `$INSTANCE_NAME`_CTRL_OVS_IRDA_LP_OVS768 : \
              ((`$INSTANCE_NAME`_UART_IRDA_LP_OVS1536 == (oversample)) ? `$INSTANCE_NAME`_CTRL_OVS_IRDA_LP_OVS1536 : \
                                                                          `$INSTANCE_NAME`_CTRL_OVS_IRDA_LP_OVS16)))))))

#define `$INSTANCE_NAME`_GET_UART_RX_CTRL_ENABLED(direction) ((0u != (`$INSTANCE_NAME`_UART_RX & (direction))) ? \
                                                                    (`$INSTANCE_NAME`_RX_CTRL_ENABLED) : (0u))

#define `$INSTANCE_NAME`_GET_UART_TX_CTRL_ENABLED(direction) ((0u != (`$INSTANCE_NAME`_UART_TX & (direction))) ? \
                                                                    (`$INSTANCE_NAME`_TX_CTRL_ENABLED) : (0u))


/***************************************
*        SPI Register Settings
***************************************/

#define `$INSTANCE_NAME`_CTRL_SPI      (`$INSTANCE_NAME`_CTRL_MODE_SPI)
#define `$INSTANCE_NAME`_SPI_RX_CTRL   (`$INSTANCE_NAME`_RX_CTRL_ENABLED)
#define `$INSTANCE_NAME`_SPI_TX_CTRL   (`$INSTANCE_NAME`_TX_CTRL_ENABLED)


/***************************************
*       SPI Init Register Settings
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG)

    /* SPI Configuration */
    #define `$INSTANCE_NAME`_SPI_DEFAULT_CTRL \
                    (`$INSTANCE_NAME`_GET_CTRL_OVS(`$INSTANCE_NAME`_SPI_OVS_FACTOR)         | \
                     `$INSTANCE_NAME`_GET_CTRL_EC_AM_MODE(`$INSTANCE_NAME`_SPI_WAKE_ENABLE) | \
                     `$INSTANCE_NAME`_CTRL_SPI)

    #define `$INSTANCE_NAME`_SPI_DEFAULT_SPI_CTRL \
                    (`$INSTANCE_NAME`_GET_SPI_CTRL_CONTINUOUS    (`$INSTANCE_NAME`_SPI_TRANSFER_SEPARATION)       | \
                     `$INSTANCE_NAME`_GET_SPI_CTRL_SELECT_PRECEDE(`$INSTANCE_NAME`_SPI_SUB_MODE &                   \
                                                                  `$INSTANCE_NAME`_SPI_MODE_TI_PRECEDES_MASK)     | \
                     `$INSTANCE_NAME`_GET_SPI_CTRL_SCLK_MODE     (`$INSTANCE_NAME`_SPI_CLOCK_MODE)                | \
                     `$INSTANCE_NAME`_GET_SPI_CTRL_LATE_MISO_SAMPLE(`$INSTANCE_NAME`_SPI_LATE_MISO_SAMPLE_ENABLE) | \
                     `$INSTANCE_NAME`_GET_SPI_CTRL_SUB_MODE      (`$INSTANCE_NAME`_SPI_SUB_MODE)                  | \
                     `$INSTANCE_NAME`_GET_SPI_CTRL_MASTER_MODE   (`$INSTANCE_NAME`_SPI_MODE))

    /* RX direction */
    #define `$INSTANCE_NAME`_SPI_DEFAULT_RX_CTRL \
                    (`$INSTANCE_NAME`_GET_RX_CTRL_DATA_WIDTH(`$INSTANCE_NAME`_SPI_RX_DATA_BITS_NUM)     | \
                     `$INSTANCE_NAME`_GET_RX_CTRL_BIT_ORDER (`$INSTANCE_NAME`_SPI_BITS_ORDER)           | \
                     `$INSTANCE_NAME`_GET_RX_CTRL_MEDIAN    (`$INSTANCE_NAME`_SPI_MEDIAN_FILTER_ENABLE) | \
                     `$INSTANCE_NAME`_SPI_RX_CTRL)

    #define `$INSTANCE_NAME`_SPI_DEFAULT_RX_FIFO_CTRL \
                    `$INSTANCE_NAME`_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(`$INSTANCE_NAME`_SPI_RX_TRIGGER_LEVEL)

    /* TX direction */
    #define `$INSTANCE_NAME`_SPI_DEFAULT_TX_CTRL \
                    (`$INSTANCE_NAME`_GET_TX_CTRL_DATA_WIDTH(`$INSTANCE_NAME`_SPI_TX_DATA_BITS_NUM) | \
                     `$INSTANCE_NAME`_GET_TX_CTRL_BIT_ORDER (`$INSTANCE_NAME`_SPI_BITS_ORDER)       | \
                     `$INSTANCE_NAME`_SPI_TX_CTRL)

    #define `$INSTANCE_NAME`_SPI_DEFAULT_TX_FIFO_CTRL \
                    `$INSTANCE_NAME`_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(`$INSTANCE_NAME`_SPI_TX_TRIGGER_LEVEL)

    /* Interrupt sources */
    #define `$INSTANCE_NAME`_SPI_DEFAULT_INTR_SPI_EC_MASK   (`$INSTANCE_NAME`_NO_INTR_SOURCES)

    #define `$INSTANCE_NAME`_SPI_DEFAULT_INTR_I2C_EC_MASK   (`$INSTANCE_NAME`_NO_INTR_SOURCES)
    #define `$INSTANCE_NAME`_SPI_DEFAULT_INTR_SLAVE_MASK \
                    (`$INSTANCE_NAME`_SPI_INTR_RX_MASK & `$INSTANCE_NAME`_INTR_SLAVE_SPI_BUS_ERROR)

    #define `$INSTANCE_NAME`_SPI_DEFAULT_INTR_MASTER_MASK \
                    (`$INSTANCE_NAME`_SPI_INTR_TX_MASK & `$INSTANCE_NAME`_INTR_MASTER_SPI_DONE)

    #define `$INSTANCE_NAME`_SPI_DEFAULT_INTR_RX_MASK \
                    (`$INSTANCE_NAME`_SPI_INTR_RX_MASK & (uint32) ~`$INSTANCE_NAME`_INTR_SLAVE_SPI_BUS_ERROR)

    #define `$INSTANCE_NAME`_SPI_DEFAULT_INTR_TX_MASK \
                    (`$INSTANCE_NAME`_SPI_INTR_TX_MASK & (uint32) ~`$INSTANCE_NAME`_INTR_MASTER_SPI_DONE)

#endif /* (`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG) */


/***************************************
*        UART Register Settings
***************************************/

#define `$INSTANCE_NAME`_CTRL_UART      (`$INSTANCE_NAME`_CTRL_MODE_UART)
#define `$INSTANCE_NAME`_UART_RX_CTRL   (`$INSTANCE_NAME`_RX_CTRL_LSB_FIRST) /* LSB for UART goes first */
#define `$INSTANCE_NAME`_UART_TX_CTRL   (`$INSTANCE_NAME`_TX_CTRL_LSB_FIRST) /* LSB for UART goes first */


/***************************************
*      UART Init Register Settings
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG)

    /* UART configuration */
    #if(`$INSTANCE_NAME`_UART_MODE_IRDA == `$INSTANCE_NAME`_UART_SUB_MODE)

        #define `$INSTANCE_NAME`_DEFAULT_CTRL_OVS   ((0u != `$INSTANCE_NAME`_UART_IRDA_LOW_POWER) ?              \
                                (`$INSTANCE_NAME`_UART_GET_CTRL_OVS_IRDA_LP(`$INSTANCE_NAME`_UART_OVS_FACTOR)) : \
                                (`$INSTANCE_NAME`_CTRL_OVS_IRDA_OVS16))

    #else

        #define `$INSTANCE_NAME`_DEFAULT_CTRL_OVS   `$INSTANCE_NAME`_GET_CTRL_OVS(`$INSTANCE_NAME`_UART_OVS_FACTOR)

    #endif /* (`$INSTANCE_NAME`_UART_MODE_IRDA == `$INSTANCE_NAME`_UART_SUB_MODE) */

    #define `$INSTANCE_NAME`_UART_DEFAULT_CTRL \
                                (`$INSTANCE_NAME`_GET_CTRL_ADDR_ACCEPT(`$INSTANCE_NAME`_UART_MP_ACCEPT_ADDRESS) | \
                                 `$INSTANCE_NAME`_DEFAULT_CTRL_OVS                                              | \
                                 `$INSTANCE_NAME`_CTRL_UART)

    #define `$INSTANCE_NAME`_UART_DEFAULT_UART_CTRL \
                                    (`$INSTANCE_NAME`_GET_UART_CTRL_MODE(`$INSTANCE_NAME`_UART_SUB_MODE))

    /* RX direction */
    #define `$INSTANCE_NAME`_UART_DEFAULT_RX_CTRL_PARITY \
                                ((`$INSTANCE_NAME`_UART_PARITY_NONE != `$INSTANCE_NAME`_UART_PARITY_TYPE) ?      \
                                  (`$INSTANCE_NAME`_GET_UART_RX_CTRL_PARITY(`$INSTANCE_NAME`_UART_PARITY_TYPE) | \
                                   `$INSTANCE_NAME`_UART_RX_CTRL_PARITY_ENABLED) : (0u))

    #define `$INSTANCE_NAME`_UART_DEFAULT_UART_RX_CTRL \
                    (`$INSTANCE_NAME`_GET_UART_RX_CTRL_MODE(`$INSTANCE_NAME`_UART_STOP_BITS_NUM)                    | \
                     `$INSTANCE_NAME`_GET_UART_RX_CTRL_POLARITY(`$INSTANCE_NAME`_UART_IRDA_POLARITY)                | \
                     `$INSTANCE_NAME`_GET_UART_RX_CTRL_MP_MODE(`$INSTANCE_NAME`_UART_MP_MODE_ENABLE)                | \
                     `$INSTANCE_NAME`_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(`$INSTANCE_NAME`_UART_DROP_ON_PARITY_ERR) | \
                     `$INSTANCE_NAME`_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(`$INSTANCE_NAME`_UART_DROP_ON_FRAME_ERR)   | \
                     `$INSTANCE_NAME`_UART_DEFAULT_RX_CTRL_PARITY)

    #define `$INSTANCE_NAME`_UART_DEFAULT_RX_CTRL \
                                (`$INSTANCE_NAME`_GET_RX_CTRL_DATA_WIDTH(`$INSTANCE_NAME`_UART_DATA_BITS_NUM)        | \
                                 `$INSTANCE_NAME`_GET_RX_CTRL_MEDIAN    (`$INSTANCE_NAME`_UART_MEDIAN_FILTER_ENABLE) | \
                                 `$INSTANCE_NAME`_GET_UART_RX_CTRL_ENABLED(`$INSTANCE_NAME`_UART_DIRECTION))

    #define `$INSTANCE_NAME`_UART_DEFAULT_RX_FIFO_CTRL \
                                `$INSTANCE_NAME`_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(`$INSTANCE_NAME`_UART_RX_TRIGGER_LEVEL)

    #define `$INSTANCE_NAME`_UART_DEFAULT_RX_MATCH_REG  ((0u != `$INSTANCE_NAME`_UART_MP_MODE_ENABLE) ?          \
                                (`$INSTANCE_NAME`_GET_RX_MATCH_ADDR(`$INSTANCE_NAME`_UART_MP_RX_ADDRESS) | \
                                 `$INSTANCE_NAME`_GET_RX_MATCH_MASK(`$INSTANCE_NAME`_UART_MP_RX_ADDRESS_MASK)) : (0u))

    /* TX direction */
    #define `$INSTANCE_NAME`_UART_DEFAULT_TX_CTRL_PARITY (`$INSTANCE_NAME`_UART_DEFAULT_RX_CTRL_PARITY)

    #define `$INSTANCE_NAME`_UART_DEFAULT_UART_TX_CTRL \
                                (`$INSTANCE_NAME`_GET_UART_TX_CTRL_MODE(`$INSTANCE_NAME`_UART_STOP_BITS_NUM)       | \
                                 `$INSTANCE_NAME`_GET_UART_TX_CTRL_RETRY_NACK(`$INSTANCE_NAME`_UART_RETRY_ON_NACK) | \
                                 `$INSTANCE_NAME`_UART_DEFAULT_TX_CTRL_PARITY)

    #define `$INSTANCE_NAME`_UART_DEFAULT_TX_CTRL \
                                (`$INSTANCE_NAME`_GET_TX_CTRL_DATA_WIDTH(`$INSTANCE_NAME`_UART_DATA_BITS_NUM) | \
                                 `$INSTANCE_NAME`_GET_UART_TX_CTRL_ENABLED(`$INSTANCE_NAME`_UART_DIRECTION))

    #define `$INSTANCE_NAME`_UART_DEFAULT_TX_FIFO_CTRL \
                                `$INSTANCE_NAME`_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(`$INSTANCE_NAME`_UART_TX_TRIGGER_LEVEL)

    /* Interrupt sources */
    #define `$INSTANCE_NAME`_UART_DEFAULT_INTR_I2C_EC_MASK  (`$INSTANCE_NAME`_NO_INTR_SOURCES)
    #define `$INSTANCE_NAME`_UART_DEFAULT_INTR_SPI_EC_MASK  (`$INSTANCE_NAME`_NO_INTR_SOURCES)
    #define `$INSTANCE_NAME`_UART_DEFAULT_INTR_SLAVE_MASK   (`$INSTANCE_NAME`_NO_INTR_SOURCES)
    #define `$INSTANCE_NAME`_UART_DEFAULT_INTR_MASTER_MASK  (`$INSTANCE_NAME`_NO_INTR_SOURCES)
    #define `$INSTANCE_NAME`_UART_DEFAULT_INTR_RX_MASK      (`$INSTANCE_NAME`_UART_INTR_RX_MASK)
    #define `$INSTANCE_NAME`_UART_DEFAULT_INTR_TX_MASK      (`$INSTANCE_NAME`_UART_INTR_TX_MASK)

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG) */

#endif /* CY_SCB_SPI_UART_`$INSTANCE_NAME`_H */


/* [] END OF FILE */
