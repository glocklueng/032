/*************************************************************************
 * Example Table Driven CRC16 Routine using 4-bit message chunks
 *
 * By Ashley Roll
 * Digital Nemesis Pty Ltd
 * www.digitalnemesis.com
 *
 * The following is an example of implementing a restricted size CRC16
 * table lookup. No optimisation as been done so the code is clear and
 * easy to understand.
 *
 * Test Vector: "123456789" (character string, no quotes)
 * Generated CRC: 0x29B1
 * 
 *************************************************************************/

#ifndef _CRC_H
#define _CRC_H

// CRC16 "Register". This is implemented as two 8bit values
extern unsigned char CRC16_High, CRC16_Low;

/* Before each message CRC is generated, the CRC register must be 
 * initialised by calling this function */
void CRC16_Init(void);

/* Process 4 bits of the message to update the CRC Value.
 * Note that the data must be in the low nibble of val.  */
void CRC16_Update4Bits(unsigned char val);

// Process one Message Byte to update the current CRC Value
void CRC16_Update(unsigned char val);

#endif
