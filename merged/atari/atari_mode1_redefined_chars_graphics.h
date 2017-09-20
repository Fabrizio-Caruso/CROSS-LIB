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

#ifndef _ATARI_MODE1_GRAPHICS
#define _ATARI_MODE1_GRAPHICS
	#include <conio.h>
	#include <peekpoke.h>
	#include <atari.h>

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>

	#define GRAPHICS_MODE (1+16)
	//#define GRAPHICS_MODE 0
	
	#define ROM_BASE 57344u		
	#define CHARS_BASE 8192

	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
	
	extern char YSize; 
	 
	void redefine(char old_char, const char *new_char)
	{
		unsigned char i;
		for(i=0;i<8;++i)
		{
			POKE(CHARS_BASE+old_char*8+i,new_char[i]);
		}
	}
	
	void INIT_GRAPHICS(void)
	{	
		unsigned int j;
		
		static const char player_down[8] =      { 24, 36, 24,102,153, 24, 36,102};
		static const char player_up[8] =        { 24, 60, 24,102,153, 24, 36,102};
		static const char player_right[8] =     { 24, 52, 25,118,152, 24, 20, 20};	
		static const char player_left[8] =      { 24, 44,152,110, 25, 24, 40, 40};
		static const char ghost[8] =            {129,126,165,129,129,189,129,126};
		static const char missile_right[8] =    {  0,  0, 15,252,252, 15,  0,  0};
		static const char missile_left[8] =     {  0,  0,240, 63, 63,240,  0,  0};
		static const char invincible_ghost[8] = { 60, 66,165,129, 90, 36, 36, 60};
		static const char gun[8] =              {  0,128,126,200,248,192,128,  0};
		static const char powerUp[8] =          {  0, 60, 54,223,231,122, 36, 24};
		static const char missile[8] =          {  0,  0,  8, 56, 28, 16,  0,  0};
		static const char bomb[8] =             { 60, 66,165,153,153,165, 66, 60};
		static const char bubble[8] =           { 24, 60, 60, 60,126, 90, 66, 66};
		
		// Mode 12 with no last monochromatic lines (12+16)
		_graphics(GRAPHICS_MODE);
		
		_setcolor_low(0, TGI_COLOR_RED);
		_setcolor_low(1, TGI_COLOR_WHITE);
		_setcolor_low(2, TGI_COLOR_CYAN);
		_setcolor_low(3, TGI_COLOR_YELLOW);
		_setcolor_low(4, TGI_COLOR_BLACK);

		for(j=0;j<1023;++j)
		{
			POKE(CHARS_BASE+j,PEEK(ROM_BASE+j));
		}
		POKE(756,CHARS_BASE/256); // Point to chars_base

			// redefine(24576+4096+1024+296+8,player_up);		
			// redefine(24576+4096+1024+296+8*2,player_right);
			// redefine(24576+4096+1024+296+8*3,player_left);
			
		redefine('*' - 160,player_down);
	
		redefine('>',missile_left);
		redefine('<',missile_right);	
	
		redefine('+',invincible_ghost);
		redefine('!' - 160,gun);

		redefine('S' - 64-32,powerUp);
		redefine('.',missile);

		redefine('X',bomb);
		redefine('o',ghost);
		redefine('^',bubble);

	}
	 
	 
	 
	// TODO: Sprite initialization (to be performed only once) should be separated from level generation
	void INIT_IMAGES(void)
	{		
		// TODO: Do something with the colors
		PLAYER_IMAGE._color = COLOR_WHITE;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
		POWERUP_IMAGE._color = COLOR_YELLOW;
		GUN_IMAGE._color = COLOR_BROWN;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;

		GHOST_IMAGE._imageData = 'o';
		INVINCIBLE_GHOST_IMAGE._imageData = '+';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '*' - 160;
		POWERUP_IMAGE._imageData = 'S' - 64-32;
		GUN_IMAGE._imageData = '!' - 160;
		MISSILE_IMAGE._imageData = '.';
		DEAD_GHOST_IMAGE._imageData = 'O';

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
			EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
			EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData + 160;
			EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
			INVINCIBILITY_IMAGE._imageData = 'V';
			INVINCIBILITY_IMAGE._color = COLOR_WHITE;
		#endif
	}

	#if defined(FULL_GAME)
		void DRAW_BROKEN_WALL(char x, char y)
		{
			gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
			cputc('X');
		}
	#endif
	
	void _draw(char x, char y, Image * image) 
	{
		if((y+Y_OFFSET)%2==1)
		{
			gotoxy((x+20+X_OFFSET),(y+Y_OFFSET)/2);
		}
		else
		{
			gotoxy((x+X_OFFSET),(y+Y_OFFSET)/2);
		}				
		cputc(image->_imageData); 
	};	
	
	void _blink_draw(char x, char y, Image * image, unsigned char * blinkCounter) 
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
	};
	
	void _delete(char x, char y) 
	{
		if((y+Y_OFFSET)%2==1)
		{
			gotoxy((x+20+X_OFFSET),(y+Y_OFFSET)/2);
		}
		else
		{
			gotoxy((x+X_OFFSET),(y+Y_OFFSET)/2);
		}				
		cputc(' ');
	};	
	
	void PRINT(unsigned char x, unsigned char y, char * str)
	{ 
		if((y+Y_OFFSET)%2==1) 
		{ 
			gotoxy(x+20+X_OFFSET,(y+Y_OFFSET)/2); 
		} 
		else 
		{ 
			gotoxy(x+X_OFFSET, (y+Y_OFFSET)/2); 
		} 
		cputs(str); 
	};	
#endif // _ATARI_MODE1_GRAPHICS