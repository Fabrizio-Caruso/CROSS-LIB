#include "cross_lib.h"

extern uint8_t ship_x;
extern uint8_t ship_fire;



#define _DO_MOVE_LEFT \
    do \
    { \
        delete_ship(); \
        --ship_x; \
    } while(0)
    
#define _DO_MOVE_RIGHT \
    do \
    { \
        delete_ship(); \
        ++ship_x; \
    } while(0)




    
#if defined(NO_INPUT)
    void MOVE_PLAYER(void) {}
#else
    void MOVE_PLAYER(void)
    {
        uint8_t input = _XL_INPUT();
        
        if(_XL_LEFT(input))
        {
            --ship_x;
        }
        else if(_XL_RIGHT(input))
        {
            ++ship_x;
        }
        else if(_XL_FIRE(input))
        {
            ship_fire = 1;
        }
    }
#endif


