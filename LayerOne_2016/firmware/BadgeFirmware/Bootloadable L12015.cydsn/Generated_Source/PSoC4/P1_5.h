/*******************************************************************************
* File Name: P1_5.h  
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

#if !defined(CY_PINS_P1_5_H) /* Pins P1_5_H */
#define CY_PINS_P1_5_H

#include "cytypes.h"
#include "cyfitter.h"
#include "P1_5_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    P1_5_Write(uint8 value) ;
void    P1_5_SetDriveMode(uint8 mode) ;
uint8   P1_5_ReadDataReg(void) ;
uint8   P1_5_Read(void) ;
uint8   P1_5_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define P1_5_DRIVE_MODE_BITS        (3)
#define P1_5_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - P1_5_DRIVE_MODE_BITS))

#define P1_5_DM_ALG_HIZ         (0x00u)
#define P1_5_DM_DIG_HIZ         (0x01u)
#define P1_5_DM_RES_UP          (0x02u)
#define P1_5_DM_RES_DWN         (0x03u)
#define P1_5_DM_OD_LO           (0x04u)
#define P1_5_DM_OD_HI           (0x05u)
#define P1_5_DM_STRONG          (0x06u)
#define P1_5_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define P1_5_MASK               P1_5__MASK
#define P1_5_SHIFT              P1_5__SHIFT
#define P1_5_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define P1_5_PS                     (* (reg32 *) P1_5__PS)
/* Port Configuration */
#define P1_5_PC                     (* (reg32 *) P1_5__PC)
/* Data Register */
#define P1_5_DR                     (* (reg32 *) P1_5__DR)
/* Input Buffer Disable Override */
#define P1_5_INP_DIS                (* (reg32 *) P1_5__PC2)


#if defined(P1_5__INTSTAT)  /* Interrupt Registers */

    #define P1_5_INTSTAT                (* (reg32 *) P1_5__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define P1_5_DRIVE_MODE_SHIFT       (0x00u)
#define P1_5_DRIVE_MODE_MASK        (0x07u << P1_5_DRIVE_MODE_SHIFT)


#endif /* End Pins P1_5_H */


/* [] END OF FILE */
