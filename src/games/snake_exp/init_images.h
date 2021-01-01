#ifndef _INIT_IMAGES_H
#define _INIT_IMAGES_H

#include "tiles.h"

#if !defined(NO_COLOR)
    #define _SNAKE_COLOR COLOR_GREEN
    
    #define _WALL_COLOR COLOR_YELLOW
#endif

#if defined(NO_CONST_IMAGES)
    void INIT_IMAGES();
#else
    #define INIT_IMAGES()
#endif

#define VERTICAL_HEAD_TILE  _TILE_0
#define HORIZONTAL_HEAD_TILE _TILE_1
#define LEFT_MINE_TILE _TILE_2
#define RIGHT_MINE_TILE _TILE_3
#define BODY_TILE _TILE_4
#define APPLE_TILE _TILE_5
#define HI_TEXT_TILE _TILE_6
#define CENTRAL_BRICK_TILE _TILE_7
#define RING_TILE _TILE_8
#define LV_TEXT_TILE _TILE_9
#define TRANSPARENT_BRICK_TILE _TILE_10
#define UP_MINE_TILE _TILE_11
#define VERTICAL_BRICK_TILE _TILE_12
#define HORIZONTAL_BRICK_TILE _TILE_13
#define SCORE_TEXT_LEFT_TILE _TILE_14
#define DOWN_MINE_TILE _TILE_15
#define SCORE_TEXT_RIGHT_TILE _TILE_16
#define MINE_TILE _TILE_17
#define SUPER_RING_TILE _TILE_18
#define EXTRA_TILE _DOLLAR_TILE

    // const Image VERTICAL_HEAD_IMAGE = { _TILE_0, _SNAKE_COLOR };
    // const Image HORIZONTAL_HEAD_IMAGE = { _TILE_1, _SNAKE_COLOR };
    // const Image LEFT_MINE_IMAGE = {_TILE_2, COLOR_CYAN};
    // const Image RIGHT_MINE_IMAGE = {_TILE_3, COLOR_CYAN};
    // const Image BODY_IMAGE = { _TILE_4, _SNAKE_COLOR};
    // const Image APPLE_IMAGE = { _TILE_5, COLOR_RED };
    // const Image HI_TEXT_IMAGE = { _TILE_6, COLOR_RED };
    // const Image CENTRAL_BRICK_IMAGE = { _TILE_7, _WALL_COLOR };
    // const Image RING_IMAGE = { _TILE_8, COLOR_WHITE };
    // const Image LV_TEXT_IMAGE = { _TILE_9, COLOR_GREEN };
    // const Image TRANSPARENT_BRICK_IMAGE = { _TILE_10, _WALL_COLOR };
    // const Image UP_MINE_IMAGE = { _TILE_11, COLOR_CYAN };
    // const Image VERTICAL_BRICK_IMAGE = { _TILE_12, _WALL_COLOR };
    // const Image HORIZONTAL_BRICK_IMAGE = { _TILE_13, _WALL_COLOR };
    // const Image SCORE_TEXT_LEFT_IMAGE = { _TILE_14, COLOR_CYAN };
    // const Image DOWN_MINE_IMAGE = { _TILE_15, COLOR_CYAN };
    // const Image SCORE_TEXT_RIGHT_IMAGE = { _TILE_16, COLOR_CYAN };
    // const Image MINE_IMAGE = { _TILE_17, COLOR_CYAN };
    // const Image SUPER_RING_IMAGE = { _TILE_18, COLOR_CYAN };
    // const Image EXTRA_IMAGE = { _DOLLAR_TILE, COLOR_YELLOW };



#endif // _INIT_IMAGES_H

