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

