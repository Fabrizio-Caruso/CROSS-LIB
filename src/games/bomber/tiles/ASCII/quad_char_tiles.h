#ifndef _CHAR_TILES_H
#define _CHAR_TILES_H

#include "cross_lib.h"


#if defined(__TARGET_SPECIFIC_ASCII) && defined(__AQUARIUS__) && !defined(__QUAD_MEMORY_MAPPED_GRAPHICS) && !defined(__DUAL_MEMORY_MAPPED_GRAPHICS)
    #include "aquarius/aquarius_char_tiles.h"
#elif defined(__TARGET_SPECIFIC_ASCII) && defined(__AQUARIUS__) && defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
    #include "aquarius/aquarius_quad_char_tiles.h"
#else
    
    #if !defined(__CIDELSA__)
        // Plane
        #define _TILE_1  '-'
        #define _TILE_2  '>'

        // Animated plane
        #define _TILE_0  '='
        #define _TILE_5 '~'

        #define _TILE_8  '='
    #else
         // Plane
        #define _TILE_1  'E'
        #define _TILE_2  'D'

        // Animated plane
        #define _TILE_0  'H'
        #define _TILE_5 'S'

        #define _TILE_8  'Z'   

    #endif

    #define _TILE_3 'O'
    #define _TILE_17 _TILE_2

    #if defined(__CIDELSA__)
        #define _TILE_4  'O'
        #define _TILE_18  'X'
        #define _TILE_7  'M'
        #define _TILE_11 'N'
        #define _TILE_15 'X'
        #define _TILE_19 'Q'
        #define _TILE_20 'T'
        #define _TILE_21 'U'
        #define _TILE_22 'B'
        #define _TILE_23 'T'
        #define _TILE_24 'W'
        #define _TILE_25 'I'
        
    #else
        #define _TILE_18  'X'
        #define _TILE_11 'N'
        #define _TILE_7  'M'
        #define _TILE_15 'X'
        #define _TILE_4  'O'
    #endif

    #define _TILE_6  'H'
    #define _TILE_9  'L'
    #define _TILE_14 'S'
    #define _TILE_16 'C'

    #define _TILE_12 'I'

    #define _TILE_10 _TILE_1

    #define _TILE_13 _TILE_10

#endif

#endif // _CHAR_TILES_H

