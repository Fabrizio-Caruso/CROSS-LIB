#ifndef _CHAR_TILES_H
#define _CHAR_TILES_H

#include "cross_lib.h"

#if defined(__TARGET_SPECIFIC_ASCII) && defined(__AQUARIUS__) && !defined(__QUAD_MEMORY_MAPPED_GRAPHICS) && !defined(__DUAL_MEMORY_MAPPED_GRAPHICS)
    #include "aquarius/aquarius_char_tiles.h"
#else
    
#define _TILE_0 'X'
#define _TILE_1 ' '

#define _TILE_2 'X'

#define _TILE_3 ' '

#define _TILE_4 'X'

#define _TILE_5 ' '

#define _TILE_6 'X'

#define _TILE_8 'O'

#define _TILE_9 'I'

#define _TILE_10 ' '

#define _TILE_11 ' '
#define _TILE_12 'I'

#define _TILE_13 'T' 

#define _TILE_14 'H'

#define _TILE_16 'T'

#define _TILE_19 'V'

#define _TILE_20 'V'

#define _TILE_21 'V'

#define _TILE_22 'V'

#define _TILE_23 'F'

#if !defined(__CIDELSA__) && !defined(__ATARI7800__)
    #define _TILE_7  '='
	#define _TILE_15 '*'
    #define _TILE_24 '-'
	#define _TILE_25 '$' 
#else
    #define _TILE_7  'S'
	#define _TILE_15 'X'
    #define _TILE_24 'M'
	#define _TILE_25 'S'

#endif


#if defined(__CIDELSA__) || defined(__MC10__)
	#define _TILE_17 'I'
	#define _TILE_18 'I'
	#if defined(__MC10__)
	#define _TILE_26 '.'
	#else
	#define _TILE_26 'I'
	#endif
#else
	#define _TILE_17 '|'
	#define _TILE_18 '|'	
	#define _TILE_26 '.'
#endif

#endif

#endif // _CHAR_TILES_H

