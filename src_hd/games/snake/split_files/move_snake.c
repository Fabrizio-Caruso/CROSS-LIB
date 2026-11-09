#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"
#include "settings.h"

#include "variables.h"

#include "tiles.h"

uint8_t move_snake(uint8_t wished_direction)
{
    uint8_t candidate_x;
    uint8_t candidate_y;
    
    i = (snake_head+snake_length-1)%snake_length;

    candidate_selection:
    
    candidate_x = snake_head_x;
    candidate_y = snake_head_y;
    switch(wished_direction)
    {
        
        case SNAKE_RIGHT:
            ++candidate_x; 
        break;
        case SNAKE_LEFT:
            --candidate_x; 
        break;
        case SNAKE_UP:
            --candidate_y;
        break;
        case SNAKE_DOWN:
            ++candidate_y;
        break;
    }
    if(!hits_wall(candidate_x,candidate_y)) // can move
    {
        snake_direction = wished_direction;
        delete_body_part(i);
        snake_x[i] = candidate_x;
        snake_y[i] = candidate_y;
        
        if(HORIZONTAL(snake_direction))
        {
            head_tile = HORIZONTAL_HEAD_TILE;
        }
        else
        {
            head_tile = VERTICAL_HEAD_TILE;
        }
        
        draw_body_part(snake_head); // Draw old head as body part
        
        snake_head = i; // new head uses tail index
        
        draw_head(); // draw new head
        return 1;
    }
    if (wished_direction != snake_direction)
    {
        wished_direction = snake_direction;
        goto candidate_selection;
    }
    return 0;
}

