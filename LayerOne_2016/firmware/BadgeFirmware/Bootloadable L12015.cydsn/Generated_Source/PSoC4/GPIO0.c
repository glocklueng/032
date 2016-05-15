/*******************************************************************************
* File Name: GPIO0.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "GPIO0.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        GPIO0_PC =   (GPIO0_PC & \
                                (uint32)(~(uint32)(GPIO0_DRIVE_MODE_IND_MASK << (GPIO0_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (GPIO0_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: GPIO0_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void GPIO0_Write(uint8 value) 
{
    uint8 drVal = (uint8)(GPIO0_DR & (uint8)(~GPIO0_MASK));
    drVal = (drVal | ((uint8)(value << GPIO0_SHIFT) & GPIO0_MASK));
    GPIO0_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: GPIO0_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  GPIO0_DM_STRONG     Strong Drive 
*  GPIO0_DM_OD_HI      Open Drain, Drives High 
*  GPIO0_DM_OD_LO      Open Drain, Drives Low 
*  GPIO0_DM_RES_UP     Resistive Pull Up 
*  GPIO0_DM_RES_DWN    Resistive Pull Down 
*  GPIO0_DM_RES_UPDWN  Resistive Pull Up/Down 
*  GPIO0_DM_DIG_HIZ    High Impedance Digital 
*  GPIO0_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void GPIO0_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(GPIO0__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: GPIO0_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro GPIO0_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 GPIO0_Read(void) 
{
    return (uint8)((GPIO0_PS & GPIO0_MASK) >> GPIO0_SHIFT);
}


/*******************************************************************************
* Function Name: GPIO0_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 GPIO0_ReadDataReg(void) 
{
    return (uint8)((GPIO0_DR & GPIO0_MASK) >> GPIO0_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(GPIO0_INTSTAT) 

    /*******************************************************************************
    * Function Name: GPIO0_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 GPIO0_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(GPIO0_INTSTAT & GPIO0_MASK);
		GPIO0_INTSTAT = maskedStatus;
        return maskedStatus >> GPIO0_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
