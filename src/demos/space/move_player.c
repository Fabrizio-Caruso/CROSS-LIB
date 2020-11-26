#include "cross_lib.h"
// #include "input_macros.h"

extern uint8_t ship_x;
extern uint8_t ship_fire;



// #if !defined(ALT_MOVE)
    // #define _DO_MOVE_LEFT \
        // do \
        // { \
            // delete_ship(); \
            // --ship_x; \
        // } while(0)
        
    // #define _DO_MOVE_RIGHT \
        // do \
        // { \
            // delete_ship(); \
            // ++ship_x; \
        // } while(0)
// #else
    // #define _DO_MOVE_LEFT() --ship_x
    // #define _DO_MOVE_RIGHT() ++ship_x
// #endif

void delete_ship(void);

#if defined(JOYSTICK_CONTROL)
    void movePlayerByJoystick(uint8_t joyInput)
    {
        if(JOY_LEFT(joyInput))
        {
            --ship_x;
        }
        else if(JOY_RIGHT(joyInput))
        {
            ++ship_x;
        }
        else if(JOY_FIRE(joyInput))
        {
            ship_fire = 1;
        }
    }
#else
    void movePlayerByKeyboard(uint8_t kbInput)
    {
        #if defined(ALT_MOVE)
            delete_ship();
        #endif

        if(kbInput==_MOVE_LEFT)
        {
            _DO_MOVE_LEFT();
        }
        else if(kbInput==_MOVE_RIGHT)
        {
            _DO_MOVE_RIGHT();
        }
        else if(kbInput==_FIRE)
        {
            ship_fire = 1;
        }
    }
#endif

    
#if defined(NO_INPUT)
    void MOVE_PLAYER(void) {}
#elif defined(KEYBOARD_CONTROL)
    void MOVE_PLAYER(void) 
    { 
        #if defined(ALT_MOVE)
            if(kbhit())
            {
        #endif
                movePlayerByKeyboard(GET_CHAR()); 
        #if defined(ALT_MOVE)
            }
        #endif
    }
#else
    
    void MOVE_PLAYER(void)
    { 
        movePlayerByJoystick(JOY_INPUT()); 
    }
#endif


