#ifndef _CHAR_TILES_H
#define _CHAR_TILES_H

#include "cross_lib.h"

#if defined(__TARGET_SPECIFIC_ASCII) && defined(__AQUARIUS__) && !defined(__QUAD_MEMORY_MAPPED_GRAPHICS) && !defined(__DUAL_MEMORY_MAPPED_GRAPHICS)
    #include "aquarius/aquarius_char_tiles.h"
#else
    
#define _TILE_0 '*'
#define _TILE_1 '*'

#define _TILE_2 '-'

#define _TILE_3 '-'

// Apple
#define _TILE_4 'O'

// Body
#define _TILE_5 'O'

#define _TILE_6 'H'

#define _TILE_7 '='

#if defined(_XL_NO_COLOR)
    #define _TILE_8 'R'
#else
    #define _TILE_8 'O'
#endif

#define _TILE_9 'L'

#define _TILE_10 '='

#define _TILE_11 'I'
#define _TILE_12 'I'

#define _TILE_13 '-'

#define _TILE_14 'S'

#define _TILE_15 'I'

#define _TILE_16 'C'

#define _TILE_17 'X'

#if defined(_XL_NO_COLOR)
    #define _TILE_18 'S'
#else
    #define _TILE_18 'O'
#endif

#define _TILE_25 '$'

#endif

#endif // _CHAR_TILES_H

