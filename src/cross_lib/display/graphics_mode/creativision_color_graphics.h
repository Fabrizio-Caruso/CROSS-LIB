#ifndef _MEMORY_MAPPED_GRAPHICS
#define _MEMORY_MAPPED_GRAPHICS

#include "standard_libs.h"


#define BASE_ADDR 0x1000

#define VDP_DATA  0x3000
#define VDP_CONTROL 0x3001
#include <peekpoke.h>
    
#define CHAR_BASE ((uint16_t) 0x0000)
#define COLOR_DEF ((uint16_t) 0x1800)   
#define DISPLAY_POKE(addr,val) \
do { \
    __asm__("sei"); \
    POKE(VDP_CONTROL,(uint8_t) (addr&0x00FF)); \
    POKE(VDP_CONTROL,(uint8_t) (addr>>8)|0x40); \
    POKE(VDP_DATA,val); \
    __asm__("cli"); \
} while(0)



#define _XL_DRAW(x,y,tile,color) \
do \
{ \
    DISPLAY_POKE(loc(x,y), (tile)+(color)); \
} \
while(0)

#define __DRAW(x,y,image) \
do \
{ \
    DISPLAY_POKE(loc(x,y), ((image)->_imageData)+((image)->_color)); \
} \
while(0)


#define _XL_DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)


#if !defined(INLINE_LOC)
    uint16_t loc(uint8_t x, uint8_t y);
#else
    #include "cross_lib.h"
    #define loc(x,y) ((uint16_t) BASE_ADDR)+(x)+(uint8_t)(y)*((uint16_t) (XSize + X_OFFSET))
#endif


#endif // _MEMORY_MAPPED_GRAPHICS



