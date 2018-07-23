
#include <spectrum.h>
#include <graphics.h>

#include "../../../image_settings/spectrum_redefined_chars_settings.h"
#include "../../../display_macros.h"

#include "../../../graphics_data/8x8_chars.h"


extern char udg[];

struct redefine_struct
{
   unsigned char ascii;
   unsigned char bitmap[8];
} ;


struct redefine_struct redefine_map[] =
{
	{_PLAYER_DOWN, _PLAYER_DOWN_UDG},
	{_PLAYER_UP, _PLAYER_UP_UDG},
	{_PLAYER_RIGHT, _PLAYER_RIGHT_UDG},	
	{_PLAYER_LEFT, _PLAYER_LEFT_UDG},
	{_GHOST, _GHOST_UDG},
	{_BOMB, _BOMB_UDG},
	
	#if defined(FULL_GAME)
	{_RIGHT_HORIZONTAL_MISSILE, _RIGHT_HORIZONTAL_MISSILE_UDG},
	{_LEFT_HORIZONTAL_MISSILE, _LEFT_HORIZONTAL_MISSILE_UDG},
	#endif
	
	#if !defined(TINY_GAME)
	{_SKULL, _SKULL_UDG},
	{_GUN, _GUN_UDG},
	{_POWERUP, _POWERUP_UDG},
	{_BULLET, _BULLET_UDG},
	{_ROCKET, _ROCKET_UDG},
	{_INVINCIBILITY, _INVINCIBILITY_UDG},
	{_VERTICAL_BRICK, _VERTICAL_BRICK_UDG},
	{_HORIZONTAL_BRICK, _HORIZONTAL_BRICK_UDG}		
	#endif
};


void INIT_GRAPHICS(void)
{
	unsigned char i;
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i)
	{
	   memcpy(udg + (redefine_map[i].ascii - 32)*8, redefine_map[i].bitmap, 8);
	}

	clg();
	zx_border(0);
	zx_colour(PAPER_BLACK|INK_WHITE);
	textbackground(COLOR_BLACK);
}


