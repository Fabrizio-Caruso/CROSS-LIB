#include <devkit/video/vis_video.h>
#include <devkit/system/system.h>

#if defined(REDEFINED_CHARS)
    #include "6x9_chars.h"
#endif

#include "standard_libs.h"

#if defined(__CIDELSA__)
    #include <devkit/video/cidelsa_char.h>
#endif

#if defined(__MICRO__)
    #include "devkit/video/vis_char.h"
#endif


#if defined(__COMX__) || defined(__PECOM__) || defined(__MICRO__) || defined(REDEFINED_CHARS)
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


    const uint8_t tile_10_udg[9] =  _TILE_10_UDG;
    const uint8_t tile_0_udg[9] = _TILE_0_UDG;
    const uint8_t tile_1_udg[9] = _TILE_1_UDG;
    const uint8_t tile_2_udg[9] = _TILE_2_UDG;
    const uint8_t tile_3_udg[9] = _TILE_3_UDG;
    const uint8_t tile_5_udg[9] =  _TILE_5_UDG;
    const uint8_t tile_4_udg[9] = _TILE_4_UDG;
    const uint8_t tile_7_udg[9] = _TILE_7_UDG;
    const uint8_t tile_8_udg[9] = _TILE_8_UDG;
    const uint8_t tile_9_udg[9] =  _TILE_9_UDG;
    const uint8_t tile_11_udg[9] =  _TILE_11_UDG;
    const uint8_t tile_6_udg[9] = _TILE_6_UDG;
    const uint8_t tile_16_udg[9] = _TILE_16_UDG;
    const uint8_t tile_14_udg[9] = _TILE_14_UDG;
    const uint8_t tile_15_udg[9] =  _TILE_15_UDG;
    const uint8_t tile_12_udg[9] =  _TILE_12_UDG;
    const uint8_t tile_13_udg[9] = _TILE_13_UDG;
    const uint8_t tile_17_udg[9] = _TILE_17_UDG;
    const uint8_t tile_18_udg[9] = _TILE_18_UDG;


#endif

#include "memory_mapped_graphics.h"

void INIT_GRAPHICS(void)
{
    initvideo();
    setvideobase(BASE_ADDR);
    
    #if defined(__TMC600__)
    	textcolor(5);
    #endif
    
    #if defined(__CIDELSA__) || defined(__MICRO__)
        character_set(4);
    #endif
    
    #if defined(__COMX__) || defined(__PECOM__) || defined(__MICRO__) || defined(REDEFINED_CHARS) 
        redefine_char(106, tile_10_udg, 3);
        redefine_char(107, tile_0_udg, 3);
        redefine_char(108, tile_1_udg, 3);
        redefine_char(109, tile_2_udg, 3);
        redefine_char(110, tile_3_udg, 3);
        redefine_char(111, tile_5_udg, 0);
        redefine_char(112, tile_4_udg, 3);

        redefine_char(113, tile_7_udg, 2);
        redefine_char(114, tile_8_udg, 2);
        redefine_char(115, tile_9_udg, 1);
        redefine_char(116, tile_11_udg, 3);
        redefine_char(117, tile_6_udg, 0);

        redefine_char(118, tile_16_udg, 3);
        redefine_char(119, tile_14_udg, 3);
        redefine_char(120, tile_15_udg, 3);
        redefine_char(121, tile_12_udg, 2);
        redefine_char(122, tile_13_udg, 2);
        redefine_char(123, tile_17_udg, 3);
        redefine_char(124, tile_18_udg, 0);
    #endif
    #if defined(__COMX__) || defined(__PECOM__) || defined(__CIDELSA__) || defined(__MICRO__)
        disableinterrupt();
    
        (void) bgcolor(0);

    	textcolordefinition(3);
        monochrome(0);
    #endif
    
    #if defined(__PECOM__)
        shapecolor(36, 22, 2); // to reshape the numbers
        shapecolor(65, 26, 3); // to reshape the capitals
    #endif

}

#if defined(__CIDELSA__)
#include <devkit/video/cidelsa_char.c>
#endif

