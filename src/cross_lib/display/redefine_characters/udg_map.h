
#ifndef _UDG_MAP
#define _UDG_MAP

struct redefine_struct
{
   unsigned char ascii;
   unsigned char bitmap[8];
} ;

#ifndef _PLAYER_DOWN_OFFSET
	#define _PLAYER_DOWN_OFFSET_ _PLAYER_DOWN
#else
	#define _PLAYER_DOWN_OFFSET_ _PLAYER_DOWN_OFFSET
#endif
#ifndef _PLAYER_UP_OFFSET
	#define _PLAYER_UP_OFFSET_ _PLAYER_UP
#else
	#define _PLAYER_UP_OFFSET_ _PLAYER_UP_OFFSET
#endif
#ifndef _PLAYER_LEFT_OFFSET
	#define _PLAYER_LEFT_OFFSET_ _PLAYER_LEFT
#else
	#define _PLAYER_LEFT_OFFSET_ _PLAYER_LEFT_OFFSET	
#endif
#ifndef _PLAYER_RIGHT_OFFSET
	#define _PLAYER_RIGHT_OFFSET_ _PLAYER_RIGHT
#else 
	#define _PLAYER_RIGHT_OFFSET_ _PLAYER_RIGHT_OFFSET	
#endif

#ifndef _GHOST_OFFSET
	#define _GHOST_OFFSET_ _GHOST
#else
	#define _GHOST_OFFSET_ _GHOST_OFFSET
#endif
#ifndef _BOMB_OFFSET
	#define _BOMB_OFFSET_ _BOMB
#else
	#define _BOMB_OFFSET_ _BOMB_OFFSET
#endif


#ifndef _DEAD_GHOST_OFFSET
	#define _DEAD_GHOST_OFFSET_ _DEAD_GHOST
#else
	#define _DEAD_GHOST_OFFSET_ _DEAD_GHOST_OFFSET
#endif


#ifndef _SKULL_OFFSET
	#define _SKULL_OFFSET_ _SKULL
#else
	#define _SKULL_OFFSET_ _SKULL_OFFSET
#endif
#ifndef _GUN_OFFSET
	#define _GUN_OFFSET_ _GUN
#else
	#define _GUN_OFFSET_ _GUN_OFFSET
#endif
#ifndef _POWERUP_OFFSET
	#define _POWERUP_OFFSET_ _POWERUP
#else
	#define _POWERUP_OFFSET_ _POWERUP_OFFSET
#endif
#ifndef _BULLET_OFFSET
	#define _BULLET_OFFSET_ _BULLET
#else
	#define _BULLET_OFFSET_ _BULLET_OFFSET
#endif
#ifndef _VERTICAL_BRICK_OFFSET 
	#define _VERTICAL_BRICK_OFFSET_ _VERTICAL_BRICK
#else
	#define _VERTICAL_BRICK_OFFSET_ _VERTICAL_BRICK_OFFSET
#endif
#ifndef _HORIZONTAL_BRICK_OFFSET
	#define _HORIZONTAL_BRICK_OFFSET_ _HORIZONTAL_BRICK
#else
	#define _HORIZONTAL_BRICK_OFFSET_ _HORIZONTAL_BRICK_OFFSET
#endif

#ifndef _ROCKET_OFFSET
	#define _ROCKET_OFFSET_ _ROCKET
#else
	#define _ROCKET_OFFSET_ _ROCKET_OFFSET
#endif
#ifndef _INVINCIBILITY_OFFSET
	#define _INVINCIBILITY_OFFSET_ _INVINCIBILITY
#else
	#define _INVINCIBILITY_OFFSET_ _INVINCIBILITY_OFFSET
#endif
#ifndef _RIGHT_HORIZONTAL_MISSILE_OFFSET
	#define _RIGHT_HORIZONTAL_MISSILE_OFFSET_ _RIGHT_HORIZONTAL_MISSILE
#else
	#define _RIGHT_HORIZONTAL_MISSILE_OFFSET_ _RIGHT_HORIZONTAL_MISSILE_OFFSET
#endif
#ifndef _LEFT_HORIZONTAL_MISSILE_OFFSET
	#define _LEFT_HORIZONTAL_MISSILE_OFFSET_ _LEFT_HORIZONTAL_MISSILE
#else
	#define _LEFT_HORIZONTAL_MISSILE_OFFSET_ _LEFT_HORIZONTAL_MISSILE_OFFSET
#endif

#ifndef _EXTRA_POINTS_OFFSET
	#define _EXTRA_POINTS_OFFSET_ _EXTRA_POINTS
#else
	#define _EXTRA_POINTS_OFFSET_ _EXTRA_POINTS_OFFSET
#endif

struct redefine_struct redefine_map[] =
{
	{_PLAYER_DOWN_OFFSET_, _PLAYER_DOWN_UDG},
	{_PLAYER_UP_OFFSET_, _PLAYER_UP_UDG},
	{_PLAYER_RIGHT_OFFSET_, _PLAYER_RIGHT_UDG},	
	{_PLAYER_LEFT_OFFSET_, _PLAYER_LEFT_UDG},
	
	{_GHOST_OFFSET_, _GHOST_UDG},
	{_BOMB_OFFSET_, _BOMB_UDG},
	
	#if !defined(NO_DEAD_GHOST)
		{_DEAD_GHOST_OFFSET_, _DEAD_GHOST_UDG},
	#endif
	
	#if !defined(TINY_GAME)
		{_SKULL_OFFSET_, _SKULL_UDG},
		{_GUN_OFFSET_, _GUN_UDG},
		{_POWERUP_OFFSET_, _POWERUP_UDG},
		{_BULLET_OFFSET_, _BULLET_UDG},
		{_VERTICAL_BRICK_OFFSET_, _VERTICAL_BRICK_UDG},
		{_HORIZONTAL_BRICK_OFFSET_, _HORIZONTAL_BRICK_UDG},		
	#endif	
	
	#if defined(FULL_GAME)
		{_ROCKET_OFFSET_, _ROCKET_UDG},
		{_INVINCIBILITY_OFFSET_, _INVINCIBILITY_UDG},	
		{_RIGHT_HORIZONTAL_MISSILE_OFFSET_, _RIGHT_HORIZONTAL_MISSILE_UDG},
		{_LEFT_HORIZONTAL_MISSILE_OFFSET_, _LEFT_HORIZONTAL_MISSILE_UDG},
	#endif

	#if defined(_EXTRA_POINTS)
		{_EXTRA_POINTS_OFFSET_, _EXTRA_POINTS_UDG}
	#endif
};

#if defined(__SPECTRUM__) && defined(CONIO)
	#define ASCII_OFFSET 32
#else
	#define ASCII_OFFSET 0
#endif

#define REDEFINE_AT(addr) \
{ \
	unsigned char i; \
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) \
	{ \
		memcpy((unsigned char *)(addr) + (redefine_map[i].ascii - ASCII_OFFSET)*8, redefine_map[i].bitmap, 8); \
	} \
}



#endif // _UDG_MAP

