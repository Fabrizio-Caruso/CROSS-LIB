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

#define _POWERUP (char) 0x7A
#define _GHOST (char) 0x76
#define _BOMB (char) 0x78
#define _GUN (char) 0x7B

#define _PLAYER_DOWN '\''
#define _PLAYER_UP ((char) 0x76 - 6)
#define _PLAYER_RIGHT ((char) 0x76 - 2)
#define _PLAYER_LEFT ((char) 0x76 - 4)
	

#define _MISSILE (char) 0x7C
#define _BUBBLE '^'
#define _EXTRA_POINTS '*'
#define _LEFT_MISSILE '>'
#define _RIGHT_MISSILE '<'
#define _INVINCIBILITY 0x73

#define _INVINCIBLE_GHOST (char) 0x77


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
	
	extern Image FREEZE_IMAGE;
	
	extern Image INVINCIBILITY_IMAGE;	
	
	extern Image CHASE_IMAGE;

	extern Image SUPER_IMAGE;	
	extern Image EXTRA_LIFE_IMAGE;
	
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif

Image PLAYER_DOWN;
Image PLAYER_UP;
Image PLAYER_RIGHT;
Image PLAYER_LEFT;

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
	#if !defined(REDEFINED_CHARS)
	PLAYER_IMAGE._color = COLOR_CYAN;
	PLAYER_IMAGE._imageData = _PLAYER_DOWN;
	#endif
	
	INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
	POWERUP_IMAGE._color = COLOR_GREEN;
	FREEZE_IMAGE._color = COLOR_CYAN;
	GUN_IMAGE._color = COLOR_BROWN;
	BOMB_IMAGE._color = COLOR_RED;
	DEAD_GHOST_IMAGE._color = COLOR_RED;
		
	GHOST_IMAGE._imageData = _GHOST;
	INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	POWERUP_IMAGE._imageData = _POWERUP;
	FREEZE_IMAGE._imageData = _POWERUP;	
	GUN_IMAGE._imageData = _GUN;
	MISSILE_IMAGE._imageData = _MISSILE;
	DEAD_GHOST_IMAGE._imageData = GHOST_IMAGE._imageData;

	GHOST_IMAGE._color = COLOR_WHITE;
	MISSILE_IMAGE._color = COLOR_BLUE;

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_MISSILE;
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_MISSILE;
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;		
		
		BUBBLE_IMAGE._imageData = _BUBBLE;
		BUBBLE_IMAGE._color = COLOR_WHITE;
		
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_DOWN._imageData;
		EXTRA_LIFE_IMAGE._color = COLOR_RED;
		
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;
		INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
		
		SUPER_IMAGE._imageData = _POWERUP;
		SUPER_IMAGE._color = COLOR_RED;		
		
		CHASE_IMAGE._imageData = _MISSILE;
		CHASE_IMAGE._color = COLOR_BLUE;
		
		CONFUSE_IMAGE._imageData = _INVINCIBLE_GHOST;
		CONFUSE_IMAGE._color = COLOR_RED;
		
		ZOMBIE_IMAGE._imageData = _GHOST;
		ZOMBIE_IMAGE._color = COLOR_YELLOW;
	#endif
	
	PLAYER_DOWN._imageData = _PLAYER_DOWN;
	PLAYER_DOWN._color = COLOR_CYAN;	
	
	PLAYER_UP._imageData = _PLAYER_UP;
	PLAYER_UP._color = COLOR_CYAN;
			
	PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
	PLAYER_RIGHT._color = COLOR_CYAN;
	
	PLAYER_LEFT._imageData = _PLAYER_LEFT;
	PLAYER_LEFT._color = COLOR_CYAN;			
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