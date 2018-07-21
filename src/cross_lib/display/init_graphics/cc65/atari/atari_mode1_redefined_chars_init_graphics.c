/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include <conio.h>
#include <peekpoke.h>
#include <atari.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../graphics_settings.h"

#include "../../../graphics_data/8x8_chars.h"

#define GRAPHICS_MODE (1+16)
	
	
void redefine(unsigned char * loc, const char *new_char)
{
	unsigned char i;
	for(i=0;i<8;++i)
	{
		POKE(loc+i,new_char[i]);
	}
}


void set_udg(void)
{	
	static const char player_down[8] =      _PLAYER_DOWN_UDG;
	static const char player_up[8] =        _PLAYER_UP_UDG;
	static const char player_right[8] =     _PLAYER_RIGHT_UDG;	
	static const char player_left[8] =      _PLAYER_LEFT_UDG;
	static const char ghost[8] =            _GHOST_UDG;
	static const char missile_right[8] =    _RIGHT_HORIZONTAL_MISSILE_UDG;
	static const char missile_left[8] =     _LEFT_HORIZONTAL_MISSILE_UDG;
	static const char invincible_ghost[8] = _SKULL_UDG;
	static const char gun[8] =              _GUN_UDG;
	static const char powerUp[8] =          _POWERUP_UDG;
	static const char bullet[8] =           _BULLET_UDG;
	static const char bomb[8] =             _BOMB_UDG;
	static const char rocket[8] =           _ROCKET_UDG;
	static const char invincibility[8] =    _INVINCIBILITY_UDG;	
	static const char vertical_brick[8] =   _VERTICAL_BRICK_UDG;
	static const char horizontal_brick[8] = _HORIZONTAL_BRICK_UDG;
	
	extern char _FONT_START__[];
	unsigned char *CHBAS = (unsigned char *)0x2f4;

	memcpy(_FONT_START__, (void *)0xE000, 512);
	
	/* modify your font at _FONT_START__, etc, then set the new font: */		
		
	redefine(_FONT_START__+_PLAYER_DOWN_OFFSET*8, player_down);
	redefine(_FONT_START__+_PLAYER_UP_OFFSET*8, player_up);		
	redefine(_FONT_START__+_PLAYER_RIGHT_OFFSET*8, player_right);
	redefine(_FONT_START__+_PLAYER_LEFT_OFFSET*8, player_left);

	redefine(_FONT_START__+_GHOST_OFFSET*8, ghost);
	redefine(_FONT_START__+_POWERUP_OFFSET*8, powerUp);		
	redefine(_FONT_START__+_SKULL_OFFSET*8, invincible_ghost);
	redefine(_FONT_START__+_BOMB_OFFSET*8, bomb);	
				
	redefine(_FONT_START__+_BULLET_OFFSET*8, bullet);
	redefine(_FONT_START__+_GUN_OFFSET*8, gun);

	#if defined(FULL_GAME)
		redefine(_FONT_START__+_LEFT_HORIZONTAL_MISSILE_OFFSET*8, missile_left);
		redefine(_FONT_START__+_RIGHT_HORIZONTAL_MISSILE_OFFSET*8, missile_right);		
		redefine(_FONT_START__+_ROCKET_OFFSET*8, rocket);
		redefine(_FONT_START__+_INVINCIBILITY_OFFSET*8, invincibility);				
	#endif

	redefine(_FONT_START__+_HORIZONTAL_BRICK_OFFSET*8, horizontal_brick);
	redefine(_FONT_START__+_VERTICAL_BRICK_OFFSET*8, vertical_brick);
		
	*CHBAS = ((int)_FONT_START__ >> 8);  /* enable the new font */	
	
}
 
void INIT_GRAPHICS(void)
{
	// Mode 12 with no last monochromatic lines (12+16)
	_graphics(GRAPHICS_MODE);
	
	_setcolor_low(0, TGI_COLOR_RED);
	_setcolor_low(1, TGI_COLOR_WHITE);
	_setcolor_low(2, TGI_COLOR_CYAN); 	
	_setcolor_low(3, TGI_COLOR_BROWN);
	_setcolor_low(4, TGI_COLOR_BLACK);

	set_udg();
}