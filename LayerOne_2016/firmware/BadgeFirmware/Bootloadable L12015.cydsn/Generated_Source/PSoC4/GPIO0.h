/*******************************************************************************
* File Name: GPIO0.h  
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

#if !defined(CY_PINS_GPIO0_H) /* Pins GPIO0_H */
#define CY_PINS_GPIO0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "GPIO0_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    GPIO0_Write(uint8 value) ;
void    GPIO0_SetDriveMode(uint8 mode) ;
uint8   GPIO0_ReadDataReg(void) ;
uint8   GPIO0_Read(void) ;
uint8   GPIO0_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define GPIO0_DRIVE_MODE_BITS        (3)
#define GPIO0_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - GPIO0_DRIVE_MODE_BITS))

#define GPIO0_DM_ALG_HIZ         (0x00u)
#define GPIO0_DM_DIG_HIZ         (0x01u)
#define GPIO0_DM_RES_UP          (0x02u)
#define GPIO0_DM_RES_DWN         (0x03u)
#define GPIO0_DM_OD_LO           (0x04u)
#define GPIO0_DM_OD_HI           (0x05u)
#define GPIO0_DM_STRONG          (0x06u)
#define GPIO0_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define GPIO0_MASK               GPIO0__MASK
#define GPIO0_SHIFT              GPIO0__SHIFT
#define GPIO0_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define GPIO0_PS                     (* (reg32 *) GPIO0__PS)
/* Port Configuration */
#define GPIO0_PC                     (* (reg32 *) GPIO0__PC)
/* Data Register */
#define GPIO0_DR                     (* (reg32 *) GPIO0__DR)
/* Input Buffer Disable Override */
#define GPIO0_INP_DIS                (* (reg32 *) GPIO0__PC2)


#if defined(GPIO0__INTSTAT)  /* Interrupt Registers */

    #define GPIO0_INTSTAT                (* (reg32 *) GPIO0__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define GPIO0_DRIVE_MODE_SHIFT       (0x00u)
#define GPIO0_DRIVE_MODE_MASK        (0x07u << GPIO0_DRIVE_MODE_SHIFT)


#endif /* End Pins GPIO0_H */


/* [] END OF FILE */
