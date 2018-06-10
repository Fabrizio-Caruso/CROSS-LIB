/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/
 
#include <peekpoke.h>
#include "display_macros.h"


 Image PLAYER_IMAGE;
 Image GHOST_IMAGE;
 Image DEAD_GHOST_IMAGE;
 Image INVINCIBLE_GHOST_IMAGE;
 Image BOMB_IMAGE;
 Image POWERUP_IMAGE;
 Image MISSILE_IMAGE;
 Image GUN_IMAGE;


 
// TODO: Sprite initialization (to be performed only once) should be separated from level generation
void INIT_IMAGES(void)
{
	#if (defined(__C64__) || defined(__C128__))&& defined(C64_HARDWARE_SPRITE_VERSION) 
		short i;
		
		/*
		static const char player_data[] = {0,127,0,1,255,192,3,255,224,3,231,224,
		                           7,217,240,7,223,240,7,217,240,3,231,224,
								   3,255,224,3,255,224,2,255,160,1,127,64,
								   1,62,64,0,156,128,0,156,128,0,73,0,0,73,0,
								   0,62,0,0,62,0,0,62,0,0,28,0};
								   
		static const char invincible_enemy_data[] = {0,127,0,1,255,192,3,255,224,3,231,224,
		                           7,217,240,7,223,240,7,217,240,3,231,224,
								   3,255,224,3,255,224,2,255,160,1,127,64,
								   1,62,64,0,156,128,0,156,128,0,73,0,0,73,0,
								   0,62,0,0,62,0,0,62,0,0,28,0};		
*/
								   
	static const char player_data[] = 
	{	
		0x00,0x00,0x00,0x0c,0x00,0x00,0x3f,0x00,0xc0,0x37,0x03,0xf0,0x37,0x03,0x70,0x3f,
		0x03,0x70,0x1d,0xa9,0xd0,0x06,0xea,0x40,0x02,0xea,0x00,0x06,0xaa,0x40,0x06,0xaa,
		0x40,0x00,0xa8,0x00,0x08,0x54,0x80,0x0a,0xaa,0x80,0x06,0xaa,0x40,0x05,0x55,0x40,
		0x01,0x55,0x00,0x00,0x44,0x00,0x00,0x44,0x10,0x14,0x44,0x50,0x15,0x45,0x40,0x85
	};

	static const char invincible_enemy_data[] =
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0x00,0x02,
		0xeb,0x80,0x0b,0x69,0xe0,0x0b,0x69,0xe0,0x06,0xaa,0x90,0x05,0x55,0x50,0x01,0x55,
		0x40,0x00,0x55,0x00,0x00,0x55,0x00,0x04,0x41,0x10,0x05,0x41,0x50,0x04,0x41,0x10,
		0x04,0x00,0x10,0x14,0x00,0x14,0x04,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x83
	};
	
	static const char powerUp_data[] =
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xa8,0x00,0x03,
		0xea,0x00,0x03,0xea,0x00,0x0b,0x56,0x80,0x0b,0x6a,0x90,0x0b,0x57,0x90,0x0a,0xa7,
		0x90,0x0a,0x57,0x90,0x02,0xaf,0x10,0x02,0xaf,0x50,0x00,0xa9,0x50,0x00,0x55,0x40,
		0x00,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83
	};
	
	// No memory left for this
	/*
	static const char gun_data[] = 
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x0a,0xef,0xf0,0x06,0xaa,0xa0,0x05,0x55,0x50,0x05,0x55,0x00,0x05,0x10,
		0x00,0x05,0x40,0x00,0x05,0x00,0x00,0x05,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8e

	};
	*/
	#endif
		

	#if defined(__C64__) || defined(__C128__) || defined(__VIC20__)
		PLAYER_IMAGE._color = COLOR_BLUE;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_BLUE;
		POWERUP_IMAGE._color = COLOR_BLUE;
		GUN_IMAGE._color = COLOR_BLACK;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
	#elif defined(__PET__) || defined(__APPLE2__)
		PLAYER_IMAGE._color = COLOR_BLACK;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_BLACK;
		POWERUP_IMAGE._color = COLOR_BLACK;
		GUN_IMAGE._color = COLOR_BLACK;
		BOMB_IMAGE._color = COLOR_BLACK;
		DEAD_GHOST_IMAGE._color = COLOR_BLACK;
	#elif defined(__C16__) || defined(__PLUS4__)
		PLAYER_IMAGE._color = COLOR_BLACK;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_BLACK;
		POWERUP_IMAGE._color = COLOR_GRAY1;
		GUN_IMAGE._color = COLOR_GRAY1;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
	#else
		PLAYER_IMAGE._color = COLOR_BLACK;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_BLACK;
		POWERUP_IMAGE._color = COLOR_BLACK;
		GUN_IMAGE._color = COLOR_BLACK;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
	#endif
		
	PLAYER_IMAGE._imageData = '*';

	GHOST_IMAGE._imageData = 'O';
	GHOST_IMAGE._color = COLOR_BLACK;

	// TODO: Maybe a different character could be used
	DEAD_GHOST_IMAGE._imageData = 'X';

	INVINCIBLE_GHOST_IMAGE._imageData = '+';

	BOMB_IMAGE._imageData = 'X';

	POWERUP_IMAGE._imageData = 'P';

	MISSILE_IMAGE._imageData = '.';
	MISSILE_IMAGE._color = COLOR_BLACK;

	GUN_IMAGE._imageData = '!';

	// This code should be only run once
	#if (defined(__C64__) || defined(__C128__))&& defined(C64_HARDWARE_SPRITE_VERSION) 
		// Experimental
	
	
		// Even more experimental
		POKE(53248u+21,1+2+4); // Enable first 3 sprites (1+2+4)
		POKE(53248u+28,1+2+4); // Enable multicolor on first 3 sprites

		POKE(53248u+32,COLOR_BROWN); 
		POKE(53248u+33,COLOR_BROWN); 
		
		POKE(53248u+37,COLOR_BLACK);
		POKE(53248u+38,COLOR_WHITE);
		
		
		// PLAYER Sprite (Third sprite)
		POKE(2042,13);
		POKE(53248u+41,COLOR_GREEN); 
		
		for(i=0;i<63;++i)
		{
			POKE(832+i,player_data[i]);
		}	
		
		
		// POWER-UP SPRITE (Second sprite)
		POKE(2041,15);
		POKE(53248u+40,COLOR_CYAN);
		
		// Initial positioning
		POKE(53248u+2,0);
		POKE(53248u+3,0);
	
		for(i=0;i<63;++i)
		{
			POKE(832+64*2+i,powerUp_data[i]);
		}
		
		// INVINCIBLE ENEMY SPRITE (First sprite)
		POKE(2040,14);
		POKE(53248u+39,COLOR_RED);
			
		// Initial positioning
		POKE(53248u+0,0);
		POKE(53248u+1,0);
		for(i=0;i<63;++i)
		{
			POKE(832+64+i,invincible_enemy_data[i]);
		}
		
		// No memory left for this
		
		/*
		// GUN SPRITE (Fourth sprite)
		POKE(2043,12);
		POKE(53248u+42,COLOR_BLUE);
			
		// Initial positioning
		POKE(53248u+6,0);
		POKE(53248u+7,0);
		
		for(i=0;i<63;++i)
		{
			POKE(832+64*3+i,gun_data[i]);
		}
		*/
	#endif
}

