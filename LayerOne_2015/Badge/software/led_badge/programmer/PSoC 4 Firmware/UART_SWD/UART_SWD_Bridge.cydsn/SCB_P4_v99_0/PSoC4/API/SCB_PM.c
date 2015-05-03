/*******************************************************************************
* File Name: `$INSTANCE_NAME`_PM.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`.h"
#include "`$INSTANCE_NAME`_PVT.h"

#if(`$INSTANCE_NAME`_SCB_MODE_I2C_INC)
    #include "`$INSTANCE_NAME`_I2C_PVT.h"
#endif /* (`$INSTANCE_NAME`_SCB_MODE_I2C_INC) */

#if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_INC)
    #include "`$INSTANCE_NAME`_EZI2C_PVT.h"
#endif /* (`$INSTANCE_NAME`_SCB_MODE_EZI2C_INC) */

#if(`$INSTANCE_NAME`_SCB_MODE_SPI_INC || `$INSTANCE_NAME`_SCB_MODE_UART_INC)
    #include "`$INSTANCE_NAME`_SPI_UART_PVT.h"
#endif /* (`$INSTANCE_NAME`_SCB_MODE_SPI_INC || `$INSTANCE_NAME`_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

`$INSTANCE_NAME`_BACKUP_STRUCT `$INSTANCE_NAME`_backup =
{
    0u, /* enableState */
};


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Sleep
********************************************************************************
*
* Summary:
*  Calls SaveConfig function fucntion for selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_Sleep(void)
{
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)

    if(0u != `$INSTANCE_NAME`_scbEnableWake)
    {
        if(`$INSTANCE_NAME`_SCB_MODE_I2C_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_I2CSaveConfig();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_SPI_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_SpiSaveConfig();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_UART_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_UartSaveConfig();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_EzI2CSaveConfig();
        }
        else
        {
            /* Unknown mode: do nothing */
        }
    }
    else
    {
        `$INSTANCE_NAME`_backup.enableState = (uint8) `$INSTANCE_NAME`_GET_CTRL_ENABLED;
        
        if(0u != `$INSTANCE_NAME`_backup.enableState)
        {
            `$INSTANCE_NAME`_Stop();
        }
    }
    
    `$INSTANCE_NAME`_DisableTxPinsInputBuffer();
    
#else
    
    #if defined (`$INSTANCE_NAME`_I2C_WAKE_ENABLE_CONST) && (`$INSTANCE_NAME`_I2C_WAKE_ENABLE_CONST)
        `$INSTANCE_NAME`_I2CSaveConfig();
        
    #elif defined (`$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST) && (`$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST)
        `$INSTANCE_NAME`_SpiSaveConfig();
        
    #elif defined (`$INSTANCE_NAME`_UART_WAKE_ENABLE_CONST) && (`$INSTANCE_NAME`_UART_WAKE_ENABLE_CONST)
        `$INSTANCE_NAME`_UartSaveConfig();
        
    #elif defined (`$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST) && (`$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST)
        `$INSTANCE_NAME`_EzI2CSaveConfig();
    
    #else
        
        `$INSTANCE_NAME`_backup.enableState = (uint8) `$INSTANCE_NAME`_GET_CTRL_ENABLED;
        
        /* Check enable state */
        if(0u != `$INSTANCE_NAME`_backup.enableState)
        {
            `$INSTANCE_NAME`_Stop();
        }
        
    #endif /* defined (`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG) && (`$INSTANCE_NAME`_I2C_WAKE_ENABLE_CONST) */
    
    `$INSTANCE_NAME`_DisableTxPinsInputBuffer();
    
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Wakeup
********************************************************************************
*
* Summary:
*  Calls RestoreConfig function fucntion for selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_Wakeup(void)
{
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)

    `$INSTANCE_NAME`_EnableTxPinsInputBuffer();
        
    if(0u != `$INSTANCE_NAME`_scbEnableWake)
    {
        if(`$INSTANCE_NAME`_SCB_MODE_I2C_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_I2CRestoreConfig();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_SPI_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_SpiRestoreConfig();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_UART_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_UartRestoreConfig();
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            `$INSTANCE_NAME`_EzI2CRestoreConfig();
        }
        else
        {
            /* Unknown mode: do nothing */
        }
    }
    else
    {    
        /* Restore enable state */
        if(0u != `$INSTANCE_NAME`_backup.enableState)
        {
            `$INSTANCE_NAME`_Enable();
        }
    }

#else
    
    `$INSTANCE_NAME`_EnableTxPinsInputBuffer();
        
    #if defined (`$INSTANCE_NAME`_I2C_WAKE_ENABLE_CONST) && (`$INSTANCE_NAME`_I2C_WAKE_ENABLE_CONST)
        `$INSTANCE_NAME`_I2CRestoreConfig();
        
    #elif defined (`$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST) && (`$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST)
        `$INSTANCE_NAME`_SpiRestoreConfig();
        
    #elif defined (`$INSTANCE_NAME`_UART_WAKE_ENABLE_CONST) && (`$INSTANCE_NAME`_UART_WAKE_ENABLE_CONST)
        `$INSTANCE_NAME`_UartRestoreConfig();
        
    #elif defined (`$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST) && (`$INSTANCE_NAME`_EZI2C_WAKE_ENABLE_CONST)
        `$INSTANCE_NAME`_EzI2CRestoreConfig();
    
    #else
        /* Check enable state */
        if(0u != `$INSTANCE_NAME`_backup.enableState)
        {
            `$INSTANCE_NAME`_Enable();
        }
        
    #endif /* (`$INSTANCE_NAME`_I2C_WAKE_ENABLE_CONST) */

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DisableTxPinsInputBuffer
********************************************************************************
*
* Summary:
*  Disables input buffers for TX pins. This action removes leakage current while
*  low power mode (Cypress ID 149635).
*   SCB mode is I2C and EZI2C: bus is pulled-up. Leave pins as it is.
*   SCB mode SPI:
*     Slave  - disable input buffer for MISO pin.
*     Master - disable input buffer for all pins.
*   SCB mode SmartCard: 
*     Standard and IrDA - disable input buffer for TX pin.
*     SmartCard - RX_TX pin is pulled-up. Leave pin as it is.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_DisableTxPinsInputBuffer(void)
{
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    
    /* SCB mode is I2C and EZI2C: bus is pulled-up. Does nothing */
       
    if(`$INSTANCE_NAME`_SCB_MODE_SPI_RUNTM_CFG)
    {
        if(0u != (`$INSTANCE_NAME`_SPI_CTRL_REG & `$INSTANCE_NAME`_SPI_CTRL_MASTER))
        /* SPI Master */
        {
        #if(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN)
            `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS |= \
                                                                `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake_MASK;
        #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN) */

        #if(`$INSTANCE_NAME`_MOSI_SCL_RX_PIN)
            `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_INP_DIS |= `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_MASK;
        #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_PIN) */

        #if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
            `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_INP_DIS |= `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_MASK;
        #endif /* (`$INSTANCE_NAME`_MISO_SDA_TX_PIN_PIN) */

        #if(`$INSTANCE_NAME`_SCLK_PIN)
            `$INSTANCE_NAME`_spi_sclk_INP_DIS |= `$INSTANCE_NAME`_spi_sclk_MASK;
        #endif /* (`$INSTANCE_NAME`_SCLK_PIN) */

        #if(`$INSTANCE_NAME`_SS0_PIN)
            `$INSTANCE_NAME`_spi_ss0_INP_DIS |= `$INSTANCE_NAME`_spi_ss0_MASK;
        #endif /* (`$INSTANCE_NAME`_SS1_PIN) */

        #if(`$INSTANCE_NAME`_SS1_PIN)
            `$INSTANCE_NAME`_spi_ss1_INP_DIS |= `$INSTANCE_NAME`_spi_ss1_MASK;
        #endif /* (`$INSTANCE_NAME`_SS1_PIN) */

        #if(`$INSTANCE_NAME`_SS2_PIN)
            `$INSTANCE_NAME`_spi_ss2_INP_DIS |= `$INSTANCE_NAME`_spi_ss2_MASK;
        #endif /* (`$INSTANCE_NAME`_SS2_PIN) */

        #if(`$INSTANCE_NAME`_SS3_PIN)
            `$INSTANCE_NAME`_spi_ss3_INP_DIS |= `$INSTANCE_NAME`_spi_ss3_MASK;
        #endif /* (`$INSTANCE_NAME`_SS3_PIN) */
        }
        else
        /* SPI Slave */
        {
        #if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
            `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_INP_DIS |= `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_MASK;
        #endif /* (`$INSTANCE_NAME`_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else if (`$INSTANCE_NAME`_SCB_MODE_UART_RUNTM_CFG)
    {
        if(`$INSTANCE_NAME`_UART_CTRL_MODE_UART_SMARTCARD != 
            (`$INSTANCE_NAME`_UART_CTRL_REG & `$INSTANCE_NAME`_UART_CTRL_MODE_MASK))
        /* UART Standard or IrDA */
        {
        #if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
            `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_INP_DIS |= `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_MASK;
        #endif /* (`$INSTANCE_NAME`_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else
    {
        /* Does nothing */
    }
    
#else
    
    /* SCB mode is I2C and EZI2C: bus is pulled-up. Does nothing */
        
    /* SCB mode is SPI Master */
    #if(`$INSTANCE_NAME`_SPI_MASTER_PINS)
        `$INSTANCE_NAME`_sclk_m_INP_DIS |= `$INSTANCE_NAME`_sclk_m_MASK;
        `$INSTANCE_NAME`_mosi_m_INP_DIS |= `$INSTANCE_NAME`_mosi_m_MASK;
        `$INSTANCE_NAME`_miso_m_INP_DIS |= `$INSTANCE_NAME`_miso_m_MASK;
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_PINS) */

    #if(`$INSTANCE_NAME`_SPI_MASTER_SS0_PIN)
        `$INSTANCE_NAME`_ss0_m_INP_DIS |= `$INSTANCE_NAME`_ss0_m_MASK;
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS0_PIN) */

    #if(`$INSTANCE_NAME`_SPI_MASTER_SS1_PIN)
        `$INSTANCE_NAME`_ss1_m_INP_DIS |= `$INSTANCE_NAME`_ss1_m_MASK;
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS1_PIN) */

    #if(`$INSTANCE_NAME`_SPI_MASTER_SS2_PIN)
        `$INSTANCE_NAME`_ss2_m_INP_DIS |= `$INSTANCE_NAME`_ss2_m_MASK;
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS2_PIN) */

    #if(`$INSTANCE_NAME`_SPI_MASTER_SS3_PIN)
        `$INSTANCE_NAME`_ss3_m_INP_DIS |= `$INSTANCE_NAME`_ss3_m_MASK;
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS3_PIN) */
    
    /* SCB mode is SPI Slave */
    #if(`$INSTANCE_NAME`_SPI_SLAVE_PINS)
        `$INSTANCE_NAME`_miso_s_INP_DIS |= `$INSTANCE_NAME`_miso_s_MASK;
    #endif /* (`$INSTANCE_NAME`_SPI_SLAVE_PINS) */

    /* SCB mode is UART */
    #if(`$INSTANCE_NAME`_UART_TX_PIN)
        `$INSTANCE_NAME`_tx_INP_DIS |= `$INSTANCE_NAME`_tx_MASK;
    #endif /* (`$INSTANCE_NAME`_UART_TX_PIN) */

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EnableTxPinsInputBuffer
********************************************************************************
*
* Summary:
*  Enables input buffers for TX pins. Restore changes done byte
*`$INSTANCE_NAME`_DisableTxPinsInputBuffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_EnableTxPinsInputBuffer(void)
{
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    if(`$INSTANCE_NAME`_SCB_MODE_SPI_RUNTM_CFG)
    {
        if(0u != (`$INSTANCE_NAME`_SPI_CTRL_REG & `$INSTANCE_NAME`_SPI_CTRL_MASTER))
        /* SPI Master */
        {
        #if(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN)
            `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS &= \
                                            (uint32) ~((uint32) `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake_MASK);
        #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN) */

        #if(`$INSTANCE_NAME`_MOSI_SCL_RX_PIN)
            `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_INP_DIS &= \
                                            (uint32) ~((uint32) `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_MASK);
        #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_PIN) */

        #if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
            `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_INP_DIS &= \
                                            (uint32) ~((uint32) `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_MASK);
        #endif /* (`$INSTANCE_NAME`_MISO_SDA_TX_PIN_PIN) */

        #if(`$INSTANCE_NAME`_SCLK_PIN)
            `$INSTANCE_NAME`_spi_sclk_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_spi_sclk_MASK);
        #endif /* (`$INSTANCE_NAME`_SCLK_PIN) */

        #if(`$INSTANCE_NAME`_SS0_PIN)
            `$INSTANCE_NAME`_spi_ss0_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_spi_ss0_MASK);
        #endif /* (`$INSTANCE_NAME`_SS1_PIN) */

        #if(`$INSTANCE_NAME`_SS1_PIN)
            `$INSTANCE_NAME`_spi_ss1_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_spi_ss1_MASK);
        #endif /* (`$INSTANCE_NAME`_SS1_PIN) */

        #if(`$INSTANCE_NAME`_SS2_PIN)
            `$INSTANCE_NAME`_spi_ss2_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_spi_ss2_MASK);
        #endif /* (`$INSTANCE_NAME`_SS2_PIN) */

        #if(`$INSTANCE_NAME`_SS3_PIN)
            `$INSTANCE_NAME`_spi_ss3_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_spi_ss3_MASK);
        #endif /* (`$INSTANCE_NAME`_SS3_PIN) */
        }
        else
        /* SPI Slave */
        {
        #if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
            `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_INP_DIS &= \
                                                (uint32) ~((uint32) `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_MASK);
        #endif /* (`$INSTANCE_NAME`_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else if (`$INSTANCE_NAME`_SCB_MODE_UART_RUNTM_CFG)
    {
        if(`$INSTANCE_NAME`_UART_CTRL_MODE_UART_SMARTCARD != 
                (`$INSTANCE_NAME`_UART_CTRL_REG & `$INSTANCE_NAME`_UART_CTRL_MODE_MASK))
        /* UART Standard or IrDA */
        {
        #if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
            `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_INP_DIS &= \
                                                (uint32) ~((uint32) `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_MASK);
        #endif /* (`$INSTANCE_NAME`_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else
    {
        /* Does nothing */
    }
    
#else
        
    /* SCB mode is SPI Master */
    #if(`$INSTANCE_NAME`_SPI_MASTER_PINS)
        `$INSTANCE_NAME`_sclk_m_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_sclk_m_MASK);
        `$INSTANCE_NAME`_mosi_m_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_mosi_m_MASK);
        `$INSTANCE_NAME`_miso_m_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_miso_m_MASK);
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_PINS) */

    #if(`$INSTANCE_NAME`_SPI_MASTER_SS0_PIN)
        `$INSTANCE_NAME`_ss0_m_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_ss0_m_MASK);
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS0_PIN) */

    #if(`$INSTANCE_NAME`_SPI_MASTER_SS1_PIN)
        `$INSTANCE_NAME`_ss1_m_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_ss1_m_MASK);
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS1_PIN) */

    #if(`$INSTANCE_NAME`_SPI_MASTER_SS2_PIN)
        `$INSTANCE_NAME`_ss2_m_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_ss2_m_MASK);
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS2_PIN) */

    #if(`$INSTANCE_NAME`_SPI_MASTER_SS3_PIN)
        `$INSTANCE_NAME`_ss3_m_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_ss3_m_MASK);
    #endif /* (`$INSTANCE_NAME`_SPI_MASTER_SS3_PIN) */
    
    /* SCB mode is SPI Slave */
    #if(`$INSTANCE_NAME`_SPI_SLAVE_PINS)
        `$INSTANCE_NAME`_miso_s_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_miso_s_MASK);
    #endif /* (`$INSTANCE_NAME`_SPI_SLAVE_PINS) */

    /* SCB mode is UART */
    #if(`$INSTANCE_NAME`_UART_TX_PIN)
        `$INSTANCE_NAME`_tx_INP_DIS &= (uint32) ~((uint32) `$INSTANCE_NAME`_tx_MASK);
    #endif /* (`$INSTANCE_NAME`_UART_TX_PIN) */

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
