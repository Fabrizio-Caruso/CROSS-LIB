#ifndef _CONIO_GRAPHICS
#define _CONIO_GRAPHICS



#if !defined(NO_COLOR)
	#define _DRAW(x,y,image) \
	do \
	{ \
		gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
		SET_TEXT_COLOR(image->_color); \
		cputc(image->_imageData); \
	} \
	while(0)
		
#else
	#define _DRAW(x,y,image) \
	do \
	{ \
		gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
		cputc(image->_imageData); \
	} \
	while(0)

#endif

#define _DELETE(x,y) \
	do \
	{ \
		gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
		cputc(_SPACE); \
	} \
	while(0)
		
#endif // _CONIO_GRAPHICS



