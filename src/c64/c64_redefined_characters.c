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

#include<conio.h>
#include <peekpoke.h>

#include "../display_macros.h"

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

extern char YSize; 

void INIT_GRAPHICS(void)
{
	// Select video bank and set the character set pointer to point to $C800
	// where the redefined charset has been placed by the linker
	POKE(56576u,0);
	POKE(53272u,2);
	POKE(648,192);
	
	// Frame color = BLACK
	POKE(53280u,0);
	
	// Background color = BLACK
	POKE(53281u,0);
}
 
 
void INIT_IMAGES(void)
{		
	PLAYER_IMAGE._color = COLOR_CYAN;
	INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
	POWERUP_IMAGE._color = COLOR_GREEN;
	GUN_IMAGE._color = COLOR_BROWN;
	BOMB_IMAGE._color = COLOR_RED;
	DEAD_GHOST_IMAGE._color = COLOR_RED;
		
	GHOST_IMAGE._imageData = (char) 0x76;
	INVINCIBLE_GHOST_IMAGE._imageData = (char) 0x77;
	BOMB_IMAGE._imageData = (char) 0x78;
	PLAYER_IMAGE._imageData = '\'';
	POWERUP_IMAGE._imageData = (char) 0x7A;
	GUN_IMAGE._imageData = (char) 0x7B;
	MISSILE_IMAGE._imageData = (char) 0x7C;
	DEAD_GHOST_IMAGE._imageData = GHOST_IMAGE._imageData;

	GHOST_IMAGE._color = COLOR_WHITE;
	MISSILE_IMAGE._color = COLOR_BLUE;

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;		
		
		BUBBLE_IMAGE._imageData = '^';
		BUBBLE_IMAGE._color = COLOR_WHITE;
		
		EXTRA_POINTS_IMAGE._imageData = '*';
		EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_DOWN._imageData;
		EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;
		
		INVINCIBILITY_IMAGE._imageData = '!';
		INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
	#endif
	
	PLAYER_UP._imageData = (char) 0x76 - 6;
	PLAYER_UP._color = PLAYER_IMAGE._color;
	
	PLAYER_DOWN._imageData = '\'';
	PLAYER_DOWN._color = PLAYER_IMAGE._color;
			
	PLAYER_RIGHT._imageData = (char) 0x76 - 2;
	PLAYER_RIGHT._color = PLAYER_IMAGE._color;
	
	PLAYER_LEFT._imageData = (char) 0x76 - 4;
	PLAYER_LEFT._color = PLAYER_IMAGE._color;			
}

void DRAW_BROKEN_WALL(char x, char y)
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	(void) textcolor (COLOR_WHITE);
	cputc('X');
}

void _draw(char x, char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	(void) textcolor (image->_color);
	cputc(image->_imageData); 
};

void _blink_draw(char x, char y, Image * image, unsigned char * blinkCounter) 
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
};


void _delete(char x, char y)
{
	gotoxy(x+X_OFFSET,y+Y_OFFSET);cputc(' ');
};
	
