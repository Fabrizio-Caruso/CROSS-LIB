#include <peekpoke.h>

#include "../../../graphics_settings.h"

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
		const unsigned char player[] =                {12,18,12,51,45,12,18,51};
	#else
		const unsigned char player_left[] =           {12,22,12,62,13,12,20,20};
		const unsigned char player_right[] =          {12,26,12,31,44,12,10,10};		
		const unsigned char player_up[] =             {12,30,12,51,45,12,18,51};
		const unsigned char player_down[] =           {12,18,12,51,45,12,18,51};		
	#endif
	const unsigned char ghost[]  =                {33,30,33,51,33,45,33,30};
	const unsigned char bomb[]  =                 {30,33,51,45,45,51,33,30};
	const unsigned char powerUp[]  =              { 0,30,51,55,51,26,18,12};
	const unsigned char skull[]  =                {30,33,51,33,33,18,18,12};
	const unsigned char gun[]  =                  { 0,32,31,40,56,32, 0, 0};
	const unsigned char missile[]  =              { 0, 0, 4,28,14, 8, 0, 0};
	
	#if !defined(NO_DEAD_GHOST)
		const unsigned char dead_ghost[]  =           {18,30,33,51,33,45,51,30};
	#endif

	const unsigned char vertical_bar[] =             {12,12,12,12,12,12,12,12};
	const unsigned char horizontal_bar[] =           { 0, 0, 0,63,63,00,00,00};
	const unsigned char corner[] =                   {63,63,63,51,51,63,63,63};

	static const char vertical_brick[] =   { 24, 24, 24, 48, 24, 12, 24, 24};
	static const char horizontal_brick[] = {  0,  0,  0,255,  0,  0,  0,  0};		
	
	#if defined(FULL_GAME)
		const unsigned char right_arrow[] =          { 0, 0,56,31,31,56, 0, 0};
		const unsigned char left_arrow[] =           { 0, 0, 7,62,62, 7, 0, 0};
		
		const unsigned char bubble[] =               {12,30,30,30,63,45,33,33};
		const unsigned char invincibility[] =        {12,18,12,51, 0,12, 0,51};		
	#endif
		
	#if defined(FULL_GAME)
		redefine(_INVINCIBILITY_OFFSET*8,invincibility);					
		redefine(_ROCKET_OFFSET*8,bubble);
		redefine(_RIGHT_HORIZONTAL_MISSILE_OFFSET*8,left_arrow);
		redefine(_LEFT_HORIZONTAL_MISSILE_OFFSET*8,right_arrow);
	#endif
	
	redefine(_GHOST_OFFSET*8, ghost);		
	redefine(_BOMB_OFFSET*8,bomb);
	redefine(_POWERUP_OFFSET*8,powerUp);		
	redefine(_SKULL_OFFSET*8,skull);
	redefine(_GUN_OFFSET*8,gun);		
	redefine(_BULLET_OFFSET*8,missile);
	
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
