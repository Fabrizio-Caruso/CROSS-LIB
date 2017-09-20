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

	#define _PLAYER_DOWN 60
	#define _PLAYER_UP 61
	#define _PLAYER_RIGHT 62
	#define _PLAYER_LEFT 63
	
	#define _GHOST 59
	#define _INVINCIBLE_GHOST 30
	#define _BOMB 29
	#define _POWERUP 28 // S
	#define _GUN 11
	#define _MISSILE 10
	
	#define _LEFT_ENEMY_MISSILE 9
	#define _RIGHT_ENEMY_MISSILE 8	
	#define _BUBBLE 7

	// 128 -> YELLOW; 32 -> RED; 160-> BLUE; 0 -> WHITE
	#define _ATARI_MODE1_WHITE 0
	#define _ATARI_MODE1_WHITE2	64
	#define _ATARI_MODE1_RED 32
	#define _ATARI_MODE1_YELLOW 128
	#define _ATARI_MODE1_BLUE 160
	
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
	
	extern char YSize; 
	 
	void redefine(unsigned char * loc, const char *new_char)
	{
		unsigned char i;
		for(i=0;i<8;++i)
		{
			POKE(loc+i,new_char[i]);
		}
	}
	
	void INIT_GRAPHICS(void)
	{	
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
		
		extern char _FONT_START__[];
		unsigned char *CHBAS = (unsigned char *)0x2f4;
		
		// Mode 12 with no last monochromatic lines (12+16)
		_graphics(GRAPHICS_MODE);
		
		_setcolor_low(0, TGI_COLOR_RED);
		_setcolor_low(1, TGI_COLOR_WHITE);
		_setcolor_low(2, TGI_COLOR_CYAN);
		_setcolor_low(3, TGI_COLOR_YELLOW);
		_setcolor_low(4, TGI_COLOR_BLACK);

		// for(j=0;j<1023;++j)
		// {
			// POKE(CHARS_BASE+j,PEEK(ROM_BASE+j));
		// }
		// POKE(756,CHARS_BASE/256); // Point to chars_base
		
		memcpy(_FONT_START__, (void *)0xE000, 512);
		
		/* modify your font at _FONT_START__, etc, then set the new font: */		
			
		redefine(_FONT_START__+_PLAYER_DOWN*8, player_down);
		redefine(_FONT_START__+_PLAYER_UP*8, player_up);		
		redefine(_FONT_START__+_PLAYER_RIGHT*8, player_right);
		redefine(_FONT_START__+_PLAYER_LEFT*8, player_left);

		redefine(_FONT_START__+_GHOST*8, ghost);
		redefine(_FONT_START__+_POWERUP*8, powerUp);		
		redefine(_FONT_START__+_INVINCIBLE_GHOST*8, invincible_ghost);
		redefine(_FONT_START__+_BOMB*8, bomb);	
		
		
		redefine(_FONT_START__+_MISSILE*8, missile);
		redefine(_FONT_START__+_GUN*8, gun);
		
		redefine(_FONT_START__+_LEFT_ENEMY_MISSILE*8, missile_left);
		redefine(_FONT_START__+_RIGHT_ENEMY_MISSILE*8, missile_right);		
		redefine(_FONT_START__+_BUBBLE*8, bubble);
		redefine(_FONT_START__+_INVINCIBLE_GHOST*8, invincible_ghost);			
		
		*CHBAS = ((int)_FONT_START__ >> 8);  /* enable the new font */	
	}
	 
	 
	 
	// TODO: Sprite initialization (to be performed only once) should be separated from level generation
	void INIT_IMAGES(void)
	{		
		// TODO: Do something with the colors
		PLAYER_IMAGE._color = 160; // BLUE
		INVINCIBLE_GHOST_IMAGE._color = 128; // 128 -> YELLOW; 32 -> RED; 160-> BLUE; 0 -> WHITE
		POWERUP_IMAGE._color = 128; //
		GUN_IMAGE._color = 128; // 
		BOMB_IMAGE._color = 32; // RED
		DEAD_GHOST_IMAGE._color = 128;
		GHOST_IMAGE._color = 64; // WHITE
		MISSILE_IMAGE._color = 32;
	
		PLAYER_DOWN._imageData = _PLAYER_DOWN+PLAYER_IMAGE._color;
		PLAYER_UP._imageData = _PLAYER_UP+PLAYER_IMAGE._color;
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT+PLAYER_IMAGE._color;		
		PLAYER_LEFT._imageData = _PLAYER_LEFT+PLAYER_IMAGE._color;
		PLAYER_IMAGE._imageData = PLAYER_DOWN._imageData;
		
		POWERUP_IMAGE._imageData = _POWERUP + POWERUP_IMAGE._color;
		GUN_IMAGE._imageData = _GUN + GUN_IMAGE._color;
		
		GHOST_IMAGE._imageData = _GHOST + GHOST_IMAGE._color;
		INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST + INVINCIBLE_GHOST_IMAGE._color;
		BOMB_IMAGE._imageData = _BOMB + BOMB_IMAGE._color;		
		MISSILE_IMAGE._imageData = _MISSILE + MISSILE_IMAGE._color;
		DEAD_GHOST_IMAGE._imageData = GHOST_IMAGE._imageData + DEAD_GHOST_IMAGE._color;

		#if defined(FULL_GAME)
			LEFT_ENEMY_MISSILE_IMAGE._color = 0;
			RIGHT_ENEMY_MISSILE_IMAGE._color = 0;	
			BUBBLE_IMAGE._color = 0;	
			EXTRA_POINTS_IMAGE._color = 0;
			EXTRA_LIFE_IMAGE._color = 0;
			INVINCIBILITY_IMAGE._color = 0;
			
			LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_ENEMY_MISSILE + LEFT_ENEMY_MISSILE_IMAGE._color;

			RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_ENEMY_MISSILE + RIGHT_ENEMY_MISSILE_IMAGE._color;

			BUBBLE_IMAGE._imageData = _BUBBLE;
			
			EXTRA_POINTS_IMAGE._imageData = '$';

			EXTRA_LIFE_IMAGE._imageData = _PLAYER_DOWN + 0;

			INVINCIBILITY_IMAGE._imageData = 'V';

		#endif
	}

	#if defined(FULL_GAME)
		void DRAW_BROKEN_WALL(char x, char y)
		{
			_draw(x,y,&BOMB_IMAGE);
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