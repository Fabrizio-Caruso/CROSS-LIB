#ifndef ___QUAD_MEMORY_MAPPED_GRAPHICS
#define ___QUAD_MEMORY_MAPPED_GRAPHICS

#include "standard_libs.h"

#include "display_macros.h"

#if defined(___QUAD_MEMORY_MAPPED_GRAPHICS)

#  if defined(__C64__) || defined(__C128__)
    #if defined(__DOUBLE_BUFFER)
        #define BASE_ADDR 0xB800
        #define COLOR_ADDR 0xBC00
        #define REAL_BASE_ADDR 0xC000
        #define REAL_COLOR_ADDR 0xD800
    #else
        #define BASE_ADDR 0xC000
        #define COLOR_ADDR 0xD800
    #endif
#elif defined(__MC10__)
	#define BASE_ADDR 0x4000   
#elif defined(__VIC20__) && defined(___QUAD_MEMORY_MAPPED_GRAPHICS) && (defined(__VIC20_EXP_8K) || defined(__VIC20_EXP_16K))
	#define BASE_ADDR 0x1000
	#define COLOR_ADDR 0x9400
#elif defined(__C16__)
	#define BASE_ADDR 0x0C00
	#define COLOR_ADDR 0x0800
#elif defined(__GAL__)
	#define BASE_ADDR 0x2800
#elif defined(__COCO__) || defined(__DRAGON__)
	#define BASE_ADDR 0x0400
#elif defined(__ATARI__) && defined(__ANTIC_MODE6_GRAPHICS)
	#define BASE_ADDR_VALUE (PEEK(88)+PEEK(89)*256)
#elif defined(__ATARI5200__)
	#define BASE_ADDR 0x3E20
#elif defined(__ATMOS__)
	#define BASE_ADDR 0xBB80
#elif defined(__VIC20__) && (defined(__VIC20_UNEXPANDED) || defined(__VIC20_EXP_3K))
	#define BASE_ADDR 0x1E00
	#define COLOR_ADDR 0x9600
#elif defined(__MSX__)
    #if !defined(USE_MSX_BIOS)
        #define BASE_ADDR 0x0000
    #else
        #define BASE_ADDR 0x1800
    #endif
#elif defined(__CREATIVISION__) && defined(___QUAD_MEMORY_MAPPED_GRAPHICS)
    #define BASE_ADDR 0x1000
#elif defined(__AQUARIUS__)
    // TODO: it could be 12288 with X_OFFSET 1 to avoid using the first character that also sets the border character
    #if defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
        #define BASE_ADDR (12288)
    #else
        #define BASE_ADDR (12288+40)
    #endif
	#define COLOR_ADDR (BASE_ADDR+1024)
#elif defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__CIDELSA__) || defined(__MICRO__)
	#define BASE_ADDR 0xF800
    #if defined(__TMC600__)
        #define COLOR_ADDR 0
    #endif
#elif defined(__PET__)
	#define BASE_ADDR 0x8000
#endif

#if defined(__MSX__)
	#define DISPLAY_POKE(addr,val) msx_vpoke(addr,val)
#elif defined(__CREATIVISION__)
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
        
#elif defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__CIDELSA__) || defined(__MICRO__)
    #include <devkit/video/vis_video.h>
    #define DISPLAY_POKE(addr,val) vidchar(addr,val)
#elif defined(__MC10__)
    void mc10_display_poke(uint16_t addr, uint8_t val);
    #define DISPLAY_POKE(addr,val) mc10_display_poke(addr,val)
#else
	#define DISPLAY_POKE(addr,val) (*(uint8_t*) (addr) = (val))
#endif

#if defined(__TMC600__)
    #include <devkit/video/vis_video.h>
    #define COLOR_POKE(addr,color) setcolor(addr,color)
#else
    #define COLOR_POKE(addr, color) \
    do \
    { \
        DISPLAY_POKE(addr, color); \
    } while(0)
        
#endif

/*
        COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+1)) +(uint16_t)(2*(y))*(XSize)),(color)); \
        DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+1)+(uint8_t)(2*(y))*((uint16_t) (XSize ))), (tile)); \
        COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+1)) +(uint16_t)(2*(y)+2)*(XSize)),(color)); \
        DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+1)+(uint8_t)(2*(y)+2)*((uint16_t) (XSize))), (tile)); \
        COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x))) +(uint16_t)(2*(y)+2)*(XSize)),(color)); \
        DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x))+(uint8_t)(2*(y)+2)*((uint16_t) (XSize))), (tile)); \
        */


#if defined(__AQUARIUS__) 
    #define HARD_Y_LIMIT (YSize-1)
#else
    #define HARD_Y_LIMIT (YSize+1)
#endif

#define SCREEN_XSIZE (XSize+X_OFFSET)

extern uint8_t quad_tile[26][4];

#if !defined(_XL_NO_COLOR)

    #define _XL_DRAW(x,y,tile,color) \
    do \
    { \
        COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+X_OFFSET)) +(uint16_t)((y))*(4*SCREEN_XSIZE)),(color)); \
        DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+X_OFFSET)+(uint8_t)((y))*((uint16_t) (4*SCREEN_XSIZE))), (quad_tile[tile][0])); \
        \
        COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+1+X_OFFSET)) +(uint16_t)((y))*(4*SCREEN_XSIZE)),(color)); \
        DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+1+X_OFFSET)+(uint8_t)((y))*((uint16_t) (4*SCREEN_XSIZE))), (quad_tile[tile][1])); \
        \
        if(y<HARD_Y_LIMIT) \
        { \
            COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+X_OFFSET)) +(uint16_t)(4*(y)+2)*(SCREEN_XSIZE)),(color)); \
            DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+X_OFFSET)+(uint8_t)(4*(y)+2)*((uint16_t) (SCREEN_XSIZE))), (quad_tile[tile][2])); \
            \
            COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+1+X_OFFSET)) +(uint16_t)(4*(y)+2)*(SCREEN_XSIZE)),(color)); \
            DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+1+X_OFFSET)+(uint8_t)(4*(y)+2)*((uint16_t) (SCREEN_XSIZE))), (quad_tile[tile][3])); \
        } \
    } \
    while(0)
    
    // #define DRAW_QUAD_CHAR(x,y,tile,color) \
    // do \
    // { \
        // COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+X_OFFSET)) +(uint16_t)((y))*(4*SCREEN_XSIZE)),(color)); \
        // DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+X_OFFSET)+(uint8_t)((y))*((uint16_t) (4*SCREEN_XSIZE))), tile); \
        // \
        // COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+1+X_OFFSET)) +(uint16_t)((y))*(4*SCREEN_XSIZE)), _XL_BLACK); \
        // DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+1+X_OFFSET)+(uint8_t)((y))*((uint16_t) (4*SCREEN_XSIZE))), _SPACE); \
        // \
        // if(y<HARD_Y_LIMIT) \
        // { \
            // COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+X_OFFSET)) +(uint16_t)(4*(y)+2)*(SCREEN_XSIZE)), _XL_BLACK); \
            // DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+X_OFFSET)+(uint8_t)((4*(y)+2))*((uint16_t) (SCREEN_XSIZE))), _SPACE); \
            // \
            // COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+1+X_OFFSET)) +(uint16_t)((4*(y))+2)*(SCREEN_XSIZE)), _XL_BLACK); \
            // DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+1+X_OFFSET)+(uint8_t)((4*(y)+2))*((uint16_t) (SCREEN_XSIZE))), _SPACE); \
        // } \
    // } \
    // while(0)
        
#else
    #define _XL_DRAW(x,y,tile,color) \
    do \
    { \
        uint8_t computed_loc = (((uint16_t) BASE_ADDR)+(2*(x))+(uint8_t)(2*(y))*((uint16_t) (SCREEN_XSIZE + X_OFFSET))); \
        \
        DISPLAY_POKE(computed_loc, (tile)); \
        DISPLAY_POKE(computed_loc+2, (tile)); \
        DISPLAY_POKE(computed_loc+4*SCREEN_XSIZE, (tile)); \
        DISPLAY_POKE(computed_loc+2+4*SCREEN_XSIZE, (tile)); \
    } \
    while(0)

#endif


#define _XL_DELETE(x,y) \
do \
{ \
    COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+X_OFFSET)) +(uint16_t)((y))*(4*SCREEN_XSIZE)), _XL_BLACK); \
    DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+X_OFFSET)+(uint8_t)((y))*((uint16_t) (4*SCREEN_XSIZE))), _SPACE); \
    \
    COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+1+X_OFFSET)) +(uint16_t)((y))*(4*SCREEN_XSIZE)), _XL_BLACK); \
    DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+1+X_OFFSET)+(uint8_t)((y))*((uint16_t) (4*SCREEN_XSIZE))), _SPACE); \
    \
    if(y<HARD_Y_LIMIT) \
    { \
        COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+X_OFFSET)) +(uint16_t)(4*(y)+2)*(SCREEN_XSIZE)), _XL_BLACK); \
        DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+X_OFFSET)+(uint8_t)((4*(y)+2))*((uint16_t) (SCREEN_XSIZE))), _SPACE); \
        \
        COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(2*(x)+1+X_OFFSET)) +(uint16_t)((4*(y))+2)*(SCREEN_XSIZE)), _XL_BLACK); \
        DISPLAY_POKE((((uint16_t) BASE_ADDR)+(2*(x)+1+X_OFFSET)+(uint8_t)((4*(y)+2))*((uint16_t) (SCREEN_XSIZE))), _SPACE); \
    } \
} \
while(0)


#endif

#endif // ___QUAD_MEMORY_MAPPED_GRAPHICS



