
#include "standard_libs.h"
#include "8x6_c_definition.h"


#define POKE(addr,val) (*(uint8_t*) (addr) = (val))

const uint8_t udgs[] =
{
    _TILE_0_UDG,  _TILE_1_UDG,  _TILE_2_UDG,  _TILE_4_UDG, 
    _TILE_5_UDG,  _TILE_6_UDG,  _TILE_7_UDG,  _TILE_8_UDG, 
    _TILE_9_UDG,  _TILE_10_UDG, _TILE_11_UDG, _TILE_12_UDG, 
    _TILE_13_UDG, _TILE_14_UDG, _TILE_15_UDG, _TILE_16_UDG, 
    _TILE_17_UDG, _TILE_18_UDG, _TILE_19_UDG, _TILE_20_UDG, 
    _TILE_21_UDG, _TILE_22_UDG, _TILE_23_UDG, _TILE_24_UDG, 
    _TILE_25_UDG, _TILE_26_UDG,    
};

void _XL_INIT_GRAPHICS(void)
{
    // uint16_t i;
    // uint8_t j;
    // uint8_t c;
    
    __asm__("sei"   );
    POKE(49151U,36); 
    
    // for(j=0;j<6 ;++j)
    // {
        // if((j&3)==0)
        // {
            // c=0xFF;
        // }
        // else if((j&3)==1)
        // {
            // c=0xAA;
        // }
        // else if((j&3)==2)
        // {
            // c=0x55;
        // }
        // else
        // {
            // c=0;
        // }
        // for(i=0;i<6000;++i)
        // {
            // POKE(0x4000+i,c);
        // }
    // }
}
