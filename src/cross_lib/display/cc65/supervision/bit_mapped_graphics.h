
#ifndef _BIT_MAPPED
#define _BIT_MAPPED

#if defined(__SUPERVISION__)
	#define BIT_MAP ((unsigned char*)0x4000)
	#define EXTRA_INVISIBLE 4
#endif


#define _DRAW(x,y,image) \
{ \
	unsigned char k; \
	for(k=0;k<8;++k) \
	{ \
		BIT_MAP[2*x+2*(XSize+EXTRA_INVISIBLE)*(k+8*y)]= 255; \
		BIT_MAP[2*x+2*(XSize+EXTRA_INVISIBLE)*(k+8*y)+1]= 255; \
	} \
}

#define _DELETE(x,y) \
{ \
	unsigned char k; \
	for(k=0;k<8;++k) \
	{ \
		BIT_MAP[2*x+2*(XSize+EXTRA_INVISIBLE)*(k+8*y)]= 255; \
		BIT_MAP[2*x+2*(XSize+EXTRA_INVISIBLE)*(k+8*y)+1]= 255; \
	} \
}



#endif _BIT_MAPPED