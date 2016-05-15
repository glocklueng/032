/*******************************************************************************
* File Name: .h
* Version 2.0
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_uWIFI_H)
#define CY_SCB_PVT_uWIFI_H

#include "uWIFI.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define uWIFI_SetI2CExtClkInterruptMode(interruptMask) uWIFI_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define uWIFI_ClearI2CExtClkInterruptSource(interruptMask) uWIFI_CLEAR_INTR_I2C_EC(interruptMask)
#define uWIFI_GetI2CExtClkInterruptSource()                (uWIFI_INTR_I2C_EC_REG)
#define uWIFI_GetI2CExtClkInterruptMode()                  (uWIFI_INTR_I2C_EC_MASK_REG)
#define uWIFI_GetI2CExtClkInterruptSourceMasked()          (uWIFI_INTR_I2C_EC_MASKED_REG)

#if (!uWIFI_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define uWIFI_SetSpiExtClkInterruptMode(interruptMask) \
                                                                uWIFI_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define uWIFI_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                uWIFI_CLEAR_INTR_SPI_EC(interruptMask)
    #define uWIFI_GetExtSpiClkInterruptSource()                 (uWIFI_INTR_SPI_EC_REG)
    #define uWIFI_GetExtSpiClkInterruptMode()                   (uWIFI_INTR_SPI_EC_MASK_REG)
    #define uWIFI_GetExtSpiClkInterruptSourceMasked()           (uWIFI_INTR_SPI_EC_MASKED_REG)
#endif /* (!uWIFI_CY_SCBIP_V1) */

#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void uWIFI_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if !defined (CY_REMOVE_uWIFI_CUSTOM_INTR_HANDLER)
    extern cyisraddress uWIFI_customIntrHandler;
#endif /* !defined (CY_REMOVE_uWIFI_CUSTOM_INTR_HANDLER) */

extern uWIFI_BACKUP_STRUCT uWIFI_backup;

#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 uWIFI_scbMode;
    extern uint8 uWIFI_scbEnableWake;
    extern uint8 uWIFI_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 uWIFI_mode;
    extern uint8 uWIFI_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * uWIFI_rxBuffer;
    extern uint8   uWIFI_rxDataBits;
    extern uint32  uWIFI_rxBufferSize;

    extern volatile uint8 * uWIFI_txBuffer;
    extern uint8   uWIFI_txDataBits;
    extern uint32  uWIFI_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 uWIFI_numberOfAddr;
    extern uint8 uWIFI_subAddrSize;
#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*        Conditional Macro
****************************************/

#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define uWIFI_SCB_MODE_I2C_RUNTM_CFG     (uWIFI_SCB_MODE_I2C      == uWIFI_scbMode)
    #define uWIFI_SCB_MODE_SPI_RUNTM_CFG     (uWIFI_SCB_MODE_SPI      == uWIFI_scbMode)
    #define uWIFI_SCB_MODE_UART_RUNTM_CFG    (uWIFI_SCB_MODE_UART     == uWIFI_scbMode)
    #define uWIFI_SCB_MODE_EZI2C_RUNTM_CFG   (uWIFI_SCB_MODE_EZI2C    == uWIFI_scbMode)
    #define uWIFI_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (uWIFI_SCB_MODE_UNCONFIG == uWIFI_scbMode)

    /* Defines wakeup enable */
    #define uWIFI_SCB_WAKE_ENABLE_CHECK       (0u != uWIFI_scbEnableWake)
#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_SCB_PINS_NUMBER    (7u)
#else
    #define uWIFI_SCB_PINS_NUMBER    (2u)
#endif /* (!uWIFI_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_uWIFI_H) */


/* [] END OF FILE */
