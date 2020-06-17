

#define UDG_N 20


char char_list[UDG_N*2] = 
{ 
33, '\0', // PLAYER
37, '\0', // GHOST
44, '\0', // BOMB
40, '\0', // SKULL
43, '\0', // BULLET
42, '\0', // POWERUP
41, '\0', // GUN
49, '\0', // EXTRA_POINTS_IMAGE
46, '\0', // INVINCIBILITY
33, '\0', // EXTRA LIFE
39, '\0', // LEFT_HORIZONTAL_MISSILE
38, '\0', // RIGHT_HORIZONTAL_MISSILE
45, '\0', // ROCKET
33, '\0', // PLAYER_DOWN
34, '\0', // PLAYER_UP
35, '\0', // PLAYER_RIGHT
36, '\0', // PLAYER_LEFT
47, '\0', // VERTICAL_BRICK
48, '\0' // HORIZONTAL_BRICK 
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
