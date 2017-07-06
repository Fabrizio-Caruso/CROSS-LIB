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
 
 
#ifndef _C264_GRAPHICS
#define _C264_GRAPHICS

	#include <peekpoke.h>

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>

	
	Image PLAYER_IMAGE;
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
	
	Image GHOST_IMAGE;
	Image DEAD_GHOST_IMAGE;
	Image INVINCIBLE_GHOST_IMAGE;
	Image BOMB_IMAGE;
	Image POWERUP_IMAGE;
	Image MISSILE_IMAGE;
	Image GUN_IMAGE;

	Image LEFT_ENEMY_MISSILE_IMAGE;
	Image RIGHT_ENEMY_MISSILE_IMAGE;
	 	 
	void INIT_GRAPHICS(void)
	{
		unsigned short i;
		static char player_down[8] =  { 24, 36, 24,102,153, 24, 36,102};
		static char player_up[8] =    { 24, 60, 24,102,153, 24, 36,102};
		static char player_right[8] = { 24, 52, 25,118,152, 24, 20, 20};	
		static char player_left[8] =  { 24, 44,152,110, 25, 24, 40, 40};	
		static char ghost[8] =  {129,126,165,129,129,189,129,126};
		
		
		POKE(1177,62); // disable switch to RAM in PEEK
		for(i=0;i<1023;++i)
		{
			POKE(24576+2048+i,PEEK((unsigned long) (54272ul+(unsigned long) i)));
		}
		POKE(1177,63); // re-enable switch to RAM in PEEK
		POKE(65299ul,(PEEK(65299ul)&3)|((16+8+2)*4)); // change character base address to 8192
		POKE(65298ul,PEEK(65298ul)&251); // make graphics chip get characters from RAM
		
		for(i=0;i<8;++i)
		{
			POKE(24576+2048+320+i,player_down[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(24576+2048+320+8+i,player_up[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(24576+2048+320+8*2+i,player_right[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(24576+2048+320+8*3+i,player_left[i]);
		}

		for(i=0;i<8;++i)
		{
			POKE(24576+2048+320+8*4+i,ghost[i]);
		}
		// for(i=0;i<8;++i)
		// {
			// POKE(24576+320+8*2+i,player[i]);
		// }

		// for(i=0;i<8;++i)
		// {
			// POKE(24576+320+8*2+i,player[i]);
		// }

		// for(i=0;i<8;++i)
		// {
			// POKE(24576+320+8*3+i,player[i]);
		// }		
	}
	 
	 
	// TODO: Sprite initialization (to be performed only once) should be separated from level generation
	void INIT_IMAGES(void)
	{		
		PLAYER_IMAGE._color = COLOR_CYAN;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
		POWERUP_IMAGE._color = COLOR_BLUE;
		GUN_IMAGE._color = COLOR_BLUE;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;

		GHOST_IMAGE._imageData = ',';
		INVINCIBLE_GHOST_IMAGE._imageData = 'o';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '(';
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
		
		PLAYER_DOWN._imageData = '(';
		PLAYER_DOWN._color = COLOR_CYAN;
		PLAYER_UP._imageData = ')';
		PLAYER_UP._color = COLOR_CYAN;
		PLAYER_RIGHT._imageData = '*';
		PLAYER_RIGHT._color = COLOR_CYAN;
		PLAYER_LEFT._imageData = '+';
		PLAYER_LEFT._color = COLOR_CYAN;
	}


#endif // _C264_GRAPHICS