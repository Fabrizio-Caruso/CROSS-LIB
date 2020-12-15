#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"
#include "settings.h"


extern uint8_t snake_x[MAX_SNAKE_LENGTH];
extern uint8_t snake_y[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;

extern Image* head_image_ptr;

extern Image VERTICAL_HEAD_IMAGE;
extern Image HORIZONTAL_HEAD_IMAGE;

extern uint8_t snake_direction;

extern uint8_t map[XSize][YSize];

extern uint8_t snake_head_x;
extern uint8_t snake_head_y;

uint8_t move_snake(uint8_t wished_direction)
{
    uint8_t tail = (snake_head+snake_length-1)%snake_length;
    uint8_t candidate_x;
    uint8_t candidate_y;
    
    // TODO: Memory might be saved by using two GOTO's
    switch(wished_direction)
    {
        case SNAKE_RIGHT:
            candidate_x = snake_head_x+1;
            goto same_y;
        break;
        case SNAKE_LEFT:
            candidate_x = snake_head_x-1;
            same_y:
            candidate_y = snake_head_y;
        break;
        case SNAKE_UP:
            candidate_y = snake_head_y-1;
            goto same_x;
        break;
        case SNAKE_DOWN:
            candidate_y = snake_head_y+1;
            same_x:
            candidate_x = snake_head_x;
        break;
    }
    if(!hits_wall(candidate_x,candidate_y)) // can move
    {
        snake_direction = wished_direction;
        delete_body_part(tail);
        snake_x[tail] = candidate_x;
        snake_y[tail] = candidate_y;
        
        if(HORIZONTAL(snake_direction))
        {
            head_image_ptr = &HORIZONTAL_HEAD_IMAGE;
        }
        else
        {
            head_image_ptr = &VERTICAL_HEAD_IMAGE;
        }
        
        draw_body_part(snake_head); // Draw old head as body part
        
        snake_head = tail; // new head uses tail index
        
        draw_head(); // draw new head
        return 1;
    }
    return 0;
}

