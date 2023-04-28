
#include "display_macros.h"

#define UDG_N 26


char char_list[] = 
{ 
33, '\0', // TILE_
37, '\0', // TILE_
44, '\0', // TILE_
40, '\0', // TILE_
43, '\0', // TILE_
42, '\0', // TILE_8
41, '\0', // TILE_
49, '\0', // TILE_
46, '\0', // TILE_
52, '\0', // TILE_
39, '\0', // TILE_
38, '\0', // TILE_
45, '\0', // TILE_
33, '\0', // TILE_
34, '\0', // TILE_
35, '\0', // TILE_
36, '\0', // TILE_
47, '\0', // TILE_12
48, '\0', // TILE_13
50, '\0',
51, '\0',
122, '\0',
123, '\0',
124, '\0',
125, '\0',
126, '\0',
127, '\0',
128, '\0'
};

char space_str[2] = {' ', '\0'};

void set_palette()
{ 
#asm
    EXTERN firmware

	ld bc,$0000
    call firmware
	defw 0xbc38	
 
    ld  a,$00
    ld  bc,$0000
    call firmware
    defw 0xbc32
	
    ld  a,$02
    ld  bc,$1a1a
    call firmware
    defw 0xbc32
	
    ld  a,$03
    ld  bc,$0606
    call firmware
    defw 0xbc32
	
    ld  a,$01
    ld  bc,$1818
    call firmware
    defw 0xbc32
#endasm 
}

void _XL_INIT_GRAPHICS(void)
{
	set_palette();

    _setScreenColors();
}

