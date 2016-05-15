/*******************************************************************************
* File Name: uWIFI.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_uWIFI_H)
#define CY_SCB_uWIFI_H

#include <cydevice_trm.h>
#include <cyfitter.h>
#include <cytypes.h>
#include <CyLib.h>

#define uWIFI_CY_SCBIP_V0    (CYIPBLOCK_m0s8scb_VERSION == 0u)
#define uWIFI_CY_SCBIP_V1    (CYIPBLOCK_m0s8scb_VERSION == 1u)
#define uWIFI_CY_SCBIP_V2    (CYIPBLOCK_m0s8scb_VERSION >= 2u)

#define uWIFI_SCB_MODE                     (4u)

/* SCB modes enum */
#define uWIFI_SCB_MODE_I2C                 (0x01u)
#define uWIFI_SCB_MODE_SPI                 (0x02u)
#define uWIFI_SCB_MODE_UART                (0x04u)
#define uWIFI_SCB_MODE_EZI2C               (0x08u)
#define uWIFI_SCB_MODE_UNCONFIG            (0xFFu)

/* Condition compilation depends on operation mode: Unconfigured implies apply to all modes */
#define uWIFI_SCB_MODE_I2C_CONST_CFG       (uWIFI_SCB_MODE_I2C       == uWIFI_SCB_MODE)
#define uWIFI_SCB_MODE_SPI_CONST_CFG       (uWIFI_SCB_MODE_SPI       == uWIFI_SCB_MODE)
#define uWIFI_SCB_MODE_UART_CONST_CFG      (uWIFI_SCB_MODE_UART      == uWIFI_SCB_MODE)
#define uWIFI_SCB_MODE_EZI2C_CONST_CFG     (uWIFI_SCB_MODE_EZI2C     == uWIFI_SCB_MODE)
#define uWIFI_SCB_MODE_UNCONFIG_CONST_CFG  (uWIFI_SCB_MODE_UNCONFIG  == uWIFI_SCB_MODE)

/* Condition compilation for includes */
#define uWIFI_SCB_MODE_I2C_INC      (0u !=(uWIFI_SCB_MODE_I2C   & uWIFI_SCB_MODE))
#define uWIFI_SCB_MODE_EZI2C_INC    (0u !=(uWIFI_SCB_MODE_EZI2C & uWIFI_SCB_MODE))
#if(!uWIFI_CY_SCBIP_V1)
    #define uWIFI_SCB_MODE_SPI_INC  (0u !=(uWIFI_SCB_MODE_SPI   & uWIFI_SCB_MODE))
    #define uWIFI_SCB_MODE_UART_INC (0u !=(uWIFI_SCB_MODE_UART  & uWIFI_SCB_MODE))
#else
    #define uWIFI_SCB_MODE_SPI_INC  (0u)
    #define uWIFI_SCB_MODE_UART_INC (0u)
#endif /* (!uWIFI_CY_SCBIP_V1) */

/* Interrupts remove options */
#define uWIFI_REMOVE_SCB_IRQ             (0u)
#define uWIFI_SCB_IRQ_INTERNAL           (0u == uWIFI_REMOVE_SCB_IRQ)

#define uWIFI_REMOVE_UART_RX_WAKEUP_IRQ  (1u)
#define uWIFI_UART_RX_WAKEUP_IRQ         (0u == uWIFI_REMOVE_UART_RX_WAKEUP_IRQ)

/* SCB interrupt enum */
#define uWIFI_SCB_INTR_MODE_NONE     (0u)
#define uWIFI_SCB_INTR_MODE_INTERNAL (1u)
#define uWIFI_SCB_INTR_MODE_EXTERNAL (2u)

/* Internal clock remove option */
#define uWIFI_REMOVE_SCB_CLK     (0u)
#define uWIFI_SCB_CLK_INTERNAL   (0u == uWIFI_REMOVE_SCB_CLK)


/***************************************
*       Includes
****************************************/

#include "uWIFI_PINS.h"

#if (uWIFI_SCB_CLK_INTERNAL)
    #include "uWIFI_SCBCLK.h"
#endif /* (uWIFI_SCB_CLK_INTERNAL) */


/***************************************
*       Type Definitions
***************************************/

typedef struct
{
    uint8 enableState;
} uWIFI_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

/* Start and Stop APIs */
void uWIFI_Init(void);
void uWIFI_Enable(void);
void uWIFI_Start(void);
void uWIFI_Stop(void);

/* Sleep and Wakeup APis */
void uWIFI_Sleep(void);
void uWIFI_Wakeup(void);

/* Custom interrupt handler */
void uWIFI_SetCustomInterruptHandler(cyisraddress func);

/* Interface to internal interrupt component */
#if (uWIFI_SCB_IRQ_INTERNAL)
    #define uWIFI_EnableInt()        CyIntEnable      (uWIFI_ISR_NUMBER)
    #define uWIFI_DisableInt()       CyIntDisable     (uWIFI_ISR_NUMBER)
    #define uWIFI_ClearPendingInt()  CyIntClearPending(uWIFI_ISR_NUMBER)
#endif /* (uWIFI_SCB_IRQ_INTERNAL) */

#if (uWIFI_UART_RX_WAKEUP_IRQ)
    #define uWIFI_RxWakeEnableInt()        CyIntEnable      (uWIFI_RX_WAKE_ISR_NUMBER)
    #define uWIFI_RxWakeDisableInt()       CyIntDisable     (uWIFI_RX_WAKE_ISR_NUMBER)
    #define uWIFI_RxWakeClearPendingInt()  CyIntClearPending(uWIFI_RX_WAKE_ISR_NUMBER)
#endif /* (uWIFI_UART_RX_WAKEUP_IRQ) */

/* Get interrupt cause */
#define uWIFI_GetInterruptCause()    (uWIFI_INTR_CAUSE_REG)

/* APIs to service INTR_RX register */
#define uWIFI_SetRxInterruptMode(interruptMask)     uWIFI_WRITE_INTR_RX_MASK(interruptMask)
#define uWIFI_ClearRxInterruptSource(interruptMask) uWIFI_CLEAR_INTR_RX(interruptMask)
#define uWIFI_SetRxInterrupt(interruptMask)         uWIFI_SET_INTR_RX(interruptMask)
#define uWIFI_GetRxInterruptSource()                (uWIFI_INTR_RX_REG)
#define uWIFI_GetRxInterruptMode()                  (uWIFI_INTR_RX_MASK_REG)
#define uWIFI_GetRxInterruptSourceMasked()          (uWIFI_INTR_RX_MASKED_REG)
void uWIFI_SetRxFifoLevel(uint32 level);

/* APIs to service INTR_TX register */
#define uWIFI_SetTxInterruptMode(interruptMask)     uWIFI_WRITE_INTR_TX_MASK(interruptMask)
#define uWIFI_ClearTxInterruptSource(interruptMask) uWIFI_CLEAR_INTR_TX(interruptMask)
#define uWIFI_SetTxInterrupt(interruptMask)         uWIFI_SET_INTR_TX(interruptMask)
#define uWIFI_GetTxInterruptSource()                (uWIFI_INTR_TX_REG)
#define uWIFI_GetTxInterruptMode()                  (uWIFI_INTR_TX_MASK_REG)
#define uWIFI_GetTxInterruptSourceMasked()          (uWIFI_INTR_TX_MASKED_REG)
void uWIFI_SetTxFifoLevel(uint32 level);

/* APIs to service INTR_MASTER register */
#define uWIFI_SetMasterInterruptMode(interruptMask)     uWIFI_WRITE_INTR_MASTER_MASK(interruptMask)
#define uWIFI_ClearMasterInterruptSource(interruptMask) uWIFI_CLEAR_INTR_MASTER(interruptMask)
#define uWIFI_SetMasterInterrupt(interruptMask)         uWIFI_SET_INTR_MASTER(interruptMask)
#define uWIFI_GetMasterInterruptSource()                (uWIFI_INTR_MASTER_REG)
#define uWIFI_GetMasterInterruptMode()                  (uWIFI_INTR_MASTER_MASK_REG)
#define uWIFI_GetMasterInterruptSourceMasked()          (uWIFI_INTR_MASTER_MASKED_REG)

/* APIs to service INTR_SLAVE register */
#define uWIFI_SetSlaveInterruptMode(interruptMask)     uWIFI_WRITE_INTR_SLAVE_MASK(interruptMask)
#define uWIFI_ClearSlaveInterruptSource(interruptMask) uWIFI_CLEAR_INTR_SLAVE(interruptMask)
#define uWIFI_SetSlaveInterrupt(interruptMask)         uWIFI_SET_INTR_SLAVE(interruptMask)
#define uWIFI_GetSlaveInterruptSource()                (uWIFI_INTR_SLAVE_REG)
#define uWIFI_GetSlaveInterruptMode()                  (uWIFI_INTR_SLAVE_MASK_REG)
#define uWIFI_GetSlaveInterruptSourceMasked()          (uWIFI_INTR_SLAVE_MASKED_REG)


/***************************************
*     Vars with External Linkage
***************************************/

extern uint8 uWIFI_initVar;


/***************************************
*              Registers
***************************************/

#define uWIFI_CTRL_REG               (*(reg32 *) uWIFI_SCB__CTRL)
#define uWIFI_CTRL_PTR               ( (reg32 *) uWIFI_SCB__CTRL)

#define uWIFI_STATUS_REG             (*(reg32 *) uWIFI_SCB__STATUS)
#define uWIFI_STATUS_PTR             ( (reg32 *) uWIFI_SCB__STATUS)

#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_SPI_CTRL_REG           (*(reg32 *) uWIFI_SCB__SPI_CTRL)
    #define uWIFI_SPI_CTRL_PTR           ( (reg32 *) uWIFI_SCB__SPI_CTRL)

    #define uWIFI_SPI_STATUS_REG         (*(reg32 *) uWIFI_SCB__SPI_STATUS)
    #define uWIFI_SPI_STATUS_PTR         ( (reg32 *) uWIFI_SCB__SPI_STATUS)

    #define uWIFI_UART_CTRL_REG          (*(reg32 *) uWIFI_SCB__UART_CTRL)
    #define uWIFI_UART_CTRL_PTR          ( (reg32 *) uWIFI_SCB__UART_CTRL)

    #define uWIFI_UART_TX_CTRL_REG       (*(reg32 *) uWIFI_SCB__UART_TX_CTRL)
    #define uWIFI_UART_TX_CTRL_PTR       ( (reg32 *) uWIFI_SCB__UART_RX_CTRL)

    #define uWIFI_UART_RX_CTRL_REG       (*(reg32 *) uWIFI_SCB__UART_RX_CTRL)
    #define uWIFI_UART_RX_CTRL_PTR       ( (reg32 *) uWIFI_SCB__UART_RX_CTRL)

    #define uWIFI_UART_RX_STATUS_REG     (*(reg32 *) uWIFI_SCB__UART_RX_STATUS)
    #define uWIFI_UART_RX_STATUS_PTR     ( (reg32 *) uWIFI_SCB__UART_RX_STATUS)
#endif /* (!uWIFI_CY_SCBIP_V1) */

#if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    #define uWIFI_UART_FLOW_CTRL_REG     (*(reg32 *) uWIFI_SCB__UART_FLOW_CTRL)
    #define uWIFI_UART_FLOW_CTRL_PTR     ( (reg32 *) uWIFI_SCB__UART_FLOW_CTRL)
#endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

#define uWIFI_I2C_CTRL_REG           (*(reg32 *) uWIFI_SCB__I2C_CTRL)
#define uWIFI_I2C_CTRL_PTR           ( (reg32 *) uWIFI_SCB__I2C_CTRL)

#define uWIFI_I2C_STATUS_REG         (*(reg32 *) uWIFI_SCB__I2C_STATUS)
#define uWIFI_I2C_STATUS_PTR         ( (reg32 *) uWIFI_SCB__I2C_STATUS)

#define uWIFI_I2C_MASTER_CMD_REG     (*(reg32 *) uWIFI_SCB__I2C_M_CMD)
#define uWIFI_I2C_MASTER_CMD_PTR     ( (reg32 *) uWIFI_SCB__I2C_M_CMD)

#define uWIFI_I2C_SLAVE_CMD_REG      (*(reg32 *) uWIFI_SCB__I2C_S_CMD)
#define uWIFI_I2C_SLAVE_CMD_PTR      ( (reg32 *) uWIFI_SCB__I2C_S_CMD)

#define uWIFI_I2C_CFG_REG            (*(reg32 *) uWIFI_SCB__I2C_CFG)
#define uWIFI_I2C_CFG_PTR            ( (reg32 *) uWIFI_SCB__I2C_CFG)

#define uWIFI_TX_CTRL_REG            (*(reg32 *) uWIFI_SCB__TX_CTRL)
#define uWIFI_TX_CTRL_PTR            ( (reg32 *) uWIFI_SCB__TX_CTRL)

#define uWIFI_TX_FIFO_CTRL_REG       (*(reg32 *) uWIFI_SCB__TX_FIFO_CTRL)
#define uWIFI_TX_FIFO_CTRL_PTR       ( (reg32 *) uWIFI_SCB__TX_FIFO_CTRL)

#define uWIFI_TX_FIFO_STATUS_REG     (*(reg32 *) uWIFI_SCB__TX_FIFO_STATUS)
#define uWIFI_TX_FIFO_STATUS_PTR     ( (reg32 *) uWIFI_SCB__TX_FIFO_STATUS)

#define uWIFI_TX_FIFO_WR_REG         (*(reg32 *) uWIFI_SCB__TX_FIFO_WR)
#define uWIFI_TX_FIFO_WR_PTR         ( (reg32 *) uWIFI_SCB__TX_FIFO_WR)

#define uWIFI_RX_CTRL_REG            (*(reg32 *) uWIFI_SCB__RX_CTRL)
#define uWIFI_RX_CTRL_PTR            ( (reg32 *) uWIFI_SCB__RX_CTRL)

#define uWIFI_RX_FIFO_CTRL_REG       (*(reg32 *) uWIFI_SCB__RX_FIFO_CTRL)
#define uWIFI_RX_FIFO_CTRL_PTR       ( (reg32 *) uWIFI_SCB__RX_FIFO_CTRL)

#define uWIFI_RX_FIFO_STATUS_REG     (*(reg32 *) uWIFI_SCB__RX_FIFO_STATUS)
#define uWIFI_RX_FIFO_STATUS_PTR     ( (reg32 *) uWIFI_SCB__RX_FIFO_STATUS)

#define uWIFI_RX_MATCH_REG           (*(reg32 *) uWIFI_SCB__RX_MATCH)
#define uWIFI_RX_MATCH_PTR           ( (reg32 *) uWIFI_SCB__RX_MATCH)

#define uWIFI_RX_FIFO_RD_REG         (*(reg32 *) uWIFI_SCB__RX_FIFO_RD)
#define uWIFI_RX_FIFO_RD_PTR         ( (reg32 *) uWIFI_SCB__RX_FIFO_RD)

#define uWIFI_RX_FIFO_RD_SILENT_REG  (*(reg32 *) uWIFI_SCB__RX_FIFO_RD_SILENT)
#define uWIFI_RX_FIFO_RD_SILENT_PTR  ( (reg32 *) uWIFI_SCB__RX_FIFO_RD_SILENT)

#define uWIFI_EZBUF_DATA00_REG       (*(reg32 *) uWIFI_SCB__EZ_DATA00)
#define uWIFI_EZBUF_DATA00_PTR       ( (reg32 *) uWIFI_SCB__EZ_DATA00)

#define uWIFI_INTR_CAUSE_REG         (*(reg32 *) uWIFI_SCB__INTR_CAUSE)
#define uWIFI_INTR_CAUSE_PTR         ( (reg32 *) uWIFI_SCB__INTR_CAUSE)

#define uWIFI_INTR_I2C_EC_REG        (*(reg32 *) uWIFI_SCB__INTR_I2C_EC)
#define uWIFI_INTR_I2C_EC_PTR        ( (reg32 *) uWIFI_SCB__INTR_I2C_EC)

#define uWIFI_INTR_I2C_EC_MASK_REG   (*(reg32 *) uWIFI_SCB__INTR_I2C_EC_MASK)
#define uWIFI_INTR_I2C_EC_MASK_PTR   ( (reg32 *) uWIFI_SCB__INTR_I2C_EC_MASK)

#define uWIFI_INTR_I2C_EC_MASKED_REG (*(reg32 *) uWIFI_SCB__INTR_I2C_EC_MASKED)
#define uWIFI_INTR_I2C_EC_MASKED_PTR ( (reg32 *) uWIFI_SCB__INTR_I2C_EC_MASKED)

#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_INTR_SPI_EC_REG        (*(reg32 *) uWIFI_SCB__INTR_SPI_EC)
    #define uWIFI_INTR_SPI_EC_PTR        ( (reg32 *) uWIFI_SCB__INTR_SPI_EC)

    #define uWIFI_INTR_SPI_EC_MASK_REG   (*(reg32 *) uWIFI_SCB__INTR_SPI_EC_MASK)
    #define uWIFI_INTR_SPI_EC_MASK_PTR   ( (reg32 *) uWIFI_SCB__INTR_SPI_EC_MASK)

    #define uWIFI_INTR_SPI_EC_MASKED_REG (*(reg32 *) uWIFI_SCB__INTR_SPI_EC_MASKED)
    #define uWIFI_INTR_SPI_EC_MASKED_PTR ( (reg32 *) uWIFI_SCB__INTR_SPI_EC_MASKED)
#endif /* (!uWIFI_CY_SCBIP_V1) */

#define uWIFI_INTR_MASTER_REG        (*(reg32 *) uWIFI_SCB__INTR_M)
#define uWIFI_INTR_MASTER_PTR        ( (reg32 *) uWIFI_SCB__INTR_M)

#define uWIFI_INTR_MASTER_SET_REG    (*(reg32 *) uWIFI_SCB__INTR_M_SET)
#define uWIFI_INTR_MASTER_SET_PTR    ( (reg32 *) uWIFI_SCB__INTR_M_SET)

#define uWIFI_INTR_MASTER_MASK_REG   (*(reg32 *) uWIFI_SCB__INTR_M_MASK)
#define uWIFI_INTR_MASTER_MASK_PTR   ( (reg32 *) uWIFI_SCB__INTR_M_MASK)

#define uWIFI_INTR_MASTER_MASKED_REG (*(reg32 *) uWIFI_SCB__INTR_M_MASKED)
#define uWIFI_INTR_MASTER_MASKED_PTR ( (reg32 *) uWIFI_SCB__INTR_M_MASKED)

#define uWIFI_INTR_SLAVE_REG         (*(reg32 *) uWIFI_SCB__INTR_S)
#define uWIFI_INTR_SLAVE_PTR         ( (reg32 *) uWIFI_SCB__INTR_S)

#define uWIFI_INTR_SLAVE_SET_REG     (*(reg32 *) uWIFI_SCB__INTR_S_SET)
#define uWIFI_INTR_SLAVE_SET_PTR     ( (reg32 *) uWIFI_SCB__INTR_S_SET)

#define uWIFI_INTR_SLAVE_MASK_REG    (*(reg32 *) uWIFI_SCB__INTR_S_MASK)
#define uWIFI_INTR_SLAVE_MASK_PTR    ( (reg32 *) uWIFI_SCB__INTR_S_MASK)

#define uWIFI_INTR_SLAVE_MASKED_REG  (*(reg32 *) uWIFI_SCB__INTR_S_MASKED)
#define uWIFI_INTR_SLAVE_MASKED_PTR  ( (reg32 *) uWIFI_SCB__INTR_S_MASKED)

#define uWIFI_INTR_TX_REG            (*(reg32 *) uWIFI_SCB__INTR_TX)
#define uWIFI_INTR_TX_PTR            ( (reg32 *) uWIFI_SCB__INTR_TX)

#define uWIFI_INTR_TX_SET_REG        (*(reg32 *) uWIFI_SCB__INTR_TX_SET)
#define uWIFI_INTR_TX_SET_PTR        ( (reg32 *) uWIFI_SCB__INTR_TX_SET)

#define uWIFI_INTR_TX_MASK_REG       (*(reg32 *) uWIFI_SCB__INTR_TX_MASK)
#define uWIFI_INTR_TX_MASK_PTR       ( (reg32 *) uWIFI_SCB__INTR_TX_MASK)

#define uWIFI_INTR_TX_MASKED_REG     (*(reg32 *) uWIFI_SCB__INTR_TX_MASKED)
#define uWIFI_INTR_TX_MASKED_PTR     ( (reg32 *) uWIFI_SCB__INTR_TX_MASKED)

#define uWIFI_INTR_RX_REG            (*(reg32 *) uWIFI_SCB__INTR_RX)
#define uWIFI_INTR_RX_PTR            ( (reg32 *) uWIFI_SCB__INTR_RX)

#define uWIFI_INTR_RX_SET_REG        (*(reg32 *) uWIFI_SCB__INTR_RX_SET)
#define uWIFI_INTR_RX_SET_PTR        ( (reg32 *) uWIFI_SCB__INTR_RX_SET)

#define uWIFI_INTR_RX_MASK_REG       (*(reg32 *) uWIFI_SCB__INTR_RX_MASK)
#define uWIFI_INTR_RX_MASK_PTR       ( (reg32 *) uWIFI_SCB__INTR_RX_MASK)

#define uWIFI_INTR_RX_MASKED_REG     (*(reg32 *) uWIFI_SCB__INTR_RX_MASKED)
#define uWIFI_INTR_RX_MASKED_PTR     ( (reg32 *) uWIFI_SCB__INTR_RX_MASKED)

#if (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    #define uWIFI_FF_DATA_NR_LOG2_PLUS1_MASK (0x0Fu) /* FF_DATA_NR_LOG2_PLUS1 = 4, MASK = 2^4 - 1 = 15 */
    #define uWIFI_FF_DATA_NR_LOG2_MASK       (0x07u) /* FF_DATA_NR_LOG2 = 3, MASK = 2^3 - 1 = 7 */
#else
    #define uWIFI_FF_DATA_NR_LOG2_PLUS1_MASK (0x1Fu) /* FF_DATA_NR_LOG2_PLUS1 = 5, MASK = 2^5 - 1 = 31 */
    #define uWIFI_FF_DATA_NR_LOG2_MASK       (0x0Fu) /* FF_DATA_NR_LOG2 = 4, MASK = 2^4 - 1 = 15 */
#endif /* (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */


/***************************************
*        Registers Constants
***************************************/

#if(uWIFI_SCB_IRQ_INTERNAL)
    #define uWIFI_ISR_NUMBER     ((uint8) uWIFI_SCB_IRQ__INTC_NUMBER)
    #define uWIFI_ISR_PRIORITY   ((uint8) uWIFI_SCB_IRQ__INTC_PRIOR_NUM)
#endif /* (uWIFI_SCB_IRQ_INTERNAL) */

#if(uWIFI_UART_RX_WAKEUP_IRQ)
    #define uWIFI_RX_WAKE_ISR_NUMBER     ((uint8) uWIFI_RX_WAKEUP_IRQ__INTC_NUMBER)
    #define uWIFI_RX_WAKE_ISR_PRIORITY   ((uint8) uWIFI_RX_WAKEUP_IRQ__INTC_PRIOR_NUM)
#endif /* (uWIFI_UART_RX_WAKEUP_IRQ) */

/* uWIFI_CTRL_REG */
#define uWIFI_CTRL_OVS_POS           (0u)  /* [3:0]   Oversampling factor                 */
#define uWIFI_CTRL_EC_AM_MODE_POS    (8u)  /* [8]     Externally clocked address match    */
#define uWIFI_CTRL_EC_OP_MODE_POS    (9u)  /* [9]     Externally clocked operation mode   */
#define uWIFI_CTRL_EZBUF_MODE_POS    (10u) /* [10]    EZ buffer is enabled                */
#if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    #define uWIFI_CTRL_BYTE_MODE_POS (11u) /* [11]    Determines the number of bits per FIFO data element */
#endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */
#define uWIFI_CTRL_ADDR_ACCEPT_POS   (16u) /* [16]    Put matched address in RX FIFO       */
#define uWIFI_CTRL_BLOCK_POS         (17u) /* [17]    Ext and Int logic to resolve collide */
#define uWIFI_CTRL_MODE_POS          (24u) /* [25:24] Operation mode                       */
#define uWIFI_CTRL_ENABLED_POS       (31u) /* [31]    Enable SCB block                     */
#define uWIFI_CTRL_OVS_MASK          ((uint32) 0x0Fu)
#define uWIFI_CTRL_EC_AM_MODE        ((uint32) 0x01u << uWIFI_CTRL_EC_AM_MODE_POS)
#define uWIFI_CTRL_EC_OP_MODE        ((uint32) 0x01u << uWIFI_CTRL_EC_OP_MODE_POS)
#define uWIFI_CTRL_EZBUF_MODE        ((uint32) 0x01u << uWIFI_CTRL_EZBUF_MODE_POS)
#if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    #define uWIFI_CTRL_BYTE_MODE ((uint32) 0x01u << uWIFI_CTRL_BYTE_MODE_POS)
#endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */
#define uWIFI_CTRL_ADDR_ACCEPT       ((uint32) 0x01u << uWIFI_CTRL_ADDR_ACCEPT_POS)
#define uWIFI_CTRL_BLOCK             ((uint32) 0x01u << uWIFI_CTRL_BLOCK_POS)
#define uWIFI_CTRL_MODE_MASK         ((uint32) 0x03u << uWIFI_CTRL_MODE_POS)
#define uWIFI_CTRL_MODE_I2C          ((uint32) 0x00u)
#define uWIFI_CTRL_MODE_SPI          ((uint32) 0x01u << uWIFI_CTRL_MODE_POS)
#define uWIFI_CTRL_MODE_UART         ((uint32) 0x02u << uWIFI_CTRL_MODE_POS)
#define uWIFI_CTRL_ENABLED           ((uint32) 0x01u << uWIFI_CTRL_ENABLED_POS)

/* uWIFI_STATUS_REG */
#define uWIFI_STATUS_EC_BUSY_POS     (0u)  /* [0] Bus busy. Externally clocked logic access to EZ memory */
#define uWIFI_STATUS_EC_BUSY         ((uint32) 0x0Fu)

/* uWIFI_SPI_CTRL_REG  */
#define uWIFI_SPI_CTRL_CONTINUOUS_POS        (0u)  /* [0]     Continuous or Separated SPI data transfers */
#define uWIFI_SPI_CTRL_SELECT_PRECEDE_POS    (1u)  /* [1]     Precedes or coincides start of data frame  */
#define uWIFI_SPI_CTRL_CPHA_POS              (2u)  /* [2]     SCLK phase                                 */
#define uWIFI_SPI_CTRL_CPOL_POS              (3u)  /* [3]     SCLK polarity                              */
#define uWIFI_SPI_CTRL_LATE_MISO_SAMPLE_POS  (4u)  /* [4]     Late MISO sample enabled                   */
#if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    #define uWIFI_SPI_CTRL_SCLK_CONTINUOUS_POS   (5u)  /* [5]     Enable continuous SCLK generation */
    #define uWIFI_SPI_CTRL_SSEL0_POLARITY_POS    (8u)  /* [8]     SS0 polarity                      */
    #define uWIFI_SPI_CTRL_SSEL1_POLARITY_POS    (9u)  /* [9]     SS1 polarity                      */
    #define uWIFI_SPI_CTRL_SSEL2_POLARITY_POS    (10u) /* [10]    SS2 polarity                      */
    #define uWIFI_SPI_CTRL_SSEL3_POLARITY_POS    (11u) /* [11]    SS3 polarity                      */
#endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */
#define uWIFI_SPI_CTRL_LOOPBACK_POS          (16u) /* [16]    Local loop-back control enabled            */
#define uWIFI_SPI_CTRL_MODE_POS              (24u) /* [25:24] Submode of SPI operation                   */
#define uWIFI_SPI_CTRL_SLAVE_SELECT_POS      (26u) /* [27:26] Selects SPI SS signal                      */
#define uWIFI_SPI_CTRL_MASTER_MODE_POS       (31u) /* [31]    Master mode enabled                        */
#define uWIFI_SPI_CTRL_CONTINUOUS            ((uint32) 0x01u)
#define uWIFI_SPI_CTRL_SELECT_PRECEDE        ((uint32) 0x01u << uWIFI_SPI_CTRL_SELECT_PRECEDE_POS)
#define uWIFI_SPI_CTRL_SCLK_MODE_MASK        ((uint32) 0x03u << uWIFI_SPI_CTRL_CPHA_POS)
#define uWIFI_SPI_CTRL_CPHA                  ((uint32) 0x01u << uWIFI_SPI_CTRL_CPHA_POS)
#define uWIFI_SPI_CTRL_CPOL                  ((uint32) 0x01u << uWIFI_SPI_CTRL_CPOL_POS)
#define uWIFI_SPI_CTRL_LATE_MISO_SAMPLE      ((uint32) 0x01u << \
                                                                    uWIFI_SPI_CTRL_LATE_MISO_SAMPLE_POS)
#if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    #define uWIFI_SPI_CTRL_SCLK_CONTINUOUS  ((uint32) 0x01u << uWIFI_SPI_CTRL_SCLK_CONTINUOUS_POS)
    #define uWIFI_SPI_CTRL_SSEL0_POLARITY   ((uint32) 0x01u << uWIFI_SPI_CTRL_SSEL_POLARITY0_POS)
    #define uWIFI_SPI_CTRL_SSEL1_POLARITY   ((uint32) 0x01u << uWIFI_SPI_CTRL_SSEL_POLARITY1_POS)
    #define uWIFI_SPI_CTRL_SSEL2_POLARITY   ((uint32) 0x01u << uWIFI_SPI_CTRL_SSEL_POLARITY2_POS)
    #define uWIFI_SPI_CTRL_SSEL3_POLARITY   ((uint32) 0x01u << uWIFI_SPI_CTRL_SSEL_POLARITY3_POS)
    #define uWIFI_SPI_CTRL_SSEL_POLARITY_MASK ((uint32)0x0Fu << uWIFI_SPI_CTRL_SSEL0_POLARITY_POS)
#endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

#define uWIFI_SPI_CTRL_LOOPBACK              ((uint32) 0x01u << uWIFI_SPI_CTRL_LOOPBACK_POS)
#define uWIFI_SPI_CTRL_MODE_MASK             ((uint32) 0x03u << uWIFI_SPI_CTRL_MODE_POS)
#define uWIFI_SPI_CTRL_MODE_MOTOROLA         ((uint32) 0x00u)
#define uWIFI_SPI_CTRL_MODE_TI               ((uint32) 0x01u << uWIFI_CTRL_MODE_POS)
#define uWIFI_SPI_CTRL_MODE_NS               ((uint32) 0x02u << uWIFI_CTRL_MODE_POS)
#define uWIFI_SPI_CTRL_SLAVE_SELECT_MASK     ((uint32) 0x03u << uWIFI_SPI_CTRL_SLAVE_SELECT_POS)
#define uWIFI_SPI_CTRL_SLAVE_SELECT0         ((uint32) 0x00u)
#define uWIFI_SPI_CTRL_SLAVE_SELECT1         ((uint32) 0x01u << uWIFI_SPI_CTRL_SLAVE_SELECT_POS)
#define uWIFI_SPI_CTRL_SLAVE_SELECT2         ((uint32) 0x02u << uWIFI_SPI_CTRL_SLAVE_SELECT_POS)
#define uWIFI_SPI_CTRL_SLAVE_SELECT3         ((uint32) 0x03u << uWIFI_SPI_CTRL_SLAVE_SELECT_POS)
#define uWIFI_SPI_CTRL_MASTER                ((uint32) 0x01u << uWIFI_SPI_CTRL_MASTER_MODE_POS)
#define uWIFI_SPI_CTRL_SLAVE                 ((uint32) 0x00u)

/* uWIFI_SPI_STATUS_REG  */
#define uWIFI_SPI_STATUS_BUS_BUSY_POS    (0u)  /* [0]    Bus busy - slave selected */
#define uWIFI_SPI_STATUS_EZBUF_ADDR_POS  (8u)  /* [15:8] EzAddress                 */
#define uWIFI_SPI_STATUS_BUS_BUSY        ((uint32) 0x01u)
#define uWIFI_SPI_STATUS_EZBUF_ADDR_MASK ((uint32) 0xFFu << uWIFI_I2C_STATUS_EZBUF_ADDR_POS)

/* uWIFI_UART_CTRL */
#define uWIFI_UART_CTRL_LOOPBACK_POS         (16u) /* [16] Loop-back    */
#define uWIFI_UART_CTRL_MODE_POS             (24u) /* [24] UART subMode */
#define uWIFI_UART_CTRL_LOOPBACK             ((uint32) 0x01u << uWIFI_UART_CTRL_LOOPBACK_POS)
#define uWIFI_UART_CTRL_MODE_UART_STD        ((uint32) 0x00u)
#define uWIFI_UART_CTRL_MODE_UART_SMARTCARD  ((uint32) 0x01u << uWIFI_UART_CTRL_MODE_POS)
#define uWIFI_UART_CTRL_MODE_UART_IRDA       ((uint32) 0x02u << uWIFI_UART_CTRL_MODE_POS)
#define uWIFI_UART_CTRL_MODE_MASK            ((uint32) 0x03u << uWIFI_UART_CTRL_MODE_POS)

/* uWIFI_UART_TX_CTRL */
#define uWIFI_UART_TX_CTRL_STOP_BITS_POS         (0u)  /* [2:0] Stop bits: (Stop bits + 1) * 0.5 period */
#define uWIFI_UART_TX_CTRL_PARITY_POS            (4u)  /* [4]   Parity bit                              */
#define uWIFI_UART_TX_CTRL_PARITY_ENABLED_POS    (5u)  /* [5]   Parity enable                           */
#define uWIFI_UART_TX_CTRL_RETRY_ON_NACK_POS     (8u)  /* [8]   Smart Card: re-send frame on NACK       */
#define uWIFI_UART_TX_CTRL_ONE_STOP_BIT          ((uint32) 0x01u)
#define uWIFI_UART_TX_CTRL_ONE_HALF_STOP_BITS    ((uint32) 0x02u)
#define uWIFI_UART_TX_CTRL_TWO_STOP_BITS         ((uint32) 0x03u)
#define uWIFI_UART_TX_CTRL_STOP_BITS_MASK        ((uint32) 0x07u)
#define uWIFI_UART_TX_CTRL_PARITY                ((uint32) 0x01u << \
                                                                    uWIFI_UART_TX_CTRL_PARITY_POS)
#define uWIFI_UART_TX_CTRL_PARITY_ENABLED        ((uint32) 0x01u << \
                                                                    uWIFI_UART_TX_CTRL_PARITY_ENABLED_POS)
#define uWIFI_UART_TX_CTRL_RETRY_ON_NACK         ((uint32) 0x01u << \
                                                                    uWIFI_UART_TX_CTRL_RETRY_ON_NACK_POS)

/* uWIFI_UART_RX_CTRL */
#define uWIFI_UART_RX_CTRL_STOP_BITS_POS             (0u)  /* [2:0] Stop bits: (Stop bits + 1) * 0.5 period*/
#define uWIFI_UART_RX_CTRL_PARITY_POS                (4u)  /* [4]   Parity bit                             */
#define uWIFI_UART_RX_CTRL_PARITY_ENABLED_POS        (5u)  /* [5]   Parity enable                          */
#define uWIFI_UART_RX_CTRL_POLARITY_POS              (6u)  /* [6]   IrDA: inverts polarity of RX signal    */
#define uWIFI_UART_RX_CTRL_DROP_ON_PARITY_ERR_POS    (8u)  /* [8]   Drop and lost RX FIFO on parity error  */
#define uWIFI_UART_RX_CTRL_DROP_ON_FRAME_ERR_POS     (9u)  /* [9]   Drop and lost RX FIFO on frame error   */
#define uWIFI_UART_RX_CTRL_MP_MODE_POS               (10u) /* [10]  Multi-processor mode                   */
#define uWIFI_UART_RX_CTRL_LIN_MODE_POS              (12u) /* [12]  Lin mode: applicable for UART Standard */
#define uWIFI_UART_RX_CTRL_SKIP_START_POS            (13u) /* [13]  Skip start not: only for UART Standard */
#define uWIFI_UART_RX_CTRL_BREAK_WIDTH_POS           (16u) /* [19:16]  Break width: (Break width + 1)      */
#define uWIFI_UART_TX_CTRL_ONE_STOP_BIT              ((uint32) 0x01u)
#define uWIFI_UART_TX_CTRL_ONE_HALF_STOP_BITS        ((uint32) 0x02u)
#define uWIFI_UART_TX_CTRL_TWO_STOP_BITS             ((uint32) 0x03u)
#define uWIFI_UART_RX_CTRL_STOP_BITS_MASK            ((uint32) 0x07u)
#define uWIFI_UART_RX_CTRL_PARITY                    ((uint32) 0x01u << \
                                                                    uWIFI_UART_RX_CTRL_PARITY_POS)
#define uWIFI_UART_RX_CTRL_PARITY_ENABLED            ((uint32) 0x01u << \
                                                                    uWIFI_UART_RX_CTRL_PARITY_ENABLED_POS)
#define uWIFI_UART_RX_CTRL_POLARITY                  ((uint32) 0x01u << \
                                                                    uWIFI_UART_RX_CTRL_POLARITY_POS)
#define uWIFI_UART_RX_CTRL_DROP_ON_PARITY_ERR        ((uint32) 0x01u << \
                                                                   uWIFI_UART_RX_CTRL_DROP_ON_PARITY_ERR_POS)
#define uWIFI_UART_RX_CTRL_DROP_ON_FRAME_ERR         ((uint32) 0x01u << \
                                                                    uWIFI_UART_RX_CTRL_DROP_ON_FRAME_ERR_POS)
#define uWIFI_UART_RX_CTRL_MP_MODE                   ((uint32) 0x01u << \
                                                                    uWIFI_UART_RX_CTRL_MP_MODE_POS)
#define uWIFI_UART_RX_CTRL_LIN_MODE                  ((uint32) 0x01u << \
                                                                    uWIFI_UART_RX_CTRL_LIN_MODE_POS)
#define uWIFI_UART_RX_CTRL_SKIP_START                ((uint32) 0x01u << \
                                                                    uWIFI_UART_RX_CTRL_SKIP_START_POS)
#define uWIFI_UART_RX_CTRL_BREAK_WIDTH_MASK          ((uint32) 0x0Fu << \
                                                                    uWIFI_UART_RX_CTRL_BREAK_WIDTH_POS)
/* uWIFI_UART_RX_STATUS_REG */
#define uWIFI_UART_RX_STATUS_BR_COUNTER_POS     (0u)  /* [11:0] Baud Rate counter */
#define uWIFI_UART_RX_STATUS_BR_COUNTER_MASK    ((uint32) 0xFFFu)

#if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    /* uWIFI_UART_FLOW_CTRL_REG */
    #define uWIFI_UART_FLOW_CTRL_TRIGGER_LEVEL_POS    (0u)  /* [7:0] RTS RX FIFO trigger level         */
    #define uWIFI_UART_FLOW_CTRL_RTS_POLARITY_POS     (16u) /* [16]  Polarity of the RTS output signal */
    #define uWIFI_UART_FLOW_CTRL_CTS_POLARITY_POS     (24u) /* [24]  Polarity of the CTS input signal  */
    #define uWIFI_UART_FLOW_CTRL_CTS_ENABLED_POS      (25u) /* [25]  Enable CTS signal                 */
    #define uWIFI_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK   ((uint32) uWIFI_FF_DATA_NR_LOG2_MASK)
    #define uWIFI_UART_FLOW_CTRL_RTS_POLARITY         ((uint32) 0x01u << \
                                                                       uWIFI_UART_FLOW_CTRL_RTS_POLARITY_POS)
    #define uWIFI_UART_FLOW_CTRL_CTS_POLARITY         ((uint32) 0x01u << \
                                                                       uWIFI_UART_FLOW_CTRL_CTS_POLARITY_POS)
    #define uWIFI_UART_FLOW_CTRL_CTS_ENABLE           ((uint32) 0x01u << \
                                                                       uWIFI_UART_FLOW_CTRL_CTS_ENABLED_POS)
#endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

/* uWIFI_I2C_CTRL */
#define uWIFI_I2C_CTRL_HIGH_PHASE_OVS_POS           (0u)   /* [3:0] Oversampling factor high: master only */
#define uWIFI_I2C_CTRL_LOW_PHASE_OVS_POS            (4u)   /* [7:4] Oversampling factor low:  master only */
#define uWIFI_I2C_CTRL_M_READY_DATA_ACK_POS         (8u)   /* [8]   Master ACKs data while RX FIFO != FULL*/
#define uWIFI_I2C_CTRL_M_NOT_READY_DATA_NACK_POS    (9u)   /* [9]   Master NACKs data if RX FIFO ==  FULL */
#define uWIFI_I2C_CTRL_S_GENERAL_IGNORE_POS         (11u)  /* [11]  Slave ignores General call            */
#define uWIFI_I2C_CTRL_S_READY_ADDR_ACK_POS         (12u)  /* [12]  Slave ACKs Address if RX FIFO != FULL */
#define uWIFI_I2C_CTRL_S_READY_DATA_ACK_POS         (13u)  /* [13]  Slave ACKs data while RX FIFO == FULL */
#define uWIFI_I2C_CTRL_S_NOT_READY_ADDR_NACK_POS    (14u)  /* [14]  Slave NACKs address if RX FIFO == FULL*/
#define uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK_POS    (15u)  /* [15]  Slave NACKs data if RX FIFO is  FULL  */
#define uWIFI_I2C_CTRL_LOOPBACK_POS                 (16u)  /* [16]  Loop-back                             */
#define uWIFI_I2C_CTRL_SLAVE_MODE_POS               (30u)  /* [30]  Slave mode enabled                    */
#define uWIFI_I2C_CTRL_MASTER_MODE_POS              (31u)  /* [31]  Master mode enabled                   */
#define uWIFI_I2C_CTRL_HIGH_PHASE_OVS_MASK  ((uint32) 0x0Fu)
#define uWIFI_I2C_CTRL_LOW_PHASE_OVS_MASK   ((uint32) 0x0Fu << \
                                                                uWIFI_I2C_CTRL_LOW_PHASE_OVS_POS)
#define uWIFI_I2C_CTRL_M_READY_DATA_ACK      ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_M_READY_DATA_ACK_POS)
#define uWIFI_I2C_CTRL_M_NOT_READY_DATA_NACK ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_M_NOT_READY_DATA_NACK_POS)
#define uWIFI_I2C_CTRL_S_GENERAL_IGNORE      ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_S_GENERAL_IGNORE_POS)
#define uWIFI_I2C_CTRL_S_READY_ADDR_ACK      ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_S_READY_ADDR_ACK_POS)
#define uWIFI_I2C_CTRL_S_READY_DATA_ACK      ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_S_READY_DATA_ACK_POS)
#define uWIFI_I2C_CTRL_S_NOT_READY_ADDR_NACK ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_S_NOT_READY_ADDR_NACK_POS)
#define uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK_POS)
#define uWIFI_I2C_CTRL_LOOPBACK              ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_LOOPBACK_POS)
#define uWIFI_I2C_CTRL_SLAVE_MODE            ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_SLAVE_MODE_POS)
#define uWIFI_I2C_CTRL_MASTER_MODE           ((uint32) 0x01u << \
                                                                uWIFI_I2C_CTRL_MASTER_MODE_POS)
#define uWIFI_I2C_CTRL_SLAVE_MASTER_MODE_MASK    ((uint32) 0x03u << \
                                                                uWIFI_I2C_CTRL_SLAVE_MODE_POS)

/* uWIFI_I2C_STATUS_REG  */
#define uWIFI_I2C_STATUS_BUS_BUSY_POS    (0u)  /* [0]    Bus busy: internally clocked */
#define uWIFI_I2C_STATUS_S_READ_POS      (4u)  /* [4]    Slave is read by master      */
#define uWIFI_I2C_STATUS_M_READ_POS      (5u)  /* [5]    Master reads Slave           */
#define uWIFI_I2C_STATUS_EZBUF_ADDR_POS  (8u)  /* [15:8] EZAddress                    */
#define uWIFI_I2C_STATUS_BUS_BUSY        ((uint32) 0x01u)
#define uWIFI_I2C_STATUS_S_READ          ((uint32) 0x01u << uWIFI_I2C_STATUS_S_READ_POS)
#define uWIFI_I2C_STATUS_M_READ          ((uint32) 0x01u << uWIFI_I2C_STATUS_M_READ_POS)
#define uWIFI_I2C_STATUS_EZBUF_ADDR_MASK ((uint32) 0xFFu << uWIFI_I2C_STATUS_EZBUF_ADDR_POS)

/* uWIFI_I2C_MASTER_CMD_REG */
#define uWIFI_I2C_MASTER_CMD_M_START_POS             (0u)  /* [0] Master generate Start                */
#define uWIFI_I2C_MASTER_CMD_M_START_ON_IDLE_POS     (1u)  /* [1] Master generate Start if bus is free */
#define uWIFI_I2C_MASTER_CMD_M_ACK_POS               (2u)  /* [2] Master generate ACK                  */
#define uWIFI_I2C_MASTER_CMD_M_NACK_POS              (3u)  /* [3] Master generate NACK                 */
#define uWIFI_I2C_MASTER_CMD_M_STOP_POS              (4u)  /* [4] Master generate Stop                 */
#define uWIFI_I2C_MASTER_CMD_M_START         ((uint32) 0x01u)
#define uWIFI_I2C_MASTER_CMD_M_START_ON_IDLE ((uint32) 0x01u << \
                                                                   uWIFI_I2C_MASTER_CMD_M_START_ON_IDLE_POS)
#define uWIFI_I2C_MASTER_CMD_M_ACK           ((uint32) 0x01u << \
                                                                   uWIFI_I2C_MASTER_CMD_M_ACK_POS)
#define uWIFI_I2C_MASTER_CMD_M_NACK          ((uint32) 0x01u << \
                                                                    uWIFI_I2C_MASTER_CMD_M_NACK_POS)
#define uWIFI_I2C_MASTER_CMD_M_STOP          ((uint32) 0x01u << \
                                                                    uWIFI_I2C_MASTER_CMD_M_STOP_POS)

/* uWIFI_I2C_SLAVE_CMD_REG  */
#define uWIFI_I2C_SLAVE_CMD_S_ACK_POS    (0u)  /* [0] Slave generate ACK  */
#define uWIFI_I2C_SLAVE_CMD_S_NACK_POS   (1u)  /* [1] Slave generate NACK */
#define uWIFI_I2C_SLAVE_CMD_S_ACK        ((uint32) 0x01u)
#define uWIFI_I2C_SLAVE_CMD_S_NACK       ((uint32) 0x01u << uWIFI_I2C_SLAVE_CMD_S_NACK_POS)

#define uWIFI_I2C_SLAVE_CMD_S_ACK_POS    (0u)  /* [0] Slave generate ACK  */
#define uWIFI_I2C_SLAVE_CMD_S_NACK_POS   (1u)  /* [1] Slave generate NACK */
#define uWIFI_I2C_SLAVE_CMD_S_ACK        ((uint32) 0x01u)
#define uWIFI_I2C_SLAVE_CMD_S_NACK       ((uint32) 0x01u << uWIFI_I2C_SLAVE_CMD_S_NACK_POS)

/* uWIFI_I2C_CFG_REG */
#if (uWIFI_CY_SCBIP_V0)
#define uWIFI_I2C_CFG_SDA_FILT_HYS_POS           (0u)  /* [1:0]   Trim bits for the I2C SDA filter         */
#define uWIFI_I2C_CFG_SDA_FILT_TRIM_POS          (2u)  /* [3:2]   Trim bits for the I2C SDA filter         */
#define uWIFI_I2C_CFG_SCL_FILT_HYS_POS           (4u)  /* [5:4]   Trim bits for the I2C SCL filter         */
#define uWIFI_I2C_CFG_SCL_FILT_TRIM_POS          (6u)  /* [7:6]   Trim bits for the I2C SCL filter         */
#define uWIFI_I2C_CFG_SDA_FILT_OUT_HYS_POS       (8u)  /* [9:8]   Trim bits for I2C SDA filter output path */
#define uWIFI_I2C_CFG_SDA_FILT_OUT_TRIM_POS      (10u) /* [11:10] Trim bits for I2C SDA filter output path */
#define uWIFI_I2C_CFG_SDA_FILT_HS_POS            (16u) /* [16]    '0': 50 ns filter, '1': 10 ns filter     */
#define uWIFI_I2C_CFG_SDA_FILT_ENABLED_POS       (17u) /* [17]    I2C SDA filter enabled                   */
#define uWIFI_I2C_CFG_SCL_FILT_HS_POS            (24u) /* [24]    '0': 50 ns filter, '1': 10 ns filter     */
#define uWIFI_I2C_CFG_SCL_FILT_ENABLED_POS       (25u) /* [25]    I2C SCL filter enabled                   */
#define uWIFI_I2C_CFG_SDA_FILT_OUT_HS_POS        (26u) /* [26]    '0': 50 ns filter, '1': 10 ns filter     */
#define uWIFI_I2C_CFG_SDA_FILT_OUT_ENABLED_POS   (27u) /* [27]    I2C SDA output delay filter enabled      */
#define uWIFI_I2C_CFG_SDA_FILT_HYS_MASK          ((uint32) 0x03u)
#define uWIFI_I2C_CFG_SDA_FILT_TRIM_MASK         ((uint32) 0x03u << \
                                                                uWIFI_I2C_CFG_SDA_FILT_TRIM_POS)
#define uWIFI_I2C_CFG_SCL_FILT_HYS_MASK          ((uint32) 0x03u << \
                                                                uWIFI_I2C_CFG_SCL_FILT_HYS_POS)
#define uWIFI_I2C_CFG_SCL_FILT_TRIM_MASK         ((uint32) 0x03u << \
                                                                uWIFI_I2C_CFG_SCL_FILT_TRIM_POS)
#define uWIFI_I2C_CFG_SDA_FILT_OUT_HYS_MASK      ((uint32) 0x03u << \
                                                                uWIFI_I2C_CFG_SDA_FILT_OUT_HYS_POS)
#define uWIFI_I2C_CFG_SDA_FILT_OUT_TRIM_MASK     ((uint32) 0x03u << \
                                                                uWIFI_I2C_CFG_SDA_FILT_OUT_TRIM_POS)
#define uWIFI_I2C_CFG_SDA_FILT_HS                ((uint32) 0x01u << \
                                                                uWIFI_I2C_CFG_SDA_FILT_HS_POS)
#define uWIFI_I2C_CFG_SDA_FILT_ENABLED           ((uint32) 0x01u << \
                                                                uWIFI_I2C_CFG_SDA_FILT_ENABLED_POS)
#define uWIFI_I2C_CFG_SCL_FILT_HS                ((uint32) 0x01u << \
                                                                uWIFI_I2C_CFG_SCL_FILT_HS_POS)
#define uWIFI_I2C_CFG_SCL_FILT_ENABLED           ((uint32) 0x01u << \
                                                                uWIFI_I2C_CFG_SCL_FILT_ENABLED_POS)
#define uWIFI_I2C_CFG_SDA_FILT_OUT_HS            ((uint32) 0x01u << \
                                                                uWIFI_I2C_CFG_SDA_FILT_OUT_HS_POS)
#define uWIFI_I2C_CFG_SDA_FILT_OUT_ENABLED       ((uint32) 0x01u << \
                                                                uWIFI_I2C_CFG_SDA_FILT_OUT_ENABLED_POS)
#else
#define uWIFI_I2C_CFG_SDA_IN_FILT_TRIM_POS   (0u)  /* [1:0] Trim bits for "i2c_sda_in" 50 ns filter */
#define uWIFI_I2C_CFG_SDA_IN_FILT_SEL_POS    (4u)  /* [4]   "i2c_sda_in" filter delay: 0 ns and 50 ns */
#define uWIFI_I2C_CFG_SCL_IN_FILT_TRIM_POS   (8u)  /* [9:8] Trim bits for "i2c_scl_in" 50 ns filter */
#define uWIFI_I2C_CFG_SCL_IN_FILT_SEL_POS    (12u) /* [12]  "i2c_scl_in" filter delay: 0 ns and 50 ns */
#define uWIFI_I2C_CFG_SDA_OUT_FILT0_TRIM_POS (16u) /* [17:16] Trim bits for "i2c_sda_out" 50 ns filter 0 */
#define uWIFI_I2C_CFG_SDA_OUT_FILT1_TRIM_POS (18u) /* [19:18] Trim bits for "i2c_sda_out" 50 ns filter 1 */
#define uWIFI_I2C_CFG_SDA_OUT_FILT2_TRIM_POS (20u) /* [21:20] Trim bits for "i2c_sda_out" 50 ns filter 2 */
#define uWIFI_I2C_CFG_SDA_OUT_FILT_SEL_POS   (28u) /* [29:28] Cumulative "i2c_sda_out" filter delay: */

#define uWIFI_I2C_CFG_SDA_IN_FILT_TRIM_MASK  ((uint32) 0x03u)
#define uWIFI_I2C_CFG_SDA_IN_FILT_SEL        ((uint32) 0x01u << uWIFI_I2C_CFG_SDA_IN_FILT_SEL_POS)
#define uWIFI_I2C_CFG_SCL_IN_FILT_TRIM_MASK  ((uint32) 0x03u << \
                                                            uWIFI_I2C_CFG_SCL_IN_FILT_TRIM_POS)
#define uWIFI_I2C_CFG_SCL_IN_FILT_SEL        ((uint32) 0x01u << uWIFI_I2C_CFG_SCL_IN_FILT_SEL_POS)
#define uWIFI_I2C_CFG_SDA_OUT_FILT0_TRIM_MASK ((uint32) 0x03u << \
                                                            uWIFI_I2C_CFG_SDA_OUT_FILT0_TRIM_POS)
#define uWIFI_I2C_CFG_SDA_OUT_FILT1_TRIM_MASK ((uint32) 0x03u << \
                                                            uWIFI_I2C_CFG_SDA_OUT_FILT1_TRIM_POS)
#define uWIFI_I2C_CFG_SDA_OUT_FILT2_TRIM_MASK ((uint32) 0x03u << \
                                                            uWIFI_I2C_CFG_SDA_OUT_FILT2_TRIM_POS)
#define uWIFI_I2C_CFG_SDA_OUT_FILT_SEL_MASK   ((uint32) 0x03u << \
                                                            uWIFI_I2C_CFG_SDA_OUT_FILT_SEL_POS)
#endif /* (uWIFI_CY_SCBIP_V0) */


/* uWIFI_TX_CTRL_REG */
#define uWIFI_TX_CTRL_DATA_WIDTH_POS     (0u)  /* [3:0] Data frame width: (Data width - 1) */
#define uWIFI_TX_CTRL_MSB_FIRST_POS      (8u)  /* [8]   MSB first shifter-out             */
#define uWIFI_TX_CTRL_ENABLED_POS        (31u) /* [31]  Transmitter enabled               */
#define uWIFI_TX_CTRL_DATA_WIDTH_MASK    ((uint32) 0x0Fu)
#define uWIFI_TX_CTRL_MSB_FIRST          ((uint32) 0x01u << uWIFI_TX_CTRL_MSB_FIRST_POS)
#define uWIFI_TX_CTRL_LSB_FIRST          ((uint32) 0x00u)
#define uWIFI_TX_CTRL_ENABLED            ((uint32) 0x01u << uWIFI_TX_CTRL_ENABLED_POS)

/* uWIFI_TX_CTRL_FIFO_REG */
#define uWIFI_TX_FIFO_CTRL_TRIGGER_LEVEL_POS     (0u)  /* [2:0] Trigger level                              */
#define uWIFI_TX_FIFO_CTRL_CLEAR_POS             (16u) /* [16]  Clear TX FIFO: cleared after set           */
#define uWIFI_TX_FIFO_CTRL_FREEZE_POS            (17u) /* [17]  Freeze TX FIFO: HW do not inc read pointer */
#define uWIFI_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK    ((uint32) uWIFI_FF_DATA_NR_LOG2_MASK)
#define uWIFI_TX_FIFO_CTRL_CLEAR                 ((uint32) 0x01u << uWIFI_TX_FIFO_CTRL_CLEAR_POS)
#define uWIFI_TX_FIFO_CTRL_FREEZE                ((uint32) 0x01u << uWIFI_TX_FIFO_CTRL_FREEZE_POS)

/* uWIFI_TX_FIFO_STATUS_REG */
#define uWIFI_TX_FIFO_STATUS_USED_POS    (0u)  /* [3:0]   Amount of entries in TX FIFO */
#define uWIFI_TX_FIFO_SR_VALID_POS       (15u) /* [15]    Shifter status of TX FIFO    */
#define uWIFI_TX_FIFO_STATUS_RD_PTR_POS  (16u) /* [18:16] TX FIFO read pointer         */
#define uWIFI_TX_FIFO_STATUS_WR_PTR_POS  (24u) /* [26:24] TX FIFO write pointer        */
#define uWIFI_TX_FIFO_STATUS_USED_MASK   ((uint32) uWIFI_FF_DATA_NR_LOG2_PLUS1_MASK)
#define uWIFI_TX_FIFO_SR_VALID           ((uint32) 0x01u << uWIFI_TX_FIFO_SR_VALID_POS)
#define uWIFI_TX_FIFO_STATUS_RD_PTR_MASK ((uint32) uWIFI_FF_DATA_NR_LOG2_MASK << \
                                                                    uWIFI_TX_FIFO_STATUS_RD_PTR_POS)
#define uWIFI_TX_FIFO_STATUS_WR_PTR_MASK ((uint32) uWIFI_FF_DATA_NR_LOG2_MASK << \
                                                                    uWIFI_TX_FIFO_STATUS_WR_PTR_POS)

/* uWIFI_TX_FIFO_WR_REG */
#define uWIFI_TX_FIFO_WR_POS    (0u)  /* [15:0] Data written into TX FIFO */
#define uWIFI_TX_FIFO_WR_MASK   ((uint32) 0xFFu)

/* uWIFI_RX_CTRL_REG */
#define uWIFI_RX_CTRL_DATA_WIDTH_POS     (0u)  /* [3:0] Data frame width: (Data width - 1) */
#define uWIFI_RX_CTRL_MSB_FIRST_POS      (8u)  /* [8]   MSB first shifter-out             */
#define uWIFI_RX_CTRL_MEDIAN_POS         (9u)  /* [9]   Median filter                     */
#define uWIFI_RX_CTRL_ENABLED_POS        (31u) /* [31]  Receiver enabled                  */
#define uWIFI_RX_CTRL_DATA_WIDTH_MASK    ((uint32) 0x0Fu)
#define uWIFI_RX_CTRL_MSB_FIRST          ((uint32) 0x01u << uWIFI_RX_CTRL_MSB_FIRST_POS)
#define uWIFI_RX_CTRL_LSB_FIRST          ((uint32) 0x00u)
#define uWIFI_RX_CTRL_MEDIAN             ((uint32) 0x01u << uWIFI_RX_CTRL_MEDIAN_POS)
#define uWIFI_RX_CTRL_ENABLED            ((uint32) 0x01u << uWIFI_RX_CTRL_ENABLED_POS)


/* uWIFI_RX_FIFO_CTRL_REG */
#define uWIFI_RX_FIFO_CTRL_TRIGGER_LEVEL_POS     (0u)   /* [2:0] Trigger level                            */
#define uWIFI_RX_FIFO_CTRL_CLEAR_POS             (16u)  /* [16]  Clear RX FIFO: clear after set           */
#define uWIFI_RX_FIFO_CTRL_FREEZE_POS            (17u)  /* [17]  Freeze RX FIFO: HW writes has not effect */
#define uWIFI_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK    ((uint32) uWIFI_FF_DATA_NR_LOG2_MASK)
#define uWIFI_RX_FIFO_CTRL_CLEAR                 ((uint32) 0x01u << uWIFI_RX_FIFO_CTRL_CLEAR_POS)
#define uWIFI_RX_FIFO_CTRL_FREEZE                ((uint32) 0x01u << uWIFI_RX_FIFO_CTRL_FREEZE_POS)

/* uWIFI_RX_FIFO_STATUS_REG */
#define uWIFI_RX_FIFO_STATUS_USED_POS    (0u)   /* [3:0]   Amount of entries in RX FIFO */
#define uWIFI_RX_FIFO_SR_VALID_POS       (15u)  /* [15]    Shifter status of RX FIFO    */
#define uWIFI_RX_FIFO_STATUS_RD_PTR_POS  (16u)  /* [18:16] RX FIFO read pointer         */
#define uWIFI_RX_FIFO_STATUS_WR_PTR_POS  (24u)  /* [26:24] RX FIFO write pointer        */
#define uWIFI_RX_FIFO_STATUS_USED_MASK   ((uint32) uWIFI_FF_DATA_NR_LOG2_PLUS1_MASK)
#define uWIFI_RX_FIFO_SR_VALID           ((uint32) 0x01u << uWIFI_RX_FIFO_SR_VALID_POS)
#define uWIFI_RX_FIFO_STATUS_RD_PTR_MASK ((uint32) uWIFI_FF_DATA_NR_LOG2_MASK << \
                                                                    uWIFI_RX_FIFO_STATUS_RD_PTR_POS)
#define uWIFI_RX_FIFO_STATUS_WR_PTR_MASK ((uint32) uWIFI_FF_DATA_NR_LOG2_MASK << \
                                                                    uWIFI_RX_FIFO_STATUS_WR_PTR_POS)

/* uWIFI_RX_MATCH_REG */
#define uWIFI_RX_MATCH_ADDR_POS     (0u)  /* [7:0]   Slave address                        */
#define uWIFI_RX_MATCH_MASK_POS     (16u) /* [23:16] Slave address mask: 0 - doesn't care */
#define uWIFI_RX_MATCH_ADDR_MASK    ((uint32) 0xFFu)
#define uWIFI_RX_MATCH_MASK_MASK    ((uint32) 0xFFu << uWIFI_RX_MATCH_MASK_POS)

/* uWIFI_RX_FIFO_WR_REG */
#define uWIFI_RX_FIFO_RD_POS    (0u)  /* [15:0] Data read from RX FIFO */
#define uWIFI_RX_FIFO_RD_MASK   ((uint32) 0xFFu)

/* uWIFI_RX_FIFO_RD_SILENT_REG */
#define uWIFI_RX_FIFO_RD_SILENT_POS     (0u)  /* [15:0] Data read from RX FIFO: not remove data from FIFO */
#define uWIFI_RX_FIFO_RD_SILENT_MASK    ((uint32) 0xFFu)

/* uWIFI_RX_FIFO_RD_SILENT_REG */
#define uWIFI_RX_FIFO_RD_SILENT_POS     (0u)  /* [15:0] Data read from RX FIFO: not remove data from FIFO */
#define uWIFI_RX_FIFO_RD_SILENT_MASK    ((uint32) 0xFFu)

/* uWIFI_EZBUF_DATA_REG */
#define uWIFI_EZBUF_DATA_POS   (0u)  /* [7:0] Data from EZ Memory */
#define uWIFI_EZBUF_DATA_MASK  ((uint32) 0xFFu)

/*  uWIFI_INTR_CAUSE_REG */
#define uWIFI_INTR_CAUSE_MASTER_POS  (0u)  /* [0] Master interrupt active                 */
#define uWIFI_INTR_CAUSE_SLAVE_POS   (1u)  /* [1] Slave interrupt active                  */
#define uWIFI_INTR_CAUSE_TX_POS      (2u)  /* [2] Transmitter interrupt active            */
#define uWIFI_INTR_CAUSE_RX_POS      (3u)  /* [3] Receiver interrupt active               */
#define uWIFI_INTR_CAUSE_I2C_EC_POS  (4u)  /* [4] Externally clock I2C interrupt active   */
#define uWIFI_INTR_CAUSE_SPI_EC_POS  (5u)  /* [5] Externally clocked SPI interrupt active */
#define uWIFI_INTR_CAUSE_MASTER      ((uint32) 0x01u)
#define uWIFI_INTR_CAUSE_SLAVE       ((uint32) 0x01u << uWIFI_INTR_CAUSE_SLAVE_POS)
#define uWIFI_INTR_CAUSE_TX          ((uint32) 0x01u << uWIFI_INTR_CAUSE_TX_POS)
#define uWIFI_INTR_CAUSE_RX          ((uint32) 0x01u << uWIFI_INTR_CAUSE_RX_POS)
#define uWIFI_INTR_CAUSE_I2C_EC      ((uint32) 0x01u << uWIFI_INTR_CAUSE_I2C_EC_POS)
#define uWIFI_INTR_CAUSE_SPI_EC      ((uint32) 0x01u << uWIFI_INTR_CAUSE_SPI_EC_POS)

/* uWIFI_INTR_SPI_EC_REG, uWIFI_INTR_SPI_EC_MASK_REG, uWIFI_INTR_SPI_EC_MASKED_REG */
#define uWIFI_INTR_SPI_EC_WAKE_UP_POS          (0u)  /* [0] Address match: triggers wakeup of chip */
#define uWIFI_INTR_SPI_EC_EZBUF_STOP_POS       (1u)  /* [1] Externally clocked Stop detected       */
#define uWIFI_INTR_SPI_EC_EZBUF_WRITE_STOP_POS (2u)  /* [2] Externally clocked Write Stop detected */
#define uWIFI_INTR_SPI_EC_WAKE_UP              ((uint32) 0x01u)
#define uWIFI_INTR_SPI_EC_EZBUF_STOP           ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SPI_EC_EZBUF_STOP_POS)
#define uWIFI_INTR_SPI_EC_EZBUF_WRITE_STOP     ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SPI_EC_EZBUF_WRITE_STOP_POS)

/* uWIFI_INTR_I2C_EC, uWIFI_INTR_I2C_EC_MASK, uWIFI_INTR_I2C_EC_MASKED */
#define uWIFI_INTR_I2C_EC_WAKE_UP_POS          (0u)  /* [0] Address match: triggers wakeup of chip */
#define uWIFI_INTR_I2C_EC_EZBUF_STOP_POS       (1u)  /* [1] Externally clocked Stop detected       */
#define uWIFI_INTR_I2C_EC_EZBUF_WRITE_STOP_POS (2u)  /* [2] Externally clocked Write Stop detected */
#define uWIFI_INTR_I2C_EC_WAKE_UP              ((uint32) 0x01u)
#define uWIFI_INTR_I2C_EC_EZBUF_STOP           ((uint32) 0x01u << \
                                                                    uWIFI_INTR_I2C_EC_EZBUF_STOP_POS)
#define uWIFI_INTR_I2C_EC_EZBUF_WRITE_STOP     ((uint32) 0x01u << \
                                                                    uWIFI_INTR_I2C_EC_EZBUF_WRITE_STOP_POS)

/* uWIFI_INTR_MASTER, uWIFI_INTR_MASTER_SET,
   uWIFI_INTR_MASTER_MASK, uWIFI_INTR_MASTER_MASKED */
#define uWIFI_INTR_MASTER_I2C_ARB_LOST_POS   (0u)  /* [0] Master lost arbitration                          */
#define uWIFI_INTR_MASTER_I2C_NACK_POS       (1u)  /* [1] Master receives NACK: address or write to slave  */
#define uWIFI_INTR_MASTER_I2C_ACK_POS        (2u)  /* [2] Master receives NACK: address or write to slave  */
#define uWIFI_INTR_MASTER_I2C_STOP_POS       (4u)  /* [4] Master detects the Stop: only self generated Stop*/
#define uWIFI_INTR_MASTER_I2C_BUS_ERROR_POS  (8u)  /* [8] Master detects bus error: misplaced Start or Stop*/
#define uWIFI_INTR_MASTER_SPI_DONE_POS       (9u)  /* [9] Master complete transfer: Only for SPI           */
#define uWIFI_INTR_MASTER_I2C_ARB_LOST       ((uint32) 0x01u)
#define uWIFI_INTR_MASTER_I2C_NACK           ((uint32) 0x01u << uWIFI_INTR_MASTER_I2C_NACK_POS)
#define uWIFI_INTR_MASTER_I2C_ACK            ((uint32) 0x01u << uWIFI_INTR_MASTER_I2C_ACK_POS)
#define uWIFI_INTR_MASTER_I2C_STOP           ((uint32) 0x01u << uWIFI_INTR_MASTER_I2C_STOP_POS)
#define uWIFI_INTR_MASTER_I2C_BUS_ERROR      ((uint32) 0x01u << \
                                                                    uWIFI_INTR_MASTER_I2C_BUS_ERROR_POS)
#define uWIFI_INTR_MASTER_SPI_DONE           ((uint32) 0x01u << uWIFI_INTR_MASTER_SPI_DONE_POS)

/*
* uWIFI_INTR_SLAVE, uWIFI_INTR_SLAVE_SET,
* uWIFI_INTR_SLAVE_MASK, uWIFI_INTR_SLAVE_MASKED
*/
#define uWIFI_INTR_SLAVE_I2C_ARB_LOST_POS         (0u)  /* [0]  Slave lost arbitration                   */
#define uWIFI_INTR_SLAVE_I2C_NACK_POS             (1u)  /* [1]  Slave receives NACK: master reads data   */
#define uWIFI_INTR_SLAVE_I2C_ACK_POS              (2u)  /* [2]  Slave receives ACK: master reads data    */
#define uWIFI_INTR_SLAVE_I2C_WRITE_STOP_POS       (3u)  /* [3]  Slave detects end of write transaction   */
#define uWIFI_INTR_SLAVE_I2C_STOP_POS             (4u)  /* [4]  Slave detects end of transaction intended */
#define uWIFI_INTR_SLAVE_I2C_START_POS            (5u)  /* [5]  Slave detects Start                      */
#define uWIFI_INTR_SLAVE_I2C_ADDR_MATCH_POS       (6u)  /* [6]  Slave address matches                    */
#define uWIFI_INTR_SLAVE_I2C_GENERAL_POS          (7u)  /* [7]  General call received                    */
#define uWIFI_INTR_SLAVE_I2C_BUS_ERROR_POS        (8u)  /* [8]  Slave detects bus error                  */
#define uWIFI_INTR_SLAVE_SPI_EZBUF_WRITE_STOP_POS (9u)  /* [9]  Slave write complete: Only for SPI       */
#define uWIFI_INTR_SLAVE_SPI_EZBUF_STOP_POS       (10u) /* [10] Slave end of transaction: Only for SPI   */
#define uWIFI_INTR_SLAVE_SPI_BUS_ERROR_POS        (11u) /* [11] Slave detects bus error: Only for SPI    */
#define uWIFI_INTR_SLAVE_I2C_ARB_LOST             ((uint32) 0x01u)
#define uWIFI_INTR_SLAVE_I2C_NACK                 ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_I2C_NACK_POS)
#define uWIFI_INTR_SLAVE_I2C_ACK                  ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_I2C_ACK_POS)
#define uWIFI_INTR_SLAVE_I2C_WRITE_STOP           ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_I2C_WRITE_STOP_POS)
#define uWIFI_INTR_SLAVE_I2C_STOP                 ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_I2C_STOP_POS)
#define uWIFI_INTR_SLAVE_I2C_START                ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_I2C_START_POS)
#define uWIFI_INTR_SLAVE_I2C_ADDR_MATCH           ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_I2C_ADDR_MATCH_POS)
#define uWIFI_INTR_SLAVE_I2C_GENERAL              ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_I2C_GENERAL_POS)
#define uWIFI_INTR_SLAVE_I2C_BUS_ERROR            ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_I2C_BUS_ERROR_POS)
#define uWIFI_INTR_SLAVE_SPI_EZBUF_WRITE_STOP     ((uint32) 0x01u << \
                                                                   uWIFI_INTR_SLAVE_SPI_EZBUF_WRITE_STOP_POS)
#define uWIFI_INTR_SLAVE_SPI_EZBUF_STOP           ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_SPI_EZBUF_STOP_POS)
#define uWIFI_INTR_SLAVE_SPI_BUS_ERROR           ((uint32) 0x01u << \
                                                                    uWIFI_INTR_SLAVE_SPI_BUS_ERROR_POS)

/*
* uWIFI_INTR_TX, uWIFI_INTR_TX_SET,
* uWIFI_INTR_TX_MASK, uWIFI_INTR_TX_MASKED
*/
#define uWIFI_INTR_TX_TRIGGER_POS        (0u)  /* [0]  Trigger on TX FIFO entires                       */
#define uWIFI_INTR_TX_NOT_FULL_POS       (1u)  /* [1]  TX FIFO is not full                              */
#define uWIFI_INTR_TX_EMPTY_POS          (4u)  /* [4]  TX FIFO is empty                                 */
#define uWIFI_INTR_TX_OVERFLOW_POS       (5u)  /* [5]  Attempt to write to a full TX FIFO               */
#define uWIFI_INTR_TX_UNDERFLOW_POS      (6u)  /* [6]  Attempt to read from an empty TX FIFO            */
#define uWIFI_INTR_TX_BLOCKED_POS        (7u)  /* [7]  No access to the EZ memory                       */
#define uWIFI_INTR_TX_UART_NACK_POS      (8u)  /* [8]  UART transmitter received a NACK: SmartCard mode */
#define uWIFI_INTR_TX_UART_DONE_POS      (9u)  /* [9]  UART transmitter done even                       */
#define uWIFI_INTR_TX_UART_ARB_LOST_POS  (10u) /* [10] UART lost arbitration: LIN or SmartCard          */
#define uWIFI_INTR_TX_TRIGGER            ((uint32) 0x01u)
#define uWIFI_INTR_TX_FIFO_LEVEL         (uWIFI_INTR_TX_TRIGGER)
#define uWIFI_INTR_TX_NOT_FULL           ((uint32) 0x01u << uWIFI_INTR_TX_NOT_FULL_POS)
#define uWIFI_INTR_TX_EMPTY              ((uint32) 0x01u << uWIFI_INTR_TX_EMPTY_POS)
#define uWIFI_INTR_TX_OVERFLOW           ((uint32) 0x01u << uWIFI_INTR_TX_OVERFLOW_POS)
#define uWIFI_INTR_TX_UNDERFLOW          ((uint32) 0x01u << uWIFI_INTR_TX_UNDERFLOW_POS)
#define uWIFI_INTR_TX_BLOCKED            ((uint32) 0x01u << uWIFI_INTR_TX_BLOCKED_POS)
#define uWIFI_INTR_TX_UART_NACK          ((uint32) 0x01u << uWIFI_INTR_TX_UART_NACK_POS)
#define uWIFI_INTR_TX_UART_DONE          ((uint32) 0x01u << uWIFI_INTR_TX_UART_DONE_POS)
#define uWIFI_INTR_TX_UART_ARB_LOST      ((uint32) 0x01u << uWIFI_INTR_TX_UART_ARB_LOST_POS)

/*
* uWIFI_INTR_RX, uWIFI_INTR_RX_SET,
* uWIFI_INTR_RX_MASK, uWIFI_INTR_RX_MASKED
*/
#define uWIFI_INTR_RX_TRIGGER_POS        (0u)   /* [0]  Trigger on RX FIFO entires            */
#define uWIFI_INTR_RX_NOT_EMPTY_POS      (2u)   /* [2]  RX FIFO is not empty                  */
#define uWIFI_INTR_RX_FULL_POS           (3u)   /* [3]  RX FIFO is full                       */
#define uWIFI_INTR_RX_OVERFLOW_POS       (5u)   /* [5]  Attempt to write to a full RX FIFO    */
#define uWIFI_INTR_RX_UNDERFLOW_POS      (6u)   /* [6]  Attempt to read from an empty RX FIFO */
#define uWIFI_INTR_RX_BLOCKED_POS        (7u)   /* [7]  No access to the EZ memory            */
#define uWIFI_INTR_RX_FRAME_ERROR_POS    (8u)   /* [8]  Frame error in received data frame    */
#define uWIFI_INTR_RX_PARITY_ERROR_POS   (9u)   /* [9]  Parity error in received data frame   */
#define uWIFI_INTR_RX_BAUD_DETECT_POS    (10u)  /* [10] LIN baud rate detection is completed   */
#define uWIFI_INTR_RX_BREAK_DETECT_POS   (11u)  /* [11] Break detection is successful         */
#define uWIFI_INTR_RX_TRIGGER            ((uint32) 0x01u)
#define uWIFI_INTR_RX_FIFO_LEVEL         (uWIFI_INTR_RX_TRIGGER)
#define uWIFI_INTR_RX_NOT_EMPTY          ((uint32) 0x01u << uWIFI_INTR_RX_NOT_EMPTY_POS)
#define uWIFI_INTR_RX_FULL               ((uint32) 0x01u << uWIFI_INTR_RX_FULL_POS)
#define uWIFI_INTR_RX_OVERFLOW           ((uint32) 0x01u << uWIFI_INTR_RX_OVERFLOW_POS)
#define uWIFI_INTR_RX_UNDERFLOW          ((uint32) 0x01u << uWIFI_INTR_RX_UNDERFLOW_POS)
#define uWIFI_INTR_RX_BLOCKED            ((uint32) 0x01u << uWIFI_INTR_RX_BLOCKED_POS)
#define uWIFI_INTR_RX_FRAME_ERROR        ((uint32) 0x01u << uWIFI_INTR_RX_FRAME_ERROR_POS)
#define uWIFI_INTR_RX_PARITY_ERROR       ((uint32) 0x01u << uWIFI_INTR_RX_PARITY_ERROR_POS)
#define uWIFI_INTR_RX_BAUD_DETECT        ((uint32) 0x01u << uWIFI_INTR_RX_BAUD_DETECT_POS)
#define uWIFI_INTR_RX_BREAK_DETECT       ((uint32) 0x01u << uWIFI_INTR_RX_BREAK_DETECT_POS)

/* Define all interrupt sources */
#define uWIFI_INTR_I2C_EC_ALL    (uWIFI_INTR_I2C_EC_WAKE_UP    | \
                                             uWIFI_INTR_I2C_EC_EZBUF_STOP | \
                                             uWIFI_INTR_I2C_EC_EZBUF_WRITE_STOP)

#define uWIFI_INTR_SPI_EC_ALL    (uWIFI_INTR_SPI_EC_WAKE_UP    | \
                                             uWIFI_INTR_SPI_EC_EZBUF_STOP | \
                                             uWIFI_INTR_SPI_EC_EZBUF_WRITE_STOP)

#define uWIFI_INTR_MASTER_ALL    (uWIFI_INTR_MASTER_I2C_ARB_LOST  | \
                                             uWIFI_INTR_MASTER_I2C_NACK      | \
                                             uWIFI_INTR_MASTER_I2C_ACK       | \
                                             uWIFI_INTR_MASTER_I2C_STOP      | \
                                             uWIFI_INTR_MASTER_I2C_BUS_ERROR | \
                                             uWIFI_INTR_MASTER_SPI_DONE)

#define uWIFI_INTR_SLAVE_ALL     (uWIFI_INTR_SLAVE_I2C_ARB_LOST      | \
                                             uWIFI_INTR_SLAVE_I2C_NACK          | \
                                             uWIFI_INTR_SLAVE_I2C_ACK           | \
                                             uWIFI_INTR_SLAVE_I2C_WRITE_STOP    | \
                                             uWIFI_INTR_SLAVE_I2C_STOP          | \
                                             uWIFI_INTR_SLAVE_I2C_START         | \
                                             uWIFI_INTR_SLAVE_I2C_ADDR_MATCH    | \
                                             uWIFI_INTR_SLAVE_I2C_GENERAL       | \
                                             uWIFI_INTR_SLAVE_I2C_BUS_ERROR     | \
                                             uWIFI_INTR_SLAVE_SPI_EZBUF_WRITE_STOP | \
                                             uWIFI_INTR_SLAVE_SPI_EZBUF_STOP       | \
                                             uWIFI_INTR_SLAVE_SPI_BUS_ERROR)

#define uWIFI_INTR_TX_ALL        (uWIFI_INTR_TX_TRIGGER   | \
                                             uWIFI_INTR_TX_NOT_FULL  | \
                                             uWIFI_INTR_TX_EMPTY     | \
                                             uWIFI_INTR_TX_OVERFLOW  | \
                                             uWIFI_INTR_TX_UNDERFLOW | \
                                             uWIFI_INTR_TX_BLOCKED   | \
                                             uWIFI_INTR_TX_UART_NACK | \
                                             uWIFI_INTR_TX_UART_DONE | \
                                             uWIFI_INTR_TX_UART_ARB_LOST)

#define uWIFI_INTR_RX_ALL        (uWIFI_INTR_RX_TRIGGER      | \
                                             uWIFI_INTR_RX_NOT_EMPTY    | \
                                             uWIFI_INTR_RX_FULL         | \
                                             uWIFI_INTR_RX_OVERFLOW     | \
                                             uWIFI_INTR_RX_UNDERFLOW    | \
                                             uWIFI_INTR_RX_BLOCKED      | \
                                             uWIFI_INTR_RX_FRAME_ERROR  | \
                                             uWIFI_INTR_RX_PARITY_ERROR | \
                                             uWIFI_INTR_RX_BAUD_DETECT  | \
                                             uWIFI_INTR_RX_BREAK_DETECT)

/* General usage HW definitions */
#define uWIFI_ONE_BYTE_WIDTH (8u)   /* Number of bits in one byte           */
#define uWIFI_FIFO_SIZE      (8u)   /* Size of TX or RX FIFO: defined by HW */
#define uWIFI_EZBUFFER_SIZE  (32u)  /* EZ Buffer size: defined by HW        */

/* I2C and EZI2C slave address defines */
#define uWIFI_I2C_SLAVE_ADDR_POS    (0x01u)    /* 7-bit address shift */
#define uWIFI_I2C_SLAVE_ADDR_MASK   (0xFEu)    /* 8-bit address mask */

/* OVS constants for IrDA Low Power operation */
#define uWIFI_CTRL_OVS_IRDA_LP_OVS16     (0x00u)
#define uWIFI_CTRL_OVS_IRDA_LP_OVS32     (0x01u)
#define uWIFI_CTRL_OVS_IRDA_LP_OVS48     (0x02u)
#define uWIFI_CTRL_OVS_IRDA_LP_OVS96     (0x03u)
#define uWIFI_CTRL_OVS_IRDA_LP_OVS192    (0x04u)
#define uWIFI_CTRL_OVS_IRDA_LP_OVS768    (0x05u)
#define uWIFI_CTRL_OVS_IRDA_LP_OVS1536   (0x06u)

/* OVS constant for IrDA */
#define uWIFI_CTRL_OVS_IRDA_OVS16        (uWIFI_UART_IRDA_LP_OVS16)


/***************************************
*    Common Macro Definitions
***************************************/

/* Re-enables the SCB IP. A clear enable bit has a different effect
* on the scb IP depending on the version:
*  CY_SCBIP_V0: resets state, status, TX and RX FIFOs.
*  CY_SCBIP_V1 or later: resets state, status, TX and RX FIFOs and interrupt sources.
*/
#define uWIFI_SCB_SW_RESET \
                        do{           \
                            uWIFI_CTRL_REG &= ((uint32) ~uWIFI_CTRL_ENABLED); \
                            uWIFI_CTRL_REG |= ((uint32)  uWIFI_CTRL_ENABLED); \
                        }while(0)

/* TX FIFO macro */
#define uWIFI_CLEAR_TX_FIFO \
                            do{        \
                                uWIFI_TX_FIFO_CTRL_REG |= ((uint32)  uWIFI_TX_FIFO_CTRL_CLEAR); \
                                uWIFI_TX_FIFO_CTRL_REG &= ((uint32) ~uWIFI_TX_FIFO_CTRL_CLEAR); \
                            }while(0)

#define uWIFI_GET_TX_FIFO_ENTRIES    (uWIFI_TX_FIFO_STATUS_REG & \
                                                 uWIFI_TX_FIFO_STATUS_USED_MASK)

#define uWIFI_GET_TX_FIFO_SR_VALID   ((0u != (uWIFI_TX_FIFO_STATUS_REG & \
                                                         uWIFI_TX_FIFO_SR_VALID)) ? (1u) : (0u))

/* RX FIFO macro */
#define uWIFI_CLEAR_RX_FIFO \
                            do{        \
                                uWIFI_RX_FIFO_CTRL_REG |= ((uint32)  uWIFI_RX_FIFO_CTRL_CLEAR); \
                                uWIFI_RX_FIFO_CTRL_REG &= ((uint32) ~uWIFI_RX_FIFO_CTRL_CLEAR); \
                            }while(0)

#define uWIFI_GET_RX_FIFO_ENTRIES    (uWIFI_RX_FIFO_STATUS_REG & \
                                                    uWIFI_RX_FIFO_STATUS_USED_MASK)

#define uWIFI_GET_RX_FIFO_SR_VALID   ((0u != (uWIFI_RX_FIFO_STATUS_REG & \
                                                         uWIFI_RX_FIFO_SR_VALID)) ? (1u) : (0u))

/* Write interrupt source: set sourceMask bits in uWIFI_INTR_X_MASK_REG */
#define uWIFI_WRITE_INTR_I2C_EC_MASK(sourceMask) \
                                                do{         \
                                                    uWIFI_INTR_I2C_EC_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_WRITE_INTR_SPI_EC_MASK(sourceMask) \
                                                do{         \
                                                    uWIFI_INTR_SPI_EC_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)
#endif /* (!uWIFI_CY_SCBIP_V1) */

#define uWIFI_WRITE_INTR_MASTER_MASK(sourceMask) \
                                                do{         \
                                                    uWIFI_INTR_MASTER_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_WRITE_INTR_SLAVE_MASK(sourceMask)  \
                                                do{         \
                                                    uWIFI_INTR_SLAVE_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_WRITE_INTR_TX_MASK(sourceMask)     \
                                                do{         \
                                                    uWIFI_INTR_TX_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_WRITE_INTR_RX_MASK(sourceMask)     \
                                                do{         \
                                                    uWIFI_INTR_RX_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

/* Enable interrupt source: set sourceMask bits in uWIFI_INTR_X_MASK_REG */
#define uWIFI_ENABLE_INTR_I2C_EC(sourceMask) \
                                                do{     \
                                                    uWIFI_INTR_I2C_EC_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)
#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_ENABLE_INTR_SPI_EC(sourceMask) \
                                                do{     \
                                                    uWIFI_INTR_SPI_EC_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)
#endif /* (!uWIFI_CY_SCBIP_V1) */

#define uWIFI_ENABLE_INTR_MASTER(sourceMask) \
                                                do{     \
                                                    uWIFI_INTR_MASTER_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_ENABLE_INTR_SLAVE(sourceMask)  \
                                                do{     \
                                                    uWIFI_INTR_SLAVE_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_ENABLE_INTR_TX(sourceMask)     \
                                                do{     \
                                                    uWIFI_INTR_TX_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_ENABLE_INTR_RX(sourceMask)     \
                                                do{     \
                                                    uWIFI_INTR_RX_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)

/* Disable interrupt source: clear sourceMask bits in uWIFI_INTR_X_MASK_REG */
#define uWIFI_DISABLE_INTR_I2C_EC(sourceMask) \
                                do{                      \
                                    uWIFI_INTR_I2C_EC_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                }while(0)

#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_DISABLE_INTR_SPI_EC(sourceMask) \
                                do{                      \
                                    uWIFI_INTR_SPI_EC_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                 }while(0)
#endif /* (!uWIFI_CY_SCBIP_V1) */

#define uWIFI_DISABLE_INTR_MASTER(sourceMask) \
                                do{                      \
                                uWIFI_INTR_MASTER_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                }while(0)

#define uWIFI_DISABLE_INTR_SLAVE(sourceMask) \
                                do{                     \
                                    uWIFI_INTR_SLAVE_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                }while(0)

#define uWIFI_DISABLE_INTR_TX(sourceMask)    \
                                do{                     \
                                    uWIFI_INTR_TX_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                 }while(0)

#define uWIFI_DISABLE_INTR_RX(sourceMask)    \
                                do{                     \
                                    uWIFI_INTR_RX_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                }while(0)

/* Set interrupt sources: write sourceMask bits in uWIFI_INTR_X_SET_REG */
#define uWIFI_SET_INTR_MASTER(sourceMask)    \
                                                do{     \
                                                    uWIFI_INTR_MASTER_SET_REG = (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_SET_INTR_SLAVE(sourceMask) \
                                                do{ \
                                                    uWIFI_INTR_SLAVE_SET_REG = (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_SET_INTR_TX(sourceMask)    \
                                                do{ \
                                                    uWIFI_INTR_TX_SET_REG = (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_SET_INTR_RX(sourceMask)    \
                                                do{ \
                                                    uWIFI_INTR_RX_SET_REG = (uint32) (sourceMask); \
                                                }while(0)

/* Clear interrupt sources: write sourceMask bits in uWIFI_INTR_X_REG */
#define uWIFI_CLEAR_INTR_I2C_EC(sourceMask)  \
                                                do{     \
                                                    uWIFI_INTR_I2C_EC_REG = (uint32) (sourceMask); \
                                                }while(0)

#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_CLEAR_INTR_SPI_EC(sourceMask)  \
                                                do{     \
                                                    uWIFI_INTR_SPI_EC_REG = (uint32) (sourceMask); \
                                                }while(0)
#endif /* (!uWIFI_CY_SCBIP_V1) */

#define uWIFI_CLEAR_INTR_MASTER(sourceMask)  \
                                                do{     \
                                                    uWIFI_INTR_MASTER_REG = (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_CLEAR_INTR_SLAVE(sourceMask)   \
                                                do{     \
                                                    uWIFI_INTR_SLAVE_REG  = (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_CLEAR_INTR_TX(sourceMask)      \
                                                do{     \
                                                    uWIFI_INTR_TX_REG     = (uint32) (sourceMask); \
                                                }while(0)

#define uWIFI_CLEAR_INTR_RX(sourceMask)      \
                                                do{     \
                                                    uWIFI_INTR_RX_REG     = (uint32) (sourceMask); \
                                                }while(0)

/* Return true if sourceMask is set in uWIFI_INTR_CAUSE_REG */
#define uWIFI_CHECK_CAUSE_INTR(sourceMask)    (0u != (uWIFI_INTR_CAUSE_REG & (sourceMask)))

/* Return true if sourceMask is set in INTR_X_MASKED_REG */
#define uWIFI_CHECK_INTR_I2C_EC(sourceMask)  (0u != (uWIFI_INTR_I2C_EC_REG & (sourceMask)))
#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_CHECK_INTR_SPI_EC(sourceMask)  (0u != (uWIFI_INTR_SPI_EC_REG & (sourceMask)))
#endif /* (!uWIFI_CY_SCBIP_V1) */
#define uWIFI_CHECK_INTR_MASTER(sourceMask)  (0u != (uWIFI_INTR_MASTER_REG & (sourceMask)))
#define uWIFI_CHECK_INTR_SLAVE(sourceMask)   (0u != (uWIFI_INTR_SLAVE_REG  & (sourceMask)))
#define uWIFI_CHECK_INTR_TX(sourceMask)      (0u != (uWIFI_INTR_TX_REG     & (sourceMask)))
#define uWIFI_CHECK_INTR_RX(sourceMask)      (0u != (uWIFI_INTR_RX_REG     & (sourceMask)))

/* Return true if sourceMask is set in uWIFI_INTR_X_MASKED_REG */
#define uWIFI_CHECK_INTR_I2C_EC_MASKED(sourceMask)   (0u != (uWIFI_INTR_I2C_EC_MASKED_REG & \
                                                                       (sourceMask)))
#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_CHECK_INTR_SPI_EC_MASKED(sourceMask)   (0u != (uWIFI_INTR_SPI_EC_MASKED_REG & \
                                                                       (sourceMask)))
#endif /* (!uWIFI_CY_SCBIP_V1) */
#define uWIFI_CHECK_INTR_MASTER_MASKED(sourceMask)   (0u != (uWIFI_INTR_MASTER_MASKED_REG & \
                                                                       (sourceMask)))
#define uWIFI_CHECK_INTR_SLAVE_MASKED(sourceMask)    (0u != (uWIFI_INTR_SLAVE_MASKED_REG  & \
                                                                       (sourceMask)))
#define uWIFI_CHECK_INTR_TX_MASKED(sourceMask)       (0u != (uWIFI_INTR_TX_MASKED_REG     & \
                                                                       (sourceMask)))
#define uWIFI_CHECK_INTR_RX_MASKED(sourceMask)       (0u != (uWIFI_INTR_RX_MASKED_REG     & \
                                                                       (sourceMask)))

/* Return true if sourceMask is set in uWIFI_CTRL_REG: generally is used to check enable bit */
#define uWIFI_GET_CTRL_ENABLED    (0u != (uWIFI_CTRL_REG & uWIFI_CTRL_ENABLED))

#define uWIFI_CHECK_SLAVE_AUTO_ADDR_NACK     (0u != (uWIFI_I2C_CTRL_REG & \
                                                                uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK))


/***************************************
*      I2C Macro Definitions
***************************************/

/* Enable auto ACK/NACK */
#define uWIFI_ENABLE_SLAVE_AUTO_ADDR_NACK \
                            do{                      \
                                uWIFI_I2C_CTRL_REG |= uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK; \
                            }while(0)

#define uWIFI_ENABLE_SLAVE_AUTO_DATA_ACK \
                            do{                     \
                                uWIFI_I2C_CTRL_REG |= uWIFI_I2C_CTRL_S_READY_DATA_ACK; \
                            }while(0)

#define uWIFI_ENABLE_SLAVE_AUTO_DATA_NACK \
                            do{                      \
                                uWIFI_I2C_CTRL_REG |= uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK; \
                            }while(0)

#define uWIFI_ENABLE_MASTER_AUTO_DATA_ACK \
                            do{                      \
                                uWIFI_I2C_CTRL_REG |= uWIFI_I2C_CTRL_M_READY_DATA_ACK; \
                            }while(0)

#define uWIFI_ENABLE_MASTER_AUTO_DATA_NACK \
                            do{                       \
                                uWIFI_I2C_CTRL_REG |= uWIFI_I2C_CTRL_M_NOT_READY_DATA_NACK; \
                            }while(0)

/* Disable auto ACK/NACK */
#define uWIFI_DISABLE_SLAVE_AUTO_ADDR_NACK \
                            do{                       \
                                uWIFI_I2C_CTRL_REG &= ~uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK; \
                            }while(0)

#define uWIFI_DISABLE_SLAVE_AUTO_DATA_ACK \
                            do{                      \
                                uWIFI_I2C_CTRL_REG &= ~uWIFI_I2C_CTRL_S_READY_DATA_ACK; \
                            }while(0)

#define uWIFI_DISABLE_SLAVE_AUTO_DATA_NACK \
                            do{                       \
                                uWIFI_I2C_CTRL_REG &= ~uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK; \
                            }while(0)

#define uWIFI_DISABLE_MASTER_AUTO_DATA_ACK \
                            do{                       \
                                uWIFI_I2C_CTRL_REG &= ~uWIFI_I2C_CTRL_M_READY_DATA_ACK; \
                            }while(0)

#define uWIFI_DISABLE_MASTER_AUTO_DATA_NACK \
                            do{                        \
                                uWIFI_I2C_CTRL_REG &= ~uWIFI_I2C_CTRL_M_NOT_READY_DATA_NACK; \
                            }while(0)

/* Enable Slave autoACK/NACK Data */
#define uWIFI_ENABLE_SLAVE_AUTO_DATA \
                            do{                 \
                                uWIFI_I2C_CTRL_REG |= (uWIFI_I2C_CTRL_S_READY_DATA_ACK |      \
                                                                  uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK); \
                            }while(0)

/* Disable Slave autoACK/NACK Data */
#define uWIFI_DISABLE_SLAVE_AUTO_DATA \
                            do{                  \
                                uWIFI_I2C_CTRL_REG &= ((uint32) \
                                                                  ~(uWIFI_I2C_CTRL_S_READY_DATA_ACK |       \
                                                                    uWIFI_I2C_CTRL_S_NOT_READY_DATA_NACK)); \
                            }while(0)

/* Disable Master autoACK/NACK Data */
#define uWIFI_DISABLE_MASTER_AUTO_DATA \
                            do{                   \
                                uWIFI_I2C_CTRL_REG &= ((uint32) \
                                                                  ~(uWIFI_I2C_CTRL_M_READY_DATA_ACK |       \
                                                                    uWIFI_I2C_CTRL_M_NOT_READY_DATA_NACK)); \
                            }while(0)

/* Master commands */
#define uWIFI_I2C_MASTER_GENERATE_START \
                            do{                    \
                                uWIFI_I2C_MASTER_CMD_REG = uWIFI_I2C_MASTER_CMD_M_START_ON_IDLE; \
                            }while(0)

#define uWIFI_I2C_MASTER_CLEAR_START \
                            do{                 \
                                uWIFI_I2C_MASTER_CMD_REG =  ((uint32) 0u); \
                            }while(0)

#define uWIFI_I2C_MASTER_GENERATE_RESTART uWIFI_I2CReStartGeneration()

#define uWIFI_I2C_MASTER_GENERATE_STOP \
                            do{                   \
                                uWIFI_I2C_MASTER_CMD_REG =                                            \
                                    (uWIFI_I2C_MASTER_CMD_M_STOP |                                    \
                                        (uWIFI_CHECK_I2C_STATUS(uWIFI_I2C_STATUS_M_READ) ? \
                                            (uWIFI_I2C_MASTER_CMD_M_NACK) : (0u)));                   \
                            }while(0)

#define uWIFI_I2C_MASTER_GENERATE_ACK \
                            do{                  \
                                uWIFI_I2C_MASTER_CMD_REG = uWIFI_I2C_MASTER_CMD_M_ACK; \
                            }while(0)

#define uWIFI_I2C_MASTER_GENERATE_NACK \
                            do{                   \
                                uWIFI_I2C_MASTER_CMD_REG = uWIFI_I2C_MASTER_CMD_M_NACK; \
                            }while(0)

/* Slave commands */
#define uWIFI_I2C_SLAVE_GENERATE_ACK \
                            do{                 \
                                uWIFI_I2C_SLAVE_CMD_REG = uWIFI_I2C_SLAVE_CMD_S_ACK; \
                            }while(0)

#if (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    /* Slave NACK generation for EC_AM logic on address phase. Ticket ID #183902 */
    void uWIFI_I2CSlaveNackGeneration(void);
    #define uWIFI_I2C_SLAVE_GENERATE_NACK uWIFI_I2CSlaveNackGeneration()

#else
    #define uWIFI_I2C_SLAVE_GENERATE_NACK \
                            do{                      \
                                uWIFI_I2C_SLAVE_CMD_REG = uWIFI_I2C_SLAVE_CMD_S_NACK; \
                            }while(0)
#endif /* (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

#define uWIFI_I2C_SLAVE_CLEAR_NACK \
                            do{               \
                                uWIFI_I2C_SLAVE_CMD_REG = 0u; \
                            }while(0)

/* Return 8-bit address. The input address should be 7-bits */
#define uWIFI_GET_I2C_8BIT_ADDRESS(addr) (((uint32) ((uint32) (addr) << \
                                                                    uWIFI_I2C_SLAVE_ADDR_POS)) & \
                                                                        uWIFI_I2C_SLAVE_ADDR_MASK)

#define uWIFI_GET_I2C_7BIT_ADDRESS(addr) ((uint32) (addr) >> uWIFI_I2C_SLAVE_ADDR_POS)

/* Adjust SDA filter Trim settings */
#define uWIFI_DEFAULT_I2C_CFG_SDA_FILT_TRIM  (0x02u)
#define uWIFI_EC_AM_I2C_CFG_SDA_FILT_TRIM    (0x03u)

#if (uWIFI_CY_SCBIP_V0)
    #define uWIFI_SET_I2C_CFG_SDA_FILT_TRIM(sdaTrim) \
        do{                                                 \
            uWIFI_I2C_CFG_REG =                  \
                            ((uWIFI_I2C_CFG_REG & (uint32) ~uWIFI_I2C_CFG_SDA_FILT_TRIM_MASK) | \
                             ((uint32) ((uint32) (sdaTrim) <<uWIFI_I2C_CFG_SDA_FILT_TRIM_POS)));           \
        }while(0)
#endif /* (uWIFI_CY_SCBIP_V0) */

/* Enable/Disable analog and digital filter */
#define uWIFI_DIGITAL_FILTER_DISABLE    (0u)
#define uWIFI_DIGITAL_FILTER_ENABLE     (1u)
#define uWIFI_I2C_DATA_RATE_FS_MODE_MAX (400u)
#if (uWIFI_CY_SCBIP_V0)
    /* uWIFI_I2C_CFG_SDA_FILT_OUT_ENABLED is disabled by default */
    #define uWIFI_I2C_CFG_FILT_MASK  (uWIFI_I2C_CFG_SDA_FILT_ENABLED | \
                                                 uWIFI_I2C_CFG_SCL_FILT_ENABLED)
#else
    /* uWIFI_I2C_CFG_SDA_OUT_FILT_SEL_MASK is disabled by default */
    #define uWIFI_I2C_CFG_FILT_MASK  (uWIFI_I2C_CFG_SDA_IN_FILT_SEL | \
                                                 uWIFI_I2C_CFG_SCL_IN_FILT_SEL)
#endif /* (uWIFI_CY_SCBIP_V0) */

#define uWIFI_I2C_CFG_ANALOG_FITER_DISABLE \
        do{                                           \
            uWIFI_I2C_CFG_REG &= (uint32) ~uWIFI_I2C_CFG_FILT_MASK; \
        }while(0)

#define uWIFI_I2C_CFG_ANALOG_FITER_ENABLE \
        do{                                          \
            uWIFI_I2C_CFG_REG |= (uint32)  uWIFI_I2C_CFG_FILT_MASK; \
        }while(0)

/* Return slave select number from SPI_CTRL register */
#define uWIFI_GET_SPI_CTRL_SS(activeSelect) (((uint32) ((uint32) (activeSelect) << \
                                                                    uWIFI_SPI_CTRL_SLAVE_SELECT_POS)) & \
                                                                        uWIFI_SPI_CTRL_SLAVE_SELECT_MASK)

/* Return true if bit is set in uWIFI_I2C_STATUS_REG */
#define uWIFI_CHECK_I2C_STATUS(sourceMask)   (0u != (uWIFI_I2C_STATUS_REG & (sourceMask)))

/* Return true if bit is set in uWIFI_SPI_STATUS_REG */
#define uWIFI_CHECK_SPI_STATUS(sourceMask)   (0u != (uWIFI_SPI_STATUS_REG & (sourceMask)))

/* Return FIFO size depends on uWIFI_CTRL_BYTE_MODE bit */
#define uWIFI_GET_FIFO_SIZE(condition) ((0u != (condition)) ? \
                                                    (2u * uWIFI_FIFO_SIZE) : (uWIFI_FIFO_SIZE))


/***************************************
*       Get Macros Definitions
***************************************/

/* uWIFI_CTRL */
#define uWIFI_GET_CTRL_OVS(oversample)       (((uint32) (oversample) - 1u) & uWIFI_CTRL_OVS_MASK)

#define uWIFI_GET_CTRL_EC_OP_MODE(opMode)        ((0u != (opMode)) ? \
                                                                (uWIFI_CTRL_EC_OP_MODE)  : (0u))

#define uWIFI_GET_CTRL_EC_AM_MODE(amMode)        ((0u != (amMode)) ? \
                                                                (uWIFI_CTRL_EC_AM_MODE)  : (0u))

#define uWIFI_GET_CTRL_BLOCK(block)              ((0u != (block))  ? \
                                                                (uWIFI_CTRL_BLOCK)       : (0u))

#define uWIFI_GET_CTRL_ADDR_ACCEPT(acceptAddr)   ((0u != (acceptAddr)) ? \
                                                                (uWIFI_CTRL_ADDR_ACCEPT) : (0u))

#if (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    #define uWIFI_GET_CTRL_BYTE_MODE(mode)   (0u)
#else
    #define uWIFI_GET_CTRL_BYTE_MODE(mode)   ((0u != (mode)) ? \
                                                            (uWIFI_CTRL_BYTE_MODE) : (0u))
#endif /* (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

/* uWIFI_I2C_CTRL */
#define uWIFI_GET_I2C_CTRL_HIGH_PHASE_OVS(oversampleHigh) (((uint32) (oversampleHigh) - 1u) & \
                                                                        uWIFI_I2C_CTRL_HIGH_PHASE_OVS_MASK)

#define uWIFI_GET_I2C_CTRL_LOW_PHASE_OVS(oversampleLow)  ((((uint32) (oversampleLow) - 1u) << \
                                                                    uWIFI_I2C_CTRL_LOW_PHASE_OVS_POS) &  \
                                                                    uWIFI_I2C_CTRL_LOW_PHASE_OVS_MASK)

#define uWIFI_GET_I2C_CTRL_S_NOT_READY_ADDR_NACK(wakeNack) ((0u != (wakeNack)) ? \
                                                            (uWIFI_I2C_CTRL_S_NOT_READY_ADDR_NACK) : (0u))

#define uWIFI_GET_I2C_CTRL_SL_MSTR_MODE(mode)    ((uint32)(mode) << uWIFI_I2C_CTRL_SLAVE_MODE_POS)

/* uWIFI_SPI_CTRL */
#define uWIFI_GET_SPI_CTRL_CONTINUOUS(separate)  ((0u != (separate)) ? \
                                                                (uWIFI_SPI_CTRL_CONTINUOUS) : (0u))

#define uWIFI_GET_SPI_CTRL_SELECT_PRECEDE(mode)  ((0u != (mode)) ? \
                                                                      (uWIFI_SPI_CTRL_SELECT_PRECEDE) : (0u))

#define uWIFI_GET_SPI_CTRL_SCLK_MODE(mode)       (((uint32) (mode) << \
                                                                        uWIFI_SPI_CTRL_CPHA_POS) & \
                                                                        uWIFI_SPI_CTRL_SCLK_MODE_MASK)

#define uWIFI_GET_SPI_CTRL_LATE_MISO_SAMPLE(lateMiso) ((0u != (lateMiso)) ? \
                                                                    (uWIFI_SPI_CTRL_LATE_MISO_SAMPLE) : (0u))

#if (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    #define uWIFI_GET_SPI_CTRL_SCLK_CONTINUOUS(sclkType) (0u)
    #define uWIFI_GET_SPI_CTRL_SSEL_POLARITY(polarity)   (0u)
#else
    #define uWIFI_GET_SPI_CTRL_SCLK_CONTINUOUS(sclkType) ((0u != (sclkType)) ? \
                                                                    (uWIFI_SPI_CTRL_SCLK_CONTINUOUS) : (0u))

    #define uWIFI_GET_SPI_CTRL_SSEL_POLARITY(polarity)   (((uint32) (polarity) << \
                                                                     uWIFI_SPI_CTRL_SSEL0_POLARITY_POS) & \
                                                                     uWIFI_SPI_CTRL_SSEL_POLARITY_MASK)
#endif /* ((uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

#define uWIFI_GET_SPI_CTRL_SUB_MODE(mode)        (((uint32) (mode) << uWIFI_SPI_CTRL_MODE_POS) & \
                                                                                 uWIFI_SPI_CTRL_MODE_MASK)

#define uWIFI_GET_SPI_CTRL_SLAVE_SELECT(select)  (((uint32) (select) << \
                                                                      uWIFI_SPI_CTRL_SLAVE_SELECT_POS) & \
                                                                      uWIFI_SPI_CTRL_SLAVE_SELECT_MASK)

#define uWIFI_GET_SPI_CTRL_MASTER_MODE(mode)     ((0u != (mode)) ? \
                                                                (uWIFI_SPI_CTRL_MASTER) : (0u))

/* uWIFI_UART_CTRL */
#define uWIFI_GET_UART_CTRL_MODE(mode)           (((uint32) (mode) << \
                                                                            uWIFI_UART_CTRL_MODE_POS) & \
                                                                            uWIFI_UART_CTRL_MODE_MASK)

/* uWIFI_UART_RX_CTRL */
#define uWIFI_GET_UART_RX_CTRL_MODE(stopBits)    (((uint32) (stopBits) - 1u) & \
                                                                        uWIFI_UART_RX_CTRL_STOP_BITS_MASK)

#define uWIFI_GET_UART_RX_CTRL_PARITY(parity)    ((0u != (parity)) ? \
                                                                    (uWIFI_UART_RX_CTRL_PARITY) : (0u))

#define uWIFI_GET_UART_RX_CTRL_POLARITY(polarity)    ((0u != (polarity)) ? \
                                                                    (uWIFI_UART_RX_CTRL_POLARITY) : (0u))

#define uWIFI_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(dropErr) ((0u != (dropErr)) ? \
                                                        (uWIFI_UART_RX_CTRL_DROP_ON_PARITY_ERR) : (0u))

#define uWIFI_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(dropErr) ((0u != (dropErr)) ? \
                                                        (uWIFI_UART_RX_CTRL_DROP_ON_FRAME_ERR) : (0u))

#define uWIFI_GET_UART_RX_CTRL_MP_MODE(mpMode)   ((0u != (mpMode)) ? \
                                                        (uWIFI_UART_RX_CTRL_MP_MODE) : (0u))

/* uWIFI_UART_TX_CTRL */
#define uWIFI_GET_UART_TX_CTRL_MODE(stopBits)    (((uint32) (stopBits) - 1u) & \
                                                                uWIFI_UART_RX_CTRL_STOP_BITS_MASK)

#define uWIFI_GET_UART_TX_CTRL_PARITY(parity)    ((0u != (parity)) ? \
                                                               (uWIFI_UART_TX_CTRL_PARITY) : (0u))

#define uWIFI_GET_UART_TX_CTRL_RETRY_NACK(nack)  ((0u != (nack)) ? \
                                                               (uWIFI_UART_TX_CTRL_RETRY_ON_NACK) : (0u))

/* uWIFI_UART_FLOW_CTRL */
#if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    #define uWIFI_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(level)   ( (uint32) (level) & \
                                                                 uWIFI_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK)

    #define uWIFI_GET_UART_FLOW_CTRL_RTS_POLARITY(polarity) ((0u != (polarity)) ? \
                                                                (uWIFI_UART_FLOW_CTRL_RTS_POLARITY) : (0u))

    #define uWIFI_GET_UART_FLOW_CTRL_CTS_POLARITY(polarity) ((0u != (polarity)) ? \
                                                                (uWIFI_UART_FLOW_CTRL_CTS_POLARITY) : (0u))

    #define uWIFI_GET_UART_FLOW_CTRL_CTS_ENABLE(ctsEn)      ((0u != (ctsEn)) ? \
                                                                (uWIFI_UART_FLOW_CTRL_CTS_ENABLE) : (0u))
#endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */

/* uWIFI_RX_CTRL */
#define uWIFI_GET_RX_CTRL_DATA_WIDTH(dataWidth)  (((uint32) (dataWidth) - 1u) & \
                                                                uWIFI_RX_CTRL_DATA_WIDTH_MASK)

#define uWIFI_GET_RX_CTRL_BIT_ORDER(bitOrder)    ((0u != (bitOrder)) ? \
                                                                (uWIFI_RX_CTRL_MSB_FIRST) : (0u))

#define uWIFI_GET_RX_CTRL_MEDIAN(filterEn)       ((0u != (filterEn)) ? \
                                                                (uWIFI_RX_CTRL_MEDIAN) : (0u))

/* uWIFI_RX_MATCH */
#define uWIFI_GET_RX_MATCH_ADDR(addr)    ((uint32) (addr) & uWIFI_RX_MATCH_ADDR_MASK)
#define uWIFI_GET_RX_MATCH_MASK(mask)    (((uint32) (mask) << \
                                                            uWIFI_RX_MATCH_MASK_POS) & \
                                                            uWIFI_RX_MATCH_MASK_MASK)

/* uWIFI_RX_FIFO_CTRL */
#define uWIFI_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(level)  ((uint32) (level) & \
                                                                    uWIFI_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK)

/* uWIFI_TX_CTRL */
#define uWIFI_GET_TX_CTRL_DATA_WIDTH(dataWidth)  (((uint32) (dataWidth) - 1u) & \
                                                                uWIFI_RX_CTRL_DATA_WIDTH_MASK)

#define uWIFI_GET_TX_CTRL_BIT_ORDER(bitOrder)    ((0u != (bitOrder)) ? \
                                                                (uWIFI_TX_CTRL_MSB_FIRST) : (0u))

/* uWIFI_TX_FIFO_CTRL */
#define uWIFI_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(level)  ((uint32) (level) & \
                                                                    uWIFI_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK)

/* Clear register constants for configuration and interrupt mask */
#define uWIFI_CLEAR_REG          ((uint32) (0u))
#define uWIFI_NO_INTR_SOURCES    ((uint32) (0u))
#define uWIFI_DUMMY_PARAM        ((uint32) (0u))
#define uWIFI_SUBMODE_SPI_SLAVE  ((uint32) (0u))

/* Return in case of I2C read error */
#define uWIFI_I2C_INVALID_BYTE   ((uint32) 0xFFFFFFFFu)
#define uWIFI_CHECK_VALID_BYTE   ((uint32) 0x80000000u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

#define uWIFI_CHECK_INTR_EC_I2C(sourceMask)  uWIFI_CHECK_INTR_I2C_EC(sourceMask)
#if (!uWIFI_CY_SCBIP_V1)
    #define uWIFI_CHECK_INTR_EC_SPI(sourceMask)  uWIFI_CHECK_INTR_SPI_EC(sourceMask)
#endif /* (!uWIFI_CY_SCBIP_V1) */

#define uWIFI_CY_SCBIP_V1_I2C_ONLY   (uWIFI_CY_SCBIP_V1)

#endif /* (CY_SCB_uWIFI_H) */


/* [] END OF FILE */
