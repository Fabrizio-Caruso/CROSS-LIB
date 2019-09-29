
#ifndef _BIT_MAPPED_4_GRAPHICS
#define _BIT_MAPPED_4_GRAPHICS

// TODO: Make this more general 
#if defined(__SUPERVISION__)	
	#define EXTRA_PADDING 8
#else
	#define EXTRA_PADDING 0
#endif

#define BYTES_PER_LINE ((XSize)*2+EXTRA_PADDING)

extern unsigned char udgs[];

#if defined(NO_COLOR)
    #define __DRAW(x,y,image) \
    { \
		unsigned char k; \
		unsigned short base = (x)+(XSize)*8*(y); \
		unsigned char delta = 0; \
		unsigned char offset = (8*(unsigned char)(image)->_imageData) ; \
        \
        for(k=0;k<8;++k) \
        { \
			SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)]  = udgs[offset+k]; \
			delta+=XSize; \
        } \
    }

    #define __DELETE(x,y) \
    { \
        unsigned short k; \
        for(k=0;k<8;++k) \
        { \
            SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)]=0; \
            SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)+1]=0; \
        } \
    }
#endif

#endif // _BIT_MAPPED_4_GRAPHICS

