#include "display_macros.h"

#include "tiles.h"

#include "init_images.h"

#if !defined(NO_COLOR)
    const Image VERTICAL_HEAD_IMAGE = { _TILE_0, _SNAKE_COLOR };
    const Image HORIZONTAL_HEAD_IMAGE = { _TILE_1, _SNAKE_COLOR };
    const Image LEFT_MINE_IMAGE = {_TILE_2, COLOR_CYAN};
    const Image RIGHT_MINE_IMAGE = {_TILE_3, COLOR_CYAN};
    const Image BODY_IMAGE = { _TILE_4, _SNAKE_COLOR};
    const Image APPLE_IMAGE = { _TILE_5, COLOR_RED };
    const Image HI_TEXT_IMAGE = { _TILE_6, COLOR_RED };
    const Image CENTRAL_BRICK_IMAGE = { _TILE_7, _WALL_COLOR };
    const Image COIN_IMAGE = { _TILE_8, COLOR_WHITE };
    const Image LV_TEXT_IMAGE = { _TILE_9, COLOR_GREEN };
    const Image TRANSPARENT_BRICK_IMAGE = { _TILE_10, _WALL_COLOR };
    const Image UP_MINE_IMAGE = { _TILE_11, COLOR_CYAN };
    const Image VERTICAL_BRICK_IMAGE = { _TILE_12, _WALL_COLOR };
    const Image HORIZONTAL_BRICK_IMAGE = { _TILE_13, _WALL_COLOR };
    const Image SCORE_TEXT_LEFT_IMAGE = { _TILE_14, COLOR_CYAN };
    const Image DOWN_MINE_IMAGE = { _TILE_15, COLOR_CYAN };
    const Image SCORE_TEXT_RIGHT_IMAGE = { _TILE_16, COLOR_CYAN };
    const Image MINE_IMAGE = { _TILE_17, COLOR_CYAN };
    const Image SUPER_COIN_IMAGE = { _TILE_18, COLOR_CYAN };
    const Image EXTRA_IMAGE = { _DOLLAR_TILE, COLOR_YELLOW };
#else
    const Image VERTICAL_HEAD_IMAGE = { _TILE_0 };
    const Image HORIZONTAL_HEAD_IMAGE = { _TILE_1 };
    const Image LEFT_MINE_IMAGE = {_TILE_2};
    const Image RIGHT_MINE_IMAGE = {_TILE_3};
    const Image BODY_IMAGE = { _TILE_4};
    const Image APPLE_IMAGE = { _TILE_5 };
    const Image HI_TEXT_IMAGE = { _TILE_6 };
    const Image CENTRAL_BRICK_IMAGE = { _TILE_7 };
    const Image COIN_IMAGE = { _TILE_8 };
    const Image LV_TEXT_IMAGE = { _TILE_9 };
    const Image TRANSPARENT_BRICK_IMAGE = { _TILE_10 };
    const Image UP_MINE_IMAGE = { _TILE_11 };
    const Image VERTICAL_BRICK_IMAGE = { _TILE_12 };
    const Image HORIZONTAL_BRICK_IMAGE = { _TILE_13 };
    const Image SCORE_TEXT_LEFT_IMAGE = { _TILE_14 };
    const Image DOWN_MINE_IMAGE = { _TILE_15 };
    const Image SCORE_TEXT_RIGHT_IMAGE = { _TILE_16 };
    const Image MINE_IMAGE = { _TILE_17 };
    const Image SUPER_COIN_IMAGE = { _TILE_18 };
    const Image EXTRA_IMAGE = { _DOLLAR_TILE };
#endif



