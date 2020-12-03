#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"

SnakeBody snake[MAX_SNAKE_LENGTH];

uint8_t snake_length;

uint8_t snake_head;

Image *head_image_ptr;

uint8_t map[100][YSize];

uint8_t snake_direction;

extern Image BODY_IMAGE;
extern Image HORIZONTAL_HEAD_IMAGE;

#define SNAKE_OFFSET 5

void draw_head(void)
{
    _XLIB_DRAW(snake[snake_head].x,snake[snake_head].y,head_image_ptr);
}

void delete_body_part(uint8_t i)
{
    _XLIB_DELETE(snake[i].x,snake[i].y);
    map[snake[i].x][snake[i].y] = 0;
}

void draw_body_part(uint8_t i)
{
    _XLIB_DRAW(snake[i].x,snake[i].y,&BODY_IMAGE);
    map[snake[i].x][snake[i].y] = 1;
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

void init_map(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<XSize;++i)
    {
        for(j=0;j<YSize;++j)
        {
            map[i][j]=0;
        }
    }
}


void init_snake(void)
{

    uint8_t i;
    
    snake_length = INIT_SNAKE_LENGTH;
    snake_head = 0;
    
    for(i=0;i<snake_length;++i)
    {
        snake[(i+snake_head)%snake_length].x = XSize/2+snake_length/2-SNAKE_OFFSET-i;
        snake[(i+snake_head)%snake_length].y = YSize/2;
    }
    
    head_image_ptr = &HORIZONTAL_HEAD_IMAGE;
    
    draw_snake();
    
    snake_direction = SNAKE_RIGHT;
}

