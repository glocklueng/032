/*******************************************************************************
* File Name: ESP_RST.h  
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

#if !defined(CY_PINS_ESP_RST_H) /* Pins ESP_RST_H */
#define CY_PINS_ESP_RST_H

#include "cytypes.h"
#include "cyfitter.h"
#include "ESP_RST_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    ESP_RST_Write(uint8 value) ;
void    ESP_RST_SetDriveMode(uint8 mode) ;
uint8   ESP_RST_ReadDataReg(void) ;
uint8   ESP_RST_Read(void) ;
uint8   ESP_RST_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define ESP_RST_DRIVE_MODE_BITS        (3)
#define ESP_RST_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - ESP_RST_DRIVE_MODE_BITS))

#define ESP_RST_DM_ALG_HIZ         (0x00u)
#define ESP_RST_DM_DIG_HIZ         (0x01u)
#define ESP_RST_DM_RES_UP          (0x02u)
#define ESP_RST_DM_RES_DWN         (0x03u)
#define ESP_RST_DM_OD_LO           (0x04u)
#define ESP_RST_DM_OD_HI           (0x05u)
#define ESP_RST_DM_STRONG          (0x06u)
#define ESP_RST_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define ESP_RST_MASK               ESP_RST__MASK
#define ESP_RST_SHIFT              ESP_RST__SHIFT
#define ESP_RST_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ESP_RST_PS                     (* (reg32 *) ESP_RST__PS)
/* Port Configuration */
#define ESP_RST_PC                     (* (reg32 *) ESP_RST__PC)
/* Data Register */
#define ESP_RST_DR                     (* (reg32 *) ESP_RST__DR)
/* Input Buffer Disable Override */
#define ESP_RST_INP_DIS                (* (reg32 *) ESP_RST__PC2)


#if defined(ESP_RST__INTSTAT)  /* Interrupt Registers */

    #define ESP_RST_INTSTAT                (* (reg32 *) ESP_RST__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define ESP_RST_DRIVE_MODE_SHIFT       (0x00u)
#define ESP_RST_DRIVE_MODE_MASK        (0x07u << ESP_RST_DRIVE_MODE_SHIFT)


#endif /* End Pins ESP_RST_H */


/* [] END OF FILE */
