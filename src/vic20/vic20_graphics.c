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
#include <stdlib.h>
#include <conio.h>
#include <peekpoke.h>
#include <vic20.h>
	
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
#define _EXTRA_LIFE 0x25
#define _EXTRA_POINTS 0x22

// GREEN
#define _POWERUP  0x2C

// RED
#define _BOMB 0x5E
#define _DEAD_GHOST 0x5B
// #define _MISSILE 0x5F

// CYAN
#define _INVINCIBILITY 0x08
#define _MISSILE 0x09

// WHITE
#define _GHOST 0x7E

#define _LEFT_ENEMY_MISSILE ((unsigned char)0x7B)
#define _RIGHT_ENEMY_MISSILE ((unsigned char)0x7D)
#define _BUBBLE ((unsigned char)0x60)

#include "../display_macros.h"

extern unsigned char XSize;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_POINTS_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	
	extern Image BROKEN_WALL_IMAGE;
#endif


Image PLAYER_DOWN;
Image PLAYER_UP;
Image PLAYER_RIGHT;
Image PLAYER_LEFT;


#define _DRAW(x,y,image) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(image->_imageData); } while(0)
#define _DELETE(x,y) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' '); } while(0)      
#define _DRAW_VERTICAL_WALL(x,y)  do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('|'); } while(0)  
#define _DRAW_HORIZONTAL_WALL(x,y)  do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('-'); } while(0)  
#define _DRAW_BROKEN_WALL(x,y) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('X'); } while(0)   	



void INIT_GRAPHICS(void)
{
	unsigned char tmp;
	
	tmp = ~0x0F & PEEK(&(VIC.addr));
	POKE(&(VIC.addr), tmp | 0x0F);
}

void INIT_IMAGES(void)
{		

	GHOST_IMAGE._imageData = _GHOST;
	INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	PLAYER_IMAGE._imageData = _PLAYER;
	POWERUP_IMAGE._imageData = _POWERUP;
	GUN_IMAGE._imageData = _GUN;
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
		
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;	
			
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




