// #if defined(__ATARI_ST__)
    // #include <ncurses/curses.h>
// #else
    #include <ncurses.h>
// #endif

#include "display_macros.h"


#if _XL_TILE_X_SIZE==8
    #define _MAX_BIT_VALUE 128

    #if _XL_TILE_Y_SIZE==8
        #include "8x8_chars.h"

    #elif _XL_TILE_Y_SIZE==6
        #include "8x6_chars.h"
    #endif
#elif _XL_TILE_X_SIZE==7
    #define _MAX_BIT_VALUE 64

    #include "7x8_chars.h"

#elif _XL_TILE_X_SIZE==6
    #define _MAX_BIT_VALUE 32

    #if _XL_TILE_Y_SIZE==8
        #include "6x8_chars.h"
        
    #elif _XL_TILE_Y_SIZE==9
        #include "6x9_chars.h"
    #endif
#endif

#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
    #define _NCURSES_BACKGROUND_COLOR COLOR_WHITE
#else
    #define _NCURSES_BACKGROUND_COLOR COLOR_BLACK
#endif

#if _XL_TILE_Y_SIZE==9
    #define _SPACE_ {0,0,0,0,0,0,0,0,0}
#elif _XL_TILE_Y_SIZE==8
    #define _SPACE_ {0,0,0,0,0,0,0,0}
#elif _XL_TILE_Y_SIZE==6
    #define _SPACE_ {0,0,0,0,0,0}
#endif

uint8_t _terminal_text_color;

uint8_t _tiles[][_XL_TILE_Y_SIZE] =
{
    _SPACE_,
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
    _TILE_0_UDG,
    _TILE_1_UDG,
    _TILE_2_UDG,
    _TILE_3_UDG,
    _TILE_4_UDG,
    _TILE_5_UDG,
    _TILE_6_UDG,
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
    _TILE_7_UDG, // 43
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
};


void _XL_INIT_GRAPHICS(void)
{
    #if defined(__ATARI_ST__)
        putenv("TERM=st52");
    #endif
	initscr();
	noecho();
    nodelay(NULL, TRUE);
	curs_set(0);
	start_color();
	cbreak();
	intrflush(stdscr, TRUE);
	// init_pair(1, COLOR_YELLOW, _NCURSES_BACKGROUND_COLOR);
	// init_pair(2, COLOR_CYAN, _NCURSES_BACKGROUND_COLOR);
	// init_pair(3, COLOR_RED, _NCURSES_BACKGROUND_COLOR);
	// init_pair(4, COLOR_GREEN, _NCURSES_BACKGROUND_COLOR);
	// init_pair(5, COLOR_BLUE, _NCURSES_BACKGROUND_COLOR);
	// init_pair(6, COLOR_WHITE, _NCURSES_BACKGROUND_COLOR);
	// init_pair(7, COLOR_MAGENTA, _NCURSES_BACKGROUND_COLOR);
	// init_pair(8, COLOR_BLACK, _NCURSES_BACKGROUND_COLOR);

	init_pair(1, _NCURSES_BACKGROUND_COLOR, COLOR_YELLOW);
	init_pair(2, _NCURSES_BACKGROUND_COLOR, COLOR_CYAN);
	init_pair(3, _NCURSES_BACKGROUND_COLOR, COLOR_RED);
	init_pair(4, _NCURSES_BACKGROUND_COLOR, COLOR_GREEN);
	init_pair(5, _NCURSES_BACKGROUND_COLOR, COLOR_BLUE);
	init_pair(6, _NCURSES_BACKGROUND_COLOR, COLOR_WHITE);
	init_pair(7, _NCURSES_BACKGROUND_COLOR, COLOR_MAGENTA);
	init_pair(8, _NCURSES_BACKGROUND_COLOR, COLOR_BLACK);

    wbkgd(stdscr, COLOR_PAIR(8));
    
    refresh();
    _setScreenColors();
}

