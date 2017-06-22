#ifndef _ATARI_GRAPHICS
#define _ATARI_GRAPHICS

	#include <atari.h>
	#include <peekpoke.h>
	#include <atari.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>

	#define SCRMEM *(unsigned int*) 88
	
	 Image PLAYER_IMAGE;
	 Image GHOST_IMAGE;
	 Image DEAD_GHOST_IMAGE;
	 Image INVINCIBLE_GHOST_IMAGE;
	 Image BOMB_IMAGE;
	 Image POWERUP_IMAGE;
	 Image MISSILE_IMAGE;
	 Image GUN_IMAGE;


	void INIT_GRAPHICS(void)
	{

		//char * screen = (char *) SCRMEM;
		
		//_graphics(12);
		
	}
	 
	 
	// TODO: Sprite initialization (to be performed only once) should be separated from level generation
	void INIT_IMAGES(void)
	{		
	
		// TODO: Maybe somewhere else
		//_graphics(12);
			
		PLAYER_IMAGE._color = COLOR_BLUE;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_BLUE;
		POWERUP_IMAGE._color = COLOR_BLUE;
		GUN_IMAGE._color = COLOR_BLACK;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;


		GHOST_IMAGE._imageData = 'o';
		INVINCIBLE_GHOST_IMAGE._imageData = '+';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '*';
		POWERUP_IMAGE._imageData = 'S';
		GUN_IMAGE._imageData = '!';
		MISSILE_IMAGE._imageData = '.';
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

		GHOST_IMAGE._color = COLOR_BLACK;
		MISSILE_IMAGE._color = COLOR_BLACK;

	}


#endif // _ATARI_GRAPHICS