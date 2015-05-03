/*******************************************************************************
* File Name: `$INSTANCE_NAME`_SPI_UART_PVT.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_`$INSTANCE_NAME`_H)
#define CY_SCB_SPI_UART_PVT_`$INSTANCE_NAME`_H

#include "`$INSTANCE_NAME`_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  `$INSTANCE_NAME`_rxBufferHead;
    extern volatile uint32  `$INSTANCE_NAME`_rxBufferTail;
    extern volatile uint8   `$INSTANCE_NAME`_rxBufferOverflow;
#endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST) */

#if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  `$INSTANCE_NAME`_txBufferHead;
    extern volatile uint32  `$INSTANCE_NAME`_txBufferTail;
#endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST) */

#if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER)
    extern volatile `$BitWidthReplacementStringRx` `$INSTANCE_NAME`_rxBufferInternal[`$INSTANCE_NAME`_RX_BUFFER_SIZE];
#endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER) */

#if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER)
    extern volatile `$BitWidthReplacementStringTx` `$INSTANCE_NAME`_txBufferInternal[`$INSTANCE_NAME`_TX_BUFFER_SIZE];
#endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG)
    void `$INSTANCE_NAME`_SpiInit(void);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG) */

#if(`$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST)
    void `$INSTANCE_NAME`_SpiSaveConfig(void);
    void `$INSTANCE_NAME`_SpiRestoreConfig(void);
#endif /* (`$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST) */

#if(`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG)
    void `$INSTANCE_NAME`_UartInit(void);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG) */

#if(`$INSTANCE_NAME`_UART_WAKE_ENABLE_CONST)
    void `$INSTANCE_NAME`_UartSaveConfig(void);
    void `$INSTANCE_NAME`_UartRestoreConfig(void);
#endif /* (`$INSTANCE_NAME`_UART_WAKE_ENABLE_CONST) */

/* Interrupt processing */
#define `$INSTANCE_NAME`_SpiUartEnableIntRx(intSourceMask)  `$INSTANCE_NAME`_SetRxInterruptMode(intSourceMask)
#define `$INSTANCE_NAME`_SpiUartEnableIntTx(intSourceMask)  `$INSTANCE_NAME`_SetTxInterruptMode(intSourceMask)
uint32 `$INSTANCE_NAME`_SpiUartDisableIntRx(void);
uint32 `$INSTANCE_NAME`_SpiUartDisableIntTx(void);

#endif /* (CY_SCB_SPI_UART_PVT_`$INSTANCE_NAME`_H) */


/* [] END OF FILE */
