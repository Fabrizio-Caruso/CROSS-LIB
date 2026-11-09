#include <stdint.h>

#if !defined(_XL_NO_COLOR)

void _bbc_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color);
#else
void _bbc_draw(uint8_t x, uint8_t y, uint8_t tile);

#endif

void _bbc_delete(uint8_t x, uint8_t y);

#if !defined(_XL_NO_COLOR)
    #define _XL_DRAW(x,y,tile,color) _bbc_draw(x,y,tile,color)
#else
    #define _XL_DRAW(x,y,tile,color) _bbc_draw(x,y,tile)
#endif

#define _XL_DELETE(x,y) _bbc_delete(x,y)

