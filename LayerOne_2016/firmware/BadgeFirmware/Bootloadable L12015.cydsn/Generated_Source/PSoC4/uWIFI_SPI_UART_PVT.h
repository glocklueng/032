/*******************************************************************************
* File Name: uWIFI_SPI_UART_PVT.h
* Version 2.0
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_uWIFI_H)
#define CY_SCB_SPI_UART_PVT_uWIFI_H

#include "uWIFI_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if(uWIFI_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  uWIFI_rxBufferHead;
    extern volatile uint32  uWIFI_rxBufferTail;
    extern volatile uint8   uWIFI_rxBufferOverflow;
#endif /* (uWIFI_INTERNAL_RX_SW_BUFFER_CONST) */

#if(uWIFI_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  uWIFI_txBufferHead;
    extern volatile uint32  uWIFI_txBufferTail;
#endif /* (uWIFI_INTERNAL_TX_SW_BUFFER_CONST) */

#if(uWIFI_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 uWIFI_rxBufferInternal[uWIFI_RX_BUFFER_SIZE];
#endif /* (uWIFI_INTERNAL_RX_SW_BUFFER) */

#if(uWIFI_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 uWIFI_txBufferInternal[uWIFI_TX_BUFFER_SIZE];
#endif /* (uWIFI_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(uWIFI_SCB_MODE_SPI_CONST_CFG)
    void uWIFI_SpiInit(void);
#endif /* (uWIFI_SCB_MODE_SPI_CONST_CFG) */

#if(uWIFI_SPI_WAKE_ENABLE_CONST)
    void uWIFI_SpiSaveConfig(void);
    void uWIFI_SpiRestoreConfig(void);
#endif /* (uWIFI_SPI_WAKE_ENABLE_CONST) */

#if(uWIFI_SCB_MODE_UART_CONST_CFG)
    void uWIFI_UartInit(void);
#endif /* (uWIFI_SCB_MODE_UART_CONST_CFG) */

#if(uWIFI_UART_WAKE_ENABLE_CONST)
    void uWIFI_UartSaveConfig(void);
    void uWIFI_UartRestoreConfig(void);
    #define uWIFI_UartStop() \
        do{                             \
            uWIFI_UART_RX_CTRL_REG &= ~uWIFI_UART_RX_CTRL_SKIP_START; \
        }while(0)
#else
        #define uWIFI_UartStop() do{ /* Does nothing */ }while(0)

#endif /* (uWIFI_UART_WAKE_ENABLE_CONST) */

/* Interrupt processing */
#define uWIFI_SpiUartEnableIntRx(intSourceMask)  uWIFI_SetRxInterruptMode(intSourceMask)
#define uWIFI_SpiUartEnableIntTx(intSourceMask)  uWIFI_SetTxInterruptMode(intSourceMask)
uint32  uWIFI_SpiUartDisableIntRx(void);
uint32  uWIFI_SpiUartDisableIntTx(void);


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in uWIFI_SetPins() */
#define uWIFI_UART_RX_PIN_ENABLE    (uWIFI_UART_RX)
#define uWIFI_UART_TX_PIN_ENABLE    (uWIFI_UART_TX)

/* UART RTS and CTS position to be used in  uWIFI_SetPins() */
#define uWIFI_UART_RTS_PIN_ENABLE    (0x10u)
#define uWIFI_UART_CTS_PIN_ENABLE    (0x20u)

#endif /* (CY_SCB_SPI_UART_PVT_uWIFI_H) */


/* [] END OF FILE */
