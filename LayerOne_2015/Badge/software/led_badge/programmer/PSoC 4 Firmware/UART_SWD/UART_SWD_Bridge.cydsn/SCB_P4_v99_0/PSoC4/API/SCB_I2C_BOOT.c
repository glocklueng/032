/*******************************************************************************
* File Name: `$INSTANCE_NAME`_I2C_BOOT.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the API for the bootloader
*  communication support in SCB Component I2C mode.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`_I2C_PVT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_I2C_BTLDR_COMM_ENABLED)

/***************************************
*    Private I/O Component Vars
***************************************/

/* Writes to this buffer */
static uint8 `$INSTANCE_NAME`_slReadBuf[`$INSTANCE_NAME`_I2C_BTLDR_SIZEOF_READ_BUFFER];

/* Reads from this buffer */
static uint8 `$INSTANCE_NAME`_slWriteBuf[`$INSTANCE_NAME`_I2C_BTLDR_SIZEOF_WRITE_BUFFER];

/* Flag to release buffer to be read */
static uint32 `$INSTANCE_NAME`_applyBuffer;


/***************************************
*    Private Function Prototypes
***************************************/

static void `$INSTANCE_NAME`_I2CResposeInsert(void);


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CCyBtldrCommStart
********************************************************************************
*
* Summary:
*  Starts the I2C component and enables its interrupt.
*  Every incoming I2C write transaction is treated as a command for the
*  bootloader.
*  Every incoming I2C read transaction returns 0xFF until the bootloader
*  provides a response to the executed command.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_I2CCyBtldrCommStart(void)
{
    `$INSTANCE_NAME`_I2CSlaveInitWriteBuf(`$INSTANCE_NAME`_slWriteBuf, `$INSTANCE_NAME`_I2C_BTLDR_SIZEOF_WRITE_BUFFER);
    `$INSTANCE_NAME`_I2CSlaveInitReadBuf (`$INSTANCE_NAME`_slReadBuf, 0u);

    `$INSTANCE_NAME`_SetCustomInterruptHandler(&`$INSTANCE_NAME`_I2CResposeInsert);
    `$INSTANCE_NAME`_applyBuffer = 0u;

    `$INSTANCE_NAME`_Start();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CyBtldrCommStop
********************************************************************************
*
* Summary:
*  Disables the I2C component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_I2CCyBtldrCommStop(void)
{
    `$INSTANCE_NAME`_Stop();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CyBtldrCommReset
********************************************************************************
*
* Summary:
*  Sets read and write I2C buffers to the initial state and resets the slave
*  status.
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
void `$INSTANCE_NAME`_I2CCyBtldrCommReset(void)
{
    /* Make the Read buffer full */
    `$INSTANCE_NAME`_slRdBufSize = 0u;

    /* Reset Write buffer and Read buffer */
    `$INSTANCE_NAME`_slRdBufIndex = 0u;
    `$INSTANCE_NAME`_slWrBufIndex = 0u;

    /* Clear read and write status */
    `$INSTANCE_NAME`_slStatus = 0u;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CyBtldrCommRead
********************************************************************************
*
* Summary:
*  Allows the caller to read data from the bootloader host.
*  The function handles polling to allow a block of data to be completely
*  received from the host device.
*
* Parameters:
*  pData:    Pointer to storage for the block of data to be read from the
*            bootloader host
*  size:     Number of bytes to be read.
*  count:    Pointer to the variable to write the number of bytes actually
*            read.
*  timeOut:  Number of units in 10 ms to wait before returning because of a
*            timeout.
*
* Return:
*  Returns CYRET_SUCCESS if no problem was encountered or returns the value
*  that best describes the problem.
*
* Global variables:
*  `$INSTANCE_NAME`_slWriteBuf - used to store received command.
*
*******************************************************************************/
cystatus `$INSTANCE_NAME`_I2CCyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    uint32 timeoutMs;

    status = CYRET_BAD_PARAM;

    if((NULL != pData) && (size > 0u))
    {
        status = CYRET_TIMEOUT;
        timeoutMs = ((uint32) 10u * timeOut); /* Convert from 10mS check to 1mS checks */

        while(0u != timeoutMs)
        {
            /* Check if the host complete write */
            if(0u != (`$INSTANCE_NAME`_I2C_SSTAT_WR_CMPLT & `$INSTANCE_NAME`_slStatus))
            {
                /* Copy to command into nootloader buffer */
                (void) memcpy((void *) pData, (const void *) `$INSTANCE_NAME`_slWriteBuf,
                                                `$INSTANCE_NAME`_I2C_MIN_UINT16(`$INSTANCE_NAME`_slWrBufIndex, size));

                /* How many bytes have been written */
                *count = (uint16) `$INSTANCE_NAME`_slWrBufIndex;

                /* Clear I2C write buffer and status */
                `$INSTANCE_NAME`_slStatus     = 0u;
                `$INSTANCE_NAME`_slWrBufIndex = 0u;

                status = CYRET_SUCCESS;
                break;
            }

            CyDelay(`$INSTANCE_NAME`_WAIT_1_MS);
            timeoutMs--;
        }
    }

    return(status);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CyBtldrCommWrite
********************************************************************************
*
* Summary:
*  Allows the caller to write data to the bootloader host.
*  The function handles polling to allow a block of data to be completely sent
*  to the host device.
*
* Parameters:
*  pData:    Pointer to the block of data to be written to the bootloader host.
*  size:     Number of bytes to be written.
*  count:    Pointer to the variable to write the number of bytes actually
*            written.
*  timeOut:  Number of units in 10 ms to wait before returning because of a
*            timeout.
*
* Return:
*  Returns CYRET_SUCCESS if no problem was encountered or returns the value
*  that best describes the problem.
*
* Global variables:
*  `$INSTANCE_NAME`_slReadBuf - used to store response.
*  `$INSTANCE_NAME`_slRdBufIndex - used to store current index within slave
*  read buffer.
*
*******************************************************************************/
cystatus `$INSTANCE_NAME`_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    uint32 timeoutMs;

    status = CYRET_BAD_PARAM;

    if((NULL != pData) && (size > 0u))
    {
        status = CYRET_TIMEOUT;
        timeoutMs = ((uint32) 10u * timeOut); /* Convert from 10mS checks to 1mS checks */

        /* Copy to response into I2C buffer */
        (void) memcpy((void *) `$INSTANCE_NAME`_slReadBuf, (const void *) pData, (uint32) size);
        *count = size; /* Buffer was copied to I2C buffer */

        /* Buffer is ready to be released to read */
        `$INSTANCE_NAME`_applyBuffer = (uint32) size;

        while(0u != timeoutMs)
        {
            /* Check if response has been read */
            if(`$INSTANCE_NAME`_slRdBufIndex == (uint32) size)
            {
                /* Makes I2C read buffer full */
                `$INSTANCE_NAME`_slRdBufSize  = 0u;
                `$INSTANCE_NAME`_slRdBufIndex = 0u;

                status = CYRET_SUCCESS;
                break;
            }

            CyDelay(`$INSTANCE_NAME`_WAIT_1_MS);
            timeoutMs--;
        }
    }

    return(status);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_I2CResposeInsert
********************************************************************************
*
* Summary:
*  Releases read buffer to be read when response is copied to the buffer and new
*  read transaction starts.
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
*  `$INSTANCE_NAME`_applyBuffer - flag to release buffer with response to be
*  read
*
*******************************************************************************/
static void `$INSTANCE_NAME`_I2CResposeInsert(void)
{
    /* INTR_SLAVE_I2C_ADDR_MATCH */
    if(`$INSTANCE_NAME`_CHECK_INTR_SLAVE_MASKED(`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH))
    {
        if(0u != `$INSTANCE_NAME`_applyBuffer)
        {
            /* The response was copied into the buffer: release buffer to the host */
            `$INSTANCE_NAME`_slRdBufSize = `$INSTANCE_NAME`_applyBuffer;
            `$INSTANCE_NAME`_applyBuffer = 0u;
        }
    }
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_I2C_BTLDR_COMM_ENABLED) */


/* [] END OF FILE */
