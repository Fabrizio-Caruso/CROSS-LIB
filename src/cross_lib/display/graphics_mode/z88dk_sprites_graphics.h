#ifndef __Z88DK_SPRITES_GRAPHICS
#define __Z88DK_SPRITES_GRAPHICS

#include <graphics.h>
#include <games.h>

#include <stdint.h>

extern uint8_t sprites[];

#include "display_target_geometry.h"

#define BLIT_OPERATION spr_or

#define __DELETE(x,y) \
    putsprite(spr_and,(x)*(SPRITE_X_STEP),(y)*(SPRITE_Y_STEP),sprites + ((_SPACE)*(2+SPRITE_Y_SIZE)))

#if defined(NO_TRANSPARENCY)
    #define _XLIB_DRAW_TILE(x,y,tile,color) \
        do \
        { \
            __DELETE(x,y); \
            putsprite(BLIT_OPERATION,(x)*(SPRITE_X_STEP),(y)*(SPRITE_Y_STEP),sprites + ((tile)*(2+SPRITE_Y_SIZE))); \
        } while(0)
#else
    #define _XLIB_DRAW_TILE(x,y,tile,color) \
        putsprite(BLIT_OPERATION,(x)*(SPRITE_X_STEP),(y)*(SPRITE_Y_STEP),sprites + ((tile)*(2+SPRITE_Y_SIZE)))
#endif


#define __DRAW(x,y,image) \
	putsprite(BLIT_OPERATION,(x)*(SPRITE_X_STEP),(y)*(SPRITE_Y_STEP),sprites + ((image)->_imageData*(2+SPRITE_Y_SIZE)))



#endif // __Z88DK_SPRITES_GRAPHICS

