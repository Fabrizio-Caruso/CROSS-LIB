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


#include <stdio.h>

#include <msx/gfx.h>
extern unsigned char XSize;

#define BASE 0

#define _DRAW(x,y,image) msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,image->_imageData-32);
#define _DELETE(x,y)     msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,0); // ' '
#define _DRAW_VERTICAL_WALL(x,y)  msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,'|'-32);
#define _DRAW_HORIZONTAL_WALL(x,y)  msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,'-'-32);
#define _DRAW_BROKEN_WALL(x,y)  msx_vpoke(BASE+x+X_OFFSET+(y-1+Y_OFFSET)*XSize,56); // 'X'	


#include "../display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image MISSILE_IMAGE;

extern Image POWERUP_IMAGE;
extern Image FREEZE_IMAGE;


extern Image GUN_IMAGE;
extern Image EXTRA_POINTS_IMAGE;

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	
	extern Image BROKEN_WALL_IMAGE;
#endif


void INIT_GRAPHICS(void)
{	set_color(15, 1, 1);
}

void INIT_IMAGES(void)
{		
	#if !defined(NO_COLOR)
		PLAYER_IMAGE._color = COLOR_WHITE;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
		POWERUP_IMAGE._color = COLOR_WHITE;
		FREEZE_IMAGE._color = COLOR_WHITE;	
		
		GUN_IMAGE._color = COLOR_WHITE;
		EXTRA_POINTS_IMAGE._color = COLOR_WHITE;	
		
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_WHITE;
	#endif
	
	GHOST_IMAGE._imageData = 'o';
	INVINCIBLE_GHOST_IMAGE._imageData = '+';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	POWERUP_IMAGE._imageData = 'S';
	FREEZE_IMAGE._imageData = 'F';
	
	GUN_IMAGE._imageData = '!';
	EXTRA_POINTS_IMAGE._imageData = '$';
	
	MISSILE_IMAGE._imageData = '.';
	DEAD_GHOST_IMAGE._imageData = 'O';

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';

		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';

		BUBBLE_IMAGE._imageData = '^';
				
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'V';
		
		#if !defined(NO_COLOR)
			LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
			RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
			BUBBLE_IMAGE._color = COLOR_WHITE;				
			EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
			INVINCIBILITY_IMAGE._color = COLOR_WHITE;	
		#endif
	#endif
}


void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	if(*blinkCounter) 
	{
		_DRAW(x,y,image);
		*blinkCounter=0;
	} 
	else 
	{
		_DELETE(x,y);
		*blinkCounter=1;
	}	
}

void _draw(unsigned char x, unsigned char y, Image * image) 
{
	_DRAW(x,y,image);
}

void _delete(unsigned char x, unsigned char y)
{
	_DELETE(x,y);
}


#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_DRAW_BROKEN_WALL(x,y);
	}
#endif
	

#if !defined(TINY_GAME)
	void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	{ 
		unsigned char i;
		for(i=0;i<length;++i)
		{
			_DRAW_VERTICAL_WALL(x,y+i);
		}
	}

	void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	{
		unsigned char i;
		for(i=0;i<length;++i)
		{
			_DRAW_HORIZONTAL_WALL(x+i,y);
		}
	}
#endif
	

