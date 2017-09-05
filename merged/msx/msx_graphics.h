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
 
//#include <peekpoke.h>
#include "display_macros.h"


Image PLAYER_IMAGE;
Image GHOST_IMAGE;
Image DEAD_GHOST_IMAGE;
Image INVINCIBLE_GHOST_IMAGE;
Image BOMB_IMAGE;
Image POWERUP_IMAGE;
Image MISSILE_IMAGE;
Image GUN_IMAGE;

Image BUBBLE_IMAGE;

Image LEFT_ENEMY_MISSILE_IMAGE;
Image RIGHT_ENEMY_MISSILE_IMAGE;

Image EXTRA_POINTS_IMAGE;
Image EXTRA_LIFE_IMAGE;
Image INVINCIBILITY_IMAGE;

#include <stdio.h>
#include <msx/gfx.h>
extern unsigned char XSize;

#if defined(MSX_MODE1)
	#define BASE 6144
#else
	#define BASE 0
#endif
#if defined(VPOKE) 
	#define _DRAW   msx_vpoke(BASE+x+2+X_OFFSET+(y-1+Y_OFFSET)*(XSize+3),image->_imageData);
	#define _DELETE msx_vpoke(BASE+x+2+X_OFFSET+(y-1+Y_OFFSET)*(XSize+3),' ');
#else
	#define _DRAW 	cputc(image->_imageData);
	#define _DELETE cputc(' '); 

#endif

void INIT_GRAPHICS(void)
{
	unsigned char i;	set_color(15, 1, 1);
	#if defined(MSX_MODE1)
		set_mode(mode_1);
		for(i=0;i<8;++i)
		{
			msx_vpoke(8192+4+i,16*(i+1));
		}
	#endif
}

void INIT_IMAGES(void)
{		

	PLAYER_IMAGE._color = COLOR_WHITE;
	INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
	POWERUP_IMAGE._color = COLOR_WHITE;
	GUN_IMAGE._color = COLOR_WHITE;
	BOMB_IMAGE._color = COLOR_RED;
	DEAD_GHOST_IMAGE._color = COLOR_RED;
	EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
	EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
	INVINCIBILITY_IMAGE._color = COLOR_WHITE;			
		
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
	
	BUBBLE_IMAGE._imageData = '^';
	BUBBLE_IMAGE._color = COLOR_WHITE;
	
	EXTRA_POINTS_IMAGE._imageData = '$';
	
	EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
	INVINCIBILITY_IMAGE._imageData = 'V';
}


void _draw_broken_wall(unsigned char x, unsigned char y)
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(COLOR_WHITE);
	cputc('X');
}

void _draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+1+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(image->_color);
	_DRAW
}

void _delete(unsigned char x, unsigned char y)
{
	gotoxy(x+1+X_OFFSET,y+Y_OFFSET);
	_DELETE
}

void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	gotoxy((x+1+X_OFFSET),(y+Y_OFFSET)); 
	if(*blinkCounter) 
	{
		_DRAW
		*blinkCounter=0;
	} 
	else 
	{
		_DELETE
		*blinkCounter=1;
	}	
}


void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length)
{ 
	unsigned char i;
	for(i=0;i<length;++i)
	{
		gotoxy(x+1+X_OFFSET,y+i+Y_OFFSET);
		cputc('|');
	}
}




