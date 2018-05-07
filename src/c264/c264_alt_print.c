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
#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../display_macros.h"

#if defined(FULL_GAME)
	#define UDG_BASE_FACTOR 29
#else
	#define UDG_BASE_FACTOR 15
#endif

// BLUE
#define _PLAYER '*'
#define _PLAYER_DOWN 0x3B
#define _PLAYER_UP 0x3C
#define _PLAYER_RIGHT 0x3D 
#define _PLAYER_LEFT 0x3E

#define _GUN '!'

// YELLOW
#define _INVINCIBLE_GHOST '+'
#define _VERTICAL_BRICK '|'
#define _HORIZONTAL_BRICK '-'
#define _EXTRA_LIFE '*'
#define _EXTRA_POINTS '$'

// GREEN
#define _POWERUP  'S'

// RED
#define _BOMB 'x'
//0x5E


// CYAN
#define _INVINCIBILITY 'V'
#define _MISSILE '.'

// WHITE
#define _GHOST 'o'
//_GHOST 0x7E

#define _DEAD_GHOST '#'


#define _LEFT_ENEMY_MISSILE '>'

//((unsigned char)0x7B)
#define _RIGHT_ENEMY_MISSILE '<'

//((unsigned char)0x7D)
#define _BUBBLE '^'
//((unsigned char)0x60)

// #define VERTICAL_BRICK '='
// #define HORIZONTAL_BRICK 62

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
#else
	#define PLAYER_DOWN PLAYER_IMAGE
#endif


void INIT_GRAPHICS(void)
{

}
 
 
// TODO: Sprite initialization (to be performed only once) should be separated from level generation
void INIT_IMAGES(void)
{		
	#if !defined(REDEFINED_CHARS)
		PLAYER_IMAGE._color = COLOR_CYAN;
		PLAYER_IMAGE._imageData = _PLAYER;
	#endif
	
	INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
	POWERUP_IMAGE._color = COLOR_GREEN;
	GUN_IMAGE._color = COLOR_BLUE;
	EXTRA_POINTS_IMAGE._imageData = '$';
	EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
	
	BOMB_IMAGE._color = COLOR_RED;
	#if !defined(NO_DEAD_GHOST)
	DEAD_GHOST_IMAGE._color = COLOR_RED;
	#endif
	
	GHOST_IMAGE._color = COLOR_WHITE;
	MISSILE_IMAGE._color = COLOR_WHITE;

	#if defined(REDEFINED_CHARS)
	PLAYER_DOWN._color = COLOR_CYAN;
	PLAYER_UP._color = COLOR_CYAN;
	PLAYER_RIGHT._color = COLOR_CYAN;
	PLAYER_LEFT._color = COLOR_CYAN;
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
	
	#if defined(REDEFINED_CHARS)
	PLAYER_DOWN._imageData = _PLAYER_DOWN;
	PLAYER_UP._imageData = _PLAYER_UP;		
	PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
	PLAYER_LEFT._imageData = _PLAYER_LEFT;	
	#endif
	
	GHOST_IMAGE._imageData = _GHOST;
	
	#if !defined(NO_DEAD_GHOSTS)
	DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;
	#endif
	
	INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;
	BOMB_IMAGE._imageData = _BOMB;		
	POWERUP_IMAGE._imageData = _POWERUP;
	GUN_IMAGE._imageData = _GUN;
	MISSILE_IMAGE._imageData = _MISSILE;

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


#define BASE_ADDR ((unsigned short) 0x0C00)
#define COLOR_ADDR ((unsigned short) 0x0800)

// unsigned short loc(unsigned char x, unsigned char y)
// {
	// return ((unsigned short) BASE_ADDR)+(x+X_OFFSET)+(y+Y_OFFSET)*((unsigned short)XSize);
// }

#define _DRAW(x,y,image) do {POKE(loc(x,y), image->_imageData); POKE((-0x0400+loc(x,y)), image->_color); } while(0)
#define _DELETE(x,y) POKE(loc(x,y), 32)


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
	// gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	// (void) textcolor (image->_color);
	// cputc(image->_imageData); 
	// _DRAW(x,y,image);
	// POKE(loc(x,y),image->_imageData);
	POKE(BASE_ADDR+x+(unsigned short)(y+2)*40,image->_imageData);
	POKE(COLOR_ADDR+x+(unsigned short)(y+2)*40,image->_color);
};

void _delete(char x, char y)
{
	//gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' ');
	POKE(BASE_ADDR+x+(unsigned short)(y+2)*40,32);
};


#if !defined(NO_BLINKING)
void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char * blinkCounter)
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(image->_color);
	if(*blinkCounter) 
	{
		// cputc(image->_imageData); 
		_draw(x,y,image);
		*blinkCounter=0;
	} 
	else 
	{
		// cputc(' '); 
		_delete(x,y);
		*blinkCounter=1;
	}	
}	
#endif


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


#if defined(ALT_PRINT)

void PRINT(unsigned char x, unsigned char y, char * str)
{
	unsigned char i = 0;
	while(str[i]!='\0')
	{
		POKE(BASE_ADDR+x+(y+2)*40+i, str[i]); 
		++i;
	}
}

void print_05u0(unsigned char x, unsigned char y, unsigned short val)
{
	// gotoxy(x+X_OFFSET,y+Y_OFFSET);
	// cprintf("%05u0",val);		
}	

void print_02u(unsigned char x, unsigned char y, unsigned short val)
{
	// gotoxy(x,y+Y_OFFSET);
	// cprintf("%02u",val);
}	


void print_u(unsigned char x, unsigned char y, unsigned short val)
{
	gotoxy(x,y+Y_OFFSET);
	cputc((char) (val+48));
	// POKE(BASE_ADDR+x+(y*40u)+80u, (char) (val + 48));
}


void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
{
	if(strlen(str)==5)
	{	
		print_05u0(x,y,val);
	}
	else if(strlen(str)==4)
	{
		print_02u(x,y,val);		
	}
	else
	{
		print_u(x,y,val);		
	}
}

// void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
// {
	// gotoxy(x,y);
	// cprintf(str,val);
// }
#endif