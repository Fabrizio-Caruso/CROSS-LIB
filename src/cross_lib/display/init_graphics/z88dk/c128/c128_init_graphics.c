#include <graphics.h>
#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))

void INIT_GRAPHICS(void)
{
	clg();
	
	__asm
	ld a,0
	ld bc,$d011
	out (c),a
	__endasm;
}