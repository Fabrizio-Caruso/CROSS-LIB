#ifndef _LCC1802_COLOR_GRAPHICS
#define _LCC1802_COLOR_GRAPHICS

#define BASE_ADDR 0xF800

#if !defined(_XL_NO_COLOR)
    #define _XL_DRAW(x,y,tile,color) vidcharxy(((x)+X_OFFSET),((y)+Y_OFFSET),(tile)+(color))
#else
    #define _XL_DRAW(x,y,tile,color) vidcharxy(((x)+X_OFFSET),((y)+Y_OFFSET),(tile))
#endif

#define _XL_DELETE(x,y) vidcharxy(((x)+X_OFFSET),((y)+Y_OFFSET),' ')

    

#endif // _LCC1802_COLOR_GRAPHICS

