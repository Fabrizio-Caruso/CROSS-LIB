#ifndef _ATARI_MODE1_GRAPHICS
#define _ATARI_MODE1_GRAPHICS
	#include <conio.h>
	#include <atari.h>
	#include <peekpoke.h>
	#include <atari.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>

	#define GRAPHICS_MODE (1+16)
	//#define GRAPHICS_MODE 0
		
	Image PLAYER_IMAGE;
	Image GHOST_IMAGE;
	Image DEAD_GHOST_IMAGE;
	Image INVINCIBLE_GHOST_IMAGE;
	Image BOMB_IMAGE;
	Image POWERUP_IMAGE;
	Image MISSILE_IMAGE;
	Image GUN_IMAGE;

	Image LEFT_ENEMY_MISSILE_IMAGE;
	Image RIGHT_ENEMY_MISSILE_IMAGE;

	Image BUBBLE_IMAGE;
	 
	Image EXTRA_POINTS_IMAGE;
	Image EXTRA_LIFE_IMAGE;
	Image INVINCIBILITY_IMAGE;
	
	extern char YSize; 
	 
	void INIT_GRAPHICS(void)
	{

		// Mode 12 with no last monochromatic lines (12+16)
		_graphics(GRAPHICS_MODE);
		SET_TEXT_COLOR(TEXT_COLOR);
	
		// SET_BORDER_COLOR(BORDER_COLOR);
	
		// SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
		//_setcolor(TGI_COLOR_BLACK,TGI_COLOR_WHITE,TGI_COLOR_YELLOW);
		
		// _setcolor_low(TGI_COLOR_GREEN, TGI_COLOR_BLUE);
		// _setcolor(0,COLOR_BLACK,8);
		// _setcolor(1,COLOR_WHITE,8);
		// _setcolor(2,COLOR_RED,8);
		// _setcolor(3,COLOR_BLUE,8);
		// _setcolor(4, COLOR_YELLOW,8);
	}
	 
	 
	// TODO: Sprite initialization (to be performed only once) should be separated from level generation
	void INIT_IMAGES(void)
	{		
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
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_WHITE;

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
	}

	#if defined(FULL_GAME)
		void DRAW_BROKEN_WALL(char x, char y)
		{
			gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
			(void) textcolor (COLOR_WHITE);
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
		(void) textcolor (image->_color);
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