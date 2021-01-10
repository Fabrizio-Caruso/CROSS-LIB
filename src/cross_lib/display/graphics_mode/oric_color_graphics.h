#ifndef _MEMORY_MAPPED_GRAPHICS
#define _MEMORY_MAPPED_GRAPHICS

#include "standard_libs.h"

#define BASE_ADDR 0xBB80


#define DISPLAY_POKE(addr,val) (*(uint8_t*) (addr) = (val))


#define _XLIB_DRAW_TILE(x,y,tile,color) \
    DISPLAY_POKE((uint16_t) loc(x+X_OFFSET,y), (tile)+(color));

#define __DRAW(x,y,image) \
    DISPLAY_POKE((uint16_t) loc(x,y), ((image)->_imageData) + ((image)->_color));

#define __DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)


#if !defined(INLINE_LOC)
    uint16_t loc(uint8_t x, uint8_t y);
#else
    #include "cross_lib.h"
    #define loc(x,y) ((uint16_t) BASE_ADDR)+(x)+(uint8_t)(y)*((uint16_t) (XSize + X_OFFSET))
#endif



#endif // _MEMORY_MAPPED_GRAPHICS



