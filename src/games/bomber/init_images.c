#include "display_macros.h"

#include "tiles.h"

#include "images.h"

#include "init_images.h"


extern Image THREE_WINDOW_WALL_1_IMAGE;
extern Image PLANE_BACK_IMAGE;
extern Image PLANE_FRONT_IMAGE;
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

extern Image BOMB_IMAGE;

extern Image ROAD_IMAGE;
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

        THREE_WINDOW_WALL_1_IMAGE._color = _PLANE_COLOR;
        PLANE_BACK_IMAGE._color = _PLANE_COLOR;
        PLANE_FRONT_IMAGE._color = _PLANE_COLOR;
        CONTROL_TOWER_IMAGE._color = _PLANE_COLOR;

        WALL_1_IMAGE._color = _WALL_1_COLOR;
        SMALL_TWO_WINDOW_WALL_1_IMAGE._color = _SMALL_TWO_WINDOW_WALL_1_COLOR;		

        TWO_WINDOW_WALL_1_IMAGE._color = _TWO_WINDOW_WALL_1_COLOR;

        VERTICAL_BRICK_IMAGE._color = _BRICK_COLOR;
        HORIZONTAL_BRICK_IMAGE._color = _BRICK_COLOR;			

        BOMB_IMAGE._color = _BOMB_COLOR;

        ROAD_IMAGE._color = _ROAD_COLOR;
        THREE_WINDOW_WALL_2_IMAGE._color = _THREE_WINDOW_WALL_2_COLOR;

        TWO_WINDOW_WALL_2_IMAGE._color = _TWO_WINDOW_WALL_2_COLOR;
        SMALL_TWO_WINDOW_WALL_2_IMAGE._color = _SMALL_TWO_WINDOW_WALL_2_COLOR;

        WALL_2_IMAGE._color = _WALL_2_COLOR;
	#endif	

	// Set Image Data
    THREE_WINDOW_WALL_1_IMAGE._imageData = _TILE_0;
    PLANE_BACK_IMAGE._imageData = _TILE_1;
    PLANE_FRONT_IMAGE._imageData = _TILE_2;
    CONTROL_TOWER_IMAGE._imageData = _TILE_3;	


    SMALL_TWO_WINDOW_WALL_1_IMAGE._imageData = _TILE_4;
    WALL_1_IMAGE._imageData = _TILE_5;

    TWO_WINDOW_WALL_1_IMAGE._imageData = _TILE_7;

    VERTICAL_BRICK_IMAGE._imageData = _TILE_12;
    HORIZONTAL_BRICK_IMAGE._imageData = _TILE_13;

    BOMB_IMAGE._imageData = _TILE_16;

    ROAD_IMAGE._imageData = _TILE_17;

    THREE_WINDOW_WALL_2_IMAGE._imageData = _TILE_0_B;

    TWO_WINDOW_WALL_2_IMAGE._imageData = _TILE_7_B;
    SMALL_TWO_WINDOW_WALL_2_IMAGE._imageData = _TILE_4_B;

    WALL_2_IMAGE._imageData = _TILE_5_B;
}



