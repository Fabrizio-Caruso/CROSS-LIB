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



#if !defined(INLINE_LOC)
    uint16_t loc(uint8_t x, uint8_t y);
#else
    #include "cross_lib.h"
    #define loc(x,y) ((uint16_t) BASE_ADDR)+(x)+(uint8_t)(y)*((uint16_t) (_CREAT_XSIZE + X_OFFSET))
#endif



#define _XL_DRAW(x,y,__tile,__color) \
do \
{ \
    DISPLAY_POKE(loc(x,y), (__tile)+(__color)); \
} \
while(0)


#define _XL_DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)




/*
#if defined(__CREATIVISION__)
    #define BASE_ADDR 0x1000
#elif defined(__TI99__)
    #define BASE_ADDR gImage
#endif

#define _VDP_MODE1_XSIZE 32
#define _VDP_MODE1_YSIZE 24

    
#if defined(__CREATIVISION__)
    #include <peekpoke.h>

    #define VDP_DATA  0x3000
    #define VDP_CONTROL 0x3001

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



#if !defined(INLINE_LOC)
    uint16_t loc(uint8_t x, uint8_t y);
#else
    #include "cross_lib.h"
    #define loc(x,y) ((uint16_t) BASE_ADDR)+(x)+(uint8_t)(y)*((uint16_t) (_VDP_MODE1_XSIZE + _X_OFFSET))
#endif



#define _XL_DRAW(x,y,tile,color) \
do \
{ \
    DISPLAY_POKE(loc(x,y), (tile)+(color)); \
} \
while(0)


#define _XL_DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)
*/

#endif // _VDP_MODE1_GRAPHICS



