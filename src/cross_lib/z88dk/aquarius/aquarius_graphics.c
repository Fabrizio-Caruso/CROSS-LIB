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
#include <conio.h>

extern unsigned char XSize;

#define VIDEO_BASE 12288
#define COLOR_BASE (VIDEO_BASE+1024)

#define _AQUARIUS_RED 16
#define _AQUARIUS_WHITE (16+32+64)
#define _AQUARIUS_CYAN (16+128)

#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))

#include "../../display/display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image SKULL_IMAGE;
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

#if defined(REDEFINED_CHARS)
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
#endif


void INIT_GRAPHICS(void)
{
	POKE(VIDEO_BASE,32);
	POKE(COLOR_BASE,7);
}

void INIT_IMAGES(void)
{		

	GHOST_IMAGE._imageData = 'o';

	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	DEAD_GHOST_IMAGE._imageData = 'O';

	#if !defined(NO_COLOR)
		GHOST_IMAGE._color = _AQUARIUS_WHITE;

		BOMB_IMAGE._color = _AQUARIUS_RED;
		PLAYER_IMAGE._color = _AQUARIUS_CYAN;
		DEAD_GHOST_IMAGE._color = _AQUARIUS_RED;		
	#endif
	
	// POWERUP_IMAGE._imageData = 'S';
	// GUN_IMAGE._imageData = '!';
	// MISSILE_IMAGE._imageData = '.';
	// SKULL_IMAGE._imageData = '+';
	
}

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		// TO BE IMPLEMENTED
	}
#endif

#if !defined(TINY_GAME)
void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length)
{ 
	unsigned char i;
	for(i=0;i<length;++i)
	{
		POKE(VIDEO_BASE+x+X_OFFSET+(y+i+Y_OFFSET)*40,'|');
	}
}

void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	if(*blinkCounter) 
	{
		_draw(x,y,image);
		*blinkCounter=0;
	} 
	else 
	{
		_delete(x,y);
		*blinkCounter=1;
	}	
}

#endif

void _draw(unsigned char x, unsigned char y, Image * image) 
{
	POKE(VIDEO_BASE+x+X_OFFSET+(y+Y_OFFSET)*40,image->_imageData);
	#if !defined(NO_COLOR)
		POKE(COLOR_BASE+x+X_OFFSET+(y+Y_OFFSET)*40,image->_color);	
	#endif
}

void _delete(unsigned char x, unsigned char y)
{
	POKE(VIDEO_BASE+x+X_OFFSET+(y+Y_OFFSET)*40,' ');	
}


#if defined(ALT_PRINT)

void PRINT(unsigned char x, unsigned char y, char * str)
{
	unsigned char i = 0;
	while(str[i]!='\0')
	{
		POKE(VIDEO_BASE+x+i+y*((unsigned short)XSize), str[i]); 
		POKE(COLOR_BASE+x+i+y*((unsigned short)XSize), _AQUARIUS_WHITE); 		
		++i;
	}
}

void print_05u0(unsigned char x, unsigned char y, unsigned short val)
{
	unsigned char i;
	unsigned char digits[6];
	unsigned short tmp = val;
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (unsigned char) ((tmp)%10);
		tmp-= digits[i];
		tmp/=10;
	}
	
	for(i=0;i<6;++i)
	{
		POKE(VIDEO_BASE+x+i+y*((unsigned short)XSize), (unsigned char) (digits[5-i])+48);
		POKE(COLOR_BASE+x+i+y*((unsigned short)XSize), _AQUARIUS_WHITE);		
	}
}	

void print_02u(unsigned char x, unsigned char y, unsigned short val)
{
	POKE(VIDEO_BASE+x+y*  ((unsigned short)XSize), ((unsigned char) val)/10+48);	
	POKE(COLOR_BASE+x+y*  ((unsigned short)XSize), _AQUARIUS_RED);		
	POKE(VIDEO_BASE+x+1+y*((unsigned short)XSize), ((unsigned char) val)%10+48);	
	POKE(COLOR_BASE+x+1+y*((unsigned short)XSize), _AQUARIUS_RED);	
}	


void print_u(unsigned char x, unsigned char y, unsigned short val)
{
	POKE(VIDEO_BASE+x+y*((unsigned short)XSize), (unsigned char) (val+48));
	POKE(COLOR_BASE+x+y*((unsigned short)XSize),  _AQUARIUS_RED);
	
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
	else //if(strlen(str)==2)
	{
		print_u(x,y,val);		
	}

}

void CLEAR_SCREEN(void)
{
	unsigned char i;

	clrscr();	
	#if !defined(NO_COLOR)
		for(i=0;i<251;++i)
		{
			POKE(COLOR_BASE+1+i,0);
			POKE(COLOR_BASE+250+i,0);
			POKE(COLOR_BASE+500+i,0);
			POKE(COLOR_BASE+749+i,0);
		}
	#endif
}


#endif
