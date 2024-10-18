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

#if !defined(__NO_GRAPHICS)
    #if defined(__INCLUDE_CONIO_H)
        #include <conio.h>
    #elif defined(__CMOC__) && !defined(__WINCMOC__)
        #include <coco.h>
    #endif
#endif

#include "conio_patch.h"

#include "standard_libs.h"

#if !defined(_XL_NUMBER_OF_TILES)
    #define _XL_NUMBER_OF_TILES 26
#endif


#if defined(__BUFFERED_GRAPHICS)
    extern uint8_t video_buffer[YSize+Y_OFFSET][XSize];
#endif

#if defined(__Z88DK_SPRITES_GRAPHICS)
    extern uint8_t sprites[];
#endif

#if defined(__Z88DK_SPRITES_GRAPHICS) && defined(__ALT_PRINT)
    void __draw_ch(uint8_t x, uint8_t y, uint8_t ch);
#endif


#if defined(__VIC20__)
    #define SET_DEBUG_BORDER()     POKE(36879u, 12)
    #define UNSET_DEBUG_BORDER()   POKE(36879u,  8)
#elif defined(__C64__)
    #define SET_DEBUG_BORDER()     POKE(53280u,  1)
    #define UNSET_DEBUG_BORDER()   POKE(53280u,  0)
#else
    #define SET_DEBUG_BORDER()
    #define UNSET_DEBUG_BORDER()
#endif


#if defined(__USE_WAIT_V_SYNC)

    #if defined(__MSX__) || defined(__SVI__) || defined(__M5__) || defined(__MTX__)
        #define __VDP_WAIT_V_SYNC
    #endif

    #if defined(__VIC20__) || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) || \
        defined(__APPLE2ENH__) || defined(__PCE__) || defined(__NES__) || \
        defined(__GAMATE__) || defined(__CBM510__) || defined(__CX16__) || \
        defined(__ATARI__) || defined(__ATARI5200__)
        #define _XL_WAIT_VSYNC() waitvsync()
    #elif defined(__GB__)
        #include <gb/gb.h>
        #define _XL_WAIT_VSYNC() wait_vbl_done()
    #elif defined(__VDP_WAIT_V_SYNC)
        void vdp_waitvsync(void);
        #include <interrupt.h>
        #define _XL_WAIT_VSYNC() vdp_waitvsync()
    #else
        #define _XL_WAIT_VSYNC()
    #endif
#else
    #define _XL_WAIT_VSYNC()
#endif


#if !defined(__BUFFERED_GRAPHICS) && !defined(__DOUBLE_BUFFER)
    #define _XL_REFRESH()
#elif defined(__DOUBLE_BUFFER) && !defined(__BUFFERED_GRAPHICS)
    void _XL_REFRESH(void);
#else
    #include "buffered_graphics.h"

    #define __REFRESH() \
        do \
        { \
            putchar('\n'); \
            display_all(); \
        } while(0);

    #if !defined(__EMCC__)
        #define _XL_REFRESH() __REFRESH()
    #else
        #define _XL_REFRESH() \
            do \
            { \
                __REFRESH(); \
                fflush(stdout); \
            } while(0)
    #endif
#endif


// PRINT AND PRINTD
#  if defined(__ALT_DISPLAY_STATS)
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str);
    // void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val);
    void _displayShort(uint8_t x, uint16_t val);
    #define _XL_PRINTD(x,y,length,val) _displayShort(x,val)
    uint16_t loc(uint8_t x, uint8_t y);
    #define _XL_CHAR(x,y,ch)
#elif defined(NO_PRINT)
    #define _XL_PRINT(x,y,str)
    #define _XL_PRINTD(x,y,length, val)
    #define _XL_CHAR(x,y,ch)
#elif defined(__ATARI__) && defined(__ANTIC_MODE6_GRAPHICS)
    void _GOTOXY(uint8_t x, uint8_t y);
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str);
    void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val);
    void _XL_CHAR(uint8_t x, uint8_t y, char ch);
#elif defined(__NCURSES__)
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str);
    #define _XL_PRINTD(x,y,length,val) \
    do { \
        move(y,x); \
        printw("%0" #length "u",val); \
        refresh(); \
    } while(0)

    #define _XL_CHAR(x,y,ch) \
    do { \
        move(y,x); \
        cputc(ch); \
    } while(0)
#elif defined(__ALT_PRINT)
    void _XL_PRINT(uint8_t x, uint8_t y, const char * str);
    void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val);
    void _XL_CHAR(uint8_t x, uint8_t y, char ch);

#elif defined(__NO_BOTTOM)
    #define _XL_PRINT(x,y,str) \
    do \
    { \
		if(y<YSize-1) \
		{ \
			gotoxy(X_OFFSET+x,Y_OFFSET+y); \
			cprintf(str); \
		} \
    } while(0)

    #define _XL_PRINTD(x,y,length,val) \
    do \
    { \
		if(y<YSize-1) \
		{ \
			gotoxy(x+X_OFFSET,Y_OFFSET+y); \
			cprintf("%0" #length "u",val); \
		} \
    } while(0)

    #define _XL_CHAR(x,y,ch) \
    do \
    { \
		if(y<YSize-1) \
		{ \
			gotoxy(x+X_OFFSET,Y_OFFSET+y); \
			cputc(ch); \
		} \
    } while(0)

#else
    #define _XL_PRINT(x,y,str) \
    do \
    { \
        gotoxy(X_OFFSET+x,Y_OFFSET+y); \
        cprintf(str); \
    } while(0)

    #define _XL_PRINTD(x,y,length,val) \
    do \
    { \
        gotoxy(x+X_OFFSET,Y_OFFSET+y); \
        cprintf("%0" #length "u",val); \
    } while(0)

    #define _XL_CHAR(x,y,ch) \
    do \
    { \
        gotoxy(x+X_OFFSET,Y_OFFSET+y); \
        cputc(ch); \
    } while(0)
#endif

#if defined(__NO_SCREEN_COLOR_INIT) || defined(__NO_GRAPHICS)
    #define _setScreenColors()
#else
    #define _setScreenColors() \
    do \
    { \
        _SET_BORDER_COLOR(__BORDER_COLOR); \
        _SET_BACKGROUND_COLOR(_XL_BACKGROUND_COLOR); \
    } while(0)
#endif


// COLORS
#if defined(__ATARI5200__)
    #define _XL_SET_TEXT_COLOR(c) textcolor(c>>6)
#elif defined(__ATARI__) && (defined(__ANTIC_MODE6_GRAPHICS))
    extern uint8_t _atari_text_color;
    #define _XL_SET_TEXT_COLOR(c) _atari_text_color = (c)
#elif defined(__APPLE2__) && (defined(__APPLE2_HGR_GRAPHICS))
    extern uint8_t _apple2_text_color;
    #define _XL_SET_TEXT_COLOR(c) _apple2_text_color = (c)
#elif defined(__MO5__) || defined(__TO7__)
    void _XL_SET_TEXT_COLOR(uint8_t c);
#elif (defined(__COCO3__)&&defined(__BIT_MAPPED_16_GRAPHICS))
    extern uint8_t _bitmap16_text_color;
    #define _XL_SET_TEXT_COLOR(c) _bitmap16_text_color = (c)
#elif defined(__SUPERVISION__) || (defined(__COCO__)&&defined(__BIT_MAPPED_4_GRAPHICS))
    extern uint8_t _bitmap4_text_color;
    #define _XL_SET_TEXT_COLOR(c) _bitmap4_text_color = (c)
#elif defined(_XL_NO_TEXT_COLOR)
    #define _XL_SET_TEXT_COLOR(c)
#elif defined(__ATARI_LYNX__)
    #if defined(__LYNX_TGI_GRAPHICS)
        extern uint8_t _atari_lynx_text_color;
        #define _XL_SET_TEXT_COLOR(c) (_atari_lynx_text_color=(c))
    #else
        #define _XL_SET_TEXT_COLOR(c) tgi_setcolor(c)
    #endif
#elif defined(__NCURSES__)
    // #if defined(__ATARI_ST__)
        // #include <ncurses/curses.h>
    // #else
        #include <ncurses.h>
    // #endif
    #define _XL_SET_TEXT_COLOR(c) attron(COLOR_PAIR(c))
#elif defined(__CPC__) && defined(__CPCRSLIB_GRAPHICS)
    #define CPC_TEXT_WHITE 3
    #define CPC_TEXT_RED 4
    #define CPC_TEXT_BLACK 7
    #define CPC_TEXT_YELLOW 0

    #define _XL_SET_TEXT_COLOR(c) \
    if(c==_XL_WHITE) \
    { \
        textcolor(CPC_TEXT_WHITE); \
    } \
    else if(c==_XL_RED) \
    { \
        textcolor(CPC_TEXT_RED); \
    } \
    else \
    { \
        textcolor(CPC_TEXT_YELLOW); \
    }
#elif defined(__MC10__)
    #define _XL_SET_TEXT_COLOR(c)
#elif defined(__AQUARIUS__) && defined(__MEMORY_MAPPED_GRAPHICS)
    void _XL_SET_TEXT_COLOR(uint8_t c);
#elif defined(__COMX__) || defined(__PECOM__) || defined(__CIDELSA__) || defined(__MICRO__)
    #define _XL_SET_TEXT_COLOR(c) textcolor(_COMX_TEXT_WHITE)
    // #define _XL_SET_TEXT_COLOR(c)
#else
    #define _XL_SET_TEXT_COLOR(c) textcolor(c)
#endif


#if defined(__Z88DK_SPRITES_GRAPHICS) || defined(__MO5__)||defined(__TO7__) || defined(__COCO3__) || defined(__COCO__) || defined(__DRAGON__)
	#define _Z88DK_SPRITE_OFFSET (0x20)
#else

	#define _Z88DK_SPRITE_OFFSET 0x00
#endif

#if defined(__COCO3__) || defined(__COCO__) || defined(__DRAGON__)
    #define NOT_INVERTED 64
    #define INVERTED -64
#endif

#  if defined(__VIC20__) && (defined(__VIC20_EXP_8K) && !defined(__CONIO_GRAPHICS)) && !defined(_XL_NO_UDG)
    #define _SPACE 0x72
#elif defined(__VIC20__) && defined(__VIC20_UNEXPANDED) && !defined(_XL_NO_UDG)
    #define _SPACE 0xA0
#elif (defined(__ATARI__) && defined(__ANTIC_MODE6_GRAPHICS))
    #define _SPACE 0
#elif defined(__C16__) && !defined(_XL_NO_UDG) && defined(__MEMORY_MAPPED_GRAPHICS)
    #define _SPACE 0x60
	// If you use the kernal routine, you risk to get a corrupted space character
#elif defined(__Z88DK_SPRITES_GRAPHICS)
    #if defined(__FEWER_SPRITES)
        #define _SPACE 64
    #else
        #define _SPACE 96
    #endif
#elif defined(__COCO3__) || defined(__COCO__) || defined(__DRAGON__)
    #define _SPACE (' '+NOT_INVERTED)
#elif defined(__ATARI5200__) || defined(__ATARI7800_COLOR_GRAPHICS)
    #define _SPACE 0
#elif defined(__CPCRSLIB_GRAPHICS)
    #define _SPACE (19*2)
#else
    #define _SPACE ' '
#endif


// CLEAR SCREEN
#  if defined(__DEFAULT_CLEAR_SCREEN)
    void _XL_CLEAR_SCREEN(void);
#elif defined(__TI99__)
    #include <vdp.h>
    #define _XL_CLEAR_SCREEN() vdpmemset(gImage, 32, 768)
#elif defined(__ATARI_LYNX__)
    #define _XL_CLEAR_SCREEN() tgi_bar(0,0,159,101)
#elif defined(__CPC__) && defined(__CPCRSLIB_GRAPHICS)
    #define _XL_CLEAR_SCREEN() printf("\x1B[37;40m\x1B[2J")
#elif defined(__ATMOS__)
    #define _XL_CLEAR_SCREEN() do {clrscr(); _XL_INIT_GRAPHICS(); } while(0)
#elif defined(__Z88DK_SPRITES_GRAPHICS)
    #include <games.h>
    #include <graphics.h>
    #define _XL_CLEAR_SCREEN() clg()
#elif defined(__NCURSES__)
    // #if defined(__ATARI_ST__)
        // #include <ncurses/curses.h>
    // #else
        #include <ncurses.h>
    // #endif
    #if defined(_DEBUG_FLAG) && _DEBUG_FLAG==1
        #define _XL_CLEAR_SCREEN() \
            do \
            { \
                uint8_t i; \
                \
                clear(); \
                \
                for(i=0;i<YSize;++i) \
                { \
                    _XL_DRAW(XSize,i,_TILE_0,_XL_WHITE); \
                } \
                for(i=0;i<XSize;++i) \
                { \
                    _XL_DRAW(i,YSize,_TILE_0,_XL_WHITE); \
                } \
            } while(0)
    #else
        #define _XL_CLEAR_SCREEN() clear()
    #endif
#elif (defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__CIDELSA__) || defined(__MICRO__))
    #include <devkit/video/vis_video.h>
    #if defined(__DRACO__)
        #define REAL_XSIZE 27
    #else
        #define REAL_XSIZE 25
    #endif
    #define _XL_CLEAR_SCREEN() vidclr(BASE_ADDR,XSize*40)
#elif defined(__MO5__)||defined(__TO7__)
    #define _XL_CLEAR_SCREEN() PUTCH(12);

#elif defined(__ASSEMBLY_CLEAR_SCREEN)
    #if defined(__C16__)
        #define _XL_CLEAR_SCREEN() \
            __asm__("LDX #$04"); \
            __asm__("LDY #$0C"); \
            __asm__("LDA #$60"); \
            __asm__("JSR $C5A7");
    #endif
// KERNAL routine uses ASCII character 0x20 (32), whereas memory mapped sets space character at 0x60
#elif defined(__KERNAL_CLEAR_SCREEN) && !defined(__MEMORY_MAPPED_GRAPHICS)
    #if defined(__C16__)
        #define _XL_CLEAR_SCREEN() __asm__("jsr $D88B")
    #elif defined(__VIC20__)
        #define _XL_CLEAR_SCREEN() __asm__("jsr $E55F")
    #endif
#else // CONIO case
    #define _XL_CLEAR_SCREEN() clrscr()
#endif


// BORDER AND BACKGROUND COLORS
#if defined(__CC65__) && !defined(__NES_CONIO_GRAPHICS) && !defined(__ATARI5200__) && !defined(__SUPERVISION__) && defined(__CONIO_GRAPHICS)
    #define _SET_BORDER_COLOR(c) (void) bordercolor(c)
    #define _SET_BACKGROUND_COLOR(c) (void) bgcolor (c)
#elif (defined(__Z88DK__) && !defined(_XL_NO_UDG)) || defined(__AQUARIUS__)
    #define _SET_BORDER_COLOR(c) (void) bordercolor(c)
    #define _SET_BACKGROUND_COLOR(c) (void) textbackground(c)
#else
    #define _SET_BORDER_COLOR(c) {}
    #define _SET_BACKGROUND_COLOR(c) {}
#endif



#if !defined(__NO_INIT_GRAPHICS)
    void _XL_INIT_GRAPHICS(void);
#else
    #define _XL_INIT_GRAPHICS() _setScreenColors()
#endif

#endif // _DISPLAY_MACROS

