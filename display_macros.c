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
	#if defined(__C64__) && defined(C64_HARDWARE_SPRITE_VERSION) 
		short i;
		
		static const char data[] = {0,127,0,1,255,192,3,255,224,3,231,224,
		                           7,217,240,7,223,240,7,217,240,3,231,224,
								   3,255,224,3,255,224,2,255,160,1,127,64,
								   1,62,64,0,156,128,0,156,128,0,73,0,0,73,0,
								   0,62,0,0,62,0,0,62,0,0,28,0};
		
	#endif
		

	#if defined(__C64__) || defined(__C128__) || defined(__VIC20__)
		PLAYER_IMAGE._color = COLOR_BLUE;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_BLUE;
		POWERUP_IMAGE._color = COLOR_BLUE;
		GUN_IMAGE._color = COLOR_BLUE;
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


	#if defined(__C64__) && defined(C64_HARDWARE_SPRITE_VERSION)
		// Experimental
		POKE(53248u+21,4);
		POKE(2042,13);
		POKE(53248u+41,COLOR_BLUE); 

		// Even more experimental
		POKE(53248u+21,7); // Enable first 3 sprites (1+2+4)
		
		// Second sprite
		POKE(2041,13);
		POKE(53248u+40,6);

		// First sprite
		POKE(2040,13);
		POKE(53248u+39,COLOR_RED);
			
		POKE(53248u+0,0);
		POKE(53248u+1,0);
		
		for(i=0;i<63;++i)
		{
			POKE(832+i,data[i]);
		}

	#endif
}

