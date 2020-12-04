#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"

SnakeBody snake[MAX_SNAKE_LENGTH];

uint8_t snake_length;

uint8_t snake_head;

Image *head_image_ptr;

uint8_t map[XSize][YSize];
SnakeBody snake_copy[MAX_SNAKE_LENGTH];


uint8_t snake_direction;

extern Image BODY_IMAGE;
extern Image HORIZONTAL_HEAD_IMAGE;
extern Image VERTICAL_HEAD_IMAGE;

#define SNAKE_OFFSET 5

void draw_head(void)
{
    _XLIB_DRAW(snake[snake_head].x,snake[snake_head].y,head_image_ptr);
}

void delete_body_part(uint8_t i)
{
    _XLIB_DELETE(snake[i].x,snake[i].y);
    map[snake[i].x][snake[i].y] = EMPTY;
}

void draw_body_part(uint8_t i)
{
    _XLIB_DRAW(snake[i].x,snake[i].y,&BODY_IMAGE);
    map[snake[i].x][snake[i].y] = SNAKE;
}


void init_map(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<XSize;++i)
    {
        for(j=0;j<YSize;++j)
        {
            map[i][j]=EMPTY;
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
        draw_body_part((i+snake_head) % snake_length);
        DO_SLOW_DOWN(SLOW_DOWN);
    }
    
    
    head_image_ptr = &HORIZONTAL_HEAD_IMAGE;
    
    // draw_snake();
    
    snake_direction = SNAKE_RIGHT;
    
    draw_head();

}

void snake_grows(void)
{
    uint8_t i;
    
    
    // Copy snake body starting at index 1
    for(i=0;i<snake_length;++i)
    {
        snake_copy[i+1].x=snake[(i+snake_head)%snake_length].x;
        snake_copy[i+1].y=snake[(i+snake_head)%snake_length].y;
    }
    
    // Generate head at index 0
    switch(snake_direction)
    {
        case SNAKE_RIGHT:
            snake_copy[0].x = snake[snake_head].x+1;
            snake_copy[0].y = snake[snake_head].y;
        break;
        case SNAKE_LEFT:
            snake_copy[0].x = snake[snake_head].x-1;
            snake_copy[0].y = snake[snake_head].y;
        break;
        case SNAKE_UP:
            snake_copy[0].x = snake[snake_head].x;
            snake_copy[0].y = snake[snake_head].y-1;
        break;
        case SNAKE_DOWN:
            snake_copy[0].x = snake[snake_head].x;
            snake_copy[0].y = snake[snake_head].y+1;
        break;
    }
    
    // New head is at index 0
    snake_head = 0;
    
    // New length = old length + 1
    ++snake_length;
    
    // Copy the new snake into snake array
    for(i=0;i<snake_length;++i)
    {
        snake[i].x = snake_copy[i].x;
        snake[i].y = snake_copy[i].y;
        
        if(i)
        {
            draw_body_part(i);
        }
    }
    draw_head();
}


