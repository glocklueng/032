/*******************************************************************************
* File Name: GPIO2.h  
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

#if !defined(CY_PINS_GPIO2_H) /* Pins GPIO2_H */
#define CY_PINS_GPIO2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "GPIO2_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    GPIO2_Write(uint8 value) ;
void    GPIO2_SetDriveMode(uint8 mode) ;
uint8   GPIO2_ReadDataReg(void) ;
uint8   GPIO2_Read(void) ;
uint8   GPIO2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define GPIO2_DRIVE_MODE_BITS        (3)
#define GPIO2_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - GPIO2_DRIVE_MODE_BITS))

#define GPIO2_DM_ALG_HIZ         (0x00u)
#define GPIO2_DM_DIG_HIZ         (0x01u)
#define GPIO2_DM_RES_UP          (0x02u)
#define GPIO2_DM_RES_DWN         (0x03u)
#define GPIO2_DM_OD_LO           (0x04u)
#define GPIO2_DM_OD_HI           (0x05u)
#define GPIO2_DM_STRONG          (0x06u)
#define GPIO2_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define GPIO2_MASK               GPIO2__MASK
#define GPIO2_SHIFT              GPIO2__SHIFT
#define GPIO2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define GPIO2_PS                     (* (reg32 *) GPIO2__PS)
/* Port Configuration */
#define GPIO2_PC                     (* (reg32 *) GPIO2__PC)
/* Data Register */
#define GPIO2_DR                     (* (reg32 *) GPIO2__DR)
/* Input Buffer Disable Override */
#define GPIO2_INP_DIS                (* (reg32 *) GPIO2__PC2)


#if defined(GPIO2__INTSTAT)  /* Interrupt Registers */

    #define GPIO2_INTSTAT                (* (reg32 *) GPIO2__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define GPIO2_DRIVE_MODE_SHIFT       (0x00u)
#define GPIO2_DRIVE_MODE_MASK        (0x07u << GPIO2_DRIVE_MODE_SHIFT)


#endif /* End Pins GPIO2_H */


/* [] END OF FILE */
