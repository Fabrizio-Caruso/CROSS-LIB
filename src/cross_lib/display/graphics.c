#include "display_macros.h"

extern Image BROKEN_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;
extern Image HORIZONTAL_BRICK_IMAGE;


#  if defined(MEMORY_MAPPED)
	#include "memory_mapped/memory_mapped_graphics.h"
#elif defined(CONIO)
	#include "conio/conio_graphics.h"
#elif defined(Z88DK_SPRITES)
	#include "z88dk/z88dk_sprites/z88dk_sprites_graphics.h"
#elif defined(TGI)
	#include "tgi/tgi_graphics.h"
#elif defined(CPCRSLIB)
	#include "z88dk/cpc/cpc_cpcrslib_graphics.h"
#endif

#include "graphics_settings.h"


#if defined(MEMORY_MAPPED)	
	#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
	#define PEEK(addr)         (*(unsigned char*) (addr))
		
	unsigned short loc(unsigned char x, char y)
	{
		return ((unsigned short) BASE_ADDR)+(x+X_OFFSET)+(unsigned char)(y+Y_OFFSET)*((unsigned short)XSize + X_OFFSET);
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


#if defined(FULL_GAME)
	void DRAW_BROKEN_BRICK(unsigned char x, unsigned char y)
	{
		_draw(x,y,&BROKEN_BRICK_IMAGE);		
	}
#endif


#if !defined(NO_BLINKING)
void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	if(*blinkCounter) 
	{
		_draw(x,y,image);
		*blinkCounter=0;
	} 
	else 
	{
		_delete(x,y);
		*blinkCounter=1;
	}	
}
#endif


#if defined(ALT_CLEAR_SCREEN)
	void CLEAR_SCREEN(void)
	{
		unsigned char i;
		unsigned char j;
		
		for(i=0;i<XSize;++i)
		{
			for(j=0;j<YSize;++j)
			{
				_delete(i,j);
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
		_draw(x+i,y,&HORIZONTAL_BRICK_IMAGE);
	}
}

void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
{
	unsigned char i;
	for(i=0;i<length;++i)
	{
		_draw(x,y+i,&VERTICAL_BRICK_IMAGE);
	}		
}
#endif





