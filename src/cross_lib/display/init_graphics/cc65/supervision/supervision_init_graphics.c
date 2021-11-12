#include <supervision.h>
#include "display_macros.h"


void supervision_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
    {
        uint8_t k;
        uint16_t offset = (8*(uint8_t)(tile)) ;
        
        for(k=0;k<8;++k)
        {
            SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)]    = left_map_one_to_two(udgs[offset+k]);
            SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)+1]  = right_map_one_to_two(udgs[offset+k]);
        }
    }



void supervision_delete(uint8_t x, uint8_t y)
{
    uint16_t k;
    for(k=0;k<8;++k)
    {
        SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)]=0;
        SV_VIDEO[2*(x)+BYTES_PER_LINE*k+BYTES_PER_LINE*8*(y)+1]=0;
    }
}


void _XL_INIT_GRAPHICS(void)
{
    SV_LCD.height = 160;
    SV_LCD.width = 160;
    
    setScreenColors();
}

