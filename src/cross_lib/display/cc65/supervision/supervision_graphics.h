
#ifndef _SUPERVISION_GRAPHICS
#define _SUPERVISION_GRAPHICS

#include <supervision.h>


//#define EXTRA_INVISIBLE 4


// TODO: Remove BOGUS graphics data (image->_imageData points to the real data and is not the data itself) 

// 		SV_VIDEO[2*x+2*(XSize+EXTRA_INVISIBLE)*(k+8*y)]= image->_imageData; 
//		SV_VIDEO[2*x+2*(XSize+EXTRA_INVISIBLE)*(k+8*y)+1]= image->_imageData; 

#define _DRAW(x,y,image) \
{ \
	unsigned short k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[2*(x)+48*k+48*8*(y)]=128+32+8+2; \
		SV_VIDEO[2*(x)+48*k+48*8*(y)+1]=128+32+8+2; \
	} \
}

#define _DELETE(x,y) \
{ \
	unsigned short k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[2*(x)+48*k+48*8*(y)]=0; \
		SV_VIDEO[2*(x)+48*k+48*8*(y)+1]=0; \
	} \
}


#endif // _SUPERVISION_GRAPHICS
