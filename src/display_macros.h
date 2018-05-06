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

	
#if defined(CONIO_LIB)
	#include<conio.h>
#endif

#if defined(__CREATIVISION__) || defined(__GAMATE__) || defined(__OSIC1P__) || defined(__ATARI5200__) || defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__APPLE2ENH__) || defined(__ATMOS__)
	#include "patch/generic_conio_patch.h"
#endif
#  if defined(Z88DK_PUTC4X6)
	#include "patch/z88dk_putc4x6_conio_implementation.h"
#elif defined(__ATMOS__)
	#include "atmos/atmos_conio_patch.h"
#elif defined(__SUPERVISION__) 
	#include "patch/supervision_conio_implementation.h"	
#elif defined(__ATARI_LYNX__) 
	#include "patch/atari_lynx_conio_implementation.h"	
#elif defined(__WINCMOC__)
	#include "patch/wincmoc_conio_patch.h"	
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#include "patch/cmoc_conio_implementation.h"
#elif defined(__SMS__)
	#include "patch/sms_conio_implementation.h"
#elif defined(__ENTERPRISE__)
	#include "patch/enterprise_conio_implementation.h"
#elif defined(__GCC__)
	#include "patch/ncurses_conio_implementation.h"	
#elif defined(__PX8__) || defined(__ABC800__) || defined(__CPM_ADM3A__)
	#include "patch/adm3a_conio_implementation.h"
#elif defined(__EINSTEIN__)
	#include "patch/vt100_conio_implementation.h"
#elif defined(__CPM_VT52__)
	#include "patch/vt52_conio_implementation.h"
#elif defined(__CPM_VT100__)
	#include "patch/vt100_conio_implementation.h"
#elif defined(__KC__)
	#include "patch/kc_conio_implementation.h"	
#elif defined(__M5__)
	#include "patch/m5_conio_implementation.h"	
#elif defined(Z88DK_SPRITES)
	#include "patch/z88dk_conio_patch.h"	
#elif defined(__X1__) || defined(__Z9001__) || defined(__Z1013__) || defined(__OSCA__) || defined(__MC1000__) \
	  || defined(__ABC80__) || defined(__PC6001__) || defined(__SRR__) || defined(__NASCOM__) || defined(__P2000__) \
	  || defined(__BEE__) || defined(__TI8X__) || defined(__TI82__) || defined(__TI83__) || defined(__TI85__) \
	  || defined(__TI86__) || defined(__TI86S__) || defined(__MZ__) || defined(__GAL__) || defined(__SC3000__) \
	  || (defined(__SPECTRUM__) && defined(CLIB_ANSI)) || defined(__SAM__) || defined(__CPC__) \
	  || (defined(__SVI__) && !defined(MSX_MODE0)) || defined(__VG5K__) || defined(__AQUARIUS__) \
	  || defined(__VZ__) || defined(__MTX__) || defined(__Z88__) || defined(__PX4__) \
	  || defined(__EG2K__) || defined(__TRS80__) || defined(__ACE__) 
	#include "patch/z88dk_conio_patch.h"					
#elif (defined(__SPECTRUM__) && !defined(CLIB_ANSI)) || defined(__MSX__) || (defined(__SVI__) && defined(MSX_MODE0)) \
	  || defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__)
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


#if defined(Z88DK_SPRITES)
	#if defined(__NC100__) 
		#define SPRITE_X_SIZE 8
		#define SPRITE_X_STEP 6
		#define SPRITE_Y_SIZE 7
		#define SPRITE_Y_STEP 5
	#else
		#define SPRITE_X_SIZE 8
		#define SPRITE_X_STEP 6	
		#define SPRITE_Y_SIZE 8
		#define SPRITE_Y_STEP 6		
	#endif
	
	#ifndef SPRITE_X_STEP
		#define SPRITE_X_STEP SPRITE_X_SIZE
	#endif
	
	#ifndef SPRITE_Y_STEP
		#define SPRITE_Y_STEP SPRITE_Y_SIZE
	#endif
#endif


#  if defined(__Z1013__)
	  #define YSize (32-Y_OFFSET)
#elif defined(__KC__) && defined(Z88DK_SPRITES)
	  #define YSize ((256/SPRITE_Y_STEP)-Y_OFFSET)	
#elif defined(__ATMOS__)
	#define YSize (28-Y_OFFSET)		  
#elif defined(__CBM610__) || defined(__PET__) || defined(__C128__) \
      || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) \
	  || defined(__PET__) || defined(__CBM610__) \
	  || defined(__MZ__) || defined(__BEE__) || defined(__AQUARIUS__) || defined(__X1__) \
	  || defined(__PCE__) || defined(__CPC__) || defined(__VG5K__) || defined(__ABC80__) \
	  || defined(__ENTERPRISE__) 
	#define YSize (25-Y_OFFSET)
#elif defined(__CREATIVISION__) || defined(__MSX__) || defined(__SVI__) \
    || defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__) \
	|| defined(__SPECTRUM__) || defined(__SC3000__) || defined(__MTX__) \
    || defined(__APPLE2ENH__) || defined(__APPLE2__) \
	|| defined(__Z9001__) || defined(__P2000__) \
	|| defined(__NES__) || (defined(__ATARI__) || defined(__ATARIXL__)) \
	|| defined(__ACE__) \
	|| defined(__CPM_ADM3A__) || defined(__CPM_VT52__) || defined(__CPM_VT100__) \
	|| defined(__EINSTEIN__) \
	|| defined(__OSIC1P__) \
	|| defined(__MC1000__)
	#define YSize (24-Y_OFFSET)		
#elif defined(__VIC20__)
	#define YSize 23	
#elif defined(__SAM__)
	#define YSize (24-2-Y_OFFSET)
#elif (defined(__NC200__) && defined(Z88DK_SPRITES))
	#define YSize ((128/SPRITE_Y_STEP)+1)
#elif (defined(__PX4__) && defined(Z88DK_PUTC4X6))
	#define YSize (10+1)
#elif defined(__ATARI_LYNX__)
	#define YSize 13
#elif defined(__Z88__) || (defined(__PX4__) && !defined(Z88DK_PUTC4X6))|| defined(__PX8__)
	#define YSize 8
#elif (defined(__NC100__) && defined(Z88DK_SPRITES)) 
	#define YSize ((64/SPRITE_Y_STEP)+1)
#else
	#define YSize 16
#endif


#if !defined(FORCE_XSIZE)
	#  if defined(__CBM610__) || defined(__PET__) || (defined(__C128__) && defined(C128_80COL_VIDEO_MODE)) \
		  || defined(__BEE__) || defined(__PET__) || defined(__CBM610__) \
		  || defined(__PX8__) || defined(__CPM_ADM3A__) || defined(__CPM_VT52__) || defined(__CPM_VT100__)
		#define XSize 80
	#elif defined(__TRS80__) || defined(__EG2K__)
		#define XSize 64
	#elif defined(__PCE__) || (defined(__PX4__) && defined(Z88DK_PUTC4X6))
		 #define XSize 60
	#elif ((defined(__NC100__) || defined(__NC200__)) && defined(Z88DK_SPRITES))
		#define XSize (480/SPRITE_X_STEP)
	#elif defined(__NASCOM__)
		#define XSize 48
	#elif defined(__VG5K__) || defined(__APPLE2ENH__) || defined(__APPLE2__) \
		  || defined(__Z9001__) || defined(__P2000__) || defined(__ABC80__) \
		  || defined(__MZ__) || defined(__X1__) || defined(__ATMOS__) \
		  || defined(__CPC__) || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) \
		  || (defined(__C128__) && !defined(C128_80COL_VIDEO_MODE)) \
		  || defined(__AQUARIUS__) || (defined(__SVI__) && defined(MSX_MODE0)) \
		  || defined(__ENTERPRISE__) \
		  || (defined(__PX4__) && !defined(Z88DK_PUTC4X6)) \
		  || defined(__EINSTEIN__) 		  
		#define XSize (40-X_OFFSET)
	#elif defined(__KC__) && defined(Z88DK_SPRITES)
		#define XSize (320/SPRITE_X_STEP)
	#elif defined(__VZ__) || defined(__NES__) || defined(__CREATIVISION__) || defined(__MSX__) \
		  || (defined(__SVI__) && !defined(MSX_MODE0) ) || defined(__ZX81__) || defined(__ZX80__) \
		  || defined(__LAMBDA__) || defined(__SPECTRUM__) || defined(__PC6001__) \
		  || defined(__SC3000__) || defined(__MC1000__) || defined(__MTX__) || defined(__SAM__) \
		  || defined(__GAL__) || defined(__CMOC__) || defined(__WINCMOC__) \
		  || defined(__Z1013__) || defined(__Z88__) || defined(__ACE__)
		#define XSize 32	
	#elif defined(__OSIC1P__)
		#define XSize 24
	#elif defined(__VIC20__)
		#define XSize 22
	#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1) \
		  || (defined(__ATARI__) || defined(__ATARIXL__)) && !defined(ATARI_MODE1) \
		  || defined(__ATARI_LINX__)
		#define XSize 20
	#else
		#define XSize 16
	#endif
#else
	#define XSize FORCE_XSIZE
#endif

#if XSize < YSize
	#define MIN_SIZE XSize
#else
	#define MIN_SIZE YSize
#endif

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
#  if defined(__C16__) && defined(ALT_PRINT)
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
#elif defined(__SPECTRUM__)
	#define PRINT(x,y,str) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); printf(str); } while(0);
	#define PRINTF(x,y,str,val) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); printf(str,val); } while(0);
#elif defined(__VIC20__) && defined(TINY_GAME) && defined(ALT_PRINT) && defined(VIC20_UNEXPANDED)
	#define PRINT(x,y,str)
	#define PRINTF(x,y,str,val)	
#elif defined(__GAL__) && defined(TINY_GAME) && defined(ALT_PRINT)
	#define PRINT(x,y,str)
	#define PRINTF(x,y,str,val)	
#elif defined(__SMS__) 
	#define PRINT(x,y,str) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); printf(str); } while(0);
	#define PRINTF(x,y,str,val) do {gotoxy(x+X_OFFSET,y+Y_OFFSET); printf(str,val); } while(0);
#elif defined(__ENTERPRISE__)
	#define PRINT(x,y,str) 
	#define PRINTF(x,y,str,val)		
#elif defined(__ATARI_LYNX__)
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);	
#elif defined(__AQUARIUS__) && defined(TINY_GAME) && defined(ALT_PRINT)
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);	
#elif defined(__VIC20__) && defined(TINY_GAME) && defined(ALT_PRINT) && !defined(VIC20_UNEXPANDED)
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);	
#elif defined(__VIC20__) && defined(FULL_GAME) && defined(ALT_PRINT)
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);		
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#include <coco.h>
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);	
#elif defined(__SUPERVISION__)
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);	
#elif defined(__TRS80__) || defined(__EG2K__)
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);	
#elif defined(Z88DK_PUTC4X6)
	void PRINT(unsigned char x, unsigned char y, char * str);
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short);	
#elif defined(Z88DK_SPRITES)
	#define PRINT(x,y,str) 
	#define PRINTF(x,y,str,val)	
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

#elif defined(__TRS80__) || defined(__EG2K__)
	#define SET_TEXT_COLOR(c) {}
	
	void CLEAR_SCREEN(void);
#elif defined(__SUPERVISION__)
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
#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED)
	#define SET_TEXT_COLOR(c)
	#define CLEAR_SCREEN() clrscr()
#elif defined(__VIC20__) && defined(ALT_PRINT)
	#define SET_TEXT_COLOR(c)
	#define CLEAR_SCREEN() clrscr()
#elif defined(__ENTERPRISE__)
	#define SET_TEXT_COLOR(c)
	#define CLEAR_SCREEN()
#elif defined(__ATARI_LYNX__) || (defined(__AQUARIUS__) && defined(TINY_GAME))
	#define SET_TEXT_COLOR(c)
	void CLEAR_SCREEN(void);
	
#elif defined(Z88DK_SPRITES)
	#include <games.h>
	#include <graphics.h>
	
	#define SET_TEXT_COLOR(c)
	#define CLEAR_SCREEN() clg()
	
#else // CC65 conio case
	#if !defined(NO_COLOR)
		#define SET_TEXT_COLOR(c) (void) textcolor (c);
	#else
		#define SET_TEXT_COLOR(c)	
	#endif
	
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

