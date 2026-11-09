#ifndef _VZ_CHAR_TILES_H
#define _VZ_CHAR_TILES_H


#define MC10_GREEN_OFFSET 0
#define MC10_YELLOW_OFFSET 16
#define MC10_BLUE_OFFSET 32
#define MC10_RED_OFFSET 48
#define MC10_WHITE_OFFSET 64
#define MC10_CYAN_OFFSET 80
#define MC10_PURPLE_OFFSET 96
#define MC10_ORANGE_OFFSET 112

#define CHEQUERED_BASE 137
#define BLOCK_BASE 143

#define GREEN_CHEQUERED (CHEQUERED_BASE+MC10_GREEN_OFFSET)
#define YELLOW_CHEQUERED (CHEQUERED_BASE+MC10_YELLOW_OFFSET)
#define BLUE_CHEQUERED (CHEQUERED_BASE+MC10_BLUE_OFFSET)
#define RED_CHEQUERED (CHEQUERED_BASE+MC10_RED_OFFSET)
#define WHITE_CHEQUERED (CHEQUERED_BASE+MC10_WHITE_OFFSET)
#define CYAN_CHEQUERED (CHEQUERED_BASE+MC10_CYAN_OFFSET)


#define RED_BLOCK (BLOCK_BASE+MC10_RED_OFFSET)
#define CYAN_BLOCK (BLOCK_BASE+MC10_CYAN_OFFSET)
#define ORANGE_BLOCK (BLOCK_BASE+MC10_ORANGE_OFFSET)
#define YELLOW_BLOCK (BLOCK_BASE+MC10_YELLOW_OFFSET)


#define T_LETTER 20
#define V_LETTER 22
#define EXPLAMATION_MARK 33
#define DOUBLE_QUOTES 34
#define COLUMNS 58

#define DIAMOND 42

    #define _TILE_0 (unsigned char) EXPLAMATION_MARK
    #define _TILE_1 (unsigned char) COLUMNS

    #define _TILE_2 (unsigned char) '-'

    #define _TILE_3 (unsigned char) '<'

    #define _TILE_4 (unsigned char) '>'

    #define _TILE_5 (unsigned char) T_LETTER

    #define _TILE_6 (unsigned char) 'H'

    #define _TILE_7 (unsigned char) V_LETTER

    #define _TILE_8 (unsigned char) '-'

    #define _TILE_9 (unsigned char) 'L'

    #define _TILE_10 (unsigned char) 'W'

    #define _TILE_11 (unsigned char) RED_BLOCK

    #define _TILE_12 (unsigned char) CYAN_BLOCK

    #define _TILE_13 (unsigned char) CYAN_BLOCK

    #define _TILE_14 (unsigned char) 'S'

    #define _TILE_15 (unsigned char) RED_BLOCK

    #define _TILE_16 (unsigned char) 'C'

    #define _TILE_17 (unsigned char) 'D'

    #define _TILE_18 (unsigned char) 217

    #define _TILE_19 (unsigned char) ORANGE_BLOCK

    #define _TILE_20 (unsigned char) DIAMOND

    #define _TILE_21 (unsigned char) 150

    #define _TILE_22 (unsigned char) YELLOW_BLOCK

    #define _TILE_23 (unsigned char) YELLOW_BLOCK

    #define _TILE_24 (unsigned char) 137

    #define _TILE_25 (unsigned char) 166

    #define _TILE_26 (unsigned char) 233



#endif // _VZ_CHAR_TILES_H

