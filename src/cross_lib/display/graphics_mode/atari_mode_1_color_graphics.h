#ifndef _ATARI_MODE1_COLOR_GRAPHICS_H
#define _ATARI_MODE1_COLOR_GRAPHICS_H

#include "standard_libs.h"


#if defined(__ATARI5200__)
    #define BASE_ADDR 0x3E20
#else
    #define BASE_ADDR_VALUE (PEEK(88)+PEEK(89)*256)
#endif

#define DISPLAY_POKE(addr,val) (*(uint8_t*) (addr) = (val))

#define _XL_DRAW(x,y, tile,color) \
    DISPLAY_POKE((uint16_t) loc(x,y), tile+color);


#define _XL_DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)

uint16_t loc(uint8_t x, uint8_t y);
  
#endif // _ATARI_MODE1_COLOR_GRAPHICS_H

