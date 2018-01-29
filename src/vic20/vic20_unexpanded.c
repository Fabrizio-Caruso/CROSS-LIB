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
#include <string.h>

#if defined(NO_MESSAGE)
	#include "../text.h"
	
	extern short highScore;
#endif

#include "../input_macros.h"


#define _PLAYER '*'
// #define _PLAYER_DOWN 0x00
// #define _PLAYER_UP 0x01
// #define _PLAYER_RIGHT 0x02 
// #define _PLAYER_LEFT 0x03

// RED
#define _BOMB 'X'
//0x5E

// WHITE
#define _GHOST 'O'



#define _DEAD_GHOST _GHOST


// BLUE
#define _GUN 0x7B

// YELLOW
#define _INVINCIBLE_GHOST 0x77
#define _VERTICAL_BRICK 0x26
#define _HORIZONTAL_BRICK 0x2B
#define _EXTRA_LIFE _PLAYER
#define _EXTRA_POINTS '*'

// GREEN
#define _POWERUP  0x7A;


// CYAN
#define _INVINCIBILITY 0x73
#define _MISSILE 0x7C

#define _LEFT_ENEMY_MISSILE '>'

//((unsigned char)0x7B)
#define _RIGHT_ENEMY_MISSILE '<'

//((unsigned char)0x7D)
#define _BUBBLE '^'

#include "../display_macros.h"

// extern unsigned char XSize;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(TINY_GAME)
	extern Image INVINCIBLE_GHOST_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image GUN_IMAGE;
	extern Image MISSILE_IMAGE;
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

#define BASE_ADDR 7680

#if !defined(NO_COLOR)
	#define _DRAW(x,y,image) POKE(7680+x+y*22, image->_imageData)
	//do { gotoxy(x+X_OFFSET,y+Y_OFFSET); textcolor(image->_color); cputc(image->_imageData); } while(0)
	#define _DELETE(x,y) POKE(7680+x+y*22, 32)
	//do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' '); } while(0)      
	#define _DRAW_VERTICAL_WALL(x,y)  do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('|'); } while(0)  
	#define _DRAW_HORIZONTAL_WALL(x,y)  do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('-'); } while(0)  
	#define _DRAW_BROKEN_WALL(x,y) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('X'); } while(0)   	
#else
	#define _DRAW(x,y,image) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(image->_imageData); } while(0)
	#define _DELETE(x,y) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' '); } while(0)      
#endif


void INIT_GRAPHICS(void)
{

	// POKE(0x9005,0xFF);	
    // POKE(0x9002,PEEK(0x9002) | 0x80);	
	
	// WAIT_PRESS();

	#if defined(TINY_GAME)
		#include<peekpoke.h>
		POKE(646,1);
		POKE(36879L,9);
	#endif		
}

void INIT_IMAGES(void)
{		
	#if !defined(NO_COLOR)
		PLAYER_IMAGE._color = COLOR_CYAN;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
		GHOST_IMAGE._color = COLOR_WHITE;		
		
		#if !defined(TINY_GAME)
			INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_GREEN;
			GUN_IMAGE._color = COLOR_BLUE;
			EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
			MISSILE_IMAGE._color = COLOR_BLUE;
		#endif
		
		// PLAYER_DOWN._color = COLOR_CYAN;
		// PLAYER_UP._color = COLOR_CYAN;
		// PLAYER_RIGHT._color = COLOR_CYAN;
		// PLAYER_LEFT._color = COLOR_CYAN;

		#if defined(FULL_GAME)
			RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;		
			LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
			
			BUBBLE_IMAGE._color = COLOR_WHITE;

			FREEZE_IMAGE._color = COLOR_CYAN;
			EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;
			
			INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
			SUPER_IMAGE._color = COLOR_RED;
			CONFUSE_IMAGE._color = COLOR_RED;
			ZOMBIE_IMAGE._color = COLOR_YELLOW;
		#endif	
	#endif

	
	GHOST_IMAGE._imageData = _GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	PLAYER_IMAGE._imageData = _PLAYER;	
	DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;	
	
	
	#if !defined(TINY_GAME)
		INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;

		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		MISSILE_IMAGE._imageData = _MISSILE;
	#endif
	
	// PLAYER_DOWN._imageData = _PLAYER_DOWN;
	// PLAYER_UP._imageData = _PLAYER_UP;	
	// PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
	// PLAYER_LEFT._imageData = _PLAYER_LEFT;	

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

#if defined(NO_MESSAGE)
	#include <peekpoke.h>
	void highScoreScreen(void) 
	{ 
		unsigned char i; 
		unsigned short tmp; 
		
		tmp = highScore; 
		
		for(i=1;i<6;++i) 
		{ 
			tmp -= POKE(7686-i,(unsigned char) ((tmp)%10)); 
			tmp/=10; 
			POKE(7686-i,PEEK(7686-i)+48); 
		} 
		POKE(7686,48); 
		
	}

#endif

// void CLEAR_SCREEN(void)
// {
	// unsigned char i;
	// for(i=0;i<254;++i)
	// {
		// POKE(7680+i,32);
		// POKE(7680+254+i,32);
	// }
// }

#if !defined(TINY_GAME)
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
#endif


