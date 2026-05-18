
#include <stdlib.h>
#include <stdio.h>

#include "8x8_chars.h"

extern void _init_vga(void);
extern void _video_mode(void);
extern void _delete_vga_tile(uint16_t x, uint16_t y);
extern void _delete_vga_segment(uint16_t x, uint16_t y);
extern void _delete_vga_pixel(uint16_t x, uint16_t y);
extern void _speaker_beep(uint16_t freq, uint16_t length);

extern void _keyboard_init(void);

extern void _plot_vga(uint16_t x, uint16_t y, uint8_t color);

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
        _SPACE_TILE,
    };


void _display_tile(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
{
    uint8_t row;
    uint8_t col;
    uint16_t row_pos;
    uint16_t col_pos;
    uint8_t tile_row;
    
    row_pos = 8*(uint16_t)y;
    for(row=0;row<8;++row)
    {
        tile_row = _i86_tiles[tile][row];
        col_pos = 8*(uint16_t)x;
        for(col=0;col<8;++col)
        {
            if(tile_row&(128>>col))
            {
                _plot_vga(col_pos,row_pos,color);
            }
            else
            {
                _delete_vga_pixel(col_pos,row_pos);
            }
            ++col_pos;
        }
        ++row_pos;
    }
}


void _XL_INIT_GRAPHICS(void)
{
    _init_vga();
    _keyboard_init();
    _speaker_beep(100,100);
}
