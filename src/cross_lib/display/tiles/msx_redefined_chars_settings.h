
#ifndef _MSX_REDEFINED_CHARS_SETTINGS
#define _MSX_REDEFINED_CHARS_SETTINGS

// Still available:
// 0x80-.... ?
#if !defined(ALTERNATIVE_COLOR)

    // CYAN

    #define _TILE_0 0x3B
    #define _TILE_1 0x3C
    #define _TILE_2 0x3D 
    #define _TILE_3 0x3E
    #define _TILE_9 0x3F
    #define _TILE_11 0x09
    #define _TILE_17 0x3A
    
    // YELLOW
    #define _TILE_7 0x27
    #define _TILE_12 0x26
    #define _TILE_13 0x22
    #define _DOLLAR_TILE 0x24
    #define _TILE_4_B _TILE_4
    #define _TILE_10 0x23

    // GREEN
    #define _TILE_8  0x2C

    // WHITE
    #define _TILE_4 0x7E

    #define _TILE_14 ((uint8_t)0x7B)
    #define _TILE_15 ((uint8_t)0x7D)
    #define _TILE_16 ((uint8_t)0x60)

    // TODO: Change it to something else
    // #define _TILE_5_B _TILE_5
    #define _TILE_5_B 'X'

    #define _TILE_19 0x80
    #define _TILE_20 0x81
    #define _TILE_21 0x82
    #define _TILE_22 0x83
    #define _TILE_23 0x84
    #define _TILE_24 0x85


    // RED
    #define _TILE_5 0xEE
    #define _TILE_6 0xEB
    #define _TILE_18 0xEC

    #define _TILE_7_B _TILE_7
    #define _TILE_0_B _TILE_0

#else
    // CYAN
    #define _TILE_2 0xEB
    #define _TILE_3 0xEC
    #define _TILE_11 0xED 
    #define _TILE_15 0xEE
    #define _TILE_17 0xEF
    #define _TILE_18 0xEA
    #define _TILE_14 0xF0
    #define _TILE_16 0xF1
    // YELLOW
    #define _TILE_7 0x27
    #define _TILE_12 0x26
    #define _TILE_13 0x22
    #define _DOLLAR_TILE 0x24
    #define _TILE_10 0x23
    #define _TILE_4_B _TILE_4

    // GREEN
    #define _TILE_0  0x2C
    #define _TILE_1  0x2D
    #define _TILE_4  0x2E
    #define _TILE_9  0x2F

    // RED
    #define _TILE_5 0x86
    #define _TILE_6 0x87
    #define _TILE_7_B _TILE_7
    #define _TILE_0_B _TILE_0

    // WHITE
    #define _TILE_8 0xA9

    // TODO: Change it to something else
    #define _TILE_5_B 'X'

    #define _TILE_19 0x80
    #define _TILE_20 0x81
    #define _TILE_21 0x82
    #define _TILE_22 0x83
    #define _TILE_23 0x84
    #define _TILE_24 0x85

#endif

#endif // _MSX_REDEFINED_CHARS_SETTINGS

