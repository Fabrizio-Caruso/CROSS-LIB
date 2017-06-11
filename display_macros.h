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

struct ImageStruct
{
	char _imageData;
	char _color;
};

typedef struct ImageStruct Image;

void initImages(void);


#define GET_SCREEN_SIZE(x,y) screensize(x,y);

#define DRAW(ghost) {SET_TEXT_COLOR(((ghost)->_imagePtr)->_color); gotoxy(((ghost)->_x),((ghost)->_y)); cputc(((ghost)->_imagePtr)->_imageData); SET_TEXT_COLOR(TEXT_COLOR);};

// TODO: Make this work!
//#define DRAW_LIVING_GHOST(ghost) {SET_TEXT_COLOR(((ghost)->_imagePtr)->_color); gotoxy(((ghost)->_x),((ghost)->_y)); cputc(((ghost)->_imagePtr)->_imageData); SET_TEXT_COLOR(TEXT_COLOR);};


// TODO: Maybe the drawing details should be in the Character 
//#define DRAW_LIVING_GHOST(ghost) {SET_TEXT_COLOR(COLOR_BLACK); gotoxy((int)((ghost)->_x),((int)(ghost)->_y)); cputc('O'); SET_TEXT_COLOR(TEXT_COLOR);};

// #define DRAW_DEAD_GHOST(ghost) {SET_TEXT_COLOR(COLOR_RED); gotoxy(((ghost)->_x),((ghost)->_y)); cputc('X'); SET_TEXT_COLOR(TEXT_COLOR);};

// #define DRAW_BOMB(ghost) {SET_TEXT_COLOR(COLOR_RED); gotoxy(((ghost)->_x),((ghost)->_y)); cputc('X'); SET_TEXT_COLOR(TEXT_COLOR);};

// #define DRAW_MISSILE(ghost) {SET_TEXT_COLOR(COLOR_BLACK); gotoxy(((ghost)->_x),((ghost)->_y)); cputc('.'); SET_TEXT_COLOR(TEXT_COLOR);};

// #if defined(__PLUS4__) || defined(__C16__)
	// #define DRAW_INVINCIBLE_GHOST(ghost) {SET_TEXT_COLOR(COLOR_BLACK); gotoxy(((ghost)->_x),((ghost)->_y)); cputc('+'); SET_TEXT_COLOR(TEXT_COLOR);};

	// #define DRAW_PLAYER(player)  {SET_TEXT_COLOR(COLOR_BLACK); gotoxy(((player)->_x),((player)->_y)); cputc('*'); SET_TEXT_COLOR(TEXT_COLOR);};

	// #define DRAW_POWERUP(ghost) {SET_TEXT_COLOR(COLOR_BLACK); gotoxy(((ghost)->_x),((ghost)->_y)); cputc('P'); SET_TEXT_COLOR(TEXT_COLOR);};

	// #define DRAW_GUN(ghost) {SET_TEXT_COLOR(COLOR_BLACK); gotoxy(((ghost)->_x),((ghost)->_y)); cputc('!'); SET_TEXT_COLOR(TEXT_COLOR);};
// #else
	// #define DRAW_INVINCIBLE_GHOST(ghost) {SET_TEXT_COLOR(COLOR_GREEN); gotoxy(((ghost)->_x),((ghost)->_y)); cputc('+'); SET_TEXT_COLOR(TEXT_COLOR);};

	// #define DRAW_PLAYER(player)  {SET_TEXT_COLOR(COLOR_BLUE); gotoxy(((player)->_x),((player)->_y)); cputc('*'); SET_TEXT_COLOR(TEXT_COLOR);};

	// #define DRAW_POWERUP(ghost) {SET_TEXT_COLOR(COLOR_CYAN); gotoxy(((ghost)->_x),((ghost)->_y)); cputc('P'); SET_TEXT_COLOR(TEXT_COLOR);};

	// #define DRAW_GUN(ghost) {SET_TEXT_COLOR(COLOR_CYAN); gotoxy(((ghost)->_x),((ghost)->_y)); cputc('!'); SET_TEXT_COLOR(TEXT_COLOR);};
// #endif
#define DELETE_CHARACTER(ghost) {gotoxy(((ghost)->_x),((ghost)->_y)); cputc(' ');};


// TODO: Implement color conversion
#if defined(__C64__)
	#include <peekpoke.h>
	#define DRAWC(x,y,ch,color) {POKE(1024+40*y+x,(ch & 127)); POKE(55296u+40*y+x, color);}
#else
	#define DRAWC(x,y,ch,color) {(void) textcolor (color); gotoxy((x),(y)); cputc(ch); (void) textcolor (TEXT_COLOR);};
#endif

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

	
#endif // _DISPLAY_MACROS