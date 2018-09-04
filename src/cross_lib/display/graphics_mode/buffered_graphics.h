#ifndef _BUFFERED_GRAPHICS
#define _BUFFERED_GRAPHICS

#include "../display_target_geometry.h"

void display_all(void);

unsigned char video_buffer[YSize+Y_OFFSET][XSize];


#define __DRAW(x,y,image) \
	video_buffer[y][x]=image->_imageData;


#define __DELETE(x,y) \
	video_buffer[y][x]=_SPACE;


#endif // _BUFFERED_GRAPHICS



