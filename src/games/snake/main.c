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

extern Image CENTRAL_BRICK_IMAGE;

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

static uint8_t remaining_apples;

static uint8_t level;

static uint8_t energy;

static uint8_t total_apples_on_level;

static uint8_t bonus_count;

#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3

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
#define BONUS_POINTS 5
#define APPLE_POINTS 20

#define INIT_APPLE_COUNT 5

#define INIT_APPLES_ON_SCREEN 4

#define APPLE_COUNT_INCREASE 5

#define MAX_BONUS_COUNT 5

#define BONUS_THRESHOLD 30

#define SPEED_INCREASE_THRESHOLD 20

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
        x = (uint8_t)(RAND()%XSize);
        y = (uint8_t)(RAND()%YSize);
        
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

void DISPLAY_REMAINING_APPLES_COUNT(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINTD(9,0,2,remaining_apples);
}


#define INITIAL_LEVEL 15

static uint8_t apples_on_screen_count;

static uint8_t level_walls[] = 
{
// level 1
    2, 
        XSize/3,       YSize/3,     XSize/3,
        XSize/3,     2*YSize/3,     XSize/3,
    2,  
        XSize/5,             1,     YSize/4,
        4*XSize/5,   3*YSize/4,     YSize/4,
    0,

// level 2
    0,
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,
    0,

// level 3
    4,
        0,                      YSize/4,      XSize/4,
       3*XSize/4,               YSize/4,      XSize/4,
        0,                    3*YSize/4,      XSize/4,
       3*XSize/4,             3*YSize/4,      XSize/4,
    2,
        XSize/2,                      1,      YSize/4,
        XSize/2,        YSize-1-YSize/4,      YSize/4,
    0,
        
// level 4
    4,
             0,                 YSize/5,      XSize/2,
       XSize-1-XSize/2,       2*YSize/5,      XSize/2,
             0,               3*YSize/5,      XSize/2,
       XSize-1-XSize/2,       4*YSize/5,      XSize/2,
    0,
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
    0,
// level 6
    0,
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,
    2,
        1,1,XSize/8,YSize/8,
        XSize-1-XSize/8,YSize-1-YSize/8,XSize/8,YSize/8,
// level 7
    2,
        0, YSize/2, XSize/4,
        XSize-XSize/4, YSize/2,XSize/4,
    4,
        XSize/2, 3, YSize/4,
        XSize/2, YSize-4-YSize/4, YSize/4,
        XSize/2-1, 3, YSize/4,
        XSize/2-1, YSize-4-YSize/4, YSize/4,
    4,
        1,1,XSize/6,YSize/6,
        1,YSize-1-YSize/6,XSize/6,YSize/6,
        XSize-1-XSize/6,YSize-1-YSize/6,XSize/6,YSize/6,
        XSize-1-XSize/6,1,XSize/6,YSize/6,
// level 8
    0,
    4, 
        XSize/6, 3, YSize-1-6,
        2*XSize/6, 3, YSize-1-6,
        XSize-1-2*XSize/6, 3, YSize-1-6,
        XSize-1-XSize/6, 3, YSize-1-6,
    0,
// level 9
    0,
    0,
    4,
        2,2,XSize/3,YSize/3,
        2,YSize-2-YSize/3,XSize/3,YSize/3,
        XSize-2-XSize/3,YSize-2-YSize/3,XSize/3,YSize/3,
        XSize-2-XSize/3,2,XSize/3,YSize/3,
// level 10
    2, 
        XSize/3,       YSize/3,     XSize/2-1,
        XSize/3,     2*YSize/3,     XSize/2-1,
    0,
    0,
// level 11
    2,
        XSize/4, YSize/2-2,XSize/2,
        XSize/4, YSize/2+2,XSize/2,
    0,
    4,
        2,2,XSize/3,YSize/3,
        2,YSize-2-YSize/3,XSize/3,YSize/3,
        XSize-2-XSize/3,YSize-2-YSize/3,XSize/3,YSize/3,
        XSize-2-XSize/3,2,XSize/3,YSize/3,
// level 12
    1,
        2,YSize/2-1,XSize-4,
    0,
    0,
// level 13
    0,
    4,
        XSize/5,                      0,    4*YSize/5,
       2*XSize/5,               YSize/5,    4*YSize/5,
       3*XSize/5,                      0,   4*YSize/5,
       4*XSize/5,               YSize/5,    4*YSize/5,
    0,
// level 14
    4,
        0,                      YSize/4,      XSize/4,
       3*XSize/4,               YSize/4,      XSize/4,
        0,                    3*YSize/4,      XSize/4,
       3*XSize/4,             3*YSize/4,      XSize/4,
// level 15
    1,
        5,YSize/2-1,XSize-10,
    0,
    1,
        XSize/2-3,YSize/2+2,6,6,
// level 16
    0,
    0,
    0,
};


static uint8_t level_walls_index[] = {0,15,24,45,60,87,104,141,156,175,184,209,215,230,243,253};


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

void build_box_wall(uint8_t x, uint8_t y, uint8_t x_length, uint8_t y_length)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<x_length;++i)
    {
        for(j=0;j<y_length;++j)
        {
            map[x+i][y+j]=WALL;
            _XLIB_DRAW(x+i,y+j,&CENTRAL_BRICK_IMAGE);
        }
    }
}


void build_level(uint8_t level)
{
    uint16_t index = level_walls_index[(level-1)&15];

    uint16_t i;
    uint16_t number_of_elements;
    
    // printf("level: %u\n", level);
    
    // printf("horizontal index: %u\n", index);

    number_of_elements = level_walls[index]; // Number of horizontal walls
    // printf("Number of horizontal walls: %u\n", number_of_elements);
    for(i=0;i<3*number_of_elements;i+=3)
    {
        // printf("\n%u %u %u\n", level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
        build_horizontal_wall(level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
    }
    index = index+number_of_elements*3+1;
    // printf("vertical index: %u\n", index);
    number_of_elements = level_walls[index]; // Number of vertical walls
    // printf("Number of vertical walls: %u\n", number_of_elements);
    for(i=0;i<3*number_of_elements;i+=3)
    {
        // printf("%u %u %u\n", level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
        build_vertical_wall(level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
    }
    
    index = index+number_of_elements*3+1;
    
    number_of_elements = level_walls[index];
    for(i=0;i<4*number_of_elements;i+=4)
    {
        build_box_wall(level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i],level_walls[index+4+i]);
    }
    
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
        total_apples_on_level=INIT_APPLE_COUNT+APPLE_COUNT_INCREASE;
        remaining_apples = total_apples_on_level;
        lives = INIT_LIVES;
        level = INITIAL_LEVEL;

        while(lives)
        {
            CLEAR_SCREEN();
            DRAW_BORDERS();
            
            bonus_count = 0;
            
            _XLIB_DRAW(XSize-3,0,&VERTICAL_HEAD_IMAGE);
            
            _XLIB_DRAW(0,0,&SCORE_TEXT_LEFT_IMAGE);
            _XLIB_DRAW(1,0,&SCORE_TEXT_RIGHT_IMAGE);
            _XLIB_DRAW(XSize-10,0,&HI_TEXT_IMAGE);
            _XLIB_DRAW(8,0,&APPLE_IMAGE);
            _XLIB_DRAW(0,YSize-1,&LV_TEXT_IMAGE);
            
            SET_TEXT_COLOR(COLOR_WHITE);
            
            PRINTD(XSize-2,0,2,lives);
            PRINTD(1,YSize-1,2,level);
            
            DISPLAY_REMAINING_APPLES_COUNT();

            DISPLAY_POINTS();
            
            PRINTD(XSize-9,0,5,record);
            
            energy = 99;
            PRINTD(XSize/2-1,YSize-1,2,energy);
            init_map();
            
            speed_increase_counter = 0;
            
            slow_down = SLOW_DOWN;
            
            init_snake(level);
            
            build_level(level);
            
            apples_on_screen_count = 1;
            spawn(APPLE, &APPLE_IMAGE);

            
            energy = 99;
            
            WAIT_PRESS();
            while(remaining_apples)
            {
                # if defined(DEBUG_APPLES)
                PRINTD(XSize-8,YSize-1,2,apples_on_screen_count);
                #endif
                if(MOVE_PLAYER())
                {
                    DO_SLOW_DOWN(slow_down);
                    ++speed_increase_counter;
                    if((speed_increase_counter>SPEED_INCREASE_THRESHOLD))
                    {
                        speed_increase_counter = 0;
                        if(!(RAND()&1) && ((!apples_on_screen_count) || (apples_on_screen_count<remaining_apples)))
                        {
                            if(!(RAND()&7))
                            {
                                spawn(BONUS, &EXTRA_POINTS_IMAGE);
                            }
                            else
                            {
                                ++apples_on_screen_count;
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
                        

                        points+=BONUS_POINTS<<bonus_count;
                        if(bonus_count<MAX_BONUS_COUNT)
                        {
                            ++bonus_count;
                        }
                        _XLIB_DRAW(XSize-3-MAX_BONUS_COUNT+bonus_count,YSize-1,&EXTRA_POINTS_IMAGE);
                        ZAP_SOUND();
                    }
                    if(hits_apple(snake_head_x,snake_head_y))
                    {
                        --apples_on_screen_count;
                        snake_grows();
                        snake_head_x = snake[snake_head].x;
                        snake_head_y = snake[snake_head].y;
                        
                        --remaining_apples;
                        DISPLAY_REMAINING_APPLES_COUNT();
                        points+=APPLE_POINTS;
                        ZAP_SOUND();
                        IF_POSSIBLE_DECREASE_SPEED();
                    }
                    
                    if(hits_snake(snake_head_x,snake_head_y) || !remaining_apples)
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
            if(remaining_apples)
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
                total_apples_on_level=INIT_APPLE_COUNT+level*APPLE_COUNT_INCREASE;
                remaining_apples = total_apples_on_level;
                WAIT_PRESS();
            }
        }

        PRINT(COL_OFFSET,YSize/2, _XL_SPACE _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R _XL_SPACE);
        WAIT_PRESS();
    }
    
    return EXIT_SUCCESS;
}

