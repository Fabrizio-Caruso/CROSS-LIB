
//#include "../../../display/display_macros.h"

#include "z88dk_sprites_settings.h"

#include <graphics.h>
#include <games.h>

#include "z88dk_sprites_definitions.h"


extern unsigned char sprites[];


#define _DRAW(x,y,image) \
{ \
	putsprite(spr_or,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char) (image*(2+SPRITE_Y_SIZE))); \
}

	
#define _DELETE(x,y) \
{ \
	putsprite(spr_and,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char *) (_SPACE*(2+SPRITE_Y_SIZE)) ); \
}

#define _DRAW_VERTICAL_WALL(x,y) \
{ \
	putsprite(spr_or,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char *) (_VERTICAL_BAR*(2+SPRITE_Y_SIZE)) ); \
}
 
#define _DRAW_HORIZONTAL_WALL(x,y) \
{ \
	putsprite(spr_or,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char *) (_HORIZONTAL_BAR*(2+SPRITE_Y_SIZE)) ); \
}

#define _DRAW_BROKEN_WALL(x,y) \
{ \
}

