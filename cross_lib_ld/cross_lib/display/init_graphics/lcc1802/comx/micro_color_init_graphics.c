#include <devkit/video/vis_video.h>
#include <devkit/system/system.h>

#if !defined(_XL_NO_UDG)
    #if !defined(NTSC)
        #define NUMBER_OF_LINES 9
        #include "6x9_chars.h"
    #else
        #define NUMBER_OF_LINES 8
        #include "6x8_chars.h"
    #endif
#endif


#include "micro_color_settings.h"

#include "standard_libs.h"
#include "cross_lib.h"

#include "devkit/video/vis_char.h"

#define _BASE_COLOR 192

void redefine_char(uint8_t ch, const uint8_t * shapelocation)
{
    uint8_t colored_shape[NUMBER_OF_LINES];
    uint8_t i;
    

    for(i=0;i<NUMBER_OF_LINES;++i)
    {
       colored_shape[i]=shapelocation[i]+_BASE_COLOR;
    }
    shapechar(colored_shape, ch, 1);
}

const struct redefine_struct
{
   const uint8_t _ascii;
   const uint8_t _bitmap[9];
} ;


// {0,16, 62, 32, 60,  4,124,  8,  0}},
const struct redefine_struct redefine_map[] =
{
    {_TILE_0, _TILE_0_UDG},
    {_TILE_1, _TILE_1_UDG},
    {_TILE_2, _TILE_2_UDG},
    {_TILE_3, _TILE_3_UDG},
    {_TILE_4, _TILE_4_UDG},
    {_TILE_5, _TILE_5_UDG},
    {_TILE_6, _TILE_6_UDG},
    {_TILE_7, _TILE_7_UDG},
    {_TILE_8, _TILE_8_UDG},
    {_TILE_9, _TILE_9_UDG},
    {_TILE_10, _TILE_10_UDG},
    {_TILE_11, _TILE_11_UDG},
    {_TILE_12, _TILE_12_UDG},
    {_TILE_13, _TILE_13_UDG},
    {_TILE_14, _TILE_14_UDG},
    {_TILE_15, _TILE_15_UDG},
    {_TILE_16, _TILE_16_UDG},
    {_TILE_17, _TILE_17_UDG},
    {_TILE_18, _TILE_18_UDG},
    #if _XL_NUMBER_OF_TILES>=20
    {_TILE_19, _TILE_19_UDG},
    #if _XL_NUMBER_OF_TILES>=21
    {_TILE_20, _TILE_20_UDG},
    #if _XL_NUMBER_OF_TILES>=22
    {_TILE_21, _TILE_21_UDG},
    #if _XL_NUMBER_OF_TILES>=23
    {_TILE_22, _TILE_22_UDG},
    #if _XL_NUMBER_OF_TILES>=24
    {_TILE_23, _TILE_23_UDG},
    #if _XL_NUMBER_OF_TILES>=25
    {_TILE_24, _TILE_24_UDG},
    #if _XL_NUMBER_OF_TILES>=26
    {_TILE_25, _TILE_25_UDG},
    #if _XL_NUMBER_OF_TILES>=27
    {_TILE_26, _TILE_26_UDG},
    #endif
    #endif
    #endif
    #endif
    #endif
    #endif
    #endif
    #endif
    // {_DOLLAR_TILE, _DOLLAR_DEFINITION},
};



#include "lcc1802_color_graphics.h"


void _XL_INIT_GRAPHICS(void)
{
    uint8_t i;
    
    initvideo();
    setvideobase(BASE_ADDR);
    
    disableinterrupt();
    
    character_set(4);
    
    
    for(i=0;i<_XL_NUMBER_OF_TILES;++i)
    {
        redefine_char(redefine_map[i]._ascii, redefine_map[i]._bitmap);
    }
    
    #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
        (void) bgcolor(7);
    #else
        (void) bgcolor(0);
    #endif
    textcolordefinition(3);
    monochrome(0);
    
    _setScreenColors();
}

