
#ifndef _APPLE2_HGR_SETTINGS
#define _APPLE2_HGR_SETTINGS


#define _TILE_0_OFFSET 10
#define _TILE_1_OFFSET 11
#define _TILE_2_OFFSET 12
#define _TILE_3_OFFSET 13

#define _TILE_4_OFFSET 14
#define _TILE_5_OFFSET 15

#define _TILE_6_OFFSET 16
#define _TILE_7_OFFSET 43

#define _TILE_8_OFFSET 44
#define _TILE_9_OFFSET 45
#define _TILE_10_OFFSET 46 
#define _TILE_11_OFFSET 47 

#define _TILE_12_OFFSET 48
#define _TILE_13_OFFSET 49
#define _TILE_14_OFFSET 50
#define _TILE_15_OFFSET 51
#define _TILE_16_OFFSET 52
#define _TILE_17_OFFSET 53
#define _TILE_18_OFFSET 54


#define _TILE_19_OFFSET 55
#define _TILE_20_OFFSET 56
#define _TILE_21_OFFSET 57
#define _TILE_22_OFFSET 58
#define _TILE_23_OFFSET 59
#define _TILE_24_OFFSET 60

#define _DOLLAR_TILE_OFFSET 61

#if defined(NO_COLOR)
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


        #define _TILE_4 (_TILE_4_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_6 (_TILE_6_OFFSET + _ATARI_MODE1_RED)
        #define _TILE_7 (_TILE_7_OFFSET + _ATARI_MODE1_YELLOW)
        #define _TILE_5 (_TILE_5_OFFSET + _ATARI_MODE1_RED)
        #define _TILE_8 (_TILE_8_OFFSET + _ATARI_MODE1_YELLOW)

        #define _TILE_17 (_TILE_17_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_18 (_TILE_18_OFFSET + _ATARI_MODE1_RED)

        #define _TILE_9 (_TILE_9_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_11 (_TILE_11_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_10 (_TILE_10_OFFSET + _ATARI_MODE1_YELLOW)

        #define _TILE_14 (_TILE_14_OFFSET + _ATARI_MODE1_WHITE)
        #define _TILE_15 (_TILE_15_OFFSET + _ATARI_MODE1_WHITE)	
        #define _TILE_16 (_TILE_16_OFFSET + _ATARI_MODE1_WHITE)

        #define _DOLLAR_TILE (_DOLLAR_TILE_OFFSET + _ATARI_MODE1_YELLOW)
        #define _TILE_12 (5 + _ATARI_MODE1_YELLOW)
        #define _TILE_13 (6 + _ATARI_MODE1_YELLOW)

    #else
        #define _TILE_19 (_TILE_19_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_20 (_TILE_20_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_21 (_TILE_21_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_22 (_TILE_22_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_23 (_TILE_23_OFFSET + _ATARI_MODE1_BLUE)
        #define _TILE_24 (_TILE_24_OFFSET + _ATARI_MODE1_YELLOW)

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


    #define _TILE_4 _TILE_4_OFFSET
    #define _TILE_6 _TILE_6_OFFSET
    #define _TILE_7 _TILE_7_OFFSET
    #define _TILE_5 _TILE_5_OFFSET
    #define _TILE_8 _TILE_8_OFFSET

    #define _TILE_17 _TILE_17_OFFSET
    #define _TILE_18 _TILE_18_OFFSET

    #define _TILE_9   _TILE_9_OFFSET
    #define _TILE_11  _TILE_11_OFFSET
    #define _TILE_10  _TILE_10_OFFSET

    #define _TILE_14 _TILE_14_OFFSET
    #define _TILE_15 _TILE_15_OFFSET
    #define _TILE_16 _TILE_16_OFFSET

    #define _DOLLAR_TILE _DOLLAR_TILE_OFFSET
    #define _TILE_12 _TILE_12_OFFSET
    #define _TILE_13 _TILE_13_OFFSET

#endif

#endif // _APPLE2_HGR_SETTINGS

