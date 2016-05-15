/*******************************************************************************
* File Name: uWIFI_PM.c
* Version 2.0
*
* Description:
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "uWIFI.h"
#include "uWIFI_PVT.h"

#if(uWIFI_SCB_MODE_I2C_INC)
    #include "uWIFI_I2C_PVT.h"
#endif /* (uWIFI_SCB_MODE_I2C_INC) */

#if(uWIFI_SCB_MODE_EZI2C_INC)
    #include "uWIFI_EZI2C_PVT.h"
#endif /* (uWIFI_SCB_MODE_EZI2C_INC) */

#if(uWIFI_SCB_MODE_SPI_INC || uWIFI_SCB_MODE_UART_INC)
    #include "uWIFI_SPI_UART_PVT.h"
#endif /* (uWIFI_SCB_MODE_SPI_INC || uWIFI_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG || \
   (uWIFI_SCB_MODE_I2C_CONST_CFG   && (!uWIFI_I2C_WAKE_ENABLE_CONST))   || \
   (uWIFI_SCB_MODE_EZI2C_CONST_CFG && (!uWIFI_EZI2C_WAKE_ENABLE_CONST)) || \
   (uWIFI_SCB_MODE_SPI_CONST_CFG   && (!uWIFI_SPI_WAKE_ENABLE_CONST))   || \
   (uWIFI_SCB_MODE_UART_CONST_CFG  && (!uWIFI_UART_WAKE_ENABLE_CONST)))

    uWIFI_BACKUP_STRUCT uWIFI_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: uWIFI_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component to enter Deep Sleep.
*  The "Enable wakeup from Sleep Mode" selection has an influence on
*  this function implementation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uWIFI_Sleep(void)
{
#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)

    if(uWIFI_SCB_WAKE_ENABLE_CHECK)
    {
        if(uWIFI_SCB_MODE_I2C_RUNTM_CFG)
        {
            uWIFI_I2CSaveConfig();
        }
        else if(uWIFI_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            uWIFI_EzI2CSaveConfig();
        }
    #if(!uWIFI_CY_SCBIP_V1)
        else if(uWIFI_SCB_MODE_SPI_RUNTM_CFG)
        {
            uWIFI_SpiSaveConfig();
        }
        else if(uWIFI_SCB_MODE_UART_RUNTM_CFG)
        {
            uWIFI_UartSaveConfig();
        }
    #endif /* (!uWIFI_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        uWIFI_backup.enableState = (uint8) uWIFI_GET_CTRL_ENABLED;

        if(0u != uWIFI_backup.enableState)
        {
            uWIFI_Stop();
        }
    }

#else

    #if (uWIFI_SCB_MODE_I2C_CONST_CFG && uWIFI_I2C_WAKE_ENABLE_CONST)
        uWIFI_I2CSaveConfig();

    #elif (uWIFI_SCB_MODE_EZI2C_CONST_CFG && uWIFI_EZI2C_WAKE_ENABLE_CONST)
        uWIFI_EzI2CSaveConfig();

    #elif (uWIFI_SCB_MODE_SPI_CONST_CFG && uWIFI_SPI_WAKE_ENABLE_CONST)
        uWIFI_SpiSaveConfig();

    #elif (uWIFI_SCB_MODE_UART_CONST_CFG && uWIFI_UART_WAKE_ENABLE_CONST)
        uWIFI_UartSaveConfig();

    #else

        uWIFI_backup.enableState = (uint8) uWIFI_GET_CTRL_ENABLED;

        if(0u != uWIFI_backup.enableState)
        {
            uWIFI_Stop();
        }

    #endif /* defined (uWIFI_SCB_MODE_I2C_CONST_CFG) && (uWIFI_I2C_WAKE_ENABLE_CONST) */

#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: uWIFI_Wakeup
********************************************************************************
*
* Summary:
*  Prepares the component for the Active mode operation after exiting
*  Deep Sleep. The "Enable wakeup from Sleep Mode" option has an influence
*  on this function implementation.
*  This function should not be called after exiting Sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uWIFI_Wakeup(void)
{
#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)

    if(uWIFI_SCB_WAKE_ENABLE_CHECK)
    {
        if(uWIFI_SCB_MODE_I2C_RUNTM_CFG)
        {
            uWIFI_I2CRestoreConfig();
        }
        else if(uWIFI_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            uWIFI_EzI2CRestoreConfig();
        }
    #if(!uWIFI_CY_SCBIP_V1)
        else if(uWIFI_SCB_MODE_SPI_RUNTM_CFG)
        {
            uWIFI_SpiRestoreConfig();
        }
        else if(uWIFI_SCB_MODE_UART_RUNTM_CFG)
        {
            uWIFI_UartRestoreConfig();
        }
    #endif /* (!uWIFI_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != uWIFI_backup.enableState)
        {
            uWIFI_Enable();
        }
    }

#else

    #if (uWIFI_SCB_MODE_I2C_CONST_CFG  && uWIFI_I2C_WAKE_ENABLE_CONST)
        uWIFI_I2CRestoreConfig();

    #elif (uWIFI_SCB_MODE_EZI2C_CONST_CFG && uWIFI_EZI2C_WAKE_ENABLE_CONST)
        uWIFI_EzI2CRestoreConfig();

    #elif (uWIFI_SCB_MODE_SPI_CONST_CFG && uWIFI_SPI_WAKE_ENABLE_CONST)
        uWIFI_SpiRestoreConfig();

    #elif (uWIFI_SCB_MODE_UART_CONST_CFG && uWIFI_UART_WAKE_ENABLE_CONST)
        uWIFI_UartRestoreConfig();

    #else

        if(0u != uWIFI_backup.enableState)
        {
            uWIFI_Enable();
        }

    #endif /* (uWIFI_I2C_WAKE_ENABLE_CONST) */

#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
