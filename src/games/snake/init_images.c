#include "display_macros.h"

#include "tiles.h"

#include "init_images.h"

#if !defined(NO_CONST_IMAGES)

    #define _CONST const

    #if !defined(NO_COLOR)
        _CONST Image VERTICAL_HEAD_IMAGE = { _TILE_0, _SNAKE_COLOR };
        _CONST Image HORIZONTAL_HEAD_IMAGE = { _TILE_1, _SNAKE_COLOR };
        _CONST Image LEFT_MINE_IMAGE = {_TILE_2, COLOR_CYAN};
        _CONST Image RIGHT_MINE_IMAGE = {_TILE_3, COLOR_CYAN};
        _CONST Image BODY_IMAGE = { _TILE_4, _SNAKE_COLOR};
        _CONST Image APPLE_IMAGE = { _TILE_5, COLOR_RED };
        _CONST Image HI_TEXT_IMAGE = { _TILE_6, COLOR_RED };
        _CONST Image CENTRAL_BRICK_IMAGE = { _TILE_7, _WALL_COLOR };
        _CONST Image COIN_IMAGE = { _TILE_8, COLOR_WHITE };
        _CONST Image LV_TEXT_IMAGE = { _TILE_9, COLOR_GREEN };
        _CONST Image TRANSPARENT_BRICK_IMAGE = { _TILE_10, _WALL_COLOR };
        _CONST Image UP_MINE_IMAGE = { _TILE_11, COLOR_CYAN };
        _CONST Image VERTICAL_BRICK_IMAGE = { _TILE_12, _WALL_COLOR };
        _CONST Image HORIZONTAL_BRICK_IMAGE = { _TILE_13, _WALL_COLOR };
        _CONST Image SCORE_TEXT_LEFT_IMAGE = { _TILE_14, COLOR_CYAN };
        _CONST Image DOWN_MINE_IMAGE = { _TILE_15, COLOR_CYAN };
        _CONST Image SCORE_TEXT_RIGHT_IMAGE = { _TILE_16, COLOR_CYAN };
        _CONST Image MINE_IMAGE = { _TILE_17, COLOR_CYAN };
        _CONST Image SUPER_COIN_IMAGE = { _TILE_18, COLOR_CYAN };
        _CONST Image EXTRA_IMAGE = { _DOLLAR_TILE, COLOR_YELLOW };
    #else
        _CONST Image VERTICAL_HEAD_IMAGE = { _TILE_0 };
        _CONST Image HORIZONTAL_HEAD_IMAGE = { _TILE_1 };
        _CONST Image LEFT_MINE_IMAGE = {_TILE_2};
        _CONST Image RIGHT_MINE_IMAGE = {_TILE_3};
        _CONST Image BODY_IMAGE = { _TILE_4};
        _CONST Image APPLE_IMAGE = { _TILE_5 };
        _CONST Image HI_TEXT_IMAGE = { _TILE_6 };
        _CONST Image CENTRAL_BRICK_IMAGE = { _TILE_7 };
        _CONST Image COIN_IMAGE = { _TILE_8 };
        _CONST Image LV_TEXT_IMAGE = { _TILE_9 };
        _CONST Image TRANSPARENT_BRICK_IMAGE = { _TILE_10 };
        _CONST Image UP_MINE_IMAGE = { _TILE_11 };
        _CONST Image VERTICAL_BRICK_IMAGE = { _TILE_12 };
        _CONST Image HORIZONTAL_BRICK_IMAGE = { _TILE_13 };
        _CONST Image SCORE_TEXT_LEFT_IMAGE = { _TILE_14 };
        _CONST Image DOWN_MINE_IMAGE = { _TILE_15 };
        _CONST Image SCORE_TEXT_RIGHT_IMAGE = { _TILE_16 };
        _CONST Image MINE_IMAGE = { _TILE_17 };
        _CONST Image SUPER_COIN_IMAGE = { _TILE_18 };
        _CONST Image EXTRA_IMAGE = { _DOLLAR_TILE };
    #endif

#else


Image VERTICAL_HEAD_IMAGE;  
Image HORIZONTAL_HEAD_IMAGE; 
Image BODY_IMAGE; 
Image HORIZONTAL_BRICK_IMAGE;
Image VERTICAL_BRICK_IMAGE;
Image SCORE_TEXT_LEFT_IMAGE;
Image SCORE_TEXT_RIGHT_IMAGE;
Image HI_TEXT_IMAGE;
Image LV_TEXT_IMAGE;
Image APPLE_IMAGE;
Image CENTRAL_BRICK_IMAGE;
Image MINE_IMAGE;
Image LEFT_MINE_IMAGE;
Image RIGHT_MINE_IMAGE;
Image TRANSPARENT_BRICK_IMAGE;
Image COIN_IMAGE;
Image SUPER_COIN_IMAGE;
Image UP_MINE_IMAGE;
Image DOWN_MINE_IMAGE;
Image EXTRA_IMAGE; 


void INIT_IMAGES(void)
{
	// Set color data 
	#if !defined(NO_COLOR)

        VERTICAL_HEAD_IMAGE._color = _SNAKE_COLOR;
        HORIZONTAL_HEAD_IMAGE._color = _SNAKE_COLOR;

        BODY_IMAGE._color = _SNAKE_COLOR;

        
        VERTICAL_BRICK_IMAGE._color = _WALL_COLOR;
        HORIZONTAL_BRICK_IMAGE._color = _WALL_COLOR;
        CENTRAL_BRICK_IMAGE._color = _WALL_COLOR;
        
        SCORE_TEXT_LEFT_IMAGE._color = COLOR_CYAN;
        SCORE_TEXT_RIGHT_IMAGE._color = COLOR_CYAN;
        
        HI_TEXT_IMAGE._color = COLOR_RED;
        LV_TEXT_IMAGE._color = COLOR_GREEN;
        
        APPLE_IMAGE._color = COLOR_RED;
        
        MINE_IMAGE._color = COLOR_CYAN;
        LEFT_MINE_IMAGE._color = COLOR_CYAN;
        RIGHT_MINE_IMAGE._color = COLOR_CYAN;
        DOWN_MINE_IMAGE._color = COLOR_CYAN;
        UP_MINE_IMAGE._color = COLOR_CYAN;
        EXTRA_IMAGE._color = COLOR_YELLOW;
        COIN_IMAGE._color = COLOR_WHITE;
        SUPER_COIN_IMAGE._color = COLOR_CYAN;
        TRANSPARENT_BRICK_IMAGE._color = COLOR_YELLOW;

	#endif	

        // Set Image Data
        VERTICAL_HEAD_IMAGE._imageData = _TILE_0;
        HORIZONTAL_HEAD_IMAGE._imageData = _TILE_1;
        LEFT_MINE_IMAGE._imageData = _TILE_2;
        RIGHT_MINE_IMAGE._imageData = _TILE_3;
        BODY_IMAGE._imageData = _TILE_4;
        APPLE_IMAGE._imageData = _TILE_5;
        HI_TEXT_IMAGE._imageData = _TILE_6;
        CENTRAL_BRICK_IMAGE._imageData = _TILE_7;
        COIN_IMAGE._imageData = _TILE_8;
        LV_TEXT_IMAGE._imageData = _TILE_9;
        TRANSPARENT_BRICK_IMAGE._imageData = _TILE_10;
        UP_MINE_IMAGE._imageData = _TILE_11;
        VERTICAL_BRICK_IMAGE._imageData = _TILE_12;
        HORIZONTAL_BRICK_IMAGE._imageData = _TILE_13;
        SCORE_TEXT_LEFT_IMAGE._imageData = _TILE_14;
        DOWN_MINE_IMAGE._imageData = _TILE_15;
        SCORE_TEXT_RIGHT_IMAGE._imageData = _TILE_16;
        MINE_IMAGE._imageData = _TILE_17;
        SUPER_COIN_IMAGE._imageData = _TILE_18;
        EXTRA_IMAGE._imageData = _DOLLAR_TILE;

}





#endif



