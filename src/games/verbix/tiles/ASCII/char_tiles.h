#ifndef _CHAR_TILES_H
#define _CHAR_TILES_H

#if defined(__TARGET_SPECIFIC_ASCII)

    #if defined(__ZX81__)
        #include "zx81/zx81_char_tiles.h"
    #elif defined(__MC10__)
        #include "mc10/mc10_char_tiles.h"
    #elif defined(__VZ__)
        #include "vz/vz_char_tiles.h"
    #elif defined(__ATARI7800__) && !defined(_XL_NO_COLOR)
        #include "atari7800/atari7800_char_tiles.h"
    #elif defined(__ATARI7800__) && defined(_XL_NO_COLOR)
        #include "atari7800/atari7800_no_color_char_tiles.h" 
    #else
        ERROR
    #endif
#else
    #define _TILE_0 (unsigned char) '|'
    #define _TILE_1 (unsigned char) '.'

    #define _TILE_2 (unsigned char) '-'

    #define _TILE_3 (unsigned char) '<'

    #define _TILE_4 (unsigned char) '>'

    #define _TILE_5 (unsigned char) '^'

    #define _TILE_6 (unsigned char) 'H'

    #define _TILE_7 (unsigned char) 'V'

    #define _TILE_8 (unsigned char) '-'

    #define _TILE_9 (unsigned char) 'L'

    #define _TILE_10 (unsigned char) 'W'

    #define _TILE_11 (unsigned char) '='

    #define _TILE_12 (unsigned char) '|'

    #define _TILE_13 (unsigned char) '-'

    #define _TILE_14 (unsigned char) 'S'

    #define _TILE_15 (unsigned char) '|'

    #define _TILE_16 (unsigned char) 'C'

    #define _TILE_17 (unsigned char) 'D'

    #define _TILE_18 (unsigned char) 'I'

    #define _TILE_19 (unsigned char) 'X'

    #define _TILE_20 (unsigned char) '$'

    #define _TILE_21 (unsigned char) '-'

    #define _TILE_22 (unsigned char) '|'

    #define _TILE_23 (unsigned char) '='

    #define _TILE_24 (unsigned char) '|'

    #define _TILE_25 (unsigned char) '|'

    #define _TILE_26 (unsigned char) '|'

#endif

#endif // _CHAR_TILES_H

