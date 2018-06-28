#ifndef _CONIO_GRAPHICS
#define _CONIO_GRAPHICS


#if !defined(NO_COLOR)
	#define _DRAW(x,y,image) \
	do \
	{ \
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); \
		SET_TEXT_COLOR(image->_color); \
		cputc(image->_imageData); \
	} \
	while(0)

	#define _DRAW_VERTICAL_WALL(x,y) \
	do \
	{ \
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); \
		SET_TEXT_COLOR(COLOR_WHITE); \
		cputc(_VERTICAL_BRICK); \
	} \
	while(0)
		
	#define _DRAW_HORIZONTAL_WALL(x,y) \
	do \
	{ \
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); \
		SET_TEXT_COLOR(COLOR_WHITE); \
		cputc(_HORIZONTAL_BRICK); \
	} \
	while(0)
		
#else
	#define _DRAW(x,y,image) \
	do \
	{ \
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); \
		cputc(image->_imageData); \
	} \
	while(0)


	#define _DRAW_VERTICAL_WALL(x,y) \
	do \
	{ \
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); \
		cputc(_VERTICAL_BRICK); \
	} \
	while(0)
		
	#define _DRAW_HORIZONTAL_WALL(x,y) \
	do \
	{ \
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); \
		cputc(_HORIZONTAL_BRICK); \
	} \
	while(0)

#endif

#define _DELETE(x,y) \
	do \
	{ \
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); \
		cputc(_SPACE); \
	} \
	while(0)
		
#define _DRAW_BROKEN_WALL(x,y) \
	do \
	{ \
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); \
		cputc(_BROKEN_WALL); \
	} \
	while(0)

#endif // _CONIO_GRAPHICS



