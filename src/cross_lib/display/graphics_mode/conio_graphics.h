#ifndef _CONIO_GRAPHICS
#define _CONIO_GRAPHICS



#if !defined(NO_COLOR)

	#define _XLIB_DRAW_TILE(x,y,tile,color) \
	do \
	{ \
		gotoxy((x),(y)); \
		SET_TEXT_COLOR(color); \
		cputc(tile); \
	} \
	while(0)

	#define __DRAW(x,y,image) \
	do \
	{ \
		gotoxy((x),(y)); \
		SET_TEXT_COLOR(image->_color); \
		cputc(image->_imageData); \
	} \
	while(0)
		
#else

	#define _XLIB_DRAW_TILE(x,y,tile,color) \
	do \
	{ \
		gotoxy((x),(y)); \
		cputc(tile); \
	} \
	while(0)
    
	#define __DRAW(x,y,image) \
	do \
	{ \
		gotoxy((x),(y)); \
		cputc(image->_imageData); \
	} \
	while(0)

#endif

#define __DELETE(x,y) \
	do \
	{ \
		gotoxy((x),(y)); \
		cputc(_SPACE); \
	} \
	while(0)
		
#endif // _CONIO_GRAPHICS



