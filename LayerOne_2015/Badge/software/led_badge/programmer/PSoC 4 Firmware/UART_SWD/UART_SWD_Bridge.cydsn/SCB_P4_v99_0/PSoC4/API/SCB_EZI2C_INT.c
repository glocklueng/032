/*******************************************************************************
* File Name: `$INSTANCE_NAME`_EZI2C_INT.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the Interrupt Servive Routine for
*  the SCB Component in EZI2C mode.
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
#include "`$INSTANCE_NAME`_EZI2C_PVT.h"

#if(`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EZI2C_STRETCH_ISR
    ********************************************************************************
    *
    * Summary:
    *  Handles Interrupt Service Routine for SCB EZI2C mode. The clock stretching is
    *  used while operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    CY_ISR_PROTO(`$INSTANCE_NAME`_EZI2C_STRETCH_ISR)
    {
        static uint16 locBufSize;
        uint32 locIndex;
        uint32 locStatus;

        uint32 endTransfer;
        uint32 fifoIndex;
        uint32 theByte;

    #if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST)
        /* Variable intended to be used with either buffer */
        static volatile uint8 * `$INSTANCE_NAME`_dataBuffer; /* Pointer to data buffer              */
        static uint16 `$INSTANCE_NAME`_bufSizeBuf;           /* Size of buffer1 in bytes            */
        static uint16 `$INSTANCE_NAME`_protectBuf;           /* Start index of write protected area */

        static uint8 activeAddress;
        uint32 ackResponse;

        ackResponse = `$INSTANCE_NAME`_EZI2C_ACK_RECEIVED_ADDRESS;
    #endif /* (`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST) */

        /* Calls registered customer routine to manage interrupt sources */
        if(NULL != `$INSTANCE_NAME`_customIntrHandler)
        {
            `$INSTANCE_NAME`_customIntrHandler();
        }

        /* Make copy of global variable */
        locIndex = `$INSTANCE_NAME`_EZI2C_GET_INDEX(activeAddress);

        /* INTR_I2C_EC_WAKE_UP */
        /* Wakes up device from deep sleep */
        if(`$INSTANCE_NAME`_CHECK_INTR_I2C_EC_MASKED(`$INSTANCE_NAME`_INTR_I2C_EC_WAKE_UP))
        {
            /* Disable wakeup interrupt source but do not clear it. Do this in INTR_SLAVE_I2C_ADDR_MATCH */
            `$INSTANCE_NAME`_SetI2CExtClkInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
        }

        /* INTR_SLAVE_I2C_BUS_ERROR or EZI2C_INTR_SLAVE_I2C_ARB_LOST */
        /* Misplaced Start or Stop condition was occurred on the bus: sets flag and interrupt sources
        * to complete on-going transaction.
        */
        if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_BUS_ERROR |
                                                    `$INSTANCE_NAME`_INTR_SLAVE_I2C_ARB_LOST))
        {
            locStatus = `$INSTANCE_NAME`_EZI2C_STATUS_ERR;

            if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ARB_LOST))
            {
                /* INTR_SLAVE_I2C_ARB_LOST: slave drives the SDA line.
                * The ARB_LOST never sets any completion interrupt sources: I2C_STOP, I2C_NACK or I2C_WRITE_STOP.
                * Case 1: master writes slave drives NACK.
                * Case 2: master reads data byte (slave drives). The TX_EMPTY is enabled in this case.
                * Case 3: slave drives NACK to address. Nothing set, does not care.
                */
                if(0u == (`$INSTANCE_NAME`_GetTxInterruptMode() & `$INSTANCE_NAME`_INTR_TX_EMPTY))
                {
                    /* Case 1: set write completion interrupt sources */
                    `$INSTANCE_NAME`_SetSlaveInterrupt(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP |
                                                       `$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP);
                }
                else
                {
                    /* Case 2: set read completion interrupt sources */
                    `$INSTANCE_NAME`_SetSlaveInterrupt(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP |
                                                       `$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK);
                }
            }
            else
            { 
                /* INTR_SLAVE_I2C_BUS_ERROR: master drives the SDA line.
                * The BUS_ERROR always sets completion interrupt I2C_STOP and optionally I2C_WRITE_STOP.
                * Case 1: master end read and drives NACK;
                * Case 2: master writes data byte;
                * Case 3: master sends address byte.
                */
                if(0u != ((uint8) `$INSTANCE_NAME`_EZI2C_STATUS_BUSY & `$INSTANCE_NAME`_curStatus))
                {
                    if(!`$INSTANCE_NAME`_CHECK_INTR_SLAVE(`$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP))
                    {
                        /* Case1: set read completion interrupt source. I2C_STOP is already set by HW */
                        `$INSTANCE_NAME`_SetSlaveInterrupt(`$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK);
                    }
                    else
                    {
                        /* Case2: write completion interrupt sources are set by HW */
                    }
                }
                else
                {
                    /* Case3: Clear all completion interrupt sources */
                    `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP |
                                                               `$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP);

                     locStatus = 0u; /* Do not report error */
                }
            }

            /* Set error status */
            `$INSTANCE_NAME`_curStatus |= (uint8) locStatus;

            /* Following interrupt sources cleans the slave:
            * INTR_RX_NOT_EMPTY: reads date from RX FIFO that were received before error.
            * INTR_SLAVE_I2C_STOP: completes current transaction and prepares to following.
            * Note: the buffer may not be modified in case of error.
            */

            `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_BUS_ERROR |
                                                       `$INSTANCE_NAME`_INTR_SLAVE_I2C_ARB_LOST);
        }

        /* INTR_RX_NOT_EMPTY */
        /* Receives data byte-by-byte. Do not use RX FIFO capabilities */
        if(`$INSTANCE_NAME`_CHECK_INTR_RX_MASKED(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY))
        {
            theByte = `$INSTANCE_NAME`_RX_FIFO_RD_REG;

            switch(`$INSTANCE_NAME`_fsmState)
            {
            case `$INSTANCE_NAME`_EZI2C_FSM_BYTE_WRITE:
                /* Check receive buffer size */
                if(0u != locBufSize)
                {
                    /* Store and ACK the byte */
                    `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_ACK;

                    `$INSTANCE_NAME`_dataBuffer`$BufNum`[locIndex] = (uint8) theByte;
                    locIndex++;
                    locBufSize--;
                }
                else
                {
                    /* Discard the data byte and NACK */
                    `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_NACK;
                }

                break;

        #if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST)
            case `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_HI8:

                `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_ACK;

                /* Store high byte of the offset */
                locBufSize = (uint16) ((uint8) theByte);
                                
                `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8;

                break;
        #endif /* (`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST) */

            case `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8:

                `$Cond`if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16)
                {
                    /* Append the offset with high byte */
                    theByte = ((uint32) ((uint32) locBufSize << 8u)) | theByte;
                }
                `$EndCond`

                /* Check offset against buffer size */
                if(theByte < (uint32) `$INSTANCE_NAME`_bufSizeBuf`$BufNum`)
                {
                    `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_ACK;

                    /* Set new buffer index equal offset */
                    locIndex = theByte;

                    /* Number of entries in buffer */
                    theByte = (uint32) `$INSTANCE_NAME`_bufSizeBuf`$BufNum` - theByte;

                    /* Exclude protected area */
                    fifoIndex = (uint32) `$INSTANCE_NAME`_bufSizeBuf`$BufNum` - (uint32) `$INSTANCE_NAME`_protectBuf`$BufNum`;

                    /* Get buffer size to write into */
                    locBufSize = (theByte > fifoIndex) ? ((uint16) (theByte - fifoIndex)) : (0u);

                    
                    if(locBufSize < `$INSTANCE_NAME`_FIFO_SIZE)
                    {
                        /* Set FSM state to receive byte-by-byte */
                        `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_BYTE_WRITE;
                    }
                    /* Receive by RX FIFO chunks */
                    else if(locBufSize == `$INSTANCE_NAME`_FIFO_SIZE)
                    {
                        `$INSTANCE_NAME`_ENABLE_SLAVE_AUTO_DATA; /* NACK when RX FIFO is full */
                        `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
                    }
                    else
                    {
                        `$INSTANCE_NAME`_ENABLE_SLAVE_AUTO_DATA_ACK; /* Stretch when RX FIFO is full */
                        `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_INTR_RX_FULL);
                    }

                    /* Store local offset into global variable */
                    `$INSTANCE_NAME`_EZI2C_SET_OFFSET(activeAddress, locIndex);
                }
                else
                {
                    /* Discard the offset byte and NACK */
                    `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_NACK;
                }

                break;

            default:
                CYASSERT(0u != 0u); /* Should never get there */
                break;
            }

            `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY);
        }
        /* INTR_RX_NOT_EMPTY disabled */
        /* The INTR_RX_FULL or INTR_SLAVE_I2C_STOP generates event to get data from RX FIFO */
        else if(`$INSTANCE_NAME`_CHECK_INTR_RX_MASKED(`$INSTANCE_NAME`_INTR_RX_FULL) ||
                `$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP))
        {
            /* INTR_SLAVE_I2C_STOP is the source - end of transfer */
            if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP))
            {
                /* Transfer complete: read remaining bytes from RX FIFO */
                fifoIndex = `$INSTANCE_NAME`_GET_RX_FIFO_ENTRIES;

                `$Cond`if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE)
                {
                    if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH))
                    {
                        /* Update RX_FIFO entries as address was received */
                        fifoIndex = `$INSTANCE_NAME`_GET_RX_FIFO_ENTRIES;
                        
                        /* If SR is valid RX FIFO is full and address is in the SHIFTER: read 8 entries to put addess 
                        * in the RX FIFO. If SR is invalid the address is already in the RX FIFO: read (entries-1).
                        */
                        fifoIndex -= ((`$INSTANCE_NAME`_GET_RX_FIFO_SR_VALID == 0u) ? (1u) : (0u));
                    }
                }
                `$EndCond`

                `$INSTANCE_NAME`_DISABLE_SLAVE_AUTO_DATA;
                endTransfer = `$INSTANCE_NAME`_EZI2C_CONTINUE_TRANSFER;
            }
            /* INTR_RX_FULL is the source - continue or complete on I2C_STOP */
            else
            {
                /* Calculate buffer size available to write data into : the wrap-around is not possible 
                * as buffer pre-calculated while offset is set.
                */
                locBufSize -= (uint16) `$INSTANCE_NAME`_FIFO_SIZE;

                if(locBufSize < `$INSTANCE_NAME`_FIFO_SIZE)
                {
                    /* Send NACK when RX FIFO overflow */
                    fifoIndex   = locBufSize;
                    endTransfer = `$INSTANCE_NAME`_EZI2C_COMPLETE_TRANSFER;
                }
                else
                {
                    /* Continue  transaction */
                    fifoIndex   = `$INSTANCE_NAME`_FIFO_SIZE;
                    endTransfer = `$INSTANCE_NAME`_EZI2C_CONTINUE_TRANSFER;
                }
            }

            for(;(0u != fifoIndex);fifoIndex--)
            {
                /* Store data in the buffer */
                `$INSTANCE_NAME`_dataBuffer`$BufNum`[locIndex] = (uint8) `$INSTANCE_NAME`_RX_FIFO_RD_REG;
                locIndex++;
            }

            /* Complete transfer sending NACK when RX FIFO overflow */
            if(`$INSTANCE_NAME`_EZI2C_COMPLETE_TRANSFER == endTransfer)
            {
                `$INSTANCE_NAME`_ENABLE_SLAVE_AUTO_DATA_NACK;

                /* The INTR_RX_FULL triggers earlier then INTR_SLAVE_I2C_STOP: disable all RX interrupt sources */
                `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
            }

            `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_FULL | 
                                                    `$INSTANCE_NAME`_INTR_RX_NOT_EMPTY);
        }
        else
        {
            ; /* None of interrupt sourcse is active: exit */
        }
        
        /* INTR_SLAVE_I2C_STOP */
        /* Catches Stop condition: completion of write or read transfer.
        * The INTR_SLAVE_I2C_STOP and INTR_RX_FULL are separeted to not cause completion hadnling if
        * there is data in RX FIFO.
        */
        if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP))
        {
            /* Disable RX interrupt sources */
            `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);

            /* Checks if buffer content was modified: the address phase resets the locIndex */
            if(locIndex == `$INSTANCE_NAME`_EZI2C_GET_OFFSET(activeAddress))
            {
                `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP);
            }

            /* Clear TX FIFO and disable TX interrupt sources */
            `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
            `$INSTANCE_NAME`_CLEAR_TX_FIFO;

            /* Resets FSM to offset receive state */
            `$Cond`if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16)
            {
                `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_HI8;
            }
            `$Cond`else
            {
                `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8;
            }
            `$EndCond`

            /* Complete read or write transcation */
            locStatus  = (uint32) `$INSTANCE_NAME`_curStatus;
            locStatus |= (`$INSTANCE_NAME`_INTR_SLAVE_REG & `$INSTANCE_NAME`_EZI2C_CMPLT_INTR_MASK);
            `$INSTANCE_NAME`_EZI2C_UPDATE_LOC_STATUS(activeAddress, locStatus);
            locStatus &= ~`$INSTANCE_NAME`_EZI2C_STATUS_BUSY;
            `$INSTANCE_NAME`_curStatus = (uint8) locStatus;

            /* Clean-up sources */
            `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP       |
                                                       `$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP |
                                                       `$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK);
        }
        /* INTR_SLAVE_I2C_ADDR_MATCH */
        /* The match address is received: slave starts its operations.
        * The completion INTR_SLAVE_I2C_STOP and slave start INTR_SLAVE_I2C_ADDR_MATCH events are separated
        * because two addresses mode requires switching between addresses and make sure that the 1st is
        * serviced earlier than the 2nd.
        */
        else if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH))
        {
            `$Cond`if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE)
            {
                /* Read address byte from the RX FIFO */
                theByte = `$INSTANCE_NAME`_GET_I2C_7BIT_ADDRESS(`$INSTANCE_NAME`_RX_FIFO_RD_REG);

                `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY);

                /* Check received address against device addresses */
                if(`$INSTANCE_NAME`_addrBuf1 == theByte)
                {
                    /* Set buffer exposed as the primary slave address */
                    `$INSTANCE_NAME`_dataBuffer = `$INSTANCE_NAME`_dataBuffer1;
                    `$INSTANCE_NAME`_bufSizeBuf = `$INSTANCE_NAME`_bufSizeBuf1;
                    `$INSTANCE_NAME`_protectBuf = `$INSTANCE_NAME`_protectBuf1;

                    activeAddress = `$INSTANCE_NAME`_EZI2C_ACTIVE_ADDRESS1;
                }
                else if(`$INSTANCE_NAME`_addrBuf2 == theByte)
                {
                    /* Set buffer exposed as the secondary slave address */
                    `$INSTANCE_NAME`_dataBuffer = `$INSTANCE_NAME`_dataBuffer2;
                    `$INSTANCE_NAME`_bufSizeBuf = `$INSTANCE_NAME`_bufSizeBuf2;
                    `$INSTANCE_NAME`_protectBuf = `$INSTANCE_NAME`_protectBuf2;

                    activeAddress = `$INSTANCE_NAME`_EZI2C_ACTIVE_ADDRESS2;
                }
                else
                {
                    /* The address does not match */
                    ackResponse = `$INSTANCE_NAME`_EZI2C_NACK_RECEIVED_ADDRESS;
                }
            }
            `$EndCond`

        #if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST)
            if(`$INSTANCE_NAME`_EZI2C_NACK_RECEIVED_ADDRESS == ackResponse)
            {
                /* Clear interrupt sources before NACK address */
                `$INSTANCE_NAME`_ClearI2CExtClkInterruptSource(`$INSTANCE_NAME`_INTR_I2C_EC_WAKE_UP);
                `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_ALL);

                /* NACK address byte: it does not match niether primary nor secondary */
                `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_NACK;
            }
            else
        #endif /* (`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST) */
            {
            
            #if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
                if(!`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE)
                {
                    /* Set buffer exposed as the primary slave address */
                    `$INSTANCE_NAME`_dataBuffer = `$INSTANCE_NAME`_dataBuffer1;
                    `$INSTANCE_NAME`_bufSizeBuf = `$INSTANCE_NAME`_bufSizeBuf1;
                    `$INSTANCE_NAME`_protectBuf = `$INSTANCE_NAME`_protectBuf1;

                    activeAddress = `$INSTANCE_NAME`_EZI2C_ACTIVE_ADDRESS1;
                }
            #endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
                
                /* Bus becomes busy after address is received */
                `$INSTANCE_NAME`_curStatus |= (uint8) `$INSTANCE_NAME`_EZI2C_STATUS_BUSY;
            
                /* Slave is read or writen: set current offset and calculate buffer size */
                locIndex = `$INSTANCE_NAME`_EZI2C_GET_OFFSET(activeAddress);
                
                /* Checks transaction direction */
                if(`$INSTANCE_NAME`_CHECK_I2C_STATUS(`$INSTANCE_NAME`_I2C_STATUS_S_READ))
                {
                    /* Calculate slave buffer size */
                    locBufSize = `$INSTANCE_NAME`_bufSizeBuf`$BufNum` - (uint16) locIndex;

                    `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_INTR_TX_EMPTY);
                }
                else
                {
                    /* Master writes: enable receiption interrupt. The FSM state was set in INTR_SLAVE_I2C_STOP */
                    `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY);
                }
                
                /* Clear interrupt sources before ACK address */
                `$INSTANCE_NAME`_ClearI2CExtClkInterruptSource(`$INSTANCE_NAME`_INTR_I2C_EC_WAKE_UP);
                `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_ALL);

                /* ACKs the address byte */
                `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_ACK;
            }
        }
        else
        {
            ; /* None of interrupt sourcse is active: exit */
        }


        /* `$INSTANCE_NAME`_INTR_TX_EMPTY */
        /* Transmits data to master: loads data into TX FIFO when last element for TX FIFO moved into SHIFTER
        *  If there is data to transmit the 0xFF send out.
        *  After any transfer completion the TX FIFO is prefilled with data from current buffer offset. In
        *  action inteneded to reduce probability of clock stretching if TX FIFO and SHIRTER are empty.
        */
        if(`$INSTANCE_NAME`_CHECK_INTR_TX_MASKED(`$INSTANCE_NAME`_INTR_TX_EMPTY))
        {
            /* Put data into the TX FIFO until there is a place */
            while(`$INSTANCE_NAME`_FIFO_SIZE != `$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES)
            {
                /* Check transmit buffer range: locBufSize calculates after address receiption */
                if(0u != locBufSize)
                {
                    /* Put data from the buffer to be read by master */
                    `$INSTANCE_NAME`_TX_FIFO_WR_REG = (uint32) `$INSTANCE_NAME`_dataBuffer`$BufNum`[locIndex];
                    locIndex++;
                    locBufSize--;
                }
                else
                {
                    /* Put 0xFF byte to be read by master */
                    `$INSTANCE_NAME`_TX_FIFO_WR_REG = `$INSTANCE_NAME`_EZI2C_OVFL_RETURN;
                }
            }

            `$INSTANCE_NAME`_ClearTxInterruptSource(`$INSTANCE_NAME`_INTR_TX_EMPTY);
        }

        /* Store local index copy into global variable */
        `$INSTANCE_NAME`_EZI2C_SET_INDEX(activeAddress, locIndex);
    }
#endif /* (`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST) */


#if(`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_DISABLE_CONST)
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EZI2C_NO_STRETCH_ISR
    ********************************************************************************
    *
    * Summary:
    *  Handles Interrupt Service Routine for SCB EZI2C mode. The clock stretching is
    *  NOT used while operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    CY_ISR_PROTO(`$INSTANCE_NAME`_EZI2C_NO_STRETCH_ISR)
    {
    #if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST)
        static uint8 locOffset;
    #endif /* (`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST) */

        uint32 theByte;
        uint32 locStatus;

        /* Calls registered customer routine to manage interrupt sources */
        if(NULL != `$INSTANCE_NAME`_customIntrHandler)
        {
            `$INSTANCE_NAME`_customIntrHandler();
        }


        /* INTR_SLAVE_I2C_BUS_ERROR or EZI2C_INTR_SLAVE_I2C_ARB_LOST */
        /* Misplaced Start or Stop condition was occurred on the bus: sets flag and interrupt sources
        * to complete on-going transaction.
        */
        if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_BUS_ERROR |
                                                    `$INSTANCE_NAME`_INTR_SLAVE_I2C_ARB_LOST))
        {
            locStatus = `$INSTANCE_NAME`_EZI2C_STATUS_ERR;

            if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ARB_LOST))
            {
                /* INTR_SLAVE_I2C_ARB_LOST: slave drives the SDA line.
                * The ARB_LOST never sets any completion interrupt sources: I2C_STOP, I2C_NACK or I2C_WRITE_STOP.
                * Case 1: master writes and overflows RX FIFO and slave drives NACK;
                * Case 2: master reads data byte (slave drives);
                * Case 3: slave drives NACK to address. Nothing set, does not care.
                */
                if(`$INSTANCE_NAME`_CHECK_INTR_RX(`$INSTANCE_NAME`_INTR_RX_OVERFLOW))
                {
                    /* Case 1: set write completion interrupt sources */
                    `$INSTANCE_NAME`_SetSlaveInterrupt(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP |
                                                       `$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP);
                }
                else
                {
                    /* Case 2: set read completion interrupt sources */
                    `$INSTANCE_NAME`_SetSlaveInterrupt(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP |
                                                       `$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK);
                }
            }
            else
            {
                /* INTR_SLAVE_I2C_BUS_ERROR: master drives the SDA line.
                * The BUS_ERROR always sets completion interrupt I2C_STOP and optionally I2C_WRITE_STOP.
                * Case 1: master end read and drives NACK;
                * Case 2: master writes data byte;
                * Case 3: master sends address byte.
                */
                if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH))
                {
                    if(!`$INSTANCE_NAME`_CHECK_INTR_SLAVE(`$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP))
                    {
                        /* Case1: set read completion interrupt source. I2C_STOP is already set by HW */
                        `$INSTANCE_NAME`_SetSlaveInterrupt(`$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK);
                    }
                    else
                    {
                        /* Case2: write completion interrupt sources are set by HW */
                    }
                }
                else
                {
                    /* Case3: Clear all completion interrupt sources */
                    `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP |
                                                               `$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP);

                     locStatus = 0u; /* Do not report error */
                }
            }

            /* Set error status */
            `$INSTANCE_NAME`_curStatus |= (uint8) locStatus;

            /* Following interrupt sources cleans the slave:
            * INTR_RX_NOT_EMPTY: reads date from RX FIFO that were received before error.
            * INTR_SLAVE_I2C_STOP: completes current transaction and prepares to following.
            * Note: the buffer may not be modified in case of error.
            */

            `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_BUS_ERROR |
                                                       `$INSTANCE_NAME`_INTR_SLAVE_I2C_ARB_LOST);
        }


        /* INTR_RX_NOT_EMPTY */
        /* Receives data from master: data resides into RX FIFO. At least entry in RX FIFO cause RX FIFO read.
        * The the offset is written first and all following bytes are data expected to be put in the buffer.
        * Slave ACKs all bytes, but it discards them if they do not match write criteria.
        * Slave NACKs the bytes in case RX FIFO overflow.
        */
        if(`$INSTANCE_NAME`_CHECK_INTR_RX_MASKED(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY))
        {
            /* Read all entries available in RX FIFO */
            while(0u != `$INSTANCE_NAME`_GET_RX_FIFO_ENTRIES)
            {
                theByte = `$INSTANCE_NAME`_RX_FIFO_RD_REG;

                switch(`$INSTANCE_NAME`_fsmState)
                {

                case `$INSTANCE_NAME`_EZI2C_FSM_BYTE_WRITE:
                    /* Checks received buffer index agaist protect arrea */
                    if(`$INSTANCE_NAME`_indexBuf1 < `$INSTANCE_NAME`_protectBuf1)
                    {
                        /* Stores recived byte into the buffer */
                        `$INSTANCE_NAME`_dataBuffer1[`$INSTANCE_NAME`_indexBuf1] = (uint8) theByte;
                        `$INSTANCE_NAME`_indexBuf1++;
                    }
                    else
                    {
                        /* Discards current byte and sets FSM state to default to discard following bytes */
                        `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_WAIT_STOP;
                    }

                    break;

            #if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST)
                case `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_HI8:

                    /* Store high byte of the offset */
                    locOffset = (uint8) theByte;
                    
                    /* Reset index to offset */
                    `$INSTANCE_NAME`_indexBuf1 = `$INSTANCE_NAME`_offsetBuf1;
                    
                    `$INSTANCE_NAME`_fsmState  = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8;

                    break;
            #endif /* (`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST) */

                case `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8:

                    `$Cond`if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16)
                    {
                        /* Append the offset with high byte */
                        theByte = ((uint32) ((uint32) locOffset << 8u)) | theByte;
                    }
                    `$EndCond`

                    /* Checks if offset within buffer range */
                    if(theByte < (uint32) `$INSTANCE_NAME`_bufSizeBuf1)
                    {
                        /* Stores and sets received offset */
                        `$INSTANCE_NAME`_offsetBuf1 = (`$EzI2cBitWidthReplacementString`) theByte;
                        `$INSTANCE_NAME`_indexBuf1  = (`$EzI2cBitWidthReplacementString`) theByte;

                        /* Move FSM to data receive state */
                        `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_BYTE_WRITE;
                    }
                    else
                    {
                        /* Reset index to offset */
                        `$INSTANCE_NAME`_indexBuf1 = `$INSTANCE_NAME`_offsetBuf1;
                         
                        /* Discards current byte and sets FSM state to default to discard following bytes */
                        `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_WAIT_STOP;
                    }

                    break;

                case `$INSTANCE_NAME`_EZI2C_FSM_WAIT_STOP:
                    /* Clears RX FIFO to discard all received data */
                    `$INSTANCE_NAME`_CLEAR_RX_FIFO;

                    break;

                default:
                    CYASSERT(0u != 0u); /* Should never get there */
                    break;
                }
            }

            `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY);
        }
        else 
        {
            /* INTR_SLAVE_I2C_STOP */
            /* Catches completion of write or read transfer. Separate INTR_RX_NOT_EMPTY and INTR_SLAVE_I2C_STOP to
            * make sure that the 1st is serviced earlier than the 2nd.
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP))
            {
                /* Get default FSM state */
                `$Cond`if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16)
                {
                    theByte = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_HI8;
                }
                `$Cond`else
                {
                    theByte = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8;
                }
                `$EndCond`
                
                if(theByte == `$INSTANCE_NAME`_fsmState)
                {
                    /* Reset index: slave reading state or incompelete set offset state */
                    `$INSTANCE_NAME`_indexBuf1 = `$INSTANCE_NAME`_offsetBuf1;
                    `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP);
                }
                else
                {
                    /* Slave writing state: check buffer content modification */
                    if(`$INSTANCE_NAME`_indexBuf1 == `$INSTANCE_NAME`_offsetBuf1)
                    {
                        /* New offset is set without buffer modification */
                        `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP);
                    }
                    else
                    {
                        /* Reset index: new offset is set and write data complete with buffer modification */
                        `$INSTANCE_NAME`_indexBuf1 = `$INSTANCE_NAME`_offsetBuf1;
                    }
                }
                
                /* Reset FSM defaults state */
                `$INSTANCE_NAME`_fsmState = theByte;

                /* Clears TX FIFO: it will be loaded on the Start/ReStart reseption */
                `$INSTANCE_NAME`_CLEAR_TX_FIFO;
                `$INSTANCE_NAME`_INTR_TX_MASK_REG = `$INSTANCE_NAME`_NO_INTR_SOURCES;
                
                
                /* Set complettion flags read or write */
                `$INSTANCE_NAME`_curStatus |= (uint8) (`$INSTANCE_NAME`_INTR_SLAVE_REG & `$INSTANCE_NAME`_EZI2C_CMPLT_INTR_MASK);

                /* Also clears address match which takes part of error tracking */
                `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_COMPLETE);

                /* Clears RX overflow that takes part in error tracking */
                `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_OVERFLOW);
            }
            
            
            /* INTR_SLAVE_I2C_START */
            /* Catches start of transfer to trigger TX FIFO update event */
            if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_START))
            {
                /* Enable interrupt source to update TX FIFO */
                `$INSTANCE_NAME`_INTR_TX_MASK_REG = `$INSTANCE_NAME`_INTR_TX_NOT_FULL;
                
                `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_START);
            }
        }


        /* `$INSTANCE_NAME`_INTR_TX_NOT_FULL */
        /* Transmits data to master: loads data into TX FIFO. At least one empty entry in TX FIFO cause load to TX FIFO.
        * If there is data to transmit the 0xFF send out.
        * After any transfer completion the TX FIFO is prefilled with data from current buffer offset. In
        * action inteneded to reduce probability of clock stretching if TX FIFO and SHIRTER are empty.
        */
        if(`$INSTANCE_NAME`_CHECK_INTR_TX_MASKED(`$INSTANCE_NAME`_INTR_TX_NOT_FULL))
        {
            /* Put data into the TX FIFO until there is a place */
            while(`$INSTANCE_NAME`_FIFO_SIZE != `$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES)
            {
                /* Check transmit buffer range */
                if(`$INSTANCE_NAME`_indexBuf1 < `$INSTANCE_NAME`_bufSizeBuf1)
                {
                    /* Put data from the buffer to be read by master */
                    `$INSTANCE_NAME`_TX_FIFO_WR_REG = (uint32) `$INSTANCE_NAME`_dataBuffer1[`$INSTANCE_NAME`_indexBuf1];
                    `$INSTANCE_NAME`_indexBuf1++;
                }
                else
                {
                    /* Put 0xFF byte to be read by master */
                    `$INSTANCE_NAME`_TX_FIFO_WR_REG = `$INSTANCE_NAME`_EZI2C_OVFL_RETURN;
                }
            }

            `$INSTANCE_NAME`_ClearTxInterruptSource(`$INSTANCE_NAME`_INTR_TX_NOT_FULL);
        }
    }
#endif /* (`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_DISABLE_CONST) */


/* [] END OF FILE */
