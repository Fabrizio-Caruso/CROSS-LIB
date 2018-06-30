#ifndef _MEMORY_MAPPED_GRAPHICS
#define _MEMORY_MAPPED_GRAPHICS

#  if defined(__C64__) || defined(__C128__)
	#define BASE_ADDR 0xC000
	#define COLOR_ADDR 0xD800
#elif defined(__VIC20__)
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
#endif


#if !defined(NO_COLOR)
	#define _DRAW(x,y,image) \
	do \
	{ \
		POKE(loc(x,y), image->_imageData); \
		POKE((unsigned short) (COLOR_ADDR+x+(unsigned short)(y+Y_OFFSET)*XSize),image->_color); \
	} \
	while(0)

	#define _DRAW_VERTICAL_WALL(x,y) \
	do \
	{ \
		POKE(loc(x,y),_VERTICAL_BRICK); \
		POKE((unsigned short) (COLOR_ADDR+(x)+(unsigned short)((y)+Y_OFFSET)*XSize),COLOR_WHITE); \
	} while(0)
	
	#define _DRAW_HORIZONTAL_WALL(x,y) \
	do \
	{ \
		POKE(loc(x,y),_HORIZONTAL_BRICK); \
		POKE((unsigned short) (COLOR_ADDR+(x)+(unsigned short)((y)+Y_OFFSET)*XSize),COLOR_WHITE); \
	} while(0)
#else
	#define _DRAW(x,y,image) \
		POKE(loc(x,y), image->_imageData);

	#define _DRAW_VERTICAL_WALL(x,y) POKE(loc(x,y),_VERTICAL_BRICK)
	#define _DRAW_HORIZONTAL_WALL(x,y)  POKE(loc(x,y),_HORIZONTAL_BRICK)
#endif
#define _DELETE(x,y) POKE(loc(x,y), _SPACE)
#define _DRAW_BROKEN_WALL(x,y) POKE(loc(x,y),_BROKEN_WALL)


unsigned short loc(unsigned char x, char y);

#endif // _MEMORY_MAPPED_GRAPHICS



