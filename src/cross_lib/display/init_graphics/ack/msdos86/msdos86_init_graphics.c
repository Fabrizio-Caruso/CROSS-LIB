
// #define POKE(addr,val)     (*(unsigned char *) (addr) = (val))
// #define POKEW(addr,val)    (*(unsigned int *) (addr) = (val))

// #define SCREEN_BASE (0x0000A000UL)
// #define TEXT_BASE   (0x0000B800UL)

#include <stdlib.h>
#include <stdio.h>
extern void init_vga(void);
extern void video_mode(void);
extern void text_mode(void);
extern void cls(void);

extern void write_tile(uint16_t tiles[][], uint16_t color, uint16_t index, uint16_t y, uint16_t  x);
// extern void write_tile(uint16_t x, uint16_t y, uint16_t index, uint16_t color, uint16_t tile[][]);

extern void plot(uint16_t x, uint16_t y, uint8_t color);

uint16_t  my_tile[] = {255,255,255,255,255,255,255,255};

uint16_t tiles[3][8] = 
    {
        {  0,  0,  0,  0,  0,  0,  0,  0},
        {255,  0,255,  0,255,  0,255,  0},
        {255,255,255,255,255,255,255,255}
    };

void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;
    uint16_t j;

    init_vga();
    video_mode();
    for(i=0;i<100;i+=4)
    {
        plot(i,i,100);
    }
    
    for(i=0;i<100;i+=4)
    {
        plot(20,i,120);
    }
    
    for(i=0;i<100;i+=4)
    {
        plot(i,20,140);
    }
    // write_tile(tiles, 120, 0, 28, 20);
    // write_tile(tiles, 140, 0, 36, 20);
    // write_tile(tiles, 140, 0, 120, 100);
    // write_tile(tiles, 140, 0, 310, 100);
    // write_tile(tiles,  80, 0, 10, 10);

    // write_tile(tiles, 120, 0, 28, 28);
    // write_tile(tiles, 140, 0, 36, 28);
    // write_tile(24,20,0,255,tiles);
    getchar();
    exit(0);
}
