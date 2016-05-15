/*******************************************************************************
* File Name: uWIFI_BOOT.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the bootloader
*  communication support in the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "uWIFI_BOOT.h"


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_BTLDR_COMM_MODE_ENABLED)

/*******************************************************************************
* Function Name: uWIFI_CyBtldrCommStart
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommStart function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uWIFI_CyBtldrCommStart(void)
{
    #if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
        if(uWIFI_SCB_MODE_I2C_RUNTM_CFG)
        {
            uWIFI_I2CCyBtldrCommStart();
        }
        else if(uWIFI_SCB_MODE_SPI_RUNTM_CFG)
        {
            uWIFI_SpiCyBtldrCommStart();
        }
        else if(uWIFI_SCB_MODE_UART_RUNTM_CFG)
        {
            uWIFI_UartCyBtldrCommStart();
        }
        else if(uWIFI_SCB_MODE_EZI2C_RUNTM_CFG)
        {
             uWIFI_EzI2CCyBtldrCommStart();
        }
        else
        {
            /* Unknown mode */
        }
    #elif(uWIFI_SCB_MODE_I2C_CONST_CFG)
        uWIFI_I2CCyBtldrCommStart();

    #elif(uWIFI_SCB_MODE_SPI_CONST_CFG)
        uWIFI_SpiCyBtldrCommStart();

    #elif(uWIFI_SCB_MODE_UART_CONST_CFG)
        uWIFI_UartCyBtldrCommStart();

    #elif(uWIFI_SCB_MODE_EZI2C_CONST_CFG)
        uWIFI_EzI2CCyBtldrCommStart();

    #else
        /* Unknown mode */

    #endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: uWIFI_CyBtldrCommStop
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommStop function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uWIFI_CyBtldrCommStop(void)
{
    #if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
        if(uWIFI_SCB_MODE_I2C_RUNTM_CFG)
        {
            uWIFI_I2CCyBtldrCommStop();
        }
        else if(uWIFI_SCB_MODE_SPI_RUNTM_CFG)
        {
            uWIFI_SpiCyBtldrCommStop();
        }
        else if(uWIFI_SCB_MODE_UART_RUNTM_CFG)
        {
            uWIFI_UartCyBtldrCommStop();
        }
        else if(uWIFI_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            uWIFI_EzI2CCyBtldrCommStop();
        }
        else
        {
            /* Unknown mode */
        }
    #elif(uWIFI_SCB_MODE_I2C_CONST_CFG)
        uWIFI_I2CCyBtldrCommStop();

    #elif(uWIFI_SCB_MODE_SPI_CONST_CFG)
        uWIFI_SpiCyBtldrCommStop();

    #elif(uWIFI_SCB_MODE_UART_CONST_CFG)
        uWIFI_UartCyBtldrCommStop();

    #elif(uWIFI_SCB_MODE_EZI2C_CONST_CFG)
        uWIFI_EzI2CCyBtldrCommStop();

    #else
        /* Unknown mode */

    #endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: uWIFI_CyBtldrCommReset
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommReset function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uWIFI_CyBtldrCommReset(void)
{
    #if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
        if(uWIFI_SCB_MODE_I2C_RUNTM_CFG)
        {
            uWIFI_I2CCyBtldrCommReset();
        }
        else if(uWIFI_SCB_MODE_SPI_RUNTM_CFG)
        {
            uWIFI_SpiCyBtldrCommReset();
        }
        else if(uWIFI_SCB_MODE_UART_RUNTM_CFG)
        {
            uWIFI_UartCyBtldrCommReset();
        }
        else if(uWIFI_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            uWIFI_EzI2CCyBtldrCommReset();
        }
        else
        {
            /* Unknown mode */
        }
    #elif(uWIFI_SCB_MODE_I2C_CONST_CFG)
        uWIFI_I2CCyBtldrCommReset();

    #elif(uWIFI_SCB_MODE_SPI_CONST_CFG)
        uWIFI_SpiCyBtldrCommReset();

    #elif(uWIFI_SCB_MODE_UART_CONST_CFG)
        uWIFI_UartCyBtldrCommReset();

    #elif(uWIFI_SCB_MODE_EZI2C_CONST_CFG)
        uWIFI_EzI2CCyBtldrCommReset();

    #else
        /* Unknown mode */

    #endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: uWIFI_CyBtldrCommRead
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommRead function of the bootloader communication
*  component for the selected mode.
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
cystatus uWIFI_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    #if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
        if(uWIFI_SCB_MODE_I2C_RUNTM_CFG)
        {
            status = uWIFI_I2CCyBtldrCommRead(pData, size, count, timeOut);
        }
        else if(uWIFI_SCB_MODE_SPI_RUNTM_CFG)
        {
            status = uWIFI_SpiCyBtldrCommRead(pData, size, count, timeOut);
        }
        else if(uWIFI_SCB_MODE_UART_RUNTM_CFG)
        {
            status = uWIFI_UartCyBtldrCommRead(pData, size, count, timeOut);
        }
        else if(uWIFI_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            status = uWIFI_EzI2CCyBtldrCommRead(pData, size, count, timeOut);
        }
        else
        {
            status = CYRET_INVALID_STATE; /* Unknown mode: return status */
        }

    #elif(uWIFI_SCB_MODE_I2C_CONST_CFG)
        status = uWIFI_I2CCyBtldrCommRead(pData, size, count, timeOut);

    #elif(uWIFI_SCB_MODE_SPI_CONST_CFG)
        status = uWIFI_SpiCyBtldrCommRead(pData, size, count, timeOut);

    #elif(uWIFI_SCB_MODE_UART_CONST_CFG)
        status = uWIFI_UartCyBtldrCommRead(pData, size, count, timeOut);

    #elif(uWIFI_SCB_MODE_EZI2C_CONST_CFG)
        status = uWIFI_EzI2CCyBtldrCommRead(pData, size, count, timeOut);

    #else
        status = CYRET_INVALID_STATE; /* Unknown mode: return status */

    #endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */

    return(status);
}


/*******************************************************************************
* Function Name: uWIFI_CyBtldrCommWrite
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommWrite  function of the bootloader communication
*  component for the selected mode.
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
cystatus uWIFI_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    #if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
        if(uWIFI_SCB_MODE_I2C_RUNTM_CFG)
        {
            status = uWIFI_I2CCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else if(uWIFI_SCB_MODE_SPI_RUNTM_CFG)
        {
            status = uWIFI_SpiCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else if(uWIFI_SCB_MODE_UART_RUNTM_CFG)
        {
            status = uWIFI_UartCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else if(uWIFI_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            status = uWIFI_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else
        {
            status = CYRET_INVALID_STATE; /* Unknown mode */
        }
    #elif(uWIFI_SCB_MODE_I2C_CONST_CFG)
        status = uWIFI_I2CCyBtldrCommWrite(pData, size, count, timeOut);

    #elif(uWIFI_SCB_MODE_SPI_CONST_CFG)
        status = uWIFI_SpiCyBtldrCommWrite(pData, size, count, timeOut);

    #elif(uWIFI_SCB_MODE_UART_CONST_CFG)
        status = uWIFI_UartCyBtldrCommWrite(pData, size, count, timeOut);

    #elif(uWIFI_SCB_MODE_EZI2C_CONST_CFG)
        status = uWIFI_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);

    #else
        status = CYRET_INVALID_STATE; /* Unknown mode */

    #endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_BTLDR_COMM_MODE_ENABLED) */


/* [] END OF FILE */
