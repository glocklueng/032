/*******************************************************************************
* File Name: `$INSTANCE_NAME`_SPI_UART_INT.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the Interrupt Servive Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`_PVT.h"
#include "`$INSTANCE_NAME`_SPI_UART_PVT.h"


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SPI_UART_ISR
********************************************************************************
*
* Summary:
*  Handles Interrupt Service Routine for SCB SPI or UART modes.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_SPI_UART_ISR)
{
    #if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locHead;
        uint32 dataRx;
    #endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST) */

    if(NULL != `$INSTANCE_NAME`_customIntrHandler)
    {
        `$INSTANCE_NAME`_customIntrHandler(); /* Call customer routine if needed */
    }

    `$Cond`if(`$INSTANCE_NAME`_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        `$INSTANCE_NAME`_ClearSpiExtClkInterruptSource(`$INSTANCE_NAME`_INTR_SPI_EC_WAKE_UP);
    }
    `$EndCond`

    `$Cond`if(`$INSTANCE_NAME`_CHECK_RX_SW_BUFFER)
    {
        /* Get data from RX FIFO */
        if(`$INSTANCE_NAME`_CHECK_INTR_RX_MASKED(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY))
        {
            while(0u != `$INSTANCE_NAME`_GET_RX_FIFO_ENTRIES)
            {
                /* Get data from RX FIFO */
                dataRx = `$INSTANCE_NAME`_RX_FIFO_RD_REG;

                /* Move local head index */
                locHead = (`$INSTANCE_NAME`_rxBufferHead + 1u);

                /* Adjust local head index */
                if(`$INSTANCE_NAME`_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if(locHead == `$INSTANCE_NAME`_rxBufferTail)
                {
                    /* Overflow: through away new data */
                    `$INSTANCE_NAME`_rxBufferOverflow = (uint8) `$INSTANCE_NAME`_INTR_RX_OVERFLOW;
                }
                else
                {
                    /* Store received data */
                    `$INSTANCE_NAME`_PutWordInRxBuffer(locHead, dataRx);

                    /* Move head index */
                    `$INSTANCE_NAME`_rxBufferHead = locHead;
                }
            }

            `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY);
        }
    }
    `$EndCond`


    `$Cond`if(`$INSTANCE_NAME`_CHECK_TX_SW_BUFFER)
    {
        if(`$INSTANCE_NAME`_CHECK_INTR_TX_MASKED(`$INSTANCE_NAME`_INTR_TX_NOT_FULL))
        {
            /* Put data into TX FIFO */
            while(`$INSTANCE_NAME`_FIFO_SIZE != `$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES)
            {
                /* There is a data in TX software buffer */
                if(`$INSTANCE_NAME`_txBufferHead != `$INSTANCE_NAME`_txBufferTail)
                {
                    /* Mode local tail index */
                    locTail = (`$INSTANCE_NAME`_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if(`$INSTANCE_NAME`_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    `$INSTANCE_NAME`_TX_FIFO_WR_REG = `$INSTANCE_NAME`_GetWordFromTxBuffer(locTail);

                    /* Mode tail index */
                    `$INSTANCE_NAME`_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is EMPTY: end of transmitting */
                    `$INSTANCE_NAME`_DISABLE_INTR_TX(`$INSTANCE_NAME`_INTR_TX_NOT_FULL);
                    break;
                }
            }

            `$INSTANCE_NAME`_ClearTxInterruptSource(`$INSTANCE_NAME`_INTR_TX_NOT_FULL);
        }
    }
    `$EndCond`
}


/* [] END OF FILE */
