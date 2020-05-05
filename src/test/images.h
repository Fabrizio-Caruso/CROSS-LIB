#ifndef _IMAGES_H
#define _IMAGES_H


#if !defined(ANIMATE_PLAYER)
	Image PLAYER_IMAGE;
#else
	#if defined(USE_MEMORY_BUFFERS) || defined(USE_TINY_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
		extern Image PLAYER_DOWN;
		extern Image PLAYER_UP;
		extern Image PLAYER_RIGHT;
		extern Image PLAYER_LEFT;	
	#else
        // #if !defined(__LCC1802__)
            Image PLAYER_DOWN;
            Image PLAYER_UP;
            Image PLAYER_RIGHT;
            Image PLAYER_LEFT;
        // #endif
	#endif
#endif

#if defined(USE_MEMORY_BUFFERS) || defined(USE_TINY_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
	extern Image GHOST_IMAGE;
	extern Image BOMB_IMAGE;
#else
	Image GHOST_IMAGE;
	Image BOMB_IMAGE;	
#endif

#if !defined(NO_DEAD_GHOSTS)
	Image DEAD_GHOST_IMAGE;
#endif

#if !defined(TINY_GAME)
	#if defined(USE_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
		extern Image POWERUP_IMAGE;
		extern Image BULLET_IMAGE;
		extern Image GUN_IMAGE;	
		extern Image SKULL_IMAGE;		
		extern Image EXTRA_POINTS_IMAGE;	
		
		extern Image HORIZONTAL_BRICK_IMAGE;
		extern Image VERTICAL_BRICK_IMAGE;	
	#else
		Image POWERUP_IMAGE;
		Image BULLET_IMAGE;
		Image GUN_IMAGE;	
		Image SKULL_IMAGE;		
		Image EXTRA_POINTS_IMAGE;	
		
		Image HORIZONTAL_BRICK_IMAGE;
		Image VERTICAL_BRICK_IMAGE;	
	#endif
#endif

#if defined(FULL_GAME)
	#if defined(USE_MEMORY_BUFFERS)
	
		extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
		extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

		extern Image ROCKET_IMAGE;

		extern Image FREEZE_IMAGE;	
		
		extern Image EXTRA_LIFE_IMAGE;
		extern Image INVINCIBILITY_IMAGE;	
		
		extern Image CHASE_IMAGE;
		
		extern Image SUPER_IMAGE;
		extern Image CONFUSE_IMAGE;
		extern Image ZOMBIE_IMAGE;
		
		extern Image BROKEN_BRICK_IMAGE;

	#else	
		
		Image LEFT_HORIZONTAL_MISSILE_IMAGE;
		Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

		Image ROCKET_IMAGE;

		Image FREEZE_IMAGE;	
		
		Image EXTRA_LIFE_IMAGE;
		Image INVINCIBILITY_IMAGE;	
		
		Image CHASE_IMAGE;
		
		Image SUPER_IMAGE;
		Image CONFUSE_IMAGE;
		Image ZOMBIE_IMAGE;
		
		Image BROKEN_BRICK_IMAGE;
	#endif
#endif



#endif // _IMAGES_H

