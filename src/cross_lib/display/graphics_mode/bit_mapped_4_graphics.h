
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

void supervision_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color);

void supervision_delete(uint8_t x, uint8_t y);


#define _XL_DRAW(x,y,tile,color) \
    supervision_draw(x,y,tile,color)

#define _XL_DELETE(x,y) \
    supervision_delete(x,y)


#endif // _BIT_MAPPED_4_GRAPHICS

