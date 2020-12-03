#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"
#include "images.h"

extern SnakeBody snake[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;

extern Image* head_image_ptr;

extern Image VERTICAL_HEAD_IMAGE;
extern Image HORIZONTAL_HEAD_IMAGE;

void move_snake(uint8_t direction)
{
    uint8_t tail = (snake_head+snake_length-1)%snake_length;
    
    _XLIB_DELETE(snake[tail].x,snake[tail].y);
    
    switch(direction)
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
    draw_body_part(snake_head);
    
    snake_head = tail;
    
    draw_head();
}
