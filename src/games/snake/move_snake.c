#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"

extern SnakeBody snake[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;

extern Image* head_image_ptr;

extern Image VERTICAL_HEAD_IMAGE;
extern Image HORIZONTAL_HEAD_IMAGE;

extern uint8_t snake_direction;

void move_snake(void)
{
    uint8_t tail = (snake_head+snake_length-1)%snake_length;
    
    delete_body_part(tail);
    
    switch(snake_direction)
    {
        case SNAKE_RIGHT:
            snake[tail].x = snake[snake_head].x+1;
            snake[tail].y = snake[snake_head].y;
            head_image_ptr = &HORIZONTAL_HEAD_IMAGE;
        break;
        case SNAKE_LEFT:
            snake[tail].x = snake[snake_head].x-1;
            snake[tail].y = snake[snake_head].y;
            head_image_ptr = &HORIZONTAL_HEAD_IMAGE;
        break;
        case SNAKE_UP:
            snake[tail].x = snake[snake_head].x;
            snake[tail].y = snake[snake_head].y-1;
            head_image_ptr = &VERTICAL_HEAD_IMAGE;
        break;
        case SNAKE_DOWN:
            snake[tail].x = snake[snake_head].x;
            snake[tail].y = snake[snake_head].y+1;
            head_image_ptr = &VERTICAL_HEAD_IMAGE;
        break;
    }
    
    draw_body_part(snake_head); // Draw old head as body part
    
    snake_head = tail; // new head uses tail index
    
    draw_head(); // draw new head
}

