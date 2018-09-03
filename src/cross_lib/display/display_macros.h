/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 
 
#ifndef _DISPLAY_MACROS
#define _DISPLAY_MACROS

#include "../cross_lib.h"
#include "display_target_settings.h"


#if defined(CONIO_LIB)
	#include<conio.h>
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#include <coco.h>
#endif

#include "conio_patch.h"

struct ImageStruct
{
	unsigned char _imageData;
	#if !defined(NO_COLOR)
		unsigned char _color;
	#endif
};

typedef struct ImageStruct Image;


#if !defined(NO_STATS) \
	|| X_OFFSET!=0 || Y_OFFSET!=0

	void _draw_stat(unsigned char x, unsigned char y, Image * image);

	void _draw(unsigned char x, unsigned char y, Image * image);
#else
	void _draw(unsigned char x, unsigned char y, Image * image);	
#endif

#if !defined(BUFFERED)
	#define REFRESH()
#else
	#include "graphics_mode/buffered_graphics.h"
	#define REFRESH() \
		do \
		{ \
			printf("\n"); \
			display_all(); \
		} while(0);
#endif

#if defined(FULL_GAME)
	void DRAW_BROKEN_BRICK(unsigned char x, unsigned char y);
#endif
	
#if !defined(NO_BLINKING)
	void _blink_draw(unsigned char x,unsigned char y,Image * image, unsigned char * blinkCounter);
#else
	#define _blink_draw(x,y,image,blinkCounter) _draw(x,y,image)
#endif

void _delete(unsigned char x, unsigned char y);

// VERTICAL AND HORIZONTAL BORDER
#if !defined(TINY_GAME)
	#define DRAW_VERTICAL_BORDER(x) DRAW_VERTICAL_LINE(x,0,YSize-1)
	#define DRAW_HORIZONTAL_BORDER(y) DRAW_HORIZONTAL_LINE(0,y,XSize-1)
#else	
	#define DRAW_VERTICAL_BORDER(x)	
	#define DRAW_HORIZONTAL_BORDER(y)
#endif

#define WALL_COLOR COLOR_YELLOW

#define DRAW_BORDERS() \
	SET_TEXT_COLOR(WALL_COLOR); \
	DRAW_HORIZONTAL_BORDER(0); \
	DRAW_HORIZONTAL_BORDER(YSize-1); \
	DRAW_VERTICAL_BORDER(0); \
	DRAW_VERTICAL_BORDER(XSize-1); 


#define DRAW_HORIZONTAL_WALLS(length) \
	SET_TEXT_COLOR(WALL_COLOR); \
	DRAW_HORIZONTAL_LINE(1,YSize/2,length); \
	DRAW_HORIZONTAL_LINE(XSize-1-length,YSize/2,length);


// PRINT AND PRINTF
#  if defined(ALT_DISPLAY_STATS)
	void PRINT(unsigned char x, unsigned char y, char * str);
	#define PRINTF(x,y,str,val) _displayShort(val)
	void _displayShort(unsigned short val);
	unsigned short loc(unsigned char x, char y);
#elif defined(NO_PRINT)
	#define PRINT(x,y,str)
	#define PRINTF(x,y,str,val)
#elif defined(ALT_PRINT)
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val);
#else
	#define PRINT(x,y,str) do {gotoxy(x+X_OFFSET,y); cprintf(str); } while(0);
	#define PRINTF(x,y,str,val) do {gotoxy(x+X_OFFSET,y); cprintf(str,val); } while(0);
#endif


// DRAW HORIZONTAL AND VERTICAL LINES
#if !defined(TINY_GAME)
	void DRAW_HORIZONTAL_LINE(unsigned char x, unsigned char y, unsigned char length);
	void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length);	
#else
	#define DRAW_HORIZONTAL_LINE(x,y,length)
	#define DRAW_VERTICAL_LINE(x,y,length)
#endif

#if !defined(NO_SET_SCREEN_COLORS)
	void setScreenColors(void);
#endif
	
// COLORS 
#  if defined(NO_COLOR) \
	|| defined(__ATARI5200__) \
	|| (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1) \
	|| defined(__CMOC__) \
	|| (defined(__SVI__) && defined(MSX_MODE0)) || defined(__MSX__) \
	|| defined(__ATMOS__) \
	|| defined(Z88DK_SPRITES)
	#define SET_TEXT_COLOR(c) 
#elif defined(__NCURSES__)
	#include <ncurses.h>
	#define SET_TEXT_COLOR(c) attron(COLOR_PAIR(c))
#else
	#define SET_TEXT_COLOR(c) textcolor(c)
#endif


// CLEAR SCREEN
#  if defined(ALT_CLEAR_SCREEN)
	void CLEAR_SCREEN(void);
#elif defined(__CPC__) 
	#define CLEAR_SCREEN() printf("\x1B[37;40m\x1B[2J")
#elif defined(__ATMOS__)
	#define CLEAR_SCREEN() do {clrscr(); INIT_GRAPHICS(); } while(0)
#elif defined(Z88DK_SPRITES)
	#include <games.h>
	#include <graphics.h>

	#define CLEAR_SCREEN() clg()
#elif defined(__NCURSES__)
	#include <ncurses.h>
	
	#define CLEAR_SCREEN() clear();
#else // CONIO case
	#define CLEAR_SCREEN() clrscr();
#endif


// BORDER AND BACKGROUND COLORS
#if defined(CC65) && !defined(__ATARI5200__) && !defined(__SUPERVISION__)
	#define SET_BORDER_COLOR(c) (void) bordercolor(c)
	#define SET_BACKGROUND_COLOR(c) (void) bgcolor (c);
#else
	#define SET_BORDER_COLOR(c) {}
	#define SET_BACKGROUND_COLOR(c) {}	
#endif	

#if defined(__CPC__)
	#define COLOR_IF_NO_BLUE_THEN_YELLOW COLOR_YELLOW
#else
	#define COLOR_IF_NO_BLUE_THEN_YELLOW COLOR_BLUE
#endif

#if !defined(TINY_GAME)
	extern unsigned char guns;
	extern Image GUN_IMAGE;
	extern Image SKULL_IMAGE;
#endif
extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;

void INIT_IMAGES(void);

#if !defined(NO_INIT_GRAPHICS)
	void INIT_GRAPHICS(void);
#else
	#define INIT_GRAPHICS() 
#endif
	
#endif // _DISPLAY_MACROS

