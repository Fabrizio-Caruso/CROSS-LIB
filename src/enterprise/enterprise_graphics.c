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


// #include <conio.h>
// #include <peekpoke.h>
#include <enterprise.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../display_macros.h"


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
#define _BOMB 0x29
//0x5E


// CYAN
#define _INVINCIBILITY 0x21
#define _MISSILE 0x23

// WHITE
#define _GHOST 0x28
//_GHOST 0x7E

#define _DEAD_GHOST _GHOST


#define _LEFT_ENEMY_MISSILE 0x22

//((unsigned char)0x7B)
#define _RIGHT_ENEMY_MISSILE 0x2E

//((unsigned char)0x7D)
#define _BUBBLE 0x2F
//((unsigned char)0x60)

// #define VERTICAL_BRICK '='
// #define HORIZONTAL_BRICK 62

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(TINY_GAME)
	extern Image INVINCIBLE_GHOST_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image MISSILE_IMAGE;
	extern Image GUN_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
#endif

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image FREEZE_IMAGE;	
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	extern Image SUPER_IMAGE;
	extern Image CHASE_IMAGE;
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif

#if defined(REDEFINED_CHARS)
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
#endif


void INIT_GRAPHICS(void)
{
    exos_set_vmode(VM_HW_TXT,CM_2,40,25);
}
 
 
// TODO: Sprite initialization (to be performed only once) should be separated from level generation
void INIT_IMAGES(void)
{		
	PLAYER_IMAGE._color = COLOR_CYAN;
	BOMB_IMAGE._color = COLOR_RED;
	DEAD_GHOST_IMAGE._color = COLOR_RED;
	GHOST_IMAGE._color = COLOR_WHITE;

	#if !defined(TINY_GAME)
		POWERUP_IMAGE._color = COLOR_GREEN;
		GUN_IMAGE._color = COLOR_BLUE;
		EXTRA_POINTS_IMAGE._imageData = '$';
		EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;	
		MISSILE_IMAGE._color = COLOR_WHITE;
		
		PLAYER_IMAGE._imageData = _PLAYER_DOWN;
		GHOST_IMAGE._imageData = _GHOST;
		DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;
		BOMB_IMAGE._imageData = _BOMB;			
	#endif
	
	#if defined(REDEFINED_CHARS)
		PLAYER_DOWN._color = COLOR_CYAN;
		PLAYER_UP._color = COLOR_CYAN;
		PLAYER_RIGHT._color = COLOR_CYAN;
		PLAYER_LEFT._color = COLOR_CYAN;
		
		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;		
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		PLAYER_LEFT._imageData = _PLAYER_LEFT;			
	#endif	
		
	#if defined(FULL_GAME)
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;		
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		
		BUBBLE_IMAGE._color = COLOR_WHITE;
		FREEZE_IMAGE._color = COLOR_CYAN;
			
		EXTRA_LIFE_IMAGE._color = COLOR_RED;
		
		INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
		SUPER_IMAGE._color = COLOR_RED;
		
		CHASE_IMAGE._color = COLOR_WHITE;
		CONFUSE_IMAGE._color = COLOR_RED;
		ZOMBIE_IMAGE._color = COLOR_YELLOW;
	#endif
	
	#if !defined(TINY_GAME)
		INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;
		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		MISSILE_IMAGE._imageData = _MISSILE;
	#endif
		
	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_ENEMY_MISSILE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_ENEMY_MISSILE;		
		BUBBLE_IMAGE._imageData = _BUBBLE;
		
		FREEZE_IMAGE._imageData = _POWERUP;		
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;
		EXTRA_LIFE_IMAGE._imageData = PLAYER_DOWN._imageData;	
		SUPER_IMAGE._imageData = _POWERUP;
		CHASE_IMAGE._imageData = _MISSILE;
		CONFUSE_IMAGE._imageData = _INVINCIBLE_GHOST;
		ZOMBIE_IMAGE._imageData = _GHOST;
	#endif	
}

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(char x, char y)
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (COLOR_RED);
		cputc(BOMB_IMAGE._imageData);
	}
#endif

void _draw(char x, char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	(void) textcolor (image->_color);
	cputc(image->_imageData); 
};


void _delete(char x, char y)
{
	gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' ');
};

#if !defined(TINY_GAME)
	void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char * blinkCounter)
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(image->_color);
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
			gotoxy(x+i+X_OFFSET,y+Y_OFFSET);  cputc(_HORIZONTAL_BRICK);
		} 	
	}


	void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
	{
		unsigned char i;
		SET_TEXT_COLOR(COLOR_YELLOW);

		for(i=0;i<length;++i) 
		{ 
			gotoxy(x+X_OFFSET,y+Y_OFFSET+i);  cputc(_VERTICAL_BRICK);
		} 	
	}

#endif

