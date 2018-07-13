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
	
#elif defined(SUPERVISION)

	// struct redefine_struct
	// {
	   // unsigned char ascii;
	   // unsigned char bitmap[8];
	// } ;


	// struct redefine_struct redefine_map[] =
	// {
		// {_PLAYER_DOWN, { 24, 36, 24,102,153, 24, 36,102}},
		// {_PLAYER_UP, { 24, 60, 24,102,153, 24, 36,102}},
		// {_PLAYER_RIGHT, { 24, 52, 25,118,152, 24, 20, 20}},	
		// {_PLAYER_LEFT, { 24, 44,152,110, 25, 24, 40, 40}},
		// {_GHOST, {129,126,165,129,129,189,129,126}},
		// {_SKULL, { 60, 66,165,129, 90, 36, 36, 60}},
		// {_GUN, {  0,128,126,200,248,192,128,  0}},
		// {_POWERUP, {  0, 60, 54,223,231,122, 36, 24}},
		// {_BULLET, {  0,  0,  8, 56, 28, 16,  0,  0}},
		// {_BOMB, { 60, 66,165,153,153,165, 66, 60}},
		// {_VERTICAL_BRICK, { 24, 24, 24, 48, 24, 12, 24, 24}},
		// {_HORIZONTAL_BRICK, {  0,  0,  0,255,  0,  0,  0,  0}},	
		// #if defined(FULL_GAME)
			// {_RIGHT_HORIZONTAL_MISSILE, {  0,  0, 15,252,252, 15,  0,  0}},
			// {_LEFT_HORIZONTAL_MISSILE, {  0,  0,240, 63, 63,240,  0,  0}},	
			// {_ROCKET, { 24, 60, 60, 60,126, 90, 66, 66}},
			// {_INVINCIBILITY, { 24, 36, 24,  0,153,  0, 36,102}},
		// #endif
	// };


	// unsigned char byte2wordlsb(unsigned char oneByte)
	// {
		// unsigned lsb = 0;
		// unsigned char i;
		
		// for(i=0;i<4;++i)
		// {
			// lsb += oneByte&(1<<(i*2));
			// lsb += (oneByte&(1<<(i*2+1)));
		// }
		// return lsb;
	// }

	// unsigned char byte2wordmsb(unsigned char oneByte)
	// {
		// unsigned msb = 0;
		// unsigned char i;
		
		// for(i=4;i<8;++i)
		// {
			// msb += oneByte&(1<<(i*2));
			// msb += (oneByte&(1<<(i*2)+1));
		// }
		// return msb;
	// }


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
		_DRAW((X_OFFSET+x),(y),image);
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





