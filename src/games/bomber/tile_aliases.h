#ifndef _IMAGES_H
#define _IMAGES_H

#include "cross_lib.h"


#if _XL_NUMBER_OF_TILES<=6
    #define ROAD_TILE _TILE_0

    #define PLANE_BACK_TILE _TILE_1
    #define PLANE_FRONT_TILE _TILE_2

    #define BOMB_TILE _TILE_3

    #define THREE_WINDOW_WALL_1_TILE _TILE_4
    #define SMALL_TWO_WINDOW_WALL_1_TILE _TILE_4
    #define WALL_1_TILE _TILE_4
    #define TWO_WINDOW_WALL_1_TILE _TILE_4
    #define THREE_WINDOW_WALL_2_TILE _TILE_4
    #define TWO_WINDOW_WALL_2_TILE _TILE_4
    #define SMALL_TWO_WINDOW_WALL_2_TILE _TILE_4
    #define WALL_2_TILE _TILE_4
    
    #define EXPLOSION_TILE _TILE_5

#else
    // Plane
    #define THREE_WINDOW_WALL_1_TILE _TILE_15
    #define PLANE_BACK_TILE _TILE_1
    #define PLANE_FRONT_TILE _TILE_2
    
    #define ANIMATED_PLANE_BACK_TILE _TILE_8
    #define ANIMATED_PLANE_CENTER_TILE _TILE_10
    #define ANIMATED_PLANE_FRONT_TILE _TILE_17
    

    // Bomb
    #define BOMB_TILE _TILE_3
    #define ANIMATED_BOMB_UP_TILE _TILE_12
    #define ANIMATED_BOMB_DOWN_TILE _TILE_13

    #define ROAD_TILE _TILE_0

    // WALL
    #define SMALL_TWO_WINDOW_WALL_1_TILE _TILE_4
    #define WALL_1_TILE _TILE_18
    #define TWO_WINDOW_WALL_1_TILE _TILE_7
    #define THREE_WINDOW_WALL_2_TILE _TILE_15
    #define TWO_WINDOW_WALL_2_TILE _TILE_7
    #define SMALL_TWO_WINDOW_WALL_2_TILE _TILE_4
    #define WALL_2_TILE _TILE_11

    // Score/Hiscore/Level
    #define SCORE_TEXT_LEFT_TILE _TILE_14
    #define SCORE_TEXT_RIGHT_TILE _TILE_16
    
    #define HI_TEXT_TILE _TILE_6
    #define LV_TEXT_TILE _TILE_9
    
    //
    #define EXPLOSION_TILE _TILE_5

#endif
    #define _PLANE_COLOR _XL_CYAN
    #define _SMALL_TWO_WINDOW_WALL_1_COLOR _XL_WHITE
    #define _TWO_WINDOW_WALL_1_COLOR _XL_YELLOW
    #define _WALL_1_COLOR _XL_RED
    #define _BOMB_COLOR _XL_WHITE
    #define _ROAD_COLOR _XL_CYAN
    #define _BRICK_COLOR _XL_GREEN
    #define _WALL_2_COLOR _XL_YELLOW
    #define _THREE_WINDOW_WALL_1_COLOR _XL_WHITE
    #define _THREE_WINDOW_WALL_2_COLOR _XL_WHITE
    #define _TWO_WINDOW_WALL_2_COLOR _XL_RED
    #define _SMALL_TWO_WINDOW_WALL_2_COLOR _XL_YELLOW



#endif // _IMAGES_H

