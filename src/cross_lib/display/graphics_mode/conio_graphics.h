#ifndef _CONIO_GRAPHICS
#define _CONIO_GRAPHICS



#if !defined(NO_COLOR)

	#define _XL_DRAW(x,y,tile,color) \
	do \
	{ \
		gotoxy((x),(y)); \
		_XL_SET_TEXT_COLOR(color); \
		cputc(tile); \
	} \
	while(0)


#else

	#define _XL_DRAW(x,y,tile,color) \
	do \
	{ \
		gotoxy((x),(y)); \
		cputc(tile); \
	} \
	while(0)
    

#endif

#define _XL_DELETE(x,y) \
	do \
	{ \
		gotoxy((x),(y)); \
		cputc(_SPACE); \
	} \
	while(0)
		
#endif // _CONIO_GRAPHICS



