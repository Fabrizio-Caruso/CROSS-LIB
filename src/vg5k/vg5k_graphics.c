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

#define POKE(loc,val) bpoke((loc),(val));

#define VIDEO_MEMORY_BASE 0x4000

#include "../display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image GUN_IMAGE;
extern Image EXTRA_POINTS_IMAGE;

extern Image MISSILE_IMAGE;

#if defined(FULL_GAME)
	extern Image FREEZE_IMAGE;

	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image CHASE_IMAGE;
	extern Image SUPER_IMAGE;
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif


#if defined(REDEFINED_CHARS)
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
	Image PLAYER_UP;
	Image PLAYER_DOWN;	
#endif


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
	#if !defined(NO_COLOR)
	PLAYER_IMAGE._color = VG5K_CYAN;		
	GHOST_IMAGE._color = VG5K_WHITE;
	MISSILE_IMAGE._color = VG5K_WHITE;
	INVINCIBLE_GHOST_IMAGE._color = VG5K_YELLOW;
	POWERUP_IMAGE._color = VG5K_GREEN;
	GUN_IMAGE._color = VG5K_VIOLET;
		
	EXTRA_POINTS_IMAGE._color = VG5K_YELLOW;
			
	BOMB_IMAGE._color = VG5K_RED;
	DEAD_GHOST_IMAGE._color = VG5K_RED;
		
	#endif
	
	GHOST_IMAGE._imageData = 'o';
	INVINCIBLE_GHOST_IMAGE._imageData = '+';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	POWERUP_IMAGE._imageData = 'S';
	GUN_IMAGE._imageData = '!';
	EXTRA_POINTS_IMAGE._imageData = '$';
	
	MISSILE_IMAGE._imageData = '.';
	
	DEAD_GHOST_IMAGE._imageData = 'o';

	#if !defined(NO_COLOR)
	GHOST_IMAGE._color = VG5K_WHITE;
	MISSILE_IMAGE._color = VG5K_WHITE;
	#endif
	
	#if defined(FULL_GAME)
		#if !defined(NO_COLOR)	
			FREEZE_IMAGE._color = VG5K_CYAN;	
			LEFT_ENEMY_MISSILE_IMAGE._color = VG5K_WHITE;
			RIGHT_ENEMY_MISSILE_IMAGE._color = VG5K_WHITE;	
			BUBBLE_IMAGE._color = VG5K_WHITE;	
			EXTRA_LIFE_IMAGE._color = VG5K_YELLOW;
			INVINCIBILITY_IMAGE._color = VG5K_YELLOW;			
			CHASE_IMAGE._color = VG5K_RED;		
			SUPER_IMAGE._color = VG5K_RED;
			CONFUSE_IMAGE._color = VG5K_RED;			
			ZOMBIE_IMAGE._color = VG5K_YELLOW;	
		#endif

		FREEZE_IMAGE._imageData = POWERUP_IMAGE._imageData;
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		
		BUBBLE_IMAGE._imageData = '^';
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'V';
		CHASE_IMAGE._imageData = '.';
		SUPER_IMAGE._imageData = 'H';
		CONFUSE_IMAGE._imageData = '+';
		ZOMBIE_IMAGE._imageData = 'O';

	#endif
}



void INIT_GRAPHICS(void)
{
	// unsigned char i;	
	// for(i=0;i<24;++i)
	// {
		// POKE((VIDEO_MEMORY_BASE+80*i),32);
		// POKE((VIDEO_MEMORY_BASE+1+80*i),1);
	// }	
	
		//POKE(0x47FD,0);	
}



#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_draw_ch(x,y,'X',VG5K_WHITE);
	}
#endif


void CLEAR_SCREEN()
{
	// no_cursor();
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

#if defined(ASM_DISPLAY)
	void _draw_ch_aux(int chCol, int xy)
	{		
		#if defined(SDCC)
			__asm
		#else
			#asm
		#endif
		pop bc   ; bc = ret address
		pop hl   ; hl = int b
		pop de  ; de = int a

		push de    ; now restore stack
		push hl
		push bc
		
		;ld ix,$47FA	
		
		call 0x0092	
		
		#if defined(SDCC)
			__endasm;
		#else
			#endasm
		#endif
	}
#endif


void _draw_ch(unsigned char x, unsigned char y, unsigned char ch, unsigned char col)
{
	
	#if defined(ASM_DISPLAY)	
	{			
		int xy = ((Y_OFFSET+y+7)<<8) | (X_OFFSET+x);
		int chCol = (ch<<8) | col;
		
		_draw_ch_aux(chCol,xy);
	}
	#else
		gotoxy(x+X_OFFSET,y+Y_OFFSET);
		textcolor(col);
		cputc(ch);
	#endif
}

void _draw(unsigned char x,unsigned char y,Image * image) 
{
	#if !defined(NO_COLOR)
		_draw_ch(x,y,image->_imageData, image->_color);
	#else
		_draw_ch(x,y,image->_imageData, VG5K_WHITE);		
	#endif
}

void _delete(unsigned char x, unsigned char y)  
{
	_draw_ch(x,y,32, 0);	
}


#if !defined(NO_BLINKING)
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
#endif

