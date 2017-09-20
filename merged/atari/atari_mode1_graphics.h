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
		
	extern char YSize; 
	 
	void INIT_GRAPHICS(void)
	{
		// Mode 12 with no last monochromatic lines (12+16)
		_graphics(GRAPHICS_MODE);
		_setcolor_low(0, COLOR_RED);
		_setcolor_low(1, TGI_COLOR_WHITE);
		_setcolor_low(2, TGI_COLOR_BLUE);
		_setcolor_low(3, TGI_COLOR_YELLOW);
		_setcolor_low(4, TGI_COLOR_BLACK);
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
		INVINCIBLE_GHOST_IMAGE._imageData = '+'-160;
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = 170; // *
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
		(void) textcolor (image->_color);
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