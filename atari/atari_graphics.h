#ifndef _ATARI_GRAPHICS
#define _ATARI_GRAPHICS

	#include <atari.h>
	#include <peekpoke.h>
	#include <atari.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>

	#define GRAPHICS_MODE 0
	
	//#define SCRMEM *(unsigned int*) 88
	
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
	 extern char YSize; 
	 
	void INIT_GRAPHICS(void)
	{

		// Mode 12 with no last monochromatic lines (12+16)
		_graphics(GRAPHICS_MODE);
		SET_TEXT_COLOR(TEXT_COLOR);
	
		SET_BORDER_COLOR(BORDER_COLOR);
	
		SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
		//_setcolor(TGI_COLOR_BLACK,TGI_COLOR_WHITE,TGI_COLOR_RED);
		//_setcolor_low(TGI_COLOR_GREEN, TGI_COLOR_BLUE);
	}
	 
	 
	// TODO: Sprite initialization (to be performed only once) should be separated from level generation
	void INIT_IMAGES(void)
	{		
		PLAYER_IMAGE._color = COLOR_WHITE;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
		POWERUP_IMAGE._color = COLOR_WHITE;
		GUN_IMAGE._color = COLOR_WHITE;
		BOMB_IMAGE._color = COLOR_WHITE;
		DEAD_GHOST_IMAGE._color = COLOR_WHITE;

		GHOST_IMAGE._imageData = 'o';
		INVINCIBLE_GHOST_IMAGE._imageData = '+';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '*';
		POWERUP_IMAGE._imageData = 'S';
		GUN_IMAGE._imageData = '!';
		MISSILE_IMAGE._imageData = '.';
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_WHITE;

		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	

		BUBBLE_IMAGE._imageData = 'o';
		BUBBLE_IMAGE._color = COLOR_WHITE;		
	}


#endif // _ATARI_GRAPHICS