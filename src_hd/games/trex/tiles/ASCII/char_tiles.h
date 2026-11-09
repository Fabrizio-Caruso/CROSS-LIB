#ifndef _CHAR_TILES_H
#define _CHAR_TILES_H

#include "cross_lib.h"


#define _TILE_0 'O'
#define _TILE_2 'L'
#define _TILE_3 'L'

#define _TILE_17 'I'
#define _TILE_16 'I'

#define _TILE_15 'O'
#define _TILE_6 'L'
#define _TILE_7 ' '

#define _TILE_8 'L'
#define _TILE_9 'L'

#if defined(__ATARI7800__) && !defined(_XL_NO_COLOR)
    #define _TILE_13 '.'
    #define _TILE_14 '.'
#else
    #define _TILE_13 '-'
    #define _TILE_14 '-'
#endif
#define _TILE_10 'W'
#define _TILE_4  'I'

// #define _TILE_11 'U'
// #define _TILE_20 'I'
// #define _TILE_12 'U'
// #define _TILE_21 'I'


#define _TILE_1  'U'
#define _TILE_4  'I'
#if defined(__ATARI7800__) && !defined(_XL_NO_COLOR)
    #define _TILE_5  '.'


    #define _TILE_22 'C'
    #define _TILE_23 '.'

    #define _TILE_24 'C'
    #define _TILE_25 'B'

    #define _TILE_26 '+'
    
#else

    #define _TILE_5  '-'

    #define _TILE_22 '<'
    #define _TILE_23 '='

    #define _TILE_24 '<'
    #define _TILE_25 '-'

    #define _TILE_26 '+'


#endif

/*
#ifndef _IMAGES_H
#define _IMAGES_H


#include "cross_lib.h"

#if !defined(TINY_GAME)

    // Initial/jump dino 
    #define TOP_DINO_0            _TILE_0
    #define TAIL_DINO_0           _TILE_2
    #define BOTTOM_DINO_0         _TILE_3

    // Dino lower feet
    #define TAIL_RAISED_DINO      _TILE_17
    #define BOTTOM_RAISED_DINO    _TILE_16

    // Dino with +4 pixel offset
    #define TOP_DINO_1            _TILE_15
    #define MIDDLE_DINO_1         _TILE_6
    #define BOTTOM_DINO_1         _TILE_7
    
    #define TAIL_DINO_1           _TILE_8
    #define FOOT_DINO_1           _TILE_9
    

    #define TERRAIN_2             _TILE_13
    #define TERRAIN_3             _TILE_14
    

    #define TOP_CACTUS_2          _TILE_10
    #define BOTTOM_CACTUS_2       _TILE_4

    #define TOP_LEFT_CACTUS_2     _TILE_11
    #define BOTTOM_LEFT_CACTUS_2  _TILE_20
    #define TOP_RIGHT_CACTUS_2    _TILE_12
    #define BOTTOM_RIGHT_CACTUS_2 _TILE_21
    

    #define TERRAIN               _TILE_5
    
    #define TOP_CACTUS            _TILE_1
    #define BOTTOM_CACTUS         _TILE_4

    #define TOP_LEFT_CACTUS       _TILE_18
    #define BOTTOM_LEFT_CACTUS    _TILE_20
    #define TOP_RIGHT_CACTUS      _TILE_19
    #define BOTTOM_RIGHT_CACTUS   _TILE_21

    #define LEFT_BIRD_0           _TILE_22
    #define LEFT_BIRD_1           _TILE_23

    #define RIGHT_BIRD_0          _TILE_24
    #define RIGHT_BIRD_1          _TILE_25

    #define TOP_DEAD_DINO_0       _TILE_26
    
#else

    // Initial/jump dino 
    #define TOP_DINO_0            _TILE_0
    #define TAIL_DINO_0           _TILE_2
    #define BOTTOM_DINO_0         _TILE_3

    #define TOP_CACTUS            _TILE_1
    #define BOTTOM_CACTUS         _TILE_4

    #define TERRAIN               _TILE_5
  
#endif

#endif // _IMAGES_H


*/

#endif // _CHAR_TILES_H

