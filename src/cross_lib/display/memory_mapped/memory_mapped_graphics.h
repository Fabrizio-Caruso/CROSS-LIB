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
#elif defined(__ATMOS__)
	#define BASE_ADDR 0xBB80
#elif defined(__VIC20__) && (defined(VIC20_UNEXPANDED) || defined(VIC20_EXP_3K))
	#define BASE_ADDR 0x1E00
	#define COLOR_ADDR 0x9600	
#elif defined(__MSX__)
	#define BASE_ADDR 0x1800
#endif

#if defined(MSX_VPOKE)
	#define _DRAW(x,y,image) \
		msx_vpoke(loc(x,y), image->_imageData);	
	
	#define _DELETE(x,y) msx_vpoke(loc(x,y), _SPACE)
#else
	#if !defined(NO_COLOR)
		#define _DRAW(x,y,image) \
		do \
		{ \
			POKE(loc(x,y), image->_imageData); \
			POKE((unsigned short) ((unsigned short) (COLOR_ADDR+x+X_OFFSET) +(unsigned short)(y+Y_OFFSET)*(XSize+X_OFFSET)),image->_color); \
		} \
		while(0)

	#else
		#define _DRAW(x,y,image) \
			POKE(loc(x,y), image->_imageData);

	#endif

	#define _DELETE(x,y) POKE(loc(x,y), _SPACE)

#endif
unsigned short loc(unsigned char x, char y);

#endif // _MEMORY_MAPPED_GRAPHICS



