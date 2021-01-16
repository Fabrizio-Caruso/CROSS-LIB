#ifndef _MEMORY_MAPPED_GRAPHICS
#define _MEMORY_MAPPED_GRAPHICS

#include "standard_libs.h"

#if defined(MEMORY_MAPPED)

#  if defined(__C64__) || defined(__C128__)
    #if defined(DOUBLE_BUFFER)
        #define BASE_ADDR 0xB800
        #define COLOR_ADDR 0xBC00
        #define REAL_BASE_ADDR 0xC000
        #define REAL_COLOR_ADDR 0xD800
    #else
        #define BASE_ADDR 0xC000
        #define COLOR_ADDR 0xD800
    #endif
#elif defined(__VIC20__) && defined(MEMORY_MAPPED) && (defined(VIC20_EXP_8K) || defined(VIC20_EXP_16K))
	#define BASE_ADDR 0x1000
	#define COLOR_ADDR 0x9400
#elif defined(__C16__)
	#define BASE_ADDR 0x0C00
	#define COLOR_ADDR 0x0800
#elif defined(__GAL__)
	#define BASE_ADDR 0x2800
#elif defined(__COCO__) || defined(__DRAGON__)
	#define BASE_ADDR 0x0400
#elif defined(__ATARI__) && defined(ATARI_MODE1)
	#define BASE_ADDR_VALUE (PEEK(88)+PEEK(89)*256)
#elif defined(__ATARI5200__)
	#define BASE_ADDR 0x3E20
#elif defined(__ATMOS__)
	#define BASE_ADDR 0xBB80
#elif defined(__VIC20__) && (defined(VIC20_UNEXPANDED) || defined(VIC20_EXP_3K))
	#define BASE_ADDR 0x1E00
	#define COLOR_ADDR 0x9600
#elif defined(__MSX__)
    #if !defined(USE_MSX_BIOS)
        #define BASE_ADDR 0x0000
    #else
        #define BASE_ADDR 0x1800
    #endif
#elif defined(__CREATIVISION__) && defined(MEMORY_MAPPED)
    #define BASE_ADDR 0x1000
#elif defined(__AQUARIUS__)
	#define BASE_ADDR (12288+40)
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
#else
	#define DISPLAY_POKE(addr,val) (*(uint8_t*) (addr) = (val))
#endif

#if defined(__TMC600__)
    #include <devkit/video/vis_video.h>
    #define COLOR_POKE(addr,val) setcolor(addr,val)
#else
    #define COLOR_POKE(addr, val) DISPLAY_POKE(addr,val)
#endif

#if !defined(NO_COLOR)

	#define _XL_DRAW(x,y,tile,color) \
	do \
	{ \
		COLOR_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(x)) +(uint16_t)(y)*(XSize)),(color)); \
		DISPLAY_POKE(loc(x,y), (tile)); \
	} \
	while(0)
#else
	#define _XL_DRAW(x,y,tile,color) \
		DISPLAY_POKE((uint16_t) loc(x+X_OFFSET,y), tile);

#endif


#define _XL_DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)


#if !defined(INLINE_LOC)
    uint16_t loc(uint8_t x, uint8_t y);
#else
    #include "cross_lib.h"
    #define loc(x,y) ((uint16_t) BASE_ADDR)+(x)+(uint8_t)(y)*((uint16_t) (XSize + X_OFFSET))
#endif

#endif

#endif // _MEMORY_MAPPED_GRAPHICS



