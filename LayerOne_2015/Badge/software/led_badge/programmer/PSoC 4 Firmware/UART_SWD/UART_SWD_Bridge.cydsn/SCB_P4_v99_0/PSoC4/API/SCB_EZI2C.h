/*******************************************************************************
* File Name: `$INSTANCE_NAME`_EZI2C.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides constants and parameter values for the SCB Component in
*  EZI2C mode.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_EZI2C_`$INSTANCE_NAME`_H)
#define CY_SCB_EZI2C_`$INSTANCE_NAME`_H

#include "`$INSTANCE_NAME`.h"


/***************************************
*   Initial Parameter Constants
****************************************/

#define `$INSTANCE_NAME`_EZI2C_CLOCK_STRETCHING         (`$EzI2cClockStretching`u)
#define `$INSTANCE_NAME`_EZI2C_MEDIAN_FILTER_ENABLE     (`$EzI2cMedianFilterEnable`u)
#define `$INSTANCE_NAME`_EZI2C_NUMBER_OF_ADDRESSES      (`$EzI2cNumberOfAddresses`u)
#define `$INSTANCE_NAME`_EZI2C_PRIMARY_SLAVE_ADDRESS    (`$EzI2cPrimarySlaveAddress`u)
#define `$INSTANCE_NAME`_EZI2C_SECONDARY_SLAVE_ADDRESS  (`$EzI2cSecondarySlaveAddress`u)
#define `$INSTANCE_NAME`_EZI2C_SUB_ADDRESS_SIZE         (`$EzI2cSubAddressSize`u)
#define `$INSTANCE_NAME`_EZI2C_WAKE_ENABLE              (`$EzI2cWakeEnable`u)
#define `$INSTANCE_NAME`_EZI2C_DATA_RATE                (`$EzI2cDataRate`u)
#define `$INSTANCE_NAME`_EZI2C_SLAVE_ADDRESS_MASK       (`$EzI2cSlaveAddressMask`u)


/***************************************
*  Conditional Compilation Parameters
****************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)

    #define `$INSTANCE_NAME`_SUB_ADDRESS_SIZE16             (0u != `$INSTANCE_NAME`_subAddrSize)
    #define `$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST       (1u)

    #define `$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE       (0u != `$INSTANCE_NAME`_numberOfAddr)
    #define `$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST (1u)

    #define `$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST  (1u)
    #define `$INSTANCE_NAME`_EZI2C_SCL_STRETCH_DISABLE_CONST (1u)
    
    #define `$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST         (1u)

#else

    #define `$INSTANCE_NAME`_SUB_ADDRESS_SIZE16         (0u != `$INSTANCE_NAME`_EZI2C_SUB_ADDRESS_SIZE)
    #define `$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST   (`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16)

    #define `$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE        (0u != `$INSTANCE_NAME`_EZI2C_NUMBER_OF_ADDRESSES)
    #define `$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST  (`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE)

    #define `$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST  (0u != `$INSTANCE_NAME`_EZI2C_CLOCK_STRETCHING)
    #define `$INSTANCE_NAME`_EZI2C_SCL_STRETCH_DISABLE_CONST (!`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST)

    #define `$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST    (0u != `$INSTANCE_NAME`_EZI2C_WAKE_ENABLE)

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */

#define `$INSTANCE_NAME`_EZI2C_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_`$INSTANCE_NAME`) || \
                                                   (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/***************************************
*       Type Definitions
***************************************/

typedef struct
{
    uint32 enableClockStretch;
    uint32 enableMedianFilter;
    uint32 numberOfAddresses;
    uint32 primarySlaveAddr;
    uint32 secondarySlaveAddr;
    uint32 subAddrSize;
    uint32 enableWake;
} `$INSTANCE_NAME`_EZI2C_INIT_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    void `$INSTANCE_NAME`_EzI2CInit(const `$INSTANCE_NAME`_EZI2C_INIT_STRUCT *config);
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */

uint32 `$INSTANCE_NAME`_EzI2CGetActivity(void);

void   `$INSTANCE_NAME`_EzI2CSetAddress1(uint32 address);
uint32 `$INSTANCE_NAME`_EzI2CGetAddress1(void);
void   `$INSTANCE_NAME`_EzI2CSetBuffer1(uint32 bufSize, uint32 rwBoundary, volatile uint8 * buffer);
void   `$INSTANCE_NAME`_EzI2CSetReadBoundaryBuffer1(uint32 rwBoundary);

#if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST)
    void   `$INSTANCE_NAME`_EzI2CSetAddress2(uint32 address);
    uint32 `$INSTANCE_NAME`_EzI2CGetAddress2(void);
    void   `$INSTANCE_NAME`_EzI2CSetBuffer2(uint32 bufSize, uint32 rwBoundary, volatile uint8 * buffer);
    void   `$INSTANCE_NAME`_EzI2CSetReadBoundaryBuffer2(uint32 rwBoundary);
#endif /* (`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST) */

#if(`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST)
    CY_ISR_PROTO(`$INSTANCE_NAME`_EZI2C_STRETCH_ISR);
#endif /* (`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST) */

#if(`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_DISABLE_CONST)
    CY_ISR_PROTO(`$INSTANCE_NAME`_EZI2C_NO_STRETCH_ISR);
#endif /* (`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_DISABLE) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void `$INSTANCE_NAME`_EZI2CCyBtldrCommStart(void);
    void `$INSTANCE_NAME`_EZI2CCyBtldrCommStop (void);
    void `$INSTANCE_NAME`_EZI2CCyBtldrCommReset(void);
    cystatus `$INSTANCE_NAME`_EZI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus `$INSTANCE_NAME`_EZI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (`$INSTANCE_NAME`_EZI2C_BTLDR_COMM_ENABLED) */


/***************************************
*            API Constants
***************************************/

/* Configuration structure constants */
#define `$INSTANCE_NAME`_EZI2C_ONE_ADDRESS      (0u)
#define `$INSTANCE_NAME`_EZI2C_TWO_ADDRESSES    (1u)

#define `$INSTANCE_NAME`_EZI2C_PRIMARY_ADDRESS  (0u)
#define `$INSTANCE_NAME`_EZI2C_BOTH_ADDRESSES   (1u)

#define `$INSTANCE_NAME`_EZI2C_SUB_ADDR8_BITS   (0u)
#define `$INSTANCE_NAME`_EZI2C_SUB_ADDR16_BITS  (1u)

/* `$INSTANCE_NAME`_EzI2CGetActivity() return constants */
#define `$INSTANCE_NAME`_EZI2C_STATUS_SECOND_OFFSET (1u)
#define `$INSTANCE_NAME`_EZI2C_STATUS_READ1     ((uint32) (`$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK))        /* Bit [1]   */
#define `$INSTANCE_NAME`_EZI2C_STATUS_WRITE1    ((uint32) (`$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP))  /* Bit [3]   */
#define `$INSTANCE_NAME`_EZI2C_STATUS_READ2     ((uint32) (`$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK >> \
                                                           `$INSTANCE_NAME`_EZI2C_STATUS_SECOND_OFFSET)) /* Bit [0]   */
#define `$INSTANCE_NAME`_EZI2C_STATUS_WRITE2    ((uint32) (`$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP >> \
                                                           `$INSTANCE_NAME`_EZI2C_STATUS_SECOND_OFFSET)) /* Bit [2]   */
#define `$INSTANCE_NAME`_EZI2C_STATUS_ERR       ((uint32) (0x10u))                                       /* Bit [4]   */
#define `$INSTANCE_NAME`_EZI2C_STATUS_BUSY      ((uint32) (0x20u))                                       /* Bit [5]   */
#define `$INSTANCE_NAME`_EZI2C_CLEAR_STATUS     ((uint32) (0x1Fu))                                       /* Bits [0-4]*/
#define `$INSTANCE_NAME`_EZI2C_CMPLT_INTR_MASK  ((uint32) (`$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK | \
                                                           `$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP))

/***************************************
*     Vars with External Linkage
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    extern const `$INSTANCE_NAME`_EZI2C_INIT_STRUCT `$INSTANCE_NAME`_configEzI2C;
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*           FSM states
***************************************/

/* Returns to master when there is no data to transmit */
#define `$INSTANCE_NAME`_EZI2C_OVFL_RETURN  (0xFFu)

/* States of EZI2C Slave FSM */
#define `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_HI8 ((uint8) (0x02)) /* Idle state for 1 addr: waits for high byte offset */
#define `$INSTANCE_NAME`_EZI2C_FSM_OFFSET_LO8 ((uint8) (0x01)) /* Idle state for 2 addr: waits for low byte offset  */
#define `$INSTANCE_NAME`_EZI2C_FSM_BYTE_WRITE ((uint8) (0x00)) /* Data byte write state: byte by byte mode          */
#define `$INSTANCE_NAME`_EZI2C_FSM_WAIT_STOP  ((uint8) (0x04)) /* Discards written bytes as they do not match write
                                                                  criteria    */

#define `$INSTANCE_NAME`_EZI2C_CONTINUE_TRANSFER    (1u)
#define `$INSTANCE_NAME`_EZI2C_COMPLETE_TRANSFER    (0u)

#define `$INSTANCE_NAME`_EZI2C_NACK_RECEIVED_ADDRESS  (0u)
#define `$INSTANCE_NAME`_EZI2C_ACK_RECEIVED_ADDRESS   (1u)

#define `$INSTANCE_NAME`_EZI2C_ACTIVE_ADDRESS1  (0u)
#define `$INSTANCE_NAME`_EZI2C_ACTIVE_ADDRESS2  (1u)


/***************************************
*       Macro Definitions
***************************************/

/* Access to gloabal variables */
#if(`$INSTANCE_NAME`_SECONDARY_ADDRESS_ENABLE_CONST)

    #define `$INSTANCE_NAME`_EZI2C_UPDATE_LOC_STATUS(activeAddress, locStatus) \
            do{ \
                (locStatus) >>= (activeAddress); \
            }while(0)

    #define `$INSTANCE_NAME`_EZI2C_GET_INDEX(activeAddress) \
            ((`$INSTANCE_NAME`_EZI2C_ACTIVE_ADDRESS1 == (activeAddress)) ? \
                ((uint32) `$INSTANCE_NAME`_indexBuf1) :                  \
                ((uint32) `$INSTANCE_NAME`_indexBuf2))

    #define `$INSTANCE_NAME`_EZI2C_GET_OFFSET(activeAddress) \
            ((`$INSTANCE_NAME`_EZI2C_ACTIVE_ADDRESS1 == (activeAddress)) ? \
                ((uint32) `$INSTANCE_NAME`_offsetBuf1) :                   \
                ((uint32) `$INSTANCE_NAME`_offsetBuf2))

    #define `$INSTANCE_NAME`_EZI2C_SET_INDEX(activeAddress, locIndex) \
            do{ \
                if(`$INSTANCE_NAME`_EZI2C_ACTIVE_ADDRESS1 == (activeAddress)) \
                {    \
                    `$INSTANCE_NAME`_indexBuf1 = (`$EzI2cBitWidthReplacementString`) (locIndex); \
                }    \
                else \
                {    \
                    `$INSTANCE_NAME`_indexBuf2 = (`$EzI2cBitWidthReplacementString`) (locIndex); \
                }    \
            }while(0)

    #define `$INSTANCE_NAME`_EZI2C_SET_OFFSET(activeAddress, locOffset) \
            do{ \
                if(`$INSTANCE_NAME`_EZI2C_ACTIVE_ADDRESS1 == (activeAddress)) \
                {    \
                    `$INSTANCE_NAME`_offsetBuf1 = (`$EzI2cBitWidthReplacementString`) (locOffset); \
                }    \
                else \
                {    \
                    `$INSTANCE_NAME`_offsetBuf2 = (`$EzI2cBitWidthReplacementString`) (locOffset); \
                }    \
            }while(0)
#else
    #define `$INSTANCE_NAME`_EZI2C_UPDATE_LOC_STATUS(activeAddress, locStatus)  do{/* Empty*/;}while(0)

    #define `$INSTANCE_NAME`_EZI2C_GET_INDEX(activeAddress)  ((uint32) (`$INSTANCE_NAME`_indexBuf1))

    #define `$INSTANCE_NAME`_EZI2C_GET_OFFSET(activeAddress) ((uint32) (`$INSTANCE_NAME`_offsetBuf1))

    #define `$INSTANCE_NAME`_EZI2C_SET_INDEX(activeAddress, locIndex) \
            do{ \
                `$INSTANCE_NAME`_indexBuf1 = (`$EzI2cBitWidthReplacementString`) (locIndex); \
            }while(0)

    #define `$INSTANCE_NAME`_EZI2C_SET_OFFSET(activeAddress, locOffset) \
            do{ \
                `$INSTANCE_NAME`_offsetBuf1 = (`$EzI2cBitWidthReplacementString`) (locOffset); \
            }while(0)

#endif  /* (`$INSTANCE_NAME`_SUB_ADDRESS_SIZE16_CONST) */


/***************************************
*      Common Register Settings
***************************************/

#define `$INSTANCE_NAME`_CTRL_EZI2C     (`$INSTANCE_NAME`_CTRL_MODE_I2C)

#define `$INSTANCE_NAME`_EZI2C_CTRL     (`$INSTANCE_NAME`_I2C_CTRL_S_GENERAL_IGNORE | \
                                         `$INSTANCE_NAME`_I2C_CTRL_SLAVE_MODE)

#define `$INSTANCE_NAME`_EZI2C_CTRL_AUTO    (`$INSTANCE_NAME`_I2C_CTRL_S_READY_ADDR_ACK      | \
                                             `$INSTANCE_NAME`_I2C_CTRL_S_READY_DATA_ACK      | \
                                             `$INSTANCE_NAME`_I2C_CTRL_S_NOT_READY_ADDR_NACK | \
                                             `$INSTANCE_NAME`_I2C_CTRL_S_NOT_READY_DATA_NACK)

#define `$INSTANCE_NAME`_EZI2C_RX_CTRL  ((`$INSTANCE_NAME`_FIFO_SIZE - 1u)   | \
                                          `$INSTANCE_NAME`_RX_CTRL_MSB_FIRST | \
                                          `$INSTANCE_NAME`_RX_CTRL_ENABLED)

#define `$INSTANCE_NAME`_EZI2C_TX_CTRL  ((`$INSTANCE_NAME`_FIFO_SIZE - 1u)   | \
                                          `$INSTANCE_NAME`_TX_CTRL_MSB_FIRST | \
                                          `$INSTANCE_NAME`_TX_CTRL_ENABLED)

#define `$INSTANCE_NAME`_EZI2C_INTR_SLAVE_MASK  (`$INSTANCE_NAME`_INTR_SLAVE_I2C_BUS_ERROR | \
                                                 `$INSTANCE_NAME`_INTR_SLAVE_I2C_ARB_LOST  | \
                                                 `$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP)

#define `$INSTANCE_NAME`_INTR_SLAVE_COMPLETE    (`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH | \
                                                 `$INSTANCE_NAME`_INTR_SLAVE_I2C_STOP       | \
                                                 `$INSTANCE_NAME`_INTR_SLAVE_I2C_NACK       | \
                                                 `$INSTANCE_NAME`_INTR_SLAVE_I2C_WRITE_STOP)


/***************************************
*    Initialization Register Settings
***************************************/

#if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG)

    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_CTRL \
                                (`$INSTANCE_NAME`_GET_CTRL_ADDR_ACCEPT(`$INSTANCE_NAME`_EZI2C_NUMBER_OF_ADDRESSES) | \
                                 `$INSTANCE_NAME`_GET_CTRL_EC_AM_MODE (`$INSTANCE_NAME`_EZI2C_WAKE_ENABLE))

    #if(`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST)
        #define `$INSTANCE_NAME`_EZI2C_DEFAULT_I2C_CTRL (`$INSTANCE_NAME`_EZI2C_CTRL)
    #else
        #define `$INSTANCE_NAME`_EZI2C_DEFAULT_I2C_CTRL (`$INSTANCE_NAME`_EZI2C_CTRL_AUTO | `$INSTANCE_NAME`_EZI2C_CTRL)
    #endif /* (`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST) */


    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_RX_MATCH \
                                (`$INSTANCE_NAME`_GET_I2C_8BIT_ADDRESS(`$INSTANCE_NAME`_EZI2C_PRIMARY_SLAVE_ADDRESS) | \
                                 `$INSTANCE_NAME`_GET_RX_MATCH_MASK   (`$INSTANCE_NAME`_EZI2C_SLAVE_ADDRESS_MASK))

    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_RX_CTRL \
                                (`$INSTANCE_NAME`_GET_RX_CTRL_MEDIAN(`$INSTANCE_NAME`_EZI2C_MEDIAN_FILTER_ENABLE) | \
                                 `$INSTANCE_NAME`_EZI2C_RX_CTRL)

    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_TX_CTRL  (`$INSTANCE_NAME`_EZI2C_TX_CTRL)

    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_RX_FIFO_CTRL (0u)
    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_TX_FIFO_CTRL (0u)

    /* Interrupt sources */
    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_I2C_EC_MASK (`$INSTANCE_NAME`_NO_INTR_SOURCES)
    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_SPI_EC_MASK (`$INSTANCE_NAME`_NO_INTR_SOURCES)
    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_MASTER_MASK (`$INSTANCE_NAME`_NO_INTR_SOURCES)
    #define `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_TX_MASK     (`$INSTANCE_NAME`_NO_INTR_SOURCES)

    #if(`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST)
        #define `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_RX_MASK     (`$INSTANCE_NAME`_NO_INTR_SOURCES)
        #define `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_SLAVE_MASK  (`$INSTANCE_NAME`_INTR_SLAVE_I2C_ADDR_MATCH | \
                                                                 `$INSTANCE_NAME`_EZI2C_INTR_SLAVE_MASK)
    #else
        #define `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_RX_MASK     (`$INSTANCE_NAME`_INTR_RX_NOT_EMPTY)
        #define `$INSTANCE_NAME`_EZI2C_DEFAULT_INTR_SLAVE_MASK  (`$INSTANCE_NAME`_INTR_SLAVE_I2C_START | \
                                                                 `$INSTANCE_NAME`_EZI2C_INTR_SLAVE_MASK)
    #endif /* (`$INSTANCE_NAME`_EZI2C_SCL_STRETCH_ENABLE_CONST) */

#endif /* (`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* (CY_SCB_EZI2C_`$INSTANCE_NAME`_H) */


/* [] END OF FILE */
