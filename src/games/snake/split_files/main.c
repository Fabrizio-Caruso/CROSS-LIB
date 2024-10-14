
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

#include "cross_lib.h"

#include "init_images.h"
#include "snake.h"
#include "move_snake.h"
#include "control_player.h"
#include "game_text.h"
#include "levels.h"
#include "settings.h"
#include "variables.h"

/*
#define EMPTY 0
#define DEADLY 1
#define SUPER_RING 2
#define RING 3
#define EXTRA 4
#define APPLE 5
#define EXTRA_LIFE 6
#define WALL  7
#define HORIZONTAL_WALL 8
#define VERTICAL_WALL 9
#define TRANSPARENT 10

*/

#if XSize<22
    #define SECRET_X 3
#else
    #define SECRET_X 4
#endif

#if !defined(NO_SECRET_ANIMATION)
    void display_secret_string(void)
    {
        _XL_PRINT(SECRET_X,YSize-1,_SECRET_STRING);
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    void set_secret(uint8_t *secret_ptr)
    {   
        if(!(*secret_ptr))
        {            
            (*secret_ptr)=1;
            
            _XL_EXPLOSION_SOUND();

            for(i=0;i<5;++i)
            {
                #if defined(_XL_NO_TEXT_COLOR)
                _XL_PRINT(SECRET_X,YSize-1,"      ");
                _XL_SLOW_DOWN(10);
                #else
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                display_secret_string();
                #endif

                _XL_SET_TEXT_COLOR(_XL_CYAN);
                display_secret_string();
            }
        }
    }
#else
    void set_secret(uint8_t *secret_ptr)
    {
        if(!(*secret_ptr))
        {
            (*secret_ptr)=1;
            _XL_SET_TEXT_COLOR(_XL_CYAN);
            _XL_PRINT(SECRET_X,YSize-1,_SECRET_STRING);
            _XL_SHOOT_SOUND();
        }
    }  
#endif



static const uint8_t images[] = {
    0, 
    MINE_TILE, 
    SUPER_RING_TILE, 
    RING_TILE, 
    EXTRA_TILE, 
    APPLE_TILE,
    VERTICAL_HEAD_TILE,
    0,
    CENTRAL_BRICK_TILE, 
    HORIZONTAL_BRICK_TILE, 
    VERTICAL_BRICK_TILE, 
    TRANSPARENT_BRICK_TILE,
    };

#if !defined(_XL_NO_COLOR)
static const uint8_t image_colors[] = {
    0, 
    _XL_CYAN, 
    _XL_CYAN, 
    _XL_WHITE, 
    _XL_CYAN, 
    _XL_RED,
    _XL_GREEN,
    0,
    _XL_YELLOW, 
    _XL_YELLOW, 
    _XL_YELLOW, 
    _XL_YELLOW,
    };

#endif

#define transparent_vertical_wall_level()   (((level&15)==3)||((level&15)==6)||((level&15)==9)||((level&15)==14))
#define transparent_horizontal_wall_level() (((level&15)==2)||((level&15)==4)||((level&15)==7)||((level&15)== 8)||((level&15)==12)||(!level))


#define hits_coin(x,y) \
    (map[x][y]==RING)

#define hits_secret(x,y) \
    (map[x][y]==SECRET)

#define hits_super_coin(x,y) \
    (map[x][y]==SUPER_RING)

#define hits_apple(x,y) \
    (map[x][y]==APPLE)
    
#define hits_extra_points(x,y) \
    (map[x][y]==EXTRA)

#define hits_extra_life(x,y) \
    (map[x][y]==EXTRA_LIFE)

#define IF_POSSIBLE_INCREASE_SPEED() \
    if(slow_down>(2*_XL_SLOW_DOWN_FACTOR/3)) \
    { \
        slow_down -= _XL_SLOW_DOWN_FACTOR/12; \
    } \
    else \
    { \
       --energy; \
       DISPLAY_ENERGY(); \
    }


#define IF_POSSIBLE_DECREASE_SPEED() \
    do \
    { \
        if(slow_down<_XL_SLOW_DOWN_FACTOR) \
        { \
            slow_down += _XL_SLOW_DOWN_FACTOR/5; \
        } \
        speed_increase_counter=0; \
    } while(0)
    

// TODO: Maybe only horizontal and vertical checks are necessary
#define safe_around(i,j) \
    (!map[i][j] && \
       (map[i][(j)-1]!=DEADLY) && (map[(i)-1][j]!=DEADLY) && (map[(i)+1][j]!=DEADLY) && (map[i][(j)+1]!=DEADLY))


void spawn(uint8_t type)
{
    _XL_PING_SOUND();
    while(1)
    {
        i = (uint8_t)(_XL_RAND()%(XSize-2)+1);
        j = (uint8_t)(_XL_RAND()%(YSize-2)+1);
        
        if(safe_around(i,j))
        {
            break;
        }
    }
    map[i][j]=type;
    
    #if !defined(_XL_NO_COLOR)
    _XL_DRAW(i,j,images[type],image_colors[type]);
    #else
    _XL_DRAW(i,j,images[type],0);
    #endif

}


void build_box_wall(uint8_t x, uint8_t y, uint8_t x_length, uint8_t y_length, uint8_t type)
{   
    for(i=0;i<x_length;++i)
    {
        for(j=0;j<y_length;++j)
        {
            map[x+i][y+j]=type;
            if(type)
            {
                #if !defined(_XL_NO_COLOR)
                _XL_DRAW(x+i,y+j,images[type],image_colors[type]);
                #else
                _XL_DRAW(x+i,y+j,images[type],0);
                #endif
            }
            else
            {
                _XL_DELETE(x+i,y+j);
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

#include "variables.h"

#define MINE_RIGHT 0
#define MINE_LEFT 1
#define MINE_UP 0
#define MINE_DOWN 1


#define TRANSPARENT_VERTICAL_WALL_LENGTH (YSize - 2*(YSize/4) - 4)
#define TRANSPARENT_VERTICAL_WALL_X ((XSize)/2)
#define TRANSPARENT_VERTICAL_WALL_Y (((YSize)/4)+2)

#if XSize<=16
    #define TRANSPARENT_HORIZONTAL_WALL_LENGTH 2
#else
    #define TRANSPARENT_HORIZONTAL_WALL_LENGTH ((XSize)/5)
#endif
#define TRANSPARENT_HORIZONTAL_WALL_X (((XSize)/2)-((TRANSPARENT_HORIZONTAL_WALL_LENGTH)/2))
#define TRANSPARENT_HORIZONTAL_WALL_Y (((YSize)/2))

void DRAW_MINE(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y,MINE_TILE, _XL_CYAN);
    map[x][y] = DEADLY;
}

void build_horizontal_mines(uint8_t level)
{
    i = horizontal_mines_on_level_index[level];
    
    horizontal_mines_on_current_level = horizontal_mines_on_level[i];
    
    ++i;
    #if defined(DEBUG_LEVELS)
        _XL_PRINTD(4,4,1,horizontal_mines_on_current_level);
    #endif
    for(j=0;j<horizontal_mines_on_current_level;++j)
    {
        horizontal_mine_x[j] = XSize/2;
        horizontal_mine_y[j] = horizontal_mines_on_level[i+j];
        DRAW_MINE(XSize/2,horizontal_mine_y[j]);
        horizontal_mine_direction[j] = j&1;
        horizontal_mine_transition[j] = 0;
    }
}

void build_vertical_mines(uint8_t level)
{
    i = vertical_mines_on_level_index[level];
    
    vertical_mines_on_current_level = vertical_mines_on_level[i];
    
    #if defined(DEBUG_LEVELS)
        _XL_PRINTD(4,8,1,vertical_mines_on_current_level);
    #endif
    ++i;
    for(j=0;j<vertical_mines_on_current_level;++j)
    {
        vertical_mine_y[j] = YSize/2-1;
        vertical_mine_x[j] = vertical_mines_on_level[i+j];
        DRAW_MINE(vertical_mine_x[j],YSize/2-1);
        vertical_mine_direction[j] = j&1;
        vertical_mine_transition[j] = 0;
    }
}


void init_map_to_empty(void)
{
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
    uint8_t length;
    uint8_t wall_index;
    uint8_t secret_wall_index;
    
    init_map_to_empty();
    _XL_CLEAR_SCREEN();
    index = map_walls_index[level-((level>16)<<4)];
    
    for(j=0;j<2;++j)
    {
        number_of_elements = map_walls[index]; // Number of horizontal walls
        
        if(number_of_elements)
        {
            secret_wall_index = (uint8_t) (_XL_RAND()%(number_of_elements));
            for(i=1, wall_index=0;i<3*number_of_elements;i+=3,++wall_index)
            {
                x=map_walls[index+i];
                y=map_walls[index+1+i];
                length=map_walls[index+2+i];
                if(j)
                {
                    build_vertical_wall(x,y,length);
                }
                else
                {
                    build_horizontal_wall(x,y,length);
                    if(secret_wall_index==wall_index)
                    {
                        #if defined(DEBUG_SECRET_HOLE)
                        {
                            
                            // uint8_t k = x+1+_XL_RAND()%(length-2);
                            // map[k][y] = SECRET;
                            // _XL_DRAW(k,y,CENTRAL_BRICK_TILE, _XL_YELLOW);
                            uint8_t k;
                            for(k=0;k<length;++k)
                            {
                                _XL_DRAW(x+1+k%(length-2),y,CENTRAL_BRICK_TILE, _XL_YELLOW);
                            }
                        }
                        #endif
                        // #else
                        map[x+1+_XL_RAND()%(length-2)][y] = SECRET;
                        // #endif
                    }
                }
            }
        }
        index += number_of_elements*3+1;
    }
    
    number_of_elements = map_walls[index];
    ++index;
    for(j=0;j<5*number_of_elements;j+=5,index+=5)
    {
        build_box_wall(map_walls[index],map_walls[1u+index],
                       map_walls[2u+index],map_walls[3u+index],
                       map_walls[4u+index]);
    }
    
    build_horizontal_mines(level);
    build_vertical_mines(level);
    
}

void display_horizontal_transition_mine(uint8_t x, uint8_t y)
{
    _XL_DRAW(x-1,y,LEFT_MINE_TILE, _XL_CYAN);
    _XL_DRAW(x,y,RIGHT_MINE_TILE, _XL_CYAN);
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
                display_horizontal_transition_mine(x,y);
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
            _XL_DELETE(x,y);
            --horizontal_mine_x[index];
            DRAW_MINE(horizontal_mine_x[index],y);
        }
    }
    else // direction is RIGHT
    {
        if(!horizontal_mine_transition[index]) // transition not performed, yet
        {
            if(!map[x+1][y])
            {
                // Do right transition
                display_horizontal_transition_mine(x+1,y);
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
            _XL_DELETE(x,y);
            ++horizontal_mine_x[index];
            DRAW_MINE(horizontal_mine_x[index],y);
        }
    }
}


void handle_horizontal_mines(void)
{
    for(i=0;i<horizontal_mines_on_current_level;++i)
    {
        handle_horizontal_mine(i);
    }
}


void display_vertical_transition_mine(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y-1,UP_MINE_TILE,_XL_CYAN);
    _XL_DRAW(x,y,DOWN_MINE_TILE,_XL_CYAN);
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
                display_vertical_transition_mine(x,y);
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
            _XL_DELETE(x,y);
            --vertical_mine_y[index];
            DRAW_MINE(x,vertical_mine_y[index]);
        }
    }
    else // direction is DOWN
    {
        if(!vertical_mine_transition[index]) // transition not performed, yet
        {
            if(!map[x][y+1])
            {
                // Do right transition
                display_vertical_transition_mine(x,y+1);
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
            _XL_DELETE(x,y);
            ++vertical_mine_y[index];
            DRAW_MINE(x,vertical_mine_y[index]);
        }
    }
}


void handle_vertical_mines(void)
{
    for(i=0;i<vertical_mines_on_current_level;++i)
    {
        handle_vertical_mine(i);
    }
}


uint8_t empty_vertical_wall_area(void)
{
    i = 0;
    
    while(i<TRANSPARENT_VERTICAL_WALL_LENGTH)
    {
        if(map[TRANSPARENT_VERTICAL_WALL_X][TRANSPARENT_VERTICAL_WALL_Y+i])
        {
            return 0;
        }
        ++i;
    }
    return (TRANSPARENT_VERTICAL_WALL_X!=snake_head_x)||
           !((snake_head_y>=TRANSPARENT_VERTICAL_WALL_Y)&&(snake_head_y<=TRANSPARENT_VERTICAL_WALL_Y+TRANSPARENT_VERTICAL_WALL_LENGTH-1));
}


uint8_t empty_horizontal_wall_area(void)
{
    i = 0;
    
    while(i<TRANSPARENT_HORIZONTAL_WALL_LENGTH)
    {
        if(map[TRANSPARENT_HORIZONTAL_WALL_X+i][TRANSPARENT_HORIZONTAL_WALL_Y])
        {
            return 0;
        }
        ++i;
    }
    return (TRANSPARENT_HORIZONTAL_WALL_Y!=snake_head_y)||
           !((snake_head_x>=TRANSPARENT_HORIZONTAL_WALL_X)&&(snake_head_x<=TRANSPARENT_HORIZONTAL_WALL_X+TRANSPARENT_HORIZONTAL_WALL_LENGTH-1));
}


void handle_transparent_vertical_wall(void)
{   
    if(!transparent_vertical_wall_triggered)
    {
        if(empty_vertical_wall_area())
        {
            _XL_TOCK_SOUND();
            transparent_vertical_wall_triggered = TRANSPARENT;
        }
        else
        {
            return;
        }
    }
    else
    {   
        transparent_vertical_wall_triggered = EMPTY;
    }
    build_box_wall(TRANSPARENT_VERTICAL_WALL_X,TRANSPARENT_VERTICAL_WALL_Y,1,TRANSPARENT_VERTICAL_WALL_LENGTH,transparent_vertical_wall_triggered);
}

void handle_transparent_horizontal_wall(void)
{
    if(!transparent_horizontal_wall_triggered)
    {
        if(empty_horizontal_wall_area())
        {
            _XL_TOCK_SOUND();
            transparent_horizontal_wall_triggered = TRANSPARENT;
        }
        else
        {
            return;
        }
    }
    else
    {   
        transparent_horizontal_wall_triggered = EMPTY;
    }
    build_box_wall(TRANSPARENT_HORIZONTAL_WALL_X,TRANSPARENT_HORIZONTAL_WALL_Y,TRANSPARENT_HORIZONTAL_WALL_LENGTH,1,transparent_horizontal_wall_triggered);

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

#if INITIAL_LEVEL!=1
    #if INITIAL_LEVEL<8
        #define initialize_remaining_apples() \
            remaining_apples = INITIAL_APPLE_COUNT + level * APPLE_COUNT_INCREASE;
    #else
        #define initialize_remaining_apples() \
            remaining_apples = MAX_APPLES;
    #endif
#else
    #define initialize_remaining_apples() \
        remaining_apples = INITIAL_APPLE_COUNT+1*APPLE_COUNT_INCREASE;
#endif


#define initialize_variables() \
    extra_life_counter = 1; \
    points = 0; \
    rings = 0; \
    lives = INITIAL_LIVES; \
    level = INITIAL_LEVEL; \
    secret_level_never_activated = 1; \
    for(i=0;i<9;++i) \
    { \
        extra_life_achievement[i] = 0; \
        coin_achievement[i] = 0; \
        magic_wall_achievement[i] = 0; \
    } \
    for(i=0;i<32;++i) \
    { \
        secret_passage[i] = 0; \
    } \
    third_coin_achievement = 0; \
    fourth_coin_achievement = 0; \
    initialize_remaining_apples();


#define DISPLAY_LEVEL_SCREEN() \
    _XL_CLEAR_SCREEN(); \
    DISPLAY_RINGS(); \
    if(!level) \
    { \
        _XL_SET_TEXT_COLOR(_XL_YELLOW); \
        _XL_PRINT(XSize/2-4,YSize/2, _SECRET_STRING); \
    } \
    else if(!(level&3)) \
    { \
        for(i=0;i<10;++i) \
        { \
            build_box_wall(XSize/2-5,YSize/2,10,1,EXTRA); \
        } \
    } \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINT(XSize/2-4,YSize/2+2,       _LEVEL_STRING); \
    _XL_PRINTD(XSize/2-4+6,YSize/2+2,2,level); \
    _XL_WAIT_FOR_INPUT(); \
    DELETE_RINGS(); 

#define initialize_level_variables() \
    energy = MAX_ENERGY; \
    coin_count = 0; \
    speed_increase_counter = 0; \
    slow_down = _XL_SLOW_DOWN_FACTOR; \
    apples_on_screen_count = 1+(remaining_apples>>3); \
    spawned_apples = 0; \
    transparent_vertical_wall_level_flag = transparent_vertical_wall_level(); \
    transparent_vertical_wall_triggered = EMPTY; \
    transparent_horizontal_wall_triggered = EMPTY; \
    transparent_horizontal_wall_level_flag = transparent_horizontal_wall_level(); \
    secret_level_active = 0; \
    extra_count = 0


#define spawn_items_at_level_startup() \
    for(i=0;i<apples_on_screen_count;++i) \
    { \
        spawn(APPLE); \
    } \
    if(level>=EXTRA_RING_SPAWN_THRESHOLD) \
    { \
        spawn(RING); \
    } \
    if(!level) \
    { \
        spawn(RING); \
        spawn(RING); \
    }

#define debug_transparent_walls() \
    _XL_WAIT_FOR_INPUT(); \
    if(transparent_horizontal_wall_level()) \
    { \
        build_box_wall(TRANSPARENT_HORIZONTAL_WALL_X,TRANSPARENT_HORIZONTAL_WALL_Y,TRANSPARENT_HORIZONTAL_WALL_LENGTH,1,TRANSPARENT); \
    } \
    if(transparent_vertical_wall_level()) \
    { \
        build_box_wall(TRANSPARENT_VERTICAL_WALL_X,TRANSPARENT_VERTICAL_WALL_Y,1,TRANSPARENT_VERTICAL_WALL_LENGTH,TRANSPARENT); \
    }

#if !defined(NO_EXTRA_LIFE_ANIMATION)
    void one_up(void)
    {
        uint16_t i;
        
        ++lives;
        DISPLAY_LIVES();
        
        for(i=0;i<10;++i)
        {
            _XL_TOCK_SOUND();
            _XL_SLOW_DOWN(32*i);

            _XL_DRAW(XSize-2,0,HORIZONTAL_HEAD_TILE,_XL_RED);
            _XL_TICK_SOUND();
            _XL_SLOW_DOWN(32*i);
            _XL_DRAW(XSize-2,0,VERTICAL_HEAD_TILE, _XL_YELLOW);
        }
        _XL_DRAW(XSize-2,0,VERTICAL_HEAD_TILE, _XL_GREEN);

    }
#else
    void one_up(void)
    {
        ++lives;
        DISPLAY_LIVES();
        _XL_PING_SOUND();
        _XL_DRAW(XSize-2,0,HORIZONTAL_HEAD_TILE,_XL_RED);
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR*5U);
        _XL_DRAW(XSize-2,0,VERTICAL_HEAD_TILE, _XL_GREEN);

        _XL_PING_SOUND();
    }
#endif


#define handle_extra_life() \
    if(points>extra_life_counter*EXTRA_LIFE_THRESHOLD) \
    { \
        ++extra_life_counter; \
        one_up(); \
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
    if((!apples_on_screen_count || (_XL_RAND()&1)) && (apples_on_screen_count<remaining_apples)) \
    { \
        ++apples_on_screen_count; \
        if((spawned_apples&7)==SPAWNED_APPLE_START) \
            { \
                spawn(RING); \
            } \
        spawn(APPLE); \
        ++spawned_apples; \
    }


#if !defined(NO_HOLE_ANIMATION)
    #define handle_secret_hole() \
        _XL_DRAW(snake_head_x,snake_head_y,TRANSPARENT_BRICK_TILE,_XL_YELLOW); \
        spawn_extra(SOME_EXTRA); \
        spawn(RING); \
        _XL_DRAW(snake_head_x,snake_head_y,VERTICAL_HEAD_TILE,_XL_GREEN); \
        set_secret(&secret_passage[level]);
#else
    #define handle_secret_hole() \
        spawn_extra(SOME_EXTRA); \
        spawn(RING); \
        set_secret(&secret_passage[level]); 
#endif


#if XSize<30
    #define OPEN_X SECRET_X
    #define OPEN_Y ((YSize)-1)
#else
    #define OPEN_X (((XSize)/2)-3)
    #define OPEN_Y 0
#endif 


#define handle_coin_effect() \
    do \
    { \
        snake_grows(); \
        increase_points(RING_POINTS<<coin_count); \
        _XL_ZAP_SOUND(); \
        _XL_DRAW(XSize-6+coin_count,YSize-1,RING_TILE,_XL_WHITE); \
        if(coin_count>=2) \
        { \
            set_secret(&third_coin_achievement); \
            spawn(SUPER_RING); \
        } \
        if(coin_count>=3) \
        { \
            set_secret(&fourth_coin_achievement); \
            spawn_extra(SOME_EXTRA); \
        } \
        if(coin_count>=4) \
        { \
            if(secret_level_never_activated) \
            { \
                secret_level_active = 1; \
                _XL_SET_TEXT_COLOR(_XL_RED); \
                _XL_PRINT(OPEN_X,OPEN_Y,_XL_O _XL_P _XL_E _XL_N "00"); \
            } \
        } \
        ++coin_count; \
    } while(0)



void spawn_extra(uint8_t quantity)
{
    uint8_t i;
    
    for(i=0;i<quantity;++i)
    {
        spawn(EXTRA);
    }
}

void build_magic_column(uint8_t x, uint8_t y, uint8_t length)
{
    build_box_wall(x,y,1,length,EXTRA);
}


void magic_wall(void)
{
    set_secret(&magic_wall_achievement[level>>2]);
    switch(level)
    {
        case 0:
            spawn_extra(MANY_EXTRA);
        break;
        case 4: 
        case 20:
            build_magic_column(XSize/3, 1, YSize-YSize/5);
            build_magic_column(2*XSize/3, YSize/5, YSize-YSize/5-1);
        break;
        case 8: 
        case 24:
            build_magic_column(XSize/6,3,YSize-1-6);
            build_magic_column(XSize-1-XSize/6, 3, YSize-1-6);
        break;
        case 12: 
        case 28:
            build_magic_column(XSize/5, 1, YSize-YSize/5);
            build_magic_column(4*XSize/5, YSize/5, YSize-YSize/5-1);
        break;
        case 16: 
        case 32:
            build_magic_column(XSize/3, YSize-1-YSize/3,YSize/3);
            build_magic_column(XSize-1-XSize/3, YSize-1-YSize/3, YSize/3);
        break;
    } 
}


#define handle_extra_points_effect() \
    do \
    { \
        snake_grows(); \
        _XL_TICK_SOUND(); \
        increase_points(EXTRA_POINTS); \
        speed_increase_counter = 0; \
        if(!(level&3)) \
        { \
            if(extra_count==MAGIC_WALL_THRESHOLD) \
            { \
                magic_wall(); \
            } \
            if(extra_count==RING_THRESHOLD) \
            { \
                set_secret(&(coin_achievement[level>>2])); \
                spawn(RING); \
            } \
            if(extra_count==EXTRA_1UP_THRESHOLD) \
            { \
                if(!(extra_life_achievement[level>>2])) \
                { \
                    spawn(EXTRA_LIFE); \
                } \
            } \
            ++extra_count; \
        } \
    } while(0)


#define handle_super_coin_effect() \
    _XL_ZAP_SOUND(); \
    increase_points(SUPER_RING_POINTS); \
    slow_down = _XL_SLOW_DOWN_FACTOR + _XL_SLOW_DOWN_FACTOR/5; \
    if(energy>10) \
    { \
        energy = MAX_ENERGY; \
    } \
    else \
    { \
        energy+= 10; \
    } \
    DISPLAY_ENERGY(); \
    active_mines = 0; \


#define handle_apple_effect() \
    --apples_on_screen_count; \
    snake_grows(); \
    --remaining_apples; \
    DISPLAY_REMAINING_APPLES_COUNT(); \
    increase_points(APPLE_POINTS); \
    _XL_ZAP_SOUND(); \
    IF_POSSIBLE_DECREASE_SPEED();


#define handle_extra_life_effect() \
    _XL_ZAP_SOUND(); \
    one_up(); \
    set_secret(&extra_life_achievement[level>>2]);


#define handle_collisions_with_objects() \
    if(hits_coin(snake_head_x,snake_head_y)) \
    { \
        handle_coin_effect(); \
    } \
    else if(hits_extra_points(snake_head_x,snake_head_y)) \
    { \
        handle_extra_points_effect(); \
    } \
    else if(hits_super_coin(snake_head_x,snake_head_y)) \
    { \
        handle_super_coin_effect(); \
    } \
    else if(hits_apple(snake_head_x,snake_head_y)) \
    { \
        handle_apple_effect(); \
    } \
    else if(hits_extra_life(snake_head_x,snake_head_y)) \
    { \
        handle_extra_life_effect(); \
    } \
    else if(hits_secret(snake_head_x,snake_head_y)) \
    { \
        handle_secret_hole(); \
    }

#define update_snake_head() \
    snake_head_x = snake_x[snake_head]; \
    snake_head_y = snake_y[snake_head];

#define handle_mine_reactivation() \
    if(slow_down<_XL_SLOW_DOWN_FACTOR) \
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
        _XL_SET_TEXT_COLOR(_XL_RED); \
        PRINT_CENTERED_ON_ROW(YSize/2, _NO_ENERGY_STRING); \
        break; \
    }

#define handle_level_cleared() \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    PRINT_CENTERED_ON_ROW(YSize/2, _CLEARED_STRING); \
    level_bonus = (uint16_t) (((uint16_t) snake_length)<<1)+(((uint16_t) energy)<<3) +(((uint16_t) coin_count)<<5) + (((uint16_t) level)<<2); \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    PRINT_CENTERED_ON_ROW(YSize/2+2, _BONUS_STRING); \
    _XL_PRINTD(XSize/2-3,YSize/2+4,5,level_bonus); \
    rings+=coin_count;


void increase_points(uint16_t value)
{
    points+=value;
    DISPLAY_POINTS();
}


#if !defined(NO_BONUS_ANIMATION)
    void get_level_bonus(uint16_t level_bonus) 
    {
        uint16_t i;
        
        _XL_WAIT_FOR_INPUT();
        increase_points(level_bonus%8);
        for(i=level_bonus%8;i<level_bonus;i+=8)
        {
            increase_points(8);
            _XL_TICK_SOUND();
            _XL_SLOW_DOWN(2*i);
        }
    }

#else
    #define get_level_bonus(level_bonux)  increase_points(level_bonus)
#endif

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
    get_level_bonus(level_bonus); \
    _XL_WAIT_FOR_INPUT();

#define handle_final_screen() \
    _XL_CLEAR_SCREEN(); \
    build_box_wall(0,1,XSize-2,YSize-2,APPLE); \
    show_intro_snake(); \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    PRINT_CENTERED_ON_ROW(YSize/8+3, _THE_END_STRING);

#define handle_lost_life() \
    --lives; \
    _XL_EXPLOSION_SOUND(); \
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
    active_mines = 1; \
    init_map_to_empty(); \
    DRAW_MAP_BORDERS(); \
    build_horizontal_mines(31); \
    for(i=0;i<XSize*2-5;++i) \
    { \
        handle_horizontal_mines(); \
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/ANIMATION_SLOW_FACTOR); \
    }; \
}

#if XSize<=19
    #define display_hi() 
    #define print_record() 
#else
    #define display_hi()  _XL_DRAW(XSize-10+HISCORE_OFFSET,0,HI_TEXT_TILE, _XL_GREEN);
    #define print_record()     _XL_PRINTD(XSize-9+HISCORE_OFFSET,0,5,record);
#endif

#define initialize_map() \
{ \
    DRAW_MAP_BORDERS(); \
    _XL_DRAW(XSize-2,0,VERTICAL_HEAD_TILE,_XL_GREEN); \
    _XL_DRAW(0,0,SCORE_TEXT_LEFT_TILE,_XL_GREEN); \
    _XL_DRAW(1,0,SCORE_TEXT_RIGHT_TILE,_XL_GREEN); \
    display_hi(); \
    _XL_DRAW(8,0,APPLE_TILE,_XL_RED); \
    _XL_DRAW(0,YSize-1,LV_TEXT_TILE,_XL_GREEN); \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    DISPLAY_LIVES(); \
    _XL_PRINTD(1,YSize-1,2,level); \
    DISPLAY_REMAINING_APPLES_COUNT(); \
    print_record(); \
    DISPLAY_ENERGY(); \
    init_snake(); \
    DISPLAY_POINTS(); \
}

#if !defined(NO_ACHIEVEMENTS_SCREEN)


void display_achievements(uint8_t row, uint8_t achievements, uint8_t max)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(ACHIEVEMENTS_X_OFFSET+5,row,_XL_O _XL_F);
    _XL_PRINTD(ACHIEVEMENTS_X_OFFSET+8,row,2,max);
    for(i=0;i<=achievements;++i)
    {
        _XL_PRINTD(ACHIEVEMENTS_X_OFFSET+2,row,2,i);
        _XL_SHOOT_SOUND();
        _XL_SLOW_DOWN((_XL_SLOW_DOWN_FACTOR/2));
    }
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR*4U);
}



#define handle_record() \
    if(points>record) \
    { \
        record = points; \
        _XL_SET_TEXT_COLOR(_XL_RED); \
        _XL_PRINT(ACHIEVEMENTS_X_OFFSET+9,ACHIEVEMENTS_Y_OFFSET, _XL_R _XL_E _XL_C _XL_O _XL_R _XL_D); \
    }

    
void display_stats(void)
{    
    _XL_CLEAR_SCREEN();
    
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    PRINT_CENTERED_ON_ROW(ACHIEVEMENTS_Y_OFFSET-2, _XL_A _XL_C _XL_H _XL_I _XL_E _XL_V _XL_E _XL_M _XL_E _XL_N _XL_T _XL_S);
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(ACHIEVEMENTS_X_OFFSET+3,ACHIEVEMENTS_Y_OFFSET,5,points);
    _XL_DRAW(ACHIEVEMENTS_X_OFFSET,ACHIEVEMENTS_Y_OFFSET,SCORE_TEXT_LEFT_TILE,_XL_GREEN);
    _XL_DRAW(ACHIEVEMENTS_X_OFFSET+1,ACHIEVEMENTS_Y_OFFSET,SCORE_TEXT_RIGHT_TILE, _XL_GREEN);

    handle_record();
    
    DISPLAY_RINGS();
    
    lives = 0; // re-used variable
    
    for(i=0;i<9;++i)
    {
        #if defined(DEBUG_ACHIEVEMENTS)
        _XL_PRINTD(2,1+i,3,extra_life_achievement[i]); _XL_PRINTD(6,1+i,3, coin_achievement[i]); _XL_PRINTD(10,1+i,3,magic_wall_achievement[i]);
        _XL_WAIT_FOR_INPUT();
        #endif
        lives+=extra_life_achievement[i]+coin_achievement[i]+magic_wall_achievement[i];
    }
    
    for(i=0;i<32;++i)
    {
        lives+=secret_passage[i];
        #if defined(DEBUG_ACHIEVEMENTS)
        _XL_PRINTD(2+i,10,2,secret_passage[i]);
        #endif
    }
    


    #if defined(DEBUG_ACHIEVEMENTS)
        _XL_PRINTD(2,12,3,!secret_level_never_activated); _XL_PRINTD(6,1+i,3, third_coin_achievement); _XL_PRINTD(10,1+i,3,fourth_coin_achievement);
    #endif

    lives+=(!secret_level_never_activated)+third_coin_achievement+fourth_coin_achievement;
    
    
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(ACHIEVEMENTS_X_OFFSET+2,ACHIEVEMENTS_Y_OFFSET+STAT_OFFSET+3,_SECRET_STRING _XL_S);

    display_achievements(ACHIEVEMENTS_Y_OFFSET+STAT_OFFSET+5,lives, 50);
    
    if(!level)
    {
        level = next_level;
    }
    --level;

    _XL_DRAW(ACHIEVEMENTS_X_OFFSET, ACHIEVEMENTS_Y_OFFSET+STAT_OFFSET+LEVEL_OFFSET+7,LV_TEXT_TILE,_XL_GREEN);    
    display_achievements(ACHIEVEMENTS_Y_OFFSET+STAT_OFFSET+LEVEL_OFFSET+7,level,32);
    
    
    if(!secret_level_never_activated)
    {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(ACHIEVEMENTS_X_OFFSET-1,ACHIEVEMENTS_Y_OFFSET+STAT_OFFSET+LEVEL_OFFSET+8,_SECRET_STRING _XL_SPACE _LEVEL_STRING);
    }
}
#endif

#define INITIALIZE() \
    uint8_t i; \
    _XL_INIT_GRAPHICS(); \
    _XL_INIT_INPUT(); \
    _XL_INIT_SOUND(); \
    record = 0;


int main(void)
{
    INITIALIZE();
    
    while(1)
    {
        title();
        initialize_variables();
        
        while(lives && (level<FINAL_LEVEL+1))
        {
            #if defined(DEBUG_LEVELS)
            debug_levels:
            #endif
            
            DISPLAY_LEVEL_SCREEN();
            
            #if !defined(NO_DISPLAY_LEVEL_ANIMATION)
                DISPLAY_LEVEL_ANIMATION();
            #else
                active_mines = 1;
            #endif
            build_level();
            initialize_level_variables();
            initialize_map();
            spawn_items_at_level_startup();
            
            _XL_WAIT_FOR_INPUT();
            #if defined(DEBUG_FREEZE)
                spawn(SUPER_RING);
            #endif
            
            #if defined(DEBUG_LEVELS)
                debug_transparent_walls();
                _XL_WAIT_FOR_INPUT();
                ++level;
                goto debug_levels;
            #endif
            
            while(remaining_apples)
            {
                handle_extra_life();
                
                _XL_SLOW_DOWN(slow_down);
                if(MOVE_PLAYER())
                {
                    ++speed_increase_counter;
                    update_snake_head();
                    
                    if(speed_increase_counter>SPEED_INCREASE_THRESHOLD)
                    {
                        handle_transparent_walls();
                        handle_mine_reactivation();
                        speed_increase_counter = 0;
                        handle_items_to_spawn();
                        increase_points(1);
                        IF_POSSIBLE_INCREASE_SPEED();
                    }
                    
                    
                    handle_collisions_with_objects();
                    
                    #if defined(DEBUG_SLOWDOWN)
                    _XL_PRINTD(XSize-1-5,YSize-1,5,slow_down);
                    _XL_PRINTD(XSize-1-5-6,YSize-1,5,speed_increase_counter);
                    #endif
                }
                handle_mines();

                if(hits_deadly_item(snake_head_x,snake_head_y) || !remaining_apples)
                {
                    break;
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
        PRINT_CENTERED_ON_ROW(YSize/2, _GAME_OVER_STRING);
        _XL_WAIT_FOR_INPUT();
        #if !defined(NO_ACHIEVEMENTS_SCREEN)
        display_stats();
        #endif
        _XL_WAIT_FOR_INPUT();
    }
    
    return EXIT_SUCCESS;
}

