#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"

extern Coordinate snake[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;

extern Image* head_image_ptr;

extern Image VERTICAL_HEAD_IMAGE;
extern Image HORIZONTAL_HEAD_IMAGE;

extern uint8_t snake_direction;

extern uint8_t map[XSize][YSize];

uint8_t move_snake(uint8_t wished_direction)
{
    uint8_t tail = (snake_head+snake_length-1)%snake_length;
    uint8_t x;
    uint8_t y;
    
    
    switch(wished_direction)
    {
        case SNAKE_RIGHT:
            x = snake[snake_head].x+1;
            y = snake[snake_head].y;
        break;
        case SNAKE_LEFT:
            x = snake[snake_head].x-1;
            y = snake[snake_head].y;
        break;
        case SNAKE_UP:
            x = snake[snake_head].x;
            y = snake[snake_head].y-1;
        break;
        case SNAKE_DOWN:
            x = snake[snake_head].x;
            y = snake[snake_head].y+1;
        break;
    }
    if(!hits_wall(x,y)) // can move
    {
        snake_direction = wished_direction;
        delete_body_part(tail);
        snake[tail].x = x;
        snake[tail].y = y;
        
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

