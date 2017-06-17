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
 
#ifndef _DISPLAY_MACROS
#define _DISPLAY_MACROS

#include <conio.h>
#include "settings.h"

#include "patch/generic_conio_patch.h"
#if defined(__ATMOS__)
	#include "atmos/atmos_conio_patch.h"
#endif



struct ImageStruct
{
	char _imageData;
	char _color;
};

typedef struct ImageStruct Image;

//
 

#define GET_SCREEN_SIZE(x,y) {screensize(x,y);}


#define DRAW_BROKEN_WALL(x,y) {gotoxy((x),(y)); cputc('X');};

#define DRAW_PLAYER(x,y,image) {(void) textcolor (image->_color); gotoxy((x),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

#define DRAW_GHOST(x,y,image) {(void) textcolor (image->_color); gotoxy((x),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

#define DRAW_INVINCIBLE_GHOST(x,y,image) {(void) textcolor (image->_color); gotoxy((x),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

#define DRAW_BOMB(x,y,image) {(void) textcolor (image->_color); gotoxy((x),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

#define DRAW_POWERUP(x,y,image) {(void) textcolor (image->_color); gotoxy((x),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

#define DRAW_GUN(x,y,image) {(void) textcolor (image->_color); gotoxy((x),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

#define DRAW_MISSILE(x,y,image) {(void) textcolor (image->_color); gotoxy((x),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

#define DRAW_BOMBS() \
{ \
	unsigned char i = 0; \
	for(;i<BOMBS_NUMBER;++i) \
	{ \
		DRAW_BOMB(bombs[i]->_x, bombs[i]->_y, bombs[i]->_imagePtr); \
	} \
}

#define DELETE_PLAYER(x,y,image) {gotoxy(x,y);cputc(' ');};

#define DELETE_GHOST(x,y,image) {gotoxy(x,y);cputc(' ');};

#define DELETE_INVINCIBLE_GHOST(x,y,image) {gotoxy(x,y);cputc(' ');};

#define DELETE_BOMB(x,y,image) {gotoxy(x,y);cputc(' ');};

#define DELETE_POWERUP(x,y,image) {gotoxy(x,y);cputc(' ');};

#define DELETE_GUN(x,y,image) {gotoxy(x,y);cputc(' ');};

#define DELETE_MISSILE(x,y,image) {gotoxy(x,y);cputc(' ');};


#define PRINT(x,y,str) {gotoxy(x,y); cputs(str); };

#define PRINTF(x,y,...) {gotoxy(x,y); cprintf(##__VA_ARGS__); };

#define CLEAR_SCREEN() clrscr();

#define DRAW_BORDERS()\
{ \
	cputc (CH_ULCORNER);\
    chline (XSize - 2);\
    cputc (CH_URCORNER);\
    cvlinexy (0, 1, YSize - 2);\
    cputc (CH_LLCORNER);\
    chline (XSize - 2);\
    cputc (CH_LRCORNER);\
    cvlinexy (XSize - 1, 1, YSize - 2); \
}

#define DRAW_VERTICAL_LINE(x,y,length) cvlinexy (x,y,length);
	
#define SET_TEXT_COLOR(c) (void) textcolor (c);

#define SET_BORDER_COLOR(c) (void) bordercolor (c);

#define SET_BACKGROUND_COLOR(c) (void) bgcolor (c);

void INIT_IMAGES(void);

	
#endif // _DISPLAY_MACROS