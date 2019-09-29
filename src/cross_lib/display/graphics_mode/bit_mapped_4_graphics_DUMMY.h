
#ifndef _BIT_MAPPED_GRAPHICS
#define _BIT_MAPPED_GRAPHICS

// TODO: Make this more general 
#if defined(__SUPERVISION__)	
	#define EXTRA_PADDING 8
#else
	#define EXTRA_PADDING 0
#endif

#define BYTES_PER_LINE ((XSize)*2+EXTRA_PADDING)
	

#define __DRAW(x,y,image) \
{ \
	unsigned short k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)] = image->_imageData; \
		SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)+1] = image->_imageData; \
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


#endif // _BIT_MAPPED_GRAPHICS

