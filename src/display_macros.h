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

#include "settings.h"

#if defined(__ATARI5200__)
	#include <atari5200.h>
	// TODO: This is a hack to get Atari5200 to compile in FULL GAME mode
	#if defined(FULL_GAME) && defined(NO_COLOR)
		unsigned char __fastcall__ textcolor (unsigned char color) {return 0;};	
	#endif
#endif
	

#if defined(REDEFINED_CHARS)
	#define PLAYER_IMAGE PLAYER_DOWN
#endif
	
#if !defined(__CMOC__) && !defined(__SUPERVISION__) && !defined(__ATARI_LYNX__) && !defined(__MSX__)  && !(defined(__SVI__) && defined(MSX_MODE0)) && !(defined(__SPECTRUM__) && !defined(CLIB_ANSI)) 
	#include <conio.h>
#endif
#if defined(__WINCMOC__)
	#include <conio.h>
#endif

#if defined(__CREATIVISION__) || defined(__GAMATE__) || defined(__OSIC1P__) || defined(__ATARI5200__) || defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__APPLE2ENH__) || defined(__ATMOS__)
	#include "patch/generic_conio_patch.h"
#endif
#if defined(__ATMOS__)
	#include "atmos/atmos_conio_patch.h"
#elif defined(__SUPERVISION__) 
	#include "patch/supervision_conio_implementation.h"	
#elif defined(__ATARI_LYNX__) 
	#include "patch/atari_lynx_conio_implementation.h"	
#elif defined(__WINCMOC__)
	#include "patch/wincmoc_conio_patch.h"	
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#include "patch/cmoc_conio_implementation.h"
#elif defined(__TI8X__) || defined(__TI82__) || defined(__TI83__) || defined(__TI85__) || defined(__TI86__) || defined(__TI86S__)
	#include "patch/z88dk_conio_patch.h"
#elif defined(__BEE__)
	#include "patch/z88dk_conio_patch.h"	
#elif defined(__X1__) || defined(__Z9001__) || defined(__Z1013__) || defined(__OSCA__) || defined(__MC1000__) || defined(__ABC80__) || defined(__PC6001__) || defined(__SRR__) || defined(__NASCOM__) || defined(__P2000__)
	#include "patch/z88dk_conio_patch.h"		
#elif defined(__MZ__)
	#include "patch/z88dk_conio_patch.h"		
#elif defined(__GAL__)
	#include "patch/z88dk_conio_patch.h"	
#elif defined(__SPECTRUM__) && defined(CLIB_ANSI)
	#include "patch/z88dk_conio_patch.h"	
#elif defined(__SPECTRUM__) && !defined(CLIB_ANSI)
	#include "patch/z88dk_conio_implementation.h"		
#elif defined(__SAM__)
	#include "patch/z88dk_conio_patch.h"	
#elif defined(__CPC__)
	#include "patch/z88dk_conio_patch.h"
#elif defined(__MSX__)
	#include "patch/z88dk_conio_implementation.h"
#elif defined(__SVI__) && !defined(MSX_MODE0)
	#include "patch/z88dk_conio_patch.h"
#elif defined(__SVI__) && defined(MSX_MODE0)
	#include "patch/z88dk_conio_implementation.h"
#elif defined(__VG5K__)
	#include "patch/z88dk_conio_patch.h"
#elif defined(__TRS80__)
	#include "patch/z88dk_conio_implementation.h"
#elif defined(__SC3000__)
	#include "patch/z88dk_conio_implementation.h"	
#elif defined(__AQUARIUS__)
	#include "patch/z88dk_conio_patch.h"	
#elif defined(__VZ__)
	#include "patch/z88dk_conio_patch.h"			
#elif defined(__ZX81__) || defined(__LAMBDA__)
	#include "patch/z88dk_conio_implementation.h"	
#elif defined(__ZX80__)
	#include "patch/z88dk_conio_implementation.h"		
#elif defined(__ACE__)
	#include "patch/z88dk_conio_implementation.h"		
#elif defined(__ENTERPRISE__)
	#include "patch/z88dk_conio_implementation.h"		
#elif defined(__MTX__)
	#include "patch/z88dk_conio_patch.h"		
#elif defined(__M5__)
	#include "patch/z88dk_conio_implementation.h"		
#endif

struct ImageStruct
{
	unsigned char _imageData;
	#if !defined(NO_COLOR)
		unsigned char _color;
	#endif
};

typedef struct ImageStruct Image;

//
#if defined(NARROW) || defined(TINY_GAME)
	#define Y_OFFSET 0 
#else
	#define Y_OFFSET 2		
#endif

#if defined(__ATMOS__)
	#define X_OFFSET 2
#elif defined(__VG5K__)
	#define X_OFFSET 1
#else
	#define X_OFFSET 0
#endif

#if defined(__CREATIVISION__) || defined(__MSX__) || defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__) || defined(__SPECTRUM__)
	#define XSize 32
#elif defined(__CBM610__) || defined(__PET__) || (defined(__C128__) && defined(C128_80COL_VIDEO_MODE))
	#define XSize 80
#elif defined(__OSIC1P__)
	#define XSize 32
#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)
	#define XSize 20
#elif (defined(__ATARI__) || defined(__ATARIXL__)) && !defined(ATARI_MODE1)
	#define XSize 20
#elif defined(__VIC20__)
	#define XSize 22
#elif defined(__CPC__) || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) \
      || (defined(__C128__) && !defined(C128_80COL_VIDEO_MODE))
	#define XSize (40-X_OFFSET)
#elif defined(__PET__) || defined(__CBM610__)
	#define XSize 80
#elif defined(__ATMOS__)
	#define XSize (40-X_OFFSET)	
#elif defined(__CMOC__) || defined(__WINCMOC__)
	#define XSize 32
#else
	#define XSize 16
#endif

#if defined(__CREATIVISION__) || defined(__MSX__) || defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__) || defined(__SPECTRUM__)
	#define YSize (24-Y_OFFSET)
#elif defined(__OSIC1P__)
	#define YSize (32-Y_OFFSET)	
#elif (defined(__ATARI__) || defined(__ATARIXL__)) 
	#define YSize (24-Y_OFFSET)	
#elif defined(__CPC__)
	#define YSize (25-Y_OFFSET)
#elif defined(__VIC20__)
	#define YSize 23
#elif defined(__CBM610__) || defined(__PET__) || defined(__C128__) || \
      defined(__C16__) || defined(__PLUS4__) || defined(__C64__) || \
	  defined(__PET__) || defined(__CBM610__) 
	#define YSize (25-Y_OFFSET)
#elif defined(__ATMOS__)
	#define YSize (28-Y_OFFSET)		
#elif defined(__CMOC__) || defined(__WINCMOC__)
	#define YSize 16	
#else
	#define YSize 16
#endif

#if XSize<YSize
	#define MIN_SIZE XSize
#else
	#define MIN_SIZE YSize
#endif

// #if defined(__ATARI5200__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1))
	// #define GET_SCREEN_SIZE(x,y) do {*x=20; *y=24;} while(0)
// #elif defined(__ATARI_LYNX__)
	// #define GET_SCREEN_SIZE(x,y) do {*x=20-X_OFFSET; *y=13-Y_OFFSET;} while(0)
// #elif defined(__WINCMOC__) || defined(__CMOC__)
	// #define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=16-Y_OFFSET;} while(0)
// #elif defined(__C128__) && defined(C128_80COL_VIDEO_MODE)
	// #define GET_SCREEN_SIZE(x,y) do {*x=80-X_OFFSET; *y=25-Y_OFFSET;} while(0)
// #elif defined (__SPECTRUM__)
	// #define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-Y_OFFSET;} while(0)		
// #elif defined(__MSX__) && !defined(MSX_MODE1)
	// #define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=24-Y_OFFSET;} while(0)
// #elif defined(__MSX__) && defined(MSX_MODE1)
	// #define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-Y_OFFSET;} while(0)
// #elif defined(__SVI__) && defined(MSX_MODE0)
	// #define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=24-Y_OFFSET;} while(0)		
// #elif defined(__CPC__) && defined(CPCRSLIB)
	// #define GET_SCREEN_SIZE(x,y) do {*x=(40-X_OFFSET); *y=(25-Y_OFFSET);} while(0)	
// #elif defined(__ZX81__) || defined(__LAMBDA__)
	// #define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-Y_OFFSET;} while(0)
// #elif defined(__ZX80__) 
	// #define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-1-Y_OFFSET;} while(0)				
// #elif defined(__TRS80__) 
	// #define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=25-Y_OFFSET;} while(0)				
// #else
	// #define GET_SCREEN_SIZE(x,y) do {screensize(x,y); *x-=X_OFFSET; *y-=Y_OFFSET;} while(0)
// #endif

#if defined(REDEFINED_CHARS)
	extern Image PLAYER_LEFT;
	extern Image PLAYER_RIGHT;
	extern Image PLAYER_UP;
	extern Image PLAYER_DOWN;

	#define SHOW_LEFT() do {player._imagePtr = (Image *)&PLAYER_LEFT; } while(0)
	#define SHOW_RIGHT() do {player._imagePtr = (Image *)&PLAYER_RIGHT; } while(0)
	#define SHOW_UP() do {player._imagePtr = (Image *) &PLAYER_UP; } while(0)
	#define SHOW_DOWN() do {player._imagePtr = (Image *)&PLAYER_DOWN; } while(0)
#else
	#define SHOW_LEFT() { }
	#define SHOW_RIGHT() { }
	#define SHOW_UP() { }
	#define SHOW_DOWN() { }		
#endif



void _draw(unsigned char x,unsigned char y,Image * image);
#define DRAW_CHARACTER(x,y,image) _draw(x,y,image)

#define DRAW_PLAYER(x,y,image) DRAW_CHARACTER(x,y,image)
#define DRAW_GHOST(x,y,image) DRAW_CHARACTER(x,y,image)
#define DRAW_INVINCIBLE_GHOST(x,y,image) DRAW_CHARACTER(x,y,image)
#define DRAW_BOMB(x,y,image) DRAW_CHARACTER(x,y,image)
#define DRAW_MISSILE(x,y,image) DRAW_CHARACTER(x,y,image)



#if defined(FULL_GAME)
	#define _DRAW_PLAYER() \
		if(invincibilityActive) \
		{ \
			DRAW_BLINKING_PLAYER(player._x, player._y, player._imagePtr); \
		} \
		else \
		{ \
			DRAW_PLAYER(player._x, player._y, player._imagePtr); \
		}
#else
	#define _DRAW_PLAYER() \
		DRAW_PLAYER(player._x, player._y, player._imagePtr); 
#endif
		
#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y);
#endif
	
void _blink_draw(unsigned char x,unsigned char y,Image * image, unsigned char * blinkCounter);
#define DRAW_BLINKING_PLAYER(x, y, image) _blink_draw(x,y,image, &playerBlink)

void _delete(unsigned char x, unsigned char y);

#define DELETE_CHARACTER(x,y) _delete(x,y)

#if defined(__GAMATE__)
	#define DELETE_PLAYER(x,y,image) do {textcolor(COLOR_CYAN); _delete(x,y);} while(0)
#else
	#define DELETE_PLAYER(x,y,image) _delete(x,y)
#endif
#define DELETE_GHOST(x,y,image) _delete(x,y)
#if defined(__GAMATE__)
	#define DELETE_INVINCIBLE_GHOST(x,y,image) do {textcolor(COLOR_YELLOW); _delete(x,y);} while(0)
#else
	#define DELETE_INVINCIBLE_GHOST(x,y,image) _delete(x,y)	
#endif
#define DELETE_BOMB(x,y,image) _delete(x,y)
#define DELETE_POWERUP(x,y,image) _delete(x,y)
#define DELETE_GUN(x,y,image) _delete(x,y)
#define DELETE_MISSILE(x,y,image) _delete(x,y)
#define DELETE_EXTRA_POINTS(x,y,image) _delete(x,y)
#define DELETE_EXTRA_LIFE(x,y,image) _delete(x,y)
#define DELETE_INVINCIBILITY(x,y,image) _delete(x,y)	

void DRAW_BOMBS(void);	

// #define DRAW_BOMBS() \
	// displayBombs();
	
// #define DRAW_BOMBS() \
// { \
	// unsigned char ij = 0; \
	// for(;ij<BOMBS_NUMBER;++ij) \
	// { \
		// DRAW_BOMB(bombs[ij]._x, bombs[ij]._y, bombs[ij]._imagePtr); \
	// } \
// }

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
#if defined(ATARI_MODE1) && (defined(__ATARI__) || defined(__ATARIXL__))
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
#elif defined(__SPECTRUM__)
	#define PRINT(x,y,str) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); printf(str); } while(0);
	#define PRINTF(x,y,str,val) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); printf(str,val); } while(0);
#elif defined(__VIC20__) && defined(TINY_GAME) && defined(ALT_PRINT)
	void PRINT(unsigned char x, unsigned char y, char * str);
	#define PRINTF
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#include <coco.h>
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);	
#else
	#define PRINT(x,y,str) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); cprintf(str); } while(0);
	#define PRINTF(x,y,str,val) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); cprintf(str,val); } while(0);
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

// COLORS AND CLEAR SCREEN
#if defined(__SPECTRUM__) && !defined(CLIB_ANSI)
	#include <stdio.h>
	#include <arch/zx.h>
	#define SET_TEXT_COLOR(c) printf("\020%c",c)

	#define CLEAR_SCREEN()  {zx_cls(PAPER_BLACK|INK_WHITE);}
#elif defined(__CPC__) 
	#define SET_TEXT_COLOR(c) textcolor(c);

	#define CLEAR_SCREEN() printf("\x1B[37;40m\x1B[2J")
#elif defined(__VG5K__) 
	#define SET_TEXT_COLOR(c) textcolor(c)

	void CLEAR_SCREEN();
#elif  defined(__ATARI5200__) || (defined(__SVI__) && defined(MSX_MODE0)) || defined(__M5__) || defined(__SC3000__) || defined(__MSX__) || defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__)
	#define SET_TEXT_COLOR(c) {};

	#define CLEAR_SCREEN() {clrscr();};		
#elif defined(__ACE__) || defined(__GAL__)
	#define SET_TEXT_COLOR(c) {};

	#define CLEAR_SCREEN() do {unsigned char i; clrscr();for(i=0;i<YSize;++i){gotoxy(0,i);cprintf("                                ");}} while(0)
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#define SET_TEXT_COLOR(c) {}
	
	void CLEAR_SCREEN(void);
	
#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)
	#define SET_TEXT_COLOR(c) {};

	// WORK AROUND to fix what clrscr destroys
	#define CLEAR_SCREEN() { \
	clrscr(); \
	_setcolor_low(0, TGI_COLOR_RED); \
	_setcolor_low(1, TGI_COLOR_WHITE); \
	_setcolor_low(2, TGI_COLOR_CYAN); 	\
	_setcolor_low(3, TGI_COLOR_BROWN); \
	_setcolor_low(4, TGI_COLOR_BLACK);	};
	
#elif defined(__ATMOS__)
	#define SET_TEXT_COLOR(c) textcolor(c)
	
	#define CLEAR_SCREEN() do {clrscr(); INIT_GRAPHICS(); } while(0)
#else // CC65 conio case
	#define SET_TEXT_COLOR(c) (void) textcolor (c);
	
	#define CLEAR_SCREEN() clrscr();
#endif
	
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

