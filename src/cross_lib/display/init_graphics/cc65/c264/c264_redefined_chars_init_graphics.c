#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../graphics_settings.h"

#include "../../../graphics_data/8x8_chars.h"


// #if defined(REDEFINED_CHARS)
// struct redefine_struct
// {
   // unsigned char ascii;
   // unsigned char bitmap[8];
// } ;
// #endif


// #if defined(REDEFINED_CHARS)
// struct redefine_struct redefine_map[] =
// {
	// {_PLAYER_DOWN, _PLAYER_DOWN_UDG},
	// {_PLAYER_UP, _PLAYER_UP_UDG},
	// {_PLAYER_RIGHT, _PLAYER_RIGHT_UDG},	
	// {_PLAYER_LEFT, _PLAYER_LEFT_UDG},
	// {_GHOST, _GHOST_UDG},
	// {_SKULL, _SKULL_UDG},
	// {_GUN, _GUN_UDG},
	// {_POWERUP, _POWERUP_UDG},
	// {_BULLET, _BULLET_UDG},
	// {_BOMB, _BOMB_UDG},
	// {_VERTICAL_BRICK, _VERTICAL_BRICK_UDG},
	// {_HORIZONTAL_BRICK, _HORIZONTAL_BRICK_UDG},	
	// #if defined(FULL_GAME)
		// {_RIGHT_HORIZONTAL_MISSILE, _RIGHT_HORIZONTAL_MISSILE_UDG},
		// {_LEFT_HORIZONTAL_MISSILE, _LEFT_HORIZONTAL_MISSILE_UDG},	
		// {_ROCKET, _ROCKET_UDG},
		// {_INVINCIBILITY, _INVINCIBILITY_UDG},
	// #endif
// };
// #endif


#include "../../../redefine_characters/udg_map.h"

void INIT_GRAPHICS(void)
{
	unsigned short i;
	
	POKE(1177,62); // disable switch to RAM in PEEK
	for(i=0;i<1023;++i)
	{
		POKE(UDG_BASE_FACTOR*1024+i,PEEK((unsigned long) (54272ul+(unsigned long) i)));
	}
	POKE(1177,63); // re-enable switch to RAM in PEEK
	POKE(65299ul,(PEEK(65299ul)&3)|((UDG_BASE_FACTOR)*4)); // change character base address to 28th Kbyte
	POKE(65298ul,PEEK(65298ul)&251); // make graphics chip get characters from RAM
		
	for(i=0;i<sizeof(redefine_map)/sizeof(*redefine_map);++i)
	{
		memcpy((unsigned char *)(UDG_BASE_FACTOR*1024 + (redefine_map[i].ascii)*8), redefine_map[i].bitmap, 8);		
	}
}

