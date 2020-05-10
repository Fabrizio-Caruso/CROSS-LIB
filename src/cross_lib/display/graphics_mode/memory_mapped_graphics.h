#ifndef _MEMORY_MAPPED_GRAPHICS
#define _MEMORY_MAPPED_GRAPHICS

#include "standard_libs.h"

#  if defined(__C64__) || defined(__C128__)
	#define BASE_ADDR 0xC000
	#define COLOR_ADDR 0xD800
#elif defined(__VIC20__) && defined(MEMORY_MAPPED) && (defined(VIC20_EXP_8K) || defined(VIC20_EXP_16K))
	#define BASE_ADDR 0x1000
	#define COLOR_ADDR 0x9400
#elif defined(__C16__)
	#define BASE_ADDR 0x0C00
	#define COLOR_ADDR 0x0800
#elif defined(__GAL__)
	#define BASE_ADDR 0x2800
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#define BASE_ADDR 0x0400
#elif defined(__ATARI__) && defined(ATARI_MODE1)
	#define BASE_ADDR 0xBD80
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
#elif defined(__COMX__) || defined(__PECOM__)
	#define BASE_ADDR 0xF800
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
        __asm__("sei"); \
        POKE(VDP_CONTROL,(uint8_t) (addr&0x00FF)); \
        POKE(VDP_CONTROL,(uint8_t) (addr>>8)|0x40); \
        POKE(VDP_DATA,val); \
        __asm__("cli");    
        
#elif defined(__COMX__) || defined(__PECOM__)
    void vidchar(int addr, int val);
    #define DISPLAY_POKE(addr,val) vidchar(addr,val)
#else
	#define DISPLAY_POKE(addr,val) (*(uint8_t*) (addr) = (val))
#endif


#if !defined(NO_COLOR)
	#define __DRAW(x,y,image) \
	do \
	{ \
		DISPLAY_POKE(loc(x,y), image->_imageData); \
		DISPLAY_POKE((uint16_t) ((uint16_t) (COLOR_ADDR+(x)) +(uint16_t)(y)*(XSize)),image->_color); \
	} \
	while(0)

#else
	#define __DRAW(x,y,image) \
		DISPLAY_POKE((uint16_t) loc(x,y), image->_imageData);

#endif

#define __DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)


uint16_t loc(uint8_t x, uint8_t y);

#endif // _MEMORY_MAPPED_GRAPHICS



