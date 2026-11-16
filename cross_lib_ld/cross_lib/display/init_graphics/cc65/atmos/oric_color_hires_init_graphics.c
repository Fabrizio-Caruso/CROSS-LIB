#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "graphics_settings.h"
#include "display_macros.h"

#include "6x8_chars.h"

#define BASE_ADDR 0xA000

#define TOTAL_TILES ((_XL_NUMBER_OF_TILES)+26+10+1)

uint8_t _oric_text_color;

uint8_t __left_12x16_tiles[TOTAL_TILES][8];
uint8_t __right_12x16_tiles[TOTAL_TILES][8];


uint8_t __oric__6x8_tiles[TOTAL_TILES][8] =
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
    _SPACE_TILE
};



// (0, 0)
// (0, 3)
// (0, 12)
// (0, 15)
// (0, 48)
// (0, 51)
// (0, 60)
// (0, 63)
uint8_t DOUBLE_3_BIT_MAP[8] = {0,3,12,15,48,51,60,63};


void compute_12x16_tiles(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<TOTAL_TILES;++i)
        {
            for(j=0;j<8;++j)
            {
                __left_12x16_tiles[i][j]  =  DOUBLE_3_BIT_MAP[__oric__6x8_tiles[i][j]>>3];
                __right_12x16_tiles[i][j] =  DOUBLE_3_BIT_MAP[__oric__6x8_tiles[i][j]&0b00000111];
            }
        }
}



void preprocess_tiles(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<TOTAL_TILES;++i)
    {
        for(j=0;j<8;++j)
        {
            __left_12x16_tiles[i][j]|=64;
            __right_12x16_tiles[i][j]|=64;
        }
    }
}


#if !defined(Force_YSize)
    #if !defined(__INVERSE_TILES)
        void _oric_hires_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color, uint8_t inverse)
        {
            uint8_t i;
            uint16_t x_offset;

            if(inverse)
            {
                if(color==_XL_WHITE)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                    {

                        POKE(x_offset  ,__left_12x16_tiles[tile][i/2]^63|0x80); 
                        POKE(x_offset+1,__right_12x16_tiles[tile][i/2]^63|0x80);

                        POKE(x_offset+40  ,64);
                        POKE(x_offset+40+1,64);
                    }
                    return;
                }
                else if(color==_XL_RED)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                    {

                        POKE(x_offset  ,64);
                        POKE(x_offset+1,64);
                    
                        POKE(x_offset+40  ,(__left_12x16_tiles[tile][i/2])|0x80);
                        POKE(x_offset+40+1,(__right_12x16_tiles[tile][i/2])|0x80);

                    }
                    return;
                }
                else
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                    {
                        if(color!=_XL_CYAN)
                        {

                            POKE(x_offset  ,__left_12x16_tiles[tile][i/2]);
                            POKE(x_offset+1,__right_12x16_tiles[tile][i/2]);


                        }
                        else
                        {
                            POKE(x_offset  ,64);
                            POKE(x_offset+1,64);
                        }
                        
                        if(color!=_XL_YELLOW)
                        {

                            POKE(x_offset+40  ,__left_12x16_tiles[tile][i/2]);
                            POKE(x_offset+40+1,__right_12x16_tiles[tile][i/2]);

                        }
                        else
                        {
                            POKE(x_offset+40  ,64);
                            POKE(x_offset+40+1,64);
                        }
                    }
                }
            }
            else
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                {
                    if(color!=_XL_CYAN)
                    {
                        POKE(x_offset  ,__left_12x16_tiles[tile][i/2]);
                        POKE(x_offset+1,__right_12x16_tiles[tile][i/2]);
                    }
                    else
                    {
                        POKE(x_offset  ,64);
                        POKE(x_offset+1,64);
                    }
                    
                    if(color!=_XL_YELLOW)
                    {
                        POKE(x_offset+40  ,__left_12x16_tiles[tile][i/2]);
                        POKE(x_offset+40+1,__right_12x16_tiles[tile][i/2]);
                    }
                    else
                    {
                        POKE(x_offset+40  ,64);
                        POKE(x_offset+40+1,64);
                    }
                }
            }
        }
    #else
        void _oric_hires_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
        {
            uint8_t i;
            uint16_t x_offset;

            if(color==_XL_WHITE)
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                {

                if((__left_12x16_tiles[tile][i/2]&63) || (__right_12x16_tiles[tile][i/2]&63)) // limit reverse artifact 
                {
                    POKE(x_offset  ,__left_12x16_tiles[tile][i/2]^63|0x80); 
                    POKE(x_offset+1,__right_12x16_tiles[tile][i/2]^63|0x80);
                }
                else
                {
                    POKE(x_offset  ,64);
                    POKE(x_offset+1,64);
                }

                POKE(x_offset+40  ,64);
                POKE(x_offset+40+1,64);
                }
                return;
            }
            else if(color==_XL_RED)
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80) // limit reverse artifact 
                {

                    POKE(x_offset  ,64);
                    POKE(x_offset+1,64);
                
                    if((__left_12x16_tiles[tile][i/2]&63) || (__right_12x16_tiles[tile][i/2]&63))
                    {
                        POKE(x_offset+40  ,(__left_12x16_tiles[tile][i/2])|0x80);
                        POKE(x_offset+40+1,(__right_12x16_tiles[tile][i/2])|0x80);
                    }
                    else
                    {
                        POKE(x_offset+40  ,64);
                        POKE(x_offset+40+1,64);
                    }
                }
                return;
            }
            else
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                {
                    if(color!=_XL_CYAN)
                    {
                        POKE(x_offset  ,__left_12x16_tiles[tile][i/2]);
                        POKE(x_offset+1,__right_12x16_tiles[tile][i/2]);
                    }
                    else
                    {
                        POKE(x_offset  ,64);
                        POKE(x_offset+1,64);
                    }
                    
                    if(color!=_XL_YELLOW)
                    {
                        POKE(x_offset+40  ,__left_12x16_tiles[tile][i/2]);
                        POKE(x_offset+40+1,__right_12x16_tiles[tile][i/2]);
                    }
                    else
                    {
                        POKE(x_offset+40  ,64);
                        POKE(x_offset+40+1,64);
                    }
                }
            }
        }
    #endif
#else // Bottom line is special here
    #if !defined(__INVERSE_TILES)
        void _oric_hires_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color, uint8_t inverse)
        {
            uint8_t i;
            uint16_t x_offset;

            if(y!=YSize-1)
            {
                if(inverse)
                {
                    if(color==_XL_WHITE)
                    {
                        for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                        {

                            POKE(x_offset  ,__left_12x16_tiles[tile][i/2]^63|0x80); 
                            POKE(x_offset+1,__right_12x16_tiles[tile][i/2]^63|0x80);

                            POKE(x_offset+40  ,64);
                            POKE(x_offset+40+1,64);
                        }
                        return;
                    }
                    else if(color==_XL_RED)
                    {
                        for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                        {

                            POKE(x_offset  ,64);
                            POKE(x_offset+1,64);
                        
                            POKE(x_offset+40  ,(__left_12x16_tiles[tile][i/2])|0x80);
                            POKE(x_offset+40+1,(__right_12x16_tiles[tile][i/2])|0x80);
                        }
                        return;
                    }
                    else
                    {
                        for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                        {
                            if(color!=_XL_CYAN)
                            {

                                POKE(x_offset  ,__left_12x16_tiles[tile][i/2]);
                                POKE(x_offset+1,__right_12x16_tiles[tile][i/2]);


                            }
                            else
                            {
                                POKE(x_offset  ,64);
                                POKE(x_offset+1,64);
                            }
                            
                            if(color!=_XL_YELLOW)
                            {

                                POKE(x_offset+40  ,__left_12x16_tiles[tile][i/2]);
                                POKE(x_offset+40+1,__right_12x16_tiles[tile][i/2]);

                            }
                            else
                            {
                                POKE(x_offset+40  ,64);
                                POKE(x_offset+40+1,64);
                            }
                        }
                    }
                }
                else
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                    {
                        if(color!=_XL_CYAN)
                        {
                            POKE(x_offset  ,__left_12x16_tiles[tile][i/2]);
                            POKE(x_offset+1,__right_12x16_tiles[tile][i/2]);
                        }
                        else
                        {
                            POKE(x_offset  ,64);
                            POKE(x_offset+1,64);
                        }
                        
                        if(color!=_XL_YELLOW)
                        {
                            POKE(x_offset+40  ,__left_12x16_tiles[tile][i/2]);
                            POKE(x_offset+40+1,__right_12x16_tiles[tile][i/2]);
                        }
                        else
                        {
                            POKE(x_offset+40  ,64);
                            POKE(x_offset+40+1,64);
                        }
                    }
                }
            }
            else // Bottom line
            {
                // if(color==_XL_WHITE)
                // {
                    // for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    // {

                                // POKE(x_offset  ,__left_12x16_tiles[tile][i]^63|0x80);
                                // POKE(x_offset+1,__right_12x16_tiles[tile][i]);
                    // }
                // }
                if(color==_XL_RED)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    {
                        if(__left_12x16_tiles[tile][i]&63 || __right_12x16_tiles[tile][i]&63)
                        {
                            if(i&1)
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]|0x80);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]|0x80);
                            }
                            else
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]^63);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]^63);
                            }
                        }
                        else
                        {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]);
                        }
                    
                    }
                }
                else if(color==_XL_CYAN)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    {
                        if(__left_12x16_tiles[tile][i]&63 || __right_12x16_tiles[tile][i]&63)
                        {
                            if(i&1)
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]^63);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]^63);
                            }
                            else
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]|0x80);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]|0x80);
                            }
                        }
                        else
                        {
                            POKE(x_offset  ,64);
                            POKE(x_offset+1,64);
                        }
                    }
                }
                else if(color==_XL_YELLOW)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    {
                        if(__left_12x16_tiles[tile][i]&63 || __right_12x16_tiles[tile][i]&63)
                        {
                            if(i&1)
                            {
                                        POKE(x_offset  ,__left_12x16_tiles[tile][i]^63|0x80);
                                        POKE(x_offset+1,__right_12x16_tiles[tile][i]^63|0x80);
                            }
                            else
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]);
                            }
                        }
                        else
                        {
                            POKE(x_offset  ,64);
                            POKE(x_offset+1,64);
                        }
                    }
                }
                else
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    {

                                POKE(x_offset  ,__left_12x16_tiles[tile][i]);
                                POKE(x_offset+1,__right_12x16_tiles[tile][i]);
                    }
                }
            }
        }
    #else // INVERSE TILES
        void _oric_hires_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
        {
            uint8_t i;
            uint16_t x_offset;

            if(y!=YSize-1)
            {
                if(color==_XL_WHITE)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                    {

                        if((__left_12x16_tiles[tile][i/2]&63) || (__right_12x16_tiles[tile][i/2]&63)) // limit reverse artifact 
                        {
                            POKE(x_offset  ,__left_12x16_tiles[tile][i/2]^63|0x80); 
                            POKE(x_offset+1,__right_12x16_tiles[tile][i/2]^63|0x80);
                        }
                        else
                        {
                            POKE(x_offset  ,64);
                            POKE(x_offset+1,64);
                        };


                        POKE(x_offset+40  ,64);
                        POKE(x_offset+40+1,64);
                    }
                    return;
                }
                else if(color==_XL_RED)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80) // limit reverse artifact 
                    {

                        POKE(x_offset  ,64);
                        POKE(x_offset+1,64);
                    

                        if((__left_12x16_tiles[tile][i/2]&63) || (__right_12x16_tiles[tile][i/2]&63))
                        {
                            POKE(x_offset+40  ,(__left_12x16_tiles[tile][i/2])|0x80);
                            POKE(x_offset+40+1,(__right_12x16_tiles[tile][i/2])|0x80);
                        }
                        else
                        {
                            POKE(x_offset+40  ,64);
                            POKE(x_offset+40+1,64);
                        }
                    }
                    return;
                }
                else
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<2*8;i+=2, x_offset+=80)
                    {
                        if(color!=_XL_CYAN)
                        {
                            POKE(x_offset  ,__left_12x16_tiles[tile][i/2]);
                            POKE(x_offset+1,__right_12x16_tiles[tile][i/2]);
                        }
                        else
                        {
                            POKE(x_offset  ,64);
                            POKE(x_offset+1,64);
                        }
                        

                        if(color!=_XL_YELLOW)
                        {
                            POKE(x_offset+40  ,__left_12x16_tiles[tile][i/2]);
                            POKE(x_offset+40+1,__right_12x16_tiles[tile][i/2]);
                        }
                        else
                        {
                            POKE(x_offset+40  ,64);
                            POKE(x_offset+40+1,64);
                        }
                    }
                }
            }
            else // Bottom line
            {
                // if(color==_XL_WHITE)
                // {
                    // for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    // {

                                // POKE(x_offset  ,__left_12x16_tiles[tile][i]^63|0x80);
                                // POKE(x_offset+1,__right_12x16_tiles[tile][i]);
                    // }
                // }
                if(color==_XL_RED)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    {
                        if(__left_12x16_tiles[tile][i]&63 || __right_12x16_tiles[tile][i]&63)
                        {
                            if(i&1)
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]|0x80);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]|0x80);
                            }
                            else
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]^63);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]^63);
                            }
                        }
                        else
                        {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]);
                        }
                    
                    }
                }
                else if(color==_XL_CYAN)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    {
                        if(__left_12x16_tiles[tile][i]&63 || __right_12x16_tiles[tile][i]&63)
                        {
                            if(i&1)
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]^63);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]^63);
                            }
                            else
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]|0x80);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]|0x80);
                            }
                        }
                        else
                        {
                            POKE(x_offset  ,64);
                            POKE(x_offset+1,64);
                        }
                    }
                }
                else if(color==_XL_YELLOW )
                {
                   for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    {
                        if(__left_12x16_tiles[tile][i]&63 || __right_12x16_tiles[tile][i]&63)
                        {
                            if(i&1)
                            {
                                        POKE(x_offset  ,__left_12x16_tiles[tile][i]^63|0x80);
                                        POKE(x_offset+1,__right_12x16_tiles[tile][i]^63|0x80);
                            }
                            else
                            {
                                    POKE(x_offset  ,__left_12x16_tiles[tile][i]);
                                    POKE(x_offset+1,__right_12x16_tiles[tile][i]);
                            }
                        }
                        else
                        {
                            POKE(x_offset  ,64);
                            POKE(x_offset+1,64);
                        }
                    }
                }
                else
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*12; i<8;++i, x_offset+=40)
                    {

                                POKE(x_offset  ,__left_12x16_tiles[tile][i]);
                                POKE(x_offset+1,__right_12x16_tiles[tile][i]);
                    }
                }
            }
        }
    #endif
#endif


#if !defined(Force_YSize)
    void _oric_hires_delete(uint8_t x, uint8_t y)
    {
        uint8_t i;
        uint16_t x_offset;
        
        for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<8;++i, x_offset+=80)
        {
            POKE(x_offset  ,64);
            POKE(x_offset+1,64);
            
            POKE(x_offset+40  ,64);
            POKE(x_offset+40+1,64);
        }
    }
#else
    void _oric_hires_delete(uint8_t x, uint8_t y)
    {
        uint8_t i;
        uint16_t x_offset;
        
        if(y!=YSize-1)
        {
            for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<8;++i, x_offset+=80)
            {
                POKE(x_offset  ,64);
                POKE(x_offset+1,64);
                
                POKE(x_offset+40  ,64);
                POKE(x_offset+40+1,64);
            }
        }
        else
        {
            for(i=0,x_offset = BASE_ADDR+X_OFFSET+2*x+320U*(uint16_t)2*y; i<8;++i, x_offset+=40)
            {
                POKE(x_offset  ,64);
                POKE(x_offset+1,64);

            }
        }
    }
#endif


void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;
    uint8_t c;
    
    compute_12x16_tiles();
    preprocess_tiles();

    for(i=0;i<8192;++i)
    {
        POKE(BASE_ADDR+i,16);
    };
    
    // POKE(0x26A,2);
    POKE(0xBFDF ,0x1E); // Set screen to hires

    for(c=0,i=0;i<40*200;i+=40,++c)
    {
       POKE(BASE_ADDR+i,3+3*(c&1));
    }
}

