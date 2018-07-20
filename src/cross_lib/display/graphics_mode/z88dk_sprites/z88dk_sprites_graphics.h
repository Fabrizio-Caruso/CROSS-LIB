
#include <graphics.h>
#include <games.h>

#include "z88dk_sprites_definitions.h"

#define _DRAW(x,y,image) \
{ \
	putsprite(spr_or,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char) (image->_imageData*(2+SPRITE_Y_SIZE))); \
}

	
#define _DELETE(x,y) \
{ \
	putsprite(spr_and,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char *) (_SPACE*(2+SPRITE_Y_SIZE)) ); \
}

