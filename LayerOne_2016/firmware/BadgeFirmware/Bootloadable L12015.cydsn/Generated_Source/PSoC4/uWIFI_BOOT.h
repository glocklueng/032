/*******************************************************************************
* File Name: uWIFI_BOOT.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the bootloader
*  communication interface of SCB component.
*
* Note:
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_uWIFI_H)
#define CY_SCB_BOOT_uWIFI_H

#include "uWIFI_PVT.h"

#if (uWIFI_SCB_MODE_I2C_INC)
    #include "uWIFI_I2C.h"
#endif /* (uWIFI_SCB_MODE_I2C_INC) */

#if (uWIFI_SCB_MODE_EZI2C_INC)
    #include "uWIFI_EZI2C.h"
#endif /* (uWIFI_SCB_MODE_EZI2C_INC) */

#if (uWIFI_SCB_MODE_SPI_INC || uWIFI_SCB_MODE_UART_INC)
    #include "uWIFI_SPI_UART.h"
#endif /* (uWIFI_SCB_MODE_SPI_INC || uWIFI_SCB_MODE_UART_INC) */


/***************************************
*        Function Prototypes
***************************************/

/* Bootloader communication interface enable */
#define uWIFI_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_uWIFI) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

#if (uWIFI_SCB_MODE_I2C_INC)

    #define uWIFI_I2C_BTLDR_COMM_ENABLED     (uWIFI_BTLDR_COMM_ENABLED && \
                                                            (uWIFI_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             uWIFI_I2C_SLAVE_CONST))

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_I2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void uWIFI_I2CCyBtldrCommStart(void);
    void uWIFI_I2CCyBtldrCommStop (void);
    void uWIFI_I2CCyBtldrCommReset(void);
    cystatus uWIFI_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus uWIFI_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Size of Read/Write buffers for I2C bootloader  */
    #define uWIFI_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
    #define uWIFI_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)
    #define uWIFI_I2C_MIN_UINT16(a, b)           ( ((uint16)(a) < (uint16) (b)) ? \
                                                                    ((uint32) (a)) : ((uint32) (b)) )
    #define uWIFI_WAIT_1_MS                      (1u)
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_I2C_BTLDR_COMM_ENABLED) */

#endif /* (uWIFI_SCB_MODE_I2C_INC) */


#if (uWIFI_SCB_MODE_EZI2C_INC)

    /* Provide EMPTY bootloader communication functions. EZI2C is NOT supported yet */
    #define uWIFI_EZI2C_BTLDR_COMM_ENABLED   (uWIFI_BTLDR_COMM_ENABLED && \
                                                         uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void uWIFI_EzI2CCyBtldrCommStart(void);
    void uWIFI_EzI2CCyBtldrCommStop (void);
    void uWIFI_EzI2CCyBtldrCommReset(void);
    cystatus uWIFI_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus uWIFI_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_EZI2C_BTLDR_COMM_ENABLED) */

#endif /* (uWIFI_SCB_MODE_EZI2C_INC) */

#if (uWIFI_SCB_MODE_SPI_INC || uWIFI_SCB_MODE_UART_INC)
    /* Provide EMPTY bootloader communication functions. SPI and UART is NOT supported yet */
    #define uWIFI_SPI_BTLDR_COMM_ENABLED     (uWIFI_BTLDR_COMM_ENABLED && \
                                                        uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)

    #define uWIFI_UART_BTLDR_COMM_ENABLED    (uWIFI_BTLDR_COMM_ENABLED && \
                                                        uWIFI_SCB_MODE_UNCONFIG_CONST_CFG)

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void uWIFI_SpiCyBtldrCommStart(void);
    void uWIFI_SpiCyBtldrCommStop (void);
    void uWIFI_SpiCyBtldrCommReset(void);
    cystatus uWIFI_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus uWIFI_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void uWIFI_UartCyBtldrCommStart(void);
    void uWIFI_UartCyBtldrCommStop (void);
    void uWIFI_UartCyBtldrCommReset(void);
    cystatus uWIFI_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus uWIFI_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_UART_BTLDR_COMM_ENABLED) */

#endif /* (uWIFI_SCB_MODE_SPI_INC || uWIFI_SCB_MODE_UART_INC) */

#if !defined (uWIFI_I2C_BTLDR_COMM_ENABLED)
    #define uWIFI_I2C_BTLDR_COMM_ENABLED     (0u)
#endif /* (uWIFI_I2C_BTLDR_COMM_ENABLED) */

#if !defined (uWIFI_EZI2C_BTLDR_COMM_ENABLED)
    #define uWIFI_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (uWIFI_EZI2C_BTLDR_COMM_ENABLED) */

#if !defined (uWIFI_SPI_BTLDR_COMM_ENABLED)
    #define uWIFI_SPI_BTLDR_COMM_ENABLED     (0u)
#endif /* (uWIFI_SPI_BTLDR_COMM_ENABLED) */

#if !defined (uWIFI_UART_BTLDR_COMM_ENABLED)
    #define uWIFI_UART_BTLDR_COMM_ENABLED    (0u)
#endif /* (uWIFI_UART_BTLDR_COMM_ENABLED) */

/* Bootloader enabled condition for each mode */
#define uWIFI_BTLDR_COMM_MODE_ENABLED    (uWIFI_I2C_BTLDR_COMM_ENABLED   || \
                                                     uWIFI_EZI2C_BTLDR_COMM_ENABLED || \
                                                     uWIFI_SPI_BTLDR_COMM_ENABLED   || \
                                                     uWIFI_UART_BTLDR_COMM_ENABLED)

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_BTLDR_COMM_ENABLED)
    #if (uWIFI_BTLDR_COMM_MODE_ENABLED)
        /* Bootloader physical layer functions */
        void uWIFI_CyBtldrCommStart(void);
        void uWIFI_CyBtldrCommStop (void);
        void uWIFI_CyBtldrCommReset(void);
        cystatus uWIFI_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus uWIFI_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (uWIFI_BTLDR_COMM_MODE_ENABLED) */

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_uWIFI)
        #define CyBtldrCommStart    uWIFI_CyBtldrCommStart
        #define CyBtldrCommStop     uWIFI_CyBtldrCommStop
        #define CyBtldrCommReset    uWIFI_CyBtldrCommReset
        #define CyBtldrCommWrite    uWIFI_CyBtldrCommWrite
        #define CyBtldrCommRead     uWIFI_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_uWIFI) */
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (uWIFI_BTLDR_COMM_ENABLED) */

#endif /* (CY_SCB_BOOT_uWIFI_H) */

/* [] END OF FILE */
