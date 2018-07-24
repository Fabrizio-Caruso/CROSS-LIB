#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../graphics_settings.h"

#include "../../../graphics_data/6x8_chars.h"

#define UDG_BASE_ADDR 0xb400

void init_colors(void)
{
	unsigned char i;
	
	// Initialize colors 	
	for(i=0;i<2;++i)
	{
		// red on black (inverted: cyan on white)		
		POKE(0xBB80+i*40,16);
		POKE(0xBB81+i*40,1); 
	}

	for(i=2;i<28;++i)
	{
		// yellow on black (inverted: blue on white)
		POKE(0xBB80+i*40,16);
		POKE(0xBB81+i*40,3); 
	}		
}

#include "../../../redefine_characters/udg_map.h"

// struct redefine_struct
// {
   // unsigned char ascii;
   // unsigned char bitmap[8];
// } ;


// struct redefine_struct redefine_map[] =
// {
	// {_PLAYER_DOWN_OFFSET, _PLAYER_DOWN_UDG},
	// {_PLAYER_UP_OFFSET, _PLAYER_UP_UDG},
	// {_PLAYER_RIGHT_OFFSET, _PLAYER_RIGHT_UDG},	
	// {_PLAYER_LEFT_OFFSET, _PLAYER_LEFT_UDG},
	// {_GHOST_OFFSET, _GHOST_UDG},
	// {_BOMB_OFFSET, _BOMB_UDG},
	
	// #if defined(FULL_GAME)
	// {_RIGHT_HORIZONTAL_MISSILE_OFFSET, _RIGHT_HORIZONTAL_MISSILE_UDG},
	// {_LEFT_HORIZONTAL_MISSILE_OFFSET, _LEFT_HORIZONTAL_MISSILE_UDG},
	// #endif
	
	// #if !defined(TINY_GAME)
	// {_SKULL_OFFSET, _SKULL_UDG},
	// {_GUN_OFFSET, _GUN_UDG},
	// {_POWERUP_OFFSET, _POWERUP_UDG},
	// {_BULLET_OFFSET, _BULLET_UDG},
	// {_ROCKET_OFFSET, _ROCKET_UDG},
	// {_INVINCIBILITY_OFFSET, _INVINCIBILITY_UDG},
	// {_VERTICAL_BRICK_OFFSET, _VERTICAL_BRICK_UDG},
	// {_HORIZONTAL_BRICK_OFFSET, _HORIZONTAL_BRICK_UDG},	
	// #endif
	
	// #if !defined(NO_DEAD_GHOST)
	// {_DEAD_GHOST_OFFSET, _DEAD_GHOST_UDG}
	// #endif
// };



void INIT_GRAPHICS(void)
{		
	unsigned char i;

	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i)
	{
		memcpy((unsigned char *)UDG_BASE_ADDR + (redefine_map[i].ascii)*8, redefine_map[i].bitmap, 8);
	}

	init_colors();
}
