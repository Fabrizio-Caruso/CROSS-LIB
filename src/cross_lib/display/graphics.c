#include "display_macros.h"

extern Image BROKEN_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;
extern Image HORIZONTAL_BRICK_IMAGE;


#include "graphics_settings.h"



#if defined(MEMORY_MAPPED)	
	#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
	#define PEEK(addr)         (*(unsigned char*) (addr))
		
	unsigned short loc(unsigned char x, char y)
	{
		return ((unsigned short) BASE_ADDR)+x+(unsigned char)y*((unsigned short) (XSize + X_OFFSET));
	}
#elif defined(CPCRSLIB)
	char char_list[UDG_N*2] = 
	{ 
	33, '\0', // PLAYER
	37, '\0', // GHOST
	44, '\0', // BOMB
	40, '\0', // SKULL
	43, '\0', // BULLET
	42, '\0', // POWERUP
	41, '\0', // GUN
	49, '\0', // EXTRA_POINTS_IMAGE
	46, '\0', // INVINCIBILITY
	33, '\0', // EXTRA LIFE
	39, '\0', // LEFT_HORIZONTAL_MISSILE
	38, '\0', // RIGHT_HORIZONTAL_MISSILE
	45, '\0', // ROCKET
	33, '\0', // PLAYER_DOWN
	34, '\0', // PLAYER_UP
	35, '\0', // PLAYER_RIGHT
	36, '\0', // PLAYER_LEFT
	47, '\0', // VERTICAL_BRICK
	48, '\0' // HORIZONTAL_BRICK 
	};

	char space_str[2] = {' ', '\0'};

#elif defined(Z88DK_SPRITES)

	extern unsigned char sprites[];
	
#endif

#ifndef X_OFFSET
  COMPILATION ERROR
#endif
	
#ifndef Y_OFFSET
  COMPILATION ERROR
#endif


#if !defined(NO_STATS) \
	|| X_OFFSET!=0 || Y_OFFSET!=0

	void _draw_stat(unsigned char x, unsigned char y, Image * image) 
	{
		_DRAW(X_OFFSET+x,y,image);
	}

	void _draw(unsigned char x, unsigned char y, Image * image) 
	{
		_draw_stat(x,Y_OFFSET+y,image);
	}
#else
	void _draw(unsigned char x, unsigned char y, Image * image) 
	{
		_DRAW(x,y,image);
	}	
#endif

void _delete(unsigned char x, unsigned char y)
{
	_DELETE(X_OFFSET+x,Y_OFFSET+y);
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





