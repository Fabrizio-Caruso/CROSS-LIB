#ifndef _MOVE_PLAYER_H
#define _MOVE_PLAYER_H

#include "images.h"
#include "cross_lib.h"

#define MAX_Y ((YSize)+(Y_OFFSET))

#define SPACE_SHIP_Y (MAX_Y-4)

void MOVE_PLAYER(void);


#if !defined(NO_UDG)

    #define draw_ship_1() \
    { \
        _XL_DRAW(ship_x/4,SPACE_SHIP_Y,SPACE_SHIP_1_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+1,SPACE_SHIP_Y,SPACE_SHIP_1_E_TILE, _XL_CYAN); \
    }


    #define draw_ship_2() \
    { \
        _XL_DRAW(ship_x/4,SPACE_SHIP_Y,SPACE_SHIP_2_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+1,SPACE_SHIP_Y,SPACE_SHIP_2_E_TILE, _XL_CYAN); \
    }

    #define draw_ship_3() \
    { \
        _XL_DRAW(ship_x/4,SPACE_SHIP_Y,SPACE_SHIP_3_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+1,SPACE_SHIP_Y,SPACE_SHIP_3_C_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+2,SPACE_SHIP_Y,SPACE_SHIP_3_E_TILE, _XL_CYAN); \
    }


    #define draw_ship_4() \
    { \
        _XL_DRAW(ship_x/4,SPACE_SHIP_Y,SPACE_SHIP_4_W_TILE, _XL_CYAN); \
        _XL_DRAW(ship_x/4+1,SPACE_SHIP_Y,SPACE_SHIP_4_E_TILE, _XL_CYAN); \
    }

    #define delete_at_the_right() \
    { \
        _XL_DELETE(ship_x/4+2,SPACE_SHIP_Y); \
    }

    #define delete_at_the_left() \
    { \
        _XL_DELETE(ship_x/4,SPACE_SHIP_Y); \
    }

#else
    #define draw_ship() \
        _XL_DRAW(ship_x,SPACE_SHIP_Y,SPACE_SHIP_1_W_TILE, _XL_WHITE); \
        _XL_DRAW(ship_x+1,SPACE_SHIP_Y,SPACE_SHIP_1_E_TILE, _XL_WHITE); \
  
    #define draw_ship_1() \
        draw_ship()
  
    #define draw_ship_2() \
        draw_ship()

    #define draw_ship_3() \
        draw_ship()

    #define draw_ship_4() \
        draw_ship() 
        
    #define delete_at_the_left() \
        _XL_DELETE(ship_x-1,SPACE_SHIP_Y); \
        _XL_DELETE(ship_x,SPACE_SHIP_Y); \

        
    #define delete_at_the_right() \
        _XL_DELETE(ship_x+1,SPACE_SHIP_Y);  \
        _XL_DELETE(ship_x,SPACE_SHIP_Y)

#endif


#endif // _MOVE_PLAYER_H
