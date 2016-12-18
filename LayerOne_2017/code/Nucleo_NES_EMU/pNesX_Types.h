/*===================================================================*/
/*                                                                   */
/*  pNesX_Types.h : Type definitions for pNesX                       */
/*                                                                   */
/*  1999/11/03  Racoon  New preparation                              */
/*                                                                   */
/*===================================================================*/

#ifndef PNESX_TYPES_H_INCLUDED
#define PNESX_TYPES_H_INCLUDED

/*-------------------------------------------------------------------*/
/*  Type definition                                                  */
/*-------------------------------------------------------------------*/
#ifndef DWORD
typedef unsigned long  DWORD;
#endif /* !DWORD */

#ifndef WORD
typedef unsigned short WORD;
#endif /* !WORD */

#ifndef BYTE
typedef unsigned char  BYTE;
#endif /* !BYTE */

/*-------------------------------------------------------------------*/
/*  NULL definition                                                  */
/*-------------------------------------------------------------------*/
#ifndef NULL
#define NULL  0
#endif /* !NULL */

// RLDUStStBA
//A, B, Select, Start, Up, Down, Left, Right

#define KEYPAD_DOWN     ( 0x20 ) 
#define KEYPAD_UP       ( 0x10 ) 
#define KEYPAD_LEFT     ( 0x40 ) 
#define KEYPAD_RIGHT    ( 0x80 ) 

#define KEYPAD_A        ( 0x1 ) 
#define KEYPAD_B        ( 0x2 ) 

#define KEYPAD_SELECT   ( 0x4 ) 
#define KEYPAD_START    ( (1<<3) ) 

#endif /* !PNESX_TYPES_H_INCLUDED */



