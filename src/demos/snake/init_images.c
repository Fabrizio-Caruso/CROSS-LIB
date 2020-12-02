#include "display_macros.h"

#include "tiles.h"

#include "images.h"

#include "init_images.h"


extern Image PLAYER_DOWN_IMAGE;
extern Image PLAYER_UP_IMAGE;
extern Image PLAYER_RIGHT_IMAGE;
extern Image PLAYER_LEFT_IMAGE;	

	
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

extern Image DEAD_GHOST_IMAGE;

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

extern Image EXTRA1_IMAGE;
extern Image EXTRA2_IMAGE;
extern Image EXTRA3_IMAGE;
extern Image EXTRA4_IMAGE;
extern Image EXTRA5_IMAGE;
extern Image EXTRA6_IMAGE;


void INIT_IMAGES(void)
{
	// Set color data 
	#if !defined(NO_COLOR)

        VERTICAL_HEAD_IMAGE._color = _SNAKE_COLOR;
        HORIZONTAL_HEAD_IMAGE._color = _SNAKE_COLOR;
        RIGHT_HEAD_IMAGE._color = _SNAKE_COLOR;
        LEFT_HEAD_IMAGE._color = _SNAKE_COLOR;

        BODY_IMAGE._color = _SNAKE_COLOR;
        BOTTOM_TAIL_IMAGE._color = _SNAKE_COLOR;		

        DEAD_GHOST_IMAGE._color = _SNAKE_COLOR;

        SKULL_IMAGE._color = _SNAKE_COLOR;
        CALM_DOWN_IMAGE._color = _SNAKE_COLOR;
        HORIZONTAL_JOINT_IMAGE._color = _SNAKE_COLOR;
        FIRE_POWER_IMAGE._color = _SNAKE_COLOR;
        EXTRA_POINTS_IMAGE._color = _SNAKE_COLOR;
        BULLET_IMAGE._color = _SNAKE_COLOR;

        VERTICAL_BRICK_IMAGE._color = _SNAKE_COLOR;
        HORIZONTAL_BRICK_IMAGE._color = _SNAKE_COLOR;

        LEFT_TAIL_IMAGE._color = _SNAKE_COLOR;
        RIGHT_TAIL_IMAGE._color = _SNAKE_COLOR;

        VERTICAL_JOINT_IMAGE._color = _SNAKE_COLOR;

        BOTTOM_HEAD_IMAGE._color = _SNAKE_COLOR;
        EXTRA_LIFE_IMAGE._color = _SNAKE_COLOR;

        INVINCIBILITY_IMAGE._color = _SNAKE_COLOR;
        SUPER_IMAGE._color = _SNAKE_COLOR;
        CONFUSE_IMAGE._color = _SNAKE_COLOR;
        SUICIDE_IMAGE._color = _SNAKE_COLOR;

        BROKEN_BRICK_IMAGE._color = _SNAKE_COLOR;
        
        TOP_HEAD_IMAGE._color = _SNAKE_COLOR;
        TOP_HEAD_JOINT_IMAGE._color = _SNAKE_COLOR;
        BOTTOM_HEAD_JOINT_IMAGE._color = _SNAKE_COLOR;
        LEFT_HEAD_JOINT_IMAGE._color = _SNAKE_COLOR;
        RIGHT_HEAD_JOINT_IMAGE._color = _SNAKE_COLOR;
        TOP_TAIL_IMAGE._color = _SNAKE_COLOR;
    
    
	#endif	

	// Set Image Data
    VERTICAL_HEAD_IMAGE._imageData = _TILE_0;
    HORIZONTAL_HEAD_IMAGE._imageData = _TILE_1;
    RIGHT_HEAD_IMAGE._imageData = _TILE_2;
    LEFT_HEAD_IMAGE._imageData = _TILE_3;	


    BODY_IMAGE._imageData = _TILE_4;
    BOTTOM_TAIL_IMAGE._imageData = _TILE_5;

    DEAD_GHOST_IMAGE._imageData = _TILE_6;

    SKULL_IMAGE._imageData = _TILE_7;

    CALM_DOWN_IMAGE._imageData = _TILE_8;
    HORIZONTAL_JOINT_IMAGE._imageData = _TILE_11;
    FIRE_POWER_IMAGE._imageData = _TILE_9;
    EXTRA_POINTS_IMAGE._imageData = _DOLLAR_TILE;

    BULLET_IMAGE._imageData = _TILE_11;

    VERTICAL_BRICK_IMAGE._imageData = _TILE_12;
    HORIZONTAL_BRICK_IMAGE._imageData = _TILE_13;


    LEFT_TAIL_IMAGE._imageData = _TILE_14;

    RIGHT_TAIL_IMAGE._imageData = _TILE_15;

    VERTICAL_JOINT_IMAGE._imageData = _TILE_16;

    BOTTOM_HEAD_IMAGE._imageData = _TILE_17;
    SUPER_IMAGE._imageData = _TILE_18;

    EXTRA_LIFE_IMAGE._imageData = _TILE_0_B;
    INVINCIBILITY_IMAGE._imageData = _TILE_10;


    CONFUSE_IMAGE._imageData = _TILE_7_B;
    SUICIDE_IMAGE._imageData = _TILE_4_B;

    BROKEN_BRICK_IMAGE._imageData = _TILE_5_B;
    
    TOP_HEAD_IMAGE._imageData = _TILE_19;
    TOP_HEAD_JOINT_IMAGE._imageData = _TILE_20;
    BOTTOM_HEAD_JOINT_IMAGE._imageData = _TILE_21;
    LEFT_HEAD_JOINT_IMAGE._imageData = _TILE_22;
    RIGHT_HEAD_JOINT_IMAGE._imageData = _TILE_23;
    TOP_TAIL_IMAGE._imageData = _TILE_24;
    
}



