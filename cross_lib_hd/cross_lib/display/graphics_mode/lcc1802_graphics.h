#ifndef _LCC1802_GRAPHICS
#define _LCC1802_GRAPHICS

#define BASE_ADDR 0xF800

#define _XL_DRAW(x,y,tile,color) vidcharxy(((x)+X_OFFSET),((y)+Y_OFFSET),tile)

#define _XL_DELETE(x,y) vidcharxy(((x)+X_OFFSET),((y)+Y_OFFSET),' ')


#endif // _LCC1802_GRAPHICS

