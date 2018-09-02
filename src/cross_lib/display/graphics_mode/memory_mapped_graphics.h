#ifndef _MEMORY_MAPPED_GRAPHICS
#define _MEMORY_MAPPED_GRAPHICS

#  if defined(__C64__) || defined(__C128__)
	#define BASE_ADDR 0xC000
	#define COLOR_ADDR 0xD800
#elif defined(__VIC20__) && defined(MEMORY_MAPPED) && (defined(VIC20_EXP_8K) || defined(VIC20_EXP_16K))
	#define BASE_ADDR 0x1000
	#define COLOR_ADDR 0x9400
#elif defined(__C16__)
	#define BASE_ADDR 0x0C00
	#define COLOR_ADDR 0x0800
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
	#define BASE_ADDR 0x1800
#elif defined(__AQUARIUS__)
	#define BASE_ADDR (12288+40)
	#define COLOR_ADDR (BASE_ADDR+1024)
#endif

#if defined(__MSX__)
	#define DISPLAY_POKE(addr,val) msx_vpoke(addr,val)
#else
	#define DISPLAY_POKE(addr,val) (*(unsigned char*) (addr) = (val))
#endif


#if !defined(NO_COLOR)
	#define __DRAW(x,y,image) \
	do \
	{ \
		DISPLAY_POKE(loc(x,y), image->_imageData); \
		DISPLAY_POKE((unsigned short) ((unsigned short) (COLOR_ADDR+(x)) +(unsigned short)(y)*(XSize)),image->_color); \
	} \
	while(0)

#else
	#define __DRAW(x,y,image) \
		DISPLAY_POKE(loc(x,y), image->_imageData);

#endif

#define __DELETE(x,y) DISPLAY_POKE(loc(x,y), _SPACE)


unsigned short loc(unsigned char x, char y);

#endif // _MEMORY_MAPPED_GRAPHICS



