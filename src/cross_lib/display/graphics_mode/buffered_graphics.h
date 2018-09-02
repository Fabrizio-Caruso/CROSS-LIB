#ifndef _BUFFERED_GRAPHICS
#define _BUFFERED_GRAPHICS



#define __DRAW(x,y,image) \
	do \
	{ \
		video_buffer[y][x]=image->_imageData; \
		display_all(); \
		printf("\n"); \
	} while(0)



#define __DELETE(x,y) \
	video_buffer[y][x]=_SPACE;


#endif // _BUFFERED_GRAPHICS



