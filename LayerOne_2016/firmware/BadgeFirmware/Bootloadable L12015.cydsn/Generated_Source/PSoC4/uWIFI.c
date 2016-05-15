/*******************************************************************************
* File Name: uWIFI.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the SCB Component.
*
* Note:
*
*******************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "uWIFI_PVT.h"

#if(uWIFI_SCB_MODE_I2C_INC)
    #include "uWIFI_I2C_PVT.h"
#endif /* (uWIFI_SCB_MODE_I2C_INC) */

#if(uWIFI_SCB_MODE_EZI2C_INC)
    #include "uWIFI_EZI2C_PVT.h"
#endif /* (uWIFI_SCB_MODE_EZI2C_INC) */

#if(uWIFI_SCB_MODE_SPI_INC || uWIFI_SCB_MODE_UART_INC)
    #include "uWIFI_SPI_UART_PVT.h"
#endif /* (uWIFI_SCB_MODE_SPI_INC || uWIFI_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 uWIFI_scbMode = uWIFI_SCB_MODE_UNCONFIG;
    uint8 uWIFI_scbEnableWake;
    uint8 uWIFI_scbEnableIntr;

    /* I2C configuration variables */
    uint8 uWIFI_mode;
    uint8 uWIFI_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * uWIFI_rxBuffer;
    uint8  uWIFI_rxDataBits;
    uint32 uWIFI_rxBufferSize;

    volatile uint8 * uWIFI_txBuffer;
    uint8  uWIFI_txDataBits;
    uint32 uWIFI_txBufferSize;

    /* EZI2C configuration variables */
    uint8 uWIFI_numberOfAddr;
    uint8 uWIFI_subAddrSize;
#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/

uint8 uWIFI_initVar = 0u;

#if !defined (CY_REMOVE_uWIFI_CUSTOM_INTR_HANDLER)
    cyisraddress uWIFI_customIntrHandler = NULL;
#endif /* !defined (CY_REMOVE_uWIFI_CUSTOM_INTR_HANDLER) */


/***************************************
*    Private Function Prototypes
***************************************/

static void uWIFI_ScbEnableIntr(void);
static void uWIFI_ScbModeStop(void);


/*******************************************************************************
* Function Name: uWIFI_Init
********************************************************************************
*
* Summary:
*  Initializes the SCB component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  SCB_I2CInit, SCB_SpiInit, SCB_UartInit or SCB_EzI2CInit.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uWIFI_Init(void)
{
#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    if(uWIFI_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        uWIFI_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif(uWIFI_SCB_MODE_I2C_CONST_CFG)
    uWIFI_I2CInit();

#elif(uWIFI_SCB_MODE_SPI_CONST_CFG)
    uWIFI_SpiInit();

#elif(uWIFI_SCB_MODE_UART_CONST_CFG)
    uWIFI_UartInit();

#elif(uWIFI_SCB_MODE_EZI2C_CONST_CFG)
    uWIFI_EzI2CInit();

#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: uWIFI_Enable
********************************************************************************
*
* Summary:
*  Enables the SCB component operation.
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
void uWIFI_Enable(void)
{
#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if(!uWIFI_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        uWIFI_CTRL_REG |= uWIFI_CTRL_ENABLED;

        uWIFI_ScbEnableIntr();
    }
#else
    uWIFI_CTRL_REG |= uWIFI_CTRL_ENABLED;

    uWIFI_ScbEnableIntr();
#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: uWIFI_Start
********************************************************************************
*
* Summary:
*  Invokes SCB_Init() and SCB_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZ I2C. Otherwise this function does not enable the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  uWIFI_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void uWIFI_Start(void)
{
    if(0u == uWIFI_initVar)
    {
        uWIFI_Init();
        uWIFI_initVar = 1u; /* Component was initialized */
    }

    uWIFI_Enable();
}


/*******************************************************************************
* Function Name: uWIFI_Stop
********************************************************************************
*
* Summary:
*  Disables the SCB component and its interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uWIFI_Stop(void)
{
#if(uWIFI_SCB_IRQ_INTERNAL)
    uWIFI_DisableInt();
#endif /* (uWIFI_SCB_IRQ_INTERNAL) */

    uWIFI_CTRL_REG &= (uint32) ~uWIFI_CTRL_ENABLED;  /* Disable scb IP */

#if(uWIFI_SCB_IRQ_INTERNAL)
    uWIFI_ClearPendingInt();
#endif /* (uWIFI_SCB_IRQ_INTERNAL) */

    uWIFI_ScbModeStop(); /* Calls scbMode specific Stop function */
}


/*******************************************************************************
* Function Name: uWIFI_SetRxFifoLevel
********************************************************************************
*
* Summary:
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
* Parameters:
*  level: Level in the RX FIFO to generate RX level interrupt.
*         The range of valid level values is between 0 and RX FIFO depth - 1.
*
* Return:
*  None
*
*******************************************************************************/
void uWIFI_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = uWIFI_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~uWIFI_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (uWIFI_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    uWIFI_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: uWIFI_SetTxFifoLevel
********************************************************************************
*
* Summary:
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has more entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
* Parameters:
*  level: Level in the TX FIFO to generate TX level interrupt.
*         The range of valid level values is between 0 and TX FIFO depth - 1.
*
* Return:
*  None
*
*******************************************************************************/
void uWIFI_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = uWIFI_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~uWIFI_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (uWIFI_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    uWIFI_TX_FIFO_CTRL_REG = txFifoCtrl;
}


/*******************************************************************************
* Function Name: uWIFI_SetCustomInterruptHandler
********************************************************************************
*
* Summary:
*  Registers a function to be called by the internal interrupt handler.
*  First the function that is registered is called, then the internal interrupt
*  handler performs any operation such as software buffer management functions
*  before the interrupt returns.  It is the user's responsibility not to break
*  the software buffer operations. Only one custom handler is supported, which
*  is the function provided by the most recent call.
*  At the initialization time no custom handler is registered.
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
void uWIFI_SetCustomInterruptHandler(cyisraddress func)
{
#if !defined (CY_REMOVE_uWIFI_CUSTOM_INTR_HANDLER)
    uWIFI_customIntrHandler = func; /* Register interrupt handler */
#else
    if(NULL != func)
    {
        /* Suppress compiler warning */
    }
#endif /* !defined (CY_REMOVE_uWIFI_CUSTOM_INTR_HANDLER) */
}


/*******************************************************************************
* Function Name: uWIFI_ScbModeEnableIntr
********************************************************************************
*
* Summary:
*  Enables an interrupt for a specific mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void uWIFI_ScbEnableIntr(void)
{
#if(uWIFI_SCB_IRQ_INTERNAL)
    #if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Enable interrupt in NVIC */
        if(0u != uWIFI_scbEnableIntr)
        {
            uWIFI_EnableInt();
        }
    #else
        uWIFI_EnableInt();

    #endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (uWIFI_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: uWIFI_ScbModeStop
********************************************************************************
*
* Summary:
*  Calls the Stop function for a specific operation mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void uWIFI_ScbModeStop(void)
{
#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    if(uWIFI_SCB_MODE_I2C_RUNTM_CFG)
    {
        uWIFI_I2CStop();
    }
    else if (uWIFI_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        uWIFI_EzI2CStop();
    }
#if (!uWIFI_CY_SCBIP_V1)
    else if (uWIFI_SCB_MODE_UART_RUNTM_CFG)
    {
        uWIFI_UartStop();
    }
#endif /* (!uWIFI_CY_SCBIP_V1) */
    else
    {
        /* Do nothing for other modes */
    }
#elif(uWIFI_SCB_MODE_I2C_CONST_CFG)
    uWIFI_I2CStop();

#elif(uWIFI_SCB_MODE_EZI2C_CONST_CFG)
    uWIFI_EzI2CStop();

#elif(uWIFI_SCB_MODE_UART_CONST_CFG)
    uWIFI_UartStop();

#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if(uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: uWIFI_SetPins
    ********************************************************************************
    *
    * Summary:
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    * Parameters:
    *  mode:      Mode of SCB operation.
    *  subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uWIFI_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 hsiomSel [uWIFI_SCB_PINS_NUMBER];
        uint32 pinsDm   [uWIFI_SCB_PINS_NUMBER];

    #if (!uWIFI_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!uWIFI_CY_SCBIP_V1) */

        uint32 i;

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for(i = 0u; i < uWIFI_SCB_PINS_NUMBER; i++)
        {
            hsiomSel[i]  = uWIFI_HSIOM_DEF_SEL;
            pinsDm[i]    = uWIFI_PIN_DM_ALG_HIZ;
        }

        if((uWIFI_SCB_MODE_I2C   == mode) ||
           (uWIFI_SCB_MODE_EZI2C == mode))
        {
            hsiomSel[uWIFI_MOSI_SCL_RX_PIN_INDEX] = uWIFI_HSIOM_I2C_SEL;
            hsiomSel[uWIFI_MISO_SDA_TX_PIN_INDEX] = uWIFI_HSIOM_I2C_SEL;

            pinsDm[uWIFI_MOSI_SCL_RX_PIN_INDEX] = uWIFI_PIN_DM_OD_LO;
            pinsDm[uWIFI_MISO_SDA_TX_PIN_INDEX] = uWIFI_PIN_DM_OD_LO;
        }
    #if (!uWIFI_CY_SCBIP_V1)
        else if(uWIFI_SCB_MODE_SPI == mode)
        {
            hsiomSel[uWIFI_MOSI_SCL_RX_PIN_INDEX] = uWIFI_HSIOM_SPI_SEL;
            hsiomSel[uWIFI_MISO_SDA_TX_PIN_INDEX] = uWIFI_HSIOM_SPI_SEL;
            hsiomSel[uWIFI_SCLK_PIN_INDEX]        = uWIFI_HSIOM_SPI_SEL;

            if(uWIFI_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[uWIFI_MOSI_SCL_RX_PIN_INDEX] = uWIFI_PIN_DM_DIG_HIZ;
                pinsDm[uWIFI_MISO_SDA_TX_PIN_INDEX] = uWIFI_PIN_DM_STRONG;
                pinsDm[uWIFI_SCLK_PIN_INDEX]        = uWIFI_PIN_DM_DIG_HIZ;

            #if(uWIFI_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[uWIFI_SS0_PIN_INDEX] = uWIFI_HSIOM_SPI_SEL;
                pinsDm  [uWIFI_SS0_PIN_INDEX] = uWIFI_PIN_DM_DIG_HIZ;
            #endif /* (uWIFI_SS1_PIN) */

            #if(uWIFI_MISO_SDA_TX_PIN)
                /* Disable input buffer */
                 pinsInBuf |= uWIFI_MISO_SDA_TX_PIN_MASK;
            #endif /* (uWIFI_MISO_SDA_TX_PIN_PIN) */
            }
            else /* (Master) */
            {
                pinsDm[uWIFI_MOSI_SCL_RX_PIN_INDEX] = uWIFI_PIN_DM_STRONG;
                pinsDm[uWIFI_MISO_SDA_TX_PIN_INDEX] = uWIFI_PIN_DM_DIG_HIZ;
                pinsDm[uWIFI_SCLK_PIN_INDEX]        = uWIFI_PIN_DM_STRONG;

            #if(uWIFI_SS0_PIN)
                hsiomSel [uWIFI_SS0_PIN_INDEX] = uWIFI_HSIOM_SPI_SEL;
                pinsDm   [uWIFI_SS0_PIN_INDEX] = uWIFI_PIN_DM_STRONG;
                pinsInBuf                                |= uWIFI_SS0_PIN_MASK;
            #endif /* (uWIFI_SS0_PIN) */

            #if(uWIFI_SS1_PIN)
                hsiomSel [uWIFI_SS1_PIN_INDEX] = uWIFI_HSIOM_SPI_SEL;
                pinsDm   [uWIFI_SS1_PIN_INDEX] = uWIFI_PIN_DM_STRONG;
                pinsInBuf                                |= uWIFI_SS1_PIN_MASK;
            #endif /* (uWIFI_SS1_PIN) */

            #if(uWIFI_SS2_PIN)
                hsiomSel [uWIFI_SS2_PIN_INDEX] = uWIFI_HSIOM_SPI_SEL;
                pinsDm   [uWIFI_SS2_PIN_INDEX] = uWIFI_PIN_DM_STRONG;
                pinsInBuf                                |= uWIFI_SS2_PIN_MASK;
            #endif /* (uWIFI_SS2_PIN) */

            #if(uWIFI_SS3_PIN)
                hsiomSel [uWIFI_SS3_PIN_INDEX] = uWIFI_HSIOM_SPI_SEL;
                pinsDm   [uWIFI_SS3_PIN_INDEX] = uWIFI_PIN_DM_STRONG;
                pinsInBuf                                |= uWIFI_SS3_PIN_MASK;
            #endif /* (uWIFI_SS2_PIN) */

                /* Disable input buffers */
            #if(uWIFI_MOSI_SCL_RX_PIN)
                pinsInBuf |= uWIFI_MOSI_SCL_RX_PIN_MASK;
            #endif /* (uWIFI_MOSI_SCL_RX_PIN) */

             #if(uWIFI_MOSI_SCL_RX_WAKE_PIN)
                pinsInBuf |= uWIFI_MOSI_SCL_RX_WAKE_PIN_MASK;
            #endif /* (uWIFI_MOSI_SCL_RX_WAKE_PIN) */

            #if(uWIFI_SCLK_PIN)
                pinsInBuf |= uWIFI_SCLK_PIN_MASK;
            #endif /* (uWIFI_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if(uWIFI_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
                hsiomSel[uWIFI_MISO_SDA_TX_PIN_INDEX] = uWIFI_HSIOM_UART_SEL;
                pinsDm  [uWIFI_MISO_SDA_TX_PIN_INDEX] = uWIFI_PIN_DM_OD_LO;
            }
            else /* Standard or IrDA */
            {
                if(0u != (uWIFI_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                    hsiomSel[uWIFI_MOSI_SCL_RX_PIN_INDEX] = uWIFI_HSIOM_UART_SEL;
                    pinsDm  [uWIFI_MOSI_SCL_RX_PIN_INDEX] = uWIFI_PIN_DM_DIG_HIZ;
                }

                if(0u != (uWIFI_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                    hsiomSel[uWIFI_MISO_SDA_TX_PIN_INDEX] = uWIFI_HSIOM_UART_SEL;
                    pinsDm  [uWIFI_MISO_SDA_TX_PIN_INDEX] = uWIFI_PIN_DM_STRONG;

                #if(uWIFI_MISO_SDA_TX_PIN)
                     pinsInBuf |= uWIFI_MISO_SDA_TX_PIN_MASK;
                #endif /* (uWIFI_MISO_SDA_TX_PIN_PIN) */
                }

            #if !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
                if(uWIFI_UART_MODE_STD == subMode)
                {
                    if(0u != (uWIFI_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                        hsiomSel[uWIFI_SCLK_PIN_INDEX] = uWIFI_HSIOM_UART_SEL;
                        pinsDm  [uWIFI_SCLK_PIN_INDEX] = uWIFI_PIN_DM_DIG_HIZ;
                    }

                    if(0u != (uWIFI_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                        hsiomSel[uWIFI_SS0_PIN_INDEX] = uWIFI_HSIOM_UART_SEL;
                        pinsDm  [uWIFI_SS0_PIN_INDEX] = uWIFI_PIN_DM_STRONG;

                    #if(uWIFI_SS0_PIN)
                        /* Disable input buffer */
                        pinsInBuf |= uWIFI_SS0_PIN_MASK;
                    #endif /* (uWIFI_SS0_PIN) */
                    }
                }
            #endif /* !(uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */
            }
        }
    #endif /* (!uWIFI_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if(uWIFI_MOSI_SCL_RX_PIN)
        uWIFI_SET_HSIOM_SEL(uWIFI_MOSI_SCL_RX_HSIOM_REG,
                                       uWIFI_MOSI_SCL_RX_HSIOM_MASK,
                                       uWIFI_MOSI_SCL_RX_HSIOM_POS,
                                       hsiomSel[uWIFI_MOSI_SCL_RX_PIN_INDEX]);

        uWIFI_spi_mosi_i2c_scl_uart_rx_SetDriveMode((uint8) pinsDm[uWIFI_MOSI_SCL_RX_PIN_INDEX]);

    #if (!uWIFI_CY_SCBIP_V1)
        uWIFI_SET_INP_DIS(uWIFI_spi_mosi_i2c_scl_uart_rx_INP_DIS,
                                     uWIFI_spi_mosi_i2c_scl_uart_rx_MASK,
                                     (0u != (pinsInBuf & uWIFI_MOSI_SCL_RX_PIN_MASK)));
    #endif /* (!uWIFI_CY_SCBIP_V1) */
    #endif /* (uWIFI_MOSI_SCL_RX_PIN) */

    #if(uWIFI_MOSI_SCL_RX_WAKE_PIN)
        uWIFI_SET_HSIOM_SEL(uWIFI_MOSI_SCL_RX_WAKE_HSIOM_REG,
                                       uWIFI_MOSI_SCL_RX_WAKE_HSIOM_MASK,
                                       uWIFI_MOSI_SCL_RX_WAKE_HSIOM_POS,
                                       hsiomSel[uWIFI_MOSI_SCL_RX_WAKE_PIN_INDEX]);

        uWIFI_spi_mosi_i2c_scl_uart_rx_wake_SetDriveMode((uint8)
                                                               pinsDm[uWIFI_MOSI_SCL_RX_WAKE_PIN_INDEX]);

        uWIFI_SET_INP_DIS(uWIFI_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS,
                                     uWIFI_spi_mosi_i2c_scl_uart_rx_wake_MASK,
                                     (0u != (pinsInBuf & uWIFI_MOSI_SCL_RX_WAKE_PIN_MASK)));

         /* Set interrupt on falling edge */
        uWIFI_SET_INCFG_TYPE(uWIFI_MOSI_SCL_RX_WAKE_INTCFG_REG,
                                        uWIFI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK,
                                        uWIFI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS,
                                        uWIFI_INTCFG_TYPE_FALLING_EDGE);
    #endif /* (uWIFI_MOSI_SCL_RX_WAKE_PIN) */

    #if(uWIFI_MISO_SDA_TX_PIN)
        uWIFI_SET_HSIOM_SEL(uWIFI_MISO_SDA_TX_HSIOM_REG,
                                       uWIFI_MISO_SDA_TX_HSIOM_MASK,
                                       uWIFI_MISO_SDA_TX_HSIOM_POS,
                                       hsiomSel[uWIFI_MISO_SDA_TX_PIN_INDEX]);

        uWIFI_spi_miso_i2c_sda_uart_tx_SetDriveMode((uint8) pinsDm[uWIFI_MISO_SDA_TX_PIN_INDEX]);

    #if (!uWIFI_CY_SCBIP_V1)
        uWIFI_SET_INP_DIS(uWIFI_spi_miso_i2c_sda_uart_tx_INP_DIS,
                                     uWIFI_spi_miso_i2c_sda_uart_tx_MASK,
                                    (0u != (pinsInBuf & uWIFI_MISO_SDA_TX_PIN_MASK)));
    #endif /* (!uWIFI_CY_SCBIP_V1) */
    #endif /* (uWIFI_MOSI_SCL_RX_PIN) */

    #if(uWIFI_SCLK_PIN)
        uWIFI_SET_HSIOM_SEL(uWIFI_SCLK_HSIOM_REG, uWIFI_SCLK_HSIOM_MASK,
                                       uWIFI_SCLK_HSIOM_POS, hsiomSel[uWIFI_SCLK_PIN_INDEX]);

        uWIFI_spi_sclk_SetDriveMode((uint8) pinsDm[uWIFI_SCLK_PIN_INDEX]);

        uWIFI_SET_INP_DIS(uWIFI_spi_sclk_INP_DIS,
                                     uWIFI_spi_sclk_MASK,
                                     (0u != (pinsInBuf & uWIFI_SCLK_PIN_MASK)));
    #endif /* (uWIFI_SCLK_PIN) */

    #if(uWIFI_SS0_PIN)
        uWIFI_SET_HSIOM_SEL(uWIFI_SS0_HSIOM_REG, uWIFI_SS0_HSIOM_MASK,
                                       uWIFI_SS0_HSIOM_POS, hsiomSel[uWIFI_SS0_PIN_INDEX]);

        uWIFI_spi_ss0_SetDriveMode((uint8) pinsDm[uWIFI_SS0_PIN_INDEX]);

        uWIFI_SET_INP_DIS(uWIFI_spi_ss0_INP_DIS,
                                     uWIFI_spi_ss0_MASK,
                                     (0u != (pinsInBuf & uWIFI_SS0_PIN_MASK)));
    #endif /* (uWIFI_SS1_PIN) */

    #if(uWIFI_SS1_PIN)
        uWIFI_SET_HSIOM_SEL(uWIFI_SS1_HSIOM_REG, uWIFI_SS1_HSIOM_MASK,
                                       uWIFI_SS1_HSIOM_POS, hsiomSel[uWIFI_SS1_PIN_INDEX]);

        uWIFI_spi_ss1_SetDriveMode((uint8) pinsDm[uWIFI_SS1_PIN_INDEX]);

        uWIFI_SET_INP_DIS(uWIFI_spi_ss1_INP_DIS,
                                     uWIFI_spi_ss1_MASK,
                                     (0u != (pinsInBuf & uWIFI_SS1_PIN_MASK)));
    #endif /* (uWIFI_SS1_PIN) */

    #if(uWIFI_SS2_PIN)
        uWIFI_SET_HSIOM_SEL(uWIFI_SS2_HSIOM_REG, uWIFI_SS2_HSIOM_MASK,
                                       uWIFI_SS2_HSIOM_POS, hsiomSel[uWIFI_SS2_PIN_INDEX]);

        uWIFI_spi_ss2_SetDriveMode((uint8) pinsDm[uWIFI_SS2_PIN_INDEX]);

        uWIFI_SET_INP_DIS(uWIFI_spi_ss2_INP_DIS,
                                     uWIFI_spi_ss2_MASK,
                                     (0u != (pinsInBuf & uWIFI_SS2_PIN_MASK)));
    #endif /* (uWIFI_SS2_PIN) */

    #if(uWIFI_SS3_PIN)
        uWIFI_SET_HSIOM_SEL(uWIFI_SS3_HSIOM_REG,  uWIFI_SS3_HSIOM_MASK,
                                       uWIFI_SS3_HSIOM_POS, hsiomSel[uWIFI_SS3_PIN_INDEX]);

        uWIFI_spi_ss3_SetDriveMode((uint8) pinsDm[uWIFI_SS3_PIN_INDEX]);

        uWIFI_SET_INP_DIS(uWIFI_spi_ss3_INP_DIS,
                                     uWIFI_spi_ss3_MASK,
                                     (0u != (pinsInBuf & uWIFI_SS3_PIN_MASK)));
    #endif /* (uWIFI_SS3_PIN) */
    }

#endif /* (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: uWIFI_I2CSlaveNackGeneration
    ********************************************************************************
    *
    * Summary:
    *  Sets command to generate NACK to the address or data.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uWIFI_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (uWIFI_CTRL_REG & uWIFI_CTRL_EC_AM_MODE)) &&
            (0u == (uWIFI_I2C_CTRL_REG & uWIFI_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            uWIFI_CTRL_REG &= ~uWIFI_CTRL_EC_AM_MODE;
            uWIFI_CTRL_REG |=  uWIFI_CTRL_EC_AM_MODE;
        }

        uWIFI_I2C_SLAVE_CMD_REG = uWIFI_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (uWIFI_CY_SCBIP_V0 || uWIFI_CY_SCBIP_V1) */


/* [] END OF FILE */
