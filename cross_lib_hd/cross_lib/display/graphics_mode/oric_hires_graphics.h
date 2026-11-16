#ifndef _MEMORY_MAPPED_GRAPHICS
#define _MEMORY_MAPPED_GRAPHICS

#include "standard_libs.h"

#define BASE_ADDR 0xA000

#if defined(_XL_NO_COLOR)
    extern void __oric_hires_draw_no_color(uint8_t x, uint8_t y, uint8_t tile);
    #define _oric_hires_draw(x,y,tile,color) __oric_hires_draw_no_color(x,y,tile)
    #define _XL_DRAW(x,y,tile,color) \
            __oric_hires_draw_no_color(x,y,tile)
#elif !defined(__INVERSE_TILES)
    extern void _oric_hires_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color, uint8_t inverse);
    #define _XL_DRAW(x,y,tile,color) \
            _oric_hires_draw(x,y,tile,color,0)
#else
    extern void _oric_hires_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color);
    #define _XL_DRAW(x,y,tile,color) \
            _oric_hires_draw(x,y,tile,color)
#endif

extern void _oric_hires_delete(uint8_t x, uint8_t y);






#define _XL_DELETE(x,y) \
        _oric_hires_delete(x,y)


#endif // _MEMORY_MAPPED_GRAPHICS



