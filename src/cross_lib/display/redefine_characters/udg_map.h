
#ifndef _UDG_MAP
#define _UDG_MAP

struct redefine_struct
{
   unsigned char ascii;
   unsigned char bitmap[8];
} ;


struct redefine_struct redefine_map[] =
{
	{_PLAYER_DOWN, _PLAYER_DOWN_UDG},
	{_PLAYER_UP, _PLAYER_UP_UDG},
	{_PLAYER_RIGHT, _PLAYER_RIGHT_UDG},	
	{_PLAYER_LEFT, _PLAYER_LEFT_UDG},
	{_GHOST, _GHOST_UDG},
	{_BOMB, _BOMB_UDG},
	
	#if defined(FULL_GAME)
	{_RIGHT_HORIZONTAL_MISSILE, _RIGHT_HORIZONTAL_MISSILE_UDG},
	{_LEFT_HORIZONTAL_MISSILE, _LEFT_HORIZONTAL_MISSILE_UDG},
	#endif
	
	#if !defined(TINY_GAME)
	{_SKULL, _SKULL_UDG},
	{_GUN, _GUN_UDG},
	{_POWERUP, _POWERUP_UDG},
	{_BULLET, _BULLET_UDG},
	{_ROCKET, _ROCKET_UDG},
	{_INVINCIBILITY, _INVINCIBILITY_UDG},
	{_VERTICAL_BRICK, _VERTICAL_BRICK_UDG},
	{_HORIZONTAL_BRICK, _HORIZONTAL_BRICK_UDG},		
	#endif	
	
	#if !defined(NO_DEAD_GHOST)
	{_DEAD_GHOST, _DEAD_GHOST_UDG},
	#endif
	
	{_EXTRA_POINTS, _EXTRA_POINTS_UDG}
};

#endif // _UDG_MAP
