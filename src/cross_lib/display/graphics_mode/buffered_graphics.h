#ifndef ___BUFFERED_GRAPHICS
#define ___BUFFERED_GRAPHICS

#include "display_target_geometry.h"

void display_all(void);

#define _XL_DRAW(x,y,tile,color) \
	video_buffer[(y)][(x)]=tile;

#define _XL_DELETE(x,y) \
	video_buffer[(y)][(x)]=_SPACE;


#endif // ___BUFFERED_GRAPHICS



