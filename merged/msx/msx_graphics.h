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

#include <stdio.h>
#include <msx/gfx.h>
extern unsigned char XSize;

#if defined(MSX_MODE1)
	#define BASE 6144
#else
	#define BASE 0
#endif
#if defined(VPOKE) 
	#define _DRAW(x,y,image) msx_vpoke(BASE+x+1+X_OFFSET+(y-1+Y_OFFSET)*(XSize+1),image->_imageData);
	#define _DELETE(x,y)     msx_vpoke(BASE+x+1+X_OFFSET+(y-1+Y_OFFSET)*(XSize+1),' ');
	#define _DRAW_WALL(x,y)  msx_vpoke(BASE+x+1+X_OFFSET+(y-1+Y_OFFSET)*(XSize+1),'|');
	#define _DRAW_BROKEN_WALL(x,y)  msx_vpoke(BASE+x+1+X_OFFSET+(y-1+Y_OFFSET)*(XSize+1),'X');	
#else
	#define _DRAW(x,y,image) {gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(image->_imageData);}
	#define _DELETE(x,y)     {gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' ');} 
	#define _DRAW_WALL(x,y)  {gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('|');}
	#define _DRAW_BROKEN_WALL(x,y)  {gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc('X');}
#endif

void INIT_GRAPHICS(void)
{
	unsigned char i;	set_color(15, 1, 1);
	#if defined(MSX_MODE1)
		set_mode(mode_1);

		msx_vpoke(8192+ 4,10*16); // White !, $ -- 32 - 39
		msx_vpoke(8192+ 5, 2*16); // Green  -- 40 - 47
		msx_vpoke(8192+ 6, 4*16); // Green -- 48 - 55
		msx_vpoke(8192+ 7, 4*16); // Green -- 56 - 63
		
		msx_vpoke(8192+ 8, 8*16); // Red 64 --
		msx_vpoke(8192+ 9, 8*16); // Red 72 --
		msx_vpoke(8192+10, 8*16); // Red 80 --
		msx_vpoke(8192+11, 8*16); // Red 88 --
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
			
	GHOST_IMAGE._imageData = 'o';
	INVINCIBLE_GHOST_IMAGE._imageData = '+';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	POWERUP_IMAGE._imageData = 'S';
	GUN_IMAGE._imageData = '!';
	MISSILE_IMAGE._imageData = '.';
	DEAD_GHOST_IMAGE._imageData = 'O';

	GHOST_IMAGE._color = COLOR_WHITE;
	MISSILE_IMAGE._color = COLOR_WHITE;

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
		
		BUBBLE_IMAGE._imageData = '^';
		BUBBLE_IMAGE._color = COLOR_WHITE;
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'V';
		
		EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
		EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
		INVINCIBILITY_IMAGE._color = COLOR_WHITE;	
	#endif
}

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_DRAW_BROKEN_WALL(x,y);
	}
#endif
	
void _draw(unsigned char x, unsigned char y, Image * image) 
{
	_DRAW(x,y,image);
}

void _delete(unsigned char x, unsigned char y)
{
	_DELETE(x,y);
}

void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	if(*blinkCounter) 
	{
		_DRAW(x,y,image);
		*blinkCounter=0;
	} 
	else 
	{
		_DELETE(x,y);
		*blinkCounter=1;
	}	
}


void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length)
{ 
	unsigned char i;
	for(i=0;i<length;++i)
	{
		_DRAW_WALL(x,y+i)
	}
}

void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length)
{
	unsigned char i;
	gotoxy(X_OFFSET+1+x,Y_OFFSET+y); 
	for(i=0;i<length;++i)
	{
		cputc('-');
	}
}
		


