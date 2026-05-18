#ifndef _PV1000_GRAPHICS_H
#define _PV1000_GRAPHICS_H



#define _XL_DRAW(x,y,tile,color) \
do \
{ \
    gotoxy((X_OFFSET+(x)),(Y_OFFSET+(y))); \
    cputc(tile+color); \
} \
while(0)




#define _XL_DELETE(x,y) \
do \
{ \
    gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
    cputc(_SPACE); \
} \
while(0)


#endif // _PV1000_GRAPHICS_H



