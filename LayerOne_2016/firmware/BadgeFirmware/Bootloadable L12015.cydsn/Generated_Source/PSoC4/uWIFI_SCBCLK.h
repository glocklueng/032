/*******************************************************************************
* File Name: uWIFI_SCBCLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_uWIFI_SCBCLK_H)
#define CY_CLOCK_uWIFI_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void uWIFI_SCBCLK_StartEx(uint32 alignClkDiv);
#define uWIFI_SCBCLK_Start() \
    uWIFI_SCBCLK_StartEx(uWIFI_SCBCLK__PA_DIV_ID)

#else

void uWIFI_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void uWIFI_SCBCLK_Stop(void);

void uWIFI_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 uWIFI_SCBCLK_GetDividerRegister(void);
uint8  uWIFI_SCBCLK_GetFractionalDividerRegister(void);

#define uWIFI_SCBCLK_Enable()                         uWIFI_SCBCLK_Start()
#define uWIFI_SCBCLK_Disable()                        uWIFI_SCBCLK_Stop()
#define uWIFI_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    uWIFI_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define uWIFI_SCBCLK_SetDivider(clkDivider)           uWIFI_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define uWIFI_SCBCLK_SetDividerValue(clkDivider)      uWIFI_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define uWIFI_SCBCLK_DIV_ID     uWIFI_SCBCLK__DIV_ID

#define uWIFI_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define uWIFI_SCBCLK_CTRL_REG   (*(reg32 *)uWIFI_SCBCLK__CTRL_REGISTER)
#define uWIFI_SCBCLK_DIV_REG    (*(reg32 *)uWIFI_SCBCLK__DIV_REGISTER)

#define uWIFI_SCBCLK_CMD_DIV_SHIFT          (0u)
#define uWIFI_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define uWIFI_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define uWIFI_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define uWIFI_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << uWIFI_SCBCLK_CMD_DISABLE_SHIFT))
#define uWIFI_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << uWIFI_SCBCLK_CMD_ENABLE_SHIFT))

#define uWIFI_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define uWIFI_SCBCLK_DIV_FRAC_SHIFT (3u)
#define uWIFI_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define uWIFI_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define uWIFI_SCBCLK_DIV_REG        (*(reg32 *)uWIFI_SCBCLK__REGISTER)
#define uWIFI_SCBCLK_ENABLE_REG     uWIFI_SCBCLK_DIV_REG
#define uWIFI_SCBCLK_DIV_FRAC_MASK  uWIFI_SCBCLK__FRAC_MASK
#define uWIFI_SCBCLK_DIV_FRAC_SHIFT (16u)
#define uWIFI_SCBCLK_DIV_INT_MASK   uWIFI_SCBCLK__DIVIDER_MASK
#define uWIFI_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_uWIFI_SCBCLK_H) */

/* [] END OF FILE */
