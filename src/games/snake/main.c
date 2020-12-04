/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 


#if !defined EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

#include "cross_lib.h"

#include "init_images.h"

#include "snake.h"
#include "move_snake.h"

#include "control_player.h"

// TODO: REMOVE THIS
#define MAX_INIT_Y_POS ((YSize)+(Y_OFFSET)-19)

#if ((YSize)+(Y_OFFSET)-1)>19
    #define MAX_TILES 19
#else
    #define MAX_TILES ((YSize)+(Y_OFFSET)-1)
#endif


extern Image VERTICAL_HEAD_IMAGE;  
extern Image HORIZONTAL_HEAD_IMAGE; 

extern Image BODY_IMAGE; 

extern Image EXTRA_POINTS_IMAGE; 

extern Image SCORE_TEXT_LEFT_IMAGE;
extern Image SCORE_TEXT_RIGHT_IMAGE;
extern Image HI_TEXT_IMAGE;
extern Image LV_TEXT_IMAGE;

extern SnakeBody snake[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;

extern uint8_t map[XSize][YSize];

extern Image VERTICAL_BRICK_IMAGE;
extern Image HORIZONTAL_BRICK_IMAGE;

static uint8_t snake_head_x;
static uint8_t snake_head_y;

static uint16_t points;

static uint8_t speed_increase_counter;

static uint16_t slow_down;

static uint8_t lives;

static uint16_t record;

#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3

#define SPEED_INCREASE_THRESHOLD 10

#define hits_snake(x,y) \
    (map[x][y]==SNAKE)

#define hits_bonus(x,y) \
    (map[x][y]==BONUS)

#define IF_POSSIBLE_INCREASE_SPEED() \
    if(slow_down>SLOW_DOWN/40) \
    { \
        slow_down -= SLOW_DOWN/40; \
    }

#define IF_POSSIBLE_DECREASE_SPEED() \
    if(slow_down<SLOW_DOWN) \
    { \
        slow_down += SLOW_DOWN/8; \
    }


#define INIT_LIVES 3
#define BONUS_POINTS 10

void PRESS_KEY(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINT(COL_OFFSET,YSize-5, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
    WAIT_PRESS();
}

void spawn_bonus(void)
{
    uint8_t x;
    uint8_t y;
    
    TICK_SOUND();
    while(1)
    {
        x = RAND()%XSize;
        y = RAND()%YSize;
        
        if(!(map[x][y]) && !hits_wall(x,y))
        {
            break;
        }
    }
    map[x][y]=BONUS;
    
    _XLIB_DRAW(x,y,&EXTRA_POINTS_IMAGE);
}

void DISPLAY_POINTS(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINTD(2,0,5,points);
}


int main(void)
{        


    INIT_GRAPHICS();

    INIT_INPUT();
    
    INIT_IMAGES();

    record = 0;

    while(1)
    {
        CLEAR_SCREEN();
        SET_TEXT_COLOR(COLOR_RED);
        PRINT(COL_OFFSET+3,YSize/5, _XL_C _XL_R _XL_O _XL_S _XL_S _XL_SPACE _XL_S _XL_N _XL_A _XL_K _XL_E);

        SET_TEXT_COLOR(COLOR_WHITE);
        PRINT(COL_OFFSET+8,YSize/5+2, _XL_b _XL_y);
        PRINT(COL_OFFSET+1,YSize/5+4, _XL_F _XL_a _XL_b _XL_r _XL_i _XL_z _XL_i _XL_o _XL_SPACE _XL_C _XL_a _XL_r _XL_u _XL_s _XL_o);

        PRESS_KEY();
        
        if(points>record)
        {
            record = points;
        }
        
        points = 0;

        lives = INIT_LIVES;

        while(lives)
        {
            CLEAR_SCREEN();
            DRAW_BORDERS();
            
            _XLIB_DRAW(XSize-3,0,&VERTICAL_HEAD_IMAGE);
            
            _XLIB_DRAW(0,0,&SCORE_TEXT_LEFT_IMAGE);
            _XLIB_DRAW(1,0,&SCORE_TEXT_RIGHT_IMAGE);
            _XLIB_DRAW(XSize-10,0,&HI_TEXT_IMAGE);
            
            SET_TEXT_COLOR(COLOR_WHITE);
            
            PRINTD(XSize-2,0,2,lives);
            


            DISPLAY_POINTS();
            
            PRINTD(XSize-9,0,5,record);
            
            init_map();
            
            speed_increase_counter = 0;
            
            slow_down = SLOW_DOWN;
            
            init_snake();
            spawn_bonus();
            
            
            WAIT_PRESS();
            while(1)
            {
                if(MOVE_PLAYER())
                {
                    DO_SLOW_DOWN(slow_down);
                    ++speed_increase_counter;
                    if((speed_increase_counter>SPEED_INCREASE_THRESHOLD) && (snake_length<MAX_SNAKE_LENGTH))
                    {
                        speed_increase_counter = 0;
                        if(!(RAND()&3))
                        {
                            spawn_bonus();
                        }
                        ++points;
                        IF_POSSIBLE_INCREASE_SPEED();
                    }
                    
                    snake_head_x = snake[snake_head].x;
                    snake_head_y = snake[snake_head].y;
                    
                    
                    DISPLAY_POINTS();
                    
                    if(hits_bonus(snake_head_x,snake_head_y))
                    {
                        snake_grows();
                        snake_head_x = snake[snake_head].x;
                        snake_head_y = snake[snake_head].y;
                        
                        points+=BONUS_POINTS;
                        ZAP_SOUND();
                        IF_POSSIBLE_DECREASE_SPEED();
                    }
                    
                    if(hits_snake(snake_head_x,snake_head_y))
                    {
                        --lives;
                        break;
                    }
                }
                else
                {
                    if(speed_increase_counter)
                    {
                        IF_POSSIBLE_INCREASE_SPEED();
                        speed_increase_counter = 0;
                    }
                }
            }
            EXPLOSION_SOUND();
            PRESS_KEY();
        }

        PRINT(COL_OFFSET,YSize/2, _XL_SPACE _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R _XL_SPACE);
        WAIT_PRESS();
    }
    
    return EXIT_SUCCESS;
}

