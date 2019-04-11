
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
	unsigned char i; \
	unsigned short base = (x)+(XSize)*8*(y); \
	unsigned char delta = 0; \
	unsigned char offset = (8*(unsigned char)(image)->_imageData) ; \
	\
	for(i=0;i<7;++i) \
	{ \
		SV_VIDEO[base+delta]  = udgs[offset+i]; \
		delta+=XSize; \
	} \
	SV_VIDEO[base+(XSize)*7] = udgs[offset+7]; \
}


#define __DELETE(x,y) \
{ \
	unsigned char i; \
	unsigned short base = (x)+(XSize)*8*(y); \
	unsigned char delta = 0; \
	\
	for(i=0;i<7;++i) \
	{ \
		SV_VIDEO[(unsigned short) base+delta] = 0; \
		delta+=XSize; \
	} \
	SV_VIDEO[base+(XSize)*7] = 0; \
}

#endif // _MO5_BIT_MAPPED_GRAPHICS

