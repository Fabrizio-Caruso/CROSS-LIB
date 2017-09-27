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
#endif
	
#if !defined(__SPECTRUM__)
	#include <conio.h>
#endif

#if defined(__ATARI5200__) || defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__APPLE2ENH__) || defined(__ATMOS__)
	#include "patch/generic_conio_patch.h"
#endif
#if defined(__ATMOS__)
	#include "atmos/atmos_conio_patch.h"
#elif defined(__SPECTRUM__)
	#include "patch/z88dk_conio_implementation.h"
#elif defined(__CPC__)
	#include "patch/z88dk_conio_patch.h"
#elif defined(__MSX__)
	#include "patch/z88dk_conio_patch.h"
#elif defined(__SVI__)
	#include "patch/z88dk_conio_patch.h"	
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
#elif defined(__ZX81__)
	#include "patch/z88dk_conio_implementation.h"	
#elif defined(__ZX80__)
	#include "patch/z88dk_conio_implementation.h"		
#elif defined(__ACE__)
	#include "patch/z88dk_conio_implementation.h"		
#elif defined(__ENTERPRISE__)
	#include "patch/z88dk_conio_implementation.h"		
#elif defined(__MTX__)
	#include "patch/z88dk_conio_implementation.h"		
#elif defined(__M5__)
	#include "patch/z88dk_conio_implementation.h"		
#endif

struct ImageStruct
{
	unsigned char _imageData;
	unsigned char _color;
};

typedef struct ImageStruct Image;

//
#if defined(NARROW)
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


#if defined(__NES__)
	#define GET_SCREEN_SIZE(x,y) do {*x=32; *y=24;} while(0)
#elif defined(__ATARI5200__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1))
	#define GET_SCREEN_SIZE(x,y) do {*x=20; *y=24;} while(0)
#elif defined(__C128__) && defined(C128_80COL_VIDEO_MODE)
	#define GET_SCREEN_SIZE(x,y) do {*x=80-X_OFFSET; *y=25-Y_OFFSET;} while(0)
#elif defined(__CBM__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__)
	#define GET_SCREEN_SIZE(x,y) do {screensize(x,y); *x-=X_OFFSET; *y-=Y_OFFSET;} while(0)
#elif defined (__SPECTRUM__) && defined(SPECTRUM_64COL)
	#define GET_SCREEN_SIZE(x,y) do {*x=64-X_OFFSET; *y=24-Y_OFFSET;} while(0)
#elif defined (__SPECTRUM__) && defined(SPECTRUM_32COL)
	#define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-Y_OFFSET;} while(0)
#elif defined(__MSX__) && !defined(MSX_MODE1)
	#define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=24-Y_OFFSET;} while(0)
#elif defined(__MSX__) && defined(MSX_MODE1)
	#define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-Y_OFFSET;} while(0)
#elif defined(__SVI__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=24-Y_OFFSET;} while(0)
#elif defined(__CPC__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=25-Y_OFFSET;} while(0)
#elif defined(__VG5K__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=25-Y_OFFSET;} while(0)
#elif defined(__SC3000__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=38-X_OFFSET; *y=24-Y_OFFSET;} while(0)	
#elif defined(__AQUARIUS__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=24-Y_OFFSET;} while(0)	
#elif defined(__ZX81__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-Y_OFFSET;} while(0)
#elif defined(__ZX80__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-1-Y_OFFSET;} while(0)	
#elif defined(__ACE__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-Y_OFFSET;} while(0)
#elif defined(__ENTERPRISE__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=25-1-Y_OFFSET;} while(0)		
#elif defined(__VZ__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=17-Y_OFFSET;} while(0)	
#elif defined(__MTX__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-Y_OFFSET;} while(0)		
#elif defined(__TRS80__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=40-X_OFFSET; *y=25-Y_OFFSET;} while(0)		
#elif defined(__M5__) 
	#define GET_SCREEN_SIZE(x,y) do {*x=32-X_OFFSET; *y=24-Y_OFFSET;} while(0)		
#else
	#define GET_SCREEN_SIZE(x,y) do {screensize(x,y); *x-=X_OFFSET; *y-=Y_OFFSET;} while(0)
#endif

#if defined(REDEFINED_CHARS)
	extern Image PLAYER_LEFT;
	extern Image PLAYER_RIGHT;
	extern Image PLAYER_UP;
	extern Image PLAYER_DOWN;

	#define SHOW_LEFT() do {player._imagePtr = &PLAYER_LEFT; } while(0)
	#define SHOW_RIGHT() do {player._imagePtr = &PLAYER_RIGHT; } while(0)
	#define SHOW_UP() do {player._imagePtr = &PLAYER_UP; } while(0)
	#define SHOW_DOWN() do {player._imagePtr = &PLAYER_DOWN; } while(0)
#else
	#define SHOW_LEFT() { }
	#define SHOW_RIGHT() { }
	#define SHOW_UP() { }
	#define SHOW_DOWN() { }		
#endif



void _draw(unsigned char x,unsigned char y,Image * image);
#define DRAW_PLAYER(x,y,image) _draw(x,y,image)
#define DRAW_GHOST(x,y,image) _draw(x,y,image)
#define DRAW_INVINCIBLE_GHOST(x,y,image) _draw(x,y,image)
#define DRAW_BOMB(x,y,image) _draw(x,y,image)
#define DRAW_MISSILE(x,y,image) _draw(x,y,image)

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y);
#endif
	
void _blink_draw(unsigned char x,unsigned char y,Image * image, unsigned char * blinkCounter);
#define DRAW_POWERUP(x, y, image) _blink_draw(x,y,image, &powerUpBlink) 
#define DRAW_GUN(x, y, image) _blink_draw(x,y,image, &gunBlink)
#define DRAW_EXTRA_POINTS(x, y, image) _blink_draw(x,y,image, &extraPointsBlink) 
#define DRAW_EXTRA_LIFE(x, y, image) _blink_draw(x,y,image, &extraLifeBlink)
#define DRAW_INVINCIBILITY(x, y, image) _blink_draw(x,y,image, &invincibilityBlink) 
#define DRAW_BLINKING_PLAYER(x, y, image) _blink_draw(x,y,image, &playerBlink)

void _delete(unsigned char x, unsigned char y);
#define DELETE_PLAYER(x,y,image) _delete(x,y)
#define DELETE_GHOST(x,y,image) _delete(x,y)
#define DELETE_INVINCIBLE_GHOST(x,y,image) _delete(x,y)
#define DELETE_BOMB(x,y,image) _delete(x,y)
#define DELETE_POWERUP(x,y,image) _delete(x,y)
#define DELETE_GUN(x,y,image) _delete(x,y)
#define DELETE_MISSILE(x,y,image) _delete(x,y)
#define DELETE_EXTRA_POINTS(x,y,image) _delete(x,y)
#define DELETE_EXTRA_LIFE(x,y,image) _delete(x,y)
#define DELETE_INVINCIBILITY(x,y,image) _delete(x,y)	
	
#define DRAW_BOMBS() \
{ \
	unsigned char i = 0; \
	for(;i<BOMBS_NUMBER;++i) \
	{ \
		DRAW_BOMB(bombs[i]._x, bombs[i]._y, bombs[i]._imagePtr); \
	} \
}

// VERTICAL AND HORIZONTAL BORDER
#if defined(__MSX__) || defined(__AQUARIUS__)
	#define DRAW_VERTICAL_BORDER(x) DRAW_VERTICAL_LINE(x,0,YSize)
	#define DRAW_HORIZONTAL_BORDER(y) DRAW_HORIZONTAL_LINE(0,y,XSize)	
#else
	#define DRAW_VERTICAL_BORDER(x) DRAW_VERTICAL_LINE(x,0,YSize-1)
	#define DRAW_HORIZONTAL_BORDER(y) DRAW_HORIZONTAL_LINE(0,y,XSize-1)	
#endif

#if defined(__MSX__) && !defined(REDEFINED_CHARS)
	#define DRAW_HORIZONTAL_LINE(x,y,length)
	#define DRAW_VERTICAL_LINE(x,y,length)
#endif

// FULL BORDER
#if defined(__ATMOS__)
	#define DRAW_BORDERS() \
	{ \
		unsigned char i; \
		\
		for(i=0;i<38-1;++i) \
		{ \
			POKE(0xBB80+(i+X_OFFSET)+(0+Y_OFFSET)*40,'-' + 128); \
		} \
		for(i=0;i<YSize-Y_OFFSET;++i) \
		{ \
			POKE(0xBB80+(0+X_OFFSET)+(1+i+Y_OFFSET)*40,'|' + 128); \
		} \
		for(i=0;i<38-1;++i) \
		{ \
			POKE(0xBB80+(i+X_OFFSET)+(YSize-1+Y_OFFSET)*40,'-' + 128); \
		} \
		for(i=0;i<YSize-Y_OFFSET;++i) \
		{ \
			POKE(0xBB80+(38-1+X_OFFSET)+(1+i+Y_OFFSET)*40,'|' + 128); \
		} \
		POKE(0xBB80+(0+X_OFFSET)+(0+Y_OFFSET)*40,CH_ULCORNER+128); \
		POKE(0xBB80+(38-1+X_OFFSET)+(0+Y_OFFSET)*40,CH_URCORNER+128); \
		POKE(0xBB80+(0+X_OFFSET)+(YSize-1+Y_OFFSET)*40,CH_LLCORNER+128); \
		POKE(0xBB80+(38-1+X_OFFSET)+(YSize-1+Y_OFFSET)*40,CH_LRCORNER+128); \
	} 
#elif defined(__SPECTRUM__)
		
	#define DRAW_BORDERS() \
	{ \
		unsigned char i; \
		SET_TEXT_COLOR(TEXT_COLOR); \
		gotoxy(0+X_OFFSET,0+Y_OFFSET); \
		printf("--------------------------------"); \
		gotoxy(0+X_OFFSET,YSize-1+Y_OFFSET); \
		printf("--------------------------------"); \
		for(i=0;i<YSize;++i) \
		{ \
			gotoxy(0 + X_OFFSET,i + Y_OFFSET); printf("|"); \
			gotoxy(XSize-1+X_OFFSET,i+Y_OFFSET);printf("|"); \
		} \
	}
#elif ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) 
	#define DRAW_BORDERS()\
	{ \
		SET_TEXT_COLOR(TEXT_COLOR); \
		gotoxy(0+X_OFFSET,0+Y_OFFSET); \
		cputc ('X');\
		DRAW_HORIZONTAL_LINE (1+X_OFFSET,0+Y_OFFSET, XSize-2);\
		cputc ('X');\
		DRAW_VERTICAL_LINE(0+X_OFFSET, 1+Y_OFFSET, YSize - 2);\
		gotoxy(0+20,(YSize-1)/2); \
		cputc ('X'); \
		DRAW_HORIZONTAL_LINE (1+X_OFFSET,YSize-1,XSize-2);\
		cputc ('X');\
		DRAW_VERTICAL_LINE(XSize - 1, 1+Y_OFFSET, YSize - 2); \
	}
#elif defined(CC65) && (defined(WIDE) || defined(__VIC20__))
	#define DRAW_BORDERS()\
		{ \
			SET_TEXT_COLOR(TEXT_COLOR); \
			gotoxy(0+X_OFFSET,0+Y_OFFSET); \
			cputc (CH_ULCORNER);\
			chline (XSize-2);\
			cputc (CH_URCORNER);\
			cvlinexy (0+X_OFFSET, 1+Y_OFFSET, YSize - 2);\
			cputc (CH_LLCORNER);\
			chline (XSize-2);\
			cputc (CH_LRCORNER);\
			cvlinexy (XSize - 1, 1+Y_OFFSET, YSize - 2); \
		}	
#elif defined(__AQUARIUS__) || defined(__ATARI5200__)
		#define DRAW_BORDERS() \
		{ \
		}	
#else
		#define DRAW_BORDERS() \
		{ \
			SET_TEXT_COLOR(TEXT_COLOR); \
			DRAW_HORIZONTAL_BORDER(0); \
			DRAW_HORIZONTAL_BORDER(YSize-1); \
			DRAW_VERTICAL_BORDER(0); \
			DRAW_VERTICAL_BORDER(XSize-1); \
		}	
#endif


// PRINT AND PRINTF
#if defined(ATARI_MODE1) && (defined(__ATARI__) || defined(__ATARIXL__))
	void PRINT(unsigned char x, unsigned char y, char * str);
	
	#define PRINTF(x,y,...)  \
	{ \
		if((y+Y_OFFSET)%2==1) \
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
	#define PRINT(x,y,str) do {gotoxy(x+X_OFFSET,y+Y_OFFSET + ADJUST); cprintf(str); } while(0);
	#define PRINTF(x,y,str,val) do {gotoxy(x+X_OFFSET,y+Y_OFFSET + ADJUST); cprintf(str,val); } while(0);
#endif

	
// DRAW HORIZONTAL AND VERTICAL LINES
#if defined(__ATMOS__)
	#include<peekpoke.h>

	#define DRAW_VERTICAL_LINE(x,y,length) \
	{ \
		unsigned char i; \
		for(i=0;i<length;++i) \
		{ \
			POKE(0xBB80+(x+X_OFFSET)+(y+i+Y_OFFSET)*40,'|'+128); \
		} \
	}

#elif defined(__VG5K__)
	void DRAW_HORIZONTAL_LINE(unsigned char x, unsigned char y, unsigned char length);

	void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length);

	void _draw_ch(unsigned char x, unsigned char y, unsigned char ch, unsigned char col);	

#elif defined(__SPECTRUM__)
	#include <stdio.h>

	#if defined(REDEFINED_CHARS)
		void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length);
	#else
		#define DRAW_VERTICAL_LINE(x, y, length) \
		{ \
			unsigned char i; \
			\
			SET_TEXT_COLOR(COLOR_WHITE); \
			for(i=0;i<length;++i) \
			{ \
				gotoxy(x+X_OFFSET,y+Y_OFFSET+i);  printf("%c",'|'); \
			} \
		}		
	#endif


#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)
	#define DRAW_VERTICAL_LINE(x,y,length) \
	{ \
		unsigned char i; \
		for(i=0;i<length;i++) \
		{ \
			if((y+Y_OFFSET+i)%2==1) \
			{ \
				gotoxy(x+20+X_OFFSET,(y+Y_OFFSET+i)/2); \
			} \
			else \
			{ \
				gotoxy(x+X_OFFSET,(y+Y_OFFSET+i)/2); \
			} \
			cputc('a'-64-128); \
		} \
	}
	
	#define DRAW_HORIZONTAL_LINE(x,y,length) \
	{ \
		unsigned char i; \
		if((y+Y_OFFSET)%2==1) \
		{ \
			for(i=0;i<length;++i) \
			{ \
				gotoxy(x+20+X_OFFSET+i,(y+Y_OFFSET)/2); \
				cputc('-'-128); \
			} \
		} \
		else \
		{ \
			for(i=0;i<length;++i) \
			{ \
				gotoxy(x+X_OFFSET+i,(y+Y_OFFSET)/2); \
				cputc('-'-128); \
			} \
		} \
	}

#elif defined(__CBM__) || defined(__ATARI5200__) || defined(__ATARI__) || defined(__ATARIXL__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__)
	#define DRAW_VERTICAL_LINE(x,y,length) do {(void) textcolor (COLOR_WHITE);cvlinexy (x+X_OFFSET,y+Y_OFFSET,length);} while(0)	
#else		
	#if defined(__MSX__) || defined(__CPC__)
		void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length);
		void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length);

	#elif defined(__AQUARIUS__)
		void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length);
		#define DRAW_HORIZONTAL_BORDER(y)
	#elif defined(__ZX81__) || defined(__ZX80__)
		void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length);
		void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length);
	#else
		#define DRAW_VERTICAL_LINE(x,y,length) \
		{ \
			unsigned char i; \
			for(i=0;i<length;++i) \
			{ \
				gotoxy(X_OFFSET+x,Y_OFFSET+y+i); \
				cputc('|'); \
			} \
		}
		
		#define DRAW_HORIZONTAL_LINE(x,y,length) \
		{ \
			unsigned char i; \
			gotoxy(X_OFFSET+x,Y_OFFSET+y);  \
			for(i=0;i<length;++i) \
			{ \
				cputc('-'); \
			} \
		} \
			
	#endif

#endif


// COLORS AND CLEAR SCREEN
#if defined(__SPECTRUM__)
	#include <stdio.h>
	#define SET_TEXT_COLOR(c) printf("\020%c",c)

	#define SET_BORDER_COLOR(c) {};

	#define SET_BACKGROUND_COLOR(c) {};

	#define CLEAR_SCREEN() printf("\xc")
#elif defined(__CPC__) 
	#define SET_TEXT_COLOR(c) textcolor(c);

	#define SET_BORDER_COLOR(c) {};

	#define SET_BACKGROUND_COLOR(c) {};	

	#define CLEAR_SCREEN() printf("\x1B[37;40m\x1B[2J")
#elif defined(__VG5K__) 
	#define SET_TEXT_COLOR(c) {};

	#define SET_BORDER_COLOR(c) {};

	#define SET_BACKGROUND_COLOR(c) {};	

	void CLEAR_SCREEN();
	
#elif defined(__M5__) || defined(__SC3000__) || defined(__MSX__) || defined(__SVI__) || defined(__AQUARIUS__) || defined(__VZ__) || defined(__ZX81__) || defined(__ZX80__) 
	#define SET_TEXT_COLOR(c) {};
	
	#define SET_BORDER_COLOR(c) {};

	#define SET_BACKGROUND_COLOR(c) {};	

	#define CLEAR_SCREEN() {clrscr();};		
#elif defined(__ACE__)
	#define SET_TEXT_COLOR(c) {};
	
	#define SET_BORDER_COLOR(c) {};

	#define SET_BACKGROUND_COLOR(c) {};	

	#define CLEAR_SCREEN() do {unsigned char i; clrscr();for(i=0;i<YSize;++i){gotoxy(0,i);cprintf("                                ");}} while(0)

#elif defined(__ATARI5200__) || defined(ATARI_MODE1)
	#define SET_TEXT_COLOR(c) {};
	
	#define SET_BORDER_COLOR(c) (void) bordercolor(c);

	#define SET_BACKGROUND_COLOR(c) {};

	#define CLEAR_SCREEN() {clrscr();};	
#elif defined(__ATMOS__)
	#define SET_TEXT_COLOR(c) textcolor(c)

	#define SET_BORDER_COLOR(c) bordercolor(c)

	#define SET_BACKGROUND_COLOR(c) bgcolor(c)
	
	#define CLEAR_SCREEN() do {clrscr(); INIT_GRAPHICS(); } while(0)
#else // CC65 conio case
	#define SET_TEXT_COLOR(c) (void) textcolor (c);

	#define SET_BORDER_COLOR(c) (void) bordercolor(c)

	#define SET_BACKGROUND_COLOR(c) (void) bgcolor (c);
	
	#define CLEAR_SCREEN() clrscr();
#endif
	
void INIT_IMAGES(void);

void INIT_GRAPHICS(void);
	
#endif // _DISPLAY_MACROS