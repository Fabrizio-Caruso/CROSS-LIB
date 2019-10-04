#define VDP_DATA 0xC000
#define VDP_WRITE 0xC002
#include <peekpoke.h>
    
#define CHAR_BASE 0x0100
#define COLOR_DEF 0x2000    
    
void VDP_REGISTER(unsigned char reg, unsigned char value)
{
    POKE(VDP_DATA,value);
    POKE(VDP_WRITE,(reg|0x80));
}

void VDP_POKE(unsigned short address, unsigned char value)
{
    POKE(VDP_DATA,(address&0x0F));
    POKE(VDP_DATA,(address>>4)|0x40);
    POKE(VDP_WRITE,value);
}

void set_group_color(unsigned char group, unsigned char color)
{
	VDP_POKE(COLOR_DEF+group, color<<4);
}

void debug(unsigned char base, unsigned char value)
{
    unsigned short i;
    
    for(i=base;i<base+0x1000;++i)
    {
        VDP_POKE(i,value);
    }
}

void INIT_GRAPHICS(void)
{
    debug(0x1000,0xAA);
    debug(0x2000,0xA0);
    debug(0x3000,0x0A);
    
	set_group_color(0,9);
	set_group_color(1,7);
	set_group_color(2,9);
	set_group_color(3,11);
	set_group_color(4,10);
	set_group_color(5,2);
	set_group_color(6,4);
	set_group_color(7,4);  
}