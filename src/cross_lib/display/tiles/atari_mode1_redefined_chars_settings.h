
#ifndef _ATARI_MODE1_SETTINGS
#define _ATARI_MODE1_SETTINGS

// Still available:
// 3,15,26,27,31,32 (TILE_19-24)
// 16-25 (0-9)
// 33-58 (A-Z)
// 13 (-)
// 4 ($)

// #define _ATARI_MODE1_RED 0
// #define _ATARI_MODE1_WHITE 64
// #define _ATARI_MODE1_BLUE 128
// #define _ATARI_MODE1_YELLOW 192


#define _TILE_0_OFFSET 60
#define _TILE_1_OFFSET 61
#define _TILE_2_OFFSET 62
#define _TILE_3_OFFSET 63

#define _TILE_0_B_OFFSET 60
#define _TILE_4_OFFSET 59
#define _TILE_5_OFFSET 29

#define _TILE_6_OFFSET 14
#define _TILE_7_OFFSET 30 

#define _TILE_8_OFFSET 28
#define _TILE_9_OFFSET 11
#define _TILE_10_OFFSET 12 
#define _TILE_11_OFFSET 10 

#define _TILE_12_OFFSET 5
#define _TILE_13_OFFSET 6
#define _TILE_14_OFFSET 9
#define _TILE_15_OFFSET 8
#define _TILE_16_OFFSET 7
#define _TILE_17_OFFSET 1
#define _TILE_18_OFFSET 2


#define _TILE_19_OFFSET 3
#define _TILE_20_OFFSET 15
#define _TILE_21_OFFSET 26
#define _TILE_22_OFFSET 27
#define _TILE_23_OFFSET 31
#define _TILE_24_OFFSET 32

#define _DOLLAR_TILE_OFFSET 4

#if !defined(COLOR)
    #if !defined(ALTERNATE_COLORS)
        #define _TILE_19 (_TILE_19_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_20 (_TILE_20_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_21 (_TILE_21_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_22 (_TILE_22_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_23 (_TILE_23_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_24 (_TILE_24_OFFSET + _ATARI_MODE1_YELLOW)



        #define _TILE_0 (_TILE_0_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_1 (_TILE_1_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_2 (_TILE_2_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_3 (_TILE_3_OFFSET + _ATARI_MODE1_BLUE)


        #define _TILE_0_B (_TILE_0_B_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_4 (_TILE_4_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_6 (_TILE_6_OFFSET + _ATARI_MODE1_RED)
        #define _TILE_7 (_TILE_7_OFFSET + _ATARI_MODE1_YELLOW)
        #define _TILE_5 (_TILE_5_OFFSET + _ATARI_MODE1_RED)
        #define _TILE_8 (_TILE_8_OFFSET + _ATARI_MODE1_YELLOW)

        #define _TILE_17 (_TILE_17_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_18 (_TILE_18_OFFSET + _ATARI_MODE1_RED)

        #define _TILE_7_B (_TILE_7_OFFSET + _ATARI_MODE1_RED)
        #define _TILE_9 (_TILE_9_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_11 (_TILE_11_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_10 (_TILE_10_OFFSET + _ATARI_MODE1_YELLOW)

        #define _TILE_14 (_TILE_14_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_15 (_TILE_15_OFFSET + _ATARI_MODE1_WHITE)	
        #define _TILE_16 (_TILE_16_OFFSET + _ATARI_MODE1_WHITE)

        #define _DOLLAR_TILE (_DOLLAR_TILE_OFFSET + _ATARI_MODE1_YELLOW)
        #define _TILE_12 (5 + _ATARI_MODE1_YELLOW)
        #define _TILE_13 (6 + _ATARI_MODE1_YELLOW)

        #define _TILE_5_B (_TILE_5_OFFSET+_ATARI_MODE1_YELLOW)

        #define _TILE_4_B (_TILE_4_OFFSET + _ATARI_MODE1_YELLOW)
    #else
        #define _TILE_19 (_TILE_19_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_20 (_TILE_20_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_21 (_TILE_21_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_22 (_TILE_22_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_23 (_TILE_23_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_24 (_TILE_24_OFFSET + _ATARI_MODE1_YELLOW)

        #define _TILE_0_B (_TILE_0_B_OFFSET + _ATARI_MODE1_YELLOW)

        #define _TILE_0 (_TILE_0_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_1 (_TILE_1_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_2 (_TILE_2_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_3 (_TILE_3_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_4 (_TILE_4_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_5 (_TILE_5_OFFSET + _ATARI_MODE1_RED)
        #define _TILE_6 (_TILE_6_OFFSET + _ATARI_MODE1_RED)
        #define _TILE_7 (_TILE_7_OFFSET + _ATARI_MODE1_YELLOW)
        #define _TILE_8 (_TILE_8_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_9 (_TILE_9_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_10 (_TILE_10_OFFSET + _ATARI_MODE1_YELLOW)
        #define _TILE_11 (_TILE_11_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_12 (5 + _ATARI_MODE1_YELLOW)
        #define _TILE_13 (6 + _ATARI_MODE1_YELLOW)
        #define _TILE_14 (_TILE_14_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_15 (_TILE_15_OFFSET + _ATARI_MODE1_WHITE)	
        #define _TILE_16 (_TILE_16_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_17 (_TILE_17_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_18 (_TILE_18_OFFSET + _ATARI_MODE1_WHITE)
        #define _DOLLAR_TILE (_DOLLAR_TILE_OFFSET + _ATARI_MODE1_YELLOW)

        #define _TILE_7_B (_TILE_7_OFFSET + _ATARI_MODE1_RED)





        #define _TILE_5_B (_TILE_5_OFFSET+_ATARI_MODE1_YELLOW)

        #define _TILE_4_B _TILE_4_OFFSET
    #endif
#else
    #define _TILE_19 _TILE_19_OFFSET
    #define _TILE_20 _TILE_20_OFFSET
    #define _TILE_21 _TILE_21_OFFSET
    #define _TILE_22 _TILE_22_OFFSET
    #define _TILE_23 _TILE_23_OFFSET
    #define _TILE_24 _TILE_24_OFFSET


    #define _TILE_0 _TILE_0_OFFSET
    #define _TILE_1 _TILE_1_OFFSET
    #define _TILE_2 _TILE_2_OFFSET
    #define _TILE_3 _TILE_3_OFFSET


    #define _TILE_0_B _TILE_0_B_OFFSET
    #define _TILE_4 _TILE_4_OFFSET
    #define _TILE_6 _TILE_6_OFFSET
    #define _TILE_7 _TILE_7_OFFSET
    #define _TILE_5 _TILE_5_OFFSET
    #define _TILE_8 _TILE_8_OFFSET

    #define _TILE_17 _TILE_17_OFFSET
    #define _TILE_18 _TILE_18_OFFSET

    #define _TILE_7_B _TILE_7_OFFSET
    #define _TILE_9   _TILE_9_OFFSET
    #define _TILE_11  _TILE_11_OFFSET
    #define _TILE_10  _TILE_10_OFFSET

    #define _TILE_14 _TILE_14_OFFSET
    #define _TILE_15 _TILE_15_OFFSET
    #define _TILE_16 _TILE_16_OFFSET

    #define _DOLLAR_TILE _DOLLAR_TILE_OFFSET
    #define _TILE_12 _TILE_12_OFFSET
    #define _TILE_13 _TILE_13_OFFSET

    #define _TILE_5_B _TILE_5_OFFSET

    #define _TILE_4_B _TILE_4_OFFSET
#endif

#endif // _ATARI_MODE1_SETTINGS

