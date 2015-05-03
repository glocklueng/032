/*******************************************************************************
* File Name: SWD_PhysicalLayer.h
* Version 1.0
*
* Description:
*  This header file contains the macro definitions, function declarations for the
*  physical layer of the  SWD protocol. These include -
*    a.) Setting a output pin high or low
*    b.) Reading the logic level of an input pin
*    c.) Configuring the drive mode of the programming pin
*
*  The pin manipulation routines are defined both as macros and functions.
*  The macros are used in "Step 1. DeviceAcquire()" as the function has strict
*  timing requirements for execution. Using the macros instead of fuctions reduces
*  execution overhead.
*
*  The pin manipulation functions are used instead of macros in all other
*  places to save code space.
*
* Note:
*  The macros, functions defined here are specific to PSoC 5LP Host Programmer.
*  Modify them as applicable for your Host Programmer.
*******************************************************************************/

#ifndef __SWD_PHYSICALLAYER_H
#define __SWD_PHYSICALLAYER_H

/* Host programmer registers, mask values are defined in "RegisterDefines.h" */

#include "cypins.h"
/********************************************************************************
*   Function Prototypes
********************************************************************************/
/* The below fuctions are for pin manipulation, and their definitions in
*  SWD_PhysicalLayer.c are the same as the corresponding macro defitnions above. */

/* Programming pin drive modes */
#if(CY_PSOC5LP)
	#define SWD_SET_XRES_HI 	CyPins_SetPin(Pin_XRES_0)    
	#define SWD_SET_XRES_LO 	CyPins_ClearPin(Pin_XRES_0)  
	#define SWD_SET_SDA_OUT 	CyPins_SetPinDriveMode(Pin_SWDIO_0, Pin_SWDIO_DM_STRONG)   
	#define SWD_SET_SDA_IN  	CyPins_SetPinDriveMode(Pin_SWDIO_0, Pin_SWDIO_DM_DIG_HIZ)   
	#define SWD_SET_SCK_OUT 	CyPins_SetPinDriveMode(Pin_SWDCK_0, Pin_SWDCK_DM_STRONG)      
	#define SWD_SET_SCK_IN  	CyPins_SetPinDriveMode(Pin_SWDCK_0, Pin_SWDCK_DM_DIG_HIZ)     
	#define SWD_SET_XRES_OUT 	CyPins_SetPinDriveMode(Pin_XRES_0, Pin_XRES_DM_STRONG)     
	#define SWD_SET_XRES_IN 	CyPins_SetPinDriveMode(Pin_XRES_0, Pin_XRES_DM_DIG_HIZ)      
#else
	#define SWD_SET_SDA_HI 		CY_SYS_PINS_SET_PIN(CYREG_PRT2_DR, 0)    
	#define SWD_SET_SDA_LO 		CY_SYS_PINS_CLEAR_PIN(CYREG_PRT2_DR, 0)
	#define	SWD_GET_SDA			CY_SYS_PINS_READ_PIN(CYREG_PRT2_PS, 0)
	#define SWD_SET_SCK_HI 		CY_SYS_PINS_SET_PIN(CYREG_PRT2_DR, 1)    
	#define SWD_SET_SCK_LO 		CY_SYS_PINS_CLEAR_PIN(CYREG_PRT2_DR, 1)
	#define	SWD_GET_SCL			CY_SYS_PINS_READ_PIN(CYREG_PRT2_PS, 1)
	#define SWD_SET_XRES_HI 	CY_SYS_PINS_SET_PIN(CYREG_PRT2_DR, 4)    
	#define SWD_SET_XRES_LO 	CY_SYS_PINS_CLEAR_PIN(CYREG_PRT2_DR, 4)
	#define	SWD_GET_XRES		CY_SYS_PINS_READ_PIN(CYREG_PRT2_PS, 4)
	#define SWD_SET_SDA_OUT 	CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_PC, 0, CY_SYS_PINS_DM_STRONG)   
	#define SWD_SET_SDA_IN  	CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_PC, 0, CY_SYS_PINS_DM_DIG_HIZ)   
	#define SWD_SET_SCK_OUT 	CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_PC, 1, CY_SYS_PINS_DM_STRONG)      
	#define SWD_SET_SCK_IN  	CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_PC, 1, CY_SYS_PINS_DM_DIG_HIZ)     
	#define SWD_SET_XRES_OUT 	CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_PC, 4, CY_SYS_PINS_DM_STRONG)     
	#define SWD_SET_XRES_IN 	CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_PC, 4, CY_SYS_PINS_DM_DIG_HIZ)   
#endif

#if(CY_PSOC5LP)
	/* Bit bandng of the SRAM addresses in order to address each bit of a byte seperately. */
	#define BIT_ADDRESS 		0x20000100u
	#define SWDByte 			*((volatile uint8 *)BIT_ADDRESS)

	/* Convert SRAM address to SRAM bit map region */
	#define BITBAND_SRAM_REF 	CYREG_SRAM_DATA_MBASE  
	#define BITBAND_SRAM_BASE  	CYREG_SRAM_DATA_MBASE + 0x2000000u
	#define BITBAND_SRAM(a,b) 	((BITBAND_SRAM_BASE + (a-BITBAND_SRAM_REF)*32 + (b*4)))  

	#define B0 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,0)))
	#define B1 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,1)))
	#define B2 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,2)))
	#define B3 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,3)))
	#define B4 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,4)))
	#define B5 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,5)))
	#define B6 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,6)))
	#define B7 					*((volatile uint8 *)(BITBAND_SRAM(BIT_ADDRESS,7)))

	/* Bit bandng of the peripheral addresses for flexibility in addressing SWDIO and SWDCK */
	/* Convert Peripheral address to peripheral bit map region */
	#define BITBAND_PERI_REF   	0x40000000u
	#define BITBAND_PERI_BASE  	0x42000000u
	#define BITBAND_PERI(a,b) 	((BITBAND_PERI_BASE +(a-BITBAND_PERI_REF)*32 + (b*4)))  

	#define SWD_BITS 			Pin_SWDCK__DR
	#define SWD_SDA 			*((volatile uint8 *)(BITBAND_PERI(SWD_BITS,0)))
	#define SWD_SCK 			*((volatile uint8 *)(BITBAND_PERI(SWD_BITS,1)))
	#define SDA_PS 				*((volatile uint8 *)(BITBAND_PERI(Pin_SWDIO__PS,Pin_SWDIO_SHIFT)))

	#define SWD_GET_SDA   		SDA_PS
	#define SWD_SET_SCK_LO   	SWD_SCK = 0
	#define SWD_SET_SCK_HI   	SWD_SCK = 1
	#define SWD_SET_SDA_LO   	SWD_SDA = 0
	#define SWD_SET_SDA_HI   	SWD_SDA = 1
	#define SWD_GET_SCL   		SCL_PS
	#define SCL_PS 				*((volatile uint8 *)(BITBAND_PERI(Pin_SWDCK__PS,Pin_SWDCK_SHIFT)))

	#define XRES1_PS 			*((volatile uint8 *)(BITBAND_PERI(Pin_XRES__PS,Pin_XRES_SHIFT)))
	#define SWD_GET_XRES  		XRES1_PS
	#define XRES1_BIT			*((volatile uint8 *)(BITBAND_PERI(Pin_XRES__DR,Pin_XRES_SHIFT)))
#else
	
#endif

#endif /* __SWD_PHYSICALLAYER_H */

/* [] END OF FILE */
