#ifndef _TGI_GRAPHICS
#define _TGI_GRAPHICS

#include <tgi.h>

#define __DRAW(x,y,image) \
{ \
    char chString[2]; \
    chString[0] = image->_imageData; \
    chString[1]='\0'; \
    tgi_setcolor(image->_color); \
    tgi_outtextxy((x*8),(y*8),chString); \
}
    

#define __DELETE(x,y) \
{ \
    tgi_outtextxy((x*8),(y*8)," "); \
}    
    
#endif // _TGI_GRAPHICS
