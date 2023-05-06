#ifndef _LYNX_TGI_GRAPHICS_GRAPHICS
#define _LYNX_TGI_GRAPHICS_GRAPHICS

#include "cross_lib.h"
#include <tgi.h>

#define FONT_SIZE (26+10+1)


extern SCB_REHV_PAL _tgi_tile[_XL_NUMBER_OF_TILES+FONT_SIZE];
extern SCB_REHV_PAL *empty_spr;
extern SCB_REHV_PAL *clean_bug_spr;

void _tgi_gfx_delete(uint8_t x, uint8_t y); 
void _tgi_gfx_clean_bug(uint8_t x, uint8_t y); 
void _tgi_gfx_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color); 


#define _XL_DELETE(__x,__y) \
    _tgi_gfx_delete(__x,__y)


#define clean_bug(__x,__y) \
    _tgi_gfx_clean_bug(__x,__y)


#define _XL_DRAW(__x,__y,__tile,__color) \
    _tgi_gfx_draw(__x,__y,__tile,__color)



#endif // _LYNX_TGI_GRAPHICS_GRAPHICS
