/*******************************************************************************
* File Name: `$INSTANCE_NAME_I2C_PVT`.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in I2C mode.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_`$INSTANCE_NAME`_H)
#define CY_SCB_PVT_`$INSTANCE_NAME`_H

#include "`$INSTANCE_NAME`.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define `$INSTANCE_NAME`_SetI2CExtClkInterruptMode(interruptMask) `$INSTANCE_NAME`_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define `$INSTANCE_NAME`_ClearI2CExtClkInterruptSource(interruptMask) `$INSTANCE_NAME`_CLEAR_INTR_I2C_EC(interruptMask)
#define `$INSTANCE_NAME`_GetI2CExtClkInterruptSource()                (`$INSTANCE_NAME`_INTR_I2C_EC_REG)
#define `$INSTANCE_NAME`_GetI2CExtClkInterruptMode()                  (`$INSTANCE_NAME`_INTR_I2C_EC_MASK_REG)
#define `$INSTANCE_NAME`_GetI2CExtClkInterruptSourceMasked()          (`$INSTANCE_NAME`_INTR_I2C_EC_MASKED_REG)

/* APIs to service INTR_SPI_EC register */
#define `$INSTANCE_NAME`_SetSpiExtClkInterruptMode(interruptMask) `$INSTANCE_NAME`_WRITE_INTR_SPI_EC_MASK(interruptMask)
#define `$INSTANCE_NAME`_ClearSpiExtClkInterruptSource(interruptMask) `$INSTANCE_NAME`_CLEAR_INTR_SPI_EC(interruptMask)
#define `$INSTANCE_NAME`_GetExtSpiClkInterruptSource()                 (`$INSTANCE_NAME`_INTR_SPI_EC_REG)
#define `$INSTANCE_NAME`_GetExtSpiClkInterruptMode()                   (`$INSTANCE_NAME`_INTR_SPI_EC_MASK_REG)
#define `$INSTANCE_NAME`_GetExtSpiClkInterruptSourceMasked()           (`$INSTANCE_NAME`_INTR_SPI_EC_MASKED_REG)

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void `$INSTANCE_NAME`_SetPins(uint32 mode, uint32 subMode, uint32 uartTxRx);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */

void `$INSTANCE_NAME`_DisableTxPinsInputBuffer(void);
void `$INSTANCE_NAME`_EnableTxPinsInputBuffer(void);


/**********************************
*     Vars with External Linkage
**********************************/

extern cyisraddress `$INSTANCE_NAME`_customIntrHandler;
extern `$INSTANCE_NAME`_BACKUP_STRUCT `$INSTANCE_NAME`_backup;

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common config vars */
    extern uint8 `$INSTANCE_NAME`_scbMode;
    extern uint8 `$INSTANCE_NAME`_scbEnableWake;
    extern uint8 `$INSTANCE_NAME`_scbEnableIntr;

    /* I2C config vars */
    extern uint8 `$INSTANCE_NAME`_mode;
    extern uint8 `$INSTANCE_NAME`_acceptAddr;

    /* SPI/UART config vars */
    extern volatile uint8 * `$INSTANCE_NAME`_rxBuffer;
    extern uint8   `$INSTANCE_NAME`_rxDataBits;
    extern uint32  `$INSTANCE_NAME`_rxBufferSize;

    extern volatile uint8 * `$INSTANCE_NAME`_txBuffer;
    extern uint8   `$INSTANCE_NAME`_txDataBits;
    extern uint32  `$INSTANCE_NAME`_txBufferSize;

    /* EZI2C config vars */
    extern uint8 `$INSTANCE_NAME`_numberOfAddr;
    extern uint8 `$INSTANCE_NAME`_subAddrSize;
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */

#endif /* (CY_SCB_PVT_`$INSTANCE_NAME`_H) */


/* [] END OF FILE */
