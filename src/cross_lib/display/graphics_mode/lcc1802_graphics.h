#ifndef _LCC1802_GRAPHICS
#define _LCC1802_GRAPHICS

#define BASE_ADDR 0xF800

#define _XLIB_DRAW_TILE(x,y,tile,color) vidcharxy(x,y,tile)

#define __DRAW(x,y,image) vidcharxy(x,y,(image)->_imageData)

#define __DELETE(x,y) vidcharxy(x,y,' ')


#endif // _LCC1802_GRAPHICS

