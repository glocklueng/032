/*----------------------------------------------------------------------*/
/* FAT file system sample project for FatFs R0.06  (C)ChaN, 2008        */
/*----------------------------------------------------------------------*/
/* Commands;
dd [<sector>] - Dump sector
ds - Show disk status
bd <addr> - Dump R/W buffer
be <addr> [<data>] ... - Edit R/W buffer
br <sector> [<n>] - Read disk into R/W buffer
bw <sector> [<n>] - Write R/W buffer into disk
bf <n> - Fill working buffer
fs [<path>] - Show logical drive status
fl [<path>] - Directory listing
fo <mode> <name> - Open a file
fc - Close a file
fe - Seek file pointer
fr <len> - read file
fd <len> - read and dump file from current fp
fw <len> <val> - write file
fv - Truncate file
fn <old_name> <new_name> - Change file/dir name
fk <name> - Create a directory
fa <atrr> <mask> <name> - Change file/dir attribute
ft <year> <month> <day> <hour> <min> <sec> <name>
fx <src_name> <dst_name> - Copy file

*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include "uart.h"
#include "xitoa.h"
#include "ff.h"
#include "diskio.h"
#include "rtc.h"
#include "main.h"

DWORD acc_size;				/* Work register for fs command */
WORD acc_files, acc_dirs;

volatile WORD Timer;		/* 100Hz increment timer */

#if _MULTI_PARTITION != 0
const PARTITION Drives[] = { {0,0}, {0,1} };
#endif

/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */
DWORD get_fattime () {
	RTCLK rtc;
	rtc_gettime(&rtc);

	return	  ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.month << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);
}


/*--------------------------------------------------------------------------*/
/* Monitor                                                                  */
static void put_dump (const BYTE *buff, uint32_t ofs, BYTE cnt) {
	BYTE n;

	xprintf(PSTR("%08lX "), ofs);
	for(n = 0; n < cnt; n++)
		xprintf(PSTR(" %02X"), buff[n]);
	xputc(' ');
	for(n = 0; n < cnt; n++) {
		if ((buff[n] < 0x20)||(buff[n] >= 0x7F))
			xputc('.');
		else
			xputc(buff[n]);
	}
	xputc('\n');
}

static void get_line (unsigned char *buff, int len) {
	unsigned char c;
	int idx = 0;

	for (;;) {
        if(key==KS) return; // Asterisk pressed
        if(uart_test())     // Data in receive buffer?
        	c = uart_get();
        else continue;
		if (c == '\r') break;
		if ((c == '\b') && idx) {
			idx--; uart_put(c);
		}
		if (((BYTE)c >= ' ') && (idx < len - 1)) {
				buff[idx++] = c; uart_put(c);
		}
	}
	buff[idx] = 0;
	uart_put(c);
	uart_put('\n');
}

static FRESULT scan_files (unsigned char* path) {
	DIR dirs;
	FRESULT res;
	int i;

	if ((res = f_opendir(&dirs, path)) == FR_OK) {
		i = strlen(path);
		while (((res = f_readdir(&dirs, &finfo)) == FR_OK) && finfo.fname[0]) {
			if (finfo.fattrib & AM_DIR) {
				acc_dirs++;
				*(path+i) = '/'; strcpy(path+i+1, &finfo.fname[0]);
				res = scan_files(path);
				*(path+i) = '\0';
				if (res != FR_OK) break;
			} else {
				acc_files++;
				acc_size += finfo.fsize;
			}
		}
	}
	return res;
}

static void put_rc (FRESULT rc) {
	const prog_char *p;
	static const prog_char str[] =
		"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
		"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
		"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0";
	FRESULT i;

	for (p = str, i = 0; i != rc && pgm_read_byte_near(p); i++) {
		while(pgm_read_byte_near(p++));
	}
	xprintf(PSTR("rc=%u FR_%S\n"), (WORD)rc, p);
}

/*-----------------------------------------------------------------------*/
/* Main                                                                  */
int SD(void) {
    BYTE text[120];				/* Console input buffer */
    BYTE Buff[1024];			/* Working buffer */
	unsigned char *ptr=NULL, *ptr2;
	DWORD p1=0, p2=0, p3=0;
	BYTE res, b1;
	WORD w1;
	UINT s1, s2, cnt;
	DWORD ofs, sect = 0;
	RTCLK rtc;
	DIR dir;				/* Directory object */
	FIL file1, file2;		/* File object */
    FATFS *fs;
    static unsigned char KeytoExit[] PROGMEM = "Press a key to exit\n";

    lcd_init(TEXT);
    lcd_putsp(PSTR("XMultiKit SDCard Test"));
    NewLine();
    res=disk_status();
    if(res&STA_NODISK) {
        lcd_putsp(PSTR("Insert SD card or\n"));
        lcd_putsp(KeytoExit);
    }
    for(b1=0; b1<50; ) {
        res=disk_status();
        if(key) return 1;
        if(!(res&STA_NODISK)) b1++;
        _delay_ms(10);
    } 

    _delay_ms(100);
    
    if(res&STA_PROTECT) {
        lcd_putsp(PSTR("SD is Write Protected"));
    }

    if(res&STA_NOINIT) {
        lcd_putsp(PSTR("Initialization failed"));
        lcd_putsp(KeytoExit);
        while(!key);
        return 1;
    }

    lcd_putsp(PSTR("SDCard is initialized"));
	if (disk_ioctl(MMC_GET_TYPE, &b1) == RES_OK) {
		lcd_putsp(PSTR("Card type: "));
            switch(b1) {
                case CT_MMC: lcd_putsp(PSTR("MMC")); break;
                case CT_SD1: lcd_putsp(PSTR("SD1")); break;
                case CT_SD2: lcd_putsp(PSTR("SD2")); break;
                case CT_SDC: lcd_putsp(PSTR("SDC")); break;
                case CT_BLOCK: lcd_putsp(PSTR("BLK")); break;
                default: printN(b1);
            }
    }
    NewLine(); NewLine(); lcd_putsp(PSTR("Initializing FAT Disk"));


    res = f_getfree(ptr, &p2, &fs);
	if (res) { 
        lcd_putsp(PSTR("FAT SYSTEM ERROR!\n"));
        return;
    }
    else lcd_putsp(PSTR("FAT OK!\n"));

    NewLine(); lcd_putsp(PSTR("Use HyperTerminal now"));
    NewLine(); lcd_putsp(PSTR("Press * to finish"));

	xputs(PSTR("\nFatFs module test monitor\n"));
    xputs(PSTR("dd [<sector>]            - Dump sector\n"));
    xputs(PSTR("ds                       - Show disk status\n"));
    xputs(PSTR("bd <addr>                - Dump R/W buffer\n"));
    xputs(PSTR("be <addr>.[<data>] ...   - Edit R/W buffer\n"));
    xputs(PSTR("br <sector>.[<n>]        - Read disk into R/W buffer\n"));
    xputs(PSTR("bw <sector>.[<n>]        - Write R/W buffer into disk\n"));
    xputs(PSTR("bf <n>                   - Fill working buffer\n"));
    xputs(PSTR("fs [<path>]              - Show logical drive status\n"));
    xputs(PSTR("fl [<path>]              - Directory listing\n"));
    xputs(PSTR("fo <mode>.<name>         - Open a file\n"));
    xputs(PSTR("fc                       - Close a file\n"));
    xputs(PSTR("fe                       - Seek file pointer\n"));
    xputs(PSTR("fr <len>                 - read file\n"));
    xputs(PSTR("fd <len>                 - read and dump file from current fp\n"));
    xputs(PSTR("fw <len>.<val>           - write file\n"));
    xputs(PSTR("fv                       - Truncate file\n"));
    xputs(PSTR("fn <old_name>.<new_name> - Change file/dir name\n"));
    xputs(PSTR("fk <name>                - Create a directory\n"));
    xputs(PSTR("fa <atrr> <mask> <name>  - Change file/dir attribute\n"));
    xputs(PSTR("ft <year> <month> <day> <hour> <min> <sec> <name>\n"));
    xputs(PSTR("fx <src> <dst>           - Copy file\n"));
 	for (;;) {
		xputc('>');
		get_line(text, sizeof(text));
		ptr = text;
        if(key==KS) return 0; // Asterisk pressed

    	switch (*ptr++) {
		case 'd' :
			switch (*ptr++) {
			case 'd' :	/* dd [<sector>] - Dump sector */
				if (!xatoi(&ptr, &p1)) p1 = sect;
				res = disk_read(Buff, p1, 1);
				if (res) { xprintf(PSTR("rc=%d\n"), (WORD)res); break; }
				sect = p1 + 1;
				xprintf(PSTR("Sector:%lu\n"), p2);
				for (ptr=Buff, ofs = 0; ofs < 0x200; ptr+=16, ofs+=16)
					put_dump(ptr, ofs, 16);
                break;
			case 's' :	/* ds - Show disk status */
				if (disk_ioctl(GET_SECTOR_COUNT, &p2) == RES_OK)
					{ xprintf(PSTR("Drive size: %lu sectors\n"), p2); }
				if (disk_ioctl(GET_SECTOR_SIZE, &w1) == RES_OK)
					{ xprintf(PSTR("Sector size: %u\n"), w1); }
				if (disk_ioctl(GET_BLOCK_SIZE, &p2) == RES_OK)
					{ xprintf(PSTR("Erase block size: %lu sectors\n"), p2); }
				if (disk_ioctl(MMC_GET_TYPE, &b1) == RES_OK)
					{ xprintf(PSTR("Card type: %u\n"), b1); }
				if (disk_ioctl(MMC_GET_CSD, Buff) == RES_OK)
					{ xputs(PSTR("CSD:\n")); put_dump(Buff, 0, 16); }
				if (disk_ioctl(MMC_GET_CID, Buff) == RES_OK)
					{ xputs(PSTR("CID:\n")); put_dump(Buff, 0, 16); }
				if (disk_ioctl(MMC_GET_OCR, Buff) == RES_OK)
					{ xputs(PSTR("OCR:\n")); put_dump(Buff, 0, 4); }
				if (disk_ioctl(MMC_GET_SDSTAT, Buff) == RES_OK) {
					xputs(PSTR("SD Status:\n"));
					for (s1 = 0; s1 < 64; s1 += 16) put_dump(Buff+s1, s1, 16);
				}
				if (disk_ioctl(ATA_GET_MODEL, text) == RES_OK)
					{ text[40] = '\0'; xprintf(PSTR("Model: %s\n"), text); }
				if (disk_ioctl(ATA_GET_SN, text) == RES_OK)
					{ text[20] = '\0'; xprintf(PSTR("S/N: %s\n"), text); }
				break;
			}
			break;

		case 'b' :
			switch (*ptr++) {
			case 'd' :	/* bd <addr> - Dump R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				for (ptr=&Buff[p1], ofs = p1, cnt = 32; cnt; cnt--, ptr+=16, ofs+=16)
					put_dump(ptr, ofs, 16);
				break;

			case 'e' :	/* be <addr> [<data>] ... - Edit R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (xatoi(&ptr, &p2)) {
					do {
						Buff[p1++] = (BYTE)p2;
					} while (xatoi(&ptr, &p2));
					break;
				}
				for (;;) {
					xprintf(PSTR("%04X %02X-"), (WORD)(p1), (WORD)Buff[p1]);
					get_line(text, sizeof(text));
					ptr = text;
					if (*ptr == '.') break;
					if (*ptr < ' ') { p1++; continue; }
					if (xatoi(&ptr, &p2))
						Buff[p1++] = (BYTE)p2;
					else
						xputs(PSTR("???\n"));
				}
				break;

			case 'r' :	/* br <sector> [<n>] - Read disk into R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				xprintf(PSTR("rc=%u\n"), (WORD)disk_read(Buff, p2, p3));
				break;

			case 'w' :	/* bw <sector> [<n>] - Write R/W buffer into disk */
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				xprintf(PSTR("rc=%u\n"), (WORD)disk_write(Buff, p2, p3));
				break;

			case 'f' :	/* bf <n> - Fill working buffer */
				if (!xatoi(&ptr, &p1)) break;
				memset(Buff, (BYTE)p1, sizeof(Buff));
				break;

			}
			break;

		case 'f' :
			switch (*ptr++) {

			case 's' :	/* fs [<path>] - Show logical drive status */
				while (*ptr == ' ') ptr++;
				res = f_getfree(ptr, &p2, &fs);
				if (res) { put_rc(res); break; }
				xprintf(PSTR("FAT type = %u\nBytes/Cluster = %lu\nNumber of FATs = %u\n"
							 "Root DIR entries = %u\nSectors/FAT = %lu\nNumber of clusters = %lu\n"
							 "FAT start (lba) = %lu\nDIR start (lba,clustor) = %lu\nData start (lba) = %lu\n"),
						(WORD)fs->fs_type, (DWORD)fs->csize * 512, (WORD)fs->n_fats,
						fs->n_rootdir, (DWORD)fs->sects_fat, (DWORD)fs->max_clust - 2,
						fs->fatbase, fs->dirbase, fs->database
				);
				acc_size = acc_files = acc_dirs = 0;
				res = scan_files(ptr);
				if (res) { put_rc(res); break; }
				xprintf(PSTR("%u files, %lu bytes.\n%u folders.\n"
							 "%lu KB total disk space.\n%lu KB available.\n"),
						acc_files, acc_size, acc_dirs,
						(fs->max_clust - 2) * (fs->csize / 2), p2 * (fs->csize / 2)
				);
				break;

			case 'l' :	/* fl [<path>] - Directory listing */
				while (*ptr == ' ') ptr++;
				res = f_opendir(&dir, ptr);
				if (res) { put_rc(res); break; }
				p1 = s1 = s2 = 0;
				for(;;) {
					res = f_readdir(&dir, &finfo);
					if ((res != FR_OK) || !finfo.fname[0]) break;
					if (finfo.fattrib & AM_DIR) {
						s2++;
					} else {
						s1++; p1 += finfo.fsize;
					}
					xprintf(PSTR("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s\n"), 
								(finfo.fattrib & AM_DIR) ? 'D' : '-',
								(finfo.fattrib & AM_RDO) ? 'R' : '-',
								(finfo.fattrib & AM_HID) ? 'H' : '-',
								(finfo.fattrib & AM_SYS) ? 'S' : '-',
								(finfo.fattrib & AM_ARC) ? 'A' : '-',
								(finfo.fdate >> 9) + 1980, (finfo.fdate >> 5) & 15, finfo.fdate & 31,
								(finfo.ftime >> 11), (finfo.ftime >> 5) & 63,
								finfo.fsize, &(finfo.fname[0]));
				}
				xprintf(PSTR("%4u File(s),%10lu bytes total\n%4u Dir(s)"), s1, p1, s2);
				if (f_getfree(ptr, &p1, &fs) == FR_OK)
					xprintf(PSTR(", %10luK bytes free\n"), p1 * fs->csize / 2);
				break;

			case 'o' :	/* fo <mode> <name> - Open a file */
				if (!xatoi(&ptr, &p1)) {
                    xprintf(PSTR("error"));
                    break;
                }
				while (*ptr == ' ') ptr++;
				put_rc(f_open(&file1, ptr, (BYTE)p1));
				break;

			case 'c' :	/* fc - Close a file */
				put_rc(f_close(&file1));
				break;

			case 'e' :	/* fe - Seek file pointer */
				if (!xatoi(&ptr, &p1)) break;
				res = f_lseek(&file1, p1);
				put_rc(res);
				if (res == FR_OK)
					xprintf(PSTR("fptr = %lu(0x%lX)\n"), file1.fptr, file1.fptr);
				break;

			case 'r' :	/* fr <len> - read file */
				if (!xatoi(&ptr, &p1)) break;
				p2 = 0;
				Timer = 0;
				while (p1) {
					if (p1 >= sizeof(Buff))	{ cnt = sizeof(Buff); p1 -= sizeof(Buff); }
					else 			{ cnt = (WORD)p1; p1 = 0; }
					res = f_read(&file1, Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				s2 = Timer;
				xprintf(PSTR("%lu bytes read with %lu bytes/sec.\n"), p2, p2 * 100 / s2);
				break;

			case 'd' :	/* fd <len> - read and dump file from current fp */
				if (!xatoi(&ptr, &p1)) break;
				ofs = file1.fptr;
				while (p1) {
					if (p1 >= 16)	{ cnt = 16; p1 -= 16; }
					else 			{ cnt = (WORD)p1; p1 = 0; }
					res = f_read(&file1, Buff, cnt, &cnt);
					if (res != FR_OK) { put_rc(res); break; }
					if (!cnt) break;
					put_dump(Buff, ofs, cnt);
					ofs += 16;
				}
				break;

			case 'w' :	/* fw <len> <val> - write file */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				memset(Buff, (BYTE)p2, sizeof(Buff));
				p2 = 0;
				Timer = 0;
				while (p1) {
					if (p1 >= sizeof(Buff))	{ cnt = sizeof(Buff); p1 -= sizeof(Buff); }
					else 			{ cnt = (WORD)p1; p1 = 0; }
					res = f_write(&file1, Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				s2 = Timer;
				xprintf(PSTR("%lu bytes written with %lu bytes/sec.\n"), p2, p2 * 100 / s2);
				break;

			case 'v' :	/* fv - Truncate file */
				put_rc(f_truncate(&file1));
				break;

			case 'n' :	/* fn <old_name> <new_name> - Change file/dir name */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == ' ') ptr2++;
				put_rc(f_rename(ptr, ptr2));
				break;

			case 'u' :	/* fu <name> - Unlink a file or dir */
				while (*ptr == ' ') ptr++;
				put_rc(f_unlink(ptr));
				break;

			case 'k' :	/* fk <name> - Create a directory */
				while (*ptr == ' ') ptr++;
				put_rc(f_mkdir(ptr));
				break;

			case 'a' :	/* fa <atrr> <mask> <name> - Change file/dir attribute */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				while (*ptr == ' ') ptr++;
				put_rc(f_chmod(ptr, p1, p2));
				break;

			case 't' :	/* ft <year> <month> <day> <hour> <min> <sec> <name> */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				finfo.fdate = ((p1 - 1980) << 9) | ((p2 & 15) << 5) | (p3 & 31);
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				finfo.ftime = ((p1 & 31) << 11) | ((p1 & 63) << 5) | ((p1 >> 1) & 31);
				put_rc(f_utime(ptr, &finfo));
				break;

			case 'x' : /* fx <src_name> <dst_name> - Copy file */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				xprintf(PSTR("Opening \"%s\""), ptr);
				res = f_open(&file1, ptr, FA_OPEN_EXISTING | FA_READ);
				if (res) {
					put_rc(res);
					break;
				}
				xprintf(PSTR("\nCreating \"%s\""), ptr2);
				res = f_open(&file2, ptr2, FA_CREATE_ALWAYS | FA_WRITE);
				if (res) {
					put_rc(res);
					f_close(&file1);
					break;
				}
				xprintf(PSTR("\nCopying..."));
				p1 = 0;
				for (;;) {
					res = f_read(&file1, Buff, sizeof(Buff), &s1);
					if (res || s1 == 0) break;   /* error or eof */
					res = f_write(&file2, Buff, s1, &s2);
					p1 += s2;
					if (res || s2 < s1) break;   /* error or disk full */
				}
				if (res) put_rc(res);
				xprintf(PSTR("\n%lu bytes copied.\n"), p1);
				f_close(&file1);
				f_close(&file2);
				break;
#if _USE_MKFS
			case 'm' :	/* fm <logi drv#> <part type> <bytes/clust> - Create file system */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				xprintf(PSTR("The drive %u will be formatted. Are you sure? (Y/n)="), (WORD)p1);
				get_line(ptr, sizeof(text));
				if (*ptr == 'Y') put_rc(f_mkfs((BYTE)p1, (BYTE)p2, (WORD)p3));
				break;
#endif
			}
			break;

		case 't' :	/* t [<year> <mon> <mday> <hour> <min> <sec>] */
			if (xatoi(&ptr, &p1)) {
				rtc.year = (WORD)p1;
				xatoi(&ptr, &p1); rtc.month = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.mday = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.hour = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.min = (BYTE)p1;
				if (!xatoi(&ptr, &p1)) break;
				rtc.sec = (BYTE)p1;
				rtc_settime(&rtc);
			}
			rtc_gettime(&rtc);
			xprintf(PSTR("%u/%u/%u %02u:%02u:%02u\n"), rtc.year, rtc.month, rtc.mday, rtc.hour, rtc.min, rtc.sec);
			break;
		}
	}
    return 0;
}

