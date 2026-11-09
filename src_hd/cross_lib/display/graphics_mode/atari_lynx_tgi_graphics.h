#ifndef _LYNX_TGI_GRAPHICS_GRAPHICS
#define _LYNX_TGI_GRAPHICS_GRAPHICS

#include "cross_lib.h"
#include <tgi.h>

#define FONT_SIZE (26+10+1)

typedef struct scb_hv_pal2 {
    unsigned char sprctl0;
    unsigned char sprctl1; // set REHV
    unsigned char sprcoll;
    char *next;
    unsigned char *data;
    signed int hpos;
    signed int vpos;
    unsigned int hsize;
    unsigned int vsize;
    unsigned char penpal[2];
} scb_hv_pal2;

extern scb_hv_pal2 _tgi_tile[_XL_NUMBER_OF_TILES+FONT_SIZE];
extern scb_hv_pal2 *empty_spr;

void _tgi_gfx_delete(uint8_t x, uint8_t y);
void _tgi_gfx_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color);

#define _XL_DELETE(__x,__y) \
    _tgi_gfx_delete(__x,__y)

#define _XL_DRAW(__x,__y,__tile,__color) \
    _tgi_gfx_draw(__x,__y,__tile,__color)

#endif // _LYNX_TGI_GRAPHICS_GRAPHICS
