
#ifndef _MO5_BIT_MAPPED_GRAPHICS
#define _MO5_BIT_MAPPED_GRAPHICS

#define SV_VIDEO  ((unsigned char*)0x0000)

/*
	unsigned short k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[x+40*(k)+40*8*(y)] = image->_imageData; \
	} \

	unsigned short k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[x+40*(k)+40*8*(y)]=0; \
	} \	
*/


#define __DRAW(x,y,image) \
{ \
	SV_VIDEO[(x)+40*8*(y)] = 255; \
}

#define __DELETE(x,y) \
{ \
	SV_VIDEO[(x)+40*8*(y)] = 0; \
}


#endif // _MO5_BIT_MAPPED_GRAPHICS

