
#include <graphics.h>
#include <games.h>

#include "../graphics_data/z88dk/z88dk_sprites_definitions.h"

#define __DRAW(x,y,image) \
{ \
	putsprite(spr_or,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char) (image->_imageData*(2+SPRITE_Y_SIZE))); \
}

	
#define __DELETE(x,y) \
{ \
	putsprite(spr_and,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char *) (_SPACE*(2+SPRITE_Y_SIZE)) ); \
}

