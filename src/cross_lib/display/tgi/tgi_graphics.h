#ifndef _TGI_GRAPHICS
#define _TGI_GRAPHICS

#include <tgi.h>


#define _BG_COLOR COLOR_GREY


#define _DRAW(x,y,image) \
	{ \
	char chString[2]; \
	chString[0] = image->_imageData; \
	chString[1]='\0'; \
	tgi_setbgcolor(COLOR_BLACK); \
	tgi_setcolor(image->_color); \
	tgi_outtextxy(x*8,y*8,chString); \
	}
	

#define _DELETE(x,y) \
	{ \
	char chString[2]; \
	chString[0] = ' '; \
	chString[1]='\0'; \
	tgi_setbgcolor(_BG_COLOR); \
	tgi_setcolor(COLOR_BLACK); \
	tgi_outtextxy(x*8,y*8,chString); \
	}    
	
#endif // _TGI_GRAPHICS
