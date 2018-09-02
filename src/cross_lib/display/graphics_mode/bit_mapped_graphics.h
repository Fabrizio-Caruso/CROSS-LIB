
#ifndef _BIT_MAPPED_GRAPHICS
#define _BIT_MAPPED_GRAPHICS

// TODO: Make this more general 
#if defined(__SUPERVISION__)
	#include <supervision.h>
#endif
	

#define __DRAW(x,y,image) \
{ \
	unsigned short k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[2*(x)+48*k+48*8*(y)] = image->_imageData; \
		SV_VIDEO[2*(x)+48*k+48*8*(y)+1] = image->_imageData; \
	} \
}

#define __DELETE(x,y) \
{ \
	unsigned short k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[2*(x)+48*k+48*8*(y)]=0; \
		SV_VIDEO[2*(x)+48*k+48*8*(y)+1]=0; \
	} \
}


#endif // _BIT_MAPPED_GRAPHICS

