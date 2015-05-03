/*******************************************************************************
* File Name: `$INSTANCE_NAME`_EZI2C.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  EZI2C mode.
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
#include "`$INSTANCE_NAME`_EZI2C_PVT.h"


/***************************************
*      EZI2C Private Vars
***************************************/

volatile uint8 `$INSTANCE_NAME`_curStatus; /* Status byte */
uint8 `$INSTANCE_NAME`_fsmState;           /* FSM state   */

/* Variables intended to be used with Buffer1: Primary slave address */
volatile uint8 * `$INSTANCE_NAME`_dataBuffer1; /* Pointer to data buffer1       */
uint16 `$INSTANCE_NAME`_bufSizeBuf1; /* Size of buffer1 in bytes      */
uint16 `$INSTANCE_NAME`_protectBuf1; /* Start index of write protected area buffer1 */
`$EzI2cBitWidthReplacementString` `$INSTANCE_NAME`_offsetBuf1; /* Current offset within buffer1 */
`$EzI2cBitWidthReplacementString` `$INSTANCE_NAME`_indexBuf1;  /* Current index within buffer1  */


#if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST)
    uint8 `$INSTANCE_NAME`_addrBuf1; /* Primary slave address. Used for software comparision   */
    uint8 `$INSTANCE_NAME`_addrBuf2; /* Secondary slave address. Used for software comparision */

    /* Variables intended to be used with Buffer1: Primary slave address */
    volatile uint8 * `$INSTANCE_NAME`_dataBuffer2; /* Pointer to data buffer2       */
    uint16 `$INSTANCE_NAME`_bufSizeBuf2; /* Size of buffer2 in bytes      */
    uint16 `$INSTANCE_NAME`_protectBuf2; /* Start index of write protected area buffer 2 */
    `$EzI2cBitWidthReplacementString` `$INSTANCE_NAME`_offsetBuf2; /* Current offset within buffer2 */
    `$EzI2cBitWidthReplacementString` `$INSTANCE_NAME`_indexBuf2;  /* Current index within buffer2  */   
#endif /* (`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST) */


/***************************************
*      EZI2C Private Functions
***************************************/

#if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST)
    static uint32 `$INSTANCE_NAME`_EzI2CUpdateRxMatchReg(uint32 addr1, uint32 addr2);
#endif /* (`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST) */

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Config Structure Initialization
    ***************************************/

    /* Constant configuration of EZI2C */
    const `$INSTANCE_NAME`_EZI2C_INIT_STRUCT `$INSTANCE_NAME`_configEzI2C =
    {
        `$INSTANCE_NAME`_EZI2C_CLOCK_STRETCHING,
        `$INSTANCE_NAME`_EZI2C_MEDIAN_FILTER_ENABLE,
        `$INSTANCE_NAME`_EZI2C_NUMBER_OF_ADDRESSES,
        `$INSTANCE_NAME`_EZI2C_PRIMARY_SLAVE_ADDRESS,
        `$INSTANCE_NAME`_EZI2C_SECONDARY_SLAVE_ADDRESS,
        `$INSTANCE_NAME`_EZI2C_SUB_ADDRESS_SIZE,
        `$INSTANCE_NAME`_EZI2C_WAKE_ENABLE
    };

    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EzI2CInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for EZI2C operation.
    *
    * Parameters:
    *  config:  Pointer to a structure that contains the following ordered list of
    *           fields. These fields match the selections available in the
    *           customizer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_EzI2CInit(const `$INSTANCE_NAME`_EZI2C_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to invalid function parameter */
        }
        else
        {
            /* Configure pins */
            `$INSTANCE_NAME`_SetPins(`$INSTANCE_NAME`_SCB_MODE_EZI2C, `$INSTANCE_NAME`_DUMMY_PARAM,
                                                                      `$INSTANCE_NAME`_DUMMY_PARAM);

            /* Store internal configuration */
            `$INSTANCE_NAME`_scbMode       = (uint8) `$INSTANCE_NAME`_SCB_MODE_EZI2C;
            `$INSTANCE_NAME`_scbEnableWake = (uint8) config->enableWake;
            `$INSTANCE_NAME`_scbEnableIntr = (uint8) `$INSTANCE_NAME`_SCB_IRQ_INTERNAL;

            `$INSTANCE_NAME`_numberOfAddr  = (uint8) config->numberOfAddresses;
            `$INSTANCE_NAME`_subAddrSize   = (uint8) config->subAddrSize;

            /* Configure I2C interface */
            `$INSTANCE_NAME`_CTRL_REG     = `$INSTANCE_NAME`_GET_CTRL_ADDR_ACCEPT(config->numberOfAddresses) |
                                            `$INSTANCE_NAME`_GET_CTRL_EC_AM_MODE (config->enableWake);

            `$INSTANCE_NAME`_I2C_CTRL_REG = `$INSTANCE_NAME`_EZI2C_CTRL;

            /* Adjust SDA filter settigns */
            `$INSTANCE_NAME`_SET_I2C_CFG_SDA_FILT_TRIM(`$INSTANCE_NAME`_EC_AM_I2C_CFG_SDA_FILT_TRIM);

            /* Configure RX direction */
            `$INSTANCE_NAME`_RX_CTRL_REG      = `$INSTANCE_NAME`_GET_RX_CTRL_MEDIAN(config->enableMedianFilter) |
                                                `$INSTANCE_NAME`_EZI2C_RX_CTRL;
            `$INSTANCE_NAME`_RX_FIFO_CTRL_REG = `$INSTANCE_NAME`_CLEAR_REG;

            /* Set default address and mask */
            if(`$INSTANCE_NAME`_EZI2C_PRIMARY_ADDRESS == config->numberOfAddresses)
            {
                `$INSTANCE_NAME`_RX_MATCH_REG = `$INSTANCE_NAME`_EzI2CUpdateRxMatchReg(config->primarySlaveAddr,
                                                                                       config->primarySlaveAddr);
            }
            else
            {
                `$INSTANCE_NAME`_RX_MATCH_REG = `$INSTANCE_NAME`_EzI2CUpdateRxMatchReg(config->primarySlaveAddr,
                                                                                       config->secondarySlaveAddr);
            }

            /* Configure TX direction */
            `$INSTANCE_NAME`_TX_CTRL_REG      = `$INSTANCE_NAME`_EZI2C_TX_CTRL;
            `$INSTANCE_NAME`_TX_FIFO_CTRL_REG = `$INSTANCE_NAME`_CLEAR_REG;

            /* Configure interrupt sources */
            `$INSTANCE_NAME`_INTR_I2C_EC_MASK_REG = `$INSTANCE_NAME`_NO_INTR_SOURCES;
            `$INSTANCE_NAME`_INTR_SPI_EC_MASK_REG = `$INSTANCE_NAME`_NO_INTR_SOURCES;
            `$INSTANCE_NAME`_INTR_MASTER_MASK_REG = `$INSTANCE_NAME`_NO_INTR_SOURCES;
            `$INSTANCE_NAME`_INTR_SLAVE_MASK_REG  = `$INSTANCE_NAME`_EZI2C_INTR_SLAVE_MASK; 
            `$INSTANCE_NAME`_INTR_TX_MASK_REG     = `$INSTANCE_NAME`_NO_INTR_SOURCES;

            /* Configure interrupt with EZI2C handler */
            `$INSTANCE_NAME`_SCB_IRQ_Disable();
            `$INSTANCE_NAME`_SCB_IRQ_SetVector((0u != (config->enableClockStretch)) ?
                                                        (&`$INSTANCE_NAME`_EZI2C_STRETCH_ISR) :
                                                        (&`$INSTANCE_NAME`_EZI2C_NO_STRETCH_ISR));
            `$INSTANCE_NAME`_SCB_IRQ_SetPriority((uint8)`$INSTANCE_NAME`_SCB_IRQ_INTC_PRIOR_NUMBER);
            
            
            if(0u != (config->enableClockStretch))
            {
                /* Configure interrupt sources */
                `$INSTANCE_NAME`_INTR_SLAVE_MASK_REG |= `$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH;
                `$INSTANCE_NAME`_INTR_RX_MASK_REG     = `$INSTANCE_NAME`_NO_INTR_SOURCES;
            }
            else
            {
                /* Enable Auto ACK/NACK features */
                `$INSTANCE_NAME`_I2C_CTRL_REG |= `$INSTANCE_NAME`_EZI2C_CTRL_AUTO;
                 
                /* Configure RX interrupt source */
                `$INSTANCE_NAME`_INTR_SLAVE_MASK_REG |= `$INSTANCE_NAME`_INTR_SLAVE_I2C_START;
                `$INSTANCE_NAME`_INTR_RX_MASK_REG     = `$INSTANCE_NAME`_INTR_RX_NOT_EMPTY;
            }

            /* Configure global variables */
            if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16)
            {
                `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_HI8;
            }
            else
            {
                `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8;
            }

            `$INSTANCE_NAME`_curStatus  = 0u;
            `$INSTANCE_NAME`_indexBuf1  = 0u;
            `$INSTANCE_NAME`_offsetBuf1 = 0u;
            `$INSTANCE_NAME`_indexBuf2  = 0u;
            `$INSTANCE_NAME`_offsetBuf2 = 0u;

            `$INSTANCE_NAME`_addrBuf1 = (uint8) config->primarySlaveAddr;
            `$INSTANCE_NAME`_addrBuf2 = (uint8) config->secondarySlaveAddr;
        }
    }

#else

    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EzI2CInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for EZI2C operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_EzI2CInit(void)
    {
        /* Configure I2C interface */
        `$INSTANCE_NAME`_CTRL_REG     = `$INSTANCE_NAME`_EZI2C_DEFAULT_CTRL;
        `$INSTANCE_NAME`_I2C_CTRL_REG = `$INSTANCE_NAME`_EZI2C_DEFAULT_I2C_CTRL;

        /* Adjust SDA filter settigns */
        `$INSTANCE_NAME`_SET_I2C_CFG_SDA_FILT_TRIM(`$INSTANCE_NAME`_EC_AM_I2C_CFG_SDA_FILT_TRIM);

        /* Configure RX direction */
        `$INSTANCE_NAME`_RX_CTRL_REG      = `$INSTANCE_NAME`_EZI2C_DEFAULT_RX_CTRL;
        `$INSTANCE_NAME`_RX_FIFO_CTRL_REG = `$INSTANCE_NAME`_EZI2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        `$INSTANCE_NAME`_RX_MATCH_REG     = `$INSTANCE_NAME`_EZI2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        `$INSTANCE_NAME`_TX_CTRL_REG      = `$INSTANCE_NAME`_EZI2C_DEFAULT_TX_CTRL;
        `$INSTANCE_NAME`_TX_FIFO_CTRL_REG = `$INSTANCE_NAME`_EZI2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with EZI2C handler and disable it */
        `$INSTANCE_NAME`_SCB_IRQ_Disable();
        
    #if(`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST)
        `$INSTANCE_NAME`_SCB_IRQ_SetVector(&`$INSTANCE_NAME`_EZI2C_STRETCH_ISR);
    #else
        `$INSTANCE_NAME`_SCB_IRQ_SetVector(&`$INSTANCE_NAME`_EZI2C_NO_STRETCH_ISR);
    #endif /* (`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST) */
        
        `$INSTANCE_NAME`_SCB_IRQ_SetPriority((uint8)`$INSTANCE_NAME`_SCB_IRQ_INTC_PRIOR_NUMBER);
    
        /* Configure interrupt sources */
        `$INSTANCE_NAME`_INTR_I2C_EC_MASK_REG = `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_I2C_EC_MASK;
        `$INSTANCE_NAME`_INTR_SPI_EC_MASK_REG = `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_SPI_EC_MASK;
        `$INSTANCE_NAME`_INTR_SLAVE_MASK_REG  = `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_SLAVE_MASK;
        `$INSTANCE_NAME`_INTR_MASTER_MASK_REG = `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_MASTER_MASK;
        `$INSTANCE_NAME`_INTR_RX_MASK_REG     = `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_RX_MASK;
        `$INSTANCE_NAME`_INTR_TX_MASK_REG     = `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
    #if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST)
        `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_HI8;
    #else
        `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8;
    #endif /* (`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST) */

        `$INSTANCE_NAME`_curStatus  = 0u;
        `$INSTANCE_NAME`_indexBuf1  = 0u;
        `$INSTANCE_NAME`_offsetBuf1 = 0u;

    #if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST)
        `$INSTANCE_NAME`_indexBuf2  = 0u;
        `$INSTANCE_NAME`_offsetBuf2 = 0u;
        
        `$INSTANCE_NAME`_addrBuf1 = `$INSTANCE_NAME`_EZI2C_PRIMARY_SLAVE_ADDRESS;
        `$INSTANCE_NAME`_addrBuf2 = `$INSTANCE_NAME`_EZI2C_SECONDARY_SLAVE_ADDRESS;
    #endif /* (`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST) */
    }
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EzI2CStop
********************************************************************************
*
* Summary:
*  Initializes I2C registers with initial values provided from customizer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*
*
*******************************************************************************/
void `$INSTANCE_NAME`_EzI2CStop(void)
{
    `$Cond`if(`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16)
    {
        `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_HI8;
    }
    `$Cond`else
    {
        `$INSTANCE_NAME`_fsmState = `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8;
    }
    `$EndCond`
}

#if(`$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EzI2CSaveConfig
    ********************************************************************************
    *
    * Summary:
    *  Wakeup disabled: does nothing.
    *  Wakeup  enabled: clears SCB_backup.enableState and enables
    *  INTR_I2C_EC_WAKE_UP interrupt used for wakeup. This interrupt source
    *  tracks in active mode therefore it does not require to be cleared.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_EzI2CSaveConfig(void)
    {
        /* Enable interrupt to wakeup the device */
        `$INSTANCE_NAME`_SetI2CExtClkInterruptMode(`$INSTANCE_NAME`_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EzI2CRestoreConfig
    ********************************************************************************
    *
    * Summary:
    *  Wakeup disabled: does nothing.
    *  Wakeup  enabled: disables INTR_I2C_EC_WAKE_UP interrupt used for wakeup.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_EzI2CRestoreConfig(void)
    {
        /* Disable interrupt to wakeup the device */
        `$INSTANCE_NAME`_SetI2CExtClkInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
    }
#endif /* (`$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST) */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EzI2CGetActivity
********************************************************************************
*
* Summary:
*  Returns a nonzero value if an I2C read or write transaction occurred since the
*  last time this function was called. The status read and write flags reset to
*  zero after read.
*  Busy status flag is cleared when transaction intended to the I2C Slave
*  completes.
*
* Parameters:
*  None
*
* Return:
*  Returns status of EZI2C Slave activity.
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_EzI2CGetActivity(void)
{
    uint32 status;

    `$INSTANCE_NAME`_DisableInt();  /* Lock from interruption */

    status = `$INSTANCE_NAME`_curStatus;

#if(`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_DISABLE_CONST)
    /* I2C_ADDR_MATCH set by HW on address match and clears on I2C_STOP interrupt */
    if(!`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH) &&
        `$INSTANCE_NAME`_CHECK_INTR_SLAVE(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH))
    {
        status |= `$INSTANCE_NAME`_EZI2C_STATUS_BUSY;
    }
#endif /* (`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_DISABLE_CONST */

    `$INSTANCE_NAME`_curStatus &= ((uint8) ~`$INSTANCE_NAME`_EZI2C_CLEAR_STATUS);

    `$INSTANCE_NAME`_EnableInt();   /* Release lock */

    return(status);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EzI2CSetAddress1
********************************************************************************
*
* Summary:
*  Sets the primary I2C slave address.
*
* Parameters:
*  address: I2C slave address for the secondary device.
*           This address is the 7-bit right-justified slave address and does
*           not include the R/W bit.
*           This value may be any address between 0 and 127 (0x00 to 0x7F).
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_EzI2CSetAddress1(uint32 address)
{
    `$Cond`if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE)
    {
        `$INSTANCE_NAME`_addrBuf1 = (uint8) address;

        `$INSTANCE_NAME`_RX_MATCH_REG = `$INSTANCE_NAME`_EzI2CUpdateRxMatchReg(address,
                                                                               (uint32) `$INSTANCE_NAME`_addrBuf2);
    }
    `$Cond`else
    {
        uint32 matchReg;

        matchReg = `$INSTANCE_NAME`_RX_MATCH_REG;

        matchReg &= ((uint32) ~`$INSTANCE_NAME`_RX_MATCH_ADDR_MASK);            /* Clear address bits */
        matchReg |= ((uint32)  `$INSTANCE_NAME`_GET_I2C_8BIT_ADDRESS(address)); /* Set mask */

        `$INSTANCE_NAME`_RX_MATCH_REG = matchReg;
    }
    `$EndCond`
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EzI2CGetAddress1
********************************************************************************
*
* Summary:
*  Gets the primary I2C slave 7-bit address.
*
* Parameters:
*  None
*
* Return:
*  Returns I2C slave address for the primary device.
*  This address is the 7-bit right-justified slave address and does not
*  include the R/W bit.
*  This value may be any address between 0 and 127 (0x00 to 0x7F).
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_EzI2CGetAddress1(void)
{
    uint32 address;

    `$Cond`if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE)
    {
        address = (uint32) `$INSTANCE_NAME`_addrBuf1;
    }
    `$Cond`else
    {
        address = (`$INSTANCE_NAME`_GET_RX_MATCH_ADDR(`$INSTANCE_NAME`_RX_MATCH_REG) >>
                   `$INSTANCE_NAME`_I2C_SLAVE_ADDR_POS);
    }
    `$EndCond`

    return(address);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EzI2CSetBuffer1
********************************************************************************
*
* Summary:
*  This function sets the buffer pointer, size, and read/write area for the
*  slave data. This is the data that is exposed to the I2C Master on primary
*  address request.
*
* Parameters:
*  bufSize: Size of the buffer in bytes.
*  rwBoundary: Sets how many bytes are writable in the beginning of the buffer.
*              This value must be less than or equal to the buffer size.
*              Data located at offset rwBoundry and greater are read only.
*  buffer: Pointer to the data buffer.
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_EzI2CSetBuffer1(uint32 bufSize, uint32 rwBoundary, volatile uint8 * buffer)
{
    if(NULL != buffer)
    {
        `$INSTANCE_NAME`_dataBuffer1 =  buffer;
        `$INSTANCE_NAME`_bufSizeBuf1 = (uint16) bufSize;
        `$INSTANCE_NAME`_protectBuf1 = (uint16) rwBoundary;
    }
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EzI2CSetReadBoundaryBuffer1
********************************************************************************
*
* Summary:
*  This function sets the read boundary of in the buffer accessed by I2C Master
*  by primary address request.
*
* Parameters:
*  rwBoundry: Sets how many bytes are writable in the beginning of the buffer.
*             This value must be less than or equal to the buffer size.
*             Data located at offset rwBoundry and greater are read only.
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_EzI2CSetReadBoundaryBuffer1(uint32 rwBoundary)
{
    `$INSTANCE_NAME`_protectBuf1 = (uint16) rwBoundary;
}


#if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EzI2CUpdateRxMatchReg
    ********************************************************************************
    *
    * Summary:
    *  Returns the value of RX MATCH register. The addr1 is accepted as primary
    *  address and commits RX_MATCH.ADDRESS value (addr1 << 0x01 as bit0 is RnW bit
    *  which equal zero). The RX_MATCH.MASK is sets as: addr1 and addr2 equal bits
    *  sets to 1 in other case to 0.
    *
    * Parameters:
    *  addr1: I2C slave address for the primary device.
    *  addr2: I2C slave address for the secondary device.
    *         This address is the 7-bit right-justified slave address and does
    *         not include the R/W bit.
    *         This value may be any address between 0 and 127 (0x00 to 0x7F).
    *
    *
    * Return:
    *  Value of RX MATCH register.
    *
    *******************************************************************************/
    static uint32 `$INSTANCE_NAME`_EzI2CUpdateRxMatchReg(uint32 addr1, uint32 addr2)
    {
        uint32 matchReg;

        matchReg  = ~(addr1 ^ addr2); /* If (addr1 and addr2) bit matches - mask bit equal 1, in other case 0 */

        matchReg  = (uint32) (`$INSTANCE_NAME`_GET_I2C_8BIT_ADDRESS(matchReg) << `$INSTANCE_NAME`_RX_MATCH_MASK_POS);
        matchReg |= `$INSTANCE_NAME`_GET_I2C_8BIT_ADDRESS(addr1);

        return(matchReg);
    }

    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EzI2CSetAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the secondary I2C slave address.
    *
    * Parameters:
    *  address: I2C slave address for the secondary device.
    *           This address is the 7-bit right-justified slave address and does
    *           not include the R/W bit.
    *           This value may be any address between 0 and 127 (0x00 to 0x7F).
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_EzI2CSetAddress2(uint32 address)
    {
        `$INSTANCE_NAME`_addrBuf2 = (uint8) address;

        `$INSTANCE_NAME`_RX_MATCH_REG = `$INSTANCE_NAME`_EzI2CUpdateRxMatchReg((uint32) `$INSTANCE_NAME`_addrBuf1,
                                                                               address);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EzI2CGetAddress2
    ********************************************************************************
    *
    * Summary:
    *  Gets secondary the I2C slave 7-bit address.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Returns I2C slave address for the secondary device.
    *  This address is the 7-bit right-justified slave address and does not
    *  include the R/W bit.
    *  This value may be any address between 0 and 127 (0x00 to 0x7F).
    *
    *******************************************************************************/
    uint32 `$INSTANCE_NAME`_EzI2CGetAddress2(void)
    {
        return((uint32) `$INSTANCE_NAME`_addrBuf2);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EzI2CSetBuffer2
    ********************************************************************************
    *
    * Summary:
    *  This function sets the buffer pointer, size, and read/write area for the
    *  slave data. This is the data that is exposed to the I2C Master on secondary
    *  address request.
    *
    * Parameters:
    *  bufSize: Size of the buffer in bytes.
    *  rwBoundary: Sets how many bytes are writable in the beginning of the buffer.
    *              This value must be less than or equal to the buffer size.
    *              Data located at offset rwBoundry and greater are read only.
    *  buffer: Pointer to the data buffer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_EzI2CSetBuffer2(uint32 bufSize, uint32 rwBoundary, volatile uint8 * buffer)
    {
        if(NULL != buffer)
        {
            `$INSTANCE_NAME`_dataBuffer2 =  buffer;
            `$INSTANCE_NAME`_bufSizeBuf2 = (uint16) bufSize;
            `$INSTANCE_NAME`_protectBuf2 = (uint16) rwBoundary;
        }
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_EzI2CSetReadBoundaryBuffer2
    ********************************************************************************
    *
    * Summary:
    *  This function sets the read boundary of in the buffer accessed by I2C Master
    *  by secondary address request.
    *
    * Parameters:
    *  rwBoundary: Sets how many bytes are writable in the beginning of the buffer.
    *              This value must be less than or equal to the buffer size.
    *              Data located at offset rwBoundry and greater are read only.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_EzI2CSetReadBoundaryBuffer2(uint32 rwBoundary)
    {
        `$INSTANCE_NAME`_protectBuf2 = (uint16) rwBoundary;
    }

#endif /* (`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST) */


/* [] END OF FILE */
