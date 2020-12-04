#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"


extern uint8_t ship_x;
extern uint8_t ship_fire;

extern SnakeBody snake[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;

extern uint8_t snake_direction;



#if defined(JOYSTICK_CONTROL)
    uint8_t movePlayerByJoystick(uint8_t joyInput)
    {
        if(JOY_LEFT(joyInput) && (snake_direction != SNAKE_RIGHT))
        {
            snake_direction = SNAKE_LEFT;
        }
        else if(JOY_RIGHT(joyInput) && (snake_direction != SNAKE_LEFT))
        {
            snake_direction = SNAKE_RIGHT;
        }
        else if(JOY_UP(joyInput) && (snake_direction != SNAKE_DOWN))
        {
            snake_direction = SNAKE_UP;
        }
        else if(JOY_DOWN(joyInput) && (snake_direction != SNAKE_UP))
        {
            snake_direction = SNAKE_DOWN;
        }
        return move_snake();
    }
#else
    uint8_t movePlayerByKeyboard(uint8_t kbInput)
    {
        #if defined(ALT_MOVE)
            delete_ship();
        #endif

        if((kbInput==_MOVE_LEFT) && (snake_direction != SNAKE_RIGHT))
        {
            snake_direction = SNAKE_LEFT;
        }
        else if((kbInput==_MOVE_RIGHT) && (snake_direction != SNAKE_LEFT))
        {
            snake_direction = SNAKE_RIGHT;
        }
        else if((kbInput==_MOVE_UP) && (snake_direction != SNAKE_DOWN)))
        {
            snake_direction = SNAKE_UP;
        }
        else if((kbInput==_MOVE_DOWN) && (snake_direction != SNAKE_UP))
        {
            snake_direction = SNAKE_DOWN;
        }

        return move_snake();

    }
#endif

    
#if defined(NO_INPUT)
    uint8_t MOVE_PLAYER(void) {}
#elif defined(KEYBOARD_CONTROL)
    uint8_t MOVE_PLAYER(void) 
    { 
        #if defined(ALT_MOVE)
            if(kbhit())
            {
        #endif
                return movePlayerByKeyboard(GET_CHAR()); 
        #if defined(ALT_MOVE)
            }
        #endif
    }
#else
    
    uint8_t MOVE_PLAYER(void)
    { 
        return movePlayerByJoystick(JOY_INPUT()); 
    }
#endif


