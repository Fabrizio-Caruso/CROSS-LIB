
#ifndef _BIT_MAPPED_4_GRAPHICS
#define _BIT_MAPPED_4_GRAPHICS

// TODO: Make this more general 
#if defined(__SUPERVISION__)    
    #define EXTRA_PADDING 8
#else
    #define EXTRA_PADDING 0
#endif

/*

NON-REVERSED MAPPING (e.g., COCO/DRAGON) 

color 1
0x00,0x01,0x04,0x05,0x10,0x11,0x14,0x15,0x40,0x41,0x44,0x45,0x50,0x51,0x54,0x55,

color 2
0x00,0x02,0x08,0x0A,0x20,0x22,0x28,0x2A,0x80,0x82,0x88,0x8A,0xA0,0xA2,0xA8,0xAA,

color 3
0x00,0x03,0x0C,0x0F,0x30,0x33,0x3C,0x3F,0xC0,0xC3,0xCC,0xCF,0xF0,0xF3,0xFC,0xFF,

*/

#if defined(__SUPERVISION__)
    #define BYTES_PER_LINE ((XSize)*2+EXTRA_PADDING)
#else
   #define BYTES_PER_LINE ((XSize)*2) 
#endif


extern uint8_t udgs[];

uint8_t left_map_one_to_two(uint8_t n);
uint8_t right_map_one_to_two(uint8_t n);

void supervision_draw(uint8_t x, uint8_t y, uint8_t tile);

void supervision_delete(uint8_t x, uint8_t y);


#define _XL_DRAW(x,y,tile,color) \
    supervision_draw(x,y,tile)

#define _XL_DELETE(x,y) \
    supervision_delete(x,y)


#endif // _BIT_MAPPED_4_GRAPHICS

