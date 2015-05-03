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

#if !defined(CY_SCB_I2C_PVT_`$INSTANCE_NAME`_H)
#define CY_SCB_I2C_PVT_`$INSTANCE_NAME`_H

#include "`$INSTANCE_NAME`_I2C.h"


/***************************************
*     Private Global Vars
***************************************/

extern volatile uint8 `$INSTANCE_NAME`_state; /* Current state of I2C FSM */

#if(`$INSTANCE_NAME`_I2C_SLAVE_CONST)
    extern volatile uint8 `$INSTANCE_NAME`_slStatus;          /* Slave Status */

    /* Receive buffer variables */
    extern volatile uint8 * `$INSTANCE_NAME`_slWrBufPtr;      /* Pointer to Receive buffer  */
    extern volatile uint32  `$INSTANCE_NAME`_slWrBufSize;     /* Slave Receive buffer size  */
    extern volatile uint32  `$INSTANCE_NAME`_slWrBufIndex;    /* Slave Receive buffer Index */

    /* Transmit buffer variables */
    extern volatile uint8 * `$INSTANCE_NAME`_slRdBufPtr;      /* Pointer to Transmit buffer  */
    extern volatile uint32  `$INSTANCE_NAME`_slRdBufSize;     /* Slave Transmit buffer size  */
    extern volatile uint32  `$INSTANCE_NAME`_slRdBufIndex;    /* Slave Transmit buffer Index */
    extern volatile uint32  `$INSTANCE_NAME`_slRdBufIndexTmp; /* Slave Transmit buffer Index Tmp */
    extern volatile uint8   `$INSTANCE_NAME`_slOverFlowCount; /* Slave Transmit Overflow counter */
#endif /* (`$INSTANCE_NAME`_I2C_SLAVE_CONST) */

#if(`$INSTANCE_NAME`_I2C_MASTER_CONST)
    extern volatile uint16 `$INSTANCE_NAME`_mstrStatus;      /* Master Status byte  */
    extern volatile uint8  `$INSTANCE_NAME`_mstrControl;     /* Master Control byte */

    /* Receive buffer variables */
    extern volatile uint8 * `$INSTANCE_NAME`_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint32  `$INSTANCE_NAME`_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint32  `$INSTANCE_NAME`_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Transmit buffer variables */
    extern volatile uint8 * `$INSTANCE_NAME`_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint32  `$INSTANCE_NAME`_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint32  `$INSTANCE_NAME`_mstrWrBufIndex; /* Master Write buffer Index      */
    extern volatile uint32  `$INSTANCE_NAME`_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */
#endif /* (`$INSTANCE_NAME`_I2C_MASTER_CONST) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG)
    void `$INSTANCE_NAME`_I2CInit(void);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG) */

void `$INSTANCE_NAME`_I2CStop(void);
void `$INSTANCE_NAME`_I2CSaveConfig(void);
void `$INSTANCE_NAME`_I2CRestoreConfig(void);

#if(`$INSTANCE_NAME`_I2C_MASTER_CONST)
    void `$INSTANCE_NAME`_I2CReStartGeneration(void);
#endif /* (`$INSTANCE_NAME`_I2C_MASTER_CONST) */

#endif /* (CY_SCB_I2C_PVT_`$INSTANCE_NAME`_H) */


/* [] END OF FILE */
