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

        THREE_WINDOW_WALL_1_IMAGE._color = _THREE_WINDOW_WALL_1_COLOR;
        PLANE_BACK_IMAGE._color = _PLANE_COLOR;
        PLANE_FRONT_IMAGE._color = _PLANE_COLOR;
        ANIMATED_PLANE_BACK_IMAGE._color = _PLANE_COLOR;
        ANIMATED_PLANE_CENTER_IMAGE._color = _PLANE_COLOR;
        ANIMATED_PLANE_FRONT_IMAGE._color = _PLANE_COLOR;

        WALL_1_IMAGE._color = _WALL_1_COLOR;
        SMALL_TWO_WINDOW_WALL_1_IMAGE._color = _SMALL_TWO_WINDOW_WALL_1_COLOR;		

        TWO_WINDOW_WALL_1_IMAGE._color = _TWO_WINDOW_WALL_1_COLOR;

        BOMB_IMAGE._color = _BOMB_COLOR;
        ANIMATED_BOMB_UP_IMAGE._color = _BOMB_COLOR;
        ANIMATED_BOMB_DOWN_IMAGE._color = _BOMB_COLOR;
        
        ROAD_IMAGE._color = _ROAD_COLOR;
        
        THREE_WINDOW_WALL_2_IMAGE._color = _THREE_WINDOW_WALL_2_COLOR;

        TWO_WINDOW_WALL_2_IMAGE._color = _TWO_WINDOW_WALL_2_COLOR;
        SMALL_TWO_WINDOW_WALL_2_IMAGE._color = _SMALL_TWO_WINDOW_WALL_2_COLOR;

        WALL_2_IMAGE._color = _WALL_2_COLOR;
        
        SCORE_TEXT_LEFT_IMAGE._color = COLOR_WHITE;
        SCORE_TEXT_RIGHT_IMAGE._color = COLOR_WHITE;
        
        HI_TEXT_IMAGE._color = COLOR_RED;;
        LV_TEXT_IMAGE._color = COLOR_YELLOW;
        
        EXPLOSION_IMAGE._color = COLOR_RED;
	#endif	

	// Set Image Data
    THREE_WINDOW_WALL_1_IMAGE._imageData = _TILE_15;
    
    PLANE_BACK_IMAGE._imageData = _TILE_1;
    PLANE_FRONT_IMAGE._imageData = _TILE_2;
    ANIMATED_PLANE_BACK_IMAGE._imageData = _TILE_0;
    ANIMATED_PLANE_CENTER_IMAGE._imageData = _TILE_3;
    ANIMATED_PLANE_FRONT_IMAGE._imageData = _TILE_17;

    SMALL_TWO_WINDOW_WALL_1_IMAGE._imageData = _TILE_4;
    WALL_1_IMAGE._imageData = _TILE_5;

    TWO_WINDOW_WALL_1_IMAGE._imageData = _TILE_7;

    BOMB_IMAGE._imageData = _TILE_10;
    ANIMATED_BOMB_UP_IMAGE._imageData = _TILE_12;
    ANIMATED_BOMB_DOWN_IMAGE._imageData = _TILE_13;

    ROAD_IMAGE._imageData = _TILE_8;

    THREE_WINDOW_WALL_2_IMAGE._imageData = _TILE_15;

    TWO_WINDOW_WALL_2_IMAGE._imageData = _TILE_7;
    SMALL_TWO_WINDOW_WALL_2_IMAGE._imageData = _TILE_4;

    WALL_2_IMAGE._imageData = _TILE_11;
    
    SCORE_TEXT_LEFT_IMAGE._imageData = _TILE_14;
    SCORE_TEXT_RIGHT_IMAGE._imageData = _TILE_16;
    
    HI_TEXT_IMAGE._imageData = _TILE_6;;
    LV_TEXT_IMAGE._imageData = _TILE_9;
    
    EXPLOSION_IMAGE._imageData = _TILE_18;
}



