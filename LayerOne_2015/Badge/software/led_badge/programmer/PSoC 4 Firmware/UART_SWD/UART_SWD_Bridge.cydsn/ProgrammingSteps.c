/*******************************************************************************
* File Name: ProgrammingSteps.c
* Version 1.0
*
* Description:
*  This file provides the source code for the high level Programming functions used 
*  by the main code to program target PSoC 4
*
* Note:
*  The functions in SWD packet layer use the bit banging macros, functions
*  in "SWD_PhysicalLayer.h"
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

unsigned long Parameter1, Parameter2;
unsigned long Checksum_All, Checksum_Privileged;
unsigned long chip_prot_data = 0x00000000;
unsigned long StatusCode = 0x00000000;

unsigned char Result;
unsigned char ChipProt = 0x00;
unsigned char ChipProtectionData_Hex;

const unsigned char RowsPerArray = 0xFF;

enum Transition_mode {OPEN_XXX, VIRGIN_OPEN, OPEN_VIRGIN, PROT_XXX, WRONG_TRANSITION }flow;

/********************************************************************************
*   Function Definitions
********************************************************************************/

/*******************************************************************************
* Function Name: GetFlashRowCount
********************************************************************************
*
* Summary:
*  Returns the total number of Flash rows in the target PSoC 4 device
*
* Parameters:
*  None
*
* Return:
*  unsigned short
*   Total number of Flash rows in target PSoC 4
*
* Note:
*  The Flash row count is from the HexImage.h file. Modify the definition based
*  on the method of getting programming data
*
*******************************************************************************/
unsigned short GetFlashRowCount()
{
    return(NUMBER_OF_FLASH_ROWS_HEX_FILE);
}

/*******************************************************************************************
* Function Name: unsigned char PollSromStatus(void)
********************************************************************************************
* Summary:
*  Polls the SROM_SYSREQ_BIT and SROM_PRIVILEGED_BIT in the CPUSS_SYSREQ register till it is
*  reset or a timeout condition occurred, whichever is earlier. For a SROM polling timeout
*  error, the timeout error status bit is set in Swd_PacketAck variable and CPUSS_SYSARG
*  register is read to get the error status code. If timeout does not happen, the CPUSS_SYSARG
*  register is read to determine if the task executed successfully.
*
* Parameters:
*  None.
*
* Return:
*  SUCCESS - SROM executed the task successfully
*  FAILURE - SROM task is not executed successfully and a timeout error occured. The failure code is
*            stored in the StatusCode global variable.
*
* Note:
*  This function is called after non volatile memory operations like Read, Write 
*  of Flash, to check if SROM task has been executed which is indicated by success
*  The status is read from the CPUSS_SYSARG register.
*
*******************************************************************************************/
unsigned char PollSromStatus(void)
{
    unsigned long time_elapsed = 0x00000000;
    
    do
    {
	    /* Read CPUSS_SYSREQ register and check if SROM_SYSREQ_BIT and SROM_PRIVILEGED_BIT
		are reset to 0 which */
		Read_IO (CPUSS_SYSREQ, &StatusCode);
		StatusCode &= (SROM_SYSREQ_BIT | SROM_PRIVILEGED_BIT);
	    time_elapsed++;
    }while ((StatusCode != 0) && (time_elapsed <= SROM_POLLING_TIMEOUT));
	
    if (time_elapsed > SROM_POLLING_TIMEOUT )
    {
		Swd_PacketAck = Swd_PacketAck | SROM_TIMEOUT_ERROR;
		Read_IO (CPUSS_SYSARG, &StatusCode);
	    return (FAILURE);
    }
	
	Read_IO (CPUSS_SYSARG, &StatusCode);
	if ((StatusCode & 0xF0000000) != SROM_STATUS_SUCCEEDED)
	{
		Swd_PacketAck = Swd_PacketAck | SROM_TIMEOUT_ERROR;
		return (FAILURE);
    }
	else
	    return (SUCCESS);

}

/*******************************************************************************************
* Function Name: unsigned char ReadSromStatus(void)
********************************************************************************************
*
* Summary:
*  It reads the StatusCode global variable and returns LSB of this long variable to main.c.
*
* Parameters:
*  None.
*
* Return:
*  LSB of StatusCode - LSB of StatusCode global variable contains the error code.
*
* Note:
* This function is called from main.c when SROM_TIMEOUT_ERROR bit is set in the Swd_PacketAck. 
*
*******************************************************************************************/

unsigned char ReadSromStatus(void)
{
	return((unsigned char)StatusCode);
}

/*******************************************************************************************
* Function Name: unsigned char GetChipProtectionVal(void)
********************************************************************************************
*
* Summary:
*  This sub-routine is used to read the Chip Protection Setting by using SROM System Calls.
*  System call to read Silicon Id returns Chip protection settings in the CPUSS_SYSREQ
*  register. The location of the data is bit [15:12] in the 32-bit register.
*
* Parameters:
*  None.
*
* Return:
*  ChipProt - 1 byte chip protection setting read from the chip
*
* Note:
* This function is called in the "Step 3. Erase All Flash" to read the chip protection settings
* to take decision whether to move the protection state to open and then erase the flash or
* directly erase the flash.
*
*******************************************************************************************/
unsigned char GetChipProtectionVal(void)
{
	Parameter1 = (SROM_KEY1 << 0) +	((SROM_KEY2 + SROM_CMD_GET_SILICON_ID) << 8);

    Write_IO (CPUSS_SYSARG, Parameter1);
    if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }

    Write_IO (CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_GET_SILICON_ID);
    if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }
	
    Result = PollSromStatus();
    if( Result != 0x01 )
    {
        return(FAILURE);
    }

    Read_IO( CPUSS_SYSREQ, &chip_prot_data);
    if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }

    ChipProt = (unsigned char)(chip_prot_data >> 12);
	return(SUCCESS);
}

/*******************************************************************************************
* Function Name: unsigned char GetTransitionMode(void)
********************************************************************************************
*
* Summary:
*  It reads the ChipProt global variable which contains the Chip protection setting stored in
*  the Chip and ChipProtectionData_Hex global variable which contains the Chip protection
*  setting stored in the HEX file. The function then validates if the two settings correspond
*  to a valid transition.
*
* Parameters:
*  None.
*
* Return:
*  SUCCESS - Returns SUCCESS if the transition is valid.
*  FAILURE - Returns Failure if the transition is invalid.
*  Stores the transition in the global enum flow.
*
* Note:
* This function is called in "Step 3. Erase All Flash" to take decision on basis of the global
* enum flow.
*
*******************************************************************************************/
unsigned char GetTransitionMode(void)
{
	flow = WRONG_TRANSITION;
	switch (ChipProt)
	{
		case CHIP_PROT_VIRGIN:
            if (ChipProtectionData_Hex == CHIP_PROT_OPEN)
                flow = VIRGIN_OPEN;
            else
                flow = WRONG_TRANSITION;
            break;
		
		case CHIP_PROT_OPEN:
            if (ChipProtectionData_Hex == CHIP_PROT_VIRGIN)
                flow = OPEN_VIRGIN;
            else
                flow = OPEN_XXX;
            break;
        
		case CHIP_PROT_PROTECTED:
            if ((ChipProtectionData_Hex == CHIP_PROT_OPEN) || (ChipProtectionData_Hex == CHIP_PROT_PROTECTED))
                flow = PROT_XXX;
            else
                flow = WRONG_TRANSITION;
            break;
        
		default:
            flow = WRONG_TRANSITION;
            break;
	}
	if ((flow == WRONG_TRANSITION)||(flow == OPEN_VIRGIN))
	{
		Swd_PacketAck = Swd_PacketAck | TRANSITION_ERROR;
		return(FAILURE);
	}
	return(SUCCESS);
}

/*******************************************************************************************
* Function Name: unsigned char LoadLatch(unsigned char arrayID, unsigned char * RowData)
********************************************************************************************
*
* Summary:
*  This function loads the page latch buffer with data to be programmed in to a row of flash.
*  Data is loaded into the page latch buffer starting at the location specified by the
*  SRAM_PARAMS_BASE input parameter. Data loaded in to the page latch buffer will remain
*  until a program operation is performed, which clears the page latch contents.
*
* Parameters:
*  ArrayID - Array Number of the flash
*  RowData - Array containing 128 bytes of programming data
*
* Return:
*  SUCCESS - Returns SUCCESS if Data is successfully latched
*  FAILURE - Returns Failure if Data is not latched successfully
*
* Note:
* This function is called in "Step 5. Program Flash" and "Step 7. Program Protection Settings"
* to latch the programming data in SRAM.
*
*******************************************************************************************/
unsigned char LoadLatch(unsigned char arrayID, unsigned char * RowData)
{
		unsigned char i;
		
		Parameter1 = (SROM_KEY1 << 0) + ((SROM_KEY2 + SROM_CMD_LOAD_LATCH) << 8) +	(0x00 << 16) + (arrayID << 24);
		Parameter2 = (BYTES_PER_FLASH_ROW - 1); //Number of Bytes to load minus 1

	    Write_IO(SRAM_PARAMS_BASE + 0x00, Parameter1);  //Write params in SRAM
		if( Swd_PacketAck != SWD_OK_ACK )
	    {
	        return (FAILURE);
	    }
	    
		Write_IO(SRAM_PARAMS_BASE + 0x04, Parameter2); //Write params in SRAM
		if( Swd_PacketAck != SWD_OK_ACK )
	    {
	        return (FAILURE);
	    }
		
		//  Put row data into SRAM buffer
	    for (i = 0; i < BYTES_PER_FLASH_ROW; i += 4)
	    {
	     	Parameter1 = (RowData[i] << 0) + (RowData[i + 1] << 8) + (RowData[i + 2] << 16) + (RowData[i + 3] << 24);
	        
			Write_IO(SRAM_PARAMS_BASE + 0x08 + i, Parameter1);//Write params in SRAM
			if( Swd_PacketAck != SWD_OK_ACK )
    	    {
    	        return (FAILURE);
    	    }
		}

	    //  Call "Load Latch" SROM API
	    Write_IO(CPUSS_SYSARG, SRAM_PARAMS_BASE); //Set location of parameters
		if( Swd_PacketAck != SWD_OK_ACK )
	    {
	        return (FAILURE);
	    }
	    
		Write_IO(CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_LOAD_LATCH);//Request SROM operation
		if( Swd_PacketAck != SWD_OK_ACK )
	    {
	        return (FAILURE);
	    }
	    
		Result = PollSromStatus();
	    if ( Result != 0x01 )
        {
            return (FAILURE);
        }
	
	return (SUCCESS);
}

/*******************************************************************************************
* Function Name: unsigned char ChecksumAPI(unsigned short ChecksumRow, unsigned long *Checksum)
********************************************************************************************
*
* Summary:
*  This function reads either the whole flash memory or a row of flash. When performing a
*  checksum on the whole flash, the user code and the supervisory flash regions are included.
*  When performing a checksum only on one row of flash, the flash row number is passed as a
*  parameter. For computing Checksum of entire flash, ChecksumRow input parameter is loaded as
*  0x8000.
*
* Parameters:
*  ChecksumRow: Row number of flash for which checksum has to be calculated. To compute
*  checksum of entire flash, this variable is set to 0x8000.
*
* Return:
*  SUCCESS - Returns SUCCESS if Checksum System call is successfully executed.
*  FAILURE - Returns Failure if Checksum system call fails to execute.
*
* Note:
* This function is called in "Step 4. Checksum Privileged Calculation" and "Step 9. Verify
* Checksum" to calculate the checksum of flash privileged rows and entire flash, respectively.
*
*******************************************************************************************/
unsigned char ChecksumAPI(unsigned short ChecksumRow, unsigned long *Checksum)
{
	unsigned long Checksum_chip;
	
	Parameter1 = (SROM_KEY1 << 00) + ((SROM_KEY2 +SROM_CMD_CHECKSUM) << 8) + ((ChecksumRow & 0x00FF) << 16) + ((ChecksumRow & 0xFF00) << 16);
	
    Write_IO (CPUSS_SYSARG, Parameter1);
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }

	Write_IO (CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_CHECKSUM);
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }
    
	Result = PollSromStatus();
	if( Result != 0x01 )
    {
        return(FAILURE);
    }

	Read_IO( CPUSS_SYSARG, &Checksum_chip);
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }

	*Checksum = (Checksum_chip & 0x0FFFFFFF);

	return (SUCCESS);
}


/*******************************************************************************************
*Function Name: unsigned char DeviceAcquire(void)
********************************************************************************************
*
* Summary:
*  This is Step 1 of the programming sequence. In this Step, target PSoC 4 is acquired by the
*  host microcontroller by sending specific Port Acquiring Sequence in a 1.5 ms time-window.
*  After acquiring SWD port, debug port is configured and bit 31 in TEST_MODE control
*  register is set.
*
* Parameters:
*  None
*
* Return:
*  SUCCESS - Returns SUCCESS if the device is successfully acquired.
*  FAILURE - Returns Failure if the function fails in any of the intermediate step.
*
* Note:
* This function has very strict timing requirements. The device must be acquired as per the 
* timing requirements given in PSoC 4 Device Programming Specification Document.
*
*******************************************************************************************/
unsigned char DeviceAcquire(void)
{
    unsigned long DAP_Id = 0x0BB11477;
	unsigned long Chip_DAP_Id;
    unsigned short total_packet_count;
    unsigned long status;
    
    total_packet_count = 0;
    
	SWD_SET_XRES_OUT;
	SWD_SET_XRES_HI;

	SWD_SET_SCK_OUT;
	SWD_SET_SCK_LO;

	SWD_SET_SDA_OUT;
	SWD_SET_SDA_LO;

	SWD_SET_XRES_LO;
	CyDelayUs(100);
	SWD_SET_XRES_HI;
	
	//Pin_Test_Write(0);
    do
    {
    	Swd_LineReset();
		Read_DAP(DPACC_DP_IDOCDE_READ, &Chip_DAP_Id);
    	total_packet_count++;
    }while((Swd_PacketAck != SWD_OK_ACK)&& (total_packet_count < DEVICE_ACQUIRE_TIMEOUT));
	
	//Pin_Test_Write(1);//To get the time to enter test mode
	
	if (total_packet_count == DEVICE_ACQUIRE_TIMEOUT)
	{
		Swd_PacketAck = Swd_PacketAck | PORT_ACQUIRE_TIMEOUT_ERROR;
        return(FAILURE);
	}

	if (Chip_DAP_Id != DAP_Id)
	{
		Swd_PacketAck = Swd_PacketAck | VERIFICATION_ERROR;
		return(FAILURE);
	}
	
	
	Write_DAP (DPACC_DP_CTRLSTAT_WRITE, 0x54000000);
    if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }
	
	Write_DAP (DPACC_DP_SELECT_WRITE, 0x00000000);
    if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }
	
	Write_DAP (DPACC_AP_CSW_WRITE, 0x00000002);
    if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }

    Write_IO (TEST_MODE, 0x80000000);
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }
    
    Read_IO (TEST_MODE, &status);
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }
	
    if((status & 0x80000000) != 0x80000000)
    {
        return (FAILURE);
    }
	
	//Pin_Test_Write(1);
	
	Result = PollSromStatus();
	
	//Pin_Test_Write(0);
	if (Result != 0x01)
	{
		return(FAILURE);		
	}
	
    return (SUCCESS);
}

/*******************************************************************************************
* Function Name: unsigned char VerifySiliconId(void)
********************************************************************************************
*
* Summary:
*  This is Step 2 of the programming sequence. In this Step, Silicon Id of the PSoC 4 device
*  is read and matched with the silicon id stored in the Hex File to verify that the correct
*  device is being programmed.
*
* Parameters:
*  None
*
* Return:
*  SUCCESS - Returns SUCCESS if Silicon Id read from chip matches the Id in the HEX File.
*  FAILURE - Returns Failure if any of the intermediate step returns a fail message.
*
* Note:
* 
*
*******************************************************************************************/
unsigned char VerifySiliconId(uint8 *SiliconID)
{
	unsigned char i;
	unsigned char DeviceSiliconID[SILICON_ID_BYTE_LENGTH];
	unsigned char HexSiliconID[SILICON_ID_BYTE_LENGTH];
    unsigned long SiliconIdData1 = 0x00000000;
	unsigned long SiliconIdData2 = 0x00000000;

	HexSiliconID[0] = SiliconID[2];
	HexSiliconID[1] = SiliconID[3];
	HexSiliconID[2] = SiliconID[4];
	HexSiliconID[3] = SiliconID[5];
	
	Parameter1 = (unsigned long)(((unsigned long)SROM_KEY1 << 0) + (((unsigned long)SROM_KEY2 + (unsigned long)SROM_CMD_GET_SILICON_ID) << 8));
	
	Write_IO (CPUSS_SYSARG, Parameter1);
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }

	Write_IO (CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_GET_SILICON_ID);
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }
	
    Result = PollSromStatus();
	if( Result != 0x01 )
    {
        return(FAILURE);
    }

	Read_IO(CPUSS_SYSARG, &SiliconIdData1);
    if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }
	
	Read_IO(CPUSS_SYSREQ, &SiliconIdData2);
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return(FAILURE);
    }
    
	DeviceSiliconID[0] = ((unsigned char)(SiliconIdData1 >> 8))  & 0xFF;
	DeviceSiliconID[1] = ((unsigned char)(SiliconIdData1 >> 0))  & 0xFF;
	DeviceSiliconID[2] = ((unsigned char)(SiliconIdData1 >> 16)) & 0xFF;
	DeviceSiliconID[3] = ((unsigned char)(SiliconIdData2 >> 0))  & 0xFF;

	for (i=0; i<SILICON_ID_BYTE_LENGTH; i++)
	{
		if (DeviceSiliconID[i] != HexSiliconID[i])
        {
            Swd_PacketAck = Swd_PacketAck | VERIFICATION_ERROR;
			return (FAILURE);
        }
	}
	return (SUCCESS);
}

/*******************************************************************************
* Function Name: HEX_ReadChipProtectionData
********************************************************************************
*
* Summary:
*  Loads the Chip Protection data in to an array 
*
* Parameters:
*
*  unsigned char * ChipProtectionData - Address of variable which will store the Chip Protection data fetched
*
* Return:
*  None.
*
* Note:
*  Modify definition based on the method of getting programming data
*
*******************************************************************************/
void HEX_ReadChipProtectionData(unsigned char * ChipProtectionData)
{
    *ChipProtectionData = 0x01;
}

/*******************************************************************************************
* Function Name: unsigned char EraseAllFlash(void)
********************************************************************************************
*
* Summary:
*  This is Step 3 of the programming sequence. In this Step, the whole user flash is erased.
*  This function uses GetChipProtectionVal() and GetTransitionMode() API's to take the decision
*  on the method to follow to erase the device.
*
* Parameters:
*  None
*
* Return:
*  SUCCESS - Returns SUCCESS if function successfully erases complete user flash region.
*  FAILURE - Returns Failure if any of the intermediate step returns a fail message.
*
* Note:
* 
*******************************************************************************************/
unsigned char EraseAllFlash(void)
{
	GetChipProtectionVal();
	
	HEX_ReadChipProtectionData(&ChipProtectionData_Hex);
	
	Result = GetTransitionMode();
	if (Result != SUCCESS)
	{
		return(FAILURE);
	}
	
	if ((flow == OPEN_XXX) || (flow == VIRGIN_OPEN))
    {
    	Parameter1 = 0x00000000;
		Parameter1 = ((SROM_KEY1 << 0) + ((SROM_KEY2 + SROM_CMD_ERASE_ALL) << 8));

		
    	Write_IO (SRAM_PARAMS_BASE + 0x00, Parameter1);
    	if( Swd_PacketAck != SWD_OK_ACK )
        {
            return(FAILURE);
        }

    	Write_IO (CPUSS_SYSARG, SRAM_PARAMS_BASE);
    	if( Swd_PacketAck != SWD_OK_ACK )
        {
            return(FAILURE);
        }

    	Write_IO (CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_ERASE_ALL);
    	if( Swd_PacketAck != SWD_OK_ACK )
        {
            return(FAILURE);
        }

    	Result = PollSromStatus();
    	if (Result != SUCCESS)
		{
    		return (FAILURE);
		}
    }

    else if (flow == PROT_XXX)
    {
    	Parameter1 = (SROM_KEY1 << 0) + ((SROM_KEY2 + SROM_CMD_WRITE_PROTECTION) << 8) + (0x01 << 16) + (0x00 << 24); //0x01 corresponds to open state, 0x00 to macro 1

    	Write_IO (CPUSS_SYSARG, Parameter1);
    	if( Swd_PacketAck != SWD_OK_ACK )
        {
            return(FAILURE);
        }
    	
        Write_IO (CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_WRITE_PROTECTION);
    	if( Swd_PacketAck != SWD_OK_ACK )
        {
            return(FAILURE);
        }
    	
        Result = PollSromStatus();
    	if( Result != SUCCESS )
        {
            return(FAILURE);
        }

    	Result = DeviceAcquire();
    	if( Result != SUCCESS )
        {
            return(FAILURE);
        }
    }
    
    return (SUCCESS);
}

/*******************************************************************************************
* Function Name: unsigned char ChecksumPrivileged(void)
********************************************************************************************
*
* Summary:
*  This is Step 4 of the programming sequence. In this Step, checksum of the privileged rows
*  is calulated using system call to determine checksum. This step uses ChecksumAPI() API to
*  store the checksum of privileged rows in a Checksum_Privileged global variable. This
*  variable is used in step 9 to calculate the checksum of user rows.
*
* Parameters:
*  None
*
* Return:
*  SUCCESS - Returns SUCCESS if function successfully calculated the checksum of privileged
*            rows.
*  FAILURE - Returns Failure if any of the intermediate step returns a fail message.
*
* Note:
*
*******************************************************************************************/
unsigned char ChecksumPrivileged()
{        
    Result = ChecksumAPI(0x8000, &Checksum_Privileged);
	if (Result != SUCCESS)
	{
		return (FAILURE);
	}
	return(SUCCESS);
}

/*******************************************************************************************
* Function Name: unsigned char ProgramFlash(uint8 *FlashData)
********************************************************************************************
*
* Summary:
*  This is Step 5 of the programming sequence. In this Step, the whole user flash is re-
*  programmed with the programming data in the HEX File. This function uses LoadLatch() API
*  to latch the row data in SRAM page latch buffer which is then programmed to the specific
*  row using system calls to program row.
*
* Parameters:
*  None
*
* Return:
*  SUCCESS - Returns SUCCESS if function successfully programs entire flash region.
*  FAILURE - Returns Failure if any of the intermediate step returns a fail message.
*
* Note:
* 
*******************************************************************************************/
unsigned char ProgramFlashRow(uint8 *HexData)
{
	unsigned char RowData[BYTES_PER_FLASH_ROW];
	unsigned char arrayID;
	unsigned char RowCount;
	uint8 index = 0;
	        
	RowCount = HexData[2];
	
	for(index = 0; index < BYTES_PER_FLASH_ROW; index++)
	{
		RowData[index] = HexData[index + 3];
	}
	
		
	arrayID = RowCount/RowsPerArray;
	
	Result = LoadLatch(arrayID, &RowData[0]);
	if(Result != SUCCESS)
	{
		return(FAILURE);
	}

       //3. Program Row - call SROM API
    Parameter1 =(SROM_KEY1 << 0) + ((SROM_KEY2+SROM_CMD_PROGRAM_ROW) << 8) + ((RowCount & 0x00FF) <<  16) +	((RowCount & 0xFF00) << 16);			    //ROW_ID_HIGH[9:8]
    // some error up dere
    
    Write_IO(SRAM_PARAMS_BASE+0x00, Parameter1);     //Write params is SRAM
   	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return (FAILURE);
    }
    
	Write_IO(CPUSS_SYSARG, SRAM_PARAMS_BASE);    //Set location of parameters
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return (FAILURE);
    }
    
	Write_IO(CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_PROGRAM_ROW);//Request SROM operation
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return (FAILURE);
    }
    
    Result = PollSromStatus();
    if ( Result != 0x01 )
    {
        return (FAILURE);
    }

	return ( SUCCESS );
}

/*******************************************************************************************
* Function Name: unsigned char VerifyFlash(uint8 *HexData)
********************************************************************************************
*
* Summary:
*  This is Step 6 of the programming sequence. This is an optional step as we verify the
*  checksum explicitly. In this Step, flash region is directly read using Read_IO API defined in
*  SWD_UpperPacketLayer.h and compared with the HEX File.
*
* Parameters:
*  None
*
* Return:
*  SUCCESS - Returns SUCCESS if function successfully verifies the entire flash with the
*		     HEX File.
*  FAILURE - Returns Failure if any of the intermediate step returns a fail message.
*
* Note:
* 
*******************************************************************************************/
unsigned char VerifyFlash(uint8 *HexData)
{
	unsigned char RowData[BYTES_PER_FLASH_ROW];
    unsigned short RowAddress;
    unsigned long FlashData;
    unsigned char chipData[BYTES_PER_FLASH_ROW];
    unsigned short RowCount;
    unsigned char i;
	uint8 index = 0;
    
	RowCount = HexData[2];
	//1. Read row from hex file
	RowAddress = BYTES_PER_FLASH_ROW * RowCount; //liner address of row in flash
	
	for(index = 0; index < BYTES_PER_FLASH_ROW; index++)
	{
		RowData[index] = HexData[index + 3];
	}

	//2. Read row from chip
	for (i = 0; i < BYTES_PER_FLASH_ROW; i += 4)
	{
		//Read flash via AHB-interface
   		Read_IO( RowAddress + i, &FlashData);
        if( Swd_PacketAck != SWD_OK_ACK )
	    {
	        return (FAILURE);
	    }
        
        chipData[i + 0] = (FlashData >> 0) & 0xFF;
        chipData[i + 1] = (FlashData >> 8) & 0xFF;
        chipData[i + 2] = (FlashData >> 16) & 0xFF;
        chipData[i + 3] = (FlashData >> 24) & 0xFF;
	}

	//3. Compare them
	for (i = 0; i < BYTES_PER_FLASH_ROW; i++)
	{
   		if (chipData[i] != RowData[i])
		{
			Swd_PacketAck = Swd_PacketAck | VERIFICATION_ERROR;
			return ( FAILURE );
		}
	}

	return ( SUCCESS );
}

/*******************************************************************************************
* Function Name: unsigned char ProgramProtectionSettings(uint8 *HexProtectionData)
********************************************************************************************
*
* Summary:
*  This is Step 7 of the programming sequence. In this step, Chip protection settings and Row
*  Protection settings are read from the HEX file and programmed to the specific loctions in
*  the flash.
*
* Parameters:
*  None
*
* Return:
*  SUCCESS - Returns SUCCESS if function successfully writes the protection settings.
*  FAILURE - Returns Failure if any of the intermediate step returns a fail message.
*
* Note:
*
*******************************************************************************************/
unsigned char ProgramProtectionSettings(uint8 *HexProtectionData)
{
	unsigned short NumOfFlashRows;
    unsigned char RowProtectionData[MAXIMUM_ROW_PROTECTION_BYTE_LENGTH];
    unsigned char RowProtectionByteSize;
    unsigned char arrayID;
	uint8 index = 0; 
	
    NumOfFlashRows   = GetFlashRowCount();
    RowProtectionByteSize = NumOfFlashRows / 8;
	
	arrayID = NumOfFlashRows/RowsPerArray;
	
	HEX_ReadChipProtectionData(&ChipProtectionData_Hex);
	
	for(index = 0; index < RowProtectionByteSize; index++)
	{
		RowProtectionData[index] = HexProtectionData[index + 2];
	}

	//3. Load protection setting of current macro into volatile latch.
	Result = LoadLatch(arrayID, &RowProtectionData[0]);
	if (Result != SUCCESS)
	{
		return(FAILURE);
	}

	//4. Program protection setting into supervisory row.
	Parameter1 = (SROM_KEY1 << 0) +	((SROM_KEY2 + SROM_CMD_WRITE_PROTECTION) << 8) + (ChipProtectionData_Hex << 16);   //Applicable only for Macro

	Write_IO(CPUSS_SYSARG, Parameter1);	                                            
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return (FAILURE);
    }
    Write_IO(CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_WRITE_PROTECTION);
	if( Swd_PacketAck != SWD_OK_ACK )
    {
        return (FAILURE);
    }

	//Read status of the operation
	Result = PollSromStatus();
	if( Result != 0x01 )
    {
        return (FAILURE);
    }

	return (SUCCESS);
}

/*******************************************************************************************
* Function Name: unsigned char VerifyProtectionSettings(uint8 *HexProtectionData)
********************************************************************************************
*
* Summary:
*  This is Step 8 of the programming sequence. In this step, Chip protection settings and Row
*  Protection settings are read from the HEX file and verified with the protection settings
*  programmed in flash
*
* Parameters:
*  None
*
* Return:
*  SUCCESS - Returns SUCCESS if function successfully verifies the protection settings.
*  FAILURE - Returns Failure if any of the intermediate step returns a fail message.
*
* Note:
*
*******************************************************************************************/
unsigned char VerifyProtectionSettings(uint8 *HexProtectionData)
{
	unsigned short NumOfFlashRows;
    unsigned char ChipProtectionData_Hex, Chip_ChipProtectionData;
    unsigned char RowProtectionData[MAXIMUM_ROW_PROTECTION_BYTE_LENGTH];
    unsigned char RowProtectionFlashData[MAXIMUM_ROW_PROTECTION_BYTE_LENGTH];
    unsigned char RowProtectionByteSize, i, index = 0;
    unsigned long ProtectionData, FlashProtectionAddress;    
    
    NumOfFlashRows = GetFlashRowCount();
    RowProtectionByteSize = NumOfFlashRows/8;   

	FlashProtectionAddress = SFLASH_MACRO; //0x0FFF F000

	//1. Read Protection settings from hex-file.
	for(index = 0; index < RowProtectionByteSize; index++)
	{
		RowProtectionData[index] = HexProtectionData[index + 2];
	}
	
	//2. Read Protection from silicon
	for (i = 0; i < RowProtectionByteSize; i += 4)
	{
		Read_IO(FlashProtectionAddress + i, &ProtectionData);
	    if( Swd_PacketAck != SWD_OK_ACK )
	    {
	        return (FAILURE);
	    }
		
		RowProtectionFlashData[i + 0] = (ProtectionData >> 0) & 0xFF;     
		RowProtectionFlashData[i + 1] = (ProtectionData >> 8) & 0xFF;
		RowProtectionFlashData[i + 2] = (ProtectionData >> 16) & 0xFF;
	    RowProtectionFlashData[i + 3] = (ProtectionData >> 24) & 0xFF;
	}

	//3. Compare hex and silicon’s data
	for (i = 0; i < RowProtectionByteSize; i++ )
	{
		if (RowProtectionData[i] != RowProtectionFlashData[i])
		{
            /* Set the verification error bit for Flash protection data mismatch and return failure */
            Swd_PacketAck = Swd_PacketAck | VERIFICATION_ERROR; 
            return(FAILURE);
        }
	}

	//4. Read Chip Level Protection from hex-file. It is 1 byte at address 0x90600000.
	HEX_ReadChipProtectionData(&ChipProtectionData_Hex);

	//5. Read Chip Level Protection from the silicon 
	Read_IO(SFLASH_CPUSS_PROTECTION, &ProtectionData);
	if( Swd_PacketAck != SWD_OK_ACK )
	    {
	        return (FAILURE);
	    }
		
	Chip_ChipProtectionData = (ProtectionData >> 24) & 0x0F;

	if (Chip_ChipProtectionData == CHIP_PROT_VIRGIN)
		Chip_ChipProtectionData = CHIP_PROT_OPEN;
		
	else if (Chip_ChipProtectionData == CHIP_PROT_OPEN)
		Chip_ChipProtectionData = CHIP_PROT_VIRGIN;

	//6. Compare hex’s and silicon’s data 
	if (Chip_ChipProtectionData != ChipProtectionData_Hex)
	{
        /* Set the verification error bit for Flash protection data mismatch and return failure */
        Swd_PacketAck = Swd_PacketAck | VERIFICATION_ERROR; 
        return(FAILURE);
    }

	return (SUCCESS);
}

/*******************************************************************************************
* Function Name: unsigned char VerifyChecksum(void)
********************************************************************************************
*
* Summary:
*  This is Step 9 of the programming sequence. In this step, Checksum of user data in flash 
*  is verified with the Checksum stored in the HEX File. This step uses the Checksum
*  of privileged rows calculated in Step 4 get the checksum of user data in flash.
*
* Parameters:
*  None
*
* Return:
*  SUCCESS - Returns SUCCESS if function successfully verifies the checksum.
*  FAILURE - Returns Failure if any of the intermediate step returns a fail message.
*
* Note:
*
*******************************************************************************************/
unsigned char VerifyChecksum(uint8 *HexCheckSumData)
{
	unsigned short Chip_Checksum = 0x0000, ChecksumData = 0x0000;
    
	Result = ChecksumAPI(0x8000, &Checksum_All);
	if (Result != SUCCESS)
	{
		return (FAILURE);
	}

    Chip_Checksum = (unsigned short)Checksum_All - ((unsigned short)Checksum_Privileged);

	ChecksumData = ((HexCheckSumData[2] << 8) | HexCheckSumData[3]);
	
	if (Chip_Checksum != ChecksumData)
    {
        Swd_PacketAck = Swd_PacketAck | VERIFICATION_ERROR;
		return (FAILURE);
    }
	
	return (SUCCESS);
}

/*******************************************************************************
* Function Name: ReadHsspErrorStatus
********************************************************************************
*
* Summary:
*  Returns the HSSP Error status in case of FAILURE return in any one of the
*  programming steps.
*
* Parameters:
*  None.
*
* Return:
*  Swd_PacketAck - Each bit of this 8-bit return value has a specific meaning.
*
* Note:
*  Refer to the application note pdf for details on the Error status bit definitions
*******************************************************************************/
unsigned char ReadHsspErrorStatus()
{
    return(Swd_PacketAck);
}

/*******************************************************************************
* Function Name: ExitProgrammingMode
********************************************************************************
*
* Summary:
*  Releases the target PSoC 4 device from Programming mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Note:
*
*******************************************************************************/
void ExitProgrammingMode()
{
    /* Drive the SWDIO, SWDCK outputs low */
    SWD_SET_SCK_LO;
    SWD_SET_SDA_LO;
    
    /* Make SWDIO, SWDCK High-Z after completing Programming */    
    SWD_SET_SDA_IN;
    SWD_SET_SCK_IN;
    
    /* Generate active low rest pulse for 100 uS */
    SWD_SET_XRES_LO;
    CyDelayUs(100);    
    SWD_SET_XRES_HI;

    /* Make XRES High-Z after generating the reset pulse */  
    SWD_SET_XRES_IN;
}


/* [] END OF FILE */