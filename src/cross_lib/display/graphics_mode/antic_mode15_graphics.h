#ifndef _ATARI_MODE15_COLOR_GRAPHICS_H
#define _ATARI_MODE15_COLOR_GRAPHICS_H

#include "standard_libs.h"

//             set_fore_color(color); \


#include "display_macros.h"
#if !defined(_XL_NO_COLOR)
    #define _XL_DRAW(x, y, tile, color) \
        do \
        { \
            set_position(x,y); \
            set_fore_color(color); \
            output_code(tile); \
        } while(0)
#else
    #define _XL_DRAW(x, y, tile, color) \
        do \
        { \
            set_position(x,y); \
            output_code(tile); \
        } while(0)
#endif

#define _XL_DELETE(x, y) \
    do \
    { \
        set_position(x,y); \
        output_code(0); \
    } while(0)

  
#endif // _ATARI_MODE15_COLOR_GRAPHICS_H

