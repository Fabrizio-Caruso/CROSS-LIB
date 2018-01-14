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

extern unsigned char XSize;

#define _DRAW(x,y,image)  do {gotoxy((x+X_OFFSET),(y+Y_OFFSET)); textcolor(image->_color); cputc(image->_imageData);} while(0); 
#define _DELETE(x,y)     do {gotoxy((x+X_OFFSET),(y+Y_OFFSET)); cputc(' ');} while(0); 
#define _DRAW_WALL(x,y)  do {gotoxy((x+X_OFFSET),(y+Y_OFFSET)); cputc('|');} while(0); 
#define _DRAW_BROKEN_WALL(x,y) do {gotoxy((x+X_OFFSET),(y+Y_OFFSET)); textcolor(COLOR_BLACK); cputc('X');} while(0); 	

#include "../display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image FREEZE_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_POINTS_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	extern Image SUPER_IMAGE;
	extern Image CHASE_IMAGE;
	extern IMAGE CONFUSE_IMAGE;
	extern IMAGE ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif


void INIT_GRAPHICS(void)
{	
}

void INIT_IMAGES(void)
{		

	// PLAYER_IMAGE._color = COLOR_CYAN;
	// INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
	// POWERUP_IMAGE._color = COLOR_YELLOW;
	// FREEZE_IMAGE._color = COLOR_CYAN;
	// GUN_IMAGE._color = COLOR_BLACK;
	// BOMB_IMAGE._color = COLOR_RED;
	// DEAD_GHOST_IMAGE._color = COLOR_RED;
	// GHOST_IMAGE._color = COLOR_BLACK;
	// MISSILE_IMAGE._color = COLOR_BLACK;
	// LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_BLACK;
	// RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_BLACK;	
	// BUBBLE_IMAGE._color = COLOR_BLACK;				
	// EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
	// EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;
	// INVINCIBILITY_IMAGE._color = COLOR_YELLOW;	
	// SUPER_IMAGE._color = COLOR_RED;
	// CHASE_IMAGE._color = COLOR_RED;
	// CONFUSE_IMAGE._color = COLOR_RED;
	// ZOMBIE_IMAGE._color = COLOR_RED;
	
	GHOST_IMAGE._imageData = 'o';
	INVINCIBLE_GHOST_IMAGE._imageData = '+';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	POWERUP_IMAGE._imageData = 'S';
	FREEZE_IMAGE._imageData = 'F';	
	GUN_IMAGE._imageData = '!';
	MISSILE_IMAGE._imageData = '.';
	DEAD_GHOST_IMAGE._imageData = '#';

	LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';

	RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';

	BUBBLE_IMAGE._imageData = '^';
	
	EXTRA_POINTS_IMAGE._imageData = '$';
	
	EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
	INVINCIBILITY_IMAGE._imageData = 'V';
	SUPER_IMAGE._imageData = 'H';
	CHASE_IMAGE._imageData = '.';
	CONFUSE_IMAGE._imageData = 'C';
	ZOMBIE_IMAGE._imageData = 'Z';
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
	void DRAW_HORIZONTAL_LINE(unsigned char x, unsigned char y, unsigned char length)
	{
		// TODO: To implement
	}

	void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length)
	{
		textcolor (COLOR_BLACK);
		cvlinexy (x+X_OFFSET,y+Y_OFFSET,length);
	}
#endif

	
	
		


