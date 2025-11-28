
// #define POKE(addr,val)     (*(unsigned char *) (addr) = (val))
// #define POKEW(addr,val)    (*(unsigned int *) (addr) = (val))

// #define SCREEN_BASE (0x0000A000UL)
// #define TEXT_BASE   (0x0000B800UL)

#include <stdlib.h>
#include <stdio.h>

#include "8x8_chars.h"

extern void _init_vga(void);
extern void _video_mode(void);

extern void plot(uint16_t x, uint16_t y, uint8_t color);

uint8_t _vga_text_color;

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
        _TILE_A,
        _TILE_B,
        _TILE_C,
        _TILE_D,
        _TILE_E,
        _TILE_F,
        _TILE_G,
        _TILE_H,
        _TILE_I,
        _TILE_J,
        _TILE_K,
        _TILE_L,
        _TILE_M,
        _TILE_N,
        _TILE_O,
        _TILE_P,
        _TILE_Q,
        _TILE_R,
        _TILE_S,
        _TILE_T,
        _TILE_U,
        _TILE_V,
        _TILE_W,
        _TILE_X,
        _TILE_Y,
        _TILE_Z,
        _TILE_DIGIT_0,
        _TILE_DIGIT_1,
        _TILE_DIGIT_2,
        _TILE_DIGIT_3,
        _TILE_DIGIT_4,
        _TILE_DIGIT_5,
        _TILE_DIGIT_6,
        _TILE_DIGIT_7,
        _TILE_DIGIT_8,
        _TILE_DIGIT_9,
        _SPACE_TILE,
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


void _delete_tile(uint8_t x, uint8_t y)
{
    uint8_t row;
    uint8_t col;
    
    for(row=0;row<8;++row)
    {
        for(col=0;col<8;++col)
        {
            plot(x*8+col,8*y+row,0);
        }
    }
}



void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;
    uint16_t j;

    _init_vga();
    _video_mode();
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
