/*--------------------------*/
/* Integer type definitions */
/*--------------------------*/

#ifndef _INTEGER

#if 0
#include <windows.h>
#else

#include <stdint.h>
#include <inttypes.h>   // include C99 standard types

/* These types must be 16-bit, 32-bit or larger integer */
typedef int				INT;
typedef unsigned int	UINT;

/* These types must be 8-bit integer */
typedef signed char		CHAR;
typedef signed char		SBYTE;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

/* These types must be 16-bit integer */
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types must be 32-bit integer */
typedef long			LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;

#ifndef __bool_true_false_are_defined
/* Boolean type */
typedef enum { FALSE = 0, TRUE } BOOL;
#endif

#endif

#define _INTEGER
#endif
