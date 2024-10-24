#ifndef _ATARI_7800_GRAPHICS_H
#define _ATARI_7800_GRAPHICS_H

#include "standard_libs.h"

#include <atari7800.h>

extern char screen[];

#define BASE_ADDR 0x0000


uint16_t loc(uint8_t x, uint8_t y);

#define DISPLAY_POKE(addr,val) (screen[addr] = (val))

#define _XL_DRAW(x, y, tile, color) \
    DISPLAY_POKE((uint16_t) loc(x,y), (unsigned char) (tile)+((color)*54U));

// +(tile+color)


#define _XL_DELETE(x,y) DISPLAY_POKE(loc(x,y), 0)

  
#endif // _ATARI_7800_GRAPHICS_H


