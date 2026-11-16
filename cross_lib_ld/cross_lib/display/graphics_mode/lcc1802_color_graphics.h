#ifndef _LCC1802_COLOR_GRAPHICS
#define _LCC1802_COLOR_GRAPHICS

#define BASE_ADDR 0xF800

#if defined(__LCC1802_UNBUFFERED)
	#define VIDCHARXY(x,y,ch) vidcharnobufxy(x,y,ch)
	#define VIDFLUSH()
#elif defined(VIS_INT)
	#define VIDCHARXY(x,y,ch) vidcharxy(x,y,ch)
	#define VIDFLUSH()
#else
	#define VIDCHARXY(x,y,ch) vidcharxy(x,y,ch)
	#define VIDFLUSH() vidflush()
#endif


#if !defined(_XL_NO_COLOR)
    #define _XL_DRAW(x,y,tile,color) VIDCHARXY(((x)+X_OFFSET),((y)+Y_OFFSET),(tile)+(color))
#else
    #define _XL_DRAW(x,y,tile,color) VIDCHARXY(((x)+X_OFFSET),((y)+Y_OFFSET),(tile))
#endif

#define _XL_DELETE(x,y) VIDCHARXY(((x)+X_OFFSET),((y)+Y_OFFSET),' ')

    

#endif // _LCC1802_COLOR_GRAPHICS

