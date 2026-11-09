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



// void _XL_PRINTD(register uint8_t x, uint8_t y, uint8_t length, register uint16_t value)
void _displayShort(uint8_t x, register uint16_t value)
{ 
    uint8_t i; 
    uint8_t old;
    uint16_t addr;

    for(i=0;i<5;++i) 
    { 
        old = value%10;
        addr = BASE_ADDR+4+x-i;
        value -= POKE(addr,(uint8_t) ((value)%10));
        value/=10; 
        POKE(addr,UDG_OFFSET+48+old); 
        POKE(_COLOR_ADDR+addr,0x1);
    } 
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

