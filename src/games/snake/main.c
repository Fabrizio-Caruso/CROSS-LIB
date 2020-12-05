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

extern Image APPLE_IMAGE;

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

static uint8_t apple_count;

static uint8_t level;

static uint8_t energy;

#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3

#define SPEED_INCREASE_THRESHOLD 15

#define hits_bonus(x,y) \
    (map[x][y]==BONUS)

#define hits_apple(x,y) \
    (map[x][y]==APPLE)


#define IF_POSSIBLE_INCREASE_SPEED() \
    if(slow_down>SLOW_DOWN/40) \
    { \
        slow_down -= SLOW_DOWN/40; \
    } \
    else \
    { \
       --energy; \
       SET_TEXT_COLOR(COLOR_RED); \
       PRINTD(XSize/2-1,YSize-1,2,energy); \
    }

#define IF_POSSIBLE_DECREASE_SPEED() \
    if(slow_down<SLOW_DOWN) \
    { \
        slow_down += SLOW_DOWN/6; \
    }


#define INIT_LIVES 3
#define BONUS_POINTS 30
#define APPLE_POINTS 20

#define INIT_APPLE_COUNT 10

#define INIT_APPLES_ON_SCREEN 4

#define APPLE_COUNT_INCREASE 5


void PRESS_KEY(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINT(COL_OFFSET,YSize-5, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
    WAIT_PRESS();
}

void spawn(uint8_t item, Image *image_ptr)
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
    map[x][y]=item;
    
    _XLIB_DRAW(x,y,image_ptr);
}


void DISPLAY_POINTS(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINTD(2,0,5,points);
}

void DISPLAY_APPLE_COUNT(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINTD(9,0,2,apple_count);
}

#define INITIAL_LEVEL 1

static uint16_t level_walls[] = 
{
// level 1
    2, 
        XSize/3,       YSize/3,     XSize/3,
        XSize/3,     2*YSize/3,     XSize/3,
    2,  
        XSize/5,             1,     YSize/4,
        4*XSize/5,   3*YSize/4,     YSize/4,

// level 2
    0,
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,

// level 3
    4,
        0,                      YSize/4,      XSize/4,
       3*XSize/4,               YSize/4,      XSize/4,
        0,                    3*YSize/4,      XSize/4,
       3*XSize/4,             3*YSize/4,      XSize/4,
    2,
        XSize/2,                      1,      YSize/4,
        XSize/2,        YSize-1-YSize/4,      YSize/4,
        
// level 4
    4,
             0,                 YSize/5,      XSize/2,
       XSize-1-XSize/2,       2*YSize/5,      XSize/2,
             0,               3*YSize/5,      XSize/2,
       XSize-1-XSize/2,       4*YSize/5,      XSize/2,
    0,
// level 5
    4,
        XSize/4,                YSize/4,              XSize/8,
        XSize/4+XSize/2-XSize/8,        YSize/4,      XSize/8,
        XSize/4,        YSize-1-YSize/4,              XSize/8,
        XSize/4+XSize/2-XSize/8,YSize-1-YSize/4,      XSize/8,
    4,  
        XSize/4,                YSize/4,              YSize/8,
        XSize-1-XSize/4,YSize/4,                      YSize/8,
        XSize/4,     YSize-1-YSize/4-YSize/8,         YSize/8,
        XSize-1-XSize/4,YSize-1-YSize/4-YSize/8,      YSize/8,
// level 6
    8, 
        0, 1, XSize/2,
        0, 2, XSize/2,
        0, 3, XSize/2,
        0, 4, XSize/2,
        
        XSize-1-XSize/2,YSize-5,XSize/2,
        XSize-1-XSize/2,YSize-4,XSize/2,
        XSize-1-XSize/2,YSize-3,XSize/2,
        XSize-1-XSize/2,YSize-2,XSize/2,
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,
// level 7
    2,
        0, YSize/2, XSize/4,
        XSize-XSize/4, YSize/2,XSize/4,
    1,
        XSize/2, 3, YSize-1-6,
// level 8
    0,
    4, 
        XSize/6, 3, YSize-1-6,
        2*XSize/6, 3, YSize-1-6,
        XSize-1-2*XSize/6, 3, YSize-1-6,
        XSize-1-XSize/6, 3, YSize-1-6,
};

#define NUM_OF_LEVELS 1

static uint8_t level_walls_index[] = {0,14,22,42,56,82,114,125};


void build_horizontal_wall(uint8_t x, uint8_t y, uint8_t length)
{
    uint8_t i;
    
    for(i=0;i<length;++i)
    {
        map[x+i][y]=WALL;
    }
    DRAW_HORIZONTAL_LINE(x,y,length);
}

void build_vertical_wall(uint8_t x, uint8_t y, uint8_t length)
{
    uint8_t i;
    
    for(i=0;i<length;++i)
    {
        map[x][y+i]=WALL;
    }
    DRAW_VERTICAL_LINE(x,y,length);
}


void build_level(uint8_t level)
{
    uint16_t index = level_walls_index[(level-1)&7];

    uint16_t i;
    uint16_t max;
    
    // printf("level: %u\n", level);
    
    // printf("horizontal index: %u\n", index);

    max = level_walls[index]; // Number of horizontal walls
    // printf("Number of horizontal walls: %u\n", max);
    for(i=0;i<3*max;i+=3)
    {
        // printf("\n%u %u %u\n", level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
        build_horizontal_wall(level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
    }
    index = index+max*3+1;
    // printf("vertical index: %u\n", index);
    max = level_walls[index]; // Number of vertical walls
    // printf("Number of vertical walls: %u\n", max);
    for(i=0;i<3*max;i+=3)
    {
        // printf("%u %u %u\n", level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
        build_vertical_wall(level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
    }
}


int main(void)
{        

    uint8_t i;
    
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
        apple_count = INIT_APPLE_COUNT;
        lives = INIT_LIVES;
        level = INITIAL_LEVEL;

        while(lives)
        {
            CLEAR_SCREEN();
            DRAW_BORDERS();
            
            _XLIB_DRAW(XSize-3,0,&VERTICAL_HEAD_IMAGE);
            
            _XLIB_DRAW(0,0,&SCORE_TEXT_LEFT_IMAGE);
            _XLIB_DRAW(1,0,&SCORE_TEXT_RIGHT_IMAGE);
            _XLIB_DRAW(XSize-10,0,&HI_TEXT_IMAGE);
            _XLIB_DRAW(8,0,&APPLE_IMAGE);
            _XLIB_DRAW(0,YSize-1,&LV_TEXT_IMAGE);
            
            SET_TEXT_COLOR(COLOR_WHITE);
            
            PRINTD(XSize-2,0,2,lives);
            PRINTD(1,YSize-1,2,level);
            
            DISPLAY_APPLE_COUNT();

            DISPLAY_POINTS();
            
            PRINTD(XSize-9,0,5,record);
            
            energy = 99;
            PRINTD(XSize/2-1,YSize-1,2,energy);
            init_map();
            
            speed_increase_counter = 0;
            
            slow_down = SLOW_DOWN;
            
            init_snake();
            
            build_level(level);
            
            for(i=0;i<INIT_APPLES_ON_SCREEN+level/4;++i)
            {
                spawn(APPLE, &APPLE_IMAGE);
            }
            
            energy = 99;
            
            WAIT_PRESS();
            while(apple_count)
            {
                if(MOVE_PLAYER())
                {
                    DO_SLOW_DOWN(slow_down);
                    ++speed_increase_counter;
                    if((speed_increase_counter>SPEED_INCREASE_THRESHOLD) && (snake_length<MAX_SNAKE_LENGTH))
                    {
                        speed_increase_counter = 0;
                        if(!(RAND()&1))
                        {
                            if(!(RAND()&7))
                            {
                                spawn(BONUS, &EXTRA_POINTS_IMAGE);
                            }
                            else
                            {
                                spawn(APPLE, &APPLE_IMAGE);
                            }
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
                    }
                    if(hits_apple(snake_head_x,snake_head_y))
                    {
                        snake_grows();
                        snake_head_x = snake[snake_head].x;
                        snake_head_y = snake[snake_head].y;
                        
                        --apple_count;
                        DISPLAY_APPLE_COUNT();
                        points+=APPLE_POINTS;
                        ZAP_SOUND();
                        IF_POSSIBLE_DECREASE_SPEED();
                    }
                    
                    if(hits_snake(snake_head_x,snake_head_y) || !apple_count)
                    {
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
                if(!energy)
                {
                    SET_TEXT_COLOR(COLOR_RED);
                    PRINT(COL_OFFSET,YSize/2, _XL_SPACE _XL_N _XL_O _XL_SPACE _XL_E _XL_N _XL_E _XL_R _XL_G _XL_Y _XL_SPACE);
                    
                    break;
                }
            }
            if(apple_count)
            {
                --lives;
                EXPLOSION_SOUND();
                PRESS_KEY();
            }
            else
            {
                SET_TEXT_COLOR(COLOR_RED);
                PRINT(COL_OFFSET,YSize/2, _XL_SPACE _XL_L _XL_E _XL_V _XL_E _XL_L _XL_SPACE _XL_C _XL_L _XL_E _XL_A _XL_R _XL_E _XL_D _XL_SPACE);
                ++level;
                apple_count=INIT_APPLE_COUNT+level*APPLE_COUNT_INCREASE;
                WAIT_PRESS();
            }
        }

        PRINT(COL_OFFSET,YSize/2, _XL_SPACE _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R _XL_SPACE);
        WAIT_PRESS();
    }
    
    return EXIT_SUCCESS;
}

