/*******************************************************************************
* File Name: `$INSTANCE_NAME`_UART_BOOT.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the API for the bootloader
*  communication support in SCB Component UART mode.
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

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_UART_BTLDR_COMM_ENABLED)

/***************************************
*    Private I/O Component Vars
***************************************/


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_UartCyBtldrCommStart
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in UART mode does not support bootloader communication.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_UartCyBtldrCommStart(void)
{
    ; /* Does nothing */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_UartCyBtldrCommStop
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in UART mode does not support bootloader communication.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_UartCyBtldrCommStop(void)
{
    ; /* Does nothing */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_UartCyBtldrCommReset
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in UART mode does not support bootloader communication.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_UartCyBtldrCommReset(void)
{
    ; /* Does nothing */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_UartCyBtldrCommRead
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in UART mode does not support bootloader communication.
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
cystatus `$INSTANCE_NAME`_UartCyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    status = CYRET_BAD_PARAM;

    return(status);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_UartCyBtldrCommWrite
********************************************************************************
*
* Summary:
*  This fucntion does nothing.
*  SCB in UART mode does not support bootloader communication.
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
cystatus `$INSTANCE_NAME`_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    status = CYRET_BAD_PARAM;

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_UART_BTLDR_COMM_ENABLED) */


/* [] END OF FILE */
