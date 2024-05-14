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
#include <atari5200.h>

#include "display_macros.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "setcolor.h"

#include "graphics_settings.h"

#include "8x8_chars.h"

#include "udg_map.h"	


void set_udg(void)
{
	extern char _FONT_START__[];
	
	uint8_t *CHBASE = (uint8_t *)0xD409;

	memcpy(_FONT_START__, (void *)0xF800, 512);
	
	REDEFINE_AT(_FONT_START__);
	
	*CHBASE = ((int)_FONT_START__ >> 8);
}

#define SETCOLOR_LOW(reg, val) *((unsigned char *)12 + (reg)) = (val)


void _XL_INIT_GRAPHICS(void)
{
    // NTSC (A5200 is only NTSC)
    #if !defined(__ALTERNATE_COLORS)
        POKE(COLOR0,_gtia_mkcolor(HUE_REDORANGE,2));
        POKE(COLOR1,GTIA_COLOR_WHITE);
        POKE(COLOR2,GTIA_COLOR_CYAN);
        POKE(COLOR3,_gtia_mkcolor(HUE_GOLDORANGE  ,4));
        #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
            POKE(COLOR4,GTIA_COLOR_WHITE);
        #else
            POKE(COLOR4,GTIA_COLOR_BLACK);
        #endif
    #else
        SETCOLOR_LOW(0,_gtia_mkcolor(HUE_REDORANGE,2));
        SETCOLOR_LOW(1,GTIA_COLOR_WHITE);
        SETCOLOR_LOW(2,_gtia_mkcolor(HUE_GREEN,3));
        SETCOLOR_LOW(3,_gtia_mkcolor(HUE_GOLDORANGE  ,4));
        #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
            SETCOLOR_LOW(4,GTIA_COLOR_WHITE);
        #else
            SETCOLOR_LOW(4,GTIA_COLOR_BLACK);
        #endif
    #endif
    set_udg();

    _setScreenColors();
}


