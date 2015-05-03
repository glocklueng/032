/******************************************************************************
* File Name: Swd_UpperPacketLayer.c
* Version 1.0
*
* Description:
*  This file provides the source code for function associated with the upper
*  packet layer of the SWD protocol.
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

/******************************************************************************
* Function Name: Read_DAP
*******************************************************************************
*
* Summary:
*  Reads the Swd_DAP_Register and stores the returned 32 bit value
*
* Parameters:
*  swd_DAP_Register - MACROS for reading Debug Port and Access Port Register
*  data_32          - 32-bit variable containing the returned value
*
* Return:
*  None
*
******************************************************************************/
void Read_DAP( unsigned char Swd_DAP_Register, unsigned long *Data_32)
{
    Swd_PacketHeader = Swd_DAP_Register;
    
    Swd_ReadPacket();
    if(Swd_PacketAck != SWD_OK_ACK)
    {
        return;
    }
    
    *Data_32 = (((unsigned long)Swd_PacketData[3] << 24) | ((unsigned long)Swd_PacketData[2] << 16) | ((unsigned long)Swd_PacketData[1] << 8) | ((unsigned long)Swd_PacketData[0]));
}

/******************************************************************************
* Function Name: Write_DAP
*******************************************************************************
*
* Summary:
*  Writes a 32-bit value to swd_DAP_Register
*
* Parameters:
*  swd_DAP_Register - MACROS for writing to Debug Port and Access Port Register
*  data_32          - 32-bit variable containing the value to be written
*
* Return:
*  None
*
******************************************************************************/
void Write_DAP( unsigned char Swd_DAP_Register, unsigned long Data_32)
{
    Swd_PacketHeader = Swd_DAP_Register;
    
    Swd_PacketData[3] = (unsigned char)(Data_32 >> 24);
    Swd_PacketData[2] = (unsigned char)(Data_32 >> 16);
    Swd_PacketData[1] = (unsigned char)(Data_32 >> 8);
    Swd_PacketData[0] = (unsigned char)(Data_32);
    
    Swd_WritePacket();
}

/******************************************************************************
* Function Name: Read_IO
*******************************************************************************
*
* Summary:
*  Reads 32-bit date from specified address of CPU address space.
*  Returns OK ACK if all SWD transactions succeeds (ACKed).
*
* Parameters:
*  addr_32 - Address from which the data has to be read from CPU address space
*  data_32 - 32-bit variable containing the read value
*
* Return:
*  Swd_PacketAck - Acknowledge packet for the last SWD transaction
*
*******************************************************************************/
unsigned char Read_IO( unsigned long Addr_32, unsigned long *Data_32)
{
    Write_DAP(DPACC_AP_TAR_WRITE, Addr_32);
    if (Swd_PacketAck != SWD_OK_ACK)
    {
        return (Swd_PacketAck);
    }
    
    Read_DAP(DPACC_AP_DRW_READ, Data_32);
    if (Swd_PacketAck != SWD_OK_ACK)
    {
        return (Swd_PacketAck);
    }
        
    Read_DAP(DPACC_AP_DRW_READ, Data_32);
    if (Swd_PacketAck != SWD_OK_ACK)
    {
        return (Swd_PacketAck);
    }
    return (Swd_PacketAck);
}

/******************************************************************************
* Function Name: Write_IO
*******************************************************************************
*
* Summary:
*  Writes 32-bit data into specified address of CPU address space.
*  Returns “true” if all SWD transactions succeeded (ACKed).
*
* Parameters:
*  addr_32 - Address at which the data has to be written in CPU address space
*  data_32 - 32-bit variable containing the value to be written
*
* Return:
*  swd_PacketAck - Acknowledge packet for the last SWD transaction
*
******************************************************************************/
unsigned char Write_IO( unsigned long Addr_32, unsigned long Data_32)
{
    Write_DAP(DPACC_AP_TAR_WRITE, Addr_32);
    if (Swd_PacketAck != SWD_OK_ACK)
    {
        return (Swd_PacketAck);
    }
    
    Write_DAP(DPACC_AP_DRW_WRITE, Data_32);
    if (Swd_PacketAck != SWD_OK_ACK)
    {
        return (Swd_PacketAck);
    }
    
    return (Swd_PacketAck);
}