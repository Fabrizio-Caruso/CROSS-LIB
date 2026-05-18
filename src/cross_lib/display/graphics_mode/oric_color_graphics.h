#ifndef _MEMORY_MAPPED_GRAPHICS
#define _MEMORY_MAPPED_GRAPHICS

#include "standard_libs.h"

#define BASE_ADDR 0xBB80


#define DISPLAY_POKE(addr,val) (*(uint8_t*) (addr) = (val))

#if defined(__MONO_COLOR)
    #define _XL_DRAW(x,y,tile,color) \
        DISPLAY_POKE((uint16_t) loc(x+X_OFFSET,y), (tile));
#else
    #define _XL_DRAW(x,y,tile,color) \
            DISPLAY_POKE((uint16_t) loc(x+X_OFFSET,y), (tile)+(color)); 
#endif

#define _XL_DELETE(x,y) DISPLAY_POKE(loc(x+X_OFFSET,y), _SPACE)


uint16_t loc(uint8_t x, uint8_t y);

#endif // _MEMORY_MAPPED_GRAPHICS



