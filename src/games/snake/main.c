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

extern Image TRANSPARENT_BRICK_IMAGE;

extern Image MINE_IMAGE;

extern Image LEFT_MINE_IMAGE;
extern Image RIGHT_MINE_IMAGE;

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

uint8_t level;

static uint8_t energy;

static uint8_t bonus_count;

static uint8_t extra_life_counter;

static uint8_t active_mines;

static uint16_t level_bonus;

#define FINAL_LEVEL 32

#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3

#define hits_bonus(x,y) \
    (map[x][y]==BONUS)

#define hits_apple(x,y) \
    (map[x][y]==APPLE)

#define RED_ENERGY_THRESHOLD 80

void DISPLAY_ENERGY(void)
{
   if(energy<RED_ENERGY_THRESHOLD)
   {
       SET_TEXT_COLOR(COLOR_RED);
   }
   else
   {
       SET_TEXT_COLOR(COLOR_WHITE);
   }
   PRINTD(XSize/2-1,YSize-1,2,energy); 
}


#define IF_POSSIBLE_INCREASE_SPEED() \
    if(slow_down>SLOW_DOWN/40) \
    { \
        slow_down -= SLOW_DOWN/40; \
    } \
    else \
    { \
       --energy; \
       DISPLAY_ENERGY(); \
    }

#define IF_POSSIBLE_DECREASE_SPEED() \
    if(slow_down<SLOW_DOWN) \
    { \
        slow_down += SLOW_DOWN/6; \
    }


#define INITIAL_LIVES 5
#define BONUS_POINTS 50
#define APPLE_POINTS 20
#define EXTRA_LIFE_THRESHOLD 5000U

#define INITIAL_APPLE_COUNT 10

#define APPLE_COUNT_INCREASE 2

#define MAX_BONUS_COUNT 3

#define SPEED_INCREASE_THRESHOLD 20



void PRESS_KEY(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    printCenteredMessageOnRow(YSize-5, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
    WAIT_PRESS();
}

uint8_t empty_around(uint8_t x, uint8_t y)
{
return 
!map[x][y] && 
       (map[x-1][y-1]!=DEADLY) && (map[x][y-1]!=DEADLY) && (map[x+1][y-1]!=DEADLY) &&
       (map[x-1][y]!=DEADLY) && (map[x+1][y]!=DEADLY) &&
       (map[x-1][y+1]!=DEADLY) && (map[x][y+1]!=DEADLY) && (map[x+1][y+1]!=DEADLY);

}

void spawn(uint8_t item, Image *image_ptr)
{
    uint8_t x;
    uint8_t y;
    
    TICK_SOUND();
    while(1)
    {
        x = (uint8_t)(RAND()%(XSize-2)+1);
        y = (uint8_t)(RAND()%(YSize-2)+1);
        
        if(empty_around(x,y))
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


#define INITIAL_LEVEL 1
// #define DEBUG_LEVELS

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
        XSize/4,                YSize/4,              XSize/5,
        XSize/4+XSize/2-XSize/5,        YSize/4,      XSize/5,
        XSize/4,        YSize-1-YSize/4,              XSize/5,
        XSize/4+XSize/2-XSize/5,YSize-1-YSize/4,      XSize/5,
    4,  
        XSize/4,                YSize/4,              YSize/5,
        XSize-1-XSize/4,YSize/4,                      YSize/5,
        XSize/4,     YSize-YSize/4-YSize/5,         YSize/5,
        XSize-1-XSize/4,YSize-YSize/4-YSize/5,      YSize/5,
    0,
// level 6
    0,
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,
    2,
        1,1,XSize/8,YSize/8,DEADLY,
        XSize-1-XSize/8,YSize-1-YSize/8,XSize/8,YSize/8,DEADLY,
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
        1,1,XSize/6,YSize/6,DEADLY,
        1,YSize-1-YSize/6,XSize/6,YSize/6,WALL,
        XSize-1-XSize/6,YSize-1-YSize/6,XSize/6,YSize/6,DEADLY,
        XSize-1-XSize/6,1,XSize/6,YSize/6,WALL,
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
        2,2,XSize/3,YSize/3,WALL,
        2,YSize-2-YSize/3,XSize/3,YSize/3,WALL,
        XSize-2-XSize/3,YSize-2-YSize/3,XSize/3,YSize/3,WALL,
        XSize-2-XSize/3,2,XSize/3,YSize/3,WALL,
// level 10
    2, 
        XSize/3-1,       YSize/3,     XSize/2-1,
        XSize/3-1,     2*YSize/3,     XSize/2-1,
    0,
    0,
// level 11
    2,
        XSize/4, YSize/2-2,XSize/2,
        XSize/4, YSize/2+2,XSize/2,
    0,
    4,
        2,2,XSize/3,YSize/3,TRANSPARENT,
        2,YSize-2-YSize/3,XSize/3,YSize/3,TRANSPARENT,
        XSize-2-XSize/3,YSize-2-YSize/3,XSize/3,YSize/3,TRANSPARENT,
        XSize-2-XSize/3,2,XSize/3,YSize/3,TRANSPARENT,
// level 12
    1,
        2,YSize/2-1,XSize-4,
    0,
    0,
// level 13
    0,
    4,
        XSize/5,                      1,    4*YSize/5,
       2*XSize/5,               YSize/5-1,    4*YSize/5,
       3*XSize/5,                      1,   4*YSize/5,
       4*XSize/5,               YSize/5-1,    4*YSize/5,
    0,
// level 14
    4,
        0,                      YSize/4,      XSize/4,
       3*XSize/4,               YSize/4,      XSize/4,
        0,                    3*YSize/4,      XSize/4,
       3*XSize/4,             3*YSize/4,      XSize/4,
    0,
    0,
// level 15
    1,
        5,YSize/2-1,XSize-10,
    0,
    1,
        XSize/2-3,YSize/2+2,6,6,DEADLY,
// level 16
    0,
    4,
        XSize/6, YSize-1-YSize/3, YSize/3,
        XSize-1-XSize/6, YSize-1-YSize/3, YSize/3,
        XSize/3, YSize-1-YSize/3, YSize/3,
        XSize-1-XSize/3, YSize-1-YSize/3, YSize/3,
    0,
};


static uint16_t level_walls_index[] = 
    {
        0,   // 1
        15,  // 2
        24,  // 3
        45,  // 4
        60,  // 5
        87,  // 6
        106, // 7
        147, // 8
        162, // 9
        185, // 10
        194, // 11
        223, // 12
        229, // 13 
        244, // 14
        259, // 15
        270, // 16
        };


static Image* images[] = {NULL, &MINE_IMAGE, NULL, NULL, &CENTRAL_BRICK_IMAGE, &HORIZONTAL_BRICK_IMAGE, &VERTICAL_BRICK_IMAGE, &TRANSPARENT_BRICK_IMAGE};

void build_box_wall(uint8_t x, uint8_t y, uint8_t x_length, uint8_t y_length, uint8_t type)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<x_length;++i)
    {
        for(j=0;j<y_length;++j)
        {
            map[x+i][y+j]=type;
            _XLIB_DRAW(x+i,y+j,images[type]);

        }
    }
}

#define build_horizontal_wall(x,y,length) \
    build_box_wall(x, y, length, 1, HORIZONTAL_WALL)

#define build_vertical_wall(x,y,length) \
    build_box_wall(x, y, 1, length, VERTICAL_WALL)

void DRAW_GAME_BORDERS(void)
{
    build_horizontal_wall(0,0,XSize);
    build_horizontal_wall(0,YSize-1,XSize);
    build_vertical_wall(0,0,YSize);
    build_vertical_wall(XSize-1,0,YSize);
}

#define MAX_NUMBER_OF_MINES 4

#define MAX_APPLES 50

static uint8_t mine_x[MAX_NUMBER_OF_MINES];
static uint8_t mine_y[MAX_NUMBER_OF_MINES];
static uint8_t mine_direction[MAX_NUMBER_OF_MINES];
static uint8_t mines_on_current_level;
static uint8_t mine_transition[MAX_NUMBER_OF_MINES];

static uint8_t mines_on_level[2*NUMBER_OF_LEVELS] = 
    {
        0, // 1
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1, // 10
        0,
        2, // 12
        0,
        2, // 14
        2,
        3, // 16
        1, // 17
        1,
        1,
        1, // 20
        1,
        1,
        1, // 23
        1,
        1, // 25
        2,
        1, // 27
        3,
        1, // 29
        2,
        3,
        4
    };
       

#define MINE_RIGHT 0
#define MINE_LEFT 1
#define MINE_UP 2
#define MINE_DOWN 3

uint8_t tight_level(void) 
{
    return (level==20) || (level==26) || (level==27) || (level==29);
}

void build_level(void)
{
    register uint16_t index = level_walls_index[(level-1)&15];

    register uint16_t i;
    uint16_t number_of_elements;
    uint8_t j;
    uint8_t y_offset;
    

    for(j=0;j<2;++j)
    {
        number_of_elements = level_walls[index]; // Number of horizontal walls
        for(i=0;i<3*number_of_elements;i+=3)
        {
            if(j)
            {
                build_vertical_wall(level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
            }
            else
            {
                build_horizontal_wall(level_walls[index+1+i],level_walls[index+2+i],level_walls[index+3+i]);
            }
        }
        index = index+number_of_elements*3+1;
    }

    
    number_of_elements = level_walls[index];
    for(i=0;i<5*number_of_elements;i+=5)
    {
        build_box_wall(level_walls[index+1+i],level_walls[index+2+i],
                       level_walls[index+3+i],level_walls[index+4+i],
                       level_walls[index+5+i]);
    }
    
    mines_on_current_level = mines_on_level[level-1];
    
    if(!tight_level())
    {
        y_offset = 3u;
    }
    else
    {
        y_offset = 1u;
    }
    
    for(j=0;j<mines_on_current_level;++j)
    {
        mine_x[j] = XSize/2;
        mine_y[j] = YSize/2 - y_offset - j;
        mine_direction[j] = j&1;
        _XLIB_DRAW(mine_x[j],mine_y[j],&MINE_IMAGE);
        map[mine_x[j]][mine_y[j]]=DEADLY;
        mine_transition[j] = 0;
    }
    
    
}


void handle_horizontal_mine(register uint8_t index)
{
    if(mine_direction[index]==MINE_LEFT)
    {
        
        if(!mine_transition[index]) // transition not performed, yet
        {
            if(!map[mine_x[index]-1][mine_y[index]] && mine_x[index]-1)
            {
                // Do left transition
                _XLIB_DRAW(mine_x[index],mine_y[index],&RIGHT_MINE_IMAGE);
                _XLIB_DRAW(mine_x[index]-1,mine_y[index],&LEFT_MINE_IMAGE);
                map[mine_x[index]-1][mine_y[index]]=DEADLY;
                ++mine_transition[index];
            }
            else
            {
                mine_direction[index]=MINE_RIGHT;
            }
        }
        else // transition already performed
        {
            mine_transition[index]=0;
            map[mine_x[index]][mine_y[index]]=EMPTY;
            _XLIB_DELETE(mine_x[index],mine_y[index]);
            --mine_x[index];
            _XLIB_DRAW(mine_x[index],mine_y[index],&MINE_IMAGE);
        }
    }
    else
    {
        if(!mine_transition[index]) // transition not performed, yet
        {
            if(!map[mine_x[index]+1][mine_y[index]] && mine_x[index]<XSize-2)
            {
                // Do right transition
                _XLIB_DRAW(mine_x[index],mine_y[index],&LEFT_MINE_IMAGE);
                _XLIB_DRAW(mine_x[index]+1,mine_y[index],&RIGHT_MINE_IMAGE);
                map[mine_x[index]+1][mine_y[index]]=DEADLY;
                ++mine_transition[index];
            }
            else
            {
                mine_direction[index]=MINE_LEFT;
            }
        }
        else // transition already performed
        {
            mine_transition[index]=0;
            map[mine_x[index]][mine_y[index]]=EMPTY;
            _XLIB_DELETE(mine_x[index],mine_y[index]);
            ++mine_x[index];
            _XLIB_DRAW(mine_x[index],mine_y[index],&MINE_IMAGE);
        }
    }
}


void handle_mines(void)
{
    uint8_t i;
    
    for(i=0;i<mines_on_current_level;++i)
    {
        handle_horizontal_mine(i);
    }
}

void DISPLAY_LIVES(void)
{
    PRINTD(XSize-1,0,1,lives);
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
        printCenteredMessageOnRow(YSize/5, _XL_C _XL_R _XL_O _XL_S _XL_S _XL_SPACE _XL_S _XL_N _XL_A _XL_K _XL_E);

        SET_TEXT_COLOR(COLOR_WHITE);
        printCenteredMessageOnRow(YSize/5+2, _XL_b _XL_y);
        printCenteredMessageOnRow(YSize/5+4, _XL_F _XL_a _XL_b _XL_r _XL_i _XL_z _XL_i _XL_o _XL_SPACE _XL_C _XL_a _XL_r _XL_u _XL_s _XL_o);

        PRESS_KEY();
        
        if(points>record)
        {
            record = points;
        }
        
        extra_life_counter = 1;
        points = 0;
        
        remaining_apples=INITIAL_APPLE_COUNT+APPLE_COUNT_INCREASE;
        
        lives = INITIAL_LIVES;
        level = INITIAL_LEVEL;

        while(lives && (level<FINAL_LEVEL+1))
        {
            #if defined(DEBUG_LEVELS)
            debug_levels:
            #endif
            CLEAR_SCREEN();
            DRAW_GAME_BORDERS();
            
            bonus_count = 0;
            
            _XLIB_DRAW(XSize-2,0,&VERTICAL_HEAD_IMAGE);
            
            _XLIB_DRAW(0,0,&SCORE_TEXT_LEFT_IMAGE);
            _XLIB_DRAW(1,0,&SCORE_TEXT_RIGHT_IMAGE);
            _XLIB_DRAW(XSize-10,0,&HI_TEXT_IMAGE);
            _XLIB_DRAW(8,0,&APPLE_IMAGE);
            _XLIB_DRAW(0,YSize-1,&LV_TEXT_IMAGE);
            
            SET_TEXT_COLOR(COLOR_WHITE);
            
            DISPLAY_LIVES();
            PRINTD(1,YSize-1,2,level);
            
            DISPLAY_REMAINING_APPLES_COUNT();

            DISPLAY_POINTS();
            
            PRINTD(XSize-9,0,5,record);
            
            energy = 99;
            DISPLAY_ENERGY();
            init_map();
            
            speed_increase_counter = 0;
            
            slow_down = SLOW_DOWN;
            
            init_snake();
            
            build_level();
            
            apples_on_screen_count = 1;
            spawn(APPLE, &APPLE_IMAGE);

            if(tight_level())
            {
                for(i=0;i<MAX_BONUS_COUNT;++i)
                {
                    spawn(BONUS, &EXTRA_POINTS_IMAGE);
                }
            }
            
            energy = 99;
            
            WAIT_PRESS();
            
            #if defined(DEBUG_LEVELS)
                ++level;
                goto debug_levels;
            #endif
            
            active_mines = 1;
            
            while(remaining_apples)
            {
                if(points>extra_life_counter*EXTRA_LIFE_THRESHOLD)
                {
                    ++extra_life_counter;
                    ++lives;
                    DISPLAY_LIVES();
                    SHOOT_SOUND();
                }
                # if defined(DEBUG_APPLES)
                PRINTD(XSize-8,YSize-1,2,apples_on_screen_count);
                #endif
                
                
                if(MOVE_PLAYER())
                {
                    if(active_mines)
                    {
                        handle_mines();
                    }
                    DO_SLOW_DOWN(slow_down);
                    ++speed_increase_counter;
                    if((!(apples_on_screen_count) || (speed_increase_counter>SPEED_INCREASE_THRESHOLD)))
                    {
                        speed_increase_counter = 0;
                        if(apples_on_screen_count<remaining_apples)
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
                        

                        points+=(BONUS_POINTS<<bonus_count);
                        if(bonus_count<MAX_BONUS_COUNT)
                        {
                            ++bonus_count;
                        }
                        else
                        {
                            slow_down = 2*SLOW_DOWN;
                            energy = 99;
                            DISPLAY_ENERGY();
                            active_mines = 0;
                            TOCK_SOUND();
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
                    
                    if(hits_deadly_item(snake_head_x,snake_head_y) || !remaining_apples)
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
                    printCenteredMessageOnRow(YSize/2, _XL_SPACE _XL_N _XL_O _XL_SPACE _XL_E _XL_N _XL_E _XL_R _XL_G _XL_Y _XL_SPACE);
                    
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
                printCenteredMessageOnRow(YSize/2, _XL_SPACE _XL_L _XL_E _XL_V _XL_E _XL_L _XL_SPACE _XL_C _XL_L _XL_E _XL_A _XL_R _XL_E _XL_D _XL_SPACE);
                level_bonus = (uint16_t) (((uint16_t) snake_length)<<1)+(((uint16_t) energy)) +(((uint16_t) bonus_count)<<5) + (((uint16_t) level)<<3);
                SET_TEXT_COLOR(COLOR_WHITE);

                printCenteredMessageOnRow(YSize/2+2, _XL_SPACE _XL_B _XL_O _XL_N _XL_U _XL_S _XL_SPACE);
                PRINTD(XSize/2-3,YSize/2+4,5,level_bonus);
                ++level;
                
                remaining_apples=INITIAL_APPLE_COUNT+level*APPLE_COUNT_INCREASE;
                if(remaining_apples>MAX_APPLES)
                {
                    remaining_apples = MAX_APPLES;
                }
                
                points+=level_bonus;
                WAIT_PRESS();
            }
        }
        if(level>FINAL_LEVEL)
        {
            CLEAR_SCREEN();
            SET_TEXT_COLOR(COLOR_WHITE);
            printCenteredMessageOnRow(YSize/2-3, _XL_SPACE _XL_T _XL_H _XL_E _XL_SPACE _XL_E _XL_N _XL_D _XL_SPACE);
        }

        printCenteredMessageOnRow(YSize/2, _XL_SPACE _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R _XL_SPACE);
        WAIT_PRESS();
    }
    
    return EXIT_SUCCESS;
}

