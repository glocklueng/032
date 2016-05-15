/*******************************************************************************
* File Name: uWIFI_PINS.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_uWIFI_H)
#define CY_SCB_PINS_uWIFI_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define uWIFI_REMOVE_MOSI_SCL_RX_WAKE_PIN    (1u)
#define uWIFI_REMOVE_MOSI_SCL_RX_PIN         (1u)
#define uWIFI_REMOVE_MISO_SDA_TX_PIN         (1u)
#define uWIFI_REMOVE_SCLK_PIN                (1u)
#define uWIFI_REMOVE_SS0_PIN                 (1u)
#define uWIFI_REMOVE_SS1_PIN                 (1u)
#define uWIFI_REMOVE_SS2_PIN                 (1u)
#define uWIFI_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define uWIFI_REMOVE_I2C_PINS                (1u)
#define uWIFI_REMOVE_SPI_MASTER_PINS         (1u)
#define uWIFI_REMOVE_SPI_SLAVE_PINS          (1u)
#define uWIFI_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define uWIFI_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define uWIFI_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define uWIFI_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define uWIFI_REMOVE_UART_TX_PIN             (0u)
#define uWIFI_REMOVE_UART_RX_TX_PIN          (1u)
#define uWIFI_REMOVE_UART_RX_PIN             (0u)
#define uWIFI_REMOVE_UART_RX_WAKE_PIN        (1u)
#define uWIFI_REMOVE_UART_RTS_PIN            (1u)
#define uWIFI_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define uWIFI_MOSI_SCL_RX_WAKE_PIN   (0u == uWIFI_REMOVE_MOSI_SCL_RX_WAKE_PIN)
#define uWIFI_MOSI_SCL_RX_PIN        (0u == uWIFI_REMOVE_MOSI_SCL_RX_PIN)
#define uWIFI_MISO_SDA_TX_PIN        (0u == uWIFI_REMOVE_MISO_SDA_TX_PIN)
#define uWIFI_SCLK_PIN               (0u == uWIFI_REMOVE_SCLK_PIN)
#define uWIFI_SS0_PIN                (0u == uWIFI_REMOVE_SS0_PIN)
#define uWIFI_SS1_PIN                (0u == uWIFI_REMOVE_SS1_PIN)
#define uWIFI_SS2_PIN                (0u == uWIFI_REMOVE_SS2_PIN)
#define uWIFI_SS3_PIN                (0u == uWIFI_REMOVE_SS3_PIN)

/* Mode defined pins */
#define uWIFI_I2C_PINS               (0u == uWIFI_REMOVE_I2C_PINS)
#define uWIFI_SPI_MASTER_PINS        (0u == uWIFI_REMOVE_SPI_MASTER_PINS)
#define uWIFI_SPI_SLAVE_PINS         (0u == uWIFI_REMOVE_SPI_SLAVE_PINS)
#define uWIFI_SPI_MASTER_SS0_PIN     (0u == uWIFI_REMOVE_SPI_MASTER_SS0_PIN)
#define uWIFI_SPI_MASTER_SS1_PIN     (0u == uWIFI_REMOVE_SPI_MASTER_SS1_PIN)
#define uWIFI_SPI_MASTER_SS2_PIN     (0u == uWIFI_REMOVE_SPI_MASTER_SS2_PIN)
#define uWIFI_SPI_MASTER_SS3_PIN     (0u == uWIFI_REMOVE_SPI_MASTER_SS3_PIN)
#define uWIFI_UART_TX_PIN            (0u == uWIFI_REMOVE_UART_TX_PIN)
#define uWIFI_UART_RX_TX_PIN         (0u == uWIFI_REMOVE_UART_RX_TX_PIN)
#define uWIFI_UART_RX_PIN            (0u == uWIFI_REMOVE_UART_RX_PIN)
#define uWIFI_UART_RX_WAKE_PIN       (0u == uWIFI_REMOVE_UART_RX_WAKE_PIN)
#define uWIFI_UART_RTS_PIN           (0u == uWIFI_REMOVE_UART_RTS_PIN)
#define uWIFI_UART_CTS_PIN           (0u == uWIFI_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if(uWIFI_MOSI_SCL_RX_WAKE_PIN)
    #include "uWIFI_spi_mosi_i2c_scl_uart_rx_wake.h"
#endif /* (uWIFI_MOSI_SCL_RX_WAKE_PIN) */

#if(uWIFI_MOSI_SCL_RX_PIN)
    #include "uWIFI_spi_mosi_i2c_scl_uart_rx.h"
#endif /* (uWIFI_MOSI_SCL_RX_PIN) */

#if(uWIFI_MISO_SDA_TX_PIN)
    #include "uWIFI_spi_miso_i2c_sda_uart_tx.h"
#endif /* (uWIFI_MISO_SDA_TX_PIN_PIN) */

#if(uWIFI_SCLK_PIN)
    #include "uWIFI_spi_sclk.h"
#endif /* (uWIFI_SCLK_PIN) */

#if(uWIFI_SS0_PIN)
    #include "uWIFI_spi_ss0.h"
#endif /* (uWIFI_SS1_PIN) */

#if(uWIFI_SS1_PIN)
    #include "uWIFI_spi_ss1.h"
#endif /* (uWIFI_SS1_PIN) */

#if(uWIFI_SS2_PIN)
    #include "uWIFI_spi_ss2.h"
#endif /* (uWIFI_SS2_PIN) */

#if(uWIFI_SS3_PIN)
    #include "uWIFI_spi_ss3.h"
#endif /* (uWIFI_SS3_PIN) */

#if(uWIFI_I2C_PINS)
    #include "uWIFI_scl.h"
    #include "uWIFI_sda.h"
#endif /* (uWIFI_I2C_PINS) */

#if(uWIFI_SPI_MASTER_PINS)
    #include "uWIFI_sclk_m.h"
    #include "uWIFI_mosi_m.h"
    #include "uWIFI_miso_m.h"
#endif /* (uWIFI_SPI_MASTER_PINS) */

#if(uWIFI_SPI_SLAVE_PINS)
    #include "uWIFI_sclk_s.h"
    #include "uWIFI_mosi_s.h"
    #include "uWIFI_miso_s.h"
    #include "uWIFI_ss_s.h"
#endif /* (uWIFI_SPI_SLAVE_PINS) */

#if(uWIFI_SPI_MASTER_SS0_PIN)
    #include "uWIFI_ss0_m.h"
#endif /* (uWIFI_SPI_MASTER_SS0_PIN) */

#if(uWIFI_SPI_MASTER_SS1_PIN)
    #include "uWIFI_ss1_m.h"
#endif /* (uWIFI_SPI_MASTER_SS1_PIN) */

#if(uWIFI_SPI_MASTER_SS2_PIN)
    #include "uWIFI_ss2_m.h"
#endif /* (uWIFI_SPI_MASTER_SS2_PIN) */

#if(uWIFI_SPI_MASTER_SS3_PIN)
    #include "uWIFI_ss3_m.h"
#endif /* (uWIFI_SPI_MASTER_SS3_PIN) */

#if(uWIFI_UART_TX_PIN)
    #include "uWIFI_tx.h"
#endif /* (uWIFI_UART_TX_PIN) */

#if(uWIFI_UART_RX_TX_PIN)
    #include "uWIFI_rx_tx.h"
#endif /* (uWIFI_UART_RX_TX_PIN) */

#if(uWIFI_UART_RX_PIN)
    #include "uWIFI_rx.h"
#endif /* (uWIFI_UART_RX_PIN) */

#if(uWIFI_UART_RX_WAKE_PIN)
    #include "uWIFI_rx_wake.h"
#endif /* (uWIFI_UART_RX_WAKE_PIN) */

#if(uWIFI_UART_RTS_PIN)
    #include "uWIFI_rts.h"
#endif /* (uWIFI_UART_RTS_PIN) */

#if(uWIFI_UART_CTS_PIN)
    #include "uWIFI_cts.h"
#endif /* (uWIFI_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if(uWIFI_MOSI_SCL_RX_WAKE_PIN)
    #define uWIFI_MOSI_SCL_RX_WAKE_HSIOM_REG  \
                                                (*(reg32 *) uWIFI_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define uWIFI_MOSI_SCL_RX_WAKE_HSIOM_PTR  \
                                                ( (reg32 *) uWIFI_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define uWIFI_MOSI_SCL_RX_WAKE_HSIOM_MASK \
                                                (uWIFI_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_MASK)
    #define uWIFI_MOSI_SCL_RX_WAKE_HSIOM_POS  \
                                                (uWIFI_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_SHIFT)

    #define uWIFI_MOSI_SCL_RX_WAKE_INTCFG_REG    (*(reg32 *) \
                                                              uWIFI_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)
    #define uWIFI_MOSI_SCL_RX_WAKE_INTCFG_PTR    ( (reg32 *) \
                                                              uWIFI_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)

    #define uWIFI_INTCFG_TYPE_MASK                  (0x03u)
    #define uWIFI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS  (uWIFI_spi_mosi_i2c_scl_uart_rx_wake__SHIFT)
    #define uWIFI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK ((uint32)                                           \
                                                                    ((uint32) uWIFI_INTCFG_TYPE_MASK << \
                                                                    uWIFI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS))
#endif /* (uWIFI_MOSI_SCL_RX_WAKE_PIN) */

#if(uWIFI_MOSI_SCL_RX_PIN)
    #define uWIFI_MOSI_SCL_RX_HSIOM_REG      (*(reg32 *) uWIFI_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define uWIFI_MOSI_SCL_RX_HSIOM_PTR      ( (reg32 *) uWIFI_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define uWIFI_MOSI_SCL_RX_HSIOM_MASK     (uWIFI_spi_mosi_i2c_scl_uart_rx__0__HSIOM_MASK)
    #define uWIFI_MOSI_SCL_RX_HSIOM_POS      (uWIFI_spi_mosi_i2c_scl_uart_rx__0__HSIOM_SHIFT)
#endif /* (uWIFI_MOSI_SCL_RX_PIN) */

#if(uWIFI_MISO_SDA_TX_PIN)
    #define uWIFI_MISO_SDA_TX_HSIOM_REG      (*(reg32 *) uWIFI_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define uWIFI_MISO_SDA_TX_HSIOM_PTR      ( (reg32 *) uWIFI_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define uWIFI_MISO_SDA_TX_HSIOM_MASK     (uWIFI_spi_miso_i2c_sda_uart_tx__0__HSIOM_MASK)
    #define uWIFI_MISO_SDA_TX_HSIOM_POS      (uWIFI_spi_miso_i2c_sda_uart_tx__0__HSIOM_SHIFT)
#endif /* (uWIFI_MISO_SDA_TX_PIN_PIN) */

#if(uWIFI_SCLK_PIN)
    #define uWIFI_SCLK_HSIOM_REG     (*(reg32 *) uWIFI_spi_sclk__0__HSIOM)
    #define uWIFI_SCLK_HSIOM_PTR     ( (reg32 *) uWIFI_spi_sclk__0__HSIOM)
    #define uWIFI_SCLK_HSIOM_MASK    (uWIFI_spi_sclk__0__HSIOM_MASK)
    #define uWIFI_SCLK_HSIOM_POS     (uWIFI_spi_sclk__0__HSIOM_SHIFT)
#endif /* (uWIFI_SCLK_PIN) */

#if(uWIFI_SS0_PIN)
    #define uWIFI_SS0_HSIOM_REG      (*(reg32 *) uWIFI_spi_ss0__0__HSIOM)
    #define uWIFI_SS0_HSIOM_PTR      ( (reg32 *) uWIFI_spi_ss0__0__HSIOM)
    #define uWIFI_SS0_HSIOM_MASK     (uWIFI_spi_ss0__0__HSIOM_MASK)
    #define uWIFI_SS0_HSIOM_POS      (uWIFI_spi_ss0__0__HSIOM_SHIFT)
#endif /* (uWIFI_SS1_PIN) */

#if(uWIFI_SS1_PIN)
    #define uWIFI_SS1_HSIOM_REG      (*(reg32 *) uWIFI_spi_ss1__0__HSIOM)
    #define uWIFI_SS1_HSIOM_PTR      ( (reg32 *) uWIFI_spi_ss1__0__HSIOM)
    #define uWIFI_SS1_HSIOM_MASK     (uWIFI_spi_ss1__0__HSIOM_MASK)
    #define uWIFI_SS1_HSIOM_POS      (uWIFI_spi_ss1__0__HSIOM_SHIFT)
#endif /* (uWIFI_SS1_PIN) */

#if(uWIFI_SS2_PIN)
    #define uWIFI_SS2_HSIOM_REG     (*(reg32 *) uWIFI_spi_ss2__0__HSIOM)
    #define uWIFI_SS2_HSIOM_PTR     ( (reg32 *) uWIFI_spi_ss2__0__HSIOM)
    #define uWIFI_SS2_HSIOM_MASK    (uWIFI_spi_ss2__0__HSIOM_MASK)
    #define uWIFI_SS2_HSIOM_POS     (uWIFI_spi_ss2__0__HSIOM_SHIFT)
#endif /* (uWIFI_SS2_PIN) */

#if(uWIFI_SS3_PIN)
    #define uWIFI_SS3_HSIOM_REG     (*(reg32 *) uWIFI_spi_ss3__0__HSIOM)
    #define uWIFI_SS3_HSIOM_PTR     ( (reg32 *) uWIFI_spi_ss3__0__HSIOM)
    #define uWIFI_SS3_HSIOM_MASK    (uWIFI_spi_ss3__0__HSIOM_MASK)
    #define uWIFI_SS3_HSIOM_POS     (uWIFI_spi_ss3__0__HSIOM_SHIFT)
#endif /* (uWIFI_SS3_PIN) */

#if(uWIFI_I2C_PINS)
    #define uWIFI_SCL_HSIOM_REG     (*(reg32 *) uWIFI_scl__0__HSIOM)
    #define uWIFI_SCL_HSIOM_PTR     ( (reg32 *) uWIFI_scl__0__HSIOM)
    #define uWIFI_SCL_HSIOM_MASK    (uWIFI_scl__0__HSIOM_MASK)
    #define uWIFI_SCL_HSIOM_POS     (uWIFI_scl__0__HSIOM_SHIFT)

    #define uWIFI_SDA_HSIOM_REG     (*(reg32 *) uWIFI_sda__0__HSIOM)
    #define uWIFI_SDA_HSIOM_PTR     ( (reg32 *) uWIFI_sda__0__HSIOM)
    #define uWIFI_SDA_HSIOM_MASK    (uWIFI_sda__0__HSIOM_MASK)
    #define uWIFI_SDA_HSIOM_POS     (uWIFI_sda__0__HSIOM_SHIFT)
#endif /* (uWIFI_I2C_PINS) */


/***************************************
*        Registers Constants
***************************************/

/* Pins constants */
#define uWIFI_HSIOM_DEF_SEL      (0x00u)
#define uWIFI_HSIOM_GPIO_SEL     (0x00u)
#define uWIFI_HSIOM_UART_SEL     (0x09u)
#define uWIFI_HSIOM_I2C_SEL      (0x0Eu)
#define uWIFI_HSIOM_SPI_SEL      (0x0Fu)

#define uWIFI_MOSI_SCL_RX_PIN_INDEX      (0u) /* RX pins without interrupt */
#define uWIFI_MOSI_SCL_RX_WAKE_PIN_INDEX (0u) /* RX pin with interrupt     */
#define uWIFI_MISO_SDA_TX_PIN_INDEX      (1u)
#define uWIFI_SCLK_PIN_INDEX             (2u)
#define uWIFI_SS0_PIN_INDEX              (3u)
#define uWIFI_SS1_PIN_INDEX              (4u)
#define uWIFI_SS2_PIN_INDEX              (5u)
#define uWIFI_SS3_PIN_INDEX              (6u)

#define uWIFI_MOSI_SCL_RX_PIN_MASK      ((uint32) 0x01u << uWIFI_MOSI_SCL_RX_PIN_INDEX)
#define uWIFI_MOSI_SCL_RX_WAKE_PIN_MASK ((uint32) 0x01u << uWIFI_MOSI_SCL_RX_WAKE_PIN_INDEX)
#define uWIFI_MISO_SDA_TX_PIN_MASK      ((uint32) 0x01u << uWIFI_MISO_SDA_TX_PIN_INDEX)
#define uWIFI_SCLK_PIN_MASK             ((uint32) 0x01u << uWIFI_SCLK_PIN_INDEX)
#define uWIFI_SS0_PIN_MASK              ((uint32) 0x01u << uWIFI_SS0_PIN_INDEX)
#define uWIFI_SS1_PIN_MASK              ((uint32) 0x01u << uWIFI_SS1_PIN_INDEX)
#define uWIFI_SS2_PIN_MASK              ((uint32) 0x01u << uWIFI_SS2_PIN_INDEX)
#define uWIFI_SS3_PIN_MASK              ((uint32) 0x01u << uWIFI_SS3_PIN_INDEX)

#define uWIFI_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin DM defines */
#define uWIFI_PIN_DM_ALG_HIZ  (0u)
#define uWIFI_PIN_DM_DIG_HIZ  (1u)
#define uWIFI_PIN_DM_OD_LO    (4u)
#define uWIFI_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Set bits-mask in register */
#define uWIFI_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define uWIFI_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define uWIFI_SET_HSIOM_SEL(reg, mask, pos, sel) uWIFI_SET_REGISTER_BITS(reg, mask, pos, sel)
#define uWIFI_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        uWIFI_SET_REGISTER_BITS(reg, mask, pos, intType)
#define uWIFI_SET_INP_DIS(reg, mask, val) uWIFI_SET_REGISTER_BIT(reg, mask, val)

/* uWIFI_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  uWIFI_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if(uWIFI_I2C_PINS)
    #define uWIFI_SET_I2C_SCL_DR(val) uWIFI_scl_Write(val)

    #define uWIFI_SET_I2C_SCL_HSIOM_SEL(sel) \
                          uWIFI_SET_HSIOM_SEL(uWIFI_SCL_HSIOM_REG,  \
                                                         uWIFI_SCL_HSIOM_MASK, \
                                                         uWIFI_SCL_HSIOM_POS,  \
                                                         (sel))
    #define uWIFI_WAIT_SCL_SET_HIGH  (0u == uWIFI_scl_Read())

/* Unconfigured SCB: scl signal */
#elif(uWIFI_MOSI_SCL_RX_WAKE_PIN)
    #define uWIFI_SET_I2C_SCL_DR(val) \
                            uWIFI_spi_mosi_i2c_scl_uart_rx_wake_Write(val)

    #define uWIFI_SET_I2C_SCL_HSIOM_SEL(sel) \
                    uWIFI_SET_HSIOM_SEL(uWIFI_MOSI_SCL_RX_WAKE_HSIOM_REG,  \
                                                   uWIFI_MOSI_SCL_RX_WAKE_HSIOM_MASK, \
                                                   uWIFI_MOSI_SCL_RX_WAKE_HSIOM_POS,  \
                                                   (sel))

    #define uWIFI_WAIT_SCL_SET_HIGH  (0u == uWIFI_spi_mosi_i2c_scl_uart_rx_wake_Read())

#elif(uWIFI_MOSI_SCL_RX_PIN)
    #define uWIFI_SET_I2C_SCL_DR(val) \
                            uWIFI_spi_mosi_i2c_scl_uart_rx_Write(val)


    #define uWIFI_SET_I2C_SCL_HSIOM_SEL(sel) \
                            uWIFI_SET_HSIOM_SEL(uWIFI_MOSI_SCL_RX_HSIOM_REG,  \
                                                           uWIFI_MOSI_SCL_RX_HSIOM_MASK, \
                                                           uWIFI_MOSI_SCL_RX_HSIOM_POS,  \
                                                           (sel))

    #define uWIFI_WAIT_SCL_SET_HIGH  (0u == uWIFI_spi_mosi_i2c_scl_uart_rx_Read())

#else
    #define uWIFI_SET_I2C_SCL_DR(val) \
                                                    do{ /* Does nothing */ }while(0)
    #define uWIFI_SET_I2C_SCL_HSIOM_SEL(sel) \
                                                    do{ /* Does nothing */ }while(0)

    #define uWIFI_WAIT_SCL_SET_HIGH  (0u)
#endif /* (uWIFI_MOSI_SCL_RX_PIN) */

/* SCB I2C: sda signal */
#if(uWIFI_I2C_PINS)
    #define uWIFI_WAIT_SDA_SET_HIGH  (0u == uWIFI_sda_Read())

/* Unconfigured SCB: sda signal */
#elif(uWIFI_MISO_SDA_TX_PIN)
    #define uWIFI_WAIT_SDA_SET_HIGH  (0u == uWIFI_spi_miso_i2c_sda_uart_tx_Read())

#else
    #define uWIFI_WAIT_SDA_SET_HIGH  (0u)
#endif /* (uWIFI_MOSI_SCL_RX_PIN) */

#endif /* (CY_SCB_PINS_uWIFI_H) */


/* [] END OF FILE */
