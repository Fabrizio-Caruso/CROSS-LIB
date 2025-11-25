
// #define POKE(addr,val)     (*(unsigned char *) (addr) = (val))
// #define POKEW(addr,val)    (*(unsigned int *) (addr) = (val))

// #define SCREEN_BASE (0x0000A000UL)
// #define TEXT_BASE   (0x0000B800UL)

#include <stdlib.h>

extern void init_vga(void);
extern void video_mode(void);
extern void text_mode(void);
extern void cls(void);

// extern void write_tile(uint8_t tile[][], uint8_t color, uint8_t index, uint8_t y, uint8_t  x);
extern void write_tile(uint8_t x, uint8_t y, uint8_t index, uint8_t color, uint8_t tile[][]);

uint8_t  my_tile[] = {255,1,255,2,255,3,255,4};

uint8_t tiles[1][8] = {{255U,1U,255U,2U,255U,3U,255U,4U}};

void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;
    // unsigned char * scr=(unsigned char *) SCREEN_BASE;
    // unsigned char * txt=(unsigned char *) TEXT_BASE;
    init_vga();
    video_mode();
    write_tile(20,20,0,255,tiles);
    while(1){};
}
