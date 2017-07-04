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

#if defined(__C64__)
	#include "c64/c64_redefined_characters.h"
#elif defined(__ATMOS__)
	#include "atmos/atmos_redefined_characters.h"
#elif defined(__ATARI__) || defined(__ATARIXL__)
	#include "atari/atari_graphics.h"
#elif defined(__NES__)
	#include "nes/nes_graphics.h"
#else		
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

	extern char YSize; 
	

	void INIT_GRAPHICS(void)
	{
	}
 
	void INIT_IMAGES(void)
	{		
		#if defined(__PET__) || defined(__APPLE2__) || defined(__APPLE2ENH__)
			PLAYER_IMAGE._color = COLOR_WHITE;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
			POWERUP_IMAGE._color = COLOR_WHITE;
			GUN_IMAGE._color = COLOR_WHITE;
			BOMB_IMAGE._color = COLOR_WHITE;
			DEAD_GHOST_IMAGE._color = COLOR_WHITE;
		#elif defined(__VIC20__)
			PLAYER_IMAGE._color = COLOR_YELLOW;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_BLUE;
			GUN_IMAGE._color = COLOR_BLUE;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;		
		#elif defined(__C16__) || defined(__PLUS4__)
			PLAYER_IMAGE._color = COLOR_WHITE;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
			POWERUP_IMAGE._color = COLOR_GRAY1;
			GUN_IMAGE._color = COLOR_GRAY1;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;
		#else
			PLAYER_IMAGE._color = COLOR_WHITE;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
			POWERUP_IMAGE._color = COLOR_WHITE;
			GUN_IMAGE._color = COLOR_WHITE;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;
		#endif
			

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
	}

#endif
