#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"

#include "settings.h"

#include "variables.h"


uint8_t _MOVE_PLAYER(uint8_t control_input)
{
    if(_XL_LEFT(control_input) && (snake_direction != SNAKE_RIGHT))
    {
        return move_snake(SNAKE_LEFT);
    }
    else if(_XL_RIGHT(control_input) && (snake_direction != SNAKE_LEFT))
    {
        return move_snake(SNAKE_RIGHT);
    }
    else if(_XL_UP(control_input) && (snake_direction != SNAKE_DOWN))
    {
        return move_snake(SNAKE_UP);
    }
    else if(_XL_DOWN(control_input) && (snake_direction != SNAKE_UP))
    {
        return move_snake(SNAKE_DOWN);
    }
    else
    {
        return move_snake(snake_direction);
    }
}

uint8_t MOVE_PLAYER(void)
{
    return _MOVE_PLAYER(_XL_INPUT());
}



