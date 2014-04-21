//-----------------------------------------------------------------------------
// Merlok - June 2011, 2012
// Gerhard de Koning Gans - May 2008
// Hagen Fritsch - June 2010
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// Routines to support ISO 14443 type A.
//-----------------------------------------------------------------------------


#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

// STM32 Library
#include "stm32f10x_lib.h"
#include "usb_lib.h"

// defines for hardware board
#include "l1_board.h"
#include "hw_config.h"
#include "proxmark3.h"
#include "apps.h"

#include "logos.h"
#include "printf.h"

#include "iso14443crc.h"
#include "iso14443a.h"
#include "crapto1.h"
#include "mifareutil.h"



uint32_t RAMFUNC GetCountSspClk()
{
  
  return 0;
  
#if 0
  
	uint32_t tmp_count;
	tmp_count = (AT91C_BASE_TC2->TC_CV << 16) | AT91C_BASE_TC0->TC_CV;
	if ((tmp_count & 0x0000ffff) == 0) { //small chance that we may have missed an increment in TC2
		return (AT91C_BASE_TC2->TC_CV << 16);
	} 
	else {
		return tmp_count;
	}
#endif
}


static uint32_t iso14a_timeout;
uint8_t *trace = (uint8_t *) BigBuf+TRACE_OFFSET;
int rsamples = 0;
int traceLen = 0;
int tracing = TRUE;
uint8_t trigger = 0;
// the block number for the ISO14443-4 PCB
static uint8_t iso14_pcb_blocknum = 0;

//
// ISO14443 timing:
//
// minimum time between the start bits of consecutive transfers from reader to tag: 7000 carrier (13.56Mhz) cycles
#define REQUEST_GUARD_TIME (7000/16 + 1)
// minimum time between last modulation of tag and next start bit from reader to tag: 1172 carrier cycles 
#define FRAME_DELAY_TIME_PICC_TO_PCD (1172/16 + 1) 
// bool LastCommandWasRequest = FALSE;

//
// Total delays including SSC-Transfers between ARM and FPGA. These are in carrier clock cycles (1/13,56MHz)
//
// When the PM acts as reader and is receiving tag data, it takes
// 3 ticks delay in the AD converter
// 16 ticks until the modulation detector completes and sets curbit
// 8 ticks until bit_to_arm is assigned from curbit
// 8*16 ticks for the transfer from FPGA to ARM
// 4*16 ticks until we measure the time
// - 8*16 ticks because we measure the time of the previous transfer 
#define DELAY_AIR2ARM_AS_READER (3 + 16 + 8 + 8*16 + 4*16 - 8*16) 

// When the PM acts as a reader and is sending, it takes
// 4*16 ticks until we can write data to the sending hold register
// 8*16 ticks until the SHR is transferred to the Sending Shift Register
// 8 ticks until the first transfer starts
// 8 ticks later the FPGA samples the data
// 1 tick to assign mod_sig_coil
#define DELAY_ARM2AIR_AS_READER (4*16 + 8*16 + 8 + 8 + 1)

// When the PM acts as tag and is receiving it takes
// 2 ticks delay in the RF part (for the first falling edge),
// 3 ticks for the A/D conversion,
// 8 ticks on average until the start of the SSC transfer,
// 8 ticks until the SSC samples the first data
// 7*16 ticks to complete the transfer from FPGA to ARM
// 8 ticks until the next ssp_clk rising edge
// 4*16 ticks until we measure the time 
// - 8*16 ticks because we measure the time of the previous transfer 
#define DELAY_AIR2ARM_AS_TAG (2 + 3 + 8 + 8 + 7*16 + 8 + 4*16 - 8*16)
 
// The FPGA will report its internal sending delay in
uint16_t FpgaSendQueueDelay;
// the 5 first bits are the number of bits buffered in mod_sig_buf
// the last three bits are the remaining ticks/2 after the mod_sig_buf shift
#define DELAY_FPGA_QUEUE (FpgaSendQueueDelay<<1)

// When the PM acts as tag and is sending, it takes
// 4*16 ticks until we can write data to the sending hold register
// 8*16 ticks until the SHR is transferred to the Sending Shift Register
// 8 ticks until the first transfer starts
// 8 ticks later the FPGA samples the data
// + a varying number of ticks in the FPGA Delay Queue (mod_sig_buf)
// + 1 tick to assign mod_sig_coil
#define DELAY_ARM2AIR_AS_TAG (4*16 + 8*16 + 8 + 8 + DELAY_FPGA_QUEUE + 1)

// When the PM acts as sniffer and is receiving tag data, it takes
// 3 ticks A/D conversion
// 14 ticks to complete the modulation detection
// 8 ticks (on average) until the result is stored in to_arm
// + the delays in transferring data - which is the same for
// sniffing reader and tag data and therefore not relevant
#define DELAY_TAG_AIR2ARM_AS_SNIFFER (3 + 14 + 8) 
 
// When the PM acts as sniffer and is receiving reader data, it takes
// 2 ticks delay in analogue RF receiver (for the falling edge of the 
// start bit, which marks the start of the communication)
// 3 ticks A/D conversion
// 8 ticks on average until the data is stored in to_arm.
// + the delays in transferring data - which is the same for
// sniffing reader and tag data and therefore not relevant
#define DELAY_READER_AIR2ARM_AS_SNIFFER (2 + 3 + 8) 

//variables used for timing purposes:
//these are in ssp_clk cycles:
uint32_t NextTransferTime;
uint32_t LastTimeProxToAirStart;
uint32_t LastProxToAirDuration;



// CARD TO READER - manchester
// Sequence D: 11110000 modulation with subcarrier during first half
// Sequence E: 00001111 modulation with subcarrier during second half
// Sequence F: 00000000 no modulation with subcarrier
// READER TO CARD - miller
// Sequence X: 00001100 drop after half a period
// Sequence Y: 00000000 no drop
// Sequence Z: 11000000 drop at start
#define	SEC_D 0xf0
#define	SEC_E 0x0f
#define	SEC_F 0x00
#define	SEC_X 0x0c
#define	SEC_Y 0x00
#define	SEC_Z 0xc0

const uint8_t OddByteParity[256] = {
  1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
  0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
  0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
  1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
  0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
  1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
  0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
  0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
  1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1
};


void iso14a_set_trigger(bool enable) {
	trigger = enable;
}

void iso14a_clear_trace() {
	memset(trace, 0x44, TRACE_SIZE);
	traceLen = 0;
}

void iso14a_set_tracing(bool enable) {
	tracing = enable;
}

void iso14a_set_timeout(uint32_t timeout) {
	iso14a_timeout = timeout;
}

//-----------------------------------------------------------------------------
// Generate the parity value for a byte sequence
//
//-----------------------------------------------------------------------------
byte_t oddparity (const byte_t bt)
{
	return OddByteParity[bt];
}

uint32_t GetParity(const uint8_t * pbtCmd, int iLen)
{
	int i;
	uint32_t dwPar = 0;

	// Generate the parity bits
	for (i = 0; i < iLen; i++) {
		// and save them to a 32Bit word
		dwPar |= ((OddByteParity[pbtCmd[i]]) << i);
	}
	return dwPar;
}

void AppendCrc14443a(uint8_t* data, int len)
{
	ComputeCrc14443(CRC_14443_A,data,len,data+len,data+len+1);
}

// The function LogTrace() is also used by the iClass implementation in iClass.c
bool  LogTrace(const uint8_t * btBytes, uint8_t iLen, uint32_t timestamp, uint32_t dwParity, bool bReader)
{
	// Return when trace is full
	if (traceLen + sizeof(timestamp) + sizeof(dwParity) + iLen >= TRACE_SIZE) {
		tracing = FALSE;	// don't trace any more
		return FALSE;
	}
	
	// Trace the random, i'm curious
	trace[traceLen++] = ((timestamp >> 0) & 0xff);
	trace[traceLen++] = ((timestamp >> 8) & 0xff);
	trace[traceLen++] = ((timestamp >> 16) & 0xff);
	trace[traceLen++] = ((timestamp >> 24) & 0xff);
	if (!bReader) {
		trace[traceLen - 1] |= 0x80;
	}
	trace[traceLen++] = ((dwParity >> 0) & 0xff);
	trace[traceLen++] = ((dwParity >> 8) & 0xff);
	trace[traceLen++] = ((dwParity >> 16) & 0xff);
	trace[traceLen++] = ((dwParity >> 24) & 0xff);
	trace[traceLen++] = iLen;
	if (btBytes != NULL && iLen != 0) {
		memcpy(trace + traceLen, btBytes, iLen);
	}
	traceLen += iLen;
	return TRUE;
}

//=============================================================================
// ISO 14443 Type A - Miller decoder
//=============================================================================
// Basics:
// This decoder is used when the PM3 acts as a tag.
// The reader will generate "pauses" by temporarily switching of the field. 
// At the PM3 antenna we will therefore measure a modulated antenna voltage. 
// The FPGA does a comparison with a threshold and would deliver e.g.:
// ........  1 1 1 1 1 1 0 0 1 1 1 1 1 1 1 1 1 1 0 0 1 1 1 1 1 1 1 1 1 1  .......
// The Miller decoder needs to identify the following sequences:
// 2 (or 3) ticks pause followed by 6 (or 5) ticks unmodulated: 	pause at beginning - Sequence Z ("start of communication" or a "0")
// 8 ticks without a modulation: 									no pause - Sequence Y (a "0" or "end of communication" or "no information")
// 4 ticks unmodulated followed by 2 (or 3) ticks pause:			pause in second half - Sequence X (a "1")
// Note 1: the bitstream may start at any time. We therefore need to sync.
// Note 2: the interpretation of Sequence Y and Z depends on the preceding sequence.
//-----------------------------------------------------------------------------
static tUart Uart;

// Lookup-Table to decide if 4 raw bits are a modulation.
// We accept two or three consecutive "0" in any position with the rest "1"
const bool Mod_Miller_LUT[] = {
	TRUE,  TRUE,  FALSE, TRUE,  FALSE, FALSE, FALSE, FALSE,
	TRUE,  TRUE,  FALSE, FALSE, TRUE,  FALSE, FALSE, FALSE
};
#define IsMillerModulationNibble1(b) (Mod_Miller_LUT[(b & 0x00F0) >> 4])
#define IsMillerModulationNibble2(b) (Mod_Miller_LUT[(b & 0x000F)])

void UartReset()
{
	Uart.state = STATE_UNSYNCD;
	Uart.bitCount = 0;
	Uart.len = 0;						// number of decoded data bytes
	Uart.shiftReg = 0;					// shiftreg to hold decoded data bits
	Uart.parityBits = 0;				// 
	Uart.twoBits = 0x0000;	 			// buffer for 2 Bits
	Uart.highCnt = 0;
	Uart.startTime = 0;
	Uart.endTime = 0;
}


// use parameter non_real_time to provide a timestamp. Set to 0 if the decoder should measure real time
static RAMFUNC bool MillerDecoding(uint8_t bit, uint32_t non_real_time)
{

	Uart.twoBits = (Uart.twoBits << 8) | bit;
	
	if (Uart.state == STATE_UNSYNCD) {												// not yet synced
		if (Uart.highCnt < 7) {													// wait for a stable unmodulated signal
			if (Uart.twoBits == 0xffff) {
				Uart.highCnt++;
			} else {
				Uart.highCnt = 0;
			}
		} else {	
			Uart.syncBit = 0xFFFF; // not set
			// look for 00xx1111 (the start bit)
			if 		((Uart.twoBits & 0x6780) == 0x0780) Uart.syncBit = 7; 
			else if ((Uart.twoBits & 0x33C0) == 0x03C0) Uart.syncBit = 6;
			else if ((Uart.twoBits & 0x19E0) == 0x01E0) Uart.syncBit = 5;
			else if ((Uart.twoBits & 0x0CF0) == 0x00F0) Uart.syncBit = 4;
			else if ((Uart.twoBits & 0x0678) == 0x0078) Uart.syncBit = 3;
			else if ((Uart.twoBits & 0x033C) == 0x003C) Uart.syncBit = 2;
			else if ((Uart.twoBits & 0x019E) == 0x001E) Uart.syncBit = 1;
			else if ((Uart.twoBits & 0x00CF) == 0x000F) Uart.syncBit = 0;
			if (Uart.syncBit != 0xFFFF) {
				Uart.startTime = non_real_time?non_real_time:(GetCountSspClk() & 0xfffffff8);
				Uart.startTime -= Uart.syncBit;
				Uart.endTime = Uart.startTime;
				Uart.state = STATE_START_OF_COMMUNICATION;
			}
		}

	} else {

		if (IsMillerModulationNibble1(Uart.twoBits >> Uart.syncBit)) {			
			if (IsMillerModulationNibble2(Uart.twoBits >> Uart.syncBit)) {		// Modulation in both halves - error
				UartReset();
				Uart.highCnt = 6;
			} else {															// Modulation in first half = Sequence Z = logic "0"
				if (Uart.state == STATE_MILLER_X) {								// error - must not follow after X
					UartReset();
					Uart.highCnt = 6;
				} else {
					Uart.bitCount++;
					Uart.shiftReg = (Uart.shiftReg >> 1);						// add a 0 to the shiftreg
					Uart.state = STATE_MILLER_Z;
					Uart.endTime = Uart.startTime + 8*(9*Uart.len + Uart.bitCount + 1) - 6;
					if(Uart.bitCount >= 9) {									// if we decoded a full byte (including parity)
						Uart.output[Uart.len++] = (Uart.shiftReg & 0xff);
						Uart.parityBits <<= 1;									// make room for the parity bit
						Uart.parityBits |= ((Uart.shiftReg >> 8) & 0x01);		// store parity bit
						Uart.bitCount = 0;
						Uart.shiftReg = 0;
					}
				}
			}
		} else {
			if (IsMillerModulationNibble2(Uart.twoBits >> Uart.syncBit)) {		// Modulation second half = Sequence X = logic "1"
				Uart.bitCount++;
				Uart.shiftReg = (Uart.shiftReg >> 1) | 0x100;					// add a 1 to the shiftreg
				Uart.state = STATE_MILLER_X;
				Uart.endTime = Uart.startTime + 8*(9*Uart.len + Uart.bitCount + 1) - 2;
				if(Uart.bitCount >= 9) {										// if we decoded a full byte (including parity)
					Uart.output[Uart.len++] = (Uart.shiftReg & 0xff);
					Uart.parityBits <<= 1;										// make room for the new parity bit
					Uart.parityBits |= ((Uart.shiftReg >> 8) & 0x01); 			// store parity bit
					Uart.bitCount = 0;
					Uart.shiftReg = 0;
				}
			} else {															// no modulation in both halves - Sequence Y
				if (Uart.state == STATE_MILLER_Z || Uart.state == STATE_MILLER_Y) {	// Y after logic "0" - End of Communication
					Uart.state = STATE_UNSYNCD;
					if(Uart.len == 0 && Uart.bitCount > 0) {										// if we decoded some bits
						Uart.shiftReg >>= (9 - Uart.bitCount);					// add them to the output
						Uart.output[Uart.len++] = (Uart.shiftReg & 0xff);
						Uart.parityBits <<= 1;									// no parity bit - add "0"
						Uart.bitCount--;										// last "0" was part of the EOC sequence
					}
					return TRUE;
				}
				if (Uart.state == STATE_START_OF_COMMUNICATION) {				// error - must not follow directly after SOC
					UartReset();
					Uart.highCnt = 6;
				} else {														// a logic "0"
					Uart.bitCount++;
					Uart.shiftReg = (Uart.shiftReg >> 1);						// add a 0 to the shiftreg
					Uart.state = STATE_MILLER_Y;
					if(Uart.bitCount >= 9) {									// if we decoded a full byte (including parity)
						Uart.output[Uart.len++] = (Uart.shiftReg & 0xff);
						Uart.parityBits <<= 1;									// make room for the parity bit
						Uart.parityBits |= ((Uart.shiftReg >> 8) & 0x01); 		// store parity bit
						Uart.bitCount = 0;
						Uart.shiftReg = 0;
					}
				}
			}
		}
			
	} 

    return FALSE;	// not finished yet, need more data
}



//=============================================================================
// ISO 14443 Type A - Manchester decoder
//=============================================================================
// Basics:
// This decoder is used when the PM3 acts as a reader.
// The tag will modulate the reader field by asserting different loads to it. As a consequence, the voltage
// at the reader antenna will be modulated as well. The FPGA detects the modulation for us and would deliver e.g. the following:
// ........ 0 0 1 1 1 1 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 .......
// The Manchester decoder needs to identify the following sequences:
// 4 ticks modulated followed by 4 ticks unmodulated: 	Sequence D = 1 (also used as "start of communication")
// 4 ticks unmodulated followed by 4 ticks modulated: 	Sequence E = 0
// 8 ticks unmodulated:									Sequence F = end of communication
// 8 ticks modulated:									A collision. Save the collision position and treat as Sequence D
// Note 1: the bitstream may start at any time. We therefore need to sync.
// Note 2: parameter offset is used to determine the position of the parity bits (required for the anticollision command only)
static tDemod Demod;

// Lookup-Table to decide if 4 raw bits are a modulation.
// We accept three or four "1" in any position
const bool Mod_Manchester_LUT[] = {
	FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE,
	FALSE, FALSE, FALSE, TRUE,  FALSE, TRUE,  TRUE,  TRUE
};

#define IsManchesterModulationNibble1(b) (Mod_Manchester_LUT[(b & 0x00F0) >> 4])
#define IsManchesterModulationNibble2(b) (Mod_Manchester_LUT[(b & 0x000F)])


void DemodReset()
{
	Demod.state = DEMOD_UNSYNCD;
	Demod.len = 0;						// number of decoded data bytes
	Demod.shiftReg = 0;					// shiftreg to hold decoded data bits
	Demod.parityBits = 0;				// 
	Demod.collisionPos = 0;				// Position of collision bit
	Demod.twoBits = 0xffff;				// buffer for 2 Bits
	Demod.highCnt = 0;
	Demod.startTime = 0;
	Demod.endTime = 0;
}


// use parameter non_real_time to provide a timestamp. Set to 0 if the decoder should measure real time
static RAMFUNC int ManchesterDecoding(uint8_t bit, uint16_t offset, uint32_t non_real_time)
{

	Demod.twoBits = (Demod.twoBits << 8) | bit;
	
	if (Demod.state == DEMOD_UNSYNCD) {

		if (Demod.highCnt < 2) {											// wait for a stable unmodulated signal
			if (Demod.twoBits == 0x0000) {
				Demod.highCnt++;
			} else {
				Demod.highCnt = 0;
			}
		} else {
			Demod.syncBit = 0xFFFF;			// not set
			if 		((Demod.twoBits & 0x7700) == 0x7000) Demod.syncBit = 7; 
			else if ((Demod.twoBits & 0x3B80) == 0x3800) Demod.syncBit = 6;
			else if ((Demod.twoBits & 0x1DC0) == 0x1C00) Demod.syncBit = 5;
			else if ((Demod.twoBits & 0x0EE0) == 0x0E00) Demod.syncBit = 4;
			else if ((Demod.twoBits & 0x0770) == 0x0700) Demod.syncBit = 3;
			else if ((Demod.twoBits & 0x03B8) == 0x0380) Demod.syncBit = 2;
			else if ((Demod.twoBits & 0x01DC) == 0x01C0) Demod.syncBit = 1;
			else if ((Demod.twoBits & 0x00EE) == 0x00E0) Demod.syncBit = 0;
			if (Demod.syncBit != 0xFFFF) {
				Demod.startTime = non_real_time?non_real_time:(GetCountSspClk() & 0xfffffff8);
				Demod.startTime -= Demod.syncBit;
				Demod.bitCount = offset;			// number of decoded data bits
				Demod.state = DEMOD_MANCHESTER_DATA;
			}
		}

	} else {

		if (IsManchesterModulationNibble1(Demod.twoBits >> Demod.syncBit)) {		// modulation in first half
			if (IsManchesterModulationNibble2(Demod.twoBits >> Demod.syncBit)) {	// ... and in second half = collision
				if (!Demod.collisionPos) {
					Demod.collisionPos = (Demod.len << 3) + Demod.bitCount;
				}
			}															// modulation in first half only - Sequence D = 1
			Demod.bitCount++;
			Demod.shiftReg = (Demod.shiftReg >> 1) | 0x100;				// in both cases, add a 1 to the shiftreg
			if(Demod.bitCount == 9) {									// if we decoded a full byte (including parity)
				Demod.output[Demod.len++] = (Demod.shiftReg & 0xff);
				Demod.parityBits <<= 1;									// make room for the parity bit
				Demod.parityBits |= ((Demod.shiftReg >> 8) & 0x01); 	// store parity bit
				Demod.bitCount = 0;
				Demod.shiftReg = 0;
			}
			Demod.endTime = Demod.startTime + 8*(9*Demod.len + Demod.bitCount + 1) - 4;
		} else {														// no modulation in first half
			if (IsManchesterModulationNibble2(Demod.twoBits >> Demod.syncBit)) {	// and modulation in second half = Sequence E = 0
				Demod.bitCount++;
				Demod.shiftReg = (Demod.shiftReg >> 1);					// add a 0 to the shiftreg
				if(Demod.bitCount >= 9) {								// if we decoded a full byte (including parity)
					Demod.output[Demod.len++] = (Demod.shiftReg & 0xff);
					Demod.parityBits <<= 1;								// make room for the new parity bit
					Demod.parityBits |= ((Demod.shiftReg >> 8) & 0x01); // store parity bit
					Demod.bitCount = 0;
					Demod.shiftReg = 0;
				}
				Demod.endTime = Demod.startTime + 8*(9*Demod.len + Demod.bitCount + 1);
			} else {													// no modulation in both halves - End of communication
				if (Demod.len > 0 || Demod.bitCount > 0) {				// received something
					if(Demod.bitCount > 0) {							// if we decoded bits
						Demod.shiftReg >>= (9 - Demod.bitCount);		// add the remaining decoded bits to the output
						Demod.output[Demod.len++] = Demod.shiftReg & 0xff;
						// No parity bit, so just shift a 0
						Demod.parityBits <<= 1;
					}
					return TRUE;										// we are finished with decoding the raw data sequence
				} else { 												// nothing received. Start over
					DemodReset();
				}
			}
		}
			
	} 

    return FALSE;	// not finished yet, need more data
}

//=============================================================================
// Finally, a `sniffer' for ISO 14443 Type A
// Both sides of communication!
//=============================================================================

//-----------------------------------------------------------------------------
// Record the sequence of commands sent by the reader to the tag, with
// triggering so that we start recording at the point that the tag is moved
// near the reader.
//-----------------------------------------------------------------------------
void RAMFUNC SnoopIso14443a(uint8_t param) {
	// param:
	// bit 0 - trigger from first card answer
	// bit 1 - trigger from first reader 7-bit request
	
	LEDsoff();
	// init trace buffer
	iso14a_clear_trace();

	// We won't start recording the frames that we acquire until we trigger;
	// a good trigger condition to get started is probably when we see a
	// response from the tag.
	// triggered == FALSE -- to wait first for card
	bool triggered = !(param & 0x03); 
	
	// The command (reader -> tag) that we're receiving.
	// The length of a received command will in most cases be no more than 18 bytes.
	// So 32 should be enough!
	uint8_t *receivedCmd = (((uint8_t *)BigBuf) + RECV_CMD_OFFSET);
	// The response (tag -> reader) that we're receiving.
	uint8_t *receivedResponse = (((uint8_t *)BigBuf) + RECV_RES_OFFSET);

	// As we receive stuff, we copy it from receivedCmd or receivedResponse
	// into trace, along with its length and other annotations.
	//uint8_t *trace = (uint8_t *)BigBuf;
	
	// The DMA buffer, used to stream samples from the FPGA
	uint8_t *dmaBuf = ((uint8_t *)BigBuf) + DMA_BUFFER_OFFSET;
	uint8_t *data = dmaBuf;
	uint8_t previous_data = 0;
	int maxDataLen = 0;
	int dataLen = 0;
	bool TagIsActive = FALSE;
	bool ReaderIsActive = FALSE;
	
	iso14443a_setup(FPGA_HF_ISO14443A_SNIFFER);

	// Set up the demodulator for tag -> reader responses.
	Demod.output = receivedResponse;

	// Set up the demodulator for the reader -> tag commands
	Uart.output = receivedCmd;

	// Setup and start DMA.
	FpgaSetupSscDma((uint8_t *)dmaBuf, DMA_BUFFER_SIZE);
	
#if 0
	// And now we loop, receiving samples.
	for(uint32_t rsamples = 0; TRUE; ) {

		if(BUTTON_PRESS()) {
			DbpString("cancelled by button");
			break;
		}

		LED_A_ON();
		WDT_HIT();
		int register readBufDataP = data - dmaBuf;
		// pdc_RCR (Receive Counter Register) contains the number of transfers to be performed.
		int register dmaBufDataP = DMA_BUFFER_SIZE - AT91C_BASE_PDC_SSC->PDC_RCR;
		
		if (readBufDataP <= dmaBufDataP){
			dataLen = dmaBufDataP - readBufDataP;
		} else {
			dataLen = DMA_BUFFER_SIZE - readBufDataP + dmaBufDataP;
		}
		// test for length of buffer
		if(dataLen > maxDataLen) {
			maxDataLen = dataLen;
			if(dataLen > 400) {
				Dbprintf("blew circular buffer! dataLen=%d", dataLen);
				break;
			}
		}
		if(dataLen < 1) continue;

		// primary buffer was stopped( <-- we lost data!
		if (!AT91C_BASE_PDC_SSC->PDC_RCR) {
			AT91C_BASE_PDC_SSC->PDC_RPR = (uint32_t) dmaBuf;
			AT91C_BASE_PDC_SSC->PDC_RCR = DMA_BUFFER_SIZE;
			Dbprintf("RxEmpty ERROR!!! data length:%d", dataLen); // temporary
		}
		// secondary buffer sets as primary, secondary buffer was stopped
		if (!AT91C_BASE_PDC_SSC->PDC_RNCR) {
			AT91C_BASE_PDC_SSC->PDC_RNPR = (uint32_t) dmaBuf;
			AT91C_BASE_PDC_SSC->PDC_RNCR = DMA_BUFFER_SIZE;
		}

		LED_A_OFF();
		
		if (rsamples & 0x01) {				// Need two samples to feed Miller and Manchester-Decoder

			if(!TagIsActive) {		// no need to try decoding reader data if the tag is sending
				uint8_t readerdata = (previous_data & 0xF0) | (*data >> 4);
				if (MillerDecoding(readerdata, (rsamples-1)*4)) {
					LED_C_ON();

					// check - if there is a short 7bit request from reader
					if ((!triggered) && (param & 0x02) && (Uart.len == 1) && (Uart.bitCount == 7)) triggered = TRUE;

					if(triggered) {
						if (!LogTrace(receivedCmd, Uart.len, Uart.startTime*16 - DELAY_READER_AIR2ARM_AS_SNIFFER, Uart.parityBits, TRUE)) break;
						if (!LogTrace(NULL, 0, Uart.endTime*16 - DELAY_READER_AIR2ARM_AS_SNIFFER, 0, TRUE)) break;
					}
					/* And ready to receive another command. */
					UartReset();
					/* And also reset the demod code, which might have been */
					/* false-triggered by the commands from the reader. */
					DemodReset();
					LED_B_OFF();
				}
				ReaderIsActive = (Uart.state != STATE_UNSYNCD);
			}

			if(!ReaderIsActive) {		// no need to try decoding tag data if the reader is sending - and we cannot afford the time
				uint8_t tagdata = (previous_data << 4) | (*data & 0x0F);
				if(ManchesterDecoding(tagdata, 0, (rsamples-1)*4)) {
					LED_B_ON();

					if (!LogTrace(receivedResponse, Demod.len, Demod.startTime*16 - DELAY_TAG_AIR2ARM_AS_SNIFFER, Demod.parityBits, FALSE)) break;
					if (!LogTrace(NULL, 0, Demod.endTime*16 - DELAY_TAG_AIR2ARM_AS_SNIFFER, 0, FALSE)) break;

					if ((!triggered) && (param & 0x01)) triggered = TRUE;

					// And ready to receive another response.
					DemodReset();
					LED_C_OFF();
				} 
				TagIsActive = (Demod.state != DEMOD_UNSYNCD);
			}
		}

		previous_data = *data;
		rsamples++;
		data++;
		if(data == dmaBuf + DMA_BUFFER_SIZE) {
			data = dmaBuf;
		}
	} // main cycle
#endif
	
	DbpString("COMMAND FINISHED");

	FpgaDisableSscDma();
	Dbprintf("maxDataLen=%d, Uart.state=%x, Uart.len=%d", maxDataLen, Uart.state, Uart.len);
	Dbprintf("traceLen=%d, Uart.output[0]=%08x", traceLen, (uint32_t)Uart.output[0]);
	LEDsoff();
}
