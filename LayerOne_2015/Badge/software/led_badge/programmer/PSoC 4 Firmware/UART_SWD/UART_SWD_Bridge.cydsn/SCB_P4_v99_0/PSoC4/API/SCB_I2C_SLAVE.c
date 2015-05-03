/*******************************************************************************
* File Name: `$INSTANCE_NAME`_I2C_SLAVE.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  I2C Slave mode.
*
* Note:
*
*******************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`_I2C_PVT.h"

#if(`$INSTANCE_NAME`_I2C_SLAVE_CONST)

/***************************************
*      I2C Slave Private Vars
***************************************/

volatile uint8 `$INSTANCE_NAME`_slStatus;          /* Slave Status */

/* Receive buffer variables */
volatile uint8 * `$INSTANCE_NAME`_slWrBufPtr;      /* Pointer to Receive buffer  */
volatile uint32  `$INSTANCE_NAME`_slWrBufSize;     /* Slave Receive buffer size  */
volatile uint32  `$INSTANCE_NAME`_slWrBufIndex;    /* Slave Receive buffer Index */

/* Transmit buffer variables */
volatile uint8 * `$INSTANCE_NAME`_slRdBufPtr;      /* Pointer to Transmit buffer  */
volatile uint32  `$INSTANCE_NAME`_slRdBufSize;     /* Slave Transmit buffer size  */
volatile uint32  `$INSTANCE_NAME`_slRdBufIndex;    /* Slave Transmit buffer Index */
volatile uint32  `$INSTANCE_NAME`_slRdBufIndexTmp; /* Slave Transmit buffer Index Tmp */
volatile uint8   `$INSTANCE_NAME`_slOverFlowCount; /* Slave Transmit Overflow counter */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveStatus
********************************************************************************
*
* Summary:
*  Returns I2C slave's communication status.
*
* Parameters:
*  None
*
* Return:
*  Current status of I2C slave.
*
* Global variables:
*  `$INSTANCE_NAME`_slStatus - used to store current status of I2C slave.
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_I2CSlaveStatus(void)
{
    return((uint32) `$INSTANCE_NAME`_slStatus);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveClearReadStatus
********************************************************************************
*
* Summary:
*  Clears the read status flags and returns their values. No other status flags
*  are affected.
*
* Parameters:
*  None
*
* Return:
*  Current read status of I2C slave.
*
* Global variables:
*  `$INSTANCE_NAME`_slStatus  - used to store current status of I2C slave.
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_I2CSlaveClearReadStatus(void)
{
    uint32 status;

    /* Mask of transfer complete flag and Error status */
    status = ((uint32) `$INSTANCE_NAME`_slStatus & `$INSTANCE_NAME`_I2C_SSTAT_RD_MASK);
    `$INSTANCE_NAME`_slStatus &= (uint8) ~`$INSTANCE_NAME`_I2C_SSTAT_RD_CLEAR;

    return(status);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveClearWriteStatus
********************************************************************************
*
* Summary:
*  Clears the write status flags and returns their values. No other status flags
*  are affected.
*
* Parameters:
*  None
*
* Return:
*  Current write status of I2C slave.
*
* Global variables:
*  `$INSTANCE_NAME`_slStatus  - used to store current status of I2C slave.
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_I2CSlaveClearWriteStatus(void)
{
    uint32 status;

    /* Mask of transfer complete flag and Error status */
    status = ((uint32) `$INSTANCE_NAME`_slStatus & `$INSTANCE_NAME`_I2C_SSTAT_WR_MASK);
    `$INSTANCE_NAME`_slStatus &= (uint8) ~`$INSTANCE_NAME`_I2C_SSTAT_WR_CLEAR;

    return(status);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveSetAddress
********************************************************************************
*
* Summary:
*  Sets the I2C slave address.
*
* Parameters:
*  address: I2C slave address for the primary device. This value may be any
*  address between 0 and 127.
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_I2CSlaveSetAddress(uint32 address)
{
    uint32 matchReg;

    matchReg = `$INSTANCE_NAME`_RX_MATCH_REG;

    matchReg &= ((uint32) ~`$INSTANCE_NAME`_RX_MATCH_ADDR_MASK); /* Clear address bits */
    matchReg |= ((uint32)  `$INSTANCE_NAME`_GET_I2C_8BIT_ADDRESS(address));

    `$INSTANCE_NAME`_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveSetAddressMask
********************************************************************************
*
* Summary:
*  Sets the I2C slave address mask.
*
* Parameters:
*  addressMask: Address mask.
*   0 - address bit does not care while comparison.
*   1 - address bit is significant while comparison.
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_I2CSlaveSetAddressMask(uint32 addressMask)
{
    uint32 matchReg;

    matchReg = `$INSTANCE_NAME`_RX_MATCH_REG;

    matchReg &= ((uint32) ~`$INSTANCE_NAME`_RX_MATCH_MASK_MASK); /* Clear address mask bits */
    matchReg |= ((uint32) (addressMask << `$INSTANCE_NAME`_RX_MATCH_MASK_POS));

    `$INSTANCE_NAME`_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveInitReadBuf
********************************************************************************
*
* Summary:
*  Sets the buffer pointer and size of the read buffer. This function also
*  resets the transfer count returned with the I2C_SlaveGetReadBufSize function.
*
* Parameters:
*  readBuf:  Pointer to the data buffer to be read by the master.
*  bufSize:  Size of the read buffer exposed to the I2C master.
*
* Return:
*  None
*
* Global variables:
*  `$INSTANCE_NAME`_slRdBufPtr   - used to store pointer to slave read buffer.
*  `$INSTANCE_NAME`_slRdBufSize  - used to store salve read buffer size.
*  `$INSTANCE_NAME`_slRdBufIndex - used to store current index within slave
*  read buffer.
*
* Side Effects:
*  If this function is called during a bus transaction, data from the previous
*  buffer location and the beginning of current buffer may be transmitted.
*
*******************************************************************************/
void `$INSTANCE_NAME`_I2CSlaveInitReadBuf(uint8 * rdBuf, uint32 bufSize)
{
    /* Check for proper buffer */
    if(NULL != rdBuf)
    {
        `$INSTANCE_NAME`_slRdBufPtr      = (volatile uint8 *) rdBuf; /* Set buffer pointer  */
        `$INSTANCE_NAME`_slRdBufSize     = bufSize;                  /* Set buffer size     */
        `$INSTANCE_NAME`_slRdBufIndex    = 0u;                       /* Clear buffer index  */
        `$INSTANCE_NAME`_slRdBufIndexTmp = 0u;                       /* Clear buffer index  */
    }
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveInitWriteBuf
********************************************************************************
*
* Summary:
*  Sets the buffer pointer and size of the read buffer. This function also
*  resets the transfer count returned with the I2C_SlaveGetReadBufSize function.
*
* Parameters:
*  writeBuf:  Pointer to the data buffer to be read by the master.
*  bufSize:  Size of the buffer exposed to the I2C master.
*
* Return:
*  None
*
* Global variables:
*  `$INSTANCE_NAME`_slWrBufPtr   - used to store pointer to slave write buffer.
*  `$INSTANCE_NAME`_slWrBufSize  - used to store salve write buffer size.
*  `$INSTANCE_NAME`_slWrBufIndex - used to store current index within slave
*  write buffer.
*
* Side Effects:
*  If this function is called during a bus transaction, data from the previous
*  buffer location and the beginning of current buffer may be transmitted.
*
*******************************************************************************/
void `$INSTANCE_NAME`_I2CSlaveInitWriteBuf(uint8 * wrBuf, uint32 bufSize)
{
    /* Check buffer pointer */
    if(NULL != wrBuf)
    {
        `$INSTANCE_NAME`_slWrBufPtr   = (volatile uint8 *) wrBuf; /* Set buffer pointer  */
        `$INSTANCE_NAME`_slWrBufSize  = bufSize;                  /* Set buffer size     */
        `$INSTANCE_NAME`_slWrBufIndex = 0u;                       /* Clear buffer index  */
    }
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveGetReadBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes read by the I2C master since an
*  I2C_SlaveInitReadBuf or I2C_SlaveClearReadBuf function was executed.
*  The maximum return value will be the size of the read buffer.
*
* Parameters:
*  None
*
* Return:
*  Bytes read by master.
*
* Global variables:
*  `$INSTANCE_NAME`_slRdBufIndex - used to store current index within slave
*  read buffer.
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_I2CSlaveGetReadBufSize(void)
{
    return(`$INSTANCE_NAME`_slRdBufIndex);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveGetWriteBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes written by the I2C master since an
*  I2C_SlaveInitWriteBuf or I2C_SlaveClearWriteBuf function was executed.
*  The maximum return value will be the size of the write buffer.
*
* Parameters:
*  None
*
* Return:
*  Bytes written by master.
*
* Global variables:
*  `$INSTANCE_NAME`_slWrBufIndex - used to store current index within slave
*  write buffer.
*
*******************************************************************************/
uint32 `$INSTANCE_NAME`_I2CSlaveGetWriteBufSize(void)
{
    return(`$INSTANCE_NAME`_slWrBufIndex);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveClearReadBuf
********************************************************************************
*
* Summary:
*  Resets the read pointer to the first byte in the read buffer. The next byte
*  read by the master will be the first byte in the read buffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  `$INSTANCE_NAME`_slRdBufIndex - used to store current index within slave
*  read buffer.
*
*******************************************************************************/
void `$INSTANCE_NAME`_I2CSlaveClearReadBuf(void)
{
    `$INSTANCE_NAME`_slRdBufIndex = 0u;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CSlaveClearWriteBuf
********************************************************************************
*
* Summary:
*  Resets the write pointer to the first byte in the write buffer. The next byte
*  written by the master will be the first byte in the write buffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  `$INSTANCE_NAME`_slWrBufIndex - used to store current index within slave
*  write buffer.
*
*******************************************************************************/
void `$INSTANCE_NAME`_I2CSlaveClearWriteBuf(void)
{
    `$INSTANCE_NAME`_slWrBufIndex = 0u;
}

#endif /* (`$INSTANCE_NAME`_I2C_SLAVE_CONST) */


/* [] END OF FILE */
