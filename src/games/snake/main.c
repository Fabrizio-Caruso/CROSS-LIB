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
#include "game_text.h"


#include "settings.h"

#if ((YSize)+(Y_OFFSET)-1)>19
    #define MAX_TILES 19
#else
    #define MAX_TILES ((YSize)+(Y_OFFSET)-1)
#endif

#define transparent_vertical_wall_level()   (((level&15)==3)||((level&15)==5)||((level&15)==9)||((level&15)==14)||(!level))
#define transparent_horizontal_wall_level() (((level&15)==2)||((level&15)==6)||((level&15)==7)||((level&15)== 8)||((level&15)==13)||(!level))


extern Image VERTICAL_HEAD_IMAGE;  
extern Image HORIZONTAL_HEAD_IMAGE; 

extern Image BODY_IMAGE; 

extern Image EXTRA_IMAGE; 

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

extern uint8_t snake_x[MAX_SNAKE_LENGTH];
extern uint8_t snake_y[MAX_SNAKE_LENGTH];

extern uint8_t snake_length;

extern uint8_t snake_head;

extern Image *head_image_ptr;

extern uint8_t map[XSize][YSize];

extern Image VERTICAL_BRICK_IMAGE;
extern Image HORIZONTAL_BRICK_IMAGE;

extern Image COIN_IMAGE;
extern Image SUPER_COIN_IMAGE;

extern Image UP_MINE_IMAGE;
extern Image DOWN_MINE_IMAGE;

uint8_t snake_head_x;
uint8_t snake_head_y;

uint16_t points;

static uint8_t speed_increase_counter;

static uint16_t slow_down;

static uint8_t spawned_apples;

uint8_t lives;

static uint16_t record;

uint8_t remaining_apples;

uint8_t level;

uint8_t energy;

static uint8_t coin_count;

static uint8_t extra_life_counter;

static uint8_t active_mines;

static uint16_t level_bonus;

static uint8_t transparent_vertical_wall_triggered;
static uint8_t transparent_horizontal_wall_triggered;

static uint8_t secret_level_active;
static uint8_t secret_level_never_activated;
static uint8_t next_level;

/*
#define EMPTY 0
#define DEADLY 1
#define SUPER_COIN 2
#define COIN 3
#define EXTRA 4
#define APPLE 5
#define WALL  6
#define HORIZONTAL_WALL 7
#define VERTICAL_WALL 8
#define TRANSPARENT 9

*/


const Image *images[] = {
    0, 
    &MINE_IMAGE, 
    &SUPER_COIN_IMAGE, 
    &COIN_IMAGE, 
    &EXTRA_IMAGE, 
    &APPLE_IMAGE,
    &CENTRAL_BRICK_IMAGE, 
    &HORIZONTAL_BRICK_IMAGE, 
    &VERTICAL_BRICK_IMAGE, 
    &TRANSPARENT_BRICK_IMAGE
    };

#define FINAL_LEVEL 32

#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3

#define hits_coin(x,y) \
    (map[x][y]==COIN)

#define hits_super_coin(x,y) \
    (map[x][y]==SUPER_COIN)

#define hits_apple(x,y) \
    (map[x][y]==APPLE)
    
#define hits_extra_points(x,y) \
    (map[x][y]==EXTRA)



#define IF_POSSIBLE_INCREASE_SPEED() \
    if(slow_down>SLOW_DOWN/2) \
    { \
        slow_down -= SLOW_DOWN/7; \
    } \
    else \
    { \
       --energy; \
       DISPLAY_ENERGY(); \
    }

#define IF_POSSIBLE_DECREASE_SPEED() \
    if(slow_down<SLOW_DOWN) \
    { \
        slow_down += SLOW_DOWN/5; \
    }


// TODO: Maybe only horizontal and vertical checks are necessary
#define safe_around(x,y) \
    (!map[x][y] && \
       (map[x][(y)-1]!=DEADLY) && (map[(x)-1][y]!=DEADLY) && (map[(x)+1][y]!=DEADLY) && (map[x][(y)+1]!=DEADLY))


void spawn(uint8_t type)
{
    uint8_t x;
    uint8_t y;
    
    TICK_SOUND();
    while(1)
    {
        x = (uint8_t)(RAND()%(XSize-2)+1);
        y = (uint8_t)(RAND()%(YSize-2)+1);
        
        if(safe_around(x,y))
        {
            break;
        }
    }
    map[x][y]=type;
    
    _XLIB_DRAW(x,y,images[type]);
}


static uint8_t apples_on_screen_count;

static uint8_t level_walls[] = 
{
// level 0 (0)
    0,
    0,
    0,
// level 1 (3) 
    2, 
        XSize/3,       YSize/3,     XSize/3,
        XSize/3,     2*YSize/3,     XSize/3,
    2,  
        XSize/5,             1,     YSize/4,
        4*XSize/5,   3*YSize/4,     YSize/4,
    0,

// level 2 (18) 
    0, 
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,
    0,

// level 3 (27)
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
// level 5 (Z88DK partial work-around for preprocessor bug on division with integer litterals)
    4,
        ((uint8_t) (XSize/4)),                ((uint8_t) (YSize/4)),              ((uint8_t) (XSize/5))+1,
        XSize-((uint8_t) (XSize/4))-((uint8_t) (XSize/5)),        ((uint8_t) (YSize/4)),      ((uint8_t) (XSize/5))+1, // (1)
        ((uint8_t) (XSize/4)),        YSize-1-((uint8_t) (YSize/4)),              ((uint8_t) (XSize/5))+1,
        XSize-((uint8_t) (XSize/4))-((uint8_t) (XSize/5)),YSize-1-((uint8_t) (YSize/4)),      ((uint8_t) (XSize/5))+1, // (2)
    4,  
        ((uint8_t) (XSize/4)),                ((uint8_t) (YSize/4)),              ((uint8_t) (YSize/5)),
        XSize-((uint8_t) (XSize/4)),((uint8_t) (YSize/4)),                      ((uint8_t) (YSize/5)), // (1)
        ((uint8_t) (XSize/4)),        YSize-((uint8_t) (YSize/4))-((uint8_t) (YSize/5)),        ((uint8_t) (YSize/5)),
        XSize-((uint8_t) (XSize/4)),YSize-((uint8_t) (YSize/4))-((uint8_t) (YSize/5)),        ((uint8_t) (YSize/5)), // (2)
    0,
// level 6
    0,
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,
    2,
        1,1,XSize/8,YSize/8,DEADLY,
        XSize-1-XSize/8,YSize-1-YSize/8,XSize/8,YSize/8,DEADLY,
// level 7 (109)
    2,
        0, YSize/2, XSize/4,
        XSize-XSize/4, YSize/2, XSize/4,
    0,
    2,
        XSize/2-XSize/8,1,XSize/4,YSize/4,DEADLY,
        XSize/2-XSize/8,YSize-1-YSize/4,XSize/4,YSize/4,DEADLY,
// level 8
    0,
    4, 
        XSize/6, 3, YSize-1-6,
        2*XSize/6, 3, YSize-1-6,
        XSize-1-2*XSize/6, 3, YSize-1-6,
        XSize-1-XSize/6, 3, YSize-1-6,
    0,
// level 9 (143)
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
        2,2,XSize/4,YSize/4,WALL,
        2,YSize-2-YSize/4,XSize/4,YSize/4,WALL,
        XSize-2-XSize/4,YSize-2-YSize/4,XSize/4,YSize/4,WALL,
        XSize-2-XSize/4,2,XSize/4,YSize/4,WALL,
// level 12
    1,
        2,YSize/2-1,XSize-4,
    0,
    0,
// level 13 (210)
    0,
    2,
         XSize/5,                      1,   4*YSize/5,
       4*XSize/5,                      YSize-1-4*YSize/5,   4*YSize/5,
    0,
// level 14 (219)
    4,
        0,                      YSize/4,      XSize/4+1,
       3*XSize/4,               YSize/4,      XSize/4,
        0,                    3*YSize/4,      XSize/4+1,
       3*XSize/4,             3*YSize/4,      XSize/4,
    0,
    0,
// level 15 (234)
    1,
        5,YSize/2+1,XSize-10,
    0,
    1,
        XSize/2-2,YSize/2+3,4,4,DEADLY,
// level 16 (245)
    0,
    2,
        XSize/3, YSize-1-YSize/3, YSize/3,
        XSize-1-XSize/3, YSize-1-YSize/3, YSize/3,
    0,
};


static uint8_t level_walls_index[] = 
    {
        0,   // 0
        3,   // 1
        18,  // 2
        27,  // 3
        48,  // 4
        63,  // 5
        90,  // 6
        109, // 7
        128, // 8
        143, // 9
        166, // 10
        175, // 11
        204, // 12
        210, // 13
        219, // 14
        234, // 15
        245, // 16
        };




void build_box_wall(uint8_t x, uint8_t y, uint8_t x_length, uint8_t y_length, uint8_t type)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<x_length;++i)
    {
        for(j=0;j<y_length;++j)
        {
            map[x+i][y+j]=type;
            if(type)
            {
                _XLIB_DRAW(x+i,y+j,images[type]);
            }
            else
            {
                _XLIB_DELETE(x+i,y+j);
            }

        }
    }
}

#define build_horizontal_wall(x,y,length) \
    build_box_wall(x, y, length, 1, HORIZONTAL_WALL)

#define build_vertical_wall(x,y,length) \
    build_box_wall(x, y, 1, length, VERTICAL_WALL)

void DRAW_MAP_BORDERS(void)
{
    build_horizontal_wall(0,0,XSize);
    build_horizontal_wall(0,YSize-1,XSize);
    build_vertical_wall(0,0,YSize);
    build_vertical_wall(XSize-1,0,YSize);
}

#define MAX_NUMBER_OF_HORIZONTAL_MINES 4
#define MAX_NUMBER_OF_VERTICAL_MINES 2


static uint8_t horizontal_mine_x[MAX_NUMBER_OF_HORIZONTAL_MINES];
static uint8_t horizontal_mine_y[MAX_NUMBER_OF_HORIZONTAL_MINES];
static uint8_t horizontal_mine_direction[MAX_NUMBER_OF_HORIZONTAL_MINES];
static uint8_t horizontal_mines_on_current_level;
static uint8_t horizontal_mine_transition[MAX_NUMBER_OF_HORIZONTAL_MINES];

static uint8_t vertical_mine_x[MAX_NUMBER_OF_VERTICAL_MINES];
static uint8_t vertical_mine_y[MAX_NUMBER_OF_VERTICAL_MINES];
static uint8_t vertical_mine_direction[MAX_NUMBER_OF_VERTICAL_MINES];
static uint8_t vertical_mines_on_current_level;
static uint8_t vertical_mine_transition[MAX_NUMBER_OF_VERTICAL_MINES];


static uint8_t horizontal_mines_on_level[] = 
    {
        2, // 0  (0)
            YSize/5,
            4*YSize/5,
        0, // 1  (3)
        0, // 2  (4)
        2, // 3  (5)
            YSize/2 - 3,
            YSize/2 + 3,
        3, // 4  (8)
            YSize/2 - 1,
            2,
            YSize-3,
        0, // 5  (12)
        3, // 6  (13)
            2,
            YSize-3,
            YSize/2-2,
        0, // 7  (17)
        2, // 8  (18)
            2,
            YSize-3,
        1, // 9  (21)
            YSize/2-1,
        2, // 10 (23)
            YSize/3 - 2,
            2*YSize/3+2,
        1, // 11 (26)
            YSize/2 - 3,
        3, // 12 (28)
            YSize/2 - 5,
            YSize/2 + 3,
            YSize/2 + 6,
        2, // 13 (32)
            2,
            YSize-3,
        4, // 14 (35)
            YSize/2 - 2,
            YSize/2 + 2,
            YSize/2 - 4,
            YSize/2 + 4,
        3, // 15 (40)
            YSize/2 - 3,
            YSize/2 - 4,
            YSize/2 - 5,
        3, // 16 (44)
            YSize/2 - 4,
            YSize/2 - 5,
            YSize/2 - 6,
        2, // 17 (48)
            YSize/2 - 3,
            YSize/2 + 3,
        2, // 18 (51)
            3,
            YSize - 3,
        4, // 19 (54)
            YSize/2 - 3,
            YSize/2 + 3,
            YSize/2 - 2,
            YSize/2 + 2,
        4, // 20 (59)
            YSize/2 - 1,
            YSize/2 + 1,
            2,
            YSize-3,
        2, // 21 (64)
            3,
            YSize - 3,
        4, // 22 (67)
            2,
            YSize-3,
            YSize/2-3,
            YSize/2+3,
        2, // 23 (72)
            YSize/2 - 3,
            YSize/2 + 2,
        3, // 24 (75)
            2,
            YSize-3,
            YSize/2 - 3,
        2, // 25 (79)
            YSize/2+1,
            YSize/2-1,
        4, // 26 (82)
            2,
            YSize-3,
            4,
            YSize-5,
        2, // 27 (87)
            YSize/2 - 3,
            YSize/2 + 3,
        4, // 28 (90)
            3,
            YSize - 4,
            5,
            YSize - 6,
        2, // 29 (95)
            2,
            YSize-3,
        4, // 30 (98)
            YSize/2 - 2,
            YSize/2 + 2,
            YSize/2 - 4,
            YSize/2 + 4,
        4, // 31 (103)
            YSize/2 - 4,
            YSize/2 - 5,
            YSize/2 - 6,
            YSize/2 - 7,
        4, // 32 (108)
            YSize/2 - 5,
            YSize/2 - 6,
            YSize/2 - 7,
            YSize/2 - 8,
    };


static uint8_t horizontal_mines_on_level_index[] =
    {
        0,  //  0
        3,  //  1
        4,  //  2
        5,  //  3
        8,  //  4
        12, //  5
        13, //  6
        17, //  7
        18, //  8
        21, //  9
        23, // 10
        26, // 11
        28, // 12
        32, // 13
        35, // 14
        40, // 15
        44, // 16
        48, // 17
        51, // 18
        54, // 19
        59, // 20
        64, // 21
        67, // 22
        72, // 23
        75, // 24
        79, // 25
        82, // 26
        87, // 27
        90, // 28
        95, // 29
        98, // 30
       103, // 31
       108  // 32
    };


static uint8_t vertical_mines_on_level[] = 
    {
        1, // 0  (0)
            XSize/2,
        1, //  1 (2)
            XSize/2-1,
        1, //  2 (4)
            XSize/2-1,
        0, //  3 (6) 
        0, //  4 (7)
        2, //  5 (8)
            XSize/2,
            XSize/2-1,
        1, //  6 (11)
            XSize/2-1,
        2, //  7 (13)
            XSize/2+1,
            XSize/2-2,
        1, //  8 (16)
            XSize/2,
        2, //  9 (18)
            XSize/2+1,
            XSize/2-1,
        1, // 10 (21)
            XSize/2-1,
        0, // 11 (23)
        0, // 12 (24)
        1, // 13 (25)
            XSize/2+1,
        0, // 14 (27)
        1, // 15 (28)
            XSize/2-1,
        1, // 16 (30),
            XSize/2-1,
        1, // 17 (32)
            XSize/2-1,
        1, // 18 (34)
            XSize/2,
        0, // 19 (36) 
        0, // 20 (37)
        2, // 21 (38)
            XSize/2,
            XSize/2-1,
        1, // 22 (41)
            XSize/2-1,
        2, // 23 (43)
            XSize/2+1,
            XSize/2-1,
        1, // 24 (46)
            XSize/2,
        2, // 25 (48)
            XSize/2,
            XSize/2-1,
        1, // 26 (51)
            XSize/2-1,
        0, // 27 (53)
        0, // 28 (54)
        2, // 29 (55)
            XSize/2+1,
            XSize/2-1,
        1, // 30 (58)
            XSize/2,
        1, // 31 (60)
            XSize/2,
        2, // 32 (62),
            XSize/6,
            XSize-1-XSize/6
    };


static uint8_t vertical_mines_on_level_index[] =
    {
         0,
         2,
         4,
         6,
         7,
         8,
        11,
        13, // 7
        16, // 8
        18, // 9
        21, // 10
        23, // 11
        24, // 12
        25, // 13
        27, // 14
        28, // 15
        30, // 16
        32, // 17
        34, // 18
        36, // 19
        37, // 20
        38, // 21
        41, // 22
        43, // 23
        46, // 24
        48, // 25
        51, // 26
        53, // 27
        54, // 28
        55, // 29
        58, // 30
        60, // 31
        62  // 32
    };



#define MINE_RIGHT 0
#define MINE_LEFT 1
#define MINE_UP 2
#define MINE_DOWN 3


static uint8_t transparent_vertical_wall_level_flag;
static uint8_t transparent_horizontal_wall_level_flag;

#define TRANSPARENT_VERTICAL_WALL_LENGTH (YSize - 2*(YSize/4) - 4)
#define TRANSPARENT_VERTICAL_WALL_X ((XSize)/2)
#define TRANSPARENT_VERTICAL_WALL_Y (((YSize)/4)+2)

#define TRANSPARENT_HORIZONTAL_WALL_LENGTH ((XSize)/5)
#define TRANSPARENT_HORIZONTAL_WALL_X (((XSize)/2)-((TRANSPARENT_HORIZONTAL_WALL_LENGTH)/2))
#define TRANSPARENT_HORIZONTAL_WALL_Y (((YSize)/2))

static uint8_t not_many_mines;
static uint8_t total_mines_on_current_level;

void build_horizontal_mines(uint8_t level)
{
    uint8_t j;
    uint8_t index =  index = horizontal_mines_on_level_index[level];
    
    horizontal_mines_on_current_level = horizontal_mines_on_level[index];
    
    ++index;
    #if defined(DEBUG_LEVELS)
        PRINTD(4,4,1,horizontal_mines_on_current_level);
    #endif
    for(j=0;j<horizontal_mines_on_current_level;++j)
    {
        horizontal_mine_x[j] = XSize/2;
        horizontal_mine_y[j] = horizontal_mines_on_level[index+j];
        #if defined(DEBUG_LEVELS)
        _XLIB_DRAW(horizontal_mine_x[j],horizontal_mine_y[j],&MINE_IMAGE);
        #endif
        horizontal_mine_direction[j] = j&1;
        horizontal_mine_transition[j] = 0;
    }
}

void build_vertical_mines(uint8_t level)
{
    uint8_t j;
    uint8_t index = vertical_mines_on_level_index[level];
    
    vertical_mines_on_current_level = vertical_mines_on_level[index];
    
    #if defined(DEBUG_LEVELS)
        PRINTD(4,8,1,vertical_mines_on_current_level);
    #endif
    ++index;
    for(j=0;j<vertical_mines_on_current_level;++j)
    {
        vertical_mine_y[j] = YSize/2-1;
        vertical_mine_x[j] = vertical_mines_on_level[index+j];
        #if defined(DEBUG_LEVELS)
        _XLIB_DRAW(vertical_mine_x[j],vertical_mine_y[j],&MINE_IMAGE);
        #endif
        vertical_mine_direction[j] = j&1;
        vertical_mine_transition[j] = 0;
    }
}


void init_map_to_empty(void)
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


void build_level(void)
{
    register uint8_t index;
    uint8_t i;
    uint8_t number_of_elements;
    uint8_t j;
    uint8_t x;
    uint8_t y;
    uint8_t type;
    
    init_map_to_empty();
    CLEAR_SCREEN();
            
    index = level_walls_index[level-((level>16)<<4)];
    
    for(j=0;j<2;++j)
    {
        number_of_elements = level_walls[index]; // Number of horizontal walls
        for(i=1;i<3*number_of_elements;i+=3)
        {
            x=level_walls[index+i];
            y=level_walls[index+1+i];
            type=level_walls[index+2+i];
            if(j)
            {
                build_vertical_wall(x,y,type);
            }
            else
            {
                build_horizontal_wall(x,y,type);
            }
        }
        index += number_of_elements*3+1;
    }
    
    number_of_elements = level_walls[index];
    ++index;
    for(j=0;j<5*number_of_elements;j+=5,index+=5)
    {
        build_box_wall(level_walls[index],level_walls[1u+index],
                       level_walls[2u+index],level_walls[3u+index],
                       level_walls[4u+index]);
    }
    
    build_horizontal_mines(level);
    build_vertical_mines(level);
}


void handle_horizontal_mine(register uint8_t index)
{
    register uint8_t x = horizontal_mine_x[index];
    register uint8_t y = horizontal_mine_y[index];
    
    if(horizontal_mine_direction[index]==MINE_LEFT)
    {
        
        if(!horizontal_mine_transition[index]) // transition not performed, yet
        {
            if(!map[x-1][y])
            {
                // Do left transition
                _XLIB_DRAW(x-1,y,&LEFT_MINE_IMAGE);
                _XLIB_DRAW(x,y,&RIGHT_MINE_IMAGE);
                map[x-1][y]=DEADLY;
                ++horizontal_mine_transition[index];
            }
            else
            {
                horizontal_mine_direction[index]=MINE_RIGHT;
            }
        }
        else // transition already performed
        {
            horizontal_mine_transition[index]=0;
            map[x][y]=EMPTY;
            _XLIB_DELETE(x,y);
            --horizontal_mine_x[index];
            _XLIB_DRAW(horizontal_mine_x[index],y,&MINE_IMAGE);
        }
    }
    else // direction is RIGHT
    {
        if(!horizontal_mine_transition[index]) // transition not performed, yet
        {
            if(!map[x+1][y])
            {
                // Do right transition
                _XLIB_DRAW(x,y,&LEFT_MINE_IMAGE);
                _XLIB_DRAW(x+1,y,&RIGHT_MINE_IMAGE);
                map[x+1][y]=DEADLY;
                ++horizontal_mine_transition[index];
            }
            else
            {
                horizontal_mine_direction[index]=MINE_LEFT;
            }
        }
        else // transition already performed
        {
            horizontal_mine_transition[index]=0;
            map[x][y]=EMPTY;
            _XLIB_DELETE(x,y);
            ++horizontal_mine_x[index];
            _XLIB_DRAW(horizontal_mine_x[index],horizontal_mine_y[index],&MINE_IMAGE);
        }
    }
}


void handle_horizontal_mines(void)
{
    uint8_t i;
    
    for(i=0;i<horizontal_mines_on_current_level;++i)
    {
        handle_horizontal_mine(i);
    }
}



void handle_vertical_mine(register uint8_t index)
{
    register uint8_t x = vertical_mine_x[index];
    register uint8_t y = vertical_mine_y[index];
    
    if(vertical_mine_direction[index]==MINE_UP)
    {
        
        if(!vertical_mine_transition[index]) // transition not performed, yet
        {
            if(!map[x][y-1])
            {
                // Do up transition
                _XLIB_DRAW(x,y-1,&UP_MINE_IMAGE);
                _XLIB_DRAW(x,y,&DOWN_MINE_IMAGE);
                map[x][y-1]=DEADLY;
                ++vertical_mine_transition[index];
            }
            else
            {
                vertical_mine_direction[index]=MINE_DOWN;
            }
        }
        else // transition already performed
        {
            vertical_mine_transition[index]=0;
            map[x][y]=EMPTY;
            _XLIB_DELETE(x,y);
            --vertical_mine_y[index];
            _XLIB_DRAW(x,vertical_mine_y[index],&MINE_IMAGE);
        }
    }
    else // direction is DOWN
    {
        if(!vertical_mine_transition[index]) // transition not performed, yet
        {
            if(!map[x][vertical_mine_y[index]+1])
            {
                // Do right transition
                _XLIB_DRAW(x,y,&UP_MINE_IMAGE);
                _XLIB_DRAW(x,y+1,&DOWN_MINE_IMAGE);
                map[x][y+1]=DEADLY;
                ++vertical_mine_transition[index];
            }
            else
            {
                vertical_mine_direction[index]=MINE_UP;
            }
        }
        else // transition already performed
        {
            vertical_mine_transition[index]=0;
            map[x][y]=EMPTY;
            _XLIB_DELETE(x,y);
            ++vertical_mine_y[index];
            _XLIB_DRAW(x,vertical_mine_y[index],&MINE_IMAGE);
        }
    }
}


void handle_vertical_mines(void)
{
    uint8_t i;
    
    for(i=0;i<vertical_mines_on_current_level;++i)
    {
        handle_vertical_mine(i);
    }
}


uint8_t empty_vertical_wall_area(void)
{
    uint8_t i = 0;
    
    while(i<TRANSPARENT_VERTICAL_WALL_LENGTH)
    {
        if(map[TRANSPARENT_VERTICAL_WALL_X][TRANSPARENT_VERTICAL_WALL_Y+i])
        {
            return 0;
        }
        ++i;
    }
    return (TRANSPARENT_VERTICAL_WALL_X!=snake_head_x)||
           !((snake_head_y>=TRANSPARENT_VERTICAL_WALL_Y)&&(snake_head_y<TRANSPARENT_VERTICAL_WALL_Y+TRANSPARENT_VERTICAL_WALL_LENGTH-1));
}


uint8_t empty_horizontal_wall_area(void)
{
    uint8_t i = 0;
    
    while(i<TRANSPARENT_HORIZONTAL_WALL_LENGTH)
    {
        if(map[TRANSPARENT_HORIZONTAL_WALL_X+i][TRANSPARENT_HORIZONTAL_WALL_Y])
        {
            return 0;
        }
        ++i;
    }
    return (TRANSPARENT_HORIZONTAL_WALL_Y!=snake_head_y)||
           !((snake_head_x>=TRANSPARENT_HORIZONTAL_WALL_X)&&(snake_head_x<TRANSPARENT_HORIZONTAL_WALL_X+TRANSPARENT_HORIZONTAL_WALL_LENGTH-1));
}



void handle_transparent_vertical_wall(void)
{
    uint8_t type;
    
    if(!transparent_vertical_wall_triggered)
    {
        if(empty_vertical_wall_area())
        {
            transparent_vertical_wall_triggered = 1;
            TOCK_SOUND();
            type = TRANSPARENT;
        }
        else
        {
            return;
        }
    }
    else
    {   
        transparent_vertical_wall_triggered = 0;
        type = EMPTY;
    }
    build_box_wall(TRANSPARENT_VERTICAL_WALL_X,TRANSPARENT_VERTICAL_WALL_Y,1,TRANSPARENT_VERTICAL_WALL_LENGTH,type);

}

void handle_transparent_horizontal_wall(void)
{
    uint8_t type;
    
    if(!transparent_horizontal_wall_triggered)
    {
        if(empty_horizontal_wall_area())
        {
            transparent_horizontal_wall_triggered = 1;
            TOCK_SOUND();
            type = TRANSPARENT;
        }
        else
        {
            return;
        }
    }
    else
    {   
        transparent_horizontal_wall_triggered = 0;
        type = EMPTY;
    }
    build_box_wall(TRANSPARENT_HORIZONTAL_WALL_X,TRANSPARENT_HORIZONTAL_WALL_Y,TRANSPARENT_HORIZONTAL_WALL_LENGTH,1,type);

}


#define handle_transparent_walls() \
    if(transparent_vertical_wall_level_flag) \
    { \
        handle_transparent_vertical_wall(); \
    } \
    if(transparent_horizontal_wall_level_flag) \
    { \
        handle_transparent_horizontal_wall(); \
    }

#define handle_record() \
    if(points>record) \
    { \
        record = points; \
    }

#define initialize_variables() \
    extra_life_counter = 1; \
    points = 0; \
    lives = INITIAL_LIVES; \
    level = INITIAL_LEVEL; \
    remaining_apples=INITIAL_APPLE_COUNT+level*APPLE_COUNT_INCREASE; \
    secret_level_never_activated = 1;

#define DISPLAY_LEVEL_SCREEN() \
    CLEAR_SCREEN(); \
    if(!level) \
    { \
        SET_TEXT_COLOR(COLOR_YELLOW); \
        PRINT(XSize/2-4,YSize/2, _SECRET_STRING); \
    } \
    else if(!(level&3)) \
    { \
        for(i=0;i<10;++i) \
        { \
            build_box_wall(XSize/2-5,YSize/2,10,1,EXTRA); \
        } \
    } \
    SET_TEXT_COLOR(COLOR_WHITE); \
    PRINT(XSize/2-4,YSize/2+2,       _LEVEL_STRING); \
    PRINTD(XSize/2-4+6,YSize/2+2,2,level);

#define initialize_level_variables() \
    energy = MAX_ENERGY; \
    coin_count = 0; \
    speed_increase_counter = 0; \
    slow_down = SLOW_DOWN; \
    apples_on_screen_count = 1+(remaining_apples>>3); \
    spawned_apples = 0; \
    total_mines_on_current_level = vertical_mines_on_current_level+horizontal_mines_on_current_level; \
    not_many_mines = total_mines_on_current_level<=EXTRA_COIN_SPAWN_THRESHOLD; \
    transparent_vertical_wall_level_flag = transparent_vertical_wall_level(); \
    transparent_horizontal_wall_level_flag = transparent_horizontal_wall_level(); \
    secret_level_active = 0;


#define spawn_items_at_level_startup() \
    for(i=0;i<apples_on_screen_count;++i) \
    { \
        spawn(APPLE); \
    } \
    if(!not_many_mines) \
    { \
        spawn(COIN); \
    }

#define debug_transparent_walls() \
    WAIT_PRESS(); \
    if(transparent_horizontal_wall_level()) \
    { \
        build_box_wall(TRANSPARENT_HORIZONTAL_WALL_X,TRANSPARENT_HORIZONTAL_WALL_Y,TRANSPARENT_HORIZONTAL_WALL_LENGTH,1,TRANSPARENT); \
    } \
    if(transparent_vertical_wall_level()) \
    { \
        build_box_wall(TRANSPARENT_VERTICAL_WALL_X,TRANSPARENT_VERTICAL_WALL_Y,1,TRANSPARENT_VERTICAL_WALL_LENGTH,TRANSPARENT); \
    }

#define handle_extra_life() \
    if(points>extra_life_counter*EXTRA_LIFE_THRESHOLD) \
    { \
        ++extra_life_counter; \
        ++lives; \
        DISPLAY_LIVES(); \
        PING_SOUND(); \
        _XLIB_DRAW(XSize-2,0,&HORIZONTAL_HEAD_IMAGE); \
        PING_SOUND(); \
        DO_SLOW_DOWN(SLOW_DOWN*5); \
        _XLIB_DRAW(XSize-2,0,&VERTICAL_HEAD_IMAGE); \
        PING_SOUND(); \
    }

#define handle_mines() \
    if(active_mines) \
    { \
        handle_horizontal_mines(); \
        handle_vertical_mines(); \
    }


#define handle_items_to_spawn() \
    if(!(level&3)) \
    { \
        spawn(EXTRA); \
    } \
    if((!apples_on_screen_count || (RAND()&1)) && (apples_on_screen_count<remaining_apples)) \
    { \
        ++apples_on_screen_count; \
        if((spawned_apples&7)==SPAWNED_APPLE_START) \
            { \
                spawn(COIN); \
            } \
        spawn(APPLE); \
        ++spawned_apples; \
    }

#define handle_coin_effect() \
    snake_grows(); \
    points+=(COIN_POINTS<<coin_count); \
    ZAP_SOUND(); \
    if(coin_count<MAX_COIN_COUNT) \
    { \
        ++coin_count; \
        _XLIB_DRAW(XSize-3-MAX_COIN_COUNT+coin_count,YSize-1,&COIN_IMAGE); \
    } \
    if(coin_count==MAX_COIN_COUNT) \
    { \
        spawn(SUPER_COIN); \
    }

#define handle_extra_points_effect() \
    snake_grows(); \
    ZAP_SOUND(); \
    points+=EXTRA_POINTS;

#define handle_super_coin_effect() \
    ZAP_SOUND(); \
    points+=SUPER_COIN_POINTS; \
    slow_down = SLOW_DOWN + SLOW_DOWN/3; \
    energy = MAX_ENERGY; \
    DISPLAY_ENERGY(); \
    active_mines = 0; \
    if(secret_level_never_activated) \
    { \
        secret_level_active = 1; \
    }


#define handle_apple_effect() \
    --apples_on_screen_count; \
    snake_grows(); \
    --remaining_apples; \
    DISPLAY_REMAINING_APPLES_COUNT(); \
    points+=APPLE_POINTS; \
    ZAP_SOUND(); \
    IF_POSSIBLE_DECREASE_SPEED();

// TODO: All these IFs are mutually exclusive
#define handle_collisions_with_objects() \
    if(hits_coin(snake_head_x,snake_head_y)) \
    { \
        handle_coin_effect(); \
    } \
    if(hits_extra_points(snake_head_x,snake_head_y)) \
    { \
        handle_extra_points_effect(); \
    } \
    if(hits_super_coin(snake_head_x,snake_head_y)) \
    { \
        handle_super_coin_effect(); \
    } \
    if(hits_apple(snake_head_x,snake_head_y)) \
    { \
        handle_apple_effect(); \
    } \
    if(hits_deadly_item(snake_head_x,snake_head_y) || !remaining_apples) \
    { \
        break; \
    }

#define update_snake_head() \
    snake_head_x = snake_x[snake_head]; \
    snake_head_y = snake_y[snake_head];

#define handle_mine_reactivation() \
    if(slow_down<SLOW_DOWN) \
    { \
        active_mines = 1; \
    }

#define update_remaining_apples() \
    remaining_apples=INITIAL_APPLE_COUNT+level*APPLE_COUNT_INCREASE; \
    if(remaining_apples>MAX_APPLES) \
    { \
        remaining_apples = MAX_APPLES; \
    }

#define handle_no_energy() \
    if(!energy) \
    { \
        SET_TEXT_COLOR(COLOR_RED); \
        printCenteredMessageOnRow(YSize/2, __NO_ENERGY__STRING); \
        break; \
    }

#define handle_level_cleared() \
    SET_TEXT_COLOR(COLOR_RED); \
    printCenteredMessageOnRow(YSize/2, __LEVEL_CLEARED__STRING); \
    level_bonus = (uint16_t) (((uint16_t) snake_length)<<1)+(((uint16_t) energy)<<3) +(((uint16_t) coin_count)<<5) + (((uint16_t) level)<<2); \
    SET_TEXT_COLOR(COLOR_WHITE); \
    printCenteredMessageOnRow(YSize/2+2, __BONUS__STRING); \
    PRINTD(XSize/2-3,YSize/2+4,5,level_bonus);

#define handle_next_level() \
    if(level) \
    { \
        if(!secret_level_active) \
        { \
            ++level; \
        } \
        else \
        { \
            secret_level_never_activated = 0; \
            next_level = level + 1; \
            level = 0; \
        } \
    } \
    else \
    { \
        level = next_level; \
    } \
    update_remaining_apples(); \
    points+=level_bonus; \
    WAIT_PRESS();

#define handle_final_screen() \
    CLEAR_SCREEN(); \
    build_box_wall(0,1,XSize-2,YSize-2,APPLE); \
    show_intro_snake(); \
    SET_TEXT_COLOR(COLOR_WHITE); \
    printCenteredMessageOnRow(YSize/8+3, __THE_END__STRING);

#define handle_lost_life() \
    --lives; \
    EXPLOSION_SOUND(); \
    PRESS_KEY();

#if XSize<32
    #define ANIMATION_SLOW_FACTOR 4
#elif XSize<64
    #define ANIMATION_SLOW_FACTOR 8
#else
    #define ANIMATION_SLOW_FACTOR 16
#endif 

#define DISPLAY_LEVEL_ANIMATION() \
{ \
    uint8_t i; \
    \
    build_horizontal_mines(31); \
    init_map_to_empty(); \
    DRAW_MAP_BORDERS(); \
    for(i=0;i<XSize*2-5;++i) \
    { \
        handle_horizontal_mines(); \
        DO_SLOW_DOWN(SLOW_DOWN/ANIMATION_SLOW_FACTOR); \
    }; \
}

#define initialize_map() \
{ \
    DRAW_MAP_BORDERS(); \
    _XLIB_DRAW(XSize-2,0,&VERTICAL_HEAD_IMAGE); \
    _XLIB_DRAW(0,0,&SCORE_TEXT_LEFT_IMAGE); \
    _XLIB_DRAW(1,0,&SCORE_TEXT_RIGHT_IMAGE); \
    _XLIB_DRAW(XSize-10+HISCORE_OFFSET,0,&HI_TEXT_IMAGE); \
    _XLIB_DRAW(8,0,&APPLE_IMAGE); \
    _XLIB_DRAW(0,YSize-1,&LV_TEXT_IMAGE); \
    SET_TEXT_COLOR(COLOR_WHITE); \
    DISPLAY_LIVES(); \
    PRINTD(1,YSize-1,2,level); \
    DISPLAY_REMAINING_APPLES_COUNT(); \
    PRINTD(XSize-9+HISCORE_OFFSET,0,5,record); \
    DISPLAY_ENERGY(); \
    init_snake(); \
    DISPLAY_POINTS(); \
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
        handle_record();
        title();
        PRESS_KEY();
        initialize_variables();
        
        while(lives && (level<FINAL_LEVEL+1))
        {
            #if defined(DEBUG_LEVELS)
            debug_levels:
            #endif
            
            DISPLAY_LEVEL_SCREEN();
            WAIT_PRESS();
            // 
            
            active_mines = 1;
            DISPLAY_LEVEL_ANIMATION();
            
            build_level();
            initialize_level_variables();
            initialize_map();
            
            spawn_items_at_level_startup();
            WAIT_PRESS();
            
            #if defined(DEBUG_LEVELS)
                debug_transparent_walls();
                WAIT_PRESS();
                ++level;
                goto debug_levels;
            #endif
            
            while(remaining_apples)
            {
                if(not_many_mines)
                {
                    DO_SLOW_DOWN(FEW_MINES_SLOWDOWN);
                }
                handle_extra_life();
                
                if(MOVE_PLAYER())
                {
                    handle_mines();
                    DO_SLOW_DOWN(slow_down);
                    ++speed_increase_counter;
                    update_snake_head();
                    
                    if(speed_increase_counter>SPEED_INCREASE_THRESHOLD)
                    {
                        handle_transparent_walls();
                        handle_mine_reactivation();
                        speed_increase_counter = 0;
                        handle_items_to_spawn();
                        ++points;
                        IF_POSSIBLE_INCREASE_SPEED();
                    }
                    
                    // TODO: This could be optimized by performing the display only when points are updated
                    DISPLAY_POINTS();
                    
                    handle_collisions_with_objects();
                    
                    #if defined(DEBUG_SLOWDOWN)
                    PRINTD(XSize+4,YSize-2,5,slow_down);
                    #endif
                }
                handle_no_energy();
            }
            if(remaining_apples)
            {
                handle_lost_life();
            }
            else
            {
                handle_level_cleared();
                handle_next_level();
            }
        }
        if(level>FINAL_LEVEL)
        {
            handle_final_screen();
        }
        printCenteredMessageOnRow(YSize/2, __GAME_OVER__STRING);
        WAIT_PRESS();
    }
    
    return EXIT_SUCCESS;
}

