#include "../../display/display_macros.h"

#include "memory_mapped_graphics.h"

#if !defined(CC65)
	#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
	#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
	#define PEEK(addr)         (*(unsigned char*) (addr))
	#define PEEKW(addr) (*(unsigned*) (addr))
#endif

unsigned short loc(unsigned char x, char y)
{
	return ((unsigned short) BASE_ADDR)+(x+X_OFFSET)+(unsigned char)(y+Y_OFFSET)*((unsigned short)XSize);
}


extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
#if !defined(NO_DEAD_GHOSTS)
extern Image DEAD_GHOST_IMAGE;
#endif
extern Image BOMB_IMAGE;

#if !defined(TINY_GAME)
	extern Image SKULL_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image GUN_IMAGE;
	extern Image BULLET_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
#endif

#if defined(FULL_GAME)
	extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
	extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

	extern Image ROCKET_IMAGE;

	extern Image FREEZE_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image SUPER_IMAGE;	
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif


#if defined(REDEFINED_CHARS)
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
#endif


void INIT_IMAGES(void)
{		
	#if !defined(NO_COLOR)
		BOMB_IMAGE._color = COLOR_RED;
		#if !defined(NO_DEAD_GHOSTS)
			DEAD_GHOST_IMAGE._color = COLOR_RED;
		#endif
		GHOST_IMAGE._color = COLOR_WHITE;		
		
		#if !defined(TINY_GAME)
			SKULL_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_GREEN;
			GUN_IMAGE._color = COLOR_BLUE;
			EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
			BULLET_IMAGE._color = COLOR_BLUE;
		#endif
		
		#if defined(REDEFINED_CHARS)
			PLAYER_DOWN._color = COLOR_CYAN;
			PLAYER_UP._color = COLOR_CYAN;
			PLAYER_RIGHT._color = COLOR_CYAN;
			PLAYER_LEFT._color = COLOR_CYAN;
		#else
			PLAYER_IMAGE._color = COLOR_CYAN;
		#endif
		
		#if defined(FULL_GAME)
			RIGHT_HORIZONTAL_MISSILE_IMAGE._color = COLOR_WHITE;		
			LEFT_HORIZONTAL_MISSILE_IMAGE._color = COLOR_WHITE;
			
			ROCKET_IMAGE._color = COLOR_WHITE;

			FREEZE_IMAGE._color = COLOR_CYAN;
			EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;
			
			INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
			SUPER_IMAGE._color = COLOR_RED;
			CONFUSE_IMAGE._color = COLOR_RED;
			ZOMBIE_IMAGE._color = COLOR_YELLOW;
		#endif	
	#endif

	GHOST_IMAGE._imageData = _GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	
	#if !defined(NO_DEAD_GHOSTS)
	DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;	
	#endif
	
	#if !defined(TINY_GAME)
		SKULL_IMAGE._imageData = _SKULL;

		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		BULLET_IMAGE._imageData = _BULLET;
	#endif
	
	#if defined(REDEFINED_CHARS)
		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;	
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		PLAYER_LEFT._imageData = _PLAYER_LEFT;	
	#else
		PLAYER_IMAGE._imageData = _PLAYER;			
	#endif
	
	#if defined(FULL_GAME)
		LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _LEFT_HORIZONTAL_MISSILE;
		RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _RIGHT_HORIZONTAL_MISSILE;
		
		ROCKET_IMAGE._imageData = _ROCKET;

		FREEZE_IMAGE._imageData = _POWERUP;		
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;	
		SUPER_IMAGE._imageData = _POWERUP;
		CONFUSE_IMAGE._imageData = _CONFUSE;
		ZOMBIE_IMAGE._imageData = _GHOST;
	#endif
}

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


#if !defined(CONIO_LIB)
void CLEAR_SCREEN(void)
{
	unsigned char i;
	unsigned char j;
	
	for(i=0;i<YSize;++i)
	{
		for(j=0;j<XSize;++j)
		{
			POKE(BASE_ADDR+j+i*((unsigned short ) XSize),_SPACE);
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

