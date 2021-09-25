#include<coco.h>
#include<cmoc.h>

#define POKE(addr,value) (*(unsigned char *)(addr)=(value))
#define PEEK(addr) (*(unsigned char *)(addr))

int main(void)
{
    uint16_t i;

    disableInterrupts();
    
    // Enable MMU
    POKE(0xFF90,0x40);

    // Set graphics ON
    POKE(0xFF98,0x80);

    // Set 256x192x16 mode
    POKE(0xFF99,0x1A);

    // Set actual screen ram at $600000
    POKE(0xFF9D,0xC0);
    POKE(0xFF9E,0x00);

    // Set palette   
    for(i=0;i<16;++i)
    {
        POKE(0xFFB0+i,(unsigned char) i&0xFF);
    }
    
    // Set MMU windows at $8000
    POKE(0xFFA4,0x30);
    POKE(0xFFA5,0x31);
    POKE(0xFFA6,0x32);

    for(i=0;i<24000;++i)
    {
        POKE(0x8000+i,(unsigned char) 255);
    }
    
    while(1){};
    
    return 0;
}
