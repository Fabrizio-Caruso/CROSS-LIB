
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
	SV_VIDEO[(x)+40*8*(y)] = ((image)->_imageData); \
	SV_VIDEO[(x)+40*8*(y)+1*40] = ((image)->_imageData); \
	SV_VIDEO[(x)+40*8*(y)+2*40] = ((image)->_imageData); \
	SV_VIDEO[(x)+40*8*(y)+3*40] = ((image)->_imageData); \
	SV_VIDEO[(x)+40*8*(y)+4*40] = ((image)->_imageData); \
	SV_VIDEO[(x)+40*8*(y)+5*40] = ((image)->_imageData); \
	SV_VIDEO[(x)+40*8*(y)+6*40] = ((image)->_imageData); \
	SV_VIDEO[(x)+40*8*(y)+7*40] = ((image)->_imageData); \
}

#define __DELETE(x,y) \
{ \
	SV_VIDEO[(x)+40*8*(y)] = 0; \
	SV_VIDEO[(x)+40*8*(y)+1*40] = 0; \
	SV_VIDEO[(x)+40*8*(y)+2*40] = 0; \
	SV_VIDEO[(x)+40*8*(y)+3*40] = 0; \
	SV_VIDEO[(x)+40*8*(y)+4*40] = 0; \
	SV_VIDEO[(x)+40*8*(y)+5*40] = 0; \
	SV_VIDEO[(x)+40*8*(y)+6*40] = 0; \
	SV_VIDEO[(x)+40*8*(y)+7*40] = 0; \
}


#endif // _MO5_BIT_MAPPED_GRAPHICS

