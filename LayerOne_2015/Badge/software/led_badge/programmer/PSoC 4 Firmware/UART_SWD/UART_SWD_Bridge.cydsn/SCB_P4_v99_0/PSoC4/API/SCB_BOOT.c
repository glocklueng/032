/*******************************************************************************
* File Name: `$INSTANCE_NAME`_BOOT.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the API for the bootloader
*  communication support in SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`.h"

#if(`$INSTANCE_NAME`_SCB_MODE_I2C_INC)
    #include "`$INSTANCE_NAME`_I2C.h"
#endif /* (`$INSTANCE_NAME`_SCB_MODE_I2C_INC) */

#if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_INC)
    #include "`$INSTANCE_NAME`_EZI2C.h"
#endif /* (`$INSTANCE_NAME`_SCB_MODE_EZI2C_INC) */

#if(`$INSTANCE_NAME`_SCB_MODE_SPI_INC || `$INSTANCE_NAME`_SCB_MODE_UART_INC)
    #include "`$INSTANCE_NAME`_SPI_UART.h"
#endif /* (`$INSTANCE_NAME`_SCB_MODE_SPI_INC || `$INSTANCE_NAME`_SCB_MODE_UART_INC) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_`$INSTANCE_NAME`) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CyBtldrCommStart
********************************************************************************
*
* Summary:
*  Calls Start function fucntion of the bootloader communication component for
*  selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_CyBtldrCommStart(void)
{
    #if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
        if(`$INSTANCE_NAME`_SCB_MODE_I2C_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_I2CCyBtldrCommStart();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_SPI_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_SpiCyBtldrCommStart();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_UART_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_UartCyBtldrCommStart();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_RUNTM_CFG)
        {
             `$INSTANCE_NAME`_EzI2CCyBtldrCommStart();
        }
        else
        {
            /* Unknown mode: do nothing */
        }
    #elif(`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG)
        `$INSTANCE_NAME`_I2CCyBtldrCommStart();

    #elif(`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG)
        `$INSTANCE_NAME`_SpiCyBtldrCommStart();

    #elif(`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG)
        `$INSTANCE_NAME`_UartCyBtldrCommStart();

    #elif(`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG)
        `$INSTANCE_NAME`_EzI2CCyBtldrCommStart();

    #else
        /* Do nothing */

    #endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CyBtldrCommStop
********************************************************************************
*
* Summary:
*  Calls Stop function fucntion of the bootloader communication component for
*  selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_CyBtldrCommStop(void)
{
    #if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
        if(`$INSTANCE_NAME`_SCB_MODE_I2C_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_I2CCyBtldrCommStop();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_SPI_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_SpiCyBtldrCommStop();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_UART_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_UartCyBtldrCommStop();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_EzI2CCyBtldrCommStop();
        }
        else
        {
            /* Unknown mode: do nothing */
        }
    #elif(`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG)
        `$INSTANCE_NAME`_I2CCyBtldrCommStop();

    #elif(`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG)
        `$INSTANCE_NAME`_SpiCyBtldrCommStop();

    #elif(`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG)
        `$INSTANCE_NAME`_UartCyBtldrCommStop();

    #elif(`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG)
        `$INSTANCE_NAME`_EzI2CCyBtldrCommStop();

    #else
        /* Do nothing */

    #endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CyBtldrCommReset
********************************************************************************
*
* Summary:
*  Calls reset function fucntion of the bootloader communication component for
*  selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_CyBtldrCommReset(void)
{
    #if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
        if(`$INSTANCE_NAME`_SCB_MODE_I2C_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_I2CCyBtldrCommReset();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_SPI_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_SpiCyBtldrCommReset();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_UART_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_UartCyBtldrCommReset();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_EzI2CCyBtldrCommReset();
        }
        else
        {
            /* Unknown mode: do nothing */
        }
    #elif(`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG)
        `$INSTANCE_NAME`_I2CCyBtldrCommReset();

    #elif(`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG)
        `$INSTANCE_NAME`_SpiCyBtldrCommReset();

    #elif(`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG)
        `$INSTANCE_NAME`_UartCyBtldrCommReset();

    #elif(`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG)
        `$INSTANCE_NAME`_EzI2CCyBtldrCommReset();

    #else
        /* Do nothing */

    #endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CyBtldrCommRead
********************************************************************************
*
* Summary:
*  Calls read fucntion of the bootloader communication component for selected
*  mode.
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
cystatus `$INSTANCE_NAME`_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    #if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
        if(`$INSTANCE_NAME`_SCB_MODE_I2C_RUNTM_CFG)
        {
            status = `$INSTANCE_NAME`_I2CCyBtldrCommRead(pData, size, count, timeOut);
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_SPI_RUNTM_CFG)
        {
            status = `$INSTANCE_NAME`_SpiCyBtldrCommRead(pData, size, count, timeOut);
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_UART_RUNTM_CFG)
        {
            status = `$INSTANCE_NAME`_UartCyBtldrCommRead(pData, size, count, timeOut);
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            status = `$INSTANCE_NAME`_EzI2CCyBtldrCommRead(pData, size, count, timeOut);
        }
        else
        {
            status = CYRET_INVALID_STATE; /* Unknown mode: return status */
        }

    #elif(`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG)
        status = `$INSTANCE_NAME`_I2CCyBtldrCommRead(pData, size, count, timeOut);

    #elif(`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG)
        status = `$INSTANCE_NAME`_SpiCyBtldrCommRead(pData, size, count, timeOut);

    #elif(`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG)
        status = `$INSTANCE_NAME`_UartCyBtldrCommRead(pData, size, count, timeOut);

    #elif(`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG)
        status = `$INSTANCE_NAME`_EzI2CCyBtldrCommRead(pData, size, count, timeOut);

    #else
        status = CYRET_INVALID_STATE; /* Unknown mode: return status */

    #endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */

    return(status);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CyBtldrCommWrite
********************************************************************************
*
* Summary:
*  Calls write fucntion of the bootloader communication component for selected
*  mode.
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
cystatus `$INSTANCE_NAME`_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    #if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
        if(`$INSTANCE_NAME`_SCB_MODE_I2C_RUNTM_CFG)
        {
            status = `$INSTANCE_NAME`_I2CCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_SPI_RUNTM_CFG)
        {
            status = `$INSTANCE_NAME`_SpiCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_UART_RUNTM_CFG)
        {
            status = `$INSTANCE_NAME`_UartCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            status = `$INSTANCE_NAME`_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else
        {
            status = CYRET_INVALID_STATE; /* Unknown mode: return status */
        }
    #elif(`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG)
        status = `$INSTANCE_NAME`_I2CCyBtldrCommWrite(pData, size, count, timeOut);

    #elif(`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG)
        status = `$INSTANCE_NAME`_SpiCyBtldrCommWrite(pData, size, count, timeOut);

    #elif(`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG)
        status = `$INSTANCE_NAME`_UartCyBtldrCommWrite(pData, size, count, timeOut);

    #elif(`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG)
        status = `$INSTANCE_NAME`_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);

    #else
        status = CYRET_INVALID_STATE; /* Unknown mode: return status */

    #endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_`$INSTANCE_NAME`) || \
                                                    (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface)) */


/* [] END OF FILE */
