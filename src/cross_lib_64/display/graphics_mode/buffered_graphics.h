#ifndef ___BUFFERED_GRAPHICS
#define ___BUFFERED_GRAPHICS

#include "display_target_geometry.h"

#if defined(__C64__) || defined(__C128__)
    #if defined(__DOUBLE_BUFFER)
        #define BASE_ADDR 0xB800
        #define COLOR_ADDR 0xBC00
        #define REAL_BASE_ADDR 0xC000
        #define REAL_COLOR_ADDR 0xD800
    #else
        #define BASE_ADDR 0xC000
        #define COLOR_ADDR 0xD800
    #endif
#endif

void display_all(void);

#define _XL_DRAW(x,y,tile,color) \
	video_buffer[(y)][(x)]=tile;

#define _XL_DELETE(x,y) \
	video_buffer[(y)][(x)]=_SPACE;


#endif // ___BUFFERED_GRAPHICS



