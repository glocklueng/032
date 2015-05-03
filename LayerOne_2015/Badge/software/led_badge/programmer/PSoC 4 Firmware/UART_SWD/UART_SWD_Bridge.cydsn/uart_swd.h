/*****************************************************************************
* File Name: uart_swd.h
* Version 1.0
*
* Description:
*  This file contains the function prototypes and constants used in
*  main.c
*
* Note:
*
******************************************************************************
* Copyright (2014), Cypress Semiconductor Corporation.
******************************************************************************
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
*****************************************************************************/
#if !defined(UART_SWD_H) 
#define UART_SWD_H 

#include "device.h"
#include "ProgrammingSteps.h"
#include "SWD_PacketLayer.h"
#include "SWD_PhysicalLayer.h"
#include "SWD_UpperPacketLayer.h"

/*****************************************************************************
* MACRO Definition
*****************************************************************************/
#define FALSE						    	0x00
#define TRUE    							0x01
#define NUMBER_OF_FLASH_ARRAYS_HEX_FILE     1
#define NUMBER_OF_FLASH_ROWS_HEX_FILE       256
#define FLASH_ROW_BYTE_SIZE_HEX_FILE        128
#define FLASH_PROTECTION_BYTE_SIZE_HEX_FILE 32
#define DEVICE_ACQUIRE_TIMEOUT 				15
#define SROM_POLLING_TIMEOUT 				20833

#define ACQUIRE_FAIL						0x01
#define SILICON_ID_FAIL						0x02
#define ERASE_ALL_FAIL						0x03
#define CHECKSUM_PRIV_FAIL					0x04
#define PROGRAM_FAIL						0x05
#define VERIFY_FAIL							0x06
#define PROTECTION_PROG_FAIL				0x07
#define PROTECTION_VERIFY_FAIL				0x08
#define CHECKSUM_VERIFY_FAIL				0x09
#define PROGRAM_OK							0x0A


/*****************************************************************************
* Function Prototypes
*****************************************************************************/
extern uint8 BulkBuf[132];
uint8 SWDAcquirePSoC4(uint8, uint8);
extern void CompleteReset(void);
unsigned char ProgramDevice(void);
uint8 Extractor(uint8* BulkBuf);



#endif /* UART_SWD_H */