#include "display_macros.h"

#include "tiles.h"

#include "init_images.h"


Image VERTICAL_HEAD_IMAGE;  
Image HORIZONTAL_HEAD_IMAGE; 

Image BODY_IMAGE; 

Image EXTRA_POINTS_IMAGE; 

Image HORIZONTAL_BRICK_IMAGE;
Image VERTICAL_BRICK_IMAGE;

void INIT_IMAGES(void)
{
	// Set color data 
	#if !defined(NO_COLOR)

        VERTICAL_HEAD_IMAGE._color = _SNAKE_COLOR;
        HORIZONTAL_HEAD_IMAGE._color = _SNAKE_COLOR;

        BODY_IMAGE._color = _SNAKE_COLOR;

        EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
        
        VERTICAL_BRICK_IMAGE._color = _WALL_COLOR;
        HORIZONTAL_BRICK_IMAGE._color = _WALL_COLOR;
    
    
	#endif	

	// Set Image Data
    VERTICAL_HEAD_IMAGE._imageData = _TILE_0;
    HORIZONTAL_HEAD_IMAGE._imageData = _TILE_1;

    BODY_IMAGE._imageData = _TILE_4;

    EXTRA_POINTS_IMAGE._imageData = _DOLLAR_TILE;

    VERTICAL_BRICK_IMAGE._imageData = _TILE_12;
    HORIZONTAL_BRICK_IMAGE._imageData = _TILE_13;

    
}



