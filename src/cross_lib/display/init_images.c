#include "display_macros.h"

#include "graphics_settings.h"

#if !defined(ANIMATE_PLAYER)
	extern Image PLAYER_IMAGE;
#else
	extern Image PLAYER_DOWN;
	extern Image PLAYER_UP;
	extern Image PLAYER_RIGHT;
	extern Image PLAYER_LEFT;	
#endif
	
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(NO_DEAD_GHOSTS)
	extern Image DEAD_GHOST_IMAGE;
#endif

#if !defined(TINY_GAME)
	extern Image SKULL_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image GUN_IMAGE;
	extern Image BULLET_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
	
	extern Image HORIZONTAL_BRICK_IMAGE;
	extern Image VERTICAL_BRICK_IMAGE;
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
	
	extern Image BROKEN_BRICK_IMAGE;
#endif

#if defined(COLOR_CYAN)
	#define COLOR_IF_EXISTS_CYAN_ELSE_BLUE COLOR_CYAN
#else
	#define COLOR_IF_EXISTS_CYAN_ELSE_BLUE COLOR_BLUE
#endif

void INIT_IMAGES(void)
{		
	#if !defined(NO_COLOR)
		
		#if defined(ANIMATE_PLAYER)
			PLAYER_DOWN._color = COLOR_IF_EXISTS_CYAN_ELSE_BLUE;
			PLAYER_UP._color = COLOR_IF_EXISTS_CYAN_ELSE_BLUE;
			PLAYER_RIGHT._color = COLOR_IF_EXISTS_CYAN_ELSE_BLUE;
			PLAYER_LEFT._color = COLOR_IF_EXISTS_CYAN_ELSE_BLUE;
		#else
			PLAYER_IMAGE._color = COLOR_IF_EXISTS_CYAN_ELSE_BLUE;
		#endif	
	
		BOMB_IMAGE._color = COLOR_RED;
		GHOST_IMAGE._color = COLOR_WHITE;		
		
		#if !defined(NO_DEAD_GHOSTS)
			DEAD_GHOST_IMAGE._color = COLOR_RED;
		#endif
		
		#if !defined(TINY_GAME)
			SKULL_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_GREEN;
			GUN_IMAGE._color = COLOR_BLUE;
			EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
			BULLET_IMAGE._color = COLOR_BLUE;
			
			VERTICAL_BRICK_IMAGE._color = COLOR_YELLOW;
			HORIZONTAL_BRICK_IMAGE._color = COLOR_YELLOW;			
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
			
			BROKEN_BRICK_IMAGE._color = COLOR_RED;
		#endif	
	#endif

	#if defined(ANIMATE_PLAYER)
		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;	
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		PLAYER_LEFT._imageData = _PLAYER_LEFT;	
	#else
		PLAYER_IMAGE._imageData = _PLAYER;			
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
		
		VERTICAL_BRICK_IMAGE._imageData = _VERTICAL_BRICK;
		HORIZONTAL_BRICK_IMAGE._imageData = _HORIZONTAL_BRICK;		
	#endif
	
	#if defined(FULL_GAME)
		LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _LEFT_HORIZONTAL_MISSILE;
		RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _RIGHT_HORIZONTAL_MISSILE;
		
		ROCKET_IMAGE._imageData = _ROCKET;

		#if !defined(NO_COLOR)
			FREEZE_IMAGE._imageData = _POWERUP;		
			SUPER_IMAGE._imageData = _POWERUP;		
		#else
			FREEZE_IMAGE._imageData = _FREEZE;
			SUPER_IMAGE._imageData = _SUPER;				
		#endif
		
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;	
		SUPER_IMAGE._imageData = _POWERUP;
		CONFUSE_IMAGE._imageData = _SKULL;
		ZOMBIE_IMAGE._imageData = _GHOST;
		
		BROKEN_BRICK_IMAGE._imageData = _BROKEN_BRICK;
	#endif
}

