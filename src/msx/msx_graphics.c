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
#if defined(__MSX__)
	#include <msx/gfx.h>#endif
	
// BLUE
#define _PLAYER 0x3B
#define _PLAYER_DOWN 0x3B
#define _PLAYER_UP 0x3C
#define _PLAYER_RIGHT 0x3D 
#define _PLAYER_LEFT 0x3E

#define _GUN 0x3F

// YELLOW
#define _INVINCIBLE_GHOST 0x27
#define _VERTICAL_BRICK 0x26
#define _HORIZONTAL_BRICK 0x24
// #define _EXTRA_LIFE 0x25
#define _EXTRA_POINTS 0x22
#define _ZOMBIE 0x28

// GREEN
#define _POWERUP  0x2C

// RED
#define _BOMB 0x5E
#define _DEAD_GHOST 0x5B
#define _SUPER 0x5C
#define _CONFUSE 0x5A
#define _EXTRA_LIFE 0x5D

// #define _MISSILE 0x5F

// CYAN
#define _INVINCIBILITY 0x08
#define _MISSILE 0x09
#define _FREEZE 0x0A

// WHITE
#define _GHOST 0x7E

#define _LEFT_ENEMY_MISSILE ((unsigned char)0x7B)
#define _RIGHT_ENEMY_MISSILE ((unsigned char)0x7D)
#define _BUBBLE ((unsigned char)0x60)

#include "../display_macros.h"

// extern unsigned char XSize;

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
	extern Image CHASE_IMAGE;
	extern Image SUPER_IMAGE;
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif


Image PLAYER_DOWN;
Image PLAYER_UP;
Image PLAYER_RIGHT;
Image PLAYER_LEFT;

#if defined(MSX_MODE1)
	#define BASE 6144
	#define CHAR_BASE 0x0100
#else
	#define BASE 0
	#define CHAR_BASE 0
#endif


#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))

#define _DRAW(x,y,image) msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,image->_imageData);
#define _DELETE(x,y)     msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,' ');
#define _DRAW_VERTICAL_WALL(x,y)  msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize, _VERTICAL_BRICK);
#define _DRAW_HORIZONTAL_WALL(x,y)  msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize, _HORIZONTAL_BRICK);
#define _DRAW_BROKEN_WALL(x,y)  msx_vpoke(BASE+x+X_OFFSET+(y+Y_OFFSET)*XSize,BOMB_IMAGE._imageData);	


void redefine(unsigned short loc, const char *new_char)
{
	unsigned char i;
	
	for(i=0;i<8;++i)
	{
		msx_vpoke(loc+i-32*8,new_char[i]);
	}
}

void INIT_GRAPHICS(void)
{
	#if defined(REDEFINED_CHARS)
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
		static const char invincibility[8] =    { 24, 36, 24,  0,153,  0, 36,102};
		
		static const char dead_ghost[8] =       {129,126,165,129,129,189,129,126};
		
		static const char vertical_brick[8] =   { 24, 24, 24, 48, 24, 12, 24, 24};
		static const char horizontal_brick[8] = {  0,  0,  0,255,  0,  0,  0,  0};	
		static const char extra_points[8]     = { 16, 62, 32, 60,  4,124,  8,  0};
	#endif
		set_color(15, 1, 1);
	#if defined(MSX_MODE1)
		set_mode(mode_1);
		
		msx_vpoke(8192+ 0, 9*16); // 
		msx_vpoke(8192+ 1, 7*16); // 
		msx_vpoke(8192+ 2, 9*16); // 
		msx_vpoke(8192+ 3, 11*16); // 
		
		msx_vpoke(8192+ 4,10*16); // White !, $ -- 32 - 39
		msx_vpoke(8192+ 5, 2*16); // Green  -- 40 - 47
		msx_vpoke(8192+ 6, 4*16); // Green -- 48 - 55
		msx_vpoke(8192+ 7, 4*16); // Green -- 56 - 63
		
		msx_vpoke(8192+ 8, 8*16); // Red 64 --
		msx_vpoke(8192+ 9, 8*16); // Red 72 --
		msx_vpoke(8192+10, 8*16); // Red 80 --
		msx_vpoke(8192+11, 8*16); // Red 88 --
	#endif
	
	#if defined(REDEFINED_CHARS)
		redefine(CHAR_BASE+8*_PLAYER,player_down);
		redefine(CHAR_BASE+8*_PLAYER_DOWN,player_down);
		redefine(CHAR_BASE+8*_PLAYER_UP,player_up);	
		redefine(CHAR_BASE+8*_PLAYER_LEFT,player_left);	
		redefine(CHAR_BASE+8*_PLAYER_RIGHT, player_right);	
		
		redefine(CHAR_BASE+8*_BOMB, bomb);
		redefine(CHAR_BASE+8*_GUN, gun);
		redefine(CHAR_BASE+8*_POWERUP, powerUp);		
		redefine(CHAR_BASE+8*_FREEZE, powerUp);
		redefine(CHAR_BASE+8*_SUPER, powerUp);
		
		redefine(CHAR_BASE+8*_GHOST, ghost);
		redefine(CHAR_BASE+8*_ZOMBIE, ghost);
		redefine(CHAR_BASE+8*_INVINCIBLE_GHOST, invincible_ghost);	
		redefine(CHAR_BASE+8*_MISSILE, missile);	
		
		redefine(CHAR_BASE+8*_EXTRA_LIFE,player_down);	
		redefine(CHAR_BASE+8*_INVINCIBILITY,invincibility);	
		redefine(CHAR_BASE+8*_DEAD_GHOST,dead_ghost);	
		
		redefine(CHAR_BASE+8*_VERTICAL_BRICK,vertical_brick);	
		redefine(CHAR_BASE+8*_HORIZONTAL_BRICK,horizontal_brick);		
	
		#if defined(FULL_GAME)
			redefine(CHAR_BASE+8*_LEFT_ENEMY_MISSILE,missile_left);	
			redefine(CHAR_BASE+8*_RIGHT_ENEMY_MISSILE,missile_right);	
			redefine(CHAR_BASE+8*_BUBBLE, bubble);
			redefine(CHAR_BASE+8*_EXTRA_POINTS, extra_points);
		#endif
	#endif
}

void INIT_IMAGES(void)
{		

	GHOST_IMAGE._imageData = _GHOST;
	INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	PLAYER_IMAGE._imageData = _PLAYER;
	POWERUP_IMAGE._imageData = _POWERUP;
	FREEZE_IMAGE._imageData = _FREEZE;
	GUN_IMAGE._imageData = _GUN;
	EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
	
	MISSILE_IMAGE._imageData = _MISSILE;
	DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;

	PLAYER_DOWN._imageData = _PLAYER_DOWN;
	PLAYER_UP._imageData = _PLAYER_UP;	
	PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
	PLAYER_LEFT._imageData = _PLAYER_LEFT;		

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_ENEMY_MISSILE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_ENEMY_MISSILE;
		
		BUBBLE_IMAGE._imageData = _BUBBLE;
				
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;
		CHASE_IMAGE._imageData = _MISSILE;
		SUPER_IMAGE._imageData = _SUPER;
		CONFUSE_IMAGE._imageData = _CONFUSE;
		ZOMBIE_IMAGE._imageData = _ZOMBIE;
			
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

#if defined(REDEFINED_CHARS)
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


