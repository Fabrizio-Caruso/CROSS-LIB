#include "display_macros.h"

#include "tiles.h"

#include "images.h"

#include "init_images.h"


Image VERTICAL_HEAD_IMAGE;  
Image HORIZONTAL_HEAD_IMAGE; 
Image RIGHT_HEAD_IMAGE; 
Image LEFT_HEAD_IMAGE; 	


Image BODY_IMAGE; 
Image BOTTOM_TAIL_IMAGE; 

Image HORIZONTAL_JOINT_IMAGE; 

Image EXTRA_POINTS_IMAGE; 

Image LEFT_TAIL_IMAGE; 

Image RIGHT_TAIL_IMAGE; 

Image VERTICAL_JOINT_IMAGE; 

Image BOTTOM_HEAD_IMAGE; 
    
Image TOP_HEAD_IMAGE; 
Image TOP_HEAD_JOINT_IMAGE; 
Image BOTTOM_HEAD_JOINT_IMAGE; 
Image LEFT_HEAD_JOINT_IMAGE; 
Image RIGHT_HEAD_JOINT_IMAGE; 
Image TOP_TAIL_IMAGE; 



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

        HORIZONTAL_JOINT_IMAGE._color = _SNAKE_COLOR;
        EXTRA_POINTS_IMAGE._color = _SNAKE_COLOR;

        LEFT_TAIL_IMAGE._color = _SNAKE_COLOR;
        RIGHT_TAIL_IMAGE._color = _SNAKE_COLOR;

        VERTICAL_JOINT_IMAGE._color = _SNAKE_COLOR;

        BOTTOM_HEAD_IMAGE._color = _SNAKE_COLOR;
        
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

    HORIZONTAL_JOINT_IMAGE._imageData = _TILE_11;
    EXTRA_POINTS_IMAGE._imageData = _DOLLAR_TILE;

    
    LEFT_TAIL_IMAGE._imageData = _TILE_14;

    RIGHT_TAIL_IMAGE._imageData = _TILE_15;

    VERTICAL_JOINT_IMAGE._imageData = _TILE_16;

    BOTTOM_HEAD_IMAGE._imageData = _TILE_17;
    TOP_HEAD_IMAGE._imageData = _TILE_19;
    TOP_HEAD_JOINT_IMAGE._imageData = _TILE_20;
    BOTTOM_HEAD_JOINT_IMAGE._imageData = _TILE_21;
    LEFT_HEAD_JOINT_IMAGE._imageData = _TILE_22;
    RIGHT_HEAD_JOINT_IMAGE._imageData = _TILE_23;
    TOP_TAIL_IMAGE._imageData = _TILE_24;
    
}



