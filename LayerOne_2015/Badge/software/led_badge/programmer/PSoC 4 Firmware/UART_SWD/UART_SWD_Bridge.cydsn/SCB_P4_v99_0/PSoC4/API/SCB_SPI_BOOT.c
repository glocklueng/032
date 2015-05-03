/*******************************************************************************
* File Name: `$INSTANCE_NAME`_SPI_BOOT.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the API for the bootloader
*  communication support in SCB Component SPI mode.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`_SPI_UART.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_SPI_BTLDR_COMM_ENABLED)

/***************************************
*    Private I/O Component Vars
***************************************/


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SpiCyBtldrCommStart
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in SPI mode does not support bootloader communication.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_SpiCyBtldrCommStart(void)
{
    ; /* Does nothing */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SpiCyBtldrCommStop
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in SPI mode does not support bootloader communication.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_SpiCyBtldrCommStop(void)
{
    ; /* Does nothing */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SpiCyBtldrCommReset
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in SPI mode does not support bootloader communication.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_SpiCyBtldrCommReset(void)
{
    ; /* Does nothing */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SpiCyBtldrCommRead
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in SPI mode does not support bootloader communication.
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
*******************************************************************************/
cystatus `$INSTANCE_NAME`_SpiCyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    status = CYRET_BAD_PARAM;

    return(status);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SpiCyBtldrCommWrite
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in SPI mode does not support bootloader communication.
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
*******************************************************************************/
cystatus `$INSTANCE_NAME`_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    status = CYRET_BAD_PARAM;

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_EZSPI_BTLDR_COMM_ENABLED) */


/* [] END OF FILE */
