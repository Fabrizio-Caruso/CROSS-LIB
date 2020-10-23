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
        #include <conio.h>
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


#define _draw_stat(x, y, image) \
    __DRAW(X_OFFSET+(x),(y),(image))

#define _XLIB_DRAW(x,y,image) \
    _draw_stat(x, y, image) 

#define _XLIB_DELETE(x,y) \
    _delete_stat(x, y) 

#define _draw(x, y, image) \
    __DRAW(X_OFFSET+(x),Y_OFFSET+(y),(image))
    


#define _delete_stat(x, y) \
    __DELETE(X_OFFSET+(x),(y))

#define _delete(x, y) \
    __DELETE(X_OFFSET+(x),Y_OFFSET+(y))

#if defined(BUFFERED)
    extern uint8_t video_buffer[YSize+Y_OFFSET][XSize];
#endif

#if defined(Z88DK_SPRITES)
    extern uint8_t sprites[];
#endif

#if defined(Z88DK_SPRITES) && defined(ALT_PRINT)
    void _draw_ch(uint8_t x, uint8_t y, uint8_t ch);
#endif

#if defined(__LASER500__) && defined(LASER500_BITMAP)
    void laser500_gr4_draw(unsigned char x, unsigned char y, unsigned char *image, unsigned char color);
#endif



#if defined(USE_WAIT_V_SYNC)
    #if defined(__VIC20__) || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) || \
        defined(__APPLE2ENH__) || defined(__PCE__) || defined(__NES__) || \
        defined(__GAMATE__) || defined(__CBM510__) || defined(__CX16__)
        #define WAIT_V_SYNC() waitvsync()
    #else
        #define WAIT_V_SYNC() 
    #endif 
#else
    #define WAIT_V_SYNC() 
#endif 

#if !defined(BUFFERED)
    #define REFRESH()
#else
    #include "buffered_graphics.h"
    
    #define _REFRESH() \
        do \
        { \
            putchar('\n'); \
            display_all(); \
        } while(0);
        
    #if !defined(__EMCC__)
        #define REFRESH() _REFRESH()
    #else
        #define REFRESH() \
            do \
            { \
                _REFRESH(); \
                fflush(stdout); \
            } while(0)
    #endif
#endif

    
#if !defined(NO_BLINKING)
    void _blink_draw(uint8_t x,uint8_t y,Image * image, uint8_t * blinkCounter);
#else
    #define _blink_draw(x,y,image,blinkCounter) _draw(x,y,image)
#endif


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
    #define PRINTD(x,y,length,val) _displayShort(val)
    void _displayShort(uint16_t val);
    uint16_t loc(uint8_t x, uint8_t y);
#elif defined(NO_PRINT)
    #define PRINT(x,y,str)
    #define PRINTD(x,y,length, val)
#elif defined(__ATARI__) && defined(ATARI_MODE1)
    void _GOTOXY(uint8_t x, uint8_t y);
    void PRINT(uint8_t x, uint8_t y, char * str);
    #define PRINTD(x,y,length,val) do {_GOTOXY(x,y); cprintf("%0" #length "u",val); } while(0);
#elif defined(__NCURSES__)
    void PRINT(uint8_t x, uint8_t y, char * str);
    #define PRINTD(x,y,length,val) \
    do { \
        move(y,x); \
        printw("%0" #length "u",val); \
        refresh(); \
    } while(0)
#elif defined(ALT_PRINT)
    void PRINT(uint8_t x, uint8_t y, char * str);
    void PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val);
#else
    #define PRINT(x,y,str) do {gotoxy(x+X_OFFSET,y); cprintf(str); } while(0);
    #define PRINTD(x,y,length,val) do {gotoxy(x+X_OFFSET,y); cprintf("%0" #length "u",val); } while(0);
#endif


// DRAW HORIZONTAL AND VERTICAL LINES
#if !defined(TINY_GAME) && !defined(NO_WALL)
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
    || defined(__COCO__) || defined(__DRAGON__) || defined(__MO5__) || defined(__TO7__)|| defined(__WINCMOC__) \
    || (defined(__SVI__) && defined(MSX_MODE0)) || defined(__MSX__) \
    || defined(__ATMOS__) \
    || defined(Z88DK_SPRITES)
    #define SET_TEXT_COLOR(c) 
#elif defined(__ATARI_LYNX__)
    #define SET_TEXT_COLOR(c) tgi_setcolor(c)
#elif defined(__NCURSES__)
    #if defined(__ATARI_ST__)
        #include <ncurses/curses.h>
    #else
        #include <ncurses.h>
    #endif
    #define SET_TEXT_COLOR(c) attron(COLOR_PAIR(c))
#elif defined(__CPC__)
    #define CPC_TEXT_WHITE 3
    #define CPC_TEXT_RED 4
    #define CPC_TEXT_BLACK 7
    #define CPC_TEXT_YELLOW 0

    #define SET_TEXT_COLOR(c) \
    if(c==COLOR_WHITE) \
    { \
        textcolor(CPC_TEXT_WHITE); \
    } \
    else if(c==COLOR_RED) \
    { \
        textcolor(CPC_TEXT_RED); \
    } \
    else \
    { \
        textcolor(CPC_TEXT_YELLOW); \
    }
     
#else
    #define SET_TEXT_COLOR(c) textcolor(c)
#endif

#if defined(Z88DK_SPRITES) || defined(__MO5__)||defined(__TO7__) 
	#define _Z88DK_SPRITE_OFFSET 0x20
// #elif defined(__GB__)
	// #define _Z88DK_SPRITE_OFFSET 0x40
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
#elif defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__CIDELSA__) || defined(__MICRO__)
    #include <devkit/video/vis_video.h>
    #include "memory_mapped_graphics.h"
    
    #define CLEAR_SCREEN() vidclr(BASE_ADDR,XSize*40)
#elif defined(__MO5__)||defined(__TO7__)
    #define CLEAR_SCREEN() PUTCH(12);
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

