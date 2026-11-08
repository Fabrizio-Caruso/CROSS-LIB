#ifndef _CHAR_TILES_H
#define _CHAR_TILES_H

#include "cross_lib.h"

#if defined(__TARGET_SPECIFIC_ASCII) && defined(__AQUARIUS__)
    #include "aquarius/aquarius_char_tiles.h"
#else
    
#define _TILE_0 'X'
#define _TILE_1 'X'

#define _TILE_2 ' '

#define _TILE_3 'X'

#define _TILE_4 ' '

#define _TILE_5 'X'

#define _TILE_6 ' '



#define _TILE_8 'O'

#define _TILE_9 'I'

#define _TILE_10 ' '

#define _TILE_11 ' '
#define _TILE_12 'I'

#define _TILE_13 'T' 

#define _TILE_14 'H'

#define _TILE_15 'L'

#define _TILE_16 'T'

#define _TILE_17 'I'

#define _TILE_18 'I'

#define _TILE_19 'V'

#define _TILE_20 'V'

#define _TILE_21 'V'

#define _TILE_22 'V'

#if !defined(__CIDELSA__)
    #define _TILE_7 '='
    #define _TILE_23 '*'
    #define _TILE_24 '-'
    
#else
    #define _TILE_7  'Z'
    #define _TILE_23 'X'
    #define _TILE_24 'M'
#endif

#define _TILE_25 'S'

#endif

#endif // _CHAR_TILES_H

