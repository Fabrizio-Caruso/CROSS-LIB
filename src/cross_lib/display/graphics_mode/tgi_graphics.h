#ifndef _TGI_GRAPHICS
#define _TGI_GRAPHICS

#include <tgi.h>

#define _XL_DRAW(__x,__y,tile,color) \
{ \
    char chString[2]; \
    chString[0] = tile; \
    chString[1]='\0'; \
    tgi_setcolor(color); \
    tgi_outtextxy((((uint16_t)__x)*(uint16_t)8U),((((uint16_t)__y)<<3)),chString); \
}


#define _XL_DELETE(__x,__y) \
{ \
    tgi_outtextxy((((uint16_t)__x)<<3),(((uint16_t)__y)<<3)," "); \
}    
    
#endif // _TGI_GRAPHICS
