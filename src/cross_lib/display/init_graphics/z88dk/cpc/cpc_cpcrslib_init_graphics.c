

#define UDG_N 20


char char_list[UDG_N*2] = 
{ 
33, '\0', // TILE_0
37, '\0', // TILE_1
44, '\0', // TILE_2
40, '\0', // TILE_3
43, '\0', // TILE_4
42, '\0', // TILE_5
41, '\0', // TILE_6
49, '\0', // TILE_7
46, '\0', // TILE_8
33, '\0', // TILE_9
39, '\0', // TILE_10
38, '\0', // TILE_11
45, '\0', // TILE_12
33, '\0', // TILE_13
34, '\0', // TILE_14
35, '\0', // TILE_15
36, '\0', // TILE_16
47, '\0', // TILE_17
48, '\0' // TILE_18
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

void INIT_GRAPHICS(void)
{
	set_palette();
}
