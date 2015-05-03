/*******************************************************************************
* File Name: ProgrammingSteps.h
* Version 1.0
*
* Description:
*  This header file contains high level definitions and function declarations associated with 
*  programming of the target PSoC 4 device
*
* Note:
*
*******************************************************************************/

#ifndef __PROGRAMMINGSTEPS_H
#define __PROGRAMMINGSTEPS_H

/********************************************************************************
*   Macro definitions
********************************************************************************/

/* Return value definitions for high level Programming functions */
#define FAILURE 0
#define SUCCESS (!FAILURE)

/*Error codes returned by Swd_PacketAck whenever any top-level step returns a failure status*/
#define PORT_ACQUIRE_TIMEOUT_ERROR 	0x10
/* This bit field is set if the SROM does not return the success status code
   within the SROM Polling timeout duration*/
#define SROM_TIMEOUT_ERROR  		0x20
/* This bit field is set in case of JTAG ID mismatch or Flash data verification mismatch
   or Checksum data mismatch */
#define VERIFICATION_ERROR 			0x40
/* This bit field is set if SROM system call request returns error when 
CPUSS_SYSARG register is read */
#define TRANSITION_ERROR			0x80

/* Constants for Address Space of CPU */
#define CPUSS_SYSREQ	            0x40000004
#define CPUSS_SYSARG	            0x40000008
#define TEST_MODE	                0x40030014
#define SRAM_PARAMS_BASE	        0x20000100
#define SFLASH_MACRO	            0x0FFFF000
#define SFLASH_CPUSS_PROTECTION	    0x0FFFF07C

/* SROM Constants */
#define SROM_KEY1	                0xB6
#define SROM_KEY2	                0xD3
#define SROM_SYSREQ_BIT	            0x80000000
#define SROM_PRIVILEGED_BIT	        0x10000000
#define SROM_STATUS_SUCCEEDED	    0xA0000000
#define SROM_STATUS_FAILED	        0xF0000000

/* SROM Commands */
#define SROM_CMD_GET_SILICON_ID	    0x00
#define SROM_CMD_LOAD_LATCH	        0x04
#define SROM_CMD_PROGRAM_ROW	    0x06
#define SROM_CMD_ERASE_ALL	        0x0A
#define SROM_CMD_CHECKSUM	        0x0B
#define SROM_CMD_WRITE_PROTECTION	0x0D

/* Chip Level Protection */
#define CHIP_PROT_VIRGIN	        0x00
#define CHIP_PROT_OPEN	            0x01
#define CHIP_PROT_PROTECTED	        0x02
#define CHIP_PROT_KILL	            0x04

/* The below definitions are not dependent on hex file and are always constant */
#define BYTES_PER_FLASH_ROW             128
#define SILICON_ID_BYTE_LENGTH          4
#define CHECKSUM_BYTE_LENGTH            2
#define MAXIMUM_ROW_PROTECTION_BYTE_LENGTH 32
/********************************************************************************
*   Function Prototypes
********************************************************************************/
unsigned char DeviceAcquire(void);
unsigned char VerifySiliconId(uint8 *SiliconID);
unsigned char EraseAllFlash(void);
unsigned char ChecksumPrivileged(void);
unsigned char ProgramFlashRow(uint8 *HexData);
unsigned char VerifyFlash(uint8 *HexData);
unsigned char ProgramProtectionSettings(uint8 *HexProtectionData);
unsigned char VerifyProtectionSettings(uint8 *HexProtectionData);
unsigned char VerifyChecksum(uint8 *HexCheckSumData);
void ExitProgrammingMode(void);

unsigned char ReadHsspErrorStatus(void);
unsigned char ReadSromStatus(void);

#endif /* __PROGRAMMINGSTEPS_H */

/* [] END OF FILE */
