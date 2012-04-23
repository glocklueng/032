#ifdef WIN32
#include <windows.h>
#include <setupapi.h>
#include <ctype.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
BOOL UsbConnect(void);
#else
#include <usb.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "prox.h"
#ifndef WIN32
#include "proxmark3.h"
#endif
#include "flash.h"
#include "elf.h"

static uint32_t ExpectedAddr;
static uint8_t QueuedToSend[256];
static bool AllWritten;
#define PHYSICAL_FLASH_START 0x100000
#define PHYSICAL_FLASH_END   0x200000

void WaitForAck(void) {
	UsbCommand ack;
	ReceiveCommand(&ack);
	if(ack.cmd != CMD_ACK) {
		printf("bad ACK\n");
		exit(-1);
	}
}

struct partition partitions[] = {
		{0x100000, 0x102000, 1, "bootrom"},
		{0x102000, 0x12C000, 0, "fpga"},
		{0x12C000, 0x140000, 0, "os"},
		{0, 0, 0, NULL},
};

/* If translate is set, subtract PHYSICAL_FLASH_START to translate for old
 * bootroms.
 */
void WriteBlock(unsigned int block_start, unsigned int len, unsigned char *buf)
{
	unsigned char temp_buf[256];
	if (block_start & 0xFF) {
		printf("moving stuff forward by %d bytes\n", block_start & 0xFF);
		memset(temp_buf, 0xFF, block_start & 0xFF);
		memcpy(temp_buf + (block_start & 0xFF), buf, len);
		block_start &= ~0xFF;
	} else memcpy(temp_buf, buf, len);
	
	UsbCommand c = {CMD_SETUP_WRITE};

//	printf("expected = %08x flush, ", ExpectedAddr);

	int i;
	for(i = 0; i < 240; i += 48) {
		memcpy(c.d.asBytes, temp_buf+i, 48);
		c.arg[0] = (i/4);
		SendCommand(&c);
		WaitForAck();
	}

	c.cmd = CMD_FINISH_WRITE;
	c.arg[0] = block_start;

	printf("writing block %08x\r", c.arg[0]);
	memcpy(c.d.asBytes, temp_buf+240, 16);
	SendCommand(&c);
	WaitForAck();

	AllWritten = true;
}

void LoadFlashFromFile(const char *file, int start_addr, int end_addr)
{
	FILE *f = fopen(file, "rb");
	if(!f) {
		printf("couldn't open file\n");
		exit(-1);
	}

	char buf[4];
	fread(buf, 1, 4, f);
	if (memcmp(buf, "\x7F" "ELF", 4) == 0) {
		int i;
		fseek(f, 0, SEEK_SET);
		Elf32_Ehdr header;
		fread(&header, 1, sizeof(header), f);
		int count = header.e_phnum;
		printf("count=%d phoff=%x\n", count, header.e_phoff);
		Elf32_Phdr phdr;
		
		for (i=0; i<header.e_phnum; i++) {
			fseek(f, header.e_phoff + i * sizeof(Elf32_Phdr), SEEK_SET);
			fread(&phdr, 1, sizeof(phdr), f);
//			printf("type=%d offset=%x paddr=%x vaddr=%x filesize=%x memsize=%x flags=%x align=%x\n",
//				phdr.p_type, phdr.p_offset, phdr.p_paddr, phdr.p_vaddr, phdr.p_filesz, phdr.p_memsz, phdr.p_flags, phdr.p_align);
			if (phdr.p_type == PT_LOAD && phdr.p_filesz > 0 && phdr.p_paddr >= PHYSICAL_FLASH_START
				&& (phdr.p_paddr + phdr.p_filesz) < PHYSICAL_FLASH_END) {
				printf("flashing offset=%x paddr=%x size=%x\n", phdr.p_offset, phdr.p_paddr, phdr.p_filesz);
				if (phdr.p_offset == 0) {
					printf("skipping forward 0x2000 because of strange linker thing\n");
					phdr.p_offset += 0x2000;
					phdr.p_paddr += 0x2000;
				}

				fseek(f, phdr.p_offset, SEEK_SET);
				ExpectedAddr = phdr.p_paddr;
				while (ExpectedAddr < (phdr.p_paddr + phdr.p_filesz)) {
					unsigned int bytes_to_read = phdr.p_paddr + phdr.p_filesz - ExpectedAddr;
					if (bytes_to_read > 256) bytes_to_read=256;
					fread(QueuedToSend, 1, bytes_to_read, f);
//					printf("read %d bytes\n", bytes_to_read);
					printf("WriteBlock(%x, %d, %p)\n", ExpectedAddr, bytes_to_read, QueuedToSend);
					WriteBlock(ExpectedAddr, bytes_to_read, QueuedToSend);
					ExpectedAddr += bytes_to_read;
				}
            }
        }

		fclose(f);
		printf("\ndone.\n");
		return;
	} else printf("Bad file format\n");		
}

int PrepareFlash(struct partition *p, const char *filename, unsigned int state)
{
	if(state & DEVICE_INFO_FLAG_UNDERSTANDS_START_FLASH) {
		UsbCommand c = {CMD_START_FLASH, {p->start, p->end, 0}};
		
		/* Only send magic when flashing bootrom */
		if(p->precious)
			c.arg[2] = START_FLASH_MAGIC;
		else
			c.arg[2] = 0;

		SendCommand(&c);
		WaitForAck();
	} else {
		fprintf(stderr, "Warning: Your bootloader does not understand the new START_FLASH command\n");
		fprintf(stderr, "         It is recommended that you update your bootloader\n\n");
		exit(0);
	}
	
	LoadFlashFromFile(filename, p->start, p->end);
	return 1;
}

unsigned int GetProxmarkState(void)
{
	unsigned int state = 0;
	
	UsbCommand c;
	c.cmd = CMD_DEVICE_INFO;
	SendCommand(&c);
	
	UsbCommand resp;
	ReceiveCommand(&resp);
	/* Three cases: 
	 * 1. The old bootrom code will ignore CMD_DEVICE_INFO, but respond with an ACK
	 * 2. The old os code will respond with CMD_DEBUG_PRINT_STRING and "unknown command"
	 * 3. The new bootrom and os codes will respond with CMD_DEVICE_INFO and flags
	 */
	
	switch(resp.cmd) {
	case CMD_ACK:
		state = DEVICE_INFO_FLAG_CURRENT_MODE_BOOTROM;
		break;
	case CMD_DEBUG_PRINT_STRING:
		state = DEVICE_INFO_FLAG_CURRENT_MODE_OS;
		break;
	case CMD_DEVICE_INFO:
		state = resp.arg[0];
		break;
	default:
		fprintf(stderr, "Couldn't get proxmark state, bad response type: 0x%04X\n", resp.cmd);
		exit(-1);
		break;
	}
	
	return state;
}

unsigned int EnterFlashState(void)
{
	unsigned int state = GetProxmarkState();
	
	if(state & DEVICE_INFO_FLAG_CURRENT_MODE_BOOTROM) {
		/* Already in flash state, we're done. */
		return state;
	}
	
	if(state & DEVICE_INFO_FLAG_CURRENT_MODE_OS) {
		fprintf(stderr,"Entering flash-mode...\n");
		UsbCommand c;
		bzero(&c, sizeof(c));
		
		if( (state & DEVICE_INFO_FLAG_BOOTROM_PRESENT) && (state & DEVICE_INFO_FLAG_OSIMAGE_PRESENT) ) {
			/* New style handover: Send CMD_START_FLASH, which will reset the board and
			 * enter the bootrom on the next boot.
			 */
			c.cmd = CMD_START_FLASH;
			SendCommand(&c);
			fprintf(stderr,"(You don't have to do anything. Press and release the button only if you want to abort)\n");
			fprintf(stderr,"Waiting for Proxmark to reappear on USB... ");
		} else {
			/* Old style handover: Ask the user to press the button, then reset the board */
			c.cmd = CMD_HARDWARE_RESET;
			SendCommand(&c);
			fprintf(stderr,"(Press and hold down button NOW if your bootloader requires it)\n");
			fprintf(stderr,"Waiting for Proxmark to reappear on USB... ");
		}

#ifdef WIN32
		Sleep(1000);
		while(!UsbConnect()) { Sleep(1000); }
#else
		CloseProxmark();
		sleep(1);

		while(!(devh=OpenProxmark(0))) { sleep(1); }
#endif
		fprintf(stderr,"Found.\n");

		return GetProxmarkState();
	}
	
	return 0;
}

/* On first call, have *offset = -1, *length = 0; */
int find_next_area(const char *str, int *offset, int *length)
{
	if(*str == '\0') return 0;
	if((*offset >= 0) && str[*offset + *length] == '\0') return 0;
	*offset += 1 + *length;
	
	char *next_comma = strchr(str + *offset, ',');
	if(next_comma == NULL) {
		*length = strlen(str) - *offset;
	} else {
		*length = next_comma-(str+*offset);
	}
	return 1;
}

void do_flash(char **argv) {
	unsigned int state = EnterFlashState();

	if (!(state & DEVICE_INFO_FLAG_CURRENT_MODE_BOOTROM)) {
		fprintf(stderr, "Proxmark would not enter flash state, abort\n");
		exit(-1);
	}

	int offset=-1, length=0;
	int current_area = 0;
	while(find_next_area(argv[1], &offset, &length)) {
		int i;
		struct partition *p = NULL;
		for (i=0; i<sizeof(partitions)/sizeof(partitions[0]); i++) {
			if (strncmp(partitions[i].name, argv[1] + offset, length) == 0) {
				/* Check if the name matches the bootrom partition, and if so, require "bootrom" to
			 	* be written in full. The other names may be abbreviated.
			 	*/
				if(!partitions[i].precious || (strlen(partitions[i].name) == length))
					p = &partitions[i];
				break;
			}
		}

		if(p == NULL) {
			fprintf(stderr, "Warning: area name '");
			fwrite(argv[1]+offset, length, 1, stderr);
			fprintf(stderr, "' unknown, ignored\n");
		} else {
			fprintf(stderr, "Flashing %s from %s\n", p->name, argv[2+current_area]);
			PrepareFlash(p, argv[2+current_area], state);
		}
		current_area++;
	}
}
