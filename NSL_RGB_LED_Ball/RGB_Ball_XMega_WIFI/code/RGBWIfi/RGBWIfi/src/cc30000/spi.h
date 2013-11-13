/*****************************************************************************
*
*  spi.h  - CC3000 Host Driver Implementation.
*  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/


#ifndef __SPI_H__
#define __SPI_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>



//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef  __cplusplus
extern "C" {
#endif

typedef void (*gcSpiHandleRx)(void *p);
typedef void (*gcSpiHandleTx)(void);

#define SYSCTL_PERIPH_IRQ_PORT		SYSCTL_PERIPH_GPIOC
#define SPI_GPIO_IRQ_BASE			GPIO_PORTC_BASE	
#define INT_GPIO_SPI				INT_GPIOC	
	
#define SPI_PORT					GPIO_PORTH_BASE
#define SPI_CS_PIN               	GPIO_PIN_1
#define SYSCTL_PERIPH_SPI_PORT		SYSCTL_PERIPH_GPIOH
	
//
// SPI IRQ and EN  PIN defines 
//	
#define SPI_IRQ_PIN					GPIO_PIN_7
#define SPI_EN_PIN					GPIO_PIN_6



extern unsigned char wlan_tx_buffer[];

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void SpiOpen(gcSpiHandleRx pfRxHandler);
extern void SpiClose(void);
extern long SpiWrite(unsigned char *pUserBuffer, unsigned short usLength);
extern void SpiResumeSpi(void);
extern void SpiConfigureHwMapping(void);
extern void SpiCleanGPIOISR(void);
extern void SSIConfigure(unsigned long ulSSIFreq, unsigned long bForceGpioConfiguration, unsigned long uiReconfigureSysClock);
extern int init_spi(void);
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef  __cplusplus
}
#endif // __cplusplus

#endif

