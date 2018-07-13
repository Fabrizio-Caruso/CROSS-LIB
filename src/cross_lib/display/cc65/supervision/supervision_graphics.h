
#ifndef _SUPERVISION_GRAPHICS
#define _SUPERVISION_GRAPHICS

#include <supervision.h>

// 		SV_VIDEO[2*(x)+48*k+48*8*(y)]=byte2wordmsb(redefine_map[(unsigned char) image->_imageData].bitmap); 
//		SV_VIDEO[2*(x)+48*k+48*8*(y)+1]=byte2wordlsb(redefine_map[(unsigned char) image->_imageData].bitmap); 

#define _DRAW(x,y,image) \
{ \
	unsigned short k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[2*(x)+48*k+48*8*(y)] = image->_imageData; \
		SV_VIDEO[2*(x)+48*k+48*8*(y)+1] = image->_imageData; \
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
