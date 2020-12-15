#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"

#include "settings.h"

extern uint8_t ship_x;
extern uint8_t ship_fire;

extern uint8_t snake_x[MAX_SNAKE_LENGTH];
extern uint8_t snake_y[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;

extern uint8_t snake_direction;


#if defined(JOYSTICK_CONTROL)
    #define LEFT_CONTROL() JOY_LEFT(control_input)
    #define RIGHT_CONTROL() JOY_RIGHT(control_input)
    #define UP_CONTROL() JOY_UP(control_input)
    #define DOWN_CONTROL() JOY_DOWN(control_input)
#else
    #define LEFT_CONTROL() (control_input==_MOVE_LEFT)
    #define RIGHT_CONTROL() (control_input==_MOVE_RIGHT)
    #define UP_CONTROL() (control_input==_MOVE_UP)
    #define DOWN_CONTROL() (control_input==_MOVE_DOWN)
#endif


uint8_t _MOVE_PLAYER(uint8_t control_input)
{
    if(LEFT_CONTROL() && (snake_direction != SNAKE_RIGHT))
    {
        return move_snake(SNAKE_LEFT);
    }
    else if(RIGHT_CONTROL() && (snake_direction != SNAKE_LEFT))
    {
        return move_snake(SNAKE_RIGHT);
    }
    else if(UP_CONTROL() && (snake_direction != SNAKE_DOWN))
    {
        return move_snake(SNAKE_UP);
    }
    else if(DOWN_CONTROL() && (snake_direction != SNAKE_UP))
    {
        return move_snake(SNAKE_DOWN);
    }
    else
    {
        return move_snake(snake_direction);
    }
}


    
#if defined(NO_INPUT)
    uint8_t MOVE_PLAYER(void) {}
#elif defined(KEYBOARD_CONTROL)
    uint8_t MOVE_PLAYER(void) 
    { 
        return _MOVE_PLAYER(GET_CHAR()); 
    }
#else
    
    uint8_t MOVE_PLAYER(void)
    { 
        return _MOVE_PLAYER(JOY_INPUT()); 
    }
#endif


