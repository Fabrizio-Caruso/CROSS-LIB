
// #define POKE(addr,val)     (*(unsigned char *) (addr) = (val))
// #define POKEW(addr,val)    (*(unsigned int *) (addr) = (val))

// #define SCREEN_BASE (0x0000A000UL)
// #define TEXT_BASE   (0x0000B800UL)

#include <stdlib.h>
#include <stdio.h>

#include "8x8_chars.h"

extern void init_vga(void);
extern void video_mode(void);
// extern void text_mode(void);
extern void cls(void);

// extern void write_tile(uint16_t tiles[][], uint8_t color, uint8_t index, uint16_t x, uint16_t  y);
// extern void write_tile(uint16_t x, uint16_t y, uint16_t index, uint16_t color, uint16_t tile[][]);

extern void plot(uint16_t x, uint16_t y, uint8_t color);

// uint16_t  my_tile[] = {255,255,255,255,255,255,255,255};

uint16_t _i86_tiles[][8] = 
    {
        _TILE_0_UDG,
        _TILE_1_UDG,
        _TILE_2_UDG,
        _TILE_3_UDG,
        _TILE_4_UDG,
        _TILE_5_UDG,
        _TILE_6_UDG,
        _TILE_7_UDG,
        _TILE_8_UDG,
        _TILE_9_UDG,
        _TILE_10_UDG,
        _TILE_11_UDG,
        _TILE_12_UDG,
        _TILE_13_UDG,
        _TILE_14_UDG,
        _TILE_15_UDG,
        _TILE_16_UDG,
        _TILE_17_UDG,
        _TILE_18_UDG,
        _TILE_19_UDG,
        _TILE_20_UDG,
        _TILE_21_UDG,
        _TILE_22_UDG,
        _TILE_23_UDG,
        _TILE_24_UDG,
        _TILE_25_UDG,
        _TILE_26_UDG,
    };


void _display_tile(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
{
    uint8_t row;
    uint8_t col;
    
    for(row=0;row<8;++row)
    {
        for(col=0;col<8;++col)
        {
            if(_i86_tiles[tile][row]&(128>>col))
            {
                plot(x*8+col,8*y+row,color);
            }
            else
            {
                plot(x*8+col,8*y+row,0);
            }
        }
    }
}


void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;
    uint16_t j;

    init_vga();
    video_mode();
    // for(i=0;i<20;++i)
    // {
        // _display_tile(10,i,i,100);
    // }
    
    // for(i=0;i<100;i+=4)
    // {
        // plot(20,i,120);
    // }
    
    // for(i=0;i<100;i+=4)
    // {
        // plot(i,20,140);
    // }
    // write_tile(tiles, 120, 0, 28, 20);
    // write_tile(tiles, 140, 0, 36, 20);
    // write_tile(tiles, 140, 0, 120, 100);
    // write_tile(tiles, 140, 0, 310, 100);
    // write_tile(tiles,  80, 0, 10, 10);

    // write_tile(tiles, 120, 0, 28, 28);
    // write_tile(tiles, 140, 0, 36, 28);
    // for(i=0;i<12;++i)
    // {
        // write_tile(4+i,4+i,0,100+i,tiles);
    // }
    // getchar();
    // exit(0);
}
