/*******************************************************************************
* File Name: `$INSTANCE_NAME_EZI2C_PVT`.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in _EZI2C mode.
*  Please, do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_EZI2C_PVT_`$INSTANCE_NAME`_H)
#define CY_SCB_EZI2C_PVT_`$INSTANCE_NAME`_H

#include "`$INSTANCE_NAME`_EZI2C.h"


/***************************************
*      EZI2C Private Vars
***************************************/

extern volatile uint8 `$INSTANCE_NAME`_curStatus; /* Status byte */
extern uint8 `$INSTANCE_NAME`_fsmState;           /* FSM state   */

/* Variables intended to be used with Buffer1: Primary slave address */
extern volatile uint8 * `$INSTANCE_NAME`_dataBuffer1; /* Pointer to data buffer1       */
extern uint16 `$INSTANCE_NAME`_bufSizeBuf1; /* Size of buffer1 in bytes      */
extern uint16 `$INSTANCE_NAME`_protectBuf1; /* Start index of write protected area buffer1 */
extern `$EzI2cBitWidthReplacementString` `$INSTANCE_NAME`_offsetBuf1; /* Current offset within buffer1 */
extern `$EzI2cBitWidthReplacementString` `$INSTANCE_NAME`_indexBuf1;  /* Current index within buffer1  */


#if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST)
    extern uint8 `$INSTANCE_NAME`_addrBuf1; /* Primary slave address. Used for software comparision   */
    extern uint8 `$INSTANCE_NAME`_addrBuf2; /* Secondary slave address. Used for software comparision */

    /* Variables intended to be used with Buffer1: Primary slave address */
    extern volatile uint8 * `$INSTANCE_NAME`_dataBuffer2; /* Pointer to data buffer2       */
    extern uint16 `$INSTANCE_NAME`_bufSizeBuf2; /* Size of buffer2 in bytes      */
    extern uint16 `$INSTANCE_NAME`_protectBuf2; /* Start index of write protected area buf2 */
    extern `$EzI2cBitWidthReplacementString` `$INSTANCE_NAME`_offsetBuf2; /* Current offset within buffer2 */
    extern `$EzI2cBitWidthReplacementString` `$INSTANCE_NAME`_indexBuf2;  /* Current index within buffer2  */
#endif /* (`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG)
    void `$INSTANCE_NAME`_EzI2CInit(void);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG) */

void `$INSTANCE_NAME`_EzI2CStop(void);
#if(`$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST)
    void `$INSTANCE_NAME`_EzI2CSaveConfig(void);
    void `$INSTANCE_NAME`_EzI2CRestoreConfig(void);
#endif /* (`$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST) */

#endif /* (CY_SCB__EZI2C_PVT_`$INSTANCE_NAME`_H) */


/* [] END OF FILE */
