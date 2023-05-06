#ifndef __Z88DK_SPRITES_GRAPHICS
#define __Z88DK_SPRITES_GRAPHICS

#include <graphics.h>
#include <games.h>

#include <stdint.h>

extern uint8_t sprites[];

#include "display_target_geometry.h"

#define BLIT_OPERATION spr_or

#define _XL_DELETE(x,y) \
    putsprite(spr_and,(x)*(__SPRITE_X_STEP),(y)*(__SPRITE_Y_STEP),sprites + ((_SPACE)*(2+SPRITE_Y_SIZE)))

#if defined(__NO_SPRITE_TRANSPARENCY)
    #define _XL_DRAW(x,y,tile,color) \
        do \
        { \
            _XL_DELETE(x,y); \
            putsprite(BLIT_OPERATION,(x)*(__SPRITE_X_STEP),(y)*(__SPRITE_Y_STEP),sprites + ((tile)*(2+SPRITE_Y_SIZE))); \
        } while(0)
#else
    #define _XL_DRAW(x,y,tile,color) \
        putsprite(BLIT_OPERATION,(x)*(__SPRITE_X_STEP),(y)*(__SPRITE_Y_STEP),sprites + ((tile)*(2+SPRITE_Y_SIZE)))
#endif


#endif // __Z88DK_SPRITES_GRAPHICS


