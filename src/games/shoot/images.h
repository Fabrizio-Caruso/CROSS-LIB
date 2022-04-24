#ifndef _IMAGES_H
#define _IMAGES_H

#include "settings.h"

#if !defined(ANIMATE_PLAYER)
	Image PLAYER_IMAGE;
#else
	#if defined(USE_MEMORY_BUFFERS) || defined(USE_TINY_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
		extern Image PLAYER_DOWN_IMAGE;
		extern Image PLAYER_UP_IMAGE;
		extern Image PLAYER_RIGHT_IMAGE;
		extern Image PLAYER_LEFT_IMAGE;	
	#else
        Image PLAYER_DOWN_IMAGE;
        Image PLAYER_UP_IMAGE;
        Image PLAYER_RIGHT_IMAGE;
        Image PLAYER_LEFT_IMAGE;
	#endif
#endif

#if defined(USE_MEMORY_BUFFERS) || defined(USE_TINY_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
	extern Image GHOST_IMAGE;
	extern Image BOMB_IMAGE;
#else
	Image GHOST_IMAGE;
	Image BOMB_IMAGE;	
#endif

Image DEAD_GHOST_IMAGE;


#if defined(USE_MEMORY_BUFFERS) || defined(USE_LIGHT_MEMORY_BUFFERS)
    Image CALM_DOWN_IMAGE;
    Image FIRE_CHARGE_IMAGE;
    extern Image BULLET_IMAGE;
    Image FIRE_POWER_IMAGE;	
    extern Image SKULL_IMAGE;		
    extern Image EXTRA_POINTS_IMAGE;	
    
    extern Image HORIZONTAL_BRICK_IMAGE;
    extern Image VERTICAL_BRICK_IMAGE;	
#else
    Image CALM_DOWN_IMAGE;
    Image FIRE_CHARGE_IMAGE;
    Image BULLET_IMAGE;
    Image FIRE_POWER_IMAGE;	
    Image SKULL_IMAGE;		
    Image EXTRA_POINTS_IMAGE;	
    
    Image HORIZONTAL_BRICK_IMAGE;
    Image VERTICAL_BRICK_IMAGE;	
#endif

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
    Image SUICIDE_IMAGE;
    
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
    Image SUICIDE_IMAGE;
    
    Image BROKEN_BRICK_IMAGE;
#endif



#endif // _IMAGES_H

