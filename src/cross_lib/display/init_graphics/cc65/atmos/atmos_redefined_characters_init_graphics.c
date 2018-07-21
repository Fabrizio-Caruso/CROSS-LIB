#include <peekpoke.h>

#include "../../../graphics_settings.h"

#include "../../../graphics_data/6x8_chars.h"

#define UDG_BASE_ADDR 0xb400

void redefine(unsigned long offset, const unsigned char * data)
{
	unsigned short i;
	for(i=0;i<8;++i)
	{
		POKE(UDG_BASE_ADDR+offset+i,data[i]);
	}
}

void init_colors(void)
{
	unsigned char i;
	
	// Initialize colors 	
	for(i=0;i<2;++i)
	{
		// red on black (inverted: cyan on white)		
		POKE(0xBB80+i*40,16);
		POKE(0xBB81+i*40,1); 
	}

	for(i=2;i<28;++i)
	{
		// yellow on black (inverted: blue on white)
		POKE(0xBB80+i*40,16);
		POKE(0xBB81+i*40,3); 
	}		
}

void INIT_GRAPHICS(void)
{		
	#if !defined(ANIMATE_PLAYER)	
		const unsigned char player[] =                _PLAYER_UDG;
	#else
		const unsigned char player_left[] =           _PLAYER_LEFT_UDG;
		const unsigned char player_right[] =          _PLAYER_RIGHT_UDG;		
		const unsigned char player_up[] =             _PLAYER_UP_UDG;
		const unsigned char player_down[] =           _PLAYER_DOWN_UDG;		
	#endif
	const unsigned char ghost[]  =                    _GHOST_UDG;
	const unsigned char bomb[]  =                     _BOMB_UDG;
	const unsigned char powerUp[]  =                  _POWERUP_UDG;
	const unsigned char skull[]  =                    _SKULL_UDG;
	const unsigned char gun[]  =                      _GUN_UDG;
	const unsigned char bullet[]  =                   _BULLET_UDG;
	
	#if !defined(NO_DEAD_GHOST)
		const unsigned char dead_ghost[]  =           _DEAD_GHOST_UDG;
	#endif

	const unsigned char vertical_brick[] =            _VERTICAL_BRICK_UDG;
	const unsigned char horizontal_brick[] =          _HORIZONTAL_BRICK_UDG;

	#if defined(FULL_GAME)
		const unsigned char right_arrow[] =           _RIGHT_HORIZONTAL_MISSILE_UDG;
		const unsigned char left_arrow[] =            _LEFT_HORIZONTAL_MISSILE_UDG;
		
		const unsigned char rocket[] =                _ROCKET_UDG;
		const unsigned char invincibility[] =         _INVINCIBILITY_UDG;		
	#endif
		
	#if defined(FULL_GAME)
		redefine(_INVINCIBILITY_OFFSET*8,invincibility);					
		redefine(_ROCKET_OFFSET*8,rocket);
		redefine(_RIGHT_HORIZONTAL_MISSILE_OFFSET*8,left_arrow);
		redefine(_LEFT_HORIZONTAL_MISSILE_OFFSET*8,right_arrow);
	#endif
	
	redefine(_GHOST_OFFSET*8, ghost);		
	redefine(_BOMB_OFFSET*8,bomb);
	redefine(_POWERUP_OFFSET*8,powerUp);		
	redefine(_SKULL_OFFSET*8,skull);
	redefine(_GUN_OFFSET*8,gun);		
	redefine(_BULLET_OFFSET*8,bullet);
	
	#if !defined(ANIMATE_PLAYER)
		redefine(_PLAYER_OFFSET*8,player);		
	#else	
		redefine(_PLAYER_LEFT_OFFSET*8,player_left);
		redefine(_PLAYER_RIGHT_OFFSET*8,player_right);
		redefine(_PLAYER_UP_OFFSET*8,player_up);		
		redefine(_PLAYER_DOWN_OFFSET*8,player_down);
	#endif
		
	#if !defined(TINY_GAME)
		redefine(_VERTICAL_BRICK_OFFSET*8,vertical_brick);
		redefine(_HORIZONTAL_BRICK_OFFSET*8,horizontal_brick);		
	#endif
	
	#if !defined(NO_DEAD_GHOST)
		redefine(_DEAD_GHOST_OFFSET*8, dead_ghost);
	#endif

	init_colors();
}
