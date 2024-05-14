
/*

// COCO 3
// Set 256x192x16 mode
POKE(0xFF99,0x1A);

// Set graphics ON
POKE(0xFF98,0x80);

// Set actual screen ram at $600000
POKE(0xFF9D,0xC0);
POKE(0xFF9E,0x00);

// Set palette
POKE(0xFFB0,...);
...
POKE(0xFFBF,...);

// Set MMU windows at $8000
POKE(0xFFA4,0x30);
POKE(0xFFA5,0x31);
POKE(0xFFA6,0x32);


*/

#include "cross_lib.h"

#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

#define _MONO_WHITE_ON_BLACK 0xF8
#define _MONO_GREEN_ON_DARK_GREEN 0xF0
#define _MULTI_COLOR_ON_WHITE (0xF8-0x10)
#define _MULTI_COLOR_ON_GREEN (0xF0-0x10)

void _XL_INIT_GRAPHICS(void)
{
    // Set monochromatic black/white ($F0 for green)
    // 0xF8    -> 2 colors (black and white)
    // 0XF0    -> 2 colors (dark green and light green)
    // 0xF8-16 -> 4 colors with white background
    // 0xF0-16 -> 4 colors with green background
    
    #if defined(__BIT_MAPPED_4_GRAPHICS)
        #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
            POKE(0xFF22,_MULTI_COLOR_ON_WHITE);
        #else
            POKE(0xFF22,_MULTI_COLOR_ON_GREEN);
        #endif
    #else
        POKE(0xFF22,_MONO_GREEN_ON_DARK_GREEN);
    #endif
    
    // Set resolution to 256x192
    POKE(0xFFC3,1);
    POKE(0xFFC5,1);
    
    // Set screen memory at $0E00
    POKE(0xFFC7,1);
    POKE(0xFFC9,1);
    POKE(0xFFCB,1);

    _setScreenColors();
}
