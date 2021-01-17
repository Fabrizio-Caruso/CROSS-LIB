
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

void _XL_INIT_GRAPHICS(void)
{
    // Set monochromatic black/white ($F0 for green)
    POKE(0xFF22,0xF8);
    
    // Set resolution to 256x192
    POKE(0xFFC3,1);
    POKE(0xFFC5,1);
    
    // Set screen memory at $0E00
    POKE(0xFFC7,1);
    POKE(0xFFC9,1);
    POKE(0xFFCB,1);

    setScreenColors();
}
