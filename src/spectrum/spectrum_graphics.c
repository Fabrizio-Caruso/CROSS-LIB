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

#include <string.h>

#include "../display_macros.h"

#if !defined(CLIB_ANSI)
	#include<arch/zx.h>
#endif

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(TINY_GAME)
	extern Image INVINCIBLE_GHOST_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image MISSILE_IMAGE;
	extern Image GUN_IMAGE;
#endif

#if defined(REDEFINED_CHARS)
	extern char udg[];
#endif
	
#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_POINTS_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	
	extern Image BROKEN_WALL_IMAGE;
#endif

	
#if defined(REDEFINED_CHARS)
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
#endif

#include <stdio.h>

#if defined(CLIB_ANSI)
	#include <graphics.h>

	#include <spectrum.h>
#endif
	

#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))

#if defined(REDEFINED_CHARS)
struct redefine_struct
{
   unsigned char ascii;
   unsigned char bitmap[8];
} ;
#endif

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
#define _HORIZONTAL_BRICK 0x2B
#define _EXTRA_LIFE 0x25
#define _EXTRA_POINTS 0x24

// GREEN
#define _POWERUP  0x2C

// RED
#define _BOMB 0x5E
#define _DEAD_GHOST 0x5B
// #define _MISSILE 0x5F

// CYAN
#define _INVINCIBILITY 0x21
#define _MISSILE 0x23

// WHITE
#define _GHOST 0x7E

#define _LEFT_ENEMY_MISSILE ((unsigned char)0x7B)
#define _RIGHT_ENEMY_MISSILE ((unsigned char)0x7D)
#define _BUBBLE ((unsigned char)0x60)

#if defined(REDEFINED_CHARS)
struct redefine_struct redefine_map[] =
{
	{_PLAYER_DOWN, { 24, 36, 24,102,153, 24, 36,102}},
	{_PLAYER_UP, { 24, 60, 24,102,153, 24, 36,102}},
	{_PLAYER_RIGHT, { 24, 52, 25,118,152, 24, 20, 20}},	
	{_PLAYER_LEFT, { 24, 44,152,110, 25, 24, 40, 40}},
	{_GHOST, {129,126,165,129,129,189,129,126}},
	{_RIGHT_ENEMY_MISSILE, {  0,  0, 15,252,252, 15,  0,  0}},
	{_LEFT_ENEMY_MISSILE, {  0,  0,240, 63, 63,240,  0,  0}},
	{_INVINCIBLE_GHOST, { 60, 66,165,129, 90, 36, 36, 60}},
	{_GUN, {  0,128,126,200,248,192,128,  0}},
	{_POWERUP, {  0, 60, 54,223,231,122, 36, 24}},
	{_MISSILE, {  0,  0,  8, 56, 28, 16,  0,  0}},
	{_BOMB, { 60, 66,165,153,153,165, 66, 60}},
	{_BUBBLE, { 24, 60, 60, 60,126, 90, 66, 66}},
	{_INVINCIBILITY, { 24, 36, 24,  0,153,  0, 36,102}},
	{_VERTICAL_BRICK, { 24, 24, 24, 48, 24, 12, 24, 24}},
	{_HORIZONTAL_BRICK, {  0,  0,  0,255,  0,  0,  0,  0}}			
};
#endif

#if !defined(TINY_GAME)
	void INIT_GRAPHICS(void)
	{
		#if defined(REDEFINED_CHARS)
			unsigned char i;
			for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i)
			{
			   memcpy(udg + (redefine_map[i].ascii - 32)*8, redefine_map[i].bitmap, 8);
			}
		#endif

		#if defined(CLIB_ANSI)
			clg();
			zx_border(0);
			zx_colour(PAPER_BLACK|INK_WHITE);	
		#else
			zx_border(INK_BLACK);
		#endif
	}
#else
	void INIT_GRAPHICS(void)
	{
		printf("\x11%c",COLOR_BLACK);		
	}	
#endif

void INIT_IMAGES(void)
{		

	#if defined(COLOR)
		PLAYER_IMAGE._color = COLOR_CYAN;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
		
		#if !defined(TINY_GAME)
			INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_GREEN;
			GUN_IMAGE._color = COLOR_MAGENTA;
		#endif
	#endif
	
	#if defined(FULL_GAME)
		EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
		EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;
		INVINCIBILITY_IMAGE._color = COLOR_YELLOW;	
	#endif
		
	#if defined(REDEFINED_CHARS)	
		PLAYER_IMAGE._imageData = _PLAYER_DOWN;
		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;		
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		PLAYER_LEFT._imageData = _PLAYER_LEFT;	
		
		GHOST_IMAGE._imageData = _GHOST;
		INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;
		BOMB_IMAGE._imageData = _BOMB;		
		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		MISSILE_IMAGE._imageData = _MISSILE;

		#if defined(FULL_GAME)
			LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_ENEMY_MISSILE;
			RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_ENEMY_MISSILE;		
			BUBBLE_IMAGE._imageData = _BUBBLE;
			INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;
		#endif
	#else
		GHOST_IMAGE._imageData = 'o';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '*';
		
		#if !defined(TINY_GAME)
			INVINCIBLE_GHOST_IMAGE._imageData = '+';
			POWERUP_IMAGE._imageData = 'S';
			GUN_IMAGE._imageData = '!';
			MISSILE_IMAGE._imageData = '.';
		#endif
		#if defined(FULL_GAME)
			LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
			RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
			BUBBLE_IMAGE._imageData = '^';
		#endif	
	#endif
	
	DEAD_GHOST_IMAGE._imageData = GHOST_IMAGE._imageData;

	#if defined(COLOR)
		GHOST_IMAGE._color = COLOR_WHITE;
		
		#if !defined(TINY_GAME)
		MISSILE_IMAGE._color = COLOR_WHITE;
		#endif
	#endif
	
	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
		
		BUBBLE_IMAGE._color = COLOR_WHITE;
		EXTRA_POINTS_IMAGE._imageData = '$';
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = '!';
	#endif
	
	#if defined(REDEFINED_CHARS)
		PLAYER_DOWN._color = COLOR_CYAN;
		PLAYER_UP._color = COLOR_CYAN;		
		PLAYER_RIGHT._color = COLOR_CYAN;
		PLAYER_LEFT._color = COLOR_CYAN;
	#endif
}

#if defined(FULL_GAME)	
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_draw(x,y,&BOMB_IMAGE);
	}
#endif
	
void _draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	#if defined(COLOR)
		SET_TEXT_COLOR(image->_color);
	#else
		SET_TEXT_COLOR(COLOR_WHITE);		
	#endif
	cputc(image->_imageData); 
}

void _delete(unsigned char x, unsigned char y)
{
	gotoxy(x+X_OFFSET,y+Y_OFFSET);
	cputc(' ');
}

void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	#if defined(COLOR)
		SET_TEXT_COLOR(image->_color);
	#endif
	if(*blinkCounter) 
	{
		cputc(image->_imageData); 
		*blinkCounter=0;
	} 
	else 
	{
		cputc(' '); 
		*blinkCounter=1;
	}	
}

#if !defined(TINY_GAME)
	void DRAW_HORIZONTAL_LINE(unsigned char x, unsigned char y, unsigned char length) 
	{ 
		unsigned char i; 
		
		SET_TEXT_COLOR(COLOR_YELLOW);
		for(i=0;i<length;++i) 
		{ 
			gotoxy(x+X_OFFSET+i,y+Y_OFFSET);  putchar(_HORIZONTAL_BRICK); 
		} 
	}


	void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length) 
	{ 
		unsigned char i; 
		
		SET_TEXT_COLOR(COLOR_YELLOW);
		for(i=0;i<length;++i) 
		{ 
			gotoxy(x+X_OFFSET,y+Y_OFFSET+i);  putchar(_VERTICAL_BRICK); 
		} 
	}
#endif



