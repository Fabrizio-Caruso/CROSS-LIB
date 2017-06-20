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
		// Select video bank and set the character set pointer to point to $C800
		// where the redefined charset has been placed by the linker
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