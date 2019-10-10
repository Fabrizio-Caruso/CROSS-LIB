#ifndef _TGI_GRAPHICS
#define _TGI_GRAPHICS

#include <tgi.h>

#define __DRAW(x,y,image) \
{ \
    char chString[2]; \
    chString[0] = image->_imageData; \
    chString[1]='\0'; \
    tgi_setcolor(image->_color); \
    tgi_setbgcolor(TGI_COLOR_BLACK); \
    tgi_outtextxy((x*8),(y*8),chString); \
    tgi_setbgcolor(TGI_COLOR_GREY); \
}
    

#define __DELETE(x,y) \
{ \
    tgi_setbgcolor(TGI_COLOR_GREY); \
    tgi_outtextxy((x*8),(y*8)," "); \
    tgi_setbgcolor(TGI_COLOR_BLACK); \
}    
    
#endif // _TGI_GRAPHICS
