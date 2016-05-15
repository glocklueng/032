/*******************************************************************************
* File Name: uWIFI_SPI_UART.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
*******************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "uWIFI_PVT.h"
#include "uWIFI_SPI_UART_PVT.h"

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(uWIFI_INTERNAL_RX_SW_BUFFER_CONST)
    volatile uint32 uWIFI_rxBufferHead;
    volatile uint32 uWIFI_rxBufferTail;
    volatile uint8  uWIFI_rxBufferOverflow;
#endif /* (uWIFI_INTERNAL_RX_SW_BUFFER_CONST) */

#if(uWIFI_INTERNAL_TX_SW_BUFFER_CONST)
    volatile uint32 uWIFI_txBufferHead;
    volatile uint32 uWIFI_txBufferTail;
#endif /* (uWIFI_INTERNAL_TX_SW_BUFFER_CONST) */

#if(uWIFI_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 uWIFI_rxBufferInternal[uWIFI_RX_BUFFER_SIZE];
#endif /* (uWIFI_INTERNAL_RX_SW_BUFFER) */

#if(uWIFI_INTERNAL_TX_SW_BUFFER)
    volatile uint8 uWIFI_txBufferInternal[uWIFI_TX_BUFFER_SIZE];
#endif /* (uWIFI_INTERNAL_TX_SW_BUFFER) */


#if(uWIFI_RX_DIRECTION)

    /*******************************************************************************
    * Function Name: uWIFI_SpiUartReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Retrieves the next data element from the receive buffer.
    *   - RX software buffer is disabled: Returns data element retrieved from
    *     RX FIFO. Undefined data will be returned if the RX FIFO is empty.
    *   - RX software buffer is enabled: Returns data element from the software
    *     receive buffer. Zero value is returned if the software receive buffer
    *     is empty.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Next data element from the receive buffer.
    *
    * Global Variables:
    *  Look into uWIFI_SpiInit for description.
    *
    *******************************************************************************/
    uint32 uWIFI_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

        #if(uWIFI_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 locTail;
        #endif /* (uWIFI_INTERNAL_RX_SW_BUFFER_CONST) */

        #if(uWIFI_CHECK_RX_SW_BUFFER)
        {
            if(uWIFI_rxBufferHead != uWIFI_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (uWIFI_rxBufferTail + 1u);

                if(uWIFI_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data from RX software buffer */
                rxData = uWIFI_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                uWIFI_rxBufferTail = locTail;
            }
        }
        #else
        {
            rxData = uWIFI_RX_FIFO_RD_REG; /* Read data from RX FIFO */
        }
        #endif

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: uWIFI_SpiUartGetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in the receive buffer. This does not include the hardware RX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of received data elements
    *
    *******************************************************************************/
    uint32 uWIFI_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
        #if(uWIFI_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 locHead;
        #endif /* (uWIFI_INTERNAL_RX_SW_BUFFER_CONST) */

        #if(uWIFI_CHECK_RX_SW_BUFFER)
        {
            locHead = uWIFI_rxBufferHead;

            if(locHead >= uWIFI_rxBufferTail)
            {
                size = (locHead - uWIFI_rxBufferTail);
            }
            else
            {
                size = (locHead + (uWIFI_RX_BUFFER_SIZE - uWIFI_rxBufferTail));
            }
        }
        #else
        {
            size = uWIFI_GET_RX_FIFO_ENTRIES;
        }
        #endif

        return(size);
    }


    /*******************************************************************************
    * Function Name: uWIFI_SpiUartClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receive buffer and RX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uWIFI_SpiUartClearRxBuffer(void)
    {
        #if(uWIFI_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 intSourceMask;
        #endif /* (uWIFI_INTERNAL_RX_SW_BUFFER_CONST) */

        #if(uWIFI_CHECK_RX_SW_BUFFER)
        {
            intSourceMask = uWIFI_SpiUartDisableIntRx();

            uWIFI_CLEAR_RX_FIFO;

            /* Flush RX software buffer */
            uWIFI_rxBufferHead     = uWIFI_rxBufferTail;
            uWIFI_rxBufferOverflow = 0u;

            /* End RX transfer */
            uWIFI_ClearRxInterruptSource(uWIFI_INTR_RX_ALL);

            uWIFI_SpiUartEnableIntRx(intSourceMask);
        }
        #else
        {
            uWIFI_CLEAR_RX_FIFO;
        }
        #endif
    }

#endif /* (uWIFI_RX_DIRECTION) */


#if(uWIFI_TX_DIRECTION)

    /*******************************************************************************
    * Function Name: uWIFI_SpiUartWriteTxData
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
    void uWIFI_SpiUartWriteTxData(uint32 txData)
    {
        #if(uWIFI_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 locHead;
            uint32 intSourceMask;
        #endif /* (uWIFI_INTERNAL_TX_SW_BUFFER_CONST) */

        #if(uWIFI_CHECK_TX_SW_BUFFER)
        {
            /* Head index to put data */
            locHead = (uWIFI_txBufferHead + 1u);

            /* Adjust TX software buffer index */
            if(uWIFI_TX_BUFFER_SIZE == locHead)
            {
                locHead = 0u;
            }

            while(locHead == uWIFI_txBufferTail)
            {
                /* Wait for space in TX software buffer */
            }

            /* TX software buffer has at least one room */

            if((uWIFI_txBufferHead == uWIFI_txBufferTail) &&
               (uWIFI_SPI_UART_FIFO_SIZE != uWIFI_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                uWIFI_TX_FIFO_WR_REG = txData;
            }
            /* Put data in TX software buffer */
            else
            {
                /* Clear old status of INTR_TX_NOT_FULL. It sets at the end of transfer when TX FIFO is empty. */
                uWIFI_ClearTxInterruptSource(uWIFI_INTR_TX_NOT_FULL);

                uWIFI_PutWordInTxBuffer(locHead, txData);

                uWIFI_txBufferHead = locHead;

                /* Enable interrupt to transmit */
                intSourceMask  = uWIFI_INTR_TX_NOT_FULL;
                intSourceMask |= uWIFI_GetTxInterruptMode();
                uWIFI_SpiUartEnableIntTx(intSourceMask);
            }
        }
        #else
        {
            while(uWIFI_SPI_UART_FIFO_SIZE == uWIFI_GET_TX_FIFO_ENTRIES)
            {
                /* Block while TX FIFO is FULL */
            }

            uWIFI_TX_FIFO_WR_REG = txData;
        }
        #endif
    }


    /*******************************************************************************
    * Function Name: uWIFI_SpiUartPutArray
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
    void uWIFI_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for(i=0u; i < count; i++)
        {
            uWIFI_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: uWIFI_SpiUartGetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    * Returns the number of elements currently in the transmit buffer.
    *  - TX software buffer is disabled: returns the number of used entries in
    *    TX FIFO.
    *  - TX software buffer is enabled: returns the number of elements currently
    *    used in the transmit buffer. This number does not include used entries in
    *    the TX FIFO. The transmit buffer size is zero until the TX FIFO is
    *    not full.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of data elements ready to transmit.
    *
    *******************************************************************************/
    uint32 uWIFI_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
        #if(uWIFI_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 locTail;
        #endif /* (uWIFI_INTERNAL_TX_SW_BUFFER_CONST) */

        #if(uWIFI_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = uWIFI_txBufferTail;

            if(uWIFI_txBufferHead >= locTail)
            {
                size = (uWIFI_txBufferHead - locTail);
            }
            else
            {
                size = (uWIFI_txBufferHead + (uWIFI_TX_BUFFER_SIZE - locTail));
            }
        }
        #else
        {
            size = uWIFI_GET_TX_FIFO_ENTRIES;
        }
        #endif

        return(size);
    }


    /*******************************************************************************
    * Function Name: uWIFI_SpiUartClearTxBuffer
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
    void uWIFI_SpiUartClearTxBuffer(void)
    {
        #if(uWIFI_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 intSourceMask;
        #endif /* (uWIFI_INTERNAL_TX_SW_BUFFER_CONST) */

        #if(uWIFI_CHECK_TX_SW_BUFFER)
        {
            intSourceMask = uWIFI_SpiUartDisableIntTx();

            uWIFI_CLEAR_TX_FIFO;

            /* Flush TX software buffer */
            uWIFI_txBufferHead = uWIFI_txBufferTail;

            /* End TX transfer if it is in progress */
            intSourceMask &= (uint32) ~uWIFI_INTR_TX_NOT_FULL;

            uWIFI_SpiUartEnableIntTx(intSourceMask);
        }
        #else
        {
            uWIFI_CLEAR_TX_FIFO;
        }
        #endif
    }

#endif /* (uWIFI_TX_DIRECTION) */


/*******************************************************************************
* Function Name: uWIFI_SpiUartDisableIntRx
********************************************************************************
*
* Summary:
*  Disables the RX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns the RX interrupt sources enabled before the function call.
*
*******************************************************************************/
uint32 uWIFI_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = uWIFI_GetRxInterruptMode();

    uWIFI_SetRxInterruptMode(uWIFI_NO_INTR_SOURCES);

    return(intSource);
}


/*******************************************************************************
* Function Name: uWIFI_SpiUartDisableIntTx
********************************************************************************
*
* Summary:
*  Disables TX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns TX interrupt sources enabled before function call.
*
*******************************************************************************/
uint32 uWIFI_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = uWIFI_GetTxInterruptMode();

    uWIFI_SetTxInterruptMode(uWIFI_NO_INTR_SOURCES);

    return(intSourceMask);
}


#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: uWIFI_PutWordInRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores a byte/word into the RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  index:      index to store data byte/word in the RX buffer.
    *  rxDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uWIFI_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in buffer */
        if(uWIFI_ONE_BYTE_WIDTH == uWIFI_rxDataBits)
        {
            uWIFI_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            uWIFI_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            uWIFI_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: uWIFI_GetWordFromRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 uWIFI_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if(uWIFI_ONE_BYTE_WIDTH == uWIFI_rxDataBits)
        {
            value = uWIFI_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) uWIFI_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)uWIFI_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return(value);
    }


    /*******************************************************************************
    * Function Name: uWIFI_PutWordInTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores byte/word into the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  idx:        index to store data byte/word in the TX buffer.
    *  txDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uWIFI_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in buffer */
        if(uWIFI_ONE_BYTE_WIDTH == uWIFI_txDataBits)
        {
            uWIFI_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            uWIFI_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            uWIFI_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: uWIFI_GetWordFromTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  idx: index to get data byte/word from the TX buffer.
    *
    * Return:
    *  Returns byte/word read from the TX buffer.
    *
    *******************************************************************************/
    uint32 uWIFI_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if(uWIFI_ONE_BYTE_WIDTH == uWIFI_txDataBits)
        {
            value = (uint32) uWIFI_txBuffer[idx];
        }
        else
        {
            value  = (uint32) uWIFI_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) uWIFI_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return(value);
    }

#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
