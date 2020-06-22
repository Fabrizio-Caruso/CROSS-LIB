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

#include "cross_lib.h"
#include "display_target_color.h"


#if !defined(NO_GRAPHICS)
    #if defined(CONIO_LIB)
        #include<conio.h>
    #elif defined(__CMOC__) && !defined(__WINCMOC__)
        #include <coco.h>
    #endif
#endif

#include "conio_patch.h"
#include "standard_libs.h"

struct ImageStruct
{
    uint8_t _imageData;
    #if !defined(NO_COLOR)
        uint8_t _color;
    #endif
};

typedef struct ImageStruct Image;


#if !defined(NO_STATS) \
    || X_OFFSET!=0 || Y_OFFSET!=0

    void _draw_stat(uint8_t x, uint8_t y, Image * image);

    void _draw(uint8_t x, uint8_t y, Image * image);
#else
    void _draw(uint8_t x, uint8_t y, Image * image);    
#endif

#if defined(Z88DK_SPRITES) && defined(ALT_PRINT)
    void _draw_ch(uint8_t x, uint8_t y, uint8_t ch);
#endif

#if defined(__LASER500__) && defined(LASER500_BITMAP)
    void laser500_gr4_draw(unsigned char x, unsigned char y, unsigned char *image, unsigned char color);
#endif


#if !defined(BUFFERED)
    #define REFRESH()
#else
    #include "buffered_graphics.h"
    
    #define REFRESH() \
        do \
        { \
            putchar('\n'); \
            display_all(); \
        } while(0);
#endif

    
#if !defined(NO_BLINKING)
    void _blink_draw(uint8_t x,uint8_t y,Image * image, uint8_t * blinkCounter);
#else
    #define _blink_draw(x,y,image,blinkCounter) _draw(x,y,image)
#endif

void _delete(uint8_t x, uint8_t y);

// VERTICAL AND HORIZONTAL BORDER
#if !defined(TINY_GAME)
    #define DRAW_VERTICAL_BORDER(x) DRAW_VERTICAL_LINE(x,0,YSize-1)
    #define DRAW_HORIZONTAL_BORDER(y) DRAW_HORIZONTAL_LINE(0,y,XSize-1)
#else    
    #define DRAW_VERTICAL_BORDER(x)    
    #define DRAW_HORIZONTAL_BORDER(y)
#endif

#define WALL_COLOR COLOR_YELLOW

#if !defined(NO_WALL)
    #define DRAW_BORDERS() \
        SET_TEXT_COLOR(WALL_COLOR); \
        DRAW_HORIZONTAL_BORDER(0); \
        DRAW_HORIZONTAL_BORDER(YSize-1); \
        DRAW_VERTICAL_BORDER(0); \
        DRAW_VERTICAL_BORDER(XSize-1); 
#else
    #define DRAW_BORDERS()
#endif


#define DRAW_HORIZONTAL_WALLS(length) \
    SET_TEXT_COLOR(WALL_COLOR); \
    DRAW_HORIZONTAL_LINE(1,YSize/2,length); \
    DRAW_HORIZONTAL_LINE(XSize-1-length,YSize/2,length);


// PRINT AND PRINTD
#  if defined(ALT_DISPLAY_STATS) 
    void PRINT(uint8_t x, uint8_t y, char * str);
    #define PRINTD(x,y,str,val) _displayShort(val)
    void _displayShort(uint16_t val);
    uint16_t loc(uint8_t x, uint8_t y);
#elif defined(NO_PRINT)
    #define PRINT(x,y,str)
    #define PRINTD(x,y,str,val)
#elif defined(ALT_PRINT)
    void PRINT(uint8_t x, uint8_t y, char * str);
    void PRINTD(uint8_t x, uint8_t y, char * str, uint16_t val);
#else
    #define PRINT(x,y,str) do {gotoxy(x+X_OFFSET,y); cprintf(str); } while(0);
    #define PRINTD(x,y,str,val) do {gotoxy(x+X_OFFSET,y); cprintf(str,val); } while(0);
#endif


// DRAW HORIZONTAL AND VERTICAL LINES
#if !defined(TINY_GAME)
    void DRAW_HORIZONTAL_LINE(uint8_t x, uint8_t y, uint8_t length);
    void DRAW_VERTICAL_LINE(uint8_t x, uint8_t y, uint8_t length);    
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
    || (defined(__ATARI__) && defined(ATARI_MODE1)) \
    || defined(__COCO__) || defined(__DRAGON__) || defined(__MO5__) || defined(__WINCMOC__) \
    || (defined(__SVI__) && defined(MSX_MODE0)) || defined(__MSX__) \
    || defined(__ATMOS__) \
    || defined(Z88DK_SPRITES)
    #define SET_TEXT_COLOR(c) 
#elif defined(__NCURSES__)
    #if defined(__ATARI_ST__)
        #include <ncurses/curses.h>
    #else
        #include <ncurses.h>
    #endif
    #define SET_TEXT_COLOR(c) attron(COLOR_PAIR(c))
#else
    #define SET_TEXT_COLOR(c) textcolor(c)
#endif

#if defined(Z88DK_SPRITES) || defined(__MO5__)
	#define _Z88DK_SPRITE_OFFSET 0x20
#else
	
	#define _Z88DK_SPRITE_OFFSET 0x00
#endif

#if defined(__COCO__) || defined(__DRAGON__)
    #define NOT_INVERTED 64
    #define INVERTED -64
#endif

#  if defined(__VIC20__) && defined(VIC20_EXP_8K) && defined(REDEFINED_CHARS)
    #define _SPACE 0x72
#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED) && defined(REDEFINED_CHARS)
    #define _SPACE 0xA0
#elif (defined(__ATARI__) && defined(ATARI_MODE1))
    #define _SPACE 0
#elif defined(__C16__) && defined(REDEFINED_CHARS) && defined(C16_UNEXPANDED)
    #define _SPACE 0x60
#elif defined(Z88DK_SPRITES)
    #define _SPACE (0x40-_Z88DK_SPRITE_OFFSET)
#elif defined(__COCO__) || defined(__DRAGON__)
    #define _SPACE (' '+NOT_INVERTED)
#elif defined(__ATARI5200__) && defined(MEMORY_MAPPED)
    #define _SPACE 0
#elif defined(CPCRSLIB)
    #define _SPACE (19*2)
#elif defined(__REX__)
	#include "cpc_cpcrslib_settings.h"
#else
    #define _SPACE ' '
#endif 


// CLEAR SCREEN
#  if defined(ALT_CLEAR_SCREEN) || defined(DEFAULT_CLEAR_SCREEN)
    void CLEAR_SCREEN(void);
#elif defined(__ATARI_LYNX__)
    #define CLEAR_SCREEN() tgi_clear()
#elif defined(__CPC__) 
    #define CLEAR_SCREEN() printf("\x1B[37;40m\x1B[2J")
#elif defined(__ATMOS__)
    #define CLEAR_SCREEN() do {clrscr(); INIT_GRAPHICS(); } while(0)
#elif defined(Z88DK_SPRITES)
    #include <games.h>
    #include <graphics.h>
    #define CLEAR_SCREEN() clg()
#elif defined(__NCURSES__)
    #if defined(__ATARI_ST__)
        #include <ncurses/curses.h>
    #else
        #include <ncurses.h>
    #endif
    #define CLEAR_SCREEN() clear()
#elif defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__CIDELSA__)
    #include <devkit/video/vis_video.h>
    #include "memory_mapped_graphics.h"
    
    #define CLEAR_SCREEN() vidclr(BASE_ADDR,XSize*40)
#else // CONIO case
    #define CLEAR_SCREEN() clrscr()
#endif


// BORDER AND BACKGROUND COLORS
#if defined(CC65) && !defined(__ATARI5200__) && !defined(__SUPERVISION__) && !(defined(FORCE_NO_CONIO_LIB) && defined(C16_UNEXPANDED))
    #define SET_BORDER_COLOR(c) (void) bordercolor(c)
    #define SET_BACKGROUND_COLOR(c) (void) bgcolor (c)
#elif defined(__LASER500__)
    #define SET_BORDER_COLOR(c) (void) bordercolor(c)
    #define SET_BACKGROUND_COLOR(c) {}
#else
    #define SET_BORDER_COLOR(c) {}
    #define SET_BACKGROUND_COLOR(c) {}
#endif    



#if !defined(NO_INIT_GRAPHICS)
    void INIT_GRAPHICS(void);
#else
    #define INIT_GRAPHICS() 
#endif
    
#endif // _DISPLAY_MACROS

