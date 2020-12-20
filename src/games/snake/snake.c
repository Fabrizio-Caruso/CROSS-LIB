#include "cross_lib.h"
#include "snake.h"
#include "move_snake.h"
#include "settings.h"

#include "variables.h"



void draw_head(void)
{
    _XLIB_DRAW(snake_x[snake_head],snake_y[snake_head],head_image_ptr);
}

void delete_body_part(uint8_t i)
{
    _XLIB_DELETE(snake_x[i],snake_y[i]);
    map[snake_x[i]][snake_y[i]] = EMPTY;
}

void draw_body_part(uint8_t i)
{
    _XLIB_DRAW(snake_x[i],snake_y[i],&BODY_IMAGE);
    map[snake_x[i]][snake_y[i]] = DEADLY;
}


void init_snake(void)
{
    uint8_t i;
    uint8_t aux;
    
    snake_length = XSize/5;
    
    snake_head = 0;
    
    for(i=0;i<snake_length;++i)
    {
        aux = (i+snake_head)%snake_length;
        snake_x[aux] = XSize/2+snake_length/2-i-1;
        snake_y[aux] = YSize/2;
        draw_body_part(aux);
    }
    
    head_image_ptr = &HORIZONTAL_HEAD_IMAGE;
    
    snake_direction = SNAKE_RIGHT;
    
    draw_head();
    
    snake_head_x = snake_x[snake_head];
    snake_head_y = snake_y[snake_head];
    
}

void snake_grows(void)
{
    uint8_t i;
    uint8_t x;
    uint8_t y;
    
    if(snake_length<MAX_SNAKE_LENGTH)
    {
        // Generate head at index 0
        
        x = snake_head_x; 
        y = snake_head_y;

        switch(snake_direction)
        {
            case SNAKE_RIGHT:
                ++x;
            case SNAKE_LEFT:
                --x;
            break;
            case SNAKE_UP:
                --y;
            break;
            case SNAKE_DOWN:
                ++y;
            break;
        }
        
        if(!(map[x][y]))
        {
            
            snake_copy_x[0] = x;
            snake_copy_y[0] = y;
        
            // Copy snake body starting at index 1
            for(i=0;i<snake_length;++i)
            {   
                x = (i+snake_head)%snake_length; // store
                snake_copy_x[i+1]=snake_x[x];
                snake_copy_y[i+1]=snake_y[x];
            }
            
            // New head is at index 0
            snake_head = 0;
            
            // New length = old length + 1
            ++snake_length;
            
            // Copy the new snake into snake array
            for(i=0;i<snake_length;++i)
            {
                snake_x[i] = snake_copy_x[i];
                snake_y[i] = snake_copy_y[i];
                
                if(i)
                {
                    draw_body_part(i);
                }
            }
            draw_head();
            snake_head_x = snake_x[snake_head];
            snake_head_y = snake_y[snake_head];
        }
    }
}


