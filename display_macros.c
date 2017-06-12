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

void INIT_IMAGES(void)
{
#ifdef __C64__ 
	short i;
	
	short data[63];
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

#ifdef __C64__ 

	data[0] = 0;
	data[1] = 127;
	data[2] = 0;
	data[3] = 1;
	data[4] = 255;
	data[5] = 192;
	data[6] = 3;
	data[7] = 255;
	data[8] = 224;
	data[9] = 3;
	data[10] = 231;
	data[11] = 224;
	
	data[12] = 7;
	data[13] = 217;
	data[14] = 240;
	data[15] = 7;
	data[16] = 223;
	data[17] = 240;
	data[18] = 7;
	data[19] = 217;
	data[20] = 240;
	data[21] = 3;
	data[22] = 231;
	data[23] = 224;
	
	data[24] = 3;
	data[25] = 255;
	data[26] = 224;
	data[27] = 3;
	data[28] = 255;
	data[29] = 224;
	data[30] = 2;
	data[31] = 255;
	data[32] = 160;
	data[33] = 1;
	data[34] = 127;
	data[35] = 64;
	
	data[36] = 1;
	data[37] = 62;
	data[38] = 64;
	data[39] = 0;
	data[40] = 156;
	data[41] = 128;
	data[42] = 0;
	data[43] = 156;
	data[44] = 128;
	data[45] = 0;
	data[46] = 73;
	data[47] = 0;
	data[48] = 0;
	data[49] = 73;
	data[50] = 0;
	
	data[51] = 0;
	data[52] = 62;
	data[53] = 0;
	data[54] = 0;
	data[55] = 62;
	data[56] = 0;
	data[57] = 0;
	data[58] = 62;
	data[59] = 0;
	data[60] = 0;
	data[61] = 28;
	data[62] = 0;

	// Experimental
	POKE(53248u+21,4);
	POKE(2042,13);

	
	for(i=0;i<63;++i)
	{
		POKE(832+i,data[i]);
	}

	// POKE(53248u+4,100); 
	// POKE(53248u+5,100);
#endif
}


// void DRAW(Character *ghost)
// {
	// short x = ghost->_x;
	// short y = ghost->_y;
	
	// SET_TEXT_COLOR(((ghost)->_imagePtr)->_color); 
	// gotoxy(x,y); 
	// cputc(((ghost)->_imagePtr)->_imageData); 
	// SET_TEXT_COLOR(TEXT_COLOR);
	
	// POKE(53248u+4,x*5); POKE(53248u+5,y*5);
// }
