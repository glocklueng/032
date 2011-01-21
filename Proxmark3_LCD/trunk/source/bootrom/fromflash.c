#include <proxmark3.h>

extern char __bootphase2_src_start__, __bootphase2_start__, __bootphase2_end__;
void 
#ifdef GCC
__attribute__((section(".bootphase1"))) 
#endif
CopyBootToRAM(void)
{
	int i;

	volatile DWORD *s = (volatile DWORD *)&__bootphase2_src_start__;
	volatile DWORD *d = (volatile DWORD *)&__bootphase2_start__;
	unsigned int l = (int)&__bootphase2_end__ - (int)&__bootphase2_start__;

	for(i = 0; i < l/sizeof(DWORD); i++) *d++ = *s++;
}
