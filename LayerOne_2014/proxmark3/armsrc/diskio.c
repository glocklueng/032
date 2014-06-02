/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"

DRESULT disk_write ( 
   BYTE drv,         /* Physical drive nmuber (0) */ 
   const BYTE *buff,   /* Pointer to the data to be written */ 
   DWORD sector,      /* Start sector number (LBA) */ 
   BYTE count         /* Sector count (1..255) */ 
) 
{
  return 0;
}

DRESULT disk_ioctl (
  BYTE pdrv,     /* [IN] Drive number */
  BYTE cmd,      /* [IN] Control command code */
  void* buff     /* [I/O] Parameter and data buffer */
)
{
  return 0;
}


DRESULT disk_read (
  BYTE pdrv,     /* [IN] Physical drive number */
  BYTE* buff,    /* [OUT] Pointer to the read data buffer */
  DWORD sector,  /* [IN] Start sector number */
  BYTE count     /* [IN] Number of sectros to read */
)
{
  return 0;
}


DSTATUS disk_status (BYTE s)
{
  return 0;
  
}

void init_port(void)
{
}
