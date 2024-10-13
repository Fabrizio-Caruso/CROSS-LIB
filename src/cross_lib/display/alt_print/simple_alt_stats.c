#include "standard_libs.h"

#include "display_macros.h"

#include "memory_mapped_graphics.h"


#if !defined(__CC65__) && !defined(POKE) && !defined(PEEK)
    #define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
    #define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
    #define PEEK(addr)         (*(uint8_t*) (addr))
    #define PEEKW(addr) (*(unsigned*) (addr))
#else
    #include <peekpoke.h>
#endif


#if defined(__VIC20__) && defined(__VIC20_UNEXPANDED) && !defined(_XL_NO_UDG)
    #define UDG_OFFSET 128
#else
    #define UDG_OFFSET 0
#endif

#define _COLOR_ADDR 0x7800

void _displayShort(uint16_t value)
{ 
    uint8_t i; 
    uint8_t old;

    for(i=1;i<6;++i) 
    { 
        old = value%10;
        value -= POKE(BASE_ADDR+6-i,(uint8_t) ((value)%10)); 
        value/=10; 
        // old = value;

        POKE(BASE_ADDR+6-i,UDG_OFFSET+48+old); 
        // POKE(_COLOR_ADDR+BASE_ADDR+6-i,0x1);
    } 
    // POKE(BASE_ADDR+6,48+UDG_OFFSET); 
}

void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
{
    uint8_t i = 0;
    while(str[i]!='\0')
    {
        POKE(loc(x+i,y), 192+str[i]); 
        POKE((_COLOR_ADDR+loc(x+i,y)),0x01);
        ++i;
    }
}	

