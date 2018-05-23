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
	
	extern short points;
	extern short highScore;
#endif

#include "../input_macros.h"

#define _PLAYER '*'

// RED
#define _BOMB 'x'

// WHITE
#define _GHOST 'o'

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

#define _RIGHT_ENEMY_MISSILE '<'

#define _BUBBLE '^'

#include "../display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(NO_DEAD_GHOSTS)
	extern Image DEAD_GHOST_IMAGE;
#endif

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
#define COLOR_ADDR 0x9600

#if defined(ALT_PRINT) && !(defined(ALT_HIGHSCORE) || defined(ALT_DISPLAY_STATS))
unsigned short loc(unsigned char x, char y)
{
	return ((unsigned short) BASE_ADDR)+(x+X_OFFSET)+(unsigned char)(y+Y_OFFSET)*((unsigned short)XSize);
}
#endif

#if !defined(NO_COLOR)
	#if defined(ALT_PRINT) && !(defined(ALT_HIGHSCORE) || defined(ALT_DISPLAY_STATS))
		#define _DRAW(x,y,image) \
		do \
		{ \
			POKE(loc(x,y), image->_imageData); \
			POKE((unsigned short) (COLOR_ADDR+x+(unsigned short)(y+Y_OFFSET)*XSize),image->_color); \
		} \
		while(0)
		#define _DELETE(x,y) POKE(loc(x,y), 32)
	#else
		#define _DRAW(x,y,image) do {POKE(BASE_ADDR+x+y*22, image->_imageData); POKE(COLOR_ADDR+x+y*22, image->_color); } while(0)
		#define _DELETE(x,y) POKE(BASE_ADDR+x+y*22, 32)
	#endif
#else
	#define _DRAW(x,y,image) POKE(7680+x+y*22, image->_imageData)
	#define _DELETE(x,y) POKE(7680+x+y*22, 32)     
#endif


void INIT_GRAPHICS(void)
{
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
		#if !defined(NO_DEAD_GHOSTS)		
			DEAD_GHOST_IMAGE._color = COLOR_RED;
		#endif
		GHOST_IMAGE._color = COLOR_WHITE;		
		
		#if !defined(TINY_GAME)
			INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_GREEN;
			GUN_IMAGE._color = COLOR_BLUE;
			EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
			MISSILE_IMAGE._color = COLOR_BLUE;
		#endif

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
	#if !defined(NO_DEAD_GHOSTS)			
		DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;	
	#endif
	
	#if !defined(TINY_GAME)
		INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;

		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		MISSILE_IMAGE._imageData = _MISSILE;
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

#if defined(ALT_DISPLAY_STATS) || defined(ALT_HISCORE)
	#include <peekpoke.h>
	void _displayShort(unsigned short value) 
	{ 
		unsigned char i; 
		unsigned short tmp; 
		
		tmp = value; 
		
		for(i=1;i<6;++i) 
		{ 
			tmp -= POKE(7686-i,(unsigned char) ((tmp)%10)); 
			tmp/=10; 
			POKE(7686-i,PEEK(7686-i)+48); 
		} 
		POKE(7686,48); 	
	}
	
	#if defined(ALT_DISPLAY_STATS)
	void displayStats(void)
	{
		_displayShort(points);
	}
	#endif
	
	
	#if defined(ALT_HIGHSCORE) 
	void highScoreScreen(void)
	{
		_displayShort(highScore);
		POKE(7686+2,'h'); 
		POKE(7686+3,'i');
		// POKE(7686+5,'s'); POKE(7686+6,'c');		
		// POKE(7686+7,'o'); POKE(7686+8,'r');		
		// POKE(7686+9,'e');				
	}
	#endif
#endif

#if defined(ALT_PRINT) && !(defined(ALT_HIGHSCORE) || defined(ALT_DISPLAY_STATS))
void PRINT(unsigned char x, unsigned char y, char * str)
{
	unsigned char i = 0;
	while(str[i])
	{
		POKE(loc(x,y)+i,str[i]);
		// POKE(BASE_ADDR+x+(unsigned short)(y+Y_OFFSET)*40+i, str[i]); 
		// ZAP_SOUND();
		++i;
	}
}

void print_05u0(unsigned char x, unsigned char y, unsigned short val)
{
	unsigned char i;
	unsigned char digits[6];
	unsigned short tmp;

	tmp = val;
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (unsigned char) ((tmp)%10);
		tmp-= digits[i];
		tmp/=10;
	}
	
	for(i=0;i<6;++i)
	{
		POKE(loc(x,y)+i, (unsigned char) (digits[5-i])+48);
	}
}	

void print_02u(unsigned char x, unsigned char y, unsigned short val)
{
	POKE((loc(x,y)), ((unsigned char) val)/10+48);
	POKE((1+loc(x,y)), ((unsigned char) val)%10+48);
}	


void print_u(unsigned char x, unsigned char y, unsigned short val)
{
	POKE(loc(x,y),val+48);
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

#endif



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


