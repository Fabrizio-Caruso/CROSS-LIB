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

#include <conio.h>
#include <peekpoke.h>
#include <atari.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "display_macros.h"

#include "graphics_settings.h"

#include "8x8_chars.h"

#define GRAPHICS_MODE_1 (1+16)

#include "udg_map.h"	

#if !defined(_XL_NO_UDG)
void set_udg(void)
{
	uint8_t *_CHBAS = (uint8_t *) 0x2F4;
	extern char _FONT_START__[];

	memcpy(_FONT_START__, (void *)0xE000, 512);
	
	/* modify your font at _FONT_START__, etc, then set the new font: */
	REDEFINE_AT(_FONT_START__);
	
	*_CHBAS = ((int)_FONT_START__ >> 8);  /* enable the new font */
}
#endif

uint16_t BASE_ADDR;

#if defined(__ANTIC_MODE6_GRAPHICS)
uint8_t _atari_text_color;
#endif

// ; Hue values

// HUE_GREY        = 0
// HUE_GOLD        = 1
// HUE_GOLDORANGE  = 2
// HUE_REDORANGE   = 3
// HUE_ORANGE      = 4
// HUE_MAGENTA     = 5
// HUE_PURPLE      = 6
// HUE_BLUE        = 7
// HUE_BLUE2       = 8
// HUE_CYAN        = 9
// HUE_BLUEGREEN   = 10
// HUE_BLUEGREEN2  = 11
// HUE_GREEN       = 12
// HUE_YELLOWGREEN = 13
// HUE_YELLOW      = 14
// HUE_YELLOWRED   = 15

// ; Color defines, similar to c64 colors (untested)

// GTIA_COLOR_BLACK      = (HUE_GREY << 4)
// GTIA_COLOR_WHITE      = (HUE_GREY << 4 | 7 << 1)
// GTIA_COLOR_RED        = (HUE_REDORANGE << 4 | 1 << 1)
// GTIA_COLOR_CYAN       = (HUE_CYAN << 4 | 3 << 1)
// GTIA_COLOR_VIOLET     = (HUE_PURPLE << 4 | 4 << 1)
// GTIA_COLOR_GREEN      = (HUE_GREEN << 4 | 2 << 1)
// GTIA_COLOR_BLUE       = (HUE_BLUE << 4 | 2 << 1)
// GTIA_COLOR_YELLOW     = (HUE_YELLOW << 4 | 7 << 1)
// GTIA_COLOR_ORANGE     = (HUE_ORANGE << 4 | 5 << 1)
// GTIA_COLOR_BROWN      = (HUE_YELLOW << 4 | 2 << 1)
// GTIA_COLOR_LIGHTRED   = (HUE_REDORANGE << 4 | 6 << 1)
// GTIA_COLOR_GRAY1      = (HUE_GREY << 4 | 2 << 1)
// GTIA_COLOR_GRAY2      = (HUE_GREY << 4 | 3 << 1)
// GTIA_COLOR_LIGHTGREEN = (HUE_GREEN << 4 | 6 << 1)
// GTIA_COLOR_LIGHTBLUE  = (HUE_BLUE << 4 | 6 << 1)
// GTIA_COLOR_GRAY3      = (HUE_GREY << 4 | 5 << 1)
    
// #define SETCOLOR_LOW(reg, val) *((unsigned char *)12 + (reg)) = (val)

#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
    #define _ATARI_BACKGROUND_COLOR GTIA_COLOR_WHITE
    #define _ATARI_EXTRA_COLOR GTIA_COLOR_BLACK
#else
    #define _ATARI_BACKGROUND_COLOR GTIA_COLOR_BLACK
    #define _ATARI_EXTRA_COLOR GTIA_COLOR_WHITE
#endif


void _XL_INIT_GRAPHICS(void)
{
	// Mode 12 with no last monochromatic lines (12+16)
	_graphics(GRAPHICS_MODE_1);
    
    #if !defined(__ALTERNATE_COLORS)
        if(get_tv())
        {
            // PAL Settings
            _setcolor_low(0, GTIA_COLOR_RED);
            _setcolor_low(1, _ATARI_EXTRA_COLOR);
            _setcolor_low(2, GTIA_COLOR_CYAN); 	
            _setcolor_low(3, GTIA_COLOR_BROWN);
            _setcolor_low(4, _ATARI_BACKGROUND_COLOR);
        }
        else
        {
            _setcolor_low(0,_gtia_mkcolor(HUE_REDORANGE,2));
            _setcolor_low(1,_ATARI_EXTRA_COLOR);
            _setcolor_low(2,GTIA_COLOR_CYAN);
            _setcolor_low(3,_gtia_mkcolor(HUE_GOLDORANGE  ,4));
            _setcolor_low(4, _ATARI_BACKGROUND_COLOR);
        }
    #else
        if(get_tv())
        {
            // PAL Settings
            _setcolor_low(0, GTIA_COLOR_RED);
            _setcolor_low(1, _ATARI_EXTRA_COLOR);
            _setcolor_low(2, GTIA_COLOR_GREEN); 
            _setcolor_low(3, GTIA_COLOR_BROWN);
            _setcolor_low(4, _ATARI_BACKGROUND_COLOR);

        }
        else
        {
            _setcolor_low(0,_gtia_mkcolor(HUE_REDORANGE,2));
            _setcolor_low(1,_ATARI_EXTRA_COLOR);
            _setcolor_low(2,_gtia_mkcolor(HUE_GREEN,3));
            _setcolor_low(3,_gtia_mkcolor(HUE_GOLDORANGE  ,4));
            _setcolor_low(4, _ATARI_BACKGROUND_COLOR);
        }
    #endif
	#if !defined(_XL_NO_UDG)
	set_udg();
	#endif
    
    BASE_ADDR = BASE_ADDR_VALUE;

    _setScreenColors();
}


