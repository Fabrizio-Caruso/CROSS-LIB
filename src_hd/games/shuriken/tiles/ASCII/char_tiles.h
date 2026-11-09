#ifndef _CHAR_TILES_H
#define _CHAR_TILES_H

#include "cross_lib.h"

#if defined(__MC10__) || defined(__CIDELSA__) 
	#define _VERTICAL_BAR 'I'
#else
	#define _VERTICAL_BAR '|'
#endif

#define _TILE_0 _VERTICAL_BAR
#define _TILE_1 '-'
#define _TILE_2 '-'
#define _TILE_3 _VERTICAL_BAR

#define _TILE_4 '-'
#define _TILE_5 _VERTICAL_BAR
#define _TILE_6 _VERTICAL_BAR
#define _TILE_7 '-'

#define _TILE_8 '-'
#define _TILE_9 _VERTICAL_BAR
#define _TILE_10 _VERTICAL_BAR
#define _TILE_11 '-'

#define _TILE_12 _VERTICAL_BAR
#define _TILE_13 '-'
#define _TILE_14 '-'
#define _TILE_15 _VERTICAL_BAR

#define _TILE_16 '*'

#define _TILE_17 _VERTICAL_BAR
#define _TILE_18 _VERTICAL_BAR

#define _TILE_19 '-'
#define _TILE_20 '-'

#define _TILE_21 'X'

#if !defined(__ATARI7800__)
    #define _TILE_22 '='
#else
    #define _TILE_22 '0' 
#endif

#define _TILE_23 'S'

#define _TILE_24 _VERTICAL_BAR

#define _TILE_25 'O'

#define _TILE_26 '.'

#endif // _CHAR_TILES_H

