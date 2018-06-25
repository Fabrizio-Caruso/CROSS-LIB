#ifndef _MEMORY_MAPPED_GRAPHICS
#define _MEMORY_MAPPED_GRAPHICS


#  if defined(__VIC20__) && defined(ALT_PRINT)
	#include "../cc65/vic20/vic20_graphics_settings.h"
#elif defined(__C16__) && defined(ALT_PRINT)
	#include "../cc65/c264/c264_graphics_settings.h"
#elif defined(__C64__) && defined(ALT_PRINT)
	#include "../cc65/c64/c64_graphics_settings.h"
#else
#endif 


#if !defined(NO_COLOR)
	#define _DRAW(x,y,image) \
	do \
	{ \
		POKE(loc(x,y), image->_imageData); \
		POKE((unsigned short) (COLOR_ADDR+x+(unsigned short)(y+Y_OFFSET)*XSize),image->_color); \
	} \
	while(0)
	#define _DELETE(x,y) POKE(loc(x,y), 32)
#else
	#define _DRAW(x,y,image) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(image->_imageData); } while(0)
	#define _DELETE(x,y) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' '); } while(0)      
#endif
#define _DRAW_VERTICAL_WALL(x,y)  POKE(loc(x,y),'|')
#define _DRAW_HORIZONTAL_WALL(x,y)  POKE(loc(x,y),'-')
#define _DRAW_BROKEN_WALL(x,y) POKE(loc(x,y),_BROKEN_WALL)


unsigned short loc(unsigned char x, char y);

#endif // _MEMORY_MAPPED_GRAPHICS

