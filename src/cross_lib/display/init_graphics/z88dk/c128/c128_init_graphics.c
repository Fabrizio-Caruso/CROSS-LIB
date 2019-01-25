#include <graphics.h>
#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))

void INIT_GRAPHICS(void)
{
	clg();
	
	// Disable VIC to get a little speed-up (about 7%)
	__asm
	ld a,0
	ld bc,$d011
	out (c),a
	__endasm;
}