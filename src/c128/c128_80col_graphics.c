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
#include <c128.h>

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
	
	extern Image BROKEN_WALL_IMAGE;
#endif

void INIT_GRAPHICS(void)
{
	fast();
	videomode(80);
}

void INIT_IMAGES(void)
{		
	PLAYER_IMAGE._color = COLOR_CYAN;
	INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
	POWERUP_IMAGE._color = COLOR_YELLOW;
	FREEZE_IMAGE._color = COLOR_CYAN;
	GUN_IMAGE._color = COLOR_BROWN;
	BOMB_IMAGE._color = COLOR_RED;
	DEAD_GHOST_IMAGE._color = COLOR_RED;

	GHOST_IMAGE._imageData = 'o';
	INVINCIBLE_GHOST_IMAGE._imageData = '+';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	POWERUP_IMAGE._imageData = 'S';
	FREEZE_IMAGE._imageData = 'F';	
	GUN_IMAGE._imageData = '!';
	MISSILE_IMAGE._imageData = '.';
	DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

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
		
		EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
		EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;	
		INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
	#endif
}

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (COLOR_WHITE);
		cputc('X');
	}
#endif

void _draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	(void) textcolor (image->_color);
	cputc(image->_imageData); 
}

void _delete(unsigned char x, unsigned char y)
{
	gotoxy(x+X_OFFSET,y+Y_OFFSET);
	cputc(' ');
}

void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char * blinkCounter) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	(void) textcolor (image->_color);
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


void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
{
	unsigned char i;
	SET_TEXT_COLOR(COLOR_YELLOW);

	for(i=0;i<length;++i) 
	{ 
		gotoxy(x+i+X_OFFSET,y+Y_OFFSET);  cputc('-');
	} 	
}

void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length)
{
	textcolor (COLOR_YELLOW);
	cvlinexy (x+X_OFFSET,y+Y_OFFSET,length);		
}


