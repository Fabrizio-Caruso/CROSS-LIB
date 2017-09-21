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
 
//#include <peekpoke.h>
#include "display_macros.h"

#include <stdio.h>
#if defined(__MSX__)
	#include <msx/gfx.h>#endif
	
#define _PLAYER '*'
#define _PLAYER_DOWN '*'
#define _PLAYER_UP ((unsigned char) 0x2C)
#define _PLAYER_RIGHT ((unsigned char)0x2D)
#define _PLAYER_LEFT ((unsigned char)0x2E)

#define _GHOST ((unsigned char)0x3A)
#define _INVINCIBLE_GHOST '+'
#define _BOMB ((unsigned char)0x3B)
#define _POWERUP ((unsigned char)28) // S
#define _GUN ((unsigned char)0x7F)
#define _MISSILE ((unsigned char)0x27)

#define _LEFT_ENEMY_MISSILE ((unsigned char)0x3C)
#define _RIGHT_ENEMY_MISSILE ((unsigned char)0x3E)
#define _BUBBLE ((unsigned char)0x7E)
	
extern unsigned char XSize;

Image PLAYER_DOWN;
Image PLAYER_UP;
Image PLAYER_RIGHT;
Image PLAYER_LEFT;

#if defined(MSX_MODE1)
	#define BASE 6144
	#define CHAR_BASE 0x0100
#else
	#define BASE 0
	#define CHAR_BASE 
#endif


#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))


#define _DRAW(x,y,image) msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,image->_imageData);
#define _DELETE(x,y)     msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,' ');
#define _DRAW_VERTICAL_WALL(x,y)  msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,'|');
#define _DRAW_HORIZONTAL_WALL(x,y)  msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,'-');	
#define _DRAW_BROKEN_WALL(x,y)  msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,'X');	

void redefine(unsigned char * loc, const char *new_char)
{
	unsigned char i;
	for(i=0;i<8;++i)
	{
		msx_vpoke((unsigned char *)(loc+i),new_char[i]);
	}
}

void INIT_GRAPHICS(void)
{
	static const char player_down[8] =      { 24, 36, 24,102,153, 24, 36,102};
	static const char player_up[8] =        { 24, 60, 24,102,153, 24, 36,102};
	static const char player_right[8] =     { 24, 52, 25,118,152, 24, 20, 20};	
	static const char player_left[8] =      { 24, 44,152,110, 25, 24, 40, 40};
	static const char ghost[8] =            {129,126,165,129,129,189,129,126};
	static const char invincible_ghost[8] = { 60, 66,165,129, 90, 36, 36, 60};
	static const char gun[8] =              {  0,128,126,200,248,192,128,  0};
	static const char powerUp[8] =          {  0, 60, 54,223,231,122, 36, 24};
	static const char missile[8] =          {  0,  0,  8, 56, 28, 16,  0,  0};
	static const char bomb[8] =             { 60, 66,165,153,153,165, 66, 60};
	
	static const char bubble[8] =           { 24, 60, 60, 60,126, 90, 66, 66};	
	static const char missile_right[8] =    {  0,  0, 15,252,252, 15,  0,  0};
	static const char missile_left[8] =     {  0,  0,240, 63, 63,240,  0,  0};	
		set_color(15, 1, 1);
	#if defined(MSX_MODE1)
		set_mode(mode_1);

		msx_vpoke(8192+ 4,10*16); // White !, $ -- 32 - 39
		msx_vpoke(8192+ 5, 2*16); // Green  -- 40 - 47
		msx_vpoke(8192+ 6, 4*16); // Green -- 48 - 55
		msx_vpoke(8192+ 7, 4*16); // Green -- 56 - 63
		
		msx_vpoke(8192+ 8, 8*16); // Red 64 --
		msx_vpoke(8192+ 9, 8*16); // Red 72 --
		msx_vpoke(8192+10, 8*16); // Red 80 --
		msx_vpoke(8192+11, 8*16); // Red 88 --
	#endif
	
	redefine((unsigned char *)(CHAR_BASE+8*_PLAYER),player_down);
	redefine(CHAR_BASE+8*_PLAYER_DOWN,player_down);
	redefine(CHAR_BASE+8*_PLAYER_UP,player_up);	
	redefine(CHAR_BASE+8*_PLAYER_LEFT,player_left);	
	redefine(CHAR_BASE+8*_PLAYER_RIGHT, player_right);	
	
	redefine(CHAR_BASE+8*_BOMB, bomb);
	redefine(CHAR_BASE+8*_GHOST,ghost);
	redefine(CHAR_BASE+8*_INVINCIBLE_GHOST,invincible_ghost);	
	redefine(CHAR_BASE+8*_MISSILE,missile);	
	redefine(CHAR_BASE+8*_POWERUP, powerUp);	

	#if defined(FULL_GAME)
		redefine(CHAR_BASE+8*_LEFT_ENEMY_MISSILE,missile_left);	
		redefine(CHAR_BASE+8*_RIGHT_ENEMY_MISSILE,missile_right);	
		redefine(CHAR_BASE+8*_BUBBLE, bubble);
	#endif
}

void INIT_IMAGES(void)
{		

	PLAYER_IMAGE._color = COLOR_WHITE;
	INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
	POWERUP_IMAGE._color = COLOR_WHITE;
	GUN_IMAGE._color = COLOR_WHITE;
	BOMB_IMAGE._color = COLOR_RED;
	DEAD_GHOST_IMAGE._color = COLOR_RED;
			
	GHOST_IMAGE._imageData = 'o';
	INVINCIBLE_GHOST_IMAGE._imageData = '+';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	POWERUP_IMAGE._imageData = 'S';
	GUN_IMAGE._imageData = '!';
	MISSILE_IMAGE._imageData = '.';
	DEAD_GHOST_IMAGE._imageData = 'O';

	GHOST_IMAGE._color = COLOR_WHITE;
	MISSILE_IMAGE._color = COLOR_WHITE;

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
		
		BUBBLE_IMAGE._imageData = '^';
		BUBBLE_IMAGE._color = COLOR_WHITE;
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'V';
		
		EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
		EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
		INVINCIBILITY_IMAGE._color = COLOR_WHITE;	
		
		PLAYER_DOWN._color = PLAYER_IMAGE._color;
		PLAYER_UP._color = PLAYER_IMAGE._color;	
		PLAYER_RIGHT._color = PLAYER_IMAGE._color;
		PLAYER_LEFT._color = PLAYER_IMAGE._color;		
	#endif
}

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_DRAW_BROKEN_WALL(x,y);
	}
#endif
	
void _draw(unsigned char x, unsigned char y, Image * image) 
{
	_DRAW(x,y,image);
}

void _delete(unsigned char x, unsigned char y)
{
	_DELETE(x,y);
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
		


