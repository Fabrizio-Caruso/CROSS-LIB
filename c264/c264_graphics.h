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
	Image LEFT_ENEMY_MISSILE_IMAGE;
	Image RIGHT_ENEMY_MISSILE_IMAGE;
	Image GUN_IMAGE;

	Image LEFT_ENEMY_MISSILE_IMAGE;
	Image RIGHT_ENEMY_MISSILE_IMAGE;
	
	Image BUBBLE_IMAGE;

	Image EXTRA_POINTS_IMAGE;
	
	char powerUp_blink = 1;
	char gun_blink = 1;
	char extra_points_blink = 1;
	
	void INIT_GRAPHICS(void)
	{
		unsigned short i;
		static char player_down[8] =      { 24, 36, 24,102,153, 24, 36,102};
		static char player_up[8] =        { 24, 60, 24,102,153, 24, 36,102};
		static char player_right[8] =     { 24, 52, 25,118,152, 24, 20, 20};	
		static char player_left[8] =      { 24, 44,152,110, 25, 24, 40, 40};
		static char ghost[8] =            {129,126,165,129,129,189,129,126};
		static char missile_right[8] =    {  0,  0, 15,252,252, 15,  0,  0};
		static char missile_left[8] =     {  0,  0,240, 63, 63,240,  0,  0};
		static char invincible_ghost[8] = { 60, 66,165,129, 90, 36, 36, 60};
		static char gun[8] =              {  0,128,126,200,248,192,128,  0};
		static char powerUp[8] =          {  0, 60, 54,223,231,122, 36, 24};
		static char missile[8] =          {  0,  0,  8, 56, 28, 16,  0,  0};
		static char bomb[8] =             { 60, 66,165,153,153,165, 66, 60};
		static char bubble[8] =          {24,60,60,60,126,90,66,66};
		
		POKE(1177,62); // disable switch to RAM in PEEK
		for(i=0;i<1023;++i)
		{
			POKE(24576+4096+1024+i,PEEK((unsigned long) (54272ul+(unsigned long) i)));
		}
		POKE(1177,63); // re-enable switch to RAM in PEEK
		POKE(65299ul,(PEEK(65299ul)&3)|((29)*4)); // change character base address to 28th Kbyte
		POKE(65298ul,PEEK(65298ul)&251); // make graphics chip get characters from RAM
		
		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+i,player_down[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8+i,player_up[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8*2+i,player_right[i]);
		}
		
		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8*3+i,player_left[i]);
		}

		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8*4+i,missile_right[i]);
		}
		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8*5+i,missile_left[i]);
		}

		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8*6+i,invincible_ghost[i]);
		}

		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8*7+i,gun[i]);
		}	

		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296-8*2+i,powerUp[i]);
		}		
		
		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8*9+i,missile[i]);
		}	

		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8*10+i,bomb[i]);
		}	
		
		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296-8*3+i,ghost[i]);
		}	
		
		for(i=0;i<8;++i)
		{
			POKE(24576+4096+1024+296+8*22+i,bubble[i]);
		}			
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

		GHOST_IMAGE._imageData = '"';
		INVINCIBLE_GHOST_IMAGE._imageData = '+';
		BOMB_IMAGE._imageData = '/';
		PLAYER_IMAGE._imageData = '(';
		POWERUP_IMAGE._imageData = '#';
		GUN_IMAGE._imageData = ',';
		MISSILE_IMAGE._imageData = '.';
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_WHITE;
		
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = ')';
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;		
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '*';
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;

		
		PLAYER_DOWN._imageData = '%';
		PLAYER_DOWN._color = COLOR_CYAN;
		PLAYER_UP._imageData = '&';
		PLAYER_UP._color = COLOR_CYAN;
		PLAYER_RIGHT._imageData = '\'';
		PLAYER_RIGHT._color = COLOR_CYAN;
		PLAYER_LEFT._imageData = '(';
		PLAYER_LEFT._color = COLOR_CYAN;
		
		BUBBLE_IMAGE._imageData = ';';
		BUBBLE_IMAGE._color = COLOR_WHITE;
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
	}

	void _draw_broken_wall(char x, char y)
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (COLOR_WHITE);
		cputc('X');
	}
	
	void _draw(char x, char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (image->_color);
		cputc(image->_imageData); 
	};
	
	
	void _blink_powerUp_draw(char x, char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (image->_color);
		if(powerUp_blink) 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40,image->_imageData + image->_color );
			cputc(image->_imageData); 
			powerUp_blink=0;
		} 
		else 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32); 
			cputc(' '); 
			powerUp_blink=1;
		}	
	};
	
	void _blink_gun_draw(char x, char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (image->_color);
		if(gun_blink) 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40,image->_imageData + image->_color );
			cputc(image->_imageData); 
			gun_blink=0;
		} 
		else 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32); 
			cputc(' '); 
			gun_blink=1;
		}	
	};

	void _blink_extra_points_draw(char x, char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (image->_color);
		if(extra_points_blink) 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40,image->_imageData + image->_color );
			cputc(image->_imageData);
			//cputc('$');			
			extra_points_blink=0;
		} 
		else 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32); 
			cputc(' '); 
			extra_points_blink=1;
		}	
	};
		
	void _delete(char x, char y)
	{
		gotoxy(x+X_OFFSET,y+Y_OFFSET);cputc(' ');
	};
#endif // _C264_GRAPHICS