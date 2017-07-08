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

#ifndef _ATMOS_REDEFINED_CHARACTERS
#define _ATMOS_REDEFINED_CHARACTERS

	#include <peekpoke.h>

	 Image PLAYER_IMAGE;
	 Image GHOST_IMAGE;
	 Image DEAD_GHOST_IMAGE;
	 Image INVINCIBLE_GHOST_IMAGE;
	 Image BOMB_IMAGE;
	 Image POWERUP_IMAGE;
	 Image MISSILE_IMAGE;
	 Image GUN_IMAGE;

	 Image PLAYER_RIGHT;
	 Image PLAYER_LEFT;
	 Image PLAYER_UP;
	 Image PLAYER_DOWN;
	 
	 Image LEFT_ENEMY_MISSILE_IMAGE;
	 Image RIGHT_ENEMY_MISSILE_IMAGE;
	 
	 Image BUBBLE_IMAGE;
	 extern char YSize; 
	 
	void INIT_IMAGES(void)
	{		
				
		unsigned char i;
		const unsigned char player[] =                {12,18,12,51,45,12,18,51};
		const unsigned char ghost[]  =                {33,30,33,51,33,45,33,30};
		const unsigned char bomb[]  =                 {30,33,51,45,45,51,33,30};
		const unsigned char powerUp[]  =              { 0,30,51,55,51,26,18,12};
		const unsigned char invincible_ghost[]  =     {30,33,51,33,33,18,18,12};
		const unsigned char gun[]  =                  { 0,32,31,40,56,32, 0, 0};
		const unsigned char missile[]  =              { 0, 0, 4,28,14, 8, 0, 0};
		
		const unsigned char player_left[] =           {12,22,12,62,13,12,20,20};
		const unsigned char player_right[] =          {12,26,12,31,44,12,10,10};		
		const unsigned char player_up[] =             {12,30,12,51,45,12,18,51};
		const unsigned char player_down[] =           {12,18,12,51,45,12,18,51};
		
		const unsigned char vertical_bar[] =             {12,12,12,12,12,12,12,12};
		const unsigned char horizontal_bar[] =           { 0, 0, 0,63,63,00,00,00};
		const unsigned char corner[] =                   {63,63,63,51,51,63,63,63};
		
		const unsigned char left_arrow[] =             { 0, 0,56,31,31,56, 0, 0};
		const unsigned char right_arrow[] =            { 0, 0, 7,62,62, 7, 0, 0};
		//const unsigned char dead_ghost[]  =           {18,30,33,51,33,45,51,30};
		
		const unsigned char bubble[] =            	   {12,30,30,30,63,45,33,33};
		//{ 30, 33, 33, 45, 45, 33, 33, 30};
		
		PLAYER_DOWN._color = 0;		
		GHOST_IMAGE._color = 0;
		MISSILE_IMAGE._color = 0;
		INVINCIBLE_GHOST_IMAGE._color = 0;
		POWERUP_IMAGE._color = 128u;
		GUN_IMAGE._color = 128u;
		BOMB_IMAGE._color = 128u;
		DEAD_GHOST_IMAGE._color = 128u;
			
		GHOST_IMAGE._imageData = (char) 0x60;
		INVINCIBLE_GHOST_IMAGE._imageData = (char) 0x7b;
		BOMB_IMAGE._imageData = (char) 0x5b;
		PLAYER_IMAGE._imageData = (char) 0x5c;
		

		POWERUP_IMAGE._imageData = (char) 0x5d;
		GUN_IMAGE._imageData = (char) 0x5e;
		MISSILE_IMAGE._imageData = (char) 0x5f;
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;


		PLAYER_IMAGE._color = 0;
		PLAYER_LEFT._color = 0;
		PLAYER_RIGHT._color = 0;
		PLAYER_UP._color = 0;
			

		PLAYER_LEFT._imageData = '*';
		PLAYER_RIGHT._imageData = ',';
		PLAYER_UP._imageData = '.';
		PLAYER_DOWN._imageData = '/';
		
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = 0;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = 0;
		
		
		BUBBLE_IMAGE._imageData = (char) 0x23;
		BUBBLE_IMAGE._color = 0;	
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + GHOST_IMAGE._imageData*8 + i, ghost[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + BOMB_IMAGE._imageData*8 + i, bomb[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + POWERUP_IMAGE._imageData*8 + i, powerUp[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + INVINCIBLE_GHOST_IMAGE._imageData*8 + i, invincible_ghost[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + GUN_IMAGE._imageData*8 + i, gun[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + MISSILE_IMAGE._imageData*8 + i, missile[i]);
		}

		for(i=0;i<8;++i)
		{
			POKE(0xb400 + PLAYER_IMAGE._imageData*8 + i, player[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + PLAYER_LEFT._imageData*8 + i, player_left[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + PLAYER_RIGHT._imageData*8 + i, player_right[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + PLAYER_UP._imageData*8 + i, player_up[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + PLAYER_DOWN._imageData*8 + i, player_down[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + '|'*8 + i, vertical_bar[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + '-'*8 + i, horizontal_bar[i]);
		}

		for(i=0;i<8;++i)
		{
			POKE(0xb400 + '+'*8 + i, corner[i]);
		}		
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + '<'*8 + i, right_arrow[i]);
		}

		for(i=0;i<8;++i)
		{
			POKE(0xb400 + '>'*8 + i, left_arrow[i]);
		}			
		
		for(i=0;i<8;++i)
		{
			POKE(0xb400 + BUBBLE_IMAGE._imageData*8 + i, bubble[i]);
		}	
		// for(i=0;i<8;++i)
		// {
			// POKE(0xb400 + DEAD_GHOST_IMAGE._imageData*8 + i, dead_ghost[i]);
		// }
	}
	
	void INIT_GRAPHICS(void)
	{
		unsigned char i;
		
		// Initialize colors 
		for(i=0;i<3;++i)
		{
			POKE(0xBB80+i*40,16);POKE(0xBB81+i*40,1); // red on black (inverted: cyan on white)
		}
		for(i=0+3;i<28;++i)
		{
			POKE(0xBB80+i*40,16);POKE(0xBB81+i*40,3); // yellow on black (inverted: blue on white)
		}		
	}
	 
	 
	
#endif // _ATMOS_REDEFINED_CHARACTERS