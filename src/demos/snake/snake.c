#include "cross_lib.h"
#include "snake.h"
#include "images.h"

SnakeBody snake[MAX_SNAKE_LENGTH];

uint8_t snake_length;

uint8_t snake_head;

Image *head_image_ptr;

extern Image BODY_IMAGE;
extern Image HORIZONTAL_HEAD_IMAGE;

void draw_head(void)
{
    _XLIB_DRAW(snake[snake_head].x,snake[snake_head].y,head_image_ptr);
}

void draw_body_part(uint8_t i)
{
    _XLIB_DRAW(snake[i].x,snake[i].y,&BODY_IMAGE);
}

void draw_snake(void)
{
    uint8_t i;
    
    draw_head();

    for(i=1;i<snake_length;++i)
    {
        draw_body_part((i+snake_head) % snake_length);
    }
}



void init_snake(void)
{

    uint8_t i;
    
    snake_length = INIT_SNAKE_LENGTH;
    snake_head = 0;
    
    for(i=0;i<snake_length;++i)
    {
        snake[(i+snake_head)%snake_length].x = XSize/2+snake_length/2-i;
        snake[(i+snake_head)%snake_length].y = YSize/2;
    }
    
    head_image_ptr = &HORIZONTAL_HEAD_IMAGE;
    
    draw_snake();
}

