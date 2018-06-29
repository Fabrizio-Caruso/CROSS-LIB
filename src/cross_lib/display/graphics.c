#include "display_macros.h"

#  if defined(MEMORY_MAPPED)
	#include "memory_mapped/memory_mapped_graphics.h"
#elif defined(CONIO)
	#include "conio/conio_graphics.h"
#endif

#include "graphics.h"


#if defined(MEMORY_MAPPED)	
	#if !defined(CC65)
		#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
		#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
		#define PEEK(addr)         (*(unsigned char*) (addr))
		#define PEEKW(addr) (*(unsigned*) (addr))
	#else
		#include <peekpoke.h>
	#endif

	unsigned short loc(unsigned char x, char y)
	{
		return ((unsigned short) BASE_ADDR)+(x+X_OFFSET)+(unsigned char)(y+Y_OFFSET)*((unsigned short)XSize);
	}
#endif


#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_DRAW_BROKEN_WALL(x,y);
	}
#endif
	
void _draw(unsigned char x, unsigned char y, Image * image) 
{
	_DRAW(x,y,image);
}

void _delete(unsigned char x, unsigned char y)
{
	_DELETE(x,y);
}

#if !defined(NO_BLINKING)
void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	if(*blinkCounter) 
	{
		_DRAW(x,y,image);
		*blinkCounter=0;
	} 
	else 
	{
		_DELETE(x,y);
		*blinkCounter=1;
	}	
}
#endif


#if defined(ALT_CLEAR_SCREEN)
	void CLEAR_SCREEN(void)
	{
		unsigned char i;
		unsigned char j;
		
		for(i=0;i<YSize;++i)
		{
			for(j=0;j<XSize;++j)
			{
				_DELETE(i,j);
			}
		}
	}
#endif


#if !defined(TINY_GAME)
void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
{
	unsigned char i;
	for(i=0;i<length;++i)
	{
		_DRAW_HORIZONTAL_WALL(x+i,y);
	}
}

void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
{
	unsigned char i;
	for(i=0;i<length;++i)
	{
		_DRAW_VERTICAL_WALL(x,y+i);
	}		
}
#endif

