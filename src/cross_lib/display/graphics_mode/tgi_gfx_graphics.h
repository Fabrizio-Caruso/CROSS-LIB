#ifndef _TGI_GFX_GRAPHICS
#define _TGI_GFX_GRAPHICS

#include "cross_lib.h"
#include <tgi.h>

extern SCB_REHV_PAL _tgi_tile[_XL_NUMBER_OF_TILES];
extern SCB_REHV_PAL *empty_spr;
extern SCB_REHV_PAL *clean_bug_spr;

#define _XL_DELETE(__x,__y) \
do \
{ \
    empty_spr->hpos = (__x)*8; \
    empty_spr->vpos = (__y)*6; \
    tgi_sprite(empty_spr); \
} while(0) 



#define clean_bug(__x,__y) \
do \
{ \
    clean_bug_spr->hpos = (__x)*8; \
    clean_bug_spr->vpos = (__y)*6; \
    tgi_sprite(clean_bug_spr); \
} while(0) 


#define _XL_DRAW(__x,__y,tile,color) \
do \
{ \
    clean_bug(__x,__y); \
    _tgi_tile[tile].hpos = (__x)*8; \
    _tgi_tile[tile].vpos = (__y)*6; \
    _tgi_tile[tile].penpal[0]=(unsigned char) (0x11+(color)); \
    tgi_sprite(&(_tgi_tile[tile])); \
} while(0)



#endif // _TGI_GFX_GRAPHICS
