/*******************************************************************************
* File Name: uWIFI_SPI_UART_INT.c
* Version 2.0
*
* Description:
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "uWIFI_PVT.h"
#include "uWIFI_SPI_UART_PVT.h"


/*******************************************************************************
* Function Name: uWIFI_SPI_UART_ISR
********************************************************************************
*
* Summary:
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR(uWIFI_SPI_UART_ISR)
{
#if(uWIFI_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
    uint32 dataRx;
#endif /* (uWIFI_INTERNAL_RX_SW_BUFFER_CONST) */

#if(uWIFI_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (uWIFI_INTERNAL_TX_SW_BUFFER_CONST) */

    if(NULL != uWIFI_customIntrHandler)
    {
        uWIFI_customIntrHandler();
    }

    #if(uWIFI_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        uWIFI_ClearSpiExtClkInterruptSource(uWIFI_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if(uWIFI_CHECK_RX_SW_BUFFER)
    {
        if(uWIFI_CHECK_INTR_RX_MASKED(uWIFI_INTR_RX_NOT_EMPTY))
        {
            while(0u != uWIFI_GET_RX_FIFO_ENTRIES)
            {
                /* Get data from RX FIFO */
                dataRx = uWIFI_RX_FIFO_RD_REG;

                /* Move local head index */
                locHead = (uWIFI_rxBufferHead + 1u);

                /* Adjust local head index */
                if(uWIFI_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if(locHead == uWIFI_rxBufferTail)
                {
                    /* Overflow: through away new data */
                    uWIFI_rxBufferOverflow = (uint8) uWIFI_INTR_RX_OVERFLOW;
                }
                else
                {
                    /* Store received data */
                    uWIFI_PutWordInRxBuffer(locHead, dataRx);

                    /* Move head index */
                    uWIFI_rxBufferHead = locHead;
                }
            }

            uWIFI_ClearRxInterruptSource(uWIFI_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if(uWIFI_CHECK_TX_SW_BUFFER)
    {
        if(uWIFI_CHECK_INTR_TX_MASKED(uWIFI_INTR_TX_NOT_FULL))
        {
            /* Put data into TX FIFO */
            while(uWIFI_SPI_UART_FIFO_SIZE != uWIFI_GET_TX_FIFO_ENTRIES)
            {
                /* Check for room in TX software buffer */
                if(uWIFI_txBufferHead != uWIFI_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (uWIFI_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if(uWIFI_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    uWIFI_TX_FIFO_WR_REG = uWIFI_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    uWIFI_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    uWIFI_DISABLE_INTR_TX(uWIFI_INTR_TX_NOT_FULL);
                    break;
                }
            }

            uWIFI_ClearTxInterruptSource(uWIFI_INTR_TX_NOT_FULL);
        }
    }
    #endif
}


/* [] END OF FILE */
