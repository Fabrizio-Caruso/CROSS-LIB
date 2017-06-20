#ifndef __C64_REDEFINED_CHARACTERS
#define __C64_REDEFINED_CHARACTERS

	#include <peekpoke.h>

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
		// Select video bank and character set pointer
		POKE(56576u,0);
		POKE(53272u,2);
		POKE(648,192);
	}
	 
	 
	void INIT_IMAGES(void)
	{		
		PLAYER_IMAGE._color = COLOR_BLUE;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_BLUE;
		POWERUP_IMAGE._color = COLOR_BLUE;
		GUN_IMAGE._color = COLOR_BLACK;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
			
		GHOST_IMAGE._imageData = (char) 0x76;
		INVINCIBLE_GHOST_IMAGE._imageData = (char) 0x77;
		BOMB_IMAGE._imageData = (char) 0x78;
		PLAYER_IMAGE._imageData = '\'';
		POWERUP_IMAGE._imageData = (char) 0x7A;
		GUN_IMAGE._imageData = (char) 0x7B;
		MISSILE_IMAGE._imageData = (char) 0x7C;
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

		GHOST_IMAGE._color = COLOR_BLACK;
		MISSILE_IMAGE._color = COLOR_BLACK;

	}
	
#endif // C64_REDEFINED_CHARACTERS