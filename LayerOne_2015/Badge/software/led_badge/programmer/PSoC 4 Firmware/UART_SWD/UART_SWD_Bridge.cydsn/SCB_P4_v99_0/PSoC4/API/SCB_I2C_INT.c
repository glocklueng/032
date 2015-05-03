/*******************************************************************************
* File Name: `$INSTANCE_NAME`_I2C_INT.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the Interrupt Servive Routine for
*  the SCB Component in I2C mode.
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
#include "`$INSTANCE_NAME`_I2C_PVT.h"


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2C_ISR
********************************************************************************
*
* Summary:
*  Handles Interrupt Service Routine for SCB I2C mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_I2C_ISR)
{
    uint32 diffCount;
    uint32 endTransfer;
    uint8 enableInterrupts;

    #if(`$INSTANCE_NAME`_CHECK_I2C_ACCEPT_ADDRESS_CONST)
        uint32 address;
    #endif /* (`$INSTANCE_NAME`_CHECK_I2C_ACCEPT_ADDRESS_CONST) */

    endTransfer = 0u; /* Continue active transfer */

    /* Call customer routine if registered */
    if(NULL != `$INSTANCE_NAME`_customIntrHandler)
    {
        `$INSTANCE_NAME`_customIntrHandler();
    }

    if(`$INSTANCE_NAME`_CHECK_INTR_I2C_EC_MASKED(`$INSTANCE_NAME`_INTR_I2C_EC_WAKE_UP))
    {
        /* Mask-off after wakeup */
        `$INSTANCE_NAME`_SetI2CExtClkInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
    }

    /* Master and Slave error tracking:
    * Add master state check to track only master errors when master is active or track
    * slave errors when slave active or idle.
    * Specil MMS case: on address phase with misplaced Start: master sets LOST_ARB and
    * slave BUS_ERR. The valid event is LOST_ARB from master.
    */
    if(`$INSTANCE_NAME`_CHECK_I2C_FSM_MASTER)
    {
        `$Cond`if(`$INSTANCE_NAME`_I2C_MASTER)
        {
            /* INTR_MASTER_I2C_BUS_ERROR:
            * Misplaced Start or Stop condition was occurred on the bus: complete transaction.
            * The interrupt is cleared in the I2C_FSM_EXIT_IDLE.
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_MASTER_MASKED(`$INSTANCE_NAME`_INTR_MASTER_I2C_BUS_ERROR))
            {
                `$INSTANCE_NAME`_mstrStatus |= (uint16) (`$INSTANCE_NAME`_I2C_MSTAT_ERR_XFER |
                                                         `$INSTANCE_NAME`_I2C_MSTAT_ERR_BUS_ERROR);

                endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER;
            }

            /* INTR_MASTER_I2C_ARB_LOST:
            * MultiMaster lost arbitrage while transaction: complete transaction.
            * Misplaced Start or Stop condition treats as lost arbitration when master drives SDA.
            * The interrupt is cleared in the I2C_FSM_EXIT_IDLE.
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_MASTER_MASKED(`$INSTANCE_NAME`_INTR_MASTER_I2C_ARB_LOST))
            {
                `$INSTANCE_NAME`_mstrStatus |= (uint16) (`$INSTANCE_NAME`_I2C_MSTAT_ERR_XFER |
                                                         `$INSTANCE_NAME`_I2C_MSTAT_ERR_ARB_LOST);

                endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER;
            }

            `$Cond`if(`$INSTANCE_NAME`_I2C_MULTI_MASTER_SLAVE)
            {
                /* I2C_MASTER_CMD_M_START_ON_IDLE:
                * MultiMaster-Slave does not generate start, because Slave was addressed
                * earlier: pass control the slave FSM.
                */
                if(`$INSTANCE_NAME`_CHECK_I2C_MASTER_CMD(`$INSTANCE_NAME`_I2C_MASTER_CMD_M_START_ON_IDLE))
                {
                    `$INSTANCE_NAME`_mstrStatus |= (uint16) (`$INSTANCE_NAME`_I2C_MSTAT_ERR_XFER |
                                                             `$INSTANCE_NAME`_I2C_MSTAT_ERR_ABORT_XFER);

                    endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER;
                }
            }
            `$EndCond`

            /* Error handling common part:
            * Set completion flag of master transcation and pass control to:
            *  - I2C_FSM_EXIT_IDLE - to complete transcation in case of: ARB_LOST or BUS_ERR.
            *  - I2C_FSM_IDLE      - to take chanse for slave to process incomming transcation.
            */
            if(0u != endTransfer)
            {
                /* Set completion before FSM change */
                `$INSTANCE_NAME`_mstrStatus |= (uint16) `$INSTANCE_NAME`_GET_I2C_MSTAT_CMPLT;

                `$Cond`if(`$INSTANCE_NAME`_I2C_MULTI_MASTER_SLAVE)
                {
                    if(`$INSTANCE_NAME`_CHECK_I2C_FSM_ADDR)
                    {
                        /* The Start generation was set after enother master start accessing the Slave.
                        * Clean-up the master and turn to slave. Set state to IDLE.
                        */
                        if(`$INSTANCE_NAME`_CHECK_I2C_MASTER_CMD(`$INSTANCE_NAME`_I2C_MASTER_CMD_M_START_ON_IDLE))
                        {
                            `$INSTANCE_NAME`_I2C_MASTER_CLEAR_START;

                            endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER; /* Pass control to Slave */
                        }
                        /* The valid arbitration lost on address phase happens only when: master LOST_ARB set and
                        * slave BUS_ERR is cleared. Only in that case set state to IDLE without SCB IP re-enable.
                        */
                        else if((!`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_BUS_ERROR))
                               && `$INSTANCE_NAME`_CHECK_INTR_MASTER_MASKED(`$INSTANCE_NAME`_INTR_MASTER_I2C_ARB_LOST))
                        {
                            endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER; /* Pass control to Slave */
                        }
                        else
                        {
                            endTransfer = 0u; /* Causes I2C_FSM_EXIT_IDLE to be set below */
                        }

                        if(0u != endTransfer) /* Clean-up master to proceed with slave */
                        {
                            `$INSTANCE_NAME`_CLEAR_TX_FIFO; /* Shifter keeps address, clear it */

                            `$INSTANCE_NAME`_DISABLE_MASTER_AUTO_DATA_ACK; /* In case of reading disable autoACK */

                            /* Clean-up master interrupt sources */
                            `$INSTANCE_NAME`_ClearMasterInterruptSource(`$INSTANCE_NAME`_INTR_MASTER_ALL);

                            /* Disable data processing interrupts: they should be cleared before */
                            `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
                            `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);

                            `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_I2C_FSM_IDLE;
                        }
                        else
                        {
                            /* Set I2C_FSM_EXIT_IDLE for BUS_ERR and ARB_LOST (that is really bus error) */
                            `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_I2C_FSM_EXIT_IDLE;
                        }
                    }
                    else
                    {
                        /* Set I2C_FSM_EXIT_IDLE if any other state than address */
                        `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_I2C_FSM_EXIT_IDLE;
                    }
                }
                `$Cond`else
                {
                    /* In case of LOST*/
                    `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_I2C_FSM_EXIT_IDLE;
                }
                `$EndCond`
            }
        }
        `$EndCond`
    }
    else /* (`$INSTANCE_NAME`_CHECK_I2C_FSM_SLAVE) */
    {
        `$Cond`if(`$INSTANCE_NAME`_I2C_SLAVE)
        {
            /* INTR_SLAVE_I2C_BUS_ERROR or `$INSTANCE_NAME`_INTR_SLAVE_I2C_ARB_LOST:
            * Misplaced Start or Stop condition was occurred on the bus: set flag
            * to notify error condition.
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_BUS_ERROR |
                                                        `$INSTANCE_NAME`_INTR_SLAVE_I2C_ARB_LOST))
            {
                if(`$INSTANCE_NAME`_CHECK_I2C_FSM_RD)
                {
                    /* TX direction: master reads from slave */
                    `$INSTANCE_NAME`_slStatus &= (uint8) ~`$INSTANCE_NAME`_I2C_SSTAT_RD_BUSY;
                    `$INSTANCE_NAME`_slStatus |= (uint8) (`$INSTANCE_NAME`_I2C_SSTAT_RD_ERR |
                                                          `$INSTANCE_NAME`_I2C_SSTAT_RD_CMPLT);
                }
                else
                {
                    /* RX direction: master writes into slave */
                    `$INSTANCE_NAME`_slStatus &= (uint8) ~`$INSTANCE_NAME`_I2C_SSTAT_WR_BUSY;
                    `$INSTANCE_NAME`_slStatus |= (uint8) (`$INSTANCE_NAME`_I2C_SSTAT_WR_ERR |
                                                          `$INSTANCE_NAME`_I2C_SSTAT_WR_CMPLT);
                }

                `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_I2C_FSM_EXIT_IDLE;
            }
        }
        `$EndCond`
    }

    /* States description:
    * Any Master operation starts from: ADDR_RD/WR state as the master generates traffic on the bus.
    * Any Slave operation starts from: IDLE state as slave always waiting actions from the master.
    */

    /* FSM Master */
    if(`$INSTANCE_NAME`_CHECK_I2C_FSM_MASTER)
    {
        `$Cond`if(`$INSTANCE_NAME`_I2C_MASTER)
        {
            /* INTR_MASTER_I2C_STOP:
            * Stop condition was generated by the master: end of transction.
            * Set completion flags to notify API.
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_MASTER_MASKED(`$INSTANCE_NAME`_INTR_MASTER_I2C_STOP))
            {
                `$INSTANCE_NAME`_ClearMasterInterruptSource(`$INSTANCE_NAME`_INTR_MASTER_I2C_STOP);

                `$INSTANCE_NAME`_mstrStatus |= (uint16) `$INSTANCE_NAME`_GET_I2C_MSTAT_CMPLT;
                `$INSTANCE_NAME`_state       = `$INSTANCE_NAME`_I2C_FSM_IDLE;
            }
            else
            {
                if(`$INSTANCE_NAME`_CHECK_I2C_FSM_ADDR) /* Address stage */
                {
                    /* INTR_MASTER_I2C_NACK:
                    * Master send address but it was NACKed by the slave: complete transaction.
                    */
                    if(`$INSTANCE_NAME`_CHECK_INTR_MASTER_MASKED(`$INSTANCE_NAME`_INTR_MASTER_I2C_NACK))
                    {
                        `$INSTANCE_NAME`_ClearMasterInterruptSource(`$INSTANCE_NAME`_INTR_MASTER_I2C_NACK);

                        `$INSTANCE_NAME`_mstrStatus |= (uint16) (`$INSTANCE_NAME`_I2C_MSTAT_ERR_XFER |
                                                                 `$INSTANCE_NAME`_I2C_MSTAT_ERR_ADDR_NAK);

                        endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER;
                    }
                    /* INTR_TX_UNDERFLOW. The address byte was sent:
                    *  - TX direction: the clock is stretched after ACK phase, because TX FIFO is
                    *    EMPTY. The TX EMPTY clean all TX interrupt sources.
                    *  - RX direction: the 1st byte is receiving, but there is no ACK permision,
                    *    clock is stretched after 1 byte will be received.
                    */
                    else
                    {
                        if(`$INSTANCE_NAME`_CHECK_I2C_FSM_RD) /* Reading */
                        {
                            `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_I2C_FSM_MSTR_RD_DATA;
                        }
                        else /* Writing */
                        {
                            `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_I2C_FSM_MSTR_WR_DATA;
                            `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_INTR_TX_EMPTY);
                        }
                    }
                }

                if(`$INSTANCE_NAME`_CHECK_I2C_FSM_DATA) /* Data phase */
                {
                    if(`$INSTANCE_NAME`_CHECK_I2C_FSM_RD) /* Reading */
                    {
                        /* INTR_RX_FULL:
                        * RX direction: master received 8 bytes, the 9th byte is receiving.
                        * Get data from RX FIFO and decide whether to ACK or  NACK following bytes.
                        */
                        if(`$INSTANCE_NAME`_CHECK_INTR_RX_MASKED(`$INSTANCE_NAME`_INTR_RX_FULL))
                        {
                            /* Calculate difference */
                            diffCount =  `$INSTANCE_NAME`_mstrRdBufSize -
                                        (`$INSTANCE_NAME`_mstrRdBufIndex + `$INSTANCE_NAME`_GET_RX_FIFO_ENTRIES);

                            /* Proceed transaction or end it when RX FIFO
                             * become FULL again .
                            */
                            if(diffCount > `$INSTANCE_NAME`_FIFO_SIZE)
                            {
                                diffCount = `$INSTANCE_NAME`_FIFO_SIZE;
                            }
                            else
                            {
                                if(0u == diffCount)
                                {
                                    `$INSTANCE_NAME`_DISABLE_MASTER_AUTO_DATA_ACK;

                                    diffCount   = `$INSTANCE_NAME`_FIFO_SIZE;
                                    endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER;
                                }
                            }

                            for(;(0u != diffCount);diffCount--)
                            {
                                `$INSTANCE_NAME`_mstrRdBufPtr[`$INSTANCE_NAME`_mstrRdBufIndex] = (uint8)
                                                                                        `$INSTANCE_NAME`_RX_FIFO_RD_REG;
                                `$INSTANCE_NAME`_mstrRdBufIndex++;
                            }
                        }
                        /* INTR_RX_NOT_EMPTY:
                        * RX direction: master received one data byte need to ACK or NACK.
                        * The last byte is stored and NACKed by the master. The NACK and Stop is
                        * generated by one command generate Stop.
                        */
                        else if(`$INSTANCE_NAME`_CHECK_INTR_RX_MASKED(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY))
                        {
                            /* Put data in the component buffer */
                            `$INSTANCE_NAME`_mstrRdBufPtr[`$INSTANCE_NAME`_mstrRdBufIndex] = (uint8) `$INSTANCE_NAME`_RX_FIFO_RD_REG;
                            `$INSTANCE_NAME`_mstrRdBufIndex++;

                            if(`$INSTANCE_NAME`_mstrRdBufIndex < `$INSTANCE_NAME`_mstrRdBufSize)
                            {
                                `$INSTANCE_NAME`_I2C_MASTER_GENERATE_ACK;
                            }
                            else
                            {
                               endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER;
                            }
                        }
                        else
                        {
                            /* Does nothing */
                        }

                        `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_ALL);
                    }
                    else /* Writing */
                    {
                        /* INTR_MASTER_I2C_NACK :
                        * Master writes data to the slave and NACK was received: not all bytes were
                        * written to the slave from TX FIFO. Revert index if there is data in
                        * TX FIFO and pass control to complete transfer.
                        */
                        if(`$INSTANCE_NAME`_CHECK_INTR_MASTER_MASKED(`$INSTANCE_NAME`_INTR_MASTER_I2C_NACK))
                        {
                            `$INSTANCE_NAME`_ClearMasterInterruptSource(`$INSTANCE_NAME`_INTR_MASTER_I2C_NACK);

                            /* Rollback the write buffer index: the NACKed byte remains in the shifter */
                            `$INSTANCE_NAME`_mstrWrBufIndexTmp -= (`$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES +
                                                                   `$INSTANCE_NAME`_GET_TX_FIFO_SR_VALID);

                            /* Update number of transfered bytes */
                            `$INSTANCE_NAME`_mstrWrBufIndex = `$INSTANCE_NAME`_mstrWrBufIndexTmp;

                            `$INSTANCE_NAME`_mstrStatus |= (uint16) (`$INSTANCE_NAME`_I2C_MSTAT_ERR_XFER |
                                                                     `$INSTANCE_NAME`_I2C_MSTAT_ERR_SHORT_XFER);

                            `$INSTANCE_NAME`_CLEAR_TX_FIFO;

                            endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER;
                        }
                        /* INTR_TX_EMPTY :
                        * TX direction: the TX FIFO is EMPTY, the data from buffer need be put there.
                        * When there is no data in the component buffer, underflow interrupt is
                        * enabled to catch when all data will be transfered.
                        */
                        else if(`$INSTANCE_NAME`_CHECK_INTR_TX_MASKED(`$INSTANCE_NAME`_INTR_TX_EMPTY))
                        {
                            while(`$INSTANCE_NAME`_FIFO_SIZE != `$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES)
                            {
                                /* The temporary mstrWrBufIndexTmp is used because slave could NACK the byte and index
                                * roll-back required in this case. The mstrWrBufIndex is updated at the end of transfer
                                */
                                if(`$INSTANCE_NAME`_mstrWrBufIndexTmp < `$INSTANCE_NAME`_mstrWrBufSize)
                                {
                                    /* Put data into TX FIFO */
                                    `$INSTANCE_NAME`_TX_FIFO_WR_REG = (uint32) `$INSTANCE_NAME`_mstrWrBufPtr[`$INSTANCE_NAME`_mstrWrBufIndexTmp];
                                    `$INSTANCE_NAME`_mstrWrBufIndexTmp++;
                                }
                                else
                                {
                                    break; /* No more data to put */
                                }
                            }

                            if(`$INSTANCE_NAME`_mstrWrBufIndexTmp == `$INSTANCE_NAME`_mstrWrBufSize)
                            {
                                `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_INTR_TX_UNDERFLOW);
                            }

                            `$INSTANCE_NAME`_ClearTxInterruptSource(`$INSTANCE_NAME`_INTR_TX_ALL);
                        }
                        /* INTR_TX_UNDERFLOW:
                        * TX direction: all data from TX FIFO was transfered to the slave.
                        * The transaction need to be completed.
                        */
                        else if(`$INSTANCE_NAME`_CHECK_INTR_TX_MASKED(`$INSTANCE_NAME`_INTR_TX_UNDERFLOW))
                        {
                            /* Update number of transfered bytes */
                            `$INSTANCE_NAME`_mstrWrBufIndex = `$INSTANCE_NAME`_mstrWrBufIndexTmp;

                            endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER;
                        }
                        else
                        {
                            /* Does nothing */
                        }
                    }
                }

                if(0u != endTransfer) /* Complete transfer */
                {
                    /* Clean-up master after reading: only in case of NACK */
                    `$INSTANCE_NAME`_DISABLE_MASTER_AUTO_DATA_ACK;

                    /* Disable data processing interrupts: they should be cleared before */
                    `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
                    `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);

                    if(`$INSTANCE_NAME`_CHECK_I2C_MODE_NO_STOP(`$INSTANCE_NAME`_mstrControl))
                    {
                        /* On-going transaction is suspend: the ReStart is generated by API request */
                        `$INSTANCE_NAME`_mstrStatus |= (uint16) (`$INSTANCE_NAME`_I2C_MSTAT_XFER_HALT |
                                                                 `$INSTANCE_NAME`_GET_I2C_MSTAT_CMPLT);

                        `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_I2C_FSM_MSTR_HALT;
                    }
                    else
                    {
                        /* Complete transaction: exclude data processing state and generate Stop.
                        * The completion status will be set after Stop generation.
                        * Specail case is read: because NACK and Stop is genered.
                        * The lost arbitration could occur while NACK generation
                        * (other master still reading and ACK is generated)
                        */
                        `$INSTANCE_NAME`_I2C_MASTER_GENERATE_STOP;
                    }
                }
            }

        } /* (`$INSTANCE_NAME`_I2C_MASTER) */
        `$EndCond`

    } /* (`$INSTANCE_NAME`_CHECK_I2C_FSM_MASTER) */


    /* FSM Slave */
    else if(`$INSTANCE_NAME`_CHECK_I2C_FSM_SLAVE)
    {
        `$Cond`if(`$INSTANCE_NAME`_I2C_SLAVE)
        {
            /* INTR_SLAVE_NACK:
            * The master completes reading the slave: the approprite flags have to be set.
            * The TX FIFO cleared after overflow condition is set.
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK))
            {
                `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK);

                /* All entries that remain in TX FIFO max value is 9: 8 (FIFO) + 1 (SHIFTER) */
                diffCount = (`$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES + `$INSTANCE_NAME`_GET_TX_FIFO_SR_VALID);

                if(`$INSTANCE_NAME`_slOverFlowCount > diffCount) /* Overflow */
                {
                    `$INSTANCE_NAME`_slStatus |= (uint8) `$INSTANCE_NAME`_I2C_SSTAT_RD_OVFL;
                }
                else /* No Overflow */
                {
                    /* Roll-back the temporay index */
                    `$INSTANCE_NAME`_slRdBufIndexTmp -= (diffCount - `$INSTANCE_NAME`_slOverFlowCount);
                }

                /* Update slave of tranfered bytes */
                `$INSTANCE_NAME`_slRdBufIndex = `$INSTANCE_NAME`_slRdBufIndexTmp;

                /* Clean-up TX FIFO */
                `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
                `$INSTANCE_NAME`_slOverFlowCount = 0u;
                `$INSTANCE_NAME`_CLEAR_TX_FIFO;

                /* Complete master reading */
                `$INSTANCE_NAME`_slStatus &= (uint8) ~`$INSTANCE_NAME`_I2C_SSTAT_RD_BUSY;
                `$INSTANCE_NAME`_slStatus |= (uint8)  `$INSTANCE_NAME`_I2C_SSTAT_RD_CMPLT;
                `$INSTANCE_NAME`_state     =  `$INSTANCE_NAME`_I2C_FSM_IDLE;
            }


            /* INTR_SLAVE_I2C_WRITE_STOP:
            * The master completes writing to slave: the approprite flags have to be set.
            * The RX FIFO contains 1-8 bytes from previous transcation which need to be read.
            * There is possibility that RX FIFO contains address, it needs to leave it there.
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP))
            {
                `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP);

                `$INSTANCE_NAME`_DISABLE_SLAVE_AUTO_DATA;

                while(0u != `$INSTANCE_NAME`_GET_RX_FIFO_ENTRIES)
                {
                    `$Cond`if(`$INSTANCE_NAME`_CHECK_I2C_ACCEPT_ADDRESS)
                    {
                        if((1u == `$INSTANCE_NAME`_GET_RX_FIFO_ENTRIES) &&
                           (`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH)))
                        {
                            break; /* Leave address in RX FIFO */
                        }
                    }
                    `$EndCond`

                    /* Put data in the component buffer */
                    `$INSTANCE_NAME`_slWrBufPtr[`$INSTANCE_NAME`_slWrBufIndex] = (uint8) `$INSTANCE_NAME`_RX_FIFO_RD_REG;
                    `$INSTANCE_NAME`_slWrBufIndex++;
                }

                if(`$INSTANCE_NAME`_CHECK_INTR_RX(`$INSTANCE_NAME`_INTR_RX_OVERFLOW))
                {
                    `$INSTANCE_NAME`_slStatus |= (uint8) `$INSTANCE_NAME`_I2C_SSTAT_WR_OVFL;
                }

                /* Clears RX interrupt sources triggered on data receiving */
                `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_ALL);

                /* Complete master writing */
                `$INSTANCE_NAME`_slStatus &= (uint8) ~`$INSTANCE_NAME`_I2C_SSTAT_WR_BUSY;
                `$INSTANCE_NAME`_slStatus |= (uint8)  `$INSTANCE_NAME`_I2C_SSTAT_WR_CMPLT;
                `$INSTANCE_NAME`_state     =  `$INSTANCE_NAME`_I2C_FSM_IDLE;
            }


            /* INTR_SLAVE_I2C_ADDR_MATCH:
            * The address match event starts the slave operation: after leaving the TX or RX
            * direction has to chosen.
            * The wakeup interrupt must be cleared only after address match is set.
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH))
            {
                `$Cond`if(`$INSTANCE_NAME`_CHECK_I2C_ACCEPT_ADDRESS) /* Address in RX FIFO */
                {
                    address  = `$INSTANCE_NAME`_RX_FIFO_RD_REG;

                    /* Clears RX sources if address was received in the RX FIFO */
                    `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_ALL);

                    if(0u != address)
                    {
                        /* Suppress compiler warning */
                    }
                }
                `$EndCond`

                if(`$INSTANCE_NAME`_CHECK_I2C_STATUS(`$INSTANCE_NAME`_I2C_STATUS_S_READ))
                /* TX direction: master reads from slave */
                {
                    `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_INTR_TX_EMPTY);

                    /* Set temporary index to address buffer clear from API */
                    `$INSTANCE_NAME`_slRdBufIndexTmp = `$INSTANCE_NAME`_slRdBufIndex;

                    /* Start master reading */
                    `$INSTANCE_NAME`_slStatus |= (uint8) `$INSTANCE_NAME`_I2C_SSTAT_RD_BUSY;
                    `$INSTANCE_NAME`_state     = `$INSTANCE_NAME`_I2C_FSM_SL_RD;
                }
                else
                /* RX direction: master writes into slave */
                {
                    /* Calculate available buffer size */
                    diffCount = (`$INSTANCE_NAME`_slWrBufSize - `$INSTANCE_NAME`_slWrBufIndex);

                    if(diffCount < `$INSTANCE_NAME`_FIFO_SIZE)
                    /* Receive data: byte-by-byte */
                    {
                        `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY);
                    }
                    else
                    /* Receive data: into RX FIFO */
                    {
                        if(diffCount == `$INSTANCE_NAME`_FIFO_SIZE)
                        {
                            /* NACK when RX FIFO become FULL */
                            `$INSTANCE_NAME`_ENABLE_SLAVE_AUTO_DATA;
                        }
                        else
                        {
                            /* Stretch clock when RX FIFO becomes FULL */
                            `$INSTANCE_NAME`_ENABLE_SLAVE_AUTO_DATA_ACK;
                            `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_INTR_RX_FULL);
                        }
                    }

                    /* Start master reading */
                    `$INSTANCE_NAME`_slStatus |= (uint8) `$INSTANCE_NAME`_I2C_SSTAT_WR_BUSY;
                    `$INSTANCE_NAME`_state     = `$INSTANCE_NAME`_I2C_FSM_SL_WR;
                }

                /* Clear interrupts before ACK address */
                `$INSTANCE_NAME`_ClearI2CExtClkInterruptSource(`$INSTANCE_NAME`_INTR_I2C_EC_WAKE_UP);
                `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_ALL);

                /* The preparation complete: ACK the address */
                `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_ACK;
            }

            /* `$INSTANCE_NAME`_INTR_RX_FULL":
            * Get data from RX FIFO and decide whether to ACK or NACK following bytes
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_RX_MASKED(`$INSTANCE_NAME`_INTR_RX_FULL))
            {
                /* Calculate available buffer size take to account that RX FIFO is FULL */
                diffCount =  `$INSTANCE_NAME`_slWrBufSize -
                            (`$INSTANCE_NAME`_slWrBufIndex + `$INSTANCE_NAME`_FIFO_SIZE);

                if(diffCount > `$INSTANCE_NAME`_FIFO_SIZE) /* Proceed transaction */
                {
                    diffCount   = `$INSTANCE_NAME`_FIFO_SIZE;
                    endTransfer = 0u;  /* Continue active transfer */
                }
                else /* End when FIFO becomes FULL again */
                {
                    endTransfer = `$INSTANCE_NAME`_I2C_CMPLT_ANY_TRANSFER;
                }

                for(;(0u != diffCount);diffCount--)
                {
                    /* Put data in the component buffer */
                    `$INSTANCE_NAME`_slWrBufPtr[`$INSTANCE_NAME`_slWrBufIndex] = (uint8) `$INSTANCE_NAME`_RX_FIFO_RD_REG;
                    `$INSTANCE_NAME`_slWrBufIndex++;
                }

                if(0u != endTransfer) /* End transfer sending NACK */
                {
                    `$INSTANCE_NAME`_ENABLE_SLAVE_AUTO_DATA_NACK;

                    /* The INTR_RX_FULL triggers earlier then INTR_SLAVE_I2C_STOP:
                    * disable all RX interrupt sources.
                    */
                    `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
                }

                `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_FULL);
            }
            /* `$INSTANCE_NAME`_INTR_RX_NOT_EMPTY:
            * The buffer size is less than 8: it requires processing in byte-by-byte mode.
            */
            else if(`$INSTANCE_NAME`_CHECK_INTR_RX_MASKED(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY))
            {
                diffCount = `$INSTANCE_NAME`_RX_FIFO_RD_REG;

                if(`$INSTANCE_NAME`_slWrBufIndex < `$INSTANCE_NAME`_slWrBufSize)
                {
                    `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_ACK;

                    /* Put data into component buffer */
                    `$INSTANCE_NAME`_slWrBufPtr[`$INSTANCE_NAME`_slWrBufIndex] = (uint8) diffCount;
                    `$INSTANCE_NAME`_slWrBufIndex++;
                }
                else /* Overflow: there is no space in the write buffer */
                {
                    `$INSTANCE_NAME`_I2C_SLAVE_GENERATE_NACK;

                    `$INSTANCE_NAME`_slStatus |= (uint8) `$INSTANCE_NAME`_I2C_SSTAT_WR_OVFL;
                }

                `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY);
            }
            else
            {
                /* Does nothing */
            }


            /* `$INSTANCE_NAME`_INTR_TX_EMPTY:
            * Master reads slave: provide data to read or 0xFF in case end of the buffer
            * The overflow condition must be captured, but not set until the end of transaction.
            * There is possibility of false overflow due of TX FIFO utilization.
            */
            if(`$INSTANCE_NAME`_CHECK_INTR_TX_MASKED(`$INSTANCE_NAME`_INTR_TX_EMPTY))
            {
                while(`$INSTANCE_NAME`_FIFO_SIZE != `$INSTANCE_NAME`_GET_TX_FIFO_ENTRIES)
                {
                    /* The temporary slRdBufIndexTmp is used because master could NACK the byte and
                    * index roll-back required in this case. The slRdBufIndex is updated at the end
                    * of the read transfer.
                    */
                    if(`$INSTANCE_NAME`_slRdBufIndexTmp < `$INSTANCE_NAME`_slRdBufSize)
                    /* Data from buffer */
                    {
                        `$INSTANCE_NAME`_TX_FIFO_WR_REG = (uint32) `$INSTANCE_NAME`_slRdBufPtr[`$INSTANCE_NAME`_slRdBufIndexTmp];
                        `$INSTANCE_NAME`_slRdBufIndexTmp++;
                    }
                    else
                    /* Probably Overflow */
                    {
                        `$INSTANCE_NAME`_TX_FIFO_WR_REG = `$INSTANCE_NAME`_I2C_SLAVE_OVFL_RETURN;

                        if(0u == (`$INSTANCE_NAME`_INTR_TX_OVERFLOW & `$INSTANCE_NAME`_slOverFlowCount))
                        {
                            /* Get counter in range of the byte: the value 10 is overflow */
                            `$INSTANCE_NAME`_slOverFlowCount++;
                        }
                    }
                }

                `$INSTANCE_NAME`_ClearTxInterruptSource(`$INSTANCE_NAME`_INTR_TX_EMPTY);
            }

        }  /* (`$INSTANCE_NAME`_I2C_SLAVE) */
        `$EndCond`
    }


    /* FSM EXIT:
    * Interrupt sources get here are errors:
    * Slave:  INTR_SLAVE_I2C_BUS_ERROR, INTR_SLAVE_I2C_ARB_LOST
    * Master: INTR_MASTER_I2C_BUS_ERROR, INTR_MASTER_I2C_ARB_LOST.
    */
    else
    {
        /* Clean the Slave and Master sources before reset */
        `$INSTANCE_NAME`_ClearSlaveInterruptSource(`$INSTANCE_NAME`_INTR_SLAVE_ALL);
        `$INSTANCE_NAME`_ClearMasterInterruptSource(`$INSTANCE_NAME`_INTR_MASTER_ALL);

        /* Re-enable SCB block: this resets part of functions */
        enableInterrupts = CyEnterCriticalSection();
        `$INSTANCE_NAME`_SCB_SW_RESET;
        CyExitCriticalSection(enableInterrupts);

        /* Clenup:
        * All other status and control bits can be cleared later.
        * Slave AUTO data ACK never happens before address ACK.
        * Slave TX and RX sources are used only after address match.
        * Master AUTO data ACK is under API control.
        * Master interrupt sources does not care after any error.
        * Master TX and RX sources are under API control.
        */

        /* Disable auto NACK before clear the FIFOs */
        `$INSTANCE_NAME`_DISABLE_SLAVE_AUTO_DATA_ACK;
        `$INSTANCE_NAME`_DISABLE_MASTER_AUTO_DATA_ACK;

        `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
        `$INSTANCE_NAME`_SetTxInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
        `$INSTANCE_NAME`_ClearTxInterruptSource(`$INSTANCE_NAME`_INTR_RX_ALL);
        `$INSTANCE_NAME`_ClearRxInterruptSource(`$INSTANCE_NAME`_INTR_TX_ALL);

        `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_I2C_FSM_IDLE;
    }
}


/* [] END OF FILE */
