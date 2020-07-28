#include "display_macros.h"

#include "tiles.h"

#include "images.h"

#include "init_images.h"


extern Image THREE_WINDOW_WALL_1_IMAGE;
extern Image PLAYER_UP_IMAGE;
extern Image PLAYER_RIGHT_IMAGE;
extern Image PLAYER_LEFT_IMAGE;	

	
extern Image SMALL_TWO_WINDOW_WALL_1_IMAGE;
extern Image WALL_1_IMAGE;

extern Image DEAD_SMALL_TWO_WINDOW_WALL_1_IMAGE;

extern Image TWO_WINDOW_WALL_1_IMAGE;
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
extern Image THREE_WINDOW_WALL_2_IMAGE;
extern Image INVINCIBILITY_IMAGE;
extern Image SUPER_IMAGE;	
extern Image TWO_WINDOW_WALL_2_IMAGE;
extern Image SMALL_TWO_WINDOW_WALL_2_IMAGE;

extern Image WALL_2_IMAGE;


void INIT_IMAGES(void)
{
	// Set color data 
	#if !defined(NO_COLOR)

        THREE_WINDOW_WALL_1_IMAGE._color = _PLAYER_COLOR;
        PLAYER_UP_IMAGE._color = _PLAYER_COLOR;
        PLAYER_RIGHT_IMAGE._color = _PLAYER_COLOR;
        PLAYER_LEFT_IMAGE._color = _PLAYER_COLOR;


        WALL_1_IMAGE._color = _WALL_1_COLOR;
        SMALL_TWO_WINDOW_WALL_1_IMAGE._color = _SMALL_TWO_WINDOW_WALL_1_COLOR;		

        DEAD_SMALL_TWO_WINDOW_WALL_1_IMAGE._color = _DEAD_SMALL_TWO_WINDOW_WALL_1_COLOR;

        TWO_WINDOW_WALL_1_IMAGE._color = _TWO_WINDOW_WALL_1_COLOR;
        CALM_DOWN_IMAGE._color = _CALM_DOWN_COLOR;
        FIRE_CHARGE_IMAGE._color = _FIRE_CHARGE_COLOR;
        FIRE_POWER_IMAGE._color = _GUN_COLOR;
        EXTRA_POINTS_IMAGE._color = _EXTRA_POINTS_COLOR;
        BULLET_IMAGE._color = _BULLET_COLOR;

        VERTICAL_BRICK_IMAGE._color = _BRICK_COLOR;
        HORIZONTAL_BRICK_IMAGE._color = _BRICK_COLOR;			

        RIGHT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;		
        LEFT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;

        ROCKET_IMAGE._color = _ROCKET_COLOR;

        FREEZE_IMAGE._color = _FREEZE_COLOR;
        THREE_WINDOW_WALL_2_IMAGE._color = _THREE_WINDOW_WALL_2_COLOR;

        INVINCIBILITY_IMAGE._color = _INVINCIBILITY_COLOR;
        SUPER_IMAGE._color = _SUPER_COLOR;
        TWO_WINDOW_WALL_2_IMAGE._color = _TWO_WINDOW_WALL_2_COLOR;
        SMALL_TWO_WINDOW_WALL_2_IMAGE._color = _SMALL_TWO_WINDOW_WALL_2_COLOR;

        WALL_2_IMAGE._color = _WALL_2_COLOR;
	#endif	

	// Set Image Data
    THREE_WINDOW_WALL_1_IMAGE._imageData = _TILE_0;
    PLAYER_UP_IMAGE._imageData = _TILE_1;
    PLAYER_RIGHT_IMAGE._imageData = _TILE_2;
    PLAYER_LEFT_IMAGE._imageData = _TILE_3;	


    SMALL_TWO_WINDOW_WALL_1_IMAGE._imageData = _TILE_4;
    WALL_1_IMAGE._imageData = _TILE_5;

    DEAD_SMALL_TWO_WINDOW_WALL_1_IMAGE._imageData = _TILE_6;

    TWO_WINDOW_WALL_1_IMAGE._imageData = _TILE_7;

    CALM_DOWN_IMAGE._imageData = _TILE_8;
    FIRE_CHARGE_IMAGE._imageData = _TILE_11;
    FIRE_POWER_IMAGE._imageData = _TILE_9;
    EXTRA_POINTS_IMAGE._imageData = _DOLLAR_TILE;

    BULLET_IMAGE._imageData = _TILE_11;

    VERTICAL_BRICK_IMAGE._imageData = _TILE_12;
    HORIZONTAL_BRICK_IMAGE._imageData = _TILE_13;


    LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _TILE_14;

    RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _TILE_15;

    ROCKET_IMAGE._imageData = _TILE_16;

    FREEZE_IMAGE._imageData = _TILE_17;
    SUPER_IMAGE._imageData = _TILE_18;

    THREE_WINDOW_WALL_2_IMAGE._imageData = _TILE_0_B;
    INVINCIBILITY_IMAGE._imageData = _TILE_10;


    TWO_WINDOW_WALL_2_IMAGE._imageData = _TILE_7_B;
    SMALL_TWO_WINDOW_WALL_2_IMAGE._imageData = _TILE_4_B;

    WALL_2_IMAGE._imageData = _TILE_5_B;
}



