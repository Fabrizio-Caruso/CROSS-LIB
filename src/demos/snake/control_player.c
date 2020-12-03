#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"


extern uint8_t ship_x;
extern uint8_t ship_fire;

extern SnakeBody snake[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;


void delete_ship(void);


#if defined(JOYSTICK_CONTROL)
    void movePlayerByJoystick(uint8_t joyInput)
    {
        if(JOY_LEFT(joyInput))
        {
            move_snake(SNAKE_LEFT);
        }
        else if(JOY_RIGHT(joyInput))
        {
            move_snake(SNAKE_RIGHT);
        }
        else if(JOY_UP(joyInput))
        {
            move_snake(SNAKE_UP);
        }
        else if(JOY_DOWN(joyInput))
        {
            move_snake(SNAKE_DOWN);
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
            move_snake(SNAKE_LEFT);
        }
        else if(kbInput==_MOVE_RIGHT)
        {
            move_snake(SNAKE_RIGHT);
        }
        else if(kbInput==_MOVE_UP)
        {
            move_snake(SNAKE_UP);
        }
        else if(kbInput==_MOVE_DOWN)
        {
            move_snake(SNAKE_DOWN);
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


