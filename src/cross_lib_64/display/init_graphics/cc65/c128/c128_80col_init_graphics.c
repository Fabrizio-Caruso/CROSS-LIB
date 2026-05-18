#include <c128.h>
#include "display_macros.h"
#include <peekpoke.h>

#include "8x8_chars.h"

#include "udg_map.h"

#define ADDRESS_PORT 0xD600
#define DATA_PORT    0xD601

#define HIGH_ADDRESS_REGISTER 0x12
#define LOW_ADDRESS_REGISTER  0x13
#define VDC_DATA_REGISTER     0x1F

#define CHAR_BASE    0x3000

void vdc_write(uint8_t vdc_register, uint8_t value)
{
    POKE(ADDRESS_PORT,vdc_register);
    while(!(PEEK(ADDRESS_PORT)&(0x80))){};
    POKE(DATA_PORT,value);
}

void redefine(uint8_t tile, const uint8_t definition[])
{
    uint8_t i;
    uint16_t address = CHAR_BASE + (((uint16_t)tile)<<4U);
    vdc_write(HIGH_ADDRESS_REGISTER,(uint8_t)(address>>8));
    vdc_write(LOW_ADDRESS_REGISTER,(uint8_t)(address&0x00FF));
    for(i=0;i<8;++i)
    {
        vdc_write(VDC_DATA_REGISTER,definition[i]);
    }
}

void set_udg(void)
{
    uint8_t i;
    
    for (i = 0; i < (sizeof(redefine_map)/sizeof(*redefine_map)); ++i)
    {
        redefine(redefine_map[i].ascii,redefine_map[i].bitmap);
    }
}


// #define DEBUG

void _XL_INIT_GRAPHICS(void)
{
    fast();
    videomode(80);
    
    #if !defined(DEBUG)

    set_udg();

    _setScreenColors();

    #else
    
    vdc_write(HIGH_ADDRESS_REGISTER,0x00);
    vdc_write(LOW_ADDRESS_REGISTER,0x00);
    {
        uint8_t i;

        for(i=0;i<255;++i)
        {
            vdc_write(VDC_DATA_REGISTER,i);
        };
    }
    // sleep(2);
    set_udg();
    // sleep(2);
    while(1){};

    _setScreenColors();
    #endif
}

