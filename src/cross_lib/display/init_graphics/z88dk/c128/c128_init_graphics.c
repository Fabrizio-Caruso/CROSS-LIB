#include <graphics.h>
#include "display_macros.h"

#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

void _XL_INIT_GRAPHICS(void)
{
	clg();
	
	// Disable VIC-IIe to get a little speed-up (about 7%)
    /*
	__asm
    ; Disable VIC-IIe chip
	ld a,0
	ld bc,$d011
	out (c),a
    
    ; Set fast mode
    ld a,1
    ld bc,$d030
    out (c),a
	__endasm;
    */
    
    _setScreenColors();
}
