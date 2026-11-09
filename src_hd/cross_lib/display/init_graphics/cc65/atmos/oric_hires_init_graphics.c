#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "graphics_settings.h"
#include "display_macros.h"

#include "6x8_chars.h"

#define BASE_ADDR 0xA000

#if !defined(_XL_NO_COLOR) && !defined(_XL_NO_TEXT_COLOR)
uint8_t _oric_text_color;
#endif

#define TOTAL_TILES ((_XL_NUMBER_OF_TILES)+26+10+1)
uint8_t __oric__tiles[TOTAL_TILES][8] =
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
    _TILE_27_UDG,
    _TILE_28_UDG,
    _TILE_29_UDG,
    _TILE_30_UDG,
    _TILE_31_UDG,
    _TILE_32_UDG,
    _TILE_33_UDG,
    _TILE_34_UDG,
    _TILE_35_UDG,
    _TILE_36_UDG,
    _TILE_37_UDG,
    _TILE_38_UDG,
    _TILE_39_UDG,
    _TILE_40_UDG,
    _TILE_41_UDG,
    _TILE_42_UDG,
    _TILE_43_UDG,
    _TILE_44_UDG,
    _TILE_45_UDG,
    _TILE_46_UDG,
    _TILE_47_UDG,
    _TILE_48_UDG,
    _TILE_49_UDG,
    _TILE_50_UDG,
    _TILE_51_UDG,
    _TILE_52_UDG,
    _TILE_53_UDG,
    _TILE_54_UDG,
    _TILE_55_UDG,
    _TILE_56_UDG,
    _TILE_57_UDG,
    _TILE_58_UDG,
    _TILE_59_UDG,
    _TILE_60_UDG,
    _TILE_61_UDG,
    _TILE_62_UDG,
    _TILE_63_UDG,
    _TILE_64_UDG,
    _TILE_65_UDG,
    _TILE_66_UDG,
    _TILE_67_UDG,
    _TILE_68_UDG,
    _TILE_69_UDG,
    _TILE_70_UDG,
    _TILE_71_UDG,
    _TILE_72_UDG,
    _TILE_73_UDG,
    _TILE_74_UDG,
    _TILE_75_UDG,
    _TILE_76_UDG,
    _TILE_77_UDG,
    _TILE_78_UDG,
    _TILE_79_UDG,
    _TILE_80_UDG,
    _TILE_81_UDG,
    _TILE_82_UDG,
    _TILE_83_UDG,
    _TILE_84_UDG,
    _TILE_85_UDG,
    _TILE_86_UDG,
    _TILE_87_UDG,
    _TILE_88_UDG,
    _TILE_89_UDG,
    _TILE_90_UDG,
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


void preprocess_tiles(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<TOTAL_TILES;++i)
    {
        for(j=0;j<8;++j)
        {
            __oric__tiles[i][j]|=64;
        }
    }
}


#if defined(_XL_NO_COLOR)
    void __oric_hires_draw_no_color(uint8_t x, uint8_t y, uint8_t tile)
    {
        uint8_t i;
        uint16_t x_offset;
        
        for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
        {
            POKE(x_offset,__oric__tiles[tile][i]);
        }
    }
#else
    
    #if !defined(__INVERSE_TILES)
        void _oric_hires_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color, uint8_t inverse)
        {
            uint8_t i;
            uint16_t x_offset;
            
            if(inverse)
            {
                if(color==_XL_RED)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                    {
                        if(__oric__tiles[tile][i]&63)
                        {
                            POKE(x_offset,__oric__tiles[tile][i]|0x80);
                        }
                        else
                        {
                            POKE(x_offset,64);
                        }
                    }
                }
                else if (color==_XL_WHITE)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                    {
                        if(__oric__tiles[tile][i]&63)
                        {
                            POKE(x_offset,__oric__tiles[tile][i]^63|0x80);
                        }
                        else
                        {
                            POKE(x_offset,64);
                        }
                    }
                }
                else if (color==_XL_YELLOW)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                    {
                        if(__oric__tiles[tile][i]&63)
                        {
                            if((i&1))
                            {
                                POKE(x_offset,__oric__tiles[tile][i]^63|0x80);
                            }
                            else
                            {
                                POKE(x_offset,__oric__tiles[tile][i]);
                            }
                        }
                        else
                        {
                            POKE(x_offset,64);
                        }
                    }
                }
                else if (color==_XL_CYAN)
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                    {
                        if(__oric__tiles[tile][i]&63)
                        {
                            if(i&1)
                            {
                                POKE(x_offset,((__oric__tiles[tile][i]^63)));
                            }
                            else
                            {
                                POKE(x_offset,((__oric__tiles[tile][i])|0x80));
                            }
                        }
                        else
                        {
                            POKE(x_offset,64);
                        }
                    }
                }
                else
                {
                    for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                    {
                        POKE(x_offset,__oric__tiles[tile][i]);
                    }
                }
            }
            else
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                {
                    POKE(x_offset,__oric__tiles[tile][i]);
                }
            }
        }
    #else
        void _oric_hires_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
        {
            uint8_t i;
            uint16_t x_offset;
            
            if(color==_XL_RED)
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                {
                    if(__oric__tiles[tile][i]&63)
                    {
                        POKE(x_offset,__oric__tiles[tile][i]|0x80);
                    }
                    else
                    {
                        POKE(x_offset,64);
                    }
                }
            }
            else if (color==_XL_WHITE)
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                {
                    if(__oric__tiles[tile][i]&63)
                    {
                        POKE(x_offset,__oric__tiles[tile][i]^63|0x80);
                    }
                    else
                    {
                        POKE(x_offset,64);
                    }
                }
            }
            else if (color==_XL_YELLOW)
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                {
                    if(__oric__tiles[tile][i]&63)
                    {
                        if((i&1))
                        {
                            POKE(x_offset,__oric__tiles[tile][i]^63|0x80);
                        }
                        else
                        {
                            POKE(x_offset,__oric__tiles[tile][i]);
                        }
                    }
                    else
                    {
                        POKE(x_offset,64);
                    }
                }
            }
            else if (color==_XL_CYAN)
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                {
                    if(__oric__tiles[tile][i]&63)
                    {
                        if(i&1)
                        {
                            POKE(x_offset,((__oric__tiles[tile][i]^63)));
                        }
                        else
                        {
                            POKE(x_offset,((__oric__tiles[tile][i])|0x80));
                        }
                    }
                    else
                    {
                        POKE(x_offset,64);
                    }
                }
            }
            else
            {
                for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
                {
                    POKE(x_offset,__oric__tiles[tile][i]);
                }
            }
        }
    #endif

#endif

void _oric_hires_delete(uint8_t x, uint8_t y)
{
    uint8_t i;
    uint16_t x_offset;
    
    for(i=0,x_offset = BASE_ADDR+X_OFFSET+x+320U*(uint16_t)y; i<8;++i, x_offset+=40)
    {
        POKE(x_offset,64);
    }
}


#if defined(_XL_NO_COLOR)
void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;
    preprocess_tiles();

    for(i=0;i<8192;++i)
    {
        POKE(BASE_ADDR+i,16);
    };
    
    POKE(0xBFDF ,0x1E); // Set screen to hires
}
#else
void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;
    uint8_t c;
    preprocess_tiles();

    for(i=0;i<8192;++i)
    {
        POKE(BASE_ADDR+i,16);
    };
    
    POKE(0xBFDF ,0x1E); // Set screen to hires

    for(c=0,i=0;i<40*200;i+=40,++c)
    {
       POKE(BASE_ADDR+i,3+3*(c&1));
        // POKE(BASE_ADDR+i,3);

    }
}
#endif