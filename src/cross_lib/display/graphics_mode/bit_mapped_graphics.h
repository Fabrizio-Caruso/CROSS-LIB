
#ifndef _MO5_BIT_MAPPED_GRAPHICS
#define _MO5_BIT_MAPPED_GRAPHICS

#if defined(__MO5__)
	#define SV_VIDEO  ((unsigned char*)0x0000)
#elif defined(__TO7__)
	#define SV_VIDEO  ((unsigned char*)0x4000)
#endif

extern unsigned char udgs[];

#define __DRAW(x,y,image) \
{ \
	SV_VIDEO[(x)+(XSize)*8*(y)]           = udgs[(8*(image)->_imageData)]; \
	SV_VIDEO[(x)+(XSize)*8*(y)+1*(XSize)] = udgs[(8*(image)->_imageData)+1]; \
	SV_VIDEO[(x)+(XSize)*8*(y)+2*(XSize)] = udgs[(8*(image)->_imageData)+2]; \
	SV_VIDEO[(x)+(XSize)*8*(y)+3*(XSize)] = udgs[(8*(image)->_imageData)+3]; \
	SV_VIDEO[(x)+(XSize)*8*(y)+4*(XSize)] = udgs[(8*(image)->_imageData)+4]; \
	SV_VIDEO[(x)+(XSize)*8*(y)+5*(XSize)] = udgs[(8*(image)->_imageData)+5]; \
	SV_VIDEO[(x)+(XSize)*8*(y)+6*(XSize)] = udgs[(8*(image)->_imageData)+6]; \
	SV_VIDEO[(x)+(XSize)*8*(y)+7*(XSize)] = udgs[(8*(image)->_imageData)+7]; \
}

#define __DELETE(x,y) \
{ \
	SV_VIDEO[(x)+(XSize)*8*(y)] = 0; \
	SV_VIDEO[(x)+(XSize)*8*(y)+1*(XSize)] = 0; \
	SV_VIDEO[(x)+(XSize)*8*(y)+2*(XSize)] = 0; \
	SV_VIDEO[(x)+(XSize)*8*(y)+3*(XSize)] = 0; \
	SV_VIDEO[(x)+(XSize)*8*(y)+4*(XSize)] = 0; \
	SV_VIDEO[(x)+(XSize)*8*(y)+5*(XSize)] = 0; \
	SV_VIDEO[(x)+(XSize)*8*(y)+6*(XSize)] = 0; \
	SV_VIDEO[(x)+(XSize)*8*(y)+7*(XSize)] = 0; \
}


#endif // _MO5_BIT_MAPPED_GRAPHICS

