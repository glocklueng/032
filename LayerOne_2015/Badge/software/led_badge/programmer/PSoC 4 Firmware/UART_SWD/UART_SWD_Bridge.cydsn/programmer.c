/*******************************************************************************
* File Name: programmer.c
* Version 1.0
*
* Description:
*  This file provides the source code to respond to the commands sent by the 
*  Python script in host.
*
* Owner:
*	Ranjith M, Application Engineer (tusr@cypress.com)
*
* Related Document:
*	AN84858 - PSoC 4 Programming using an External Microcontroller (HSSP)
*
* Hardware Dependency:
*   CY8CKIT-049
*
* Code Tested With:
*	PSoC Creator 3.0 SP1
*	ARM GCC 4.7.3
*	CY8CKIT-049
*
*******************************************************************************
* Copyright (2014), Cypress Semiconductor Corporation.
*******************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.

******************************************************************************/

/******************************************************************************
*   Header file Inclusion
******************************************************************************/
#include "uart_swd.h"

/******************************************************************************/
unsigned char CurrentStep;
unsigned short rowCount = 0;

uint8 Extractor(uint8* BulkBuf)
{
	switch(BulkBuf[1])
	{
		/* Acquire */
		case 'A': if(DeviceAcquire() == FAILURE)     
        			return(FAILURE);
				break;
		/* Verify Silicon ID */			
		case 'I': if(VerifySiliconId(BulkBuf) == FAILURE)
					return(FAILURE);
				break;
		
		/* Erase All Flash */
		case 'E': if(EraseAllFlash() == FAILURE)
					return(FAILURE);
				break;
					
		/* Checksum priviliged */
		case 'C': if(ChecksumPrivileged() == FAILURE)
					return(FAILURE);
				break;
					
		/* Program Flash */
		case 'P': if(ProgramFlashRow(BulkBuf) == FAILURE)
					return(FAILURE);
				break;
		
		/* Verify Flash */
		case 'V': if(VerifyFlash(BulkBuf) == FAILURE)
					return(FAILURE);
				break;
					
		
		/* Program Protection Settings */			
		case 'R': if(ProgramProtectionSettings(BulkBuf) == FAILURE)
					return(FAILURE);
				break;
					
		/* Verify Protection Settings */			
		case 'T': if(VerifyProtectionSettings(BulkBuf) == FAILURE)
					return(FAILURE);
				break;
					
		/* Verify Checksum */			
		case 'S': if(VerifyChecksum(BulkBuf) == FAILURE)
					return(FAILURE);
				break;
					
		default: return(FAILURE);
	}
	return(SUCCESS);
}


/* [] END OF FILE */
