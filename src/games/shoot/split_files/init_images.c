#include "display_macros.h"

#include "tiles.h"

#include "images.h"

#include "init_images.h"

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

extern Image DESTROYER_IMAGE;

extern Image BOSS_IMAGE;

extern Image SKULL_IMAGE;
extern Image FIRE_CHARGE_IMAGE;
extern Image FIRE_POWER_IMAGE;
extern Image BULLET_IMAGE;
extern Image EXTRA_POINTS_IMAGE;

extern Image HORIZONTAL_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;

extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

extern Image ROCKET_IMAGE;

extern Image FREEZE_IMAGE;
extern Image EXTRA_LIFE_IMAGE;
extern Image INVINCIBILITY_IMAGE;
extern Image SUPER_IMAGE;
extern Image CONFUSE_IMAGE;
extern Image SUICIDE_IMAGE;

extern Image BROKEN_BRICK_IMAGE;



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
	
        FAST_GHOST_IMAGE._color = _FAST_GHOST_COLOR;
		BOMB_IMAGE._color = _BOMB_COLOR;
		GHOST_IMAGE._color = _GHOST_COLOR;		
		
		BOSS_IMAGE._color = _DEAD_GHOST_COLOR;
		
        // SKULL_IMAGE._color = _SKULL_COLOR;
        CALM_DOWN_IMAGE._color = _CALM_DOWN_COLOR;
        FIRE_CHARGE_IMAGE._color = _FIRE_CHARGE_COLOR;
        FIRE_POWER_IMAGE._color = _FIRE_POWER_COLOR;
        EXTRA_POINTS_IMAGE._color = _EXTRA_POINTS_COLOR;
        BULLET_IMAGE._color = _BULLET_COLOR;
        
        // VERTICAL_BRICK_IMAGE._color = _BRICK_COLOR;
        // HORIZONTAL_BRICK_IMAGE._color = _BRICK_COLOR;
    
        RIGHT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;		
        LEFT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;
        
        #if !defined(NO_ROCKETS)
        ROCKET_IMAGE._color = _ROCKET_COLOR;
        #endif
        FREEZE_IMAGE._color = _FREEZE_COLOR;
        EXTRA_LIFE_IMAGE._color = _EXTRA_LIFE_COLOR;
        
        INVINCIBILITY_IMAGE._color = _INVINCIBILITY_COLOR;
        SUPER_IMAGE._color = _SUPER_COLOR;
        CONFUSE_IMAGE._color = _CONFUSE_COLOR;
        SUICIDE_IMAGE._color = _SUICIDE_COLOR;
        
        #if !defined(_XL_NO_COLOR) && defined(NO_BLINKING)
        BROKEN_BRICK_IMAGE._color = _BROKEN_BRICK_COLOR;
        #endif
        
        DESTROYER_IMAGE._color = _DESTROYER_COLOR;
	#endif	

	// Set Image Data
	#if defined(ANIMATE_PLAYER)
		PLAYER_DOWN_IMAGE._imageData = _PLAYER_DOWN_TILE;
		PLAYER_UP_IMAGE._imageData = _PLAYER_UP_TILE;
		PLAYER_RIGHT_IMAGE._imageData = _PLAYER_RIGHT_TILE;
		PLAYER_LEFT_IMAGE._imageData = _PLAYER_LEFT_TILE;	
	#else
		PLAYER_IMAGE._imageData = _PLAYER_DOWN_TILE;
	#endif
	
	GHOST_IMAGE._imageData = _GHOST_TILE;
	BOMB_IMAGE._imageData = _BOMB_TILE;
	

	BOSS_IMAGE._imageData = _BOSS_TILE;
	
    SKULL_IMAGE._imageData = _SKULL_TILE;

    CALM_DOWN_IMAGE._imageData = _CALM_DOWN_TILE;
    FIRE_CHARGE_IMAGE._imageData = _BULLET_TILE;
    FIRE_POWER_IMAGE._imageData = _FIRE_POWER_TILE;
    EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS_TILE;
    
    BULLET_IMAGE._imageData = _BULLET_TILE;
    
    VERTICAL_BRICK_IMAGE._imageData = _VERTICAL_BRICK_TILE;
    HORIZONTAL_BRICK_IMAGE._imageData = _HORIZONTAL_BRICK_TILE;

    LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _LEFT_HORIZONTAL_MISSILE_TILE;
    
    RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _RIGHT_HORIZONTAL_MISSILE_TILE;
    
    #if !defined(NO_ROCKETS)
    ROCKET_IMAGE._imageData = _ROCKET_TILE;
    #endif
    
    FREEZE_IMAGE._imageData = _FREEZE_TILE;
    SUPER_IMAGE._imageData = _SUPER_TILE;

    EXTRA_LIFE_IMAGE._imageData = _PLAYER_DOWN_TILE;
    INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY_TILE;
    
    CONFUSE_IMAGE._imageData = _SKULL_TILE;
    SUICIDE_IMAGE._imageData = _GHOST_TILE;
    
    #if !defined(_XL_NO_COLOR) && defined(NO_BLINKING)
    BROKEN_BRICK_IMAGE._imageData = _BOMB_TILE;
    #endif
    
    DESTROYER_IMAGE._imageData = _DESTROYER_TILE;
    FAST_GHOST_IMAGE._imageData = _FAST_GHOST_TILE;
#endif 
}



