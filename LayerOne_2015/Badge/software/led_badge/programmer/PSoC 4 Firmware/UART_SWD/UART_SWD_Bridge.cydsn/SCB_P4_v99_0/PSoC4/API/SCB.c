/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the API for the SCB Component.
*
* Note:
*
*******************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

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


/**********************************
*    Run Time Configuration Vars
**********************************/

/* Stores internal component configuration for unconfigured mode */
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common config vars */
    uint8 `$INSTANCE_NAME`_scbMode = `$INSTANCE_NAME`_SCB_MODE_UNCONFIG;
    uint8 `$INSTANCE_NAME`_scbEnableWake;
    uint8 `$INSTANCE_NAME`_scbEnableIntr;

    /* I2C config vars */
    uint8 `$INSTANCE_NAME`_mode;
    uint8 `$INSTANCE_NAME`_acceptAddr;

    /* SPI/UART config vars */
    volatile uint8 * `$INSTANCE_NAME`_rxBuffer;
    uint8  `$INSTANCE_NAME`_rxDataBits;
    uint32 `$INSTANCE_NAME`_rxBufferSize;

    volatile uint8 * `$INSTANCE_NAME`_txBuffer;
    uint8  `$INSTANCE_NAME`_txDataBits;
    uint32 `$INSTANCE_NAME`_txBufferSize;

    /* EZI2C config vars */
    uint8 `$INSTANCE_NAME`_numberOfAddr;
    uint8 `$INSTANCE_NAME`_subAddrSize;
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */


/**********************************
*     Common SCB Vars
**********************************/

uint8 `$INSTANCE_NAME`_initVar = 0u;
cyisraddress `$INSTANCE_NAME`_customIntrHandler = NULL;


/***************************************
*    Private Function Prototypes
***************************************/

static void `$INSTANCE_NAME`_ScbEnableIntr(void);
static void `$INSTANCE_NAME`_ScbModeStop(void);


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Init
********************************************************************************
*
* Summary:
*  Initializes SCB component to operate in one of selected configurations:
*  I2C, SPI, UART, EZI2C or EZSPI.
*  This function does not do any initialization when configuration is set to
*  Unconfigured SCB.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_Init(void)
{
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        `$INSTANCE_NAME`_initVar = 0u; /* Clear init var */
    }
    else
    {
        /* Initialization was done before call */
    }

#elif(`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG)
    `$INSTANCE_NAME`_I2CInit();

#elif(`$INSTANCE_NAME`_SCB_MODE_SPI_CONST_CFG)
    `$INSTANCE_NAME`_SpiInit();

#elif(`$INSTANCE_NAME`_SCB_MODE_UART_CONST_CFG)
    `$INSTANCE_NAME`_UartInit();

#elif(`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG)
    `$INSTANCE_NAME`_EzI2CInit();

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Enable
********************************************************************************
*
* Summary:
*  Enables SCB component operation.
*  The SCB configuration should be not changed when the component is enabled.
*  Any configuration changes should be made after disabling the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_Enable(void)
{
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    if(!`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        /* Enable SCB block, only if already configured */
        `$INSTANCE_NAME`_CTRL_REG |= `$INSTANCE_NAME`_CTRL_ENABLED;
        
        /* Enable interrupt */
        `$INSTANCE_NAME`_ScbEnableIntr();
    }
#else
    `$INSTANCE_NAME`_CTRL_REG |= `$INSTANCE_NAME`_CTRL_ENABLED; /* Enable SCB block */
    
    `$INSTANCE_NAME`_ScbEnableIntr();
#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
*
* Summary:
*  Invokes SCB_Init() and SCB_Enable().
*  After this function call the component is enabled and ready for operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  `$INSTANCE_NAME`_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Start(void)
{
    if(0u == `$INSTANCE_NAME`_initVar)
    {
        `$INSTANCE_NAME`_initVar = 1u; /* Component was initialized */
        `$INSTANCE_NAME`_Init();       /* Initialize component      */
    }

    `$INSTANCE_NAME`_Enable();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Stop
********************************************************************************
*
* Summary:
*  Disables the SCB component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_Stop(void)
{
#if(`$INSTANCE_NAME`_SCB_IRQ_INTERNAL)
    `$INSTANCE_NAME`_SCB_IRQ_Disable();     /* Disable interrupt before block */
#endif /* (`$INSTANCE_NAME`_SCB_IRQ_INTERNAL) */

    `$INSTANCE_NAME`_CTRL_REG &= (uint32) ~`$INSTANCE_NAME`_CTRL_ENABLED;  /* Disable SCB block */

#if(`$INSTANCE_NAME`_SCB_IRQ_INTERNAL)
    `$INSTANCE_NAME`_SCB_IRQ_ClearPending(); /* Clear pending interrupt */
#endif /* (`$INSTANCE_NAME`_SCB_IRQ_INTERNAL) */
    
    `$INSTANCE_NAME`_ScbModeStop(); /* Calls scbMode specific Stop function */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetCustomInterruptHandler
********************************************************************************
*
* Summary:
*  Registers a function to be called by the internal interrupt handler.
*  First the function that is registered is called, then the internal interrupt
*  handler performs any operations such as software buffer management functions
*  before the interrupt returns.  It is user's responsibility to not break the
*  software buffer operations. Only one custom handler is supported, which is
*  the function provided by the most recent call.
*  At initialization time no custom handler is registered.
*
* Parameters:
*  func: Pointer to the function to register.
*        The value NULL indicates to remove the current custom interrupt
*        handler.
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetCustomInterruptHandler(cyisraddress func)
{
    `$INSTANCE_NAME`_customIntrHandler = func; /* Register interrupt handler */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ScbModeEnableIntr
********************************************************************************
*
* Summary:
*  Enables interrupt for specific mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void `$INSTANCE_NAME`_ScbEnableIntr(void)
{
#if(`$INSTANCE_NAME`_SCB_IRQ_INTERNAL)
    #if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Enable interrupt source */
        if(0u != `$INSTANCE_NAME`_scbEnableIntr)
        {
            `$INSTANCE_NAME`_SCB_IRQ_Enable();
        }
    #else
        `$INSTANCE_NAME`_SCB_IRQ_Enable();
        
    #endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (`$INSTANCE_NAME`_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ScbModeEnableIntr
********************************************************************************
*
* Summary:
*  Calls Stop function for specific operation mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void `$INSTANCE_NAME`_ScbModeStop(void)
{
#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    if(`$INSTANCE_NAME`_SCB_MODE_I2C_RUNTM_CFG)
    {
        `$INSTANCE_NAME`_I2CStop();
    }
    else if(`$INSTANCE_NAME`_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        `$INSTANCE_NAME`_EzI2CStop();
    }
    else
    {
        /* None of modes above */
    }
#elif(`$INSTANCE_NAME`_SCB_MODE_I2C_CONST_CFG)
    `$INSTANCE_NAME`_I2CStop();

#elif(`$INSTANCE_NAME`_SCB_MODE_EZI2C_CONST_CFG)
    `$INSTANCE_NAME`_EzI2CStop();

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SetPins
    ********************************************************************************
    *
    * Summary:
    *  Sets pins settings accordingly to selected operation mode.
    *  Only available in Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when specific mode of operation
    *  selected in design time.
    *
    * Parameters:
    *  mode:      Mode of SCB operation.
    *  subMode:   Submode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  uartTxRx:  Direction for UART.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SetPins(uint32 mode, uint32 subMode, uint32 uartTxRx)
    {
        uint32 hsiomSel[`$INSTANCE_NAME`_SCB_PINS_NUMBER];
        uint32 pinsDm  [`$INSTANCE_NAME`_SCB_PINS_NUMBER];
        uint32 i;

        /* Make all unused */
        for(i = 0u; i < `$INSTANCE_NAME`_SCB_PINS_NUMBER; i++)
        {
            hsiomSel[i] = `$INSTANCE_NAME`_HSIOM_DEF_SEL;
            pinsDm[i]   = `$INSTANCE_NAME`_PIN_DM_ALG_HIZ;
        }

        /* Choice the Dm and HSIOM */
        if((`$INSTANCE_NAME`_SCB_MODE_I2C   == mode) ||
           (`$INSTANCE_NAME`_SCB_MODE_EZI2C == mode))
        {
            hsiomSel[`$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_I2C_SEL;
            hsiomSel[`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_I2C_SEL;

            pinsDm[`$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_OD_LO;
            pinsDm[`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_OD_LO;
        }
        else if(`$INSTANCE_NAME`_SCB_MODE_SPI == mode)
        {
            hsiomSel[`$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_SPI_SEL;
            hsiomSel[`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_SPI_SEL;
            hsiomSel[`$INSTANCE_NAME`_SCLK_PIN_INDEX]        = `$INSTANCE_NAME`_HSIOM_SPI_SEL;

            if(`$INSTANCE_NAME`_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[`$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_DIG_HIZ;
                pinsDm[`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_STRONG;
                pinsDm[`$INSTANCE_NAME`_SCLK_PIN_INDEX]        = `$INSTANCE_NAME`_PIN_DM_DIG_HIZ;

            #if(`$INSTANCE_NAME`_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[`$INSTANCE_NAME`_SS0_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_SPI_SEL;
                pinsDm  [`$INSTANCE_NAME`_SS0_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_DIG_HIZ;
            #endif /* (`$INSTANCE_NAME`_SS1_PIN) */
            }
            else /* (Master) */
            {
                pinsDm[`$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_STRONG;
                pinsDm[`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_DIG_HIZ;
                pinsDm[`$INSTANCE_NAME`_SCLK_PIN_INDEX]        = `$INSTANCE_NAME`_PIN_DM_STRONG;

            #if(`$INSTANCE_NAME`_SS0_PIN)
                hsiomSel[`$INSTANCE_NAME`_SS0_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_SPI_SEL;
                pinsDm  [`$INSTANCE_NAME`_SS0_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_STRONG;
            #endif /* (`$INSTANCE_NAME`_SS0_PIN) */

            #if(`$INSTANCE_NAME`_SS1_PIN)
                hsiomSel[`$INSTANCE_NAME`_SS1_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_SPI_SEL;
                pinsDm  [`$INSTANCE_NAME`_SS1_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_STRONG;
            #endif /* (`$INSTANCE_NAME`_SS1_PIN) */

            #if(`$INSTANCE_NAME`_SS2_PIN)
                hsiomSel[`$INSTANCE_NAME`_SS2_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_SPI_SEL;
                pinsDm  [`$INSTANCE_NAME`_SS2_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_STRONG;
            #endif /* (`$INSTANCE_NAME`_SS2_PIN) */

            #if(`$INSTANCE_NAME`_SS3_PIN)
                hsiomSel[`$INSTANCE_NAME`_SS3_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_SPI_SEL;
                pinsDm  [`$INSTANCE_NAME`_SS3_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_STRONG;
            #endif /* (`$INSTANCE_NAME`_SS2_PIN) */
            }
        }
        else /* UART */
        {
            if(`$INSTANCE_NAME`_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
                hsiomSel[`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_UART_SEL;
                pinsDm  [`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_OD_LO;
            }
            else /* Standard or IrDA */
            {
                if(0u != (`$INSTANCE_NAME`_UART_RX & uartTxRx))
                {
                    hsiomSel[`$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_UART_SEL;
                    pinsDm  [`$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_DIG_HIZ;
                }

                if(0u != (`$INSTANCE_NAME`_UART_TX & uartTxRx))
                {
                    hsiomSel[`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX] = `$INSTANCE_NAME`_HSIOM_UART_SEL;
                    pinsDm  [`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX] = `$INSTANCE_NAME`_PIN_DM_STRONG;
                }
            }
        }

        /* Condfigure pins: set HSIOM and DM */
        /* Condfigure pins: DR registers configuration remains unchanged for cyfitter_cfg() */

    #if(`$INSTANCE_NAME`_MOSI_SCL_RX_PIN)
        `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_REG,
                                       `$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_MASK,
                                       `$INSTANCE_NAME`_MOSI_SCL_RX_HSIOM_POS,
                                       hsiomSel[`$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_PIN) */

    #if(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN)
        `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_REG,
                                       `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_MASK,
                                       `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_HSIOM_POS,
                                       hsiomSel[`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN) */

    #if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
        `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_MISO_SDA_TX_HSIOM_REG,
                                       `$INSTANCE_NAME`_MISO_SDA_TX_HSIOM_MASK,
                                       `$INSTANCE_NAME`_MISO_SDA_TX_HSIOM_POS,
                                       hsiomSel[`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_PIN) */

    #if(`$INSTANCE_NAME`_SCLK_PIN)
        `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_SCLK_HSIOM_REG, `$INSTANCE_NAME`_SCLK_HSIOM_MASK,
                                       `$INSTANCE_NAME`_SCLK_HSIOM_POS, hsiomSel[`$INSTANCE_NAME`_SCLK_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SCLK_PIN) */

    #if(`$INSTANCE_NAME`_SS0_PIN)
        `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_SS0_HSIOM_REG, `$INSTANCE_NAME`_SS0_HSIOM_MASK,
                                       `$INSTANCE_NAME`_SS0_HSIOM_POS, hsiomSel[`$INSTANCE_NAME`_SS0_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SS1_PIN) */

    #if(`$INSTANCE_NAME`_SS1_PIN)
        `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_SS1_HSIOM_REG, `$INSTANCE_NAME`_SS1_HSIOM_MASK,
                                       `$INSTANCE_NAME`_SS1_HSIOM_POS, hsiomSel[`$INSTANCE_NAME`_SS1_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SS1_PIN) */

    #if(`$INSTANCE_NAME`_SS2_PIN)
        `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_SS2_HSIOM_REG, `$INSTANCE_NAME`_SS2_HSIOM_MASK,
                                       `$INSTANCE_NAME`_SS2_HSIOM_POS, hsiomSel[`$INSTANCE_NAME`_SS2_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SS2_PIN) */

    #if(`$INSTANCE_NAME`_SS3_PIN)
        `$INSTANCE_NAME`_SET_HSIOM_SEL(`$INSTANCE_NAME`_SS3_HSIOM_REG,  `$INSTANCE_NAME`_SS3_HSIOM_MASK,
                                       `$INSTANCE_NAME`_SS3_HSIOM_POS, hsiomSel[`$INSTANCE_NAME`_SS3_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SS3_PIN) */



    #if(`$INSTANCE_NAME`_MOSI_SCL_RX_PIN)
        `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_SetDriveMode((uint8)
                                                               pinsDm[`$INSTANCE_NAME`_MOSI_SCL_RX_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_PIN) */

    #if(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN)
    `$INSTANCE_NAME`_spi_mosi_i2c_scl_uart_rx_wake_SetDriveMode((uint8)
                                                               pinsDm[`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN_INDEX]);

    /* Set interrupt on rising edge */
    `$INSTANCE_NAME`_SET_INCFG_TYPE(`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_INTCFG_REG,
                                    `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK,
                                    `$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS,
                                    `$INSTANCE_NAME`_INTCFG_TYPE_FALLING_EDGE);

    #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_WAKE_PIN) */

    #if(`$INSTANCE_NAME`_MISO_SDA_TX_PIN)
        `$INSTANCE_NAME`_spi_miso_i2c_sda_uart_tx_SetDriveMode((uint8)
                                                                    pinsDm[`$INSTANCE_NAME`_MISO_SDA_TX_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_MOSI_SCL_RX_PIN) */

    #if(`$INSTANCE_NAME`_SCLK_PIN)
        `$INSTANCE_NAME`_spi_sclk_SetDriveMode((uint8) pinsDm[`$INSTANCE_NAME`_SCLK_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SCLK_PIN) */

    #if(`$INSTANCE_NAME`_SS0_PIN)
        `$INSTANCE_NAME`_spi_ss0_SetDriveMode((uint8) pinsDm[`$INSTANCE_NAME`_SS0_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SS0_PIN) */

    #if(`$INSTANCE_NAME`_SS1_PIN)
        `$INSTANCE_NAME`_spi_ss1_SetDriveMode((uint8) pinsDm[`$INSTANCE_NAME`_SS1_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SS1_PIN) */

    #if(`$INSTANCE_NAME`_SS2_PIN)
        `$INSTANCE_NAME`_spi_ss2_SetDriveMode((uint8) pinsDm[`$INSTANCE_NAME`_SS2_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SS2_PIN) */

    #if(`$INSTANCE_NAME`_SS3_PIN)
        `$INSTANCE_NAME`_spi_ss3_SetDriveMode((uint8) pinsDm[`$INSTANCE_NAME`_SS3_PIN_INDEX]);
    #endif /* (`$INSTANCE_NAME`_SS3_PIN) */
    }

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
