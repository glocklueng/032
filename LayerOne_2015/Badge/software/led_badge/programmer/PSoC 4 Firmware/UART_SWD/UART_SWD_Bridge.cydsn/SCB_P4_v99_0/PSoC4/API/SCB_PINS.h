/*******************************************************************************
* File Name: `$INSTANCE_NAME`_PINS.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_`$INSTANCE_NAME`_H)
#define CY_SCB_PINS_`$INSTANCE_NAME`_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define `$INSTANCE_NAME`_REMOVE_MOSI_SCL_RX_WAKE_PIN    (`$RemoveMosiSclRxWake`u)
#define `$INSTANCE_NAME`_REMOVE_MOSI_SCL_RX_PIN         (`$RemoveMosiSclRx`u)
#define `$INSTANCE_NAME`_REMOVE_MISO_SDA_TX_PIN         (`$RemoveMisoSdaTx`u)
#define `$INSTANCE_NAME`_REMOVE_SCLK_PIN                (`$RemoveSpiSclk`u)
#define `$INSTANCE_NAME`_REMOVE_SS0_PIN                 (`$RemoveSpiSs0`u)
#define `$INSTANCE_NAME`_REMOVE_SS1_PIN                 (`$RemoveSpiSs1`u)
#define `$INSTANCE_NAME`_REMOVE_SS2_PIN                 (`$RemoveSpiSs2`u)
#define `$INSTANCE_NAME`_REMOVE_SS3_PIN                 (`$RemoveSpiSs3`u)

/* Mode defined pins */
#define `$INSTANCE_NAME`_REMOVE_I2C_PINS                (`$RemoveI2cPins`u)
#define `$INSTANCE_NAME`_REMOVE_SPI_MASTER_PINS         (`$RemoveSpiMasterPins`u)
#define `$INSTANCE_NAME`_REMOVE_SPI_SLAVE_PINS          (`$RemoveSpiSlavePins`u)
#define `$INSTANCE_NAME`_REMOVE_SPI_MASTER_SS0_PIN      (`$RemoveSpiMasterSs0Pin`u)
#define `$INSTANCE_NAME`_REMOVE_SPI_MASTER_SS1_PIN      (`$RemoveSpiMasterSs1Pin`u)
#define `$INSTANCE_NAME`_REMOVE_SPI_MASTER_SS2_PIN      (`$RemoveSpiMasterSs2Pin`u)
#define `$INSTANCE_NAME`_REMOVE_SPI_MASTER_SS3_PIN      (`$RemoveSpiMasterSs3Pin`u)
#define `$INSTANCE_NAME`_REMOVE_UART_TX_PIN             (`$RemoveUartTxPin`u)
#define `$INSTANCE_NAME`_REMOVE_UART_RX_TX_PIN          (`$RemoveUartRxTxPin`u)
#define `$INSTANCE_NAME`_REMOVE_UART_RX_PIN             (`$RemoveUartRxPin`u)
#define `$INSTANCE_NAME`_REMOVE_UART_RX_WAKE_PIN        (`$RemoveUartRxWake`u)

/* Unconfigured pins */
#define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN   (0u == `$INSTANCE_NAME`_REMOVE_MOSI_SCL_RX_WAKE_PIN)
#define `$INSTANCE_NAME`_MOSI_SCL_RX_PIN        (0u == `$INSTANCE_NAME`_REMOVE_MOSI_SCL_RX_PIN)
#define `$INSTANCE_NAME`_MISO_SDA_TX_PIN        (0u == `$INSTANCE_NAME`_REMOVE_MISO_SDA_TX_PIN)
#define `$INSTANCE_NAME`_SCLK_PIN               (0u == `$INSTANCE_NAME`_REMOVE_SCLK_PIN)
#define `$INSTANCE_NAME`_SS0_PIN                (0u == `$INSTANCE_NAME`_REMOVE_SS0_PIN)
#define `$INSTANCE_NAME`_SS1_PIN                (0u == `$INSTANCE_NAME`_REMOVE_SS1_PIN)
#define `$INSTANCE_NAME`_SS2_PIN                (0u == `$INSTANCE_NAME`_REMOVE_SS2_PIN)
#define `$INSTANCE_NAME`_SS3_PIN                (0u == `$INSTANCE_NAME`_REMOVE_SS3_PIN)

/* Mode defined pins */
#define `$INSTANCE_NAME`_I2C_PINS               (0u == `$INSTANCE_NAME`_REMOVE_I2C_PINS)
#define `$INSTANCE_NAME`_SPI_MASTER_PINS        (0u == `$INSTANCE_NAME`_REMOVE_SPI_MASTER_PINS) 
#define `$INSTANCE_NAME`_SPI_SLAVE_PINS         (0u == `$INSTANCE_NAME`_REMOVE_SPI_SLAVE_PINS)
#define `$INSTANCE_NAME`_SPI_MASTER_SS0_PIN     (0u == `$INSTANCE_NAME`_REMOVE_SPI_MASTER_SS0_PIN)
#define `$INSTANCE_NAME`_SPI_MASTER_SS1_PIN     (0u == `$INSTANCE_NAME`_REMOVE_SPI_MASTER_SS1_PIN)
#define `$INSTANCE_NAME`_SPI_MASTER_SS2_PIN     (0u == `$INSTANCE_NAME`_REMOVE_SPI_MASTER_SS2_PIN)
#define `$INSTANCE_NAME`_SPI_MASTER_SS3_PIN     (0u == `$INSTANCE_NAME`_REMOVE_SPI_MASTER_SS3_PIN)
#define `$INSTANCE_NAME`_UART_TX_PIN            (0u == `$INSTANCE_NAME`_REMOVE_UART_TX_PIN)
#define `$INSTANCE_NAME`_UART_RX_TX_PIN         (0u == `$INSTANCE_NAME`_REMOVE_UART_RX_TX_PIN)
#define `$INSTANCE_NAME`_UART_RX_PIN            (0u == `$INSTANCE_NAME`_REMOVE_UART_RX_PIN)
#define `$INSTANCE_NAME`_UART_RX_WAKE_PIN       (0u == `$INSTANCE_NAME`_REMOVE_UART_RX_WAKE_PIN)


/***************************************
*             Includes
****************************************/

#if(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN)
    #include "`$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake.h"
#endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN) */

#if(`$INSTANCE_NAME`_MOSI_SCL_RX_PIN)
    #include "`$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx.h"
#endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_PIN) */

#if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
    #include "`$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx.h"
#endif /* (`$INSTANCE_NAME`_MISO_SDA_TX_PIN_PIN) */

#if(`$INSTANCE_NAME`_SCLK_PIN)
    #include "`$INSTANCE_NAME`_spi_sclk.h"
#endif /* (`$INSTANCE_NAME`_SCLK_PIN) */

#if(`$INSTANCE_NAME`_SS0_PIN)
    #include "`$INSTANCE_NAME`_spi_ss0.h"
#endif /* (`$INSTANCE_NAME`_SS1_PIN) */

#if(`$INSTANCE_NAME`_SS1_PIN)
    #include "`$INSTANCE_NAME`_spi_ss1.h"
#endif /* (`$INSTANCE_NAME`_SS1_PIN) */

#if(`$INSTANCE_NAME`_SS2_PIN)
    #include "`$INSTANCE_NAME`_spi_ss2.h"
#endif /* (`$INSTANCE_NAME`_SS2_PIN) */

#if(`$INSTANCE_NAME`_SS3_PIN)
    #include "`$INSTANCE_NAME`_spi_ss3.h"
#endif /* (`$INSTANCE_NAME`_SS3_PIN) */

#if(`$INSTANCE_NAME`_I2C_PINS)
    #include "`$INSTANCE_NAME`_scl.h"
    #include "`$INSTANCE_NAME`_sda.h"
#endif /* (`$INSTANCE_NAME`_I2C_PINS) */
    
#if(`$INSTANCE_NAME`_SPI_MASTER_PINS)
    #include "`$INSTANCE_NAME`_sclk_m.h"
    #include "`$INSTANCE_NAME`_mosi_m.h"
    #include "`$INSTANCE_NAME`_miso_m.h"
#endif /* (`$INSTANCE_NAME`_SPI_MASTER_PINS) */

#if(`$INSTANCE_NAME`_SPI_SLAVE_PINS)
    #include "`$INSTANCE_NAME`_sclk_s.h"
    #include "`$INSTANCE_NAME`_mosi_s.h"
    #include "`$INSTANCE_NAME`_miso_s.h"
    #include "`$INSTANCE_NAME`_ss_s.h"
#endif /* (`$INSTANCE_NAME`_SPI_SLAVE_PINS) */

#if(`$INSTANCE_NAME`_SPI_MASTER_SS0_PIN)
    #include "`$INSTANCE_NAME`_ss0_m.h"
#endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS0_PIN) */

#if(`$INSTANCE_NAME`_SPI_MASTER_SS1_PIN)
    #include "`$INSTANCE_NAME`_ss1_m.h"
#endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS1_PIN) */

#if(`$INSTANCE_NAME`_SPI_MASTER_SS2_PIN)
    #include "`$INSTANCE_NAME`_ss2_m.h"
#endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS2_PIN) */

#if(`$INSTANCE_NAME`_SPI_MASTER_SS3_PIN)
    #include "`$INSTANCE_NAME`_ss3_m.h"
#endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS3_PIN) */

#if(`$INSTANCE_NAME`_UART_TX_PIN)
    #include "`$INSTANCE_NAME`_tx.h"
#endif /* (`$INSTANCE_NAME`_UART_TX_PIN) */

#if(`$INSTANCE_NAME`_UART_RX_TX_PIN)
    #include "`$INSTANCE_NAME`_rx_tx.h"
#endif /* (`$INSTANCE_NAME`_UART_RX_TX_PIN) */

#if(`$INSTANCE_NAME`_UART_RX_PIN)
    #include "`$INSTANCE_NAME`_rx.h"
#endif /* (`$INSTANCE_NAME`_UART_RX_PIN) */

#if(`$INSTANCE_NAME`_UART_RX_WAKE_PIN)
    #include "`$INSTANCE_NAME`_rx_wake.h"
#endif /* (`$INSTANCE_NAME`_UART_RX_WAKE_PIN) */


/***************************************
*              Registers
***************************************/

#if(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_REG  \
                                                (*(reg32 *) `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_PTR  \
                                                ( (reg32 *) `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_MASK \
                                                (`$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_POS  \
                                                (`$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_SHIFT)

    #define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_INTCFG_REG    (*(reg32 *) \
                                                              `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_INTCFG_PTR    ( (reg32 *) \
                                                              `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)

    #define `$INSTANCE_NAME`_INTCFG_TYPE_MASK                  (0x03u)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS  (`$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake__SHIFT)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK ((uint32)                                           \
                                                                    ((uint32) `$INSTANCE_NAME`_INTCFG_TYPE_MASK << \
                                                                    `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS))
#endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN) */

#if(`$INSTANCE_NAME`_MOSI_SCL_RX_PIN)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_REG      (*(reg32 *) `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_PTR      ( (reg32 *) `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_MASK     (`$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_POS      (`$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx__0__HSIOM_SHIFT)
#endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_PIN) */

#if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
    #define `$INSTANCE_NAME`_MISO_SDA_TX_HSIOM_REG      (*(reg32 *) `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define `$INSTANCE_NAME`_MISO_SDA_TX_HSIOM_PTR      ( (reg32 *) `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define `$INSTANCE_NAME`_MISO_SDA_TX_HSIOM_MASK     (`$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_MISO_SDA_TX_HSIOM_POS      (`$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx__0__HSIOM_SHIFT)
#endif /* (`$INSTANCE_NAME`_MISO_SDA_TX_PIN_PIN) */

#if(`$INSTANCE_NAME`_SCLK_PIN)
    #define `$INSTANCE_NAME`_SCLK_HSIOM_REG     (*(reg32 *) `$INSTANCE_NAME`_spi_sclk__0__HSIOM)
    #define `$INSTANCE_NAME`_SCLK_HSIOM_PTR     ( (reg32 *) `$INSTANCE_NAME`_spi_sclk__0__HSIOM)
    #define `$INSTANCE_NAME`_SCLK_HSIOM_MASK    (`$INSTANCE_NAME`_spi_sclk__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_SCLK_HSIOM_POS     (`$INSTANCE_NAME`_spi_sclk__0__HSIOM_SHIFT)
#endif /* (`$INSTANCE_NAME`_SCLK_PIN) */

#if(`$INSTANCE_NAME`_SS0_PIN)
    #define `$INSTANCE_NAME`_SS0_HSIOM_REG      (*(reg32 *) `$INSTANCE_NAME`_spi_ss0__0__HSIOM)
    #define `$INSTANCE_NAME`_SS0_HSIOM_PTR      ( (reg32 *) `$INSTANCE_NAME`_spi_ss0__0__HSIOM)
    #define `$INSTANCE_NAME`_SS0_HSIOM_MASK     (`$INSTANCE_NAME`_spi_ss0__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_SS0_HSIOM_POS      (`$INSTANCE_NAME`_spi_ss0__0__HSIOM_SHIFT)
#endif /* (`$INSTANCE_NAME`_SS1_PIN) */

#if(`$INSTANCE_NAME`_SS1_PIN)
    #define `$INSTANCE_NAME`_SS1_HSIOM_REG      (*(reg32 *) `$INSTANCE_NAME`_spi_ss1__0__HSIOM)
    #define `$INSTANCE_NAME`_SS1_HSIOM_PTR      ( (reg32 *) `$INSTANCE_NAME`_spi_ss1__0__HSIOM)
    #define `$INSTANCE_NAME`_SS1_HSIOM_MASK     (`$INSTANCE_NAME`_spi_ss1__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_SS1_HSIOM_POS      (`$INSTANCE_NAME`_spi_ss1__0__HSIOM_SHIFT)
#endif /* (`$INSTANCE_NAME`_SS1_PIN) */

#if(`$INSTANCE_NAME`_SS2_PIN)
    #define `$INSTANCE_NAME`_SS2_HSIOM_REG     (*(reg32 *) `$INSTANCE_NAME`_spi_ss2__0__HSIOM)
    #define `$INSTANCE_NAME`_SS2_HSIOM_PTR     ( (reg32 *) `$INSTANCE_NAME`_spi_ss2__0__HSIOM)
    #define `$INSTANCE_NAME`_SS2_HSIOM_MASK    (`$INSTANCE_NAME`_spi_ss2__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_SS2_HSIOM_POS     (`$INSTANCE_NAME`_spi_ss2__0__HSIOM_SHIFT)
#endif /* (`$INSTANCE_NAME`_SS2_PIN) */

#if(`$INSTANCE_NAME`_SS3_PIN)
    #define `$INSTANCE_NAME`_SS3_HSIOM_REG     (*(reg32 *) `$INSTANCE_NAME`_spi_ss3__0__HSIOM)
    #define `$INSTANCE_NAME`_SS3_HSIOM_PTR     ( (reg32 *) `$INSTANCE_NAME`_spi_ss3__0__HSIOM)
    #define `$INSTANCE_NAME`_SS3_HSIOM_MASK    (`$INSTANCE_NAME`_spi_ss3__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_SS3_HSIOM_POS     (`$INSTANCE_NAME`_spi_ss3__0__HSIOM_SHIFT)
#endif /* (`$INSTANCE_NAME`_SS3_PIN) */

#if(`$INSTANCE_NAME`_I2C_PINS)
    #define `$INSTANCE_NAME`_SCL_HSIOM_REG     (*(reg32 *) `$INSTANCE_NAME`_scl__0__HSIOM)
    #define `$INSTANCE_NAME`_SCL_HSIOM_PTR     ( (reg32 *) `$INSTANCE_NAME`_scl__0__HSIOM)
    #define `$INSTANCE_NAME`_SCL_HSIOM_MASK    (`$INSTANCE_NAME`_scl__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_SCL_HSIOM_POS     (`$INSTANCE_NAME`_scl__0__HSIOM_SHIFT)

    #define `$INSTANCE_NAME`_SDA_HSIOM_REG     (*(reg32 *) `$INSTANCE_NAME`_sda__0__HSIOM)
    #define `$INSTANCE_NAME`_SDA_HSIOM_PTR     ( (reg32 *) `$INSTANCE_NAME`_sda__0__HSIOM)
    #define `$INSTANCE_NAME`_SDA_HSIOM_MASK    (`$INSTANCE_NAME`_sda__0__HSIOM_MASK)
    #define `$INSTANCE_NAME`_SDA_HSIOM_POS     (`$INSTANCE_NAME`_sda__0__HSIOM_SHIFT)
#endif /* (`$INSTANCE_NAME`_I2C_PINS) */


/***************************************
*        Registers Constants
***************************************/

/* Pins constanst */
#define `$INSTANCE_NAME`_HSIOM_DEF_SEL      (0x00u)
#define `$INSTANCE_NAME`_HSIOM_GPIO_SEL     (0x00u)
#define `$INSTANCE_NAME`_HSIOM_UART_SEL     (0x09u)
#define `$INSTANCE_NAME`_HSIOM_I2C_SEL      (0x0Eu)
#define `$INSTANCE_NAME`_HSIOM_SPI_SEL      (0x0Fu)

#define `$INSTANCE_NAME`_SCB_PINS_NUMBER            (7u)
#define `$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX      (0u) /* RX pins without interrupt */
#define `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN_INDEX (0u) /* RX pin with interrupt     */
#define `$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX      (1u)
#define `$INSTANCE_NAME`_SCLK_PIN_INDEX             (2u)
#define `$INSTANCE_NAME`_SS0_PIN_INDEX              (3u)
#define `$INSTANCE_NAME`_SS1_PIN_INDEX              (4u)
#define `$INSTANCE_NAME`_SS2_PIN_INDEX              (5u)
#define `$INSTANCE_NAME`_SS3_PIN_INDEX              (6u)

#define `$INSTANCE_NAME`_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin DM defines */
#define `$INSTANCE_NAME`_PIN_DM_ALG_HIZ  (0u)
#define `$INSTANCE_NAME`_PIN_DM_DIG_HIZ  (1u)
#define `$INSTANCE_NAME`_PIN_DM_OD_LO    (4u)
#define `$INSTANCE_NAME`_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

#define `$INSTANCE_NAME`_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        /* Sets new bits-mask */                 \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

#define `$INSTANCE_NAME`_SET_HSIOM_SEL(reg, mask, pos, sel) `$INSTANCE_NAME`_SET_REGISTER_BITS(reg, mask, pos, sel)
#define `$INSTANCE_NAME`_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        `$INSTANCE_NAME`_SET_REGISTER_BITS(reg, mask, pos, intType)


/* `$INSTANCE_NAME`_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  `$INSTANCE_NAME`_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* Unconfigured SCB: scl singnal */
#if(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN)
    #define `$INSTANCE_NAME`_SET_I2C_SCL_DR(val) \
                            `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake_Write(val)

    #define `$INSTANCE_NAME`_SET_I2C_SCL_HSIOM_SEL(sel) \
                    `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_REG,  \
                                                   `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_MASK, \
                                                   `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_POS,  \
                                                   (sel))

#elif(`$INSTANCE_NAME`_MOSI_SCL_RX_PIN)
    #define `$INSTANCE_NAME`_SET_I2C_SCL_DR(val) \
                            `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_Write(val)


    #define `$INSTANCE_NAME`_SET_I2C_SCL_HSIOM_SEL(sel) \
                            `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_REG,  \
                                                           `$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_MASK, \
                                                           `$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_POS,  \
                                                           (sel))
#else
    #if(!`$INSTANCE_NAME`_I2C_PINS)
        #define `$INSTANCE_NAME`_SET_I2C_SCL_DR(val) \
                                                 do{ ; }while(0)
        #define `$INSTANCE_NAME`_SET_I2C_SCL_HSIOM_SEL(sel) \
                                                        do{ ; }while(0)

    #endif /* (!`$INSTANCE_NAME`_I2C_PINS) */
#endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_PIN) */

/* SCB I2C: scl singal */
#if(`$INSTANCE_NAME`_I2C_PINS)
    #define `$INSTANCE_NAME`_SET_I2C_SCL_DR(val) `$INSTANCE_NAME`_scl_Write(val)

    #define `$INSTANCE_NAME`_SET_I2C_SCL_HSIOM_SEL(sel) \
                          `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_SCL_HSIOM_REG,  \
                                                         `$INSTANCE_NAME`_SCL_HSIOM_MASK, \
                                                         `$INSTANCE_NAME`_SCL_HSIOM_POS,  \
                                                         (sel))

#endif /* (`$INSTANCE_NAME`_I2C_PINS) */


#endif /* (CY_SCB_PINS_`$INSTANCE_NAME`_H) */


/* [] END OF FILE */
