
#ifndef _BIT_MAPPED_4_GRAPHICS
#define _BIT_MAPPED_4_GRAPHICS

// TODO: Make this more general 
#if defined(__SUPERVISION__)    
    #define EXTRA_PADDING 8
#else
    #define EXTRA_PADDING 0
#endif

#define BYTES_PER_LINE ((XSize)*2+EXTRA_PADDING)

extern uint8_t udgs[];

uint8_t left_map_one_to_two(uint8_t n);
uint8_t right_map_one_to_two(uint8_t n);

#define _XL_DRAW(x,y,tile,color) \
    { \
        uint8_t k; \
        uint16_t offset = (8*(uint8_t)(tile)) ; \
        \
        for(k=0;k<8;++k) \
        { \
            SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)]    = left_map_one_to_two(udgs[offset+k]); \
            SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)+1]  = right_map_one_to_two(udgs[offset+k]); \
        } \
    }


#if defined(_XL_NO_COLOR)
    #define _XL_DELETE(x,y) \
    { \
        uint16_t k; \
        for(k=0;k<8;++k) \
        { \
            SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)]=0; \
            SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)+1]=0; \
        } \
    }
#endif

#endif // _BIT_MAPPED_4_GRAPHICS

