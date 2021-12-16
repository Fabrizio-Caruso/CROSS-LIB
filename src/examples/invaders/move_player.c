#include "cross_lib.h"

#include "move_player.h"

#include "tile_aliases.h"

extern uint8_t ship_x;
extern uint8_t ship_fire;

    
#if defined(NO_INPUT)
    void MOVE_PLAYER(void) {}
#else
    void MOVE_PLAYER(void)
    {
        uint8_t input = _XL_INPUT();
        
        if(_XL_LEFT(input))
        {
            delete_at_the_right();
            --ship_x;
        }
        else if(_XL_RIGHT(input))
        {
            delete_at_the_left();
            ++ship_x;
        }
        else if(_XL_FIRE(input))
        {
            ship_fire = 1;
        }
        
        switch(ship_x&3)
        {
            case 1:
                draw_ship_1();
            break;
            
            case 2:
                draw_ship_2();
            break;
            
            case 3:
                draw_ship_3();
            break;
            
            case 0:
                draw_ship_4();
            break;
        }
    }
#endif


