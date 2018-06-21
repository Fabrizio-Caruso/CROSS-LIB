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

#if defined(__CREATIVISION__) || defined(__GAMATE__) \
	|| defined(__OSIC1P__) || defined(__ATARI5200__) || defined(__PET__) \
	|| defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) \
	|| defined(__APPLE2ENH__) || defined(__ATMOS__)
	#include "../cc65/generic/generic_conio_patch.h"
#endif
#  if defined(CONIO_ADM3A)
	#include "../generic/patch/adm3a_conio_implementation.h"
#elif defined(CONIO_VT52)
	#include "../generic/patch/vt52_conio_implementation.h"
#elif defined(CONIO_VT100)
	#include "../generic/patch/vt100_conio_implementation.h"
#elif defined(Z88DK_PUTC4X6)
	#include "../z88dk/generic/z88dk_putc4x6_conio_implementation.h"
#elif defined(__ATMOS__)
	#include "../cc65/atmos/atmos_conio_patch.h"
#elif defined(__SUPERVISION__) 
	#include "../cc65/supervision/supervision_conio_implementation.h"	
#elif defined(__ATARI_LYNX__) 
	#include "../cc65/atari_lynx/atari_lynx_conio_implementation.h"	
#elif defined(__WINCMOC__)
	#include "../wincmoc/wincmoc_conio_patch.h"	
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#include "../cmoc/cmoc_conio_implementation.h"
#elif defined(__SMS__)
	#include "../z88dk/sms/sms_conio_implementation.h"
#elif defined(__ENTERPRISE__)
	#include "../z88dk/enterprise/enterprise_conio_implementation.h"
#elif defined(__NCURSES__)
	#include "../ncurses/ncurses_conio_implementation.h"	
#elif defined(__KC__)
	#include "../z88dk/kc/kc_conio_implementation.h"	
#elif defined(Z88DK_SPRITES)
	#include "../z88dk/generic/z88dk_conio_patch.h"	
#elif (defined(__SPECTRUM__) && !defined(CLIB_ANSI))
	#include "../z88dk/spectrum/spectrum_conio_implementation.h"
#elif defined(__C128_Z80__) || defined(__X1__) || defined(__Z9001__) \
	|| defined(__Z1013__) || defined(__OSCA__) || defined(__MC1000__) \
	|| defined(__ABC80__) || defined(__PC6001__) || defined(__SRR__) \
	|| defined(__NASCOM__) || defined(__P2000__) || defined(__BEE__) \
	|| defined(__TI8X__) || defined(__TI82__) || defined(__TI83__) \
	|| defined(__TI82__) || defined(__TI83__) || defined(__TI8X__) \
	|| defined(__TI85__) || defined(__TI86__) || defined(__TI86S__) || defined(__MZ__) \
	|| defined(__GAL__) || defined(__SC3000__) || (defined(__SPECTRUM__) && defined(CLIB_ANSI)) \
	|| defined(__SAM__) || defined(__CPC__) || (defined(__SVI__) && !defined(MSX_MODE0)) \
	|| defined(__VG5K__) || defined(__AQUARIUS__) || defined(__VZ__) \
	|| defined(__MTX__) || defined(__Z88__) || defined(__PX4__) \
	|| defined(__EG2K__) || defined(__TRS80__) || defined(__ACE__) \
	|| defined(__EINSTEIN__) || defined(__M5__)
	#include "../z88dk/generic/z88dk_conio_patch.h"					
#elif defined(__MSX__) || (defined(__SVI__) && defined(MSX_MODE0)) \
	  || defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__)
	#include "../z88dk/generic/z88dk_conio_implementation.h"
#endif

struct ImageStruct
{
	unsigned char _imageData;
	#if !defined(NO_COLOR)
		unsigned char _color;
	#endif
};

typedef struct ImageStruct Image;


void _draw(unsigned char x,unsigned char y,Image * image);

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y);
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
#if defined(NO_PRINT)
	#define PRINT(x,y,str)
	#define PRINTF(x,y,str,val)
#else
	#  if defined(NO_PRINT)
		#define PRINT(x,y,str)
		#define PRINTF(x,y,str,val)
	#elif defined(__NCURSES__)
		#include <ncurses.h>
		
		#define PRINT(x,y,str) do {move(y,x); printw(str); refresh();} while(0)
		#define PRINTF(x,y,str, val) do {move(y,x); printw(str,val); refresh();} while(0)
	#elif defined(ALT_PRINT) \
		|| (defined(__CMOC__) && !defined(__WINCMOC__)) \
		|| defined(Z88DK_PUTC4X6)
		void PRINT(unsigned char x, unsigned char y, char * str);
		void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);	
	#elif defined(ATARI_MODE1) && (defined(__ATARI__) || defined(__ATARIXL__))
		void PRINT(unsigned char x, unsigned char y, char * str);
		
		#define PRINTF(x,y,...)  \
		{ \
			if((y+Y_OFFSET)&1) \
			{ \
				gotoxy(x+20+X_OFFSET,(y+Y_OFFSET)/2); \
			} \
			else \
			{ \
				gotoxy(x+X_OFFSET, (y+Y_OFFSET)/2); \
			} \
			cprintf(##__VA_ARGS__); \
		};
	#else
		#define PRINT(x,y,str) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); cprintf(str); } while(0);
		#define PRINTF(x,y,str,val) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); cprintf(str,val); } while(0);
	#endif
#endif


// DRAW HORIZONTAL AND VERTICAL LINES
#if !defined(TINY_GAME)
	#if defined(__VG5K__)
		void _draw_ch(unsigned char x, unsigned char y, unsigned char ch, unsigned char col);
	#endif
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
#  if defined(ALT_PRINT) || defined(NO_COLOR) \
	|| defined(__ATARI5200__) \
	|| (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1) \
	|| defined(__CMOC__) \
	|| (defined(__SVI__) && defined(MSX_MODE0)) || defined(__MSX__) \
	|| defined(__ATMOS__) \
	|| (defined(__VIC20__) && defined(VIC20_UNEXPANDED)) \
	|| defined(__ATARI_LYNX__) || (defined(__AQUARIUS__) && defined(EXT_GRAPHICS)) \
	|| defined(Z88DK_SPRITES)
	#define SET_TEXT_COLOR(c) 
#elif defined(__SPECTRUM__) && !defined(CLIB_ANSI)
	#include <stdio.h> 
	#include <arch/zx.h>
	#define SET_TEXT_COLOR(c) printf("\020%c",c)
#elif defined(__NCURSES__)
	#include <ncurses.h>
	#define SET_TEXT_COLOR(c) attron(COLOR_PAIR(c)) 
#else
	#define SET_TEXT_COLOR(c) textcolor(c);
#endif


// CLEAR SCREEN
#if defined(__SPECTRUM__) && !defined(CLIB_ANSI)
	#include <stdio.h>
	#include <arch/zx.h>
	#define CLEAR_SCREEN()  {zx_cls(PAPER_BLACK|INK_WHITE);}
#elif defined(__CPC__) 
	#define CLEAR_SCREEN() printf("\x1B[37;40m\x1B[2J")
#elif defined(__ATARI_LYNX__) || (defined(__AQUARIUS__) && defined(EXT_GRAPHICS)) \
		|| defined(__TRS80__) || defined(__EG2K__) \
		|| defined(__CMOC__) && !defined(__WINCMOC__) \
		|| defined(__VG5K__)  \
		|| defined(__SUPERVISION__)	 
	void CLEAR_SCREEN(void);	
#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)
	#define CLEAR_SCREEN() { \
	clrscr(); \
	_setcolor_low(0, TGI_COLOR_RED); \
	_setcolor_low(1, TGI_COLOR_WHITE); \
	_setcolor_low(2, TGI_COLOR_CYAN); 	\
	_setcolor_low(3, TGI_COLOR_BROWN); \
	_setcolor_low(4, TGI_COLOR_BLACK);	};
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
#if defined(CC65) && !defined(__ATARI5200__)
	#define SET_BORDER_COLOR(c) (void) bordercolor(c)
	#define SET_BACKGROUND_COLOR(c) (void) bgcolor (c);
#else
	#define SET_BORDER_COLOR(c) {}
	#define SET_BACKGROUND_COLOR(c) {}	
#endif	

	
void INIT_IMAGES(void);

void INIT_GRAPHICS(void);
	
#endif // _DISPLAY_MACROS

