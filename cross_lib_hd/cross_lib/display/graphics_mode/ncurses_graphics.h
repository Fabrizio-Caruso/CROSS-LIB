#define gotoxy(x,y) do { move(y,x); refresh(); } while(0)
#define cputc(c) do { addch(c); refresh(); } while(0)

#define _XL_DRAW(x,y,tile,color) \
do \
{ \
    gotoxy((X_OFFSET+(x)),(Y_OFFSET+(y))); \
    _XL_SET_TEXT_COLOR(color); \
    cputc(tile); \
} \
while(0)

#define _XL_DELETE(x,y) \
    do \
    { \
        gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
        cputc(_SPACE); \
    } \
    while(0)


