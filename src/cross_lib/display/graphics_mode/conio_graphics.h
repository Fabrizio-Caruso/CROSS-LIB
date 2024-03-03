#ifndef _CONIO_GRAPHICS_H
#define _CONIO_GRAPHICS_H


#if !defined(_XL_NO_COLOR)

	#if defined(__NO_BOTTOM)
		#define _XL_DRAW(x,y,tile,color) \
		do \
		{ \
			if(y<YSize-1) \
			{ \
				gotoxy((X_OFFSET+(x)),(Y_OFFSET+(y))); \
				_XL_SET_TEXT_COLOR(color); \
				cputc(tile); \
			} \
		} \
		while(0)
	#else
		#define _XL_DRAW(x,y,tile,color) \
		do \
		{ \
			gotoxy((X_OFFSET+(x)),(Y_OFFSET+(y))); \
			_XL_SET_TEXT_COLOR(color); \
			cputc(tile); \
		} \
		while(0)
		
	#endif


#else

	#if defined(__NO_BOTTOM)
		#define _XL_DRAW(x,y,tile,color) \
		do \
		{ \
			if(y<YSize-1) \
			{ \
				gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
				cputc(tile); \
			} \
		} \
		while(0)
	#else

		#define _XL_DRAW(x,y,tile,color) \
		do \
		{ \
			gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
			cputc(tile); \
		} \
		while(0)
    #endif

#endif

#if defined(__NO_BOTTOM)
	#define _XL_DELETE(x,y) \
		do \
		{ \
			if(y<YSize-1) \
			{ \
				gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
				cputc(_SPACE); \
			} \
		} \
		while(0)
#else
	
	#define _XL_DELETE(x,y) \
		do \
		{ \
			gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
			cputc(_SPACE); \
		} \
		while(0)

#endif
		
#endif // _CONIO_GRAPHICS_H



