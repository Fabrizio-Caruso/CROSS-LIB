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
#define _PLAYER '\'';
#define _PLAYER_DOWN '\'';
#define _PLAYER_UP 0x70
#define _PLAYER_RIGHT 0x74 
#define _PLAYER_LEFT 0x72

#define _GUN 0x7B

// YELLOW
#define _INVINCIBLE_GHOST 0x77
#define _VERTICAL_BRICK 0x26
#define _HORIZONTAL_BRICK 0x2B
#define _EXTRA_LIFE _PLAYER
#define _EXTRA_POINTS '*'

// GREEN
#define _POWERUP  0x7A;

// RED
#define _BOMB 0x78
//0x5E


// CYAN
#define _INVINCIBILITY 0x73
#define _MISSILE 0x7C

// WHITE
#define _GHOST 0x76
//_GHOST 0x7E

#define _DEAD_GHOST _GHOST


#define _LEFT_ENEMY_MISSILE '>'

//((unsigned char)0x7B)
#define _RIGHT_ENEMY_MISSILE '<'

//((unsigned char)0x7D)
#define _BUBBLE '^'

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
extern Image EXTRA_POINTS_IMAGE;

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image FREEZE_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image SUPER_IMAGE;	
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif


Image PLAYER_DOWN;
Image PLAYER_UP;
Image PLAYER_RIGHT;
Image PLAYER_LEFT;


#if !defined(NO_COLOR)
	#define _DRAW(x,y,image) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); textcolor(image->_color); cputc(image->_imageData); } while(0)
	#define _DELETE(x,y) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' '); } while(0)      
	#define _DRAW_VERTICAL_WALL(x,y)  do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('|'); } while(0)  
	#define _DRAW_HORIZONTAL_WALL(x,y)  do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('-'); } while(0)  
	#define _DRAW_BROKEN_WALL(x,y) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('X'); } while(0)   	
#else
	#define _DRAW(x,y,image) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(image->_imageData); } while(0)
	#define _DELETE(x,y) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' '); } while(0)      
#endif


void INIT_GRAPHICS(void)
{
	unsigned char tmp;
	
	tmp = ~0x0F & PEEK(&(VIC.addr));
	POKE(&(VIC.addr), tmp | 0x0F);
}

void INIT_IMAGES(void)
{		
	#if !defined(NO_COLOR)
		// PLAYER_IMAGE._color = COLOR_CYAN;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
		POWERUP_IMAGE._color = COLOR_GREEN;
		GUN_IMAGE._color = COLOR_BLUE;
		BOMB_IMAGE._color = COLOR_RED;
		EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
		
		DEAD_GHOST_IMAGE._color = COLOR_RED;

		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_BLUE;

		PLAYER_DOWN._color = COLOR_CYAN;
		PLAYER_UP._color = COLOR_CYAN;
		PLAYER_RIGHT._color = COLOR_CYAN;
		PLAYER_LEFT._color = COLOR_CYAN;

		#if defined(FULL_GAME)
			RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;		
			LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
			
			BUBBLE_IMAGE._color = COLOR_WHITE;

			FREEZE_IMAGE._color = COLOR_CYAN;
			EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;
			
			INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
			SUPER_IMAGE._color = COLOR_RED;
			CONFUSE_IMAGE._color = COLOR_RED;
			ZOMBIE_IMAGE._color = COLOR_RED;
		#endif	
	#endif

	
	GHOST_IMAGE._imageData = _GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	// PLAYER_IMAGE._imageData = _PLAYER;	
	
	#if !defined(TINY_GAME)
		INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;

		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		MISSILE_IMAGE._imageData = _MISSILE;
		DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;

		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;	
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		PLAYER_LEFT._imageData = _PLAYER_LEFT;		
	#endif

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_ENEMY_MISSILE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_ENEMY_MISSILE;
		
		BUBBLE_IMAGE._imageData = _BUBBLE;

		FREEZE_IMAGE._imageData = _POWERUP;		
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;	
		SUPER_IMAGE._imageData = _POWERUP;
		CONFUSE_IMAGE._imageData = _INVINCIBLE_GHOST;
		ZOMBIE_IMAGE._imageData = _GHOST;
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


void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
{
	unsigned char i;
	SET_TEXT_COLOR(COLOR_YELLOW);

	for(i=0;i<length;++i) 
	{ 
		gotoxy(x+i+X_OFFSET,y+Y_OFFSET);  cputc('-');
	} 	
}


void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
{
	unsigned char i;
	SET_TEXT_COLOR(COLOR_YELLOW);

	for(i=0;i<length;++i) 
	{ 
		gotoxy(x+X_OFFSET,y+Y_OFFSET+i);  cputc('|');
	} 	
}


