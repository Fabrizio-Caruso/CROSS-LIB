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
 
#include "display_macros.h"

 Image PLAYER_IMAGE;
 Image GHOST_IMAGE;
 Image DEAD_GHOST_IMAGE;
 Image INVINCIBLE_GHOST_IMAGE;
 Image BOMB_IMAGE;
 Image POWERUP_IMAGE;
 Image MISSILE_IMAGE;
 Image GUN_IMAGE;

void initImages(void)
{
PLAYER_IMAGE._imageData = '*';
#if defined(__C64__) || defined(__C128__) || defined(__VIC20__)
	PLAYER_IMAGE._color = COLOR_BLUE;
	INVINCIBLE_GHOST_IMAGE._color = COLOR_BLUE;
	POWERUP_IMAGE._color = COLOR_BLUE;
	GUN_IMAGE._color = COLOR_BLUE;
	BOMB_IMAGE._color = COLOR_RED;
	DEAD_GHOST_IMAGE._color = COLOR_RED;
#elif defined(__PET__) || defined(__APPLE2__)
	BOMB_IMAGE._color = COLOR_BLACK;
	DEAD_GHOST_IMAGE._color = COLOR_BLACK;
#else
	PLAYER_IMAGE._color = COLOR_BLACK;
	INVINCIBLE_GHOST_IMAGE._color = COLOR_BLACK;
	POWERUP_IMAGE._color = COLOR_BLACK;
	GUN_IMAGE._color = COLOR_BLACK;
	BOMB_IMAGE._color = COLOR_RED;
	DEAD_GHOST_IMAGE._color = COLOR_RED;
#endif
	
GHOST_IMAGE._imageData = 'O';
GHOST_IMAGE._color = COLOR_BLACK;

// TODO: Maybe a different character could be used
DEAD_GHOST_IMAGE._imageData = 'X';


INVINCIBLE_GHOST_IMAGE._imageData = '+';


BOMB_IMAGE._imageData = 'X';


POWERUP_IMAGE._imageData = 'P';


MISSILE_IMAGE._imageData = 'O';
MISSILE_IMAGE._color = COLOR_BLACK;

GUN_IMAGE._imageData = '!';

}

