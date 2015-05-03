/*****************************************************************************
* File Name: main.c
*
* Version 1.00
*
* Description:
*  This file provides the source code to implement a UART SWD Bridge
*
* Owner:
*	Ranjith M, Applications Engineer (rnjt@cypress.com)
*
* Related Document:
* 	001-85925 - CY8CKIT-042 PSoC 4 Pioneer Kit IROS
*
* Hardware Dependency:
* 	CY8CKIT-042 (kitprog) 
*
* Code Tested With:
* 	Creator 3.0 SP1
*	GCC1 4.7.3
*	CY8CKIT-042 Rev 2, CY8CKIT-049
*
******************************************************************************
* Copyright (2013), Cypress Semiconductor Corporation.
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
#include "uart_swd.h"

/* Set this to 1 in CY8CKIT-049. Disable to use UDB UART in PSoC 4200 designs. */
#define SCB_UART	1

uint8 BulkBuf[132];
uint8 prev_buf[132];

int main()
{
	uint8 i = 0;
	
	/* Enable global Interrupts */
    CyGlobalIntEnable; 
	
	/* Status LED ON by default */
	Pin_StatusLED_Write(1);
	
	#if(SCB_UART)
		UART_SCB_Start();
	#else
		UART_Rx_Start();
	#endif
	
	for(;;)
    {
		#if(SCB_UART)
			if(UART_SCB_SpiUartGetRxBufferSize() >= 132)
			{
				/* Turn OFF status LED if UART data is received */
				Pin_StatusLED_Write(0);
				for(i = 0; i < 132; i++)
				{
					prev_buf[i] = BulkBuf[i];
					BulkBuf[i] = (uint8)UART_SCB_UartGetByte();
				}
				UART_Rx_ClearRxBuffer();
				if(BulkBuf[0] == 'S')
				{
					if(Extractor(BulkBuf) == FAILURE)
					
					/* Status LED blinking if programming was unsuccessful. 
					 * Unplug and replug the programmer to restart. */
					for(;;)
					{
						Pin_StatusLED_Write(1);
						CyDelay(200);
						Pin_StatusLED_Write(0);
						CyDelay(200);
					}
				}
				if(BulkBuf[0] == 'E')
				{
					ExitProgrammingMode();
				}
				
				/* Turn ON status LED if programming is successful */
				Pin_StatusLED_Write(1);
			}
		#else
			if(UART_Rx_GetRxBufferSize() >= 132)
			{
				/* Turn OFF status LED if UART data is received */
				Pin_StatusLED_Write(0);
				for(i = 0; i < 132; i++)
				{
					prev_buf[i] = BulkBuf[i];
					BulkBuf[i] = (uint8)UART_Rx_GetByte();
				}
				UART_Rx_ClearRxBuffer();
				if(BulkBuf[0] == 'S')
				{
					if(Extractor(BulkBuf) == FAILURE)
					
					/* Status LED blinking if programming was unsuccessful. 
					 * Unplug and replug the programmer to restart. */
					for(;;)
					{
						Pin_StatusLED_Write(1);
						CyDelay(200);
						Pin_StatusLED_Write(0);
						CyDelay(200);
					}
				}
				if(BulkBuf[0] == 'E')
				{
					ExitProgrammingMode();
				}
				
				/* Turn ON status LED if programming is successful */
				Pin_StatusLED_Write(1);
			}
		#endif
	}
}

/* [] END OF FILE */
