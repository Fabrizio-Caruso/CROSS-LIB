#ifndef _TGI_GFX_GRAPHICS
#define _TGI_GFX_GRAPHICS

#include "cross_lib.h"
#include <tgi.h>

extern SCB_REHV_PAL _tgi_tile[_XL_NUMBER_OF_TILES];
extern SCB_REHV_PAL *empty_spr;


#define _XL_DRAW(__x,__y,tile,color) \
do \
{ \
    _tgi_tile[tile].hpos = (__x)*8; \
    _tgi_tile[tile].vpos = (__y)*6; \
    tgi_sprite(&(_tgi_tile[tile])); \
} while(0)


#define _XL_DELETE(__x,__y) \
do \
{ \
    empty_spr->hpos = (__x)*8; \
    empty_spr->vpos = (__y)*6; \
    tgi_sprite(empty_spr); \
} while(0) 

/*
#define _XL_DRAW(__x,__y,tile,color) \
do \
{ \
	while (tgi_busy())  {  }; \
    _tgi_tile[tile].hpos = (__x)*8; \
    _tgi_tile[tile].vpos = (__y)*6; \
    tgi_sprite(&_tgi_tile[tile]); \
    tgi_updatedisplay(); \
} while(0)


#define _XL_DELETE(__x,__y) \
do \
{ \
    empty_spr->hpos = (__x)*8; \
    empty_spr->vpos = (__y)*6; \
    tgi_sprite(empty_spr); \
    tgi_updatedisplay(); \
} while(0) 
*/
#endif // _TGI_GFX_GRAPHICS
