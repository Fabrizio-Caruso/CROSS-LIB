#ifndef __Z88DK_SPRITES_GRAPHICS
#define __Z88DK_SPRITES_GRAPHICS

#include <graphics.h>
#include <games.h>

#if !defined(SEPARATE_SPRITE_DEFINITIONS)
    #include "z88dk_sprites_definitions.h"
#endif
#include "display_target_geometry.h"


#define __DRAW(x,y,image) \
{ \
	putsprite(spr_or,x*(SPRITE_X_STEP),y*(SPRITE_Y_STEP),sprites + (unsigned char *) (image->_imageData*(2+SPRITE_Y_SIZE))); \
}

	
#define __DELETE(x,y) \
{ \
	putsprite(spr_and,x*(SPRITE_X_STEP),y*(SPRITE_Y_STEP),sprites + (unsigned char *) ((_SPACE)*(2+SPRITE_Y_SIZE)) ); \
}

#endif // __Z88DK_SPRITES_GRAPHICS

