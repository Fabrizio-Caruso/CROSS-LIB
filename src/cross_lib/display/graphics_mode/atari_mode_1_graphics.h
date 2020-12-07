#ifndef _ATARI_MODE1_GRAPHICS_H
#define _ATARI_MODE1_GRAPHICS_H

#include "standard_libs.h"


#define BASE_ADDR_VALUE (PEEK(88)+PEEK(89)*256)

#define DISPLAY_POKE(addr,val) (*(uint8_t*) (addr) = (val))

#define __DRAW(x,y,image) \
    DISPLAY_POKE((uint16_t) loc(x,y), ((image)->_imageData) + ((image)->_color)); //  + (image)->_color


#define __DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)

#if !defined(INLINE_LOC)
    uint16_t loc(uint8_t x, uint8_t y);
#else
    #include "cross_lib.h"
    extern uint16_t BASE_ADDR;

    #define loc(x,y) ((uint16_t) BASE_ADDR)+(x)+(uint8_t)(y)*((uint16_t) (XSize + X_OFFSET))
#endif
    
#endif // _ATARI_MODE1_GRAPHICS_H

