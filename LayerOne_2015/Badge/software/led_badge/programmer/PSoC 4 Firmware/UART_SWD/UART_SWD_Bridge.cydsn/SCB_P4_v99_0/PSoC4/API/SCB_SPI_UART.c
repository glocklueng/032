/*******************************************************************************
* File Name: `$INSTANCE_NAME`_SPI_UART.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
*******************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`_PVT.h"
#include "`$INSTANCE_NAME`_SPI_UART_PVT.h"

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST)
    volatile uint32 `$INSTANCE_NAME`_rxBufferHead;
    volatile uint32 `$INSTANCE_NAME`_rxBufferTail;
    volatile uint8  `$INSTANCE_NAME`_rxBufferOverflow;
#endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST) */

#if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST)
    volatile uint32 `$INSTANCE_NAME`_txBufferHead;
    volatile uint32 `$INSTANCE_NAME`_txBufferTail;
#endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST) */

#if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile `$BitWidthReplacementStringRx` `$INSTANCE_NAME`_rxBufferInternal[`$INSTANCE_NAME`_RX_BUFFER_SIZE];
#endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER) */

#if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER)
    volatile `$BitWidthReplacementStringTx` `$INSTANCE_NAME`_txBufferInternal[`$INSTANCE_NAME`_TX_BUFFER_SIZE];
#endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER) */


#if(`$INSTANCE_NAME`_RX_DIRECTION)

    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiUartReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Retrieves the next data element from the receive buffer. Undefined data will
    *  be returned if the RX buffer is empty.
    *  Call `$INSTANCE_NAME`_SpiUartGetRxBufferSize() to return buffer size.
    *   - RX software buffer disabled: Returns data element retrieved from RX FIFO.
    *   - RX software buffer enabled: Returns data element from the software
    *     receive buffer.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Next data element from the receive buffer.
    *
    * Global Variables:
    *  Look into `$INSTANCE_NAME`_SpiInit for description.
    *
    *******************************************************************************/
    uint32 `$INSTANCE_NAME`_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

        #if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 locTail;
        #endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST) */

        `$Cond`if(`$INSTANCE_NAME`_CHECK_RX_SW_BUFFER)
        {
            if(`$INSTANCE_NAME`_rxBufferHead != `$INSTANCE_NAME`_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (`$INSTANCE_NAME`_rxBufferTail + 1u);

                if(`$INSTANCE_NAME`_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data fron RX software buffer */
                rxData = `$INSTANCE_NAME`_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                `$INSTANCE_NAME`_rxBufferTail = locTail;
            }
        }
        `$Cond`else
        {
            rxData = `$INSTANCE_NAME`_RX_FIFO_RD_REG; /* Read data from RX FIFO */
        }
        `$EndCond`

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiUartGetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in receive buffer.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of received data elements
    *
    *******************************************************************************/
    uint32 `$INSTANCE_NAME`_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
        #if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 locHead;
        #endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST) */

        `$Cond`if(`$INSTANCE_NAME`_CHECK_RX_SW_BUFFER)
        {
            locHead = `$INSTANCE_NAME`_rxBufferHead;

            if(locHead >= `$INSTANCE_NAME`_rxBufferTail)
            {
                size = (locHead - `$INSTANCE_NAME`_rxBufferTail);
            }
            else
            {
                size = (locHead + (`$INSTANCE_NAME`_RX_BUFFER_SIZE - `$INSTANCE_NAME`_rxBufferTail));
            }
        }
        `$Cond`else
        {
            size = `$INSTANCE_NAME`_GET_RX_FIFO_ENTRIES;
        }
        `$EndCond`

        return(size);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiUartClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clear the receive buffer and RX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SpiUartClearRxBuffer(void)
    {
        #if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 intSourceMask;
        #endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST) */

        `$Cond`if(`$INSTANCE_NAME`_CHECK_RX_SW_BUFFER)
        {
            intSourceMask = `$INSTANCE_NAME`_SpiUartDisableIntRx();

            `$INSTANCE_NAME`_CLEAR_RX_FIFO;

            /* Flush RX software buffer */
            `$INSTANCE_NAME`_rxBufferHead     = `$INSTANCE_NAME`_rxBufferTail;
            `$INSTANCE_NAME`_rxBufferOverflow = 0u;

            /* End RX transfer */
            `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_ALL);

            `$INSTANCE_NAME`_SpiUartEnableIntRx(intSourceMask);
        }
        `$Cond`else
        {
            `$INSTANCE_NAME`_CLEAR_RX_FIFO;
        }
        `$EndCond`
    }

#endif /* (`$INSTANCE_NAME`_RX_DIRECTION) */


#if(`$INSTANCE_NAME`_TX_DIRECTION)

    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiUartWriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    * Parameters:
    *  txDataByte: the data to be transmitted.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SpiUartWriteTxData(uint32 txDataByte)
    {
        #if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 locHead;
            uint32 intSourceMask;
        #endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST) */

        `$Cond`if(`$INSTANCE_NAME`_CHECK_TX_SW_BUFFER)
        {
            /* Head index to put data */
            locHead = (`$INSTANCE_NAME`_txBufferHead + 1u);

            /* Adjust TX software buffer index */
            if(`$INSTANCE_NAME`_TX_BUFFER_SIZE == locHead)
            {
                locHead = 0u;
            }

            while(locHead == `$INSTANCE_NAME`_txBufferTail)
            {
                /* Wait for space in the TX software buffer */
            }

            /* The TX software buffer has at least one room */

            if((`$INSTANCE_NAME`_txBufferHead == `$INSTANCE_NAME`_txBufferTail) &&
               (`$INSTANCE_NAME`_FIFO_SIZE != `$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                `$INSTANCE_NAME`_TX_FIFO_WR_REG = txDataByte;
            }
            /* Put data in the TX software buffer */
            else
            {
                /* Clear old status of INTR_TX_EMPTY. It sets at the end of transfer: TX FIFO empty. */
                `$INSTANCE_NAME`_ClearTxInterruptSource(`$INSTANCE_NAME`_INTR_TX_NOT_FULL);

                `$INSTANCE_NAME`_PutWordInTxBuffer(locHead, txDataByte);

                `$INSTANCE_NAME`_txBufferHead = locHead;

                /* Enable interrupt to transmit */
                intSourceMask  = `$INSTANCE_NAME`_INTR_TX_NOT_FULL;
                intSourceMask |= `$INSTANCE_NAME`_GetTxInterruptMode();
                `$INSTANCE_NAME`_SpiUartEnableIntTx(intSourceMask);
            }
        }
        `$Cond`else
        {
            while(`$INSTANCE_NAME`_FIFO_SIZE == `$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES)
            {
                /* Block while TX FIFO is FULL */
            }

            `$INSTANCE_NAME`_TX_FIFO_WR_REG = txDataByte;
        }
        `$EndCond`
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiUartPutArray
    ********************************************************************************
    *
    * Summary:
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * Parameters:
    *  wrBuf:  pointer to an array with data to be placed in transmit buffer.
    *  count:  number of data elements to be placed in the transmit buffer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SpiUartPutArray(const `$BitWidthReplacementStringTx` wrBuf[], uint32 count)
    {
        uint32 i;

        for(i=0u; i < count; i++)
        {
            `$INSTANCE_NAME`_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiUartGetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of elements currently in the transmit buffer.
    *  TX software buffer disabled: returns the number of used entries in TX FIFO.
    *  TX software buffer enabled: returns the number of elements currently used
    *  in the transmit buffer. This number does not include used entries in the
    *  TX FIFO. The transmit buffer size is zero until the TX FIFO is full.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of data elements ready to transmit.
    *
    *******************************************************************************/
    uint32 `$INSTANCE_NAME`_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
        #if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 locTail;
        #endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST) */

        `$Cond`if(`$INSTANCE_NAME`_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = `$INSTANCE_NAME`_txBufferTail;

            if(`$INSTANCE_NAME`_txBufferHead >= locTail)
            {
                size = (`$INSTANCE_NAME`_txBufferHead - locTail);
            }
            else
            {
                size = (`$INSTANCE_NAME`_txBufferHead + (`$INSTANCE_NAME`_TX_BUFFER_SIZE - locTail));
            }
        }
        `$Cond`else
        {
            size = `$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES;
        }
        `$EndCond`

        return(size);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiUartClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the transmit buffer and TX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SpiUartClearTxBuffer(void)
    {
        #if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 intSourceMask;
        #endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST) */

        `$Cond`if(`$INSTANCE_NAME`_CHECK_TX_SW_BUFFER)
        {
            intSourceMask = `$INSTANCE_NAME`_SpiUartDisableIntTx();

            `$INSTANCE_NAME`_CLEAR_TX_FIFO;

            /* Flush TX software buffer */
            `$INSTANCE_NAME`_txBufferHead = `$INSTANCE_NAME`_txBufferTail;

            /* End TX transfer if it is in progress */
            intSourceMask &= (uint32) ~`$INSTANCE_NAME`_INTR_TX_NOT_FULL;

            `$INSTANCE_NAME`_SpiUartEnableIntTx(intSourceMask);
        }
        `$Cond`else
        {
            `$INSTANCE_NAME`_CLEAR_TX_FIFO;
        }
        `$EndCond`
    }

#endif /* (`$INSTANCE_NAME`_TX_DIRECTION) */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SpiUartDisableIntRx
********************************************************************************
*
* Summary:
*  Disables RX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns RX interrupt soureces enabled before function call.
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = `$INSTANCE_NAME`_GetRxInterruptMode();

    `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);

    return(intSource);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SpiUartDisableIntTx
********************************************************************************
*
* Summary:
*  Disables TX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns TX interrupt soureces enabled before function call.
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = `$INSTANCE_NAME`_GetTxInterruptMode();

    `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);

    return(intSourceMask);
}


#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)

    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_PutWordInRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores byte/word into the RX buffer.
    *  Only available in Unconfigured operation mode.
    *
    * Parameters:
    *  index:      index to store data byte/word in the RX buffer.
    *  rxDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in the buffer */
        if(`$INSTANCE_NAME`_ONE_BYTE_WIDTH == `$INSTANCE_NAME`_rxDataBits)
        {
            `$INSTANCE_NAME`_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            `$INSTANCE_NAME`_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            `$INSTANCE_NAME`_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_GetWordFromRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from RX buffer.
    *  Only available in Unconfigured operation mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 `$INSTANCE_NAME`_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if(`$INSTANCE_NAME`_ONE_BYTE_WIDTH == `$INSTANCE_NAME`_rxDataBits)
        {
            value = `$INSTANCE_NAME`_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) `$INSTANCE_NAME`_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)`$INSTANCE_NAME`_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return(value);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_PutWordInTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores byte/word into the TX buffer.
    * Only available in Unconfigured operation mode.
    *
    * Parameters:
    *  idx:        index to store data byte/word in the TX buffer.
    *  txDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in the buffer */
        if(`$INSTANCE_NAME`_ONE_BYTE_WIDTH == `$INSTANCE_NAME`_txDataBits)
        {
            `$INSTANCE_NAME`_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            `$INSTANCE_NAME`_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            `$INSTANCE_NAME`_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_GetWordFromTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from TX buffer.
    *  Only available in Unconfigured operation mode.
    *
    * Parameters:
    *  idx: index to get data byte/word from the TX buffer.
    *
    * Return:
    *  Returns byte/word read from TX buffer.
    *
    *******************************************************************************/
    uint32 `$INSTANCE_NAME`_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if(`$INSTANCE_NAME`_ONE_BYTE_WIDTH == `$INSTANCE_NAME`_txDataBits)
        {
            value = (uint32) `$INSTANCE_NAME`_txBuffer[idx];
        }
        else
        {
            value  = (uint32) `$INSTANCE_NAME`_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) `$INSTANCE_NAME`_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return(value);
    }

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
