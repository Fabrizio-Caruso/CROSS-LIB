
#include <msx/gfx.h>

#define CHAR_BASE 0x0100
#define COLOR_DEF 0x2000

#include "../../../image_settings/msx_redefined_chars_settings.h"

#include "../../../graphics_data/8x8_chars.h"

void redefine(unsigned short offset, const char *new_char)
{
	unsigned char i;
	
	for(i=0;i<8;++i)
	{
		msx_vpoke(CHAR_BASE+(offset<<3)+i-32*8,new_char[i]);
	}
}

void set_group_color(unsigned char group, unsigned char color)
{
	msx_vpoke(COLOR_DEF+group, color<<4);
}


void set_udg_colors(void)
{
	unsigned char i;
	
	set_group_color(0,9);
	set_group_color(1,7);
	set_group_color(2,9);
	set_group_color(3,11);
	set_group_color(4,10);
	set_group_color(5,2);
	set_group_color(6,4);
	set_group_color(7,4);
	
	for(i=8;i<=11;++i)
	{
		set_group_color(i,8);
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
	// {_PLAYER_DOWN, _PLAYER_DOWN_UDG},
	// {_PLAYER_UP, _PLAYER_UP_UDG},
	// {_PLAYER_RIGHT, _PLAYER_RIGHT_UDG},	
	// {_PLAYER_LEFT, _PLAYER_LEFT_UDG},
	// {_GHOST, _GHOST_UDG},
	// {_BOMB, _BOMB_UDG},
	
	// #if defined(FULL_GAME)
	// {_RIGHT_HORIZONTAL_MISSILE, _RIGHT_HORIZONTAL_MISSILE_UDG},
	// {_LEFT_HORIZONTAL_MISSILE, _LEFT_HORIZONTAL_MISSILE_UDG},
	// #endif
	
	// #if !defined(TINY_GAME)
	// {_SKULL, _SKULL_UDG},
	// {_GUN, _GUN_UDG},
	// {_POWERUP, _POWERUP_UDG},
	// {_BULLET, _BULLET_UDG},
	// {_ROCKET, _ROCKET_UDG},
	// {_INVINCIBILITY, _INVINCIBILITY_UDG},
	// {_VERTICAL_BRICK, _VERTICAL_BRICK_UDG},
	// {_HORIZONTAL_BRICK, _HORIZONTAL_BRICK_UDG},		
	// #endif	
	
	// #if !defined(NO_DEAD_GHOST)
	// {_DEAD_GHOST, _DEAD_GHOST_UDG},
	// #endif
	
	// {_EXTRA_POINTS, _EXTRA_POINTS_UDG}
// };


void set_udg_images(void)
{
	unsigned char i;
	
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i)
	{
	   redefine(redefine_map[i].ascii, redefine_map[i].bitmap);
	}
}

void INIT_GRAPHICS(void)
{
	set_color(15, 1, 1);	
	set_mode(mode_1);
	
	set_udg_colors();
	set_udg_images();
}
