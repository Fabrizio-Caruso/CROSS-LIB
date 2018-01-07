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

#include<stdlib.h> 
//#include "character.h"

#define POKE(loc,val) bpoke((loc),(val));

#define VIDEO_MEMORY_BASE 0x4000

//extern Character bombs[BOMBS_NUMBER];

#include "../display_macros.h"

extern unsigned char XSize;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image GUN_IMAGE;
extern Image EXTRA_POINTS_IMAGE;
extern Image FREEZE_IMAGE;

extern Image MISSILE_IMAGE;

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	extern Image SUPER_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif


#if defined(REDEFINED_CHARS)
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
	Image PLAYER_UP;
	Image PLAYER_DOWN;	
#endif

extern unsigned char YSize; 
extern unsigned char XSize;

#define VG5K_BLACK 0
#define VG5K_RED 1
#define VG5K_GREEN 2
#define VG5K_YELLOW 3
#define VG5K_BLUE 4
#define VG5K_VIOLET 5
#define VG5K_CYAN 6
#define VG5K_WHITE 7

void INIT_IMAGES(void)
{		
	
	PLAYER_IMAGE._color = VG5K_CYAN;		
	GHOST_IMAGE._color = VG5K_WHITE;
	MISSILE_IMAGE._color = VG5K_WHITE;
	INVINCIBLE_GHOST_IMAGE._color = VG5K_YELLOW;
	POWERUP_IMAGE._color = VG5K_GREEN;
	GUN_IMAGE._color = VG5K_VIOLET;
		
	EXTRA_POINTS_IMAGE._color = VG5K_YELLOW;
	FREEZE_IMAGE._color = VG5K_CYAN;
			
	BOMB_IMAGE._color = VG5K_RED;
	DEAD_GHOST_IMAGE._color = VG5K_RED;
		
	GHOST_IMAGE._imageData = 'o';
	INVINCIBLE_GHOST_IMAGE._imageData = '+';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	POWERUP_IMAGE._imageData = 'S';
	GUN_IMAGE._imageData = '!';
	EXTRA_POINTS_IMAGE._imageData = '$';
	FREEZE_IMAGE._imageData = POWERUP_IMAGE._imageData;
	
	MISSILE_IMAGE._imageData = '.';
	
	DEAD_GHOST_IMAGE._imageData = 'o';

	GHOST_IMAGE._color = VG5K_WHITE;
	MISSILE_IMAGE._color = VG5K_WHITE;
	
	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = VG5K_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = VG5K_WHITE;	
		
		BUBBLE_IMAGE._imageData = '^';
		BUBBLE_IMAGE._color = VG5K_WHITE;

		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		EXTRA_LIFE_IMAGE._color = VG5K_YELLOW;

		INVINCIBILITY_IMAGE._imageData = 'V';
		INVINCIBILITY_IMAGE._color = VG5K_YELLOW;	

		SUPER_IMAGE._imageData = 'H';
		SUPER_IMAGE._color = VG5K_RED;
	#endif
}



void INIT_GRAPHICS(void)
{
	{
		unsigned char i;	
		for(i=0;i<24;++i)
		{
			POKE(VIDEO_MEMORY_BASE+80*i,32);
			POKE(VIDEO_MEMORY_BASE+1+80*i,1);
		}	
		//POKE(0x47FD,0);
	}	
}



#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_draw_ch(x,y,'X',VG5K_WHITE);
	}
#endif

void no_cursor(void)
{	
	#asm
	jr clean_cursor
	ef9345:
		defb 0x04,0x20,0x82,0x29,0x00
	clean_cursor:
		ld hl,ef9345
		ld ix,$47FA
		call 0x00AD
	#endasm		
}

void CLEAR_SCREEN()
{
	no_cursor();
	clrscr();
	INIT_GRAPHICS();
}		

void DRAW_HORIZONTAL_LINE(unsigned char x, unsigned char y, unsigned char length)
{
	unsigned char i; 
	for(i=0;i<length;++i)
	{ 
		_draw_ch(x+i,y,'-',VG5K_WHITE); 
	} 
}

void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length)
{ 
	unsigned char i; 
	for(i=0;i<length;++i)
	{ 
		_draw_ch(x,y+i,'|',VG5K_WHITE); 
	} 
}

void _draw_ch_aux(int chCol, int xy)
{		
	#asm
	
	pop bc   ; bc = ret address
	pop hl   ; hl = int b
	pop de  ; de = int a

	push de    ; now restore stack
	push hl
	push bc
	
	ld ix,$47FA	
	
	call 0x0092	
	
	#endasm	
}

void _draw_ch(unsigned char x, unsigned char y, unsigned char ch, unsigned char col)
{
	no_cursor();		
		
	{			
		int xy = ((Y_OFFSET+y+7)<<8) | (X_OFFSET+x);
		int chCol = (ch<<8) | col;
		
		_draw_ch_aux(chCol,xy);
	}
}

void _draw(unsigned char x,unsigned char y,Image * image) 
{
	_draw_ch(x,y,image->_imageData, image->_color);
}

void _delete(unsigned char x, unsigned char y)  
{
	_draw_ch(x,y,32, 0);	
}



void _blink_draw(unsigned char x,unsigned char y,Image * image, unsigned char *blinkCounter)
{
		_draw_ch(x,y, image->_imageData, image->_color);
		if(*blinkCounter) 
		{
			_draw_ch(x,y,image->_imageData, image->_color);
			*blinkCounter=0;
		} 
		else 
		{
			_draw_ch(x,y,32, 0);
			*blinkCounter=1;
		}
}


