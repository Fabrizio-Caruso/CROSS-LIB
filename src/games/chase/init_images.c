
#include "tiles.h"
#include "images.h"
#include "init_images.h"

#include "settings.h"

#include "cross_lib.h"


#if !defined(ANIMATE_PLAYER)
	extern Image PLAYER_IMAGE;
#else
	extern Image PLAYER_DOWN_IMAGE;
	extern Image PLAYER_UP_IMAGE;
	extern Image PLAYER_RIGHT_IMAGE;
	extern Image PLAYER_LEFT_IMAGE;	
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

void INIT_IMAGES(void)
{
# if !defined(__NO_GRAPHICS)
	// Set color data 
	#if !defined(_XL_NO_COLOR)
		#if defined(ANIMATE_PLAYER)
			PLAYER_DOWN_IMAGE._color = _PLAYER_COLOR;
			PLAYER_UP_IMAGE._color = _PLAYER_COLOR;
			PLAYER_RIGHT_IMAGE._color = _PLAYER_COLOR;
			PLAYER_LEFT_IMAGE._color = _PLAYER_COLOR;
		#else
			PLAYER_IMAGE._color = _PLAYER_COLOR;
		#endif	
	
		BOMB_IMAGE._color = _BOMB_COLOR;
		GHOST_IMAGE._color = _GHOST_COLOR;		
		
		#if !defined(NO_DEAD_GHOSTS)
			DEAD_GHOST_IMAGE._color = _DEAD_GHOST_COLOR;
		#endif
		
		#if !defined(TINY_GAME)
			SKULL_IMAGE._color = _SKULL_COLOR;
			POWERUP_IMAGE._color = _POWERUP_COLOR;
			GUN_IMAGE._color = _GUN_COLOR;
			EXTRA_POINTS_IMAGE._color = _EXTRA_POINTS_COLOR;
			BULLET_IMAGE._color = _BULLET_COLOR;
			
			VERTICAL_BRICK_IMAGE._color = _BRICK_COLOR;
			HORIZONTAL_BRICK_IMAGE._color = _BRICK_COLOR;
		#endif
		
		#if defined(FULL_GAME)
			RIGHT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;
			LEFT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;
			
			ROCKET_IMAGE._color = _ROCKET_COLOR;

			FREEZE_IMAGE._color = _FREEZE_COLOR;
			EXTRA_LIFE_IMAGE._color = _EXTRA_LIFE_COLOR;
			
			INVINCIBILITY_IMAGE._color = _INVINCIBILITY_COLOR;
			SUPER_IMAGE._color = _SUPER_COLOR;
			CONFUSE_IMAGE._color = _CONFUSE_COLOR;
			ZOMBIE_IMAGE._color = _ZOMBIE_COLOR;
			
			BROKEN_BRICK_IMAGE._color = _BRICK_COLOR;
		#endif	
	#endif	

	// Set Image Data
	#if defined(ANIMATE_PLAYER)
		PLAYER_DOWN_IMAGE._imageData = _PLAYER_DOWN_TILE; //_TILE_0;
		PLAYER_UP_IMAGE._imageData = _PLAYER_UP_TILE; // _TILE_1;
		PLAYER_RIGHT_IMAGE._imageData = _PLAYER_RIGHT_TILE; //_TILE_2;
		PLAYER_LEFT_IMAGE._imageData = _PLAYER_LEFT_TILE; // _TILE_3;	
	#else
		PLAYER_IMAGE._imageData = _PLAYER_DOWN_TILE;
	#endif
	
	GHOST_IMAGE._imageData = _GHOST_TILE;
	BOMB_IMAGE._imageData = _BOMB_TILE;
	
	#if !defined(NO_DEAD_GHOSTS)
		DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST_TILE;
	#endif
	
	#if !defined(TINY_GAME)
		SKULL_IMAGE._imageData = _SKULL_TILE;

		POWERUP_IMAGE._imageData = _POWERUP_TILE;
		GUN_IMAGE._imageData = _GUN_TILE;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS_TILE;
		
		BULLET_IMAGE._imageData = _BULLET_TILE;
		
		VERTICAL_BRICK_IMAGE._imageData = _VERTICAL_BRICK_TILE;
		HORIZONTAL_BRICK_IMAGE._imageData = _HORIZONTAL_BRICK_TILE;
	#endif
	
	#if defined(FULL_GAME)
		LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _LEFT_MISSILE_TILE;
		
		RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _RIGHT_MISSILE_TILE;
		
		ROCKET_IMAGE._imageData = _ROCKET_TILE;
		
        FREEZE_IMAGE._imageData = _FREEZE_TILE;
        SUPER_IMAGE._imageData = _SUPER_TILE;

		EXTRA_LIFE_IMAGE._imageData = _PLAYER_DOWN_TILE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY_TILE;
        
        CONFUSE_IMAGE._imageData = _SKULL_TILE;
        ZOMBIE_IMAGE._imageData = _GHOST_TILE;
		
		BROKEN_BRICK_IMAGE._imageData = _BOMB_TILE;
	#endif
#endif 
}



