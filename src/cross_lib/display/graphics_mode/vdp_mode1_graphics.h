#ifndef _VDP_MODE1_GRAPHICS
#define _VDP_MODE1_GRAPHICS


#include "standard_libs.h"

#define _CREAT_XSIZE 32
#define _CREAT_YSIZE 24

#if defined(__CREATIVISION__)
    #define BASE_ADDR 0x1000
#elif defined(__TI99__)
    #define BASE_ADDR gImage
#endif

#define VDP_DATA  0x3000
#define VDP_CONTROL 0x3001


   
    
#if defined(__CREATIVISION__)
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
#elif defined(__TI99__)
    
    #define DISPLAY_POKE(addr,val) \
    do { \
        vdpmemset(addr,val,1); \
    } while(0)
#endif

uint16_t loc(uint8_t x, uint8_t y);

#if defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
    #define X_MULT 2
    #define Y_MULT 2
#else
    #define X_MULT 1
    #define Y_MULT 1
#endif

// TODO: Isolate the bug when _XL_DRAW uses loc(x,y)
// loc(x,y) -> 
// no string displayed after _XL_DRAW in title screen of Snake
//
// locti99(x,y) ->
// games/stinger/main.o: In function `L300':
// (.text+0x1585): relocation truncated to fit: R_TMS9900_PC8 against `.text'
// make: *** [makefiles.common/targets/gcc4ti/Makefile_ti99:132: xchase.ea5.elf] Error 1

// #define locti99(x, y) ((uint16_t) BASE_ADDR)+(X_MULT)*x+(uint8_t)((Y_MULT)*y)*(uint16_t) ((XSize) + X_OFFSET)


// #define _XL_DRAW(x,y,__tile,__color) \
// do \
// { \
    // DISPLAY_POKE(locti99(x,y), (__tile)+(__color)); \
// } \
// while(0)

// #define _XL_DELETE(x,y) DISPLAY_POKE(locti99(x,y), _SPACE)


#define _XL_DRAW(x,y,__tile,__color) \
do \
{ \
    DISPLAY_POKE(loc(x,y), (__tile)+(__color)); \
} \
while(0)

#define _XL_DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)



#endif // _VDP_MODE1_GRAPHICS



