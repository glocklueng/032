/*******************************************************************************
* File Name: CH_PD.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_CH_PD_H) /* Pins CH_PD_H */
#define CY_PINS_CH_PD_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CH_PD_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    CH_PD_Write(uint8 value) ;
void    CH_PD_SetDriveMode(uint8 mode) ;
uint8   CH_PD_ReadDataReg(void) ;
uint8   CH_PD_Read(void) ;
uint8   CH_PD_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define CH_PD_DRIVE_MODE_BITS        (3)
#define CH_PD_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - CH_PD_DRIVE_MODE_BITS))

#define CH_PD_DM_ALG_HIZ         (0x00u)
#define CH_PD_DM_DIG_HIZ         (0x01u)
#define CH_PD_DM_RES_UP          (0x02u)
#define CH_PD_DM_RES_DWN         (0x03u)
#define CH_PD_DM_OD_LO           (0x04u)
#define CH_PD_DM_OD_HI           (0x05u)
#define CH_PD_DM_STRONG          (0x06u)
#define CH_PD_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define CH_PD_MASK               CH_PD__MASK
#define CH_PD_SHIFT              CH_PD__SHIFT
#define CH_PD_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CH_PD_PS                     (* (reg32 *) CH_PD__PS)
/* Port Configuration */
#define CH_PD_PC                     (* (reg32 *) CH_PD__PC)
/* Data Register */
#define CH_PD_DR                     (* (reg32 *) CH_PD__DR)
/* Input Buffer Disable Override */
#define CH_PD_INP_DIS                (* (reg32 *) CH_PD__PC2)


#if defined(CH_PD__INTSTAT)  /* Interrupt Registers */

    #define CH_PD_INTSTAT                (* (reg32 *) CH_PD__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define CH_PD_DRIVE_MODE_SHIFT       (0x00u)
#define CH_PD_DRIVE_MODE_MASK        (0x07u << CH_PD_DRIVE_MODE_SHIFT)


#endif /* End Pins CH_PD_H */


/* [] END OF FILE */
