#include "display_macros.h"

#include "tiles.h"

#include "images.h"

#include "init_images.h"


extern Image SPACE_SHIP_1_NW_IMAGE;
extern Image SPACE_SHIP_1_NE_IMAGE;
extern Image SPACE_SHIP_1_SW_IMAGE;
extern Image SPACE_SHIP_1_SE_IMAGE;	



void INIT_IMAGES(void)
{
	// Set color data 
	#if !defined(NO_COLOR)
        SPACE_SHIP_1_W_IMAGE._color = _SPACE_SHIP_COLOR;
        SPACE_SHIP_1_E_IMAGE._color = _SPACE_SHIP_COLOR;
        
        SPACE_SHIP_2_W_IMAGE._color = _SPACE_SHIP_COLOR;
        SPACE_SHIP_2_E_IMAGE._color = _SPACE_SHIP_COLOR;
        
        SPACE_SHIP_3_W_IMAGE._color = _SPACE_SHIP_COLOR;
        SPACE_SHIP_3_C_IMAGE._color = _SPACE_SHIP_COLOR;
        SPACE_SHIP_3_E_IMAGE._color = _SPACE_SHIP_COLOR;
        
        SPACE_SHIP_4_W_IMAGE._color = _SPACE_SHIP_COLOR;
        SPACE_SHIP_4_E_IMAGE._color = _SPACE_SHIP_COLOR;
	#endif	

	// Set Image Data
    SPACE_SHIP_1_W_IMAGE._imageData = _TILE_0;
    SPACE_SHIP_1_E_IMAGE._imageData = _TILE_1;
    
    SPACE_SHIP_2_W_IMAGE._imageData = _TILE_2;
    SPACE_SHIP_2_E_IMAGE._imageData = _TILE_3;

    SPACE_SHIP_3_W_IMAGE._imageData = _TILE_19;
    SPACE_SHIP_3_C_IMAGE._imageData = _TILE_20;
    SPACE_SHIP_3_E_IMAGE._imageData = _TILE_21;
    
    SPACE_SHIP_4_W_IMAGE._imageData = _TILE_22;
    SPACE_SHIP_4_E_IMAGE._imageData = _TILE_23;
}



