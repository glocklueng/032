/*******************************************************************************
* File Name: `$INSTANCE_NAME`_SPI.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  SPI mode.
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
#include "`$INSTANCE_NAME`_SPI_UART_PVT.h"

#if(`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Config Structure Initialization
    ***************************************/

    const `$INSTANCE_NAME`_SPI_INIT_STRUCT `$INSTANCE_NAME`_configSpi =
    {
        `$INSTANCE_NAME`_SPI_MODE,
        `$INSTANCE_NAME`_SPI_SUB_MODE,
        `$INSTANCE_NAME`_SPI_CLOCK_MODE,
        `$INSTANCE_NAME`_SPI_OVS_FACTOR,
        `$INSTANCE_NAME`_SPI_MEDIAN_FILTER_ENABLE,
        `$INSTANCE_NAME`_SPI_LATE_MISO_SAMPLE_ENABLE,
        `$INSTANCE_NAME`_SPI_WAKE_ENABLE,
        `$INSTANCE_NAME`_SPI_RX_DATA_BITS_NUM,
        `$INSTANCE_NAME`_SPI_TX_DATA_BITS_NUM,
        `$INSTANCE_NAME`_SPI_BITS_ORDER,
        `$INSTANCE_NAME`_SPI_TRANSFER_SEPARATION,
        0u,
        NULL,
        0u,
        NULL,
        `$INSTANCE_NAME`_SCB_IRQ_INTERNAL,
        `$INSTANCE_NAME`_SPI_INTR_RX_MASK,
        `$INSTANCE_NAME`_SPI_RX_TRIGGER_LEVEL,
        `$INSTANCE_NAME`_SPI_INTR_TX_MASK,
        `$INSTANCE_NAME`_SPI_TX_TRIGGER_LEVEL
    };


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for SPI operation.
    *
    * Parameters:
    *  config:  Pointer to a structure that contains the following ordered list of
    *           fields. These fields match the selections available in the
    *           customizer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SpiInit(const `$INSTANCE_NAME`_SPI_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due bad fucntion parameter */
        }
        else
        {
            /* Configure pins */
            `$INSTANCE_NAME`_SetPins(`$INSTANCE_NAME`_SCB_MODE_SPI, config->mode, `$INSTANCE_NAME`_DUMMY_PARAM);

            /* Store internal configuration */
            `$INSTANCE_NAME`_scbMode       = (uint8) `$INSTANCE_NAME`_SCB_MODE_SPI;
            `$INSTANCE_NAME`_scbEnableWake = (uint8) config->enableWake;
            `$INSTANCE_NAME`_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            `$INSTANCE_NAME`_rxBuffer      =         config->rxBuffer;
            `$INSTANCE_NAME`_rxDataBits    = (uint8) config->rxDataBits;
            `$INSTANCE_NAME`_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            `$INSTANCE_NAME`_txBuffer      =         config->txBuffer;
            `$INSTANCE_NAME`_txDataBits    = (uint8) config->txDataBits;
            `$INSTANCE_NAME`_txBufferSize  = (uint8) config->txBufferSize;


            /* Configure SPI interface */
            `$INSTANCE_NAME`_CTRL_REG     = `$INSTANCE_NAME`_GET_CTRL_OVS(config->oversample)        |
                                            `$INSTANCE_NAME`_GET_CTRL_EC_AM_MODE(config->enableWake) |
                                            `$INSTANCE_NAME`_CTRL_SPI;

            `$INSTANCE_NAME`_SPI_CTRL_REG = `$INSTANCE_NAME`_GET_SPI_CTRL_CONTINUOUS    (config->transferSeperation)  |
                                            `$INSTANCE_NAME`_GET_SPI_CTRL_SELECT_PRECEDE(config->submode &
                                                                          `$INSTANCE_NAME`_SPI_MODE_TI_PRECEDES_MASK) |
                                            `$INSTANCE_NAME`_GET_SPI_CTRL_SCLK_MODE     (config->sclkMode)            |
                                            `$INSTANCE_NAME`_GET_SPI_CTRL_LATE_MISO_SAMPLE(config->enableLateSampling)|
                                            `$INSTANCE_NAME`_GET_SPI_CTRL_SUB_MODE      (config->submode)             |
                                            `$INSTANCE_NAME`_GET_SPI_CTRL_MASTER_MODE   (config->mode);

            /* Configure RX direction */
            `$INSTANCE_NAME`_RX_CTRL_REG     =  `$INSTANCE_NAME`_GET_RX_CTRL_DATA_WIDTH(config->rxDataBits)         |
                                                `$INSTANCE_NAME`_GET_RX_CTRL_BIT_ORDER (config->bitOrder)           |
                                                `$INSTANCE_NAME`_GET_RX_CTRL_MEDIAN    (config->enableMedianFilter) |
                                                `$INSTANCE_NAME`_SPI_RX_CTRL;

            `$INSTANCE_NAME`_RX_FIFO_CTRL_REG = `$INSTANCE_NAME`_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure TX direction */
            `$INSTANCE_NAME`_TX_CTRL_REG      = `$INSTANCE_NAME`_GET_TX_CTRL_DATA_WIDTH(config->txDataBits) |
                                                `$INSTANCE_NAME`_GET_TX_CTRL_BIT_ORDER (config->bitOrder)   |
                                                `$INSTANCE_NAME`_SPI_TX_CTRL;

            `$INSTANCE_NAME`_TX_FIFO_CTRL_REG = `$INSTANCE_NAME`_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

            /* Configure interrupt with SPI handler */
            `$INSTANCE_NAME`_SCB_IRQ_Disable();
            `$INSTANCE_NAME`_SCB_IRQ_SetVector(&`$INSTANCE_NAME`_SPI_UART_ISR);
            `$INSTANCE_NAME`_SCB_IRQ_SetPriority((uint8)`$INSTANCE_NAME`_SCB_IRQ_INTC_PRIOR_NUMBER);

            /* Configure interrupt sources */
            `$INSTANCE_NAME`_INTR_I2C_EC_MASK_REG = `$INSTANCE_NAME`_NO_INTR_SOURCES;
            `$INSTANCE_NAME`_INTR_SPI_EC_MASK_REG = `$INSTANCE_NAME`_NO_INTR_SOURCES;
            `$INSTANCE_NAME`_INTR_SLAVE_MASK_REG  = `$INSTANCE_NAME`_GET_SPI_INTR_SLAVE_MASK(config->rxInterruptMask);
            `$INSTANCE_NAME`_INTR_MASTER_MASK_REG = `$INSTANCE_NAME`_GET_SPI_INTR_MASTER_MASK(config->txInterruptMask);
            `$INSTANCE_NAME`_INTR_RX_MASK_REG     = `$INSTANCE_NAME`_GET_SPI_INTR_RX_MASK(config->rxInterruptMask);
            `$INSTANCE_NAME`_INTR_TX_MASK_REG     = `$INSTANCE_NAME`_GET_SPI_INTR_TX_MASK(config->txInterruptMask);


            /* Clear RX buffer indexes */
            `$INSTANCE_NAME`_rxBufferHead     = 0u;
            `$INSTANCE_NAME`_rxBufferTail     = 0u;
            `$INSTANCE_NAME`_rxBufferOverflow = 0u;

            /* Clrea TX buffer indexes */
            `$INSTANCE_NAME`_txBufferHead = 0u;
            `$INSTANCE_NAME`_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for SPI operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SpiInit(void)
    {
        /* Configure SPI interface */
        `$INSTANCE_NAME`_CTRL_REG     = `$INSTANCE_NAME`_SPI_DEFAULT_CTRL;
        `$INSTANCE_NAME`_SPI_CTRL_REG = `$INSTANCE_NAME`_SPI_DEFAULT_SPI_CTRL;

        /* Configure TX and RX direction */
        `$INSTANCE_NAME`_RX_CTRL_REG      = `$INSTANCE_NAME`_SPI_DEFAULT_RX_CTRL;
        `$INSTANCE_NAME`_RX_FIFO_CTRL_REG = `$INSTANCE_NAME`_SPI_DEFAULT_RX_FIFO_CTRL;

        /* Configure TX and RX direction */
        `$INSTANCE_NAME`_TX_CTRL_REG      = `$INSTANCE_NAME`_SPI_DEFAULT_TX_CTRL;
        `$INSTANCE_NAME`_TX_FIFO_CTRL_REG = `$INSTANCE_NAME`_SPI_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with SPI handler */
        #if(`$INSTANCE_NAME`_SCB_IRQ_INTERNAL)
            `$INSTANCE_NAME`_SCB_IRQ_Disable();
            `$INSTANCE_NAME`_SCB_IRQ_SetVector(&`$INSTANCE_NAME`_SPI_UART_ISR);
            `$INSTANCE_NAME`_SCB_IRQ_SetPriority((uint8)`$INSTANCE_NAME`_SCB_IRQ_INTC_PRIOR_NUMBER);
        #endif /* (`$INSTANCE_NAME`_SCB_IRQ_INTERNAL) */
        
        /* Configure interrupt sources */
        `$INSTANCE_NAME`_INTR_I2C_EC_MASK_REG = `$INSTANCE_NAME`_SPI_DEFAULT_INTR_I2C_EC_MASK;
        `$INSTANCE_NAME`_INTR_SPI_EC_MASK_REG = `$INSTANCE_NAME`_SPI_DEFAULT_INTR_SPI_EC_MASK;
        `$INSTANCE_NAME`_INTR_SLAVE_MASK_REG  = `$INSTANCE_NAME`_SPI_DEFAULT_INTR_SLAVE_MASK;
        `$INSTANCE_NAME`_INTR_MASTER_MASK_REG = `$INSTANCE_NAME`_SPI_DEFAULT_INTR_MASTER_MASK;
        `$INSTANCE_NAME`_INTR_RX_MASK_REG     = `$INSTANCE_NAME`_SPI_DEFAULT_INTR_RX_MASK;
        `$INSTANCE_NAME`_INTR_TX_MASK_REG     = `$INSTANCE_NAME`_SPI_DEFAULT_INTR_TX_MASK;

        #if(`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST)
            `$INSTANCE_NAME`_rxBufferHead     = 0u;
            `$INSTANCE_NAME`_rxBufferTail     = 0u;
            `$INSTANCE_NAME`_rxBufferOverflow = 0u;
        #endif /* (`$INSTANCE_NAME`_INTERNAL_RX_SW_BUFFER_CONST) */

        #if(`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST)
            `$INSTANCE_NAME`_txBufferHead = 0u;
            `$INSTANCE_NAME`_txBufferTail = 0u;
        #endif /* (`$INSTANCE_NAME`_INTERNAL_TX_SW_BUFFER_CONST) */
    }

#endif /* (`$INSTANCE_NAME`_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetActiveSlaveSelect
********************************************************************************
*
* Summary:
*  Selects one of the four SPI slave select signals.
*  The component should be in one of the following states to change the active
*  slave select signal source correctly:
*   - the component is disabled
*   - the component has completed all transactions (TX FIFO is empty and the
*     SpiDone flag is set)
*  This function does not check that these conditions are met.
*  At initialization time the active slave select line is slave select 0.
*
* Parameters:
*  activeSelect: The four lines available to utilize Slave Select function.
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_SpiSetActiveSlaveSelect(uint32 activeSelect)
{
    uint32 spiCtrl;

    spiCtrl = `$INSTANCE_NAME`_SPI_CTRL_REG;

    spiCtrl &= (uint32) ~`$INSTANCE_NAME`_SPI_CTRL_SLAVE_SELECT_MASK; /* Clear SS bits */
    spiCtrl |= (uint32)  `$INSTANCE_NAME`_GET_SPI_CTRL_SS(activeSelect);

    `$INSTANCE_NAME`_SPI_CTRL_REG = spiCtrl;
}


#if(`$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiSaveConfig
    ********************************************************************************
    *
    * Summary:
    *  Wakeup disabled: does nothing.
    *  Wakeup  enabled: clears SCB_backup.enableState to keep component enabled
    *  while DeepSleep. The SCB_INTR_SPI_EC_WAKE_UP enabled while initialization
    *  and tracks in active mode therefore it does not require to be cleared.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global variables:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SpiSaveConfig(void)
    {
        /* Clear wake-up before enable */
        `$INSTANCE_NAME`_ClearSpiExtClkInterruptSource(`$INSTANCE_NAME`_INTR_SPI_EC_WAKE_UP);

        /* Enable interrupt to wakeup the device */
        `$INSTANCE_NAME`_SetSpiExtClkInterruptMode(`$INSTANCE_NAME`_INTR_SPI_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SpiRestoreConfig
    ********************************************************************************
    *
    * Summary:
    *  Does nothing.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global variables:
    *  None
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SpiRestoreConfig(void)
    {
        /* Disable interrupt to wakeup the device */
        `$INSTANCE_NAME`_SetSpiExtClkInterruptMode(`$INSTANCE_NAME`_NO_INTR_SOURCES);
    }
#endif /* (`$INSTANCE_NAME`_SPI_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
