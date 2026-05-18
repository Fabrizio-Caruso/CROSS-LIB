#include <devkit/video/vis_video.h>
#include <devkit/system/system.h>

#if !defined(_XL_NO_UDG)
    #include "6x9_chars.h"
#endif

#if defined(__COMX__) || defined(__PECOM__) || (defined(__CIDELSA__) && !defined(_XL_NO_UDG))
    #include "comx_settings.h"
#elif defined(__MICRO__)
    #include "micro_settings.h"

#endif

#include "standard_libs.h"
#include "cross_lib.h"

#if defined(__CIDELSA__)
    #include <devkit/video/cidelsa_char.h>
#endif

#if defined(__MICRO__)
    #include "devkit/video/vis_char.h"
#endif

#if defined(__COMX__) || defined(__PECOM__) || defined(__MICRO__) || !defined(_XL_NO_UDG)
    void redefine_char(uint8_t ch, const uint8_t * shapelocation, int color)
    {
        uint8_t colored_shape[9];
        uint8_t i;
        
        for(i=0;i<9;++i)
        {
           colored_shape[i]=shapelocation[i]+color*64;
        }
        shapechar(colored_shape, ch, 1);
    }

    const struct redefine_struct
    {
       const uint8_t _ascii;
       const uint8_t _bitmap[9];
       const uint8_t _color;
    } ;


const struct redefine_struct redefine_map[] =
{
    {_TILE_0, _TILE_0_UDG, 3},
    {_TILE_1, _TILE_1_UDG, 3},
    {_TILE_2, _TILE_2_UDG, 3},
    {_TILE_3, _TILE_3_UDG, 3},
    {_TILE_4, _TILE_4_UDG, 3},
    {_TILE_5, _TILE_5_UDG, 0},
    {_TILE_6, _TILE_6_UDG, 0},
    {_TILE_7, _TILE_7_UDG, 2},
    {_TILE_8, _TILE_8_UDG, 2},
    {_TILE_9, _TILE_9_UDG, 1},
    {_TILE_10, _TILE_10_UDG, 3},
    {_TILE_11, _TILE_11_UDG, 3},
    {_TILE_12, _TILE_12_UDG, 2},
    {_TILE_13, _TILE_13_UDG, 2},
    {_TILE_14, _TILE_14_UDG, 3},
    {_TILE_15, _TILE_15_UDG, 3},
    {_TILE_16, _TILE_16_UDG, 3},
    {_TILE_17, _TILE_17_UDG, 3},
    {_TILE_18, _TILE_18_UDG, 0},
    {_TILE_19, _TILE_19_UDG, 3},
    {_TILE_20, _TILE_20_UDG, 3},
    {_TILE_21, _TILE_21_UDG, 3},
    {_TILE_22, _TILE_22_UDG, 3},
    {_TILE_23, _TILE_23_UDG, 3},
    {_TILE_24, _TILE_24_UDG, 3},
    {_TILE_25, _TILE_25_UDG, 3},
    {_TILE_26, _TILE_26_UDG, 3},
};



#endif

#if defined(__MEMORY_MAPPED_GRAPHICS)
    #include "memory_mapped_graphics.h"
#elif defined(__LCC1802_GRAPHICS)
    #include "lcc1802_graphics.h"
#endif

void _XL_INIT_GRAPHICS(void)
{
    uint8_t i;
    
    initvideo();
    setvideobase(BASE_ADDR);
    
    disableinterrupt();
    
    #if defined(__TMC600__)
    	textcolor(5);
    #endif
    
    #if defined(__CIDELSA__) || defined(__MICRO__)
        character_set(4);
    #endif
    
    #if defined(__COMX__) || defined(__PECOM__) || defined(__MICRO__) || !defined(_XL_NO_UDG) 
        for(i=0;i<_XL_NUMBER_OF_TILES;++i)
        {
            redefine_char(redefine_map[i]._ascii, redefine_map[i]._bitmap, redefine_map[i]._color);
        }
    #endif
    
    #if defined(__COMX__) || defined(__PECOM__) || defined(__CIDELSA__) || defined(__MICRO__)
        #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
            (void) bgcolor(7);
        #else
            (void) bgcolor(0);
        #endif
    	textcolordefinition(3);
        monochrome(0);
    #endif
    
    #if defined(__PECOM__)
        shapecolor(36, 22, 2); // to reshape the numbers
        shapecolor(65, 26, 3); // to reshape the capitals
    #endif

    _setScreenColors();
}


#if defined(__CIDELSA__)
#include <devkit/video/cidelsa_char.c>
#endif

