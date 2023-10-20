
#ifndef _BIT_MAPPED_16_GRAPHICS
#define _BIT_MAPPED_16_GRAPHICS

#if defined(__COCO3__)
	#define SV_VIDEO ((uint8_t*)0x8000)
#endif

extern uint8_t udgs[];

#define BYTES_PER_LINE ((XSize)*4)

uint8_t first_map_one_to_four(uint8_t n);
uint8_t second_map_one_to_four(uint8_t n);
uint8_t third_map_one_to_four(uint8_t n);
uint8_t fourth_map_one_to_four(uint8_t n);

void _color_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color);

void _color_delete(uint8_t x, uint8_t y);


#define _XL_DRAW(x,y,tile,color) \
    _color_draw(x,y,tile,color)

#define _XL_DELETE(x,y) \
    _color_delete(x,y)


#endif // _BIT_MAPPED_16_GRAPHICS

