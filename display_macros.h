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

#if defined(__C64__)
	#include "c64/c64_hardware_sprite.h"
#endif



struct ImageStruct
{
	char _imageData;
	char _color;
};

typedef struct ImageStruct Image;

//
 
#if defined(__ATMOS__)
	#define GET_SCREEN_SIZE(x,y) {screensize(x,y); *x-=2;}
#elif defined(__ATARIXL__) || defined(__ATARI__)
	#define GET_SCREEN_SIZE(x,y) {screensize(x,y); *y=24;}
#else
	#define GET_SCREEN_SIZE(x,y) {screensize(x,y);}
#endif

#ifndef __ATMOS__
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
	
#else
	#define DRAW_BROKEN_WALL(x,y) {gotoxy((x+2),(y)); cputc('X');};

	#define DRAW_PLAYER(x,y,image) {(void) textcolor (image->_color); gotoxy((x+2),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

	#define DRAW_GHOST(x,y,image) {(void) textcolor (image->_color); gotoxy((x+2),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

	#define DRAW_INVINCIBLE_GHOST(x,y,image) {(void) textcolor (image->_color); gotoxy((x+2),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

	#define DRAW_BOMB(x,y,image) {(void) textcolor (image->_color); gotoxy((x+2),(y)); cputc(image->_imageData + 128); (void) textcolor (TEXT_COLOR);};

	#define DRAW_POWERUP(x,y,image) {(void) textcolor (image->_color); gotoxy((x+2),(y)); cputc(image->_imageData + 128); (void) textcolor (TEXT_COLOR);};

	#define DRAW_GUN(x,y,image) {(void) textcolor (image->_color); gotoxy((x+2),(y)); cputc(image->_imageData + 128); (void) textcolor (TEXT_COLOR);};

	#define DRAW_MISSILE(x,y,image) {(void) textcolor (image->_color); gotoxy((x+2),(y)); cputc(image->_imageData); (void) textcolor (TEXT_COLOR);};

	#define DRAW_BOMBS() \
	{ \
		unsigned char i = 0; \
		for(;i<BOMBS_NUMBER;++i) \
		{ \
			DRAW_BOMB(bombs[i]->_x, bombs[i]->_y, bombs[i]->_imagePtr); \
		} \
	}

	#define DELETE_PLAYER(x,y,image) {gotoxy(x+2,y);cputc(' ');};

	#define DELETE_GHOST(x,y,image) {gotoxy(x+2,y);cputc(' ');};

	#define DELETE_INVINCIBLE_GHOST(x,y,image) {gotoxy(x+2,y);cputc(' ');};

	#define DELETE_BOMB(x,y,image) {gotoxy(x+2,y);cputc(' ');};

	#define DELETE_POWERUP(x,y,image) {gotoxy(x+2,y);cputc(' ');};

	#define DELETE_GUN(x,y,image) {gotoxy(x+2,y);cputc(' ');};

	#define DELETE_MISSILE(x,y,image) {gotoxy(x+2,y);cputc(' ');};


	#define PRINT(x,y,str) {gotoxy(x+2,y); cputs(str); };

	#define PRINTF(x,y,...) {gotoxy(x+2,y); cprintf(##__VA_ARGS__); };




	#define DRAW_BORDERS() \
	{ \
		unsigned char i; \
		gotoxy(0+2,0); \
		cputc (CH_ULCORNER+128); \
		for(i=0;i<38-2;++i) cputc('-' + 128); \
		gotoxy(40-1,0); \
		cputc (CH_URCORNER+128); \
		for(i=0;i<28-2;++i) \
		{ \
			gotoxy(0+2,1+i); \
			cputc('|'+128); \
		} \
		gotoxy(0+2,28-1); \
		cputc (CH_LLCORNER+128); \
		for(i=0;i<38-2;++i) cputc('-' + 128); \
		gotoxy(40-1, 28-1); \
		cputc (CH_LRCORNER+128); \
		for(i=0;i<28-2;++i) \
		{ \
			gotoxy(38-1+2,1+i); \
			cputc('|'+128); \
		} \
	} 

	
	#define DRAW_VERTICAL_LINE(x,y,length) \
	{ \
		unsigned char i; \
		for(i=0;i<length;++i) \
		{ \
			gotoxy(x+2,y+i); \
			cputc('|'+128); \
		} \
	}
#endif

#define CLEAR_SCREEN() clrscr();


	
#define SET_TEXT_COLOR(c) (void) textcolor (c);

#define SET_BORDER_COLOR(c) (void) bordercolor (c);

#define SET_BACKGROUND_COLOR(c) (void) bgcolor (c);

void INIT_IMAGES(void);

void INIT_GRAPHICS(void);
	
#endif // _DISPLAY_MACROS