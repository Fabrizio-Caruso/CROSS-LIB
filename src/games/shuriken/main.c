/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHURIKEN by Fabrizio Caruso
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

#include "screen_types.h"
#include "levels.h"

#define INITIAL_LEVEL 0
#define FINAL_LEVEL 11

#define INITIAL_LIVES 5

// DEBUG
// #define SHOW_LEVELS
// #define INVINCIBLE

// TILES

#if XSize>=20
   #define MAX_NUMBER_OF_SHURIKENS 12
#else
   #define MAX_NUMBER_OF_SHURIKENS 10
#endif


#define MAX_TIME 5

// Left low player in the 2x2 multi-tile
#define LEFT_LOW_TILE0      _TILE_2
#define LEFT_LOW_TILE1      _TILE_6
#define LEFT_LOW_TILE2      _TILE_10
#define LEFT_LOW_TILE3      _TILE_14

#define RIGHT_LOW_TILE0     _TILE_3
#define RIGHT_LOW_TILE1     _TILE_7
#define RIGHT_LOW_TILE2     _TILE_11
#define RIGHT_LOW_TILE3     _TILE_15

#define LEFT_UP_TILE0       _TILE_0
#define LEFT_UP_TILE1       _TILE_4
#define LEFT_UP_TILE2       _TILE_8
#define LEFT_UP_TILE3       _TILE_12

#define RIGHT_UP_TILE0      _TILE_1
#define RIGHT_UP_TILE1      _TILE_5
#define RIGHT_UP_TILE2      _TILE_9
#define RIGHT_UP_TILE3      _TILE_13

#define SHURIKEN_TILE       _TILE_16
#define SHURIKEN_TILE_DOWN  _TILE_17
#define SHURIKEN_TILE_UP    _TILE_18
#define SHURIKEN_TILE_LEFT  _TILE_19
#define SHURIKEN_TILE_RIGHT _TILE_20

#define WALL_TILE           _TILE_21

#define BLOCK_TILE          _TILE_22

#define FREEZE_TILE         _TILE_23

#define MINI_SHURIKEN_TILE  _TILE_24

#define RING_TILE           _TILE_25

#define DIAMOND_TILE        _TILE_26

#define RINGS_X XSize/2-3

#define SHURIKEN_LEFT 0
#define SHURIKEN_RIGHT 1
#define SHURIKEN_UP 0
#define SHURIKEN_DOWN 1

#define PLAYER_LEFT 0
#define PLAYER_RIGHT 1
#define PLAYER_UP 2
#define PLAYER_DOWN 3

#define MIN_PLAYER_Y 3
#define MAX_PLAYER_Y (2*YSize-6)
#define MIN_PLAYER_X 2
#define MAX_PLAYER_X (2*XSize-5)

#define MOVE_FORCE 3U
#define DESTROY_FORCE 14U

#if XSize>=20
#define MAX_NUMBER_OF_MINI_SHURIKENS 6
#else
#define MAX_NUMBER_OF_MINI_SHURIKENS 4
#endif

#define MAX_NUMBER_OF_BARRIERS 4

#define DIAMOND_POINTS 10U
#define FREEZE_POINTS 30U
#define RING_POINTS 50U
#define SHURIKEN_POINTS 100U

#define ITEM_BONUS 50U

#if XSize<32
    #define BASE_RING_EFFECT 40U
#else
    #define BASE_RING_EFFECT 30U
#endif

#define START_RING_EFFECT 35U

#define EXTRA_LIFE_THRESHOLD 5000U

#define BARRIER_THRESHOLD 25

uint8_t player_x;
uint8_t player_y;

uint8_t screen_x;
uint8_t screen_y;

uint8_t alive;
uint8_t lives;
uint8_t level;
uint8_t remaining_diamonds;

uint8_t shuriken_x[MAX_NUMBER_OF_SHURIKENS];
uint8_t shuriken_y[MAX_NUMBER_OF_SHURIKENS];
uint8_t shuriken_direction[MAX_NUMBER_OF_SHURIKENS];
uint8_t shuriken_transition[MAX_NUMBER_OF_SHURIKENS];
#define shuriken_status shuriken_x

uint8_t shuriken_axis[MAX_NUMBER_OF_SHURIKENS];

#define SHURIKEN_HORIZONTAL 0
#define SHURIKEN_VERTICAL   1

uint8_t mini_shuriken_x[MAX_NUMBER_OF_MINI_SHURIKENS];
uint8_t mini_shuriken_y[MAX_NUMBER_OF_MINI_SHURIKENS];

uint8_t map[XSize][YSize];

uint8_t force;

uint8_t player_cell[4];

uint16_t score;
uint16_t hiscore;

#if !defined(_XL_NO_COLOR)
uint8_t player_color;   
#else
#define player_color 0
#endif

uint8_t level_shurikens;
uint8_t level_mini_shurikens;

uint8_t barrier_x[MAX_NUMBER_OF_BARRIERS];
uint8_t barrier_y[MAX_NUMBER_OF_BARRIERS];
uint8_t barrier_width[MAX_NUMBER_OF_BARRIERS];
uint8_t barrier_height[MAX_NUMBER_OF_BARRIERS];
uint8_t barrier_counter;
uint8_t barrier_triggered[MAX_NUMBER_OF_BARRIERS];

uint8_t number_of_barriers;

uint8_t freeze_active;
uint8_t freeze_counter;

uint8_t ring_active;
uint8_t ring_counter;

uint8_t counter;

uint8_t shuriken_counter;

uint8_t new_level;

uint8_t tile_group;

uint8_t time_counter;

uint8_t player_direction;

uint8_t extra_life_counter;

uint8_t shuriken_challenge;

uint8_t barrier_type;

uint8_t challenge_level;

uint8_t remaining_shurikens;

uint8_t barrier_threshold;

uint8_t input;
    

static const uint8_t screen_tile[7+1] =
{
    0, // unused
    RING_TILE,
    FREEZE_TILE,
    DIAMOND_TILE,
    BLOCK_TILE,
    WALL_TILE,
    SHURIKEN_TILE,
    MINI_SHURIKEN_TILE,
};  


#if !defined(_XL_NO_COLOR)
static uint8_t screen_color[7+1] =
{
    0, // unused
    _XL_WHITE,
    _XL_CYAN,
    _XL_GREEN,
    _XL_GREEN,
    _XL_RED,
    _XL_CYAN,
    _XL_YELLOW,
};  


static const uint8_t border_colors[] = {   _XL_RED,   _XL_CYAN, _XL_YELLOW, _XL_CYAN};
    
static const uint8_t wall_colors[] =   {_XL_YELLOW, _XL_YELLOW,   _XL_CYAN, _XL_RED};
#endif


#define SHURIKEN_COLOR _XL_CYAN

static const uint8_t player_tile[4][4] =
{
    { // left lower 12x12 multi-tile inside a 16x16 quad tile
        LEFT_UP_TILE0,
        RIGHT_UP_TILE0,
        LEFT_LOW_TILE0,
        RIGHT_LOW_TILE0,
    },
    { // right lower 12x12 multi-tile inside a 16x16 quad tile
        LEFT_UP_TILE1,
        RIGHT_UP_TILE1,
        LEFT_LOW_TILE1,
        RIGHT_LOW_TILE1,
    },
    { // left upper 12x12 multi-tile inside a 16x16 quad tile
        LEFT_UP_TILE2,
        RIGHT_UP_TILE2,
        LEFT_LOW_TILE2,
        RIGHT_LOW_TILE2,
    },
    { // right upper 12x12 multi-tile inside a 16x16 quad tile 
        LEFT_UP_TILE3,
        RIGHT_UP_TILE3,
        LEFT_LOW_TILE3,
        RIGHT_LOW_TILE3,
    },
};


void short_pause(void)
{
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
}


void one_second_pause(void)
{
    _XL_SLEEP(1);
}



void build_element(uint8_t type, uint8_t x, uint8_t y)
{
    map[x][y] = type;
    #if !defined(_XL_NO_COLOR)
    _XL_DRAW(x,y,screen_tile[type], screen_color[type]);
    #else
    _XL_DRAW(x,y,screen_tile[type], 0);
    #endif
}


void delete_element(uint8_t x, uint8_t y)
{
    map[x][y] = EMPTY;
    _XL_DELETE(x,y);
}


#define update_screen_xy() \
do { \
    screen_x = player_x>>1; \
    screen_y = (player_y+1)>>1; \
} while(0)
    

#define update_score_display() \
do \
{ \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(0,0,5,score); \
} while(0)


#if XSize>=20
    #define update_remaining_display() \
    do \
    { \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        _XL_PRINTD(7,0,2,remaining_diamonds); \
    } while(0)
#else
    #define update_remaining_display() 
#endif


void delete_player_down(void)
{
    _XL_DELETE(screen_x,screen_y+1);
    _XL_DELETE(screen_x+1,screen_y+1);      
}


void delete_player_up(void)
{
    _XL_DELETE(screen_x,screen_y);
    _XL_DELETE(screen_x+1,screen_y);   
}


#define delete_player_left() \
do \
{ \
    _XL_DELETE(screen_x,screen_y); \
    _XL_DELETE(screen_x,screen_y+1); \
} while(0)


#define delete_player_right() \
do \
{ \
    _XL_DELETE(screen_x+1,screen_y); \
    _XL_DELETE(screen_x+1,screen_y+1); \
} while(0)


void delete_player(void)
{
    delete_player_down();
    delete_player_up();
}


void display_player(void)
{
    // #if !defined(_XL_NO_COLOR)
    _XL_DRAW(screen_x,screen_y,player_tile[tile_group][2],player_color);
    _XL_DRAW(screen_x+1,screen_y,player_tile[tile_group][3],player_color);  
    _XL_DRAW(screen_x,screen_y+1,player_tile[tile_group][0],player_color);
    _XL_DRAW(screen_x+1,screen_y+1,player_tile[tile_group][1],player_color);  
    // #else
    // if(!ring_active || (counter&1))
    // {
        // _XL_DRAW(screen_x,screen_y,player_tile[tile_group][2],player_color);
        // _XL_DRAW(screen_x+1,screen_y,player_tile[tile_group][3],player_color);  
        // _XL_DRAW(screen_x,screen_y+1,player_tile[tile_group][0],player_color);
        // _XL_DRAW(screen_x+1,screen_y+1,player_tile[tile_group][1],player_color);     
    // }
    // else
    // {
        // delete_player();
    // }
    
    // #endif
}


#define update_freeze_display() \
    _XL_PRINTD(4,YSize-1,1,freeze_counter)


#define update_ring_display() \
do \
{ \
    _XL_SET_TEXT_COLOR(_XL_GREEN); \
    _XL_PRINTD(1,YSize-1,1,ring_counter); \
} while(0)


#define update_shuriken_display() \
    _XL_PRINTD(7,YSize-1,2,shuriken_counter)


void update_time_counter_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(11,YSize-1,1,time_counter);    
}


void update_item_display(void)
{
    update_score_display();
    
    update_remaining_display();
    
    update_time_counter_display();
    
    update_freeze_display();
    update_shuriken_display();
    
    update_ring_display();

}


void increase_time_counter_if_not_max(void)
{
    if(time_counter<MAX_TIME)
    {
        ++time_counter;
    }
    else
    {
        counter=0;
    }
    update_item_display();
}


void delete_player_cells(void)
{
    map[screen_x][screen_y]=EMPTY;
    map[screen_x+1][screen_y]=EMPTY;
    map[screen_x][screen_y+1]=EMPTY;
    map[screen_x+1][screen_y+1]=EMPTY;
}


void handle_collisions(void)
{
    uint8_t i;
    uint8_t cell_value;
    
    player_cell[0] = map[screen_x][screen_y];
    player_cell[1] = map[screen_x+1][screen_y];
    player_cell[2] = map[screen_x][screen_y+1];
    player_cell[3] = map[screen_x+1][screen_y+1];
    
    for(i=0;i<4;++i)
    {
        cell_value = player_cell[i];

        if(cell_value)
        {
            if(cell_value==DIAMOND)
            {
                _XL_PING_SOUND();
                score+=DIAMOND_POINTS;
                --remaining_diamonds;
                update_item_display();
            }
            else if(cell_value==FREEZE)
            {
                _XL_ZAP_SOUND();
                score+=FREEZE_POINTS;
                ++freeze_counter;
                freeze_active=freeze_counter<<4;
                increase_time_counter_if_not_max();
            }
            else if(cell_value==RING)
            {
                _XL_ZAP_SOUND();
                score+=RING_POINTS;
                ++ring_counter;
                #if !defined(_XL_NO_COLOR)
                player_color = _XL_CYAN;
                #endif
                ring_active=BASE_RING_EFFECT+(ring_counter<<4);
                increase_time_counter_if_not_max();
            }
            else if(cell_value>=DEADLY)
            {
                if(ring_active)
                {
                    display_player();
                }
                else
                {
                    alive=0;
                }
            }
            else if(cell_value==BLOCK) // Useful in challenge levels to avoid visually deleting blocks 
            {
                delete_player_cells();
            }
        }
    } 
}


void update_player(void)
{
    tile_group = (player_x&1)+2*(player_y&1);
    
    update_screen_xy();
    
    display_player();
    
    handle_collisions();
    
    delete_player_cells();
}


uint8_t allowed(uint8_t cell1, uint8_t cell2, uint8_t beyond_cell1, uint8_t beyond_cell2)
{
    if((cell1==BLOCK)||(cell2==BLOCK))
    {     
        ++force;
    }

    if((cell1==WALL)||(cell2==WALL))
    {
        return 0;
    }

    // From here on, the player is not against any wall
    if(force>=DESTROY_FORCE)
    {
        force=0;
        _XL_EXPLOSION_SOUND();
        return 1;
    }

    // If no wall and no block, you can always move
    if((cell1!=BLOCK) && (cell2!=BLOCK))
    {
        return 1;
    }

    // From here on, the player is against at least one block and no wall and force<DESTROY_FORCE
    if(force<MOVE_FORCE)
    {
        // If there is a block, you can't move
        return 0;
    }
    else 
    {   // You can push blocks but you cannot destroy them
        return ((!cell1)||(!beyond_cell1)) && ((!cell2)||(!beyond_cell2));
    }
}


uint8_t allowed_down(void)
{
    return allowed(map[screen_x][screen_y+2],map[screen_x+1][screen_y+2],map[screen_x][screen_y+3],map[screen_x+1][screen_y+3]);

}


uint8_t allowed_up(void)
{
    return allowed(map[screen_x][screen_y-1],map[screen_x+1][screen_y-1],map[screen_x][screen_y-2],map[screen_x+1][screen_y-2]);
}


uint8_t allowed_left(void)
{
    return allowed(map[screen_x-1][screen_y],map[screen_x-1][screen_y+1],map[screen_x-2][screen_y],map[screen_x-2][screen_y+1]);
}


uint8_t allowed_right(void)
{
    return allowed(map[screen_x+2][screen_y],map[screen_x+2][screen_y+1],map[screen_x+3][screen_y],map[screen_x+3][screen_y+1]);
}


void _if_block_push_down(uint8_t x)
{
    if((map[x][screen_y+2]==BLOCK)&&!map[x][screen_y+3])
    {
        build_element(BLOCK, x,screen_y+3);
        force=1;    
    }
}


#define if_block_push_down() \
do \
{ \
    _if_block_push_down(screen_x); \
    _if_block_push_down(screen_x+1); \
} while(0)


void _if_block_push_up(uint8_t x)
{
    if((map[x][screen_y-1]==BLOCK)&&!map[x][screen_y-2])
    {
        build_element(BLOCK, x,screen_y-2);
        force=1;
    }
}


#define if_block_push_up() \
do \
{ \
    _if_block_push_up(screen_x); \
    _if_block_push_up(screen_x+1); \
} while(0)


void _if_block_push_left(uint8_t y)
{
    if((map[screen_x-1][y]==BLOCK)&&!map[screen_x-2][y])
    {
        build_element(BLOCK,screen_x-2,y);
        force=1;
    }
}


#define if_block_push_left() \
do \
{ \
    _if_block_push_left(screen_y); \
    _if_block_push_left(screen_y+1); \
} while(0)


void _if_block_push_right(uint8_t y)
{
    if((map[screen_x+2][y]==BLOCK)&&!map[screen_x+3][y])
    {
        build_element(BLOCK,screen_x+3,y);
        force=1;
    }
}


#define if_block_push_right() \
do \
{ \
    _if_block_push_right(screen_y); \
    _if_block_push_right(screen_y+1); \
} while(0)


void init_map(void)
{
    uint8_t i;
    uint8_t j;
    
    #if !defined(_XL_NO_COLOR)
    screen_color[WALL]=border_colors[(level)&3];
    #endif
    
    _XL_CLEAR_SCREEN();
    for(i=0;i<XSize-1;++i)
    {
        for(j=0;j<YSize-1;++j)
        {
            map[i][j]=EMPTY;
        }
    }
    
    for(i=0;i<XSize;++i)
    {
        build_element(WALL,i,1);
        build_element(WALL,i,YSize-1);
    }
    for(i=1;i<YSize-1;++i)
    {
        build_element(WALL,0,i);
        build_element(WALL,XSize-1,i);
    }
}


void update_lives_display(void)
{     
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    
    _XL_PRINTD(XSize-9,0,1,lives);
}


#if XSize>=20
    #define _display_diamond_icon_if_possible() \
            build_element(DIAMOND,6,0); \

#else
    #define _display_diamond_icon_if_possible()
#endif

#define initialize_level_display() \
do \
{ \
    _XL_DRAW(XSize-10,0,SHURIKEN_TILE,_XL_WHITE); \
    \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    _XL_PRINT(XSize-7,0,"HI"); \
    \
    _display_diamond_icon_if_possible(); \
    \
    update_lives_display(); \
    \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(XSize-5,0,5,hiscore); \
    _XL_PRINTD(XSize-2,YSize-1,2,level+1); \
    \
    _XL_DRAW(0,YSize-1,RING_TILE,_XL_WHITE); \
    _XL_DRAW(3,YSize-1,FREEZE_TILE,_XL_CYAN); \
    _XL_DRAW(6,YSize-1,SHURIKEN_TILE,_XL_CYAN); \
    \
    _XL_SET_TEXT_COLOR(_XL_GREEN); \
    _XL_CHAR(10,YSize-1,'T'); \
    \
    update_item_display(); \
} while(0)



// too many parameters
void build_rectangle(uint8_t type, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    uint8_t i;
    uint8_t j;
    
    for(i=x;i<x+width;++i)
    {
        for(j=y;j<y+height;++j)
        {
            
            if(type)
            {
                build_element(type,i,j);
            }
            else // REMARK: Necessary to switch walls
            {
                delete_element(i,j);
            }
        }
    }
}


void build_objects(uint8_t level)
{
    uint16_t index = objects_index[level];
    uint8_t no_of_objects = objects_map[index];
    uint8_t i;
    uint8_t x;
    uint8_t y;
    uint8_t x_size;
    uint8_t y_size;
    uint8_t type;



    #if !defined(_XL_NO_COLOR)
    screen_color[WALL]=wall_colors[(level)&3];
    #endif
    
    for(i=0;i<no_of_objects;++i)
    {        
        x = objects_map[++index];
        y = objects_map[++index];
        x_size = objects_map[++index];
        y_size = objects_map[++index];
        type = objects_map[++index];

        if(type==DIAMOND)
        {
            remaining_diamonds +=x_size*y_size;;
        }

        build_rectangle(type,x,y,x_size,y_size);
    }    
}


#define build_barriers() \
do \
{ \
    uint8_t index = walls_index[level]; \
    uint8_t i; \
    \
    number_of_barriers = walls_map[index];  \
    \
    for(i=0;i<number_of_barriers;++i) \
    { \
        ++index; \
        barrier_x[i] = walls_map[index]; \
        ++index; \
        barrier_y[i] = walls_map[index]; \
        ++index; \
        barrier_width[i] = walls_map[index]; \
        ++index; \
        barrier_height[i] = walls_map[index]; \
        barrier_triggered[i] = 0; \
    } \
    if(challenge_level) \
    { \
        barrier_counter = BARRIER_THRESHOLD*4; \
    } \
    else \
    { \
        barrier_counter = 0; \
    } \
} while(0)


void build_shurikens(void)
{
    uint8_t index = shurikens_index[level];
    uint8_t i;

    uint8_t level_horizontal_shurikens;
    
    
    // _XL_PRINTD(0,0,5,index);
    
    level_horizontal_shurikens = shurikens_map[index];
    
    // _XL_PRINTD(0,1,5,level_horizontal_shurikens);
    
    ++index;
    level_shurikens = level_horizontal_shurikens + shurikens_map[index];;

    // _XL_PRINTD(0,2,5,level_shurikens);

    ++index;
    level_mini_shurikens = shurikens_map[index];

    // _XL_PRINTD(0,3,5,level_mini_shurikens);

    // _XL_WAIT_FOR_INPUT();
    
    for(i=0;i<level_shurikens;++i)
    {
        if(shuriken_status[i])
        {
            ++index;
            shuriken_x[i]=shurikens_map[index];
            ++index;
            shuriken_y[i]=shurikens_map[index];
            shuriken_direction[i]=0;
            shuriken_transition[i]=0;
            build_element(SHURIKEN,shuriken_x[i],shuriken_y[i]);
            if(i<level_horizontal_shurikens)
            {
                shuriken_axis[i]=SHURIKEN_HORIZONTAL;
            }
            else
            {
                shuriken_axis[i]=SHURIKEN_VERTICAL;
            }
        }
        else
        {
            index+=2;
        }
    }
    for(i=level_shurikens;i<MAX_NUMBER_OF_SHURIKENS;++i)
    {
        shuriken_status[i]=0;
    }
    
    for(i=0;i<level_mini_shurikens;++i)
    {
        ++index;
        mini_shuriken_x[i]=shurikens_map[index];
        build_element(WALL,mini_shuriken_x[i],1);
        mini_shuriken_y[i]=2;
        build_element(MINI_SHURIKEN,mini_shuriken_x[i],mini_shuriken_y[i]);
    }
}


uint8_t safe_area(uint8_t x, uint8_t y, uint8_t x_size, uint8_t y_size)
{
    uint8_t j;
    uint8_t k;
    
    for(j=x;j<x+x_size;++j)
    {
        for(k=y;k<y+y_size;++k)
        {
            if(map[j][k])
            {
                return 0;
            }
        }
    }
    return 1;
}


void handle_barriers(void)
{
    uint8_t i;
    uint8_t barrier;

    if(barrier_counter<barrier_threshold)
    {
        ++barrier_counter;
    }
    else
    {
        barrier_counter=0;
        for(i=0;i<number_of_barriers;++i)
        {
            if(!barrier_triggered[i])
            {
                if(safe_area(barrier_x[i],barrier_y[i],barrier_width[i], barrier_height[i]))
                {
                    _XL_TOCK_SOUND();
                    barrier = barrier_type;
                    ++barrier_triggered[i];
                }
                else
                {
                    continue;
                }
            }
            else if(!challenge_level)
            {
                barrier = EMPTY;
                barrier_triggered[i] = 0;
            }
            else
            {
                barrier = barrier_type;
            }
            
            build_rectangle(barrier,barrier_x[i],barrier_y[i],barrier_width[i], barrier_height[i]);
        }
    }
}


void activate_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<MAX_NUMBER_OF_SHURIKENS;++i)
    {
        shuriken_status[i]=1;
    }
}


void initialize_level_parameters(void)
{
    counter = 0;
    
    if(!((level+1)&3)) // challenge level
    {
        challenge_level = 1;
        barrier_type = BLOCK;
        #if !defined(_XL_NO_COLOR)
        screen_color[DIAMOND]=_XL_YELLOW;
        screen_color[SHURIKEN]=_XL_YELLOW;
        screen_color[MINI_SHURIKEN]=_XL_RED;
        #endif
        barrier_threshold=BARRIER_THRESHOLD*5U;
    }
    else
    {
        challenge_level = 0;
        barrier_type = SHURIKEN;
        #if !defined(_XL_NO_COLOR)
        screen_color[DIAMOND]=_XL_GREEN;
        screen_color[SHURIKEN]=_XL_CYAN;
        screen_color[MINI_SHURIKEN]=_XL_YELLOW;
        #endif
        barrier_threshold=BARRIER_THRESHOLD;
    }
}


#if YSize>=20
    #define SHURIKEN_Y 5
    #define AUTHOR_Y 8
    #define COLLECT_Y YSize-8
    #define USE_AGAINST_Y YSize-5
#else
    #define SHURIKEN_Y 2
    #define AUTHOR_Y 5
    #define COLLECT_Y YSize-5
    #define USE_AGAINST_Y YSize-2
#endif


void print_use_block_against_shurikens(uint8_t y)
{
    _XL_PRINT(XSize/2-7,y, "USE   AGAINST");

    _XL_DRAW(XSize/2-6+3,y,BLOCK_TILE, _XL_GREEN);

    _XL_DRAW(XSize/2-6+13,y,SHURIKEN_TILE, _XL_CYAN);
}


void initialize_new_level(void)
{    

    initialize_level_parameters();    

    if(challenge_level)
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_CLEAR_SCREEN();
        print_use_block_against_shurikens(YSize/2);
        _XL_WAIT_FOR_INPUT();    
    }
    
    init_map();    
    
    remaining_diamonds = 0; // build_objects increases remaining_diamonds

    if(challenge_level)
    {
        build_objects(FINAL_LEVEL+1);
    }    
    
    build_objects(level);
    
    time_counter = MAX_TIME;

    activate_shurikens();

    build_shurikens();
    
    remaining_shurikens = level_shurikens;
    
    build_barriers();
    
    // REMARK: Initialize counter *only* at level start (not after losing a life)
    counter = 0;
}


void display_horizontal_transition_shuriken(uint8_t x, uint8_t y)
{
    _XL_DRAW(x-1,y,SHURIKEN_TILE_LEFT, SHURIKEN_COLOR);
    _XL_DRAW(x,y,SHURIKEN_TILE_RIGHT, SHURIKEN_COLOR);
}


void shuriken_death(uint8_t index)
{
    _XL_SHOOT_SOUND();
    score+=SHURIKEN_POINTS;
    ++shuriken_counter;
    --remaining_shurikens;
    increase_time_counter_if_not_max();
    shuriken_status[index]=0;
}


void block_explosion(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y,BLOCK_TILE,_XL_RED);
    short_pause();
    _XL_SHOOT_SOUND();
    short_pause();
    delete_element(x,y);
}


#if XSize<31
    #define CHASE_BIT_MASK 15
#else
    #define CHASE_BIT_MASK 7
#endif


uint8_t player_chased(void)
{
    return (challenge_level)&&(!(_XL_RAND()&CHASE_BIT_MASK));
}


#define display_shuriken(x,y,index) \
    build_element(SHURIKEN,x,y)


void chase_vertically(uint8_t index)
{
    shuriken_axis[index]=SHURIKEN_VERTICAL;
    
    if(screen_y<shuriken_y[index])
    {
        shuriken_direction[index]=SHURIKEN_UP;
    }
    else
    {
        shuriken_direction[index]=SHURIKEN_DOWN; 
    }
}


void handle_horizontal_shuriken(register uint8_t index)
{
    register uint8_t x = shuriken_x[index];
    register uint8_t y = shuriken_y[index];
    
    if(shuriken_direction[index]==SHURIKEN_LEFT)
    {
        if(!shuriken_transition[index]) // transition not performed, yet
        {
            
            if(player_chased())
            {
                chase_vertically(index);
            }                
            else
            {
                if(!map[x-1][y])
                {
                    // Do left transition
                    display_horizontal_transition_shuriken(x,y);
                    map[x-1][y]=SHURIKEN;
                    ++shuriken_transition[index];
                }
                else if(map[x-1][y]==BLOCK)
                {
                    delete_element(x,y);
                    shuriken_death(index);
                    block_explosion(x-1,y);
                }
                else
                {
                    shuriken_direction[index]=SHURIKEN_RIGHT;
                }
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);
            --shuriken_x[index];
            display_shuriken(shuriken_x[index],y,index);
        }

    }
    else // direction is RIGHT
    {
        if(!shuriken_transition[index]) // transition not performed, yet
        {
            if(player_chased())
            {
                chase_vertically(index);
            }                
            else
            {
                if(!map[x+1][y])
                {
                    // Do right transition
                    display_horizontal_transition_shuriken(x+1,y);
                    map[x+1][y]=SHURIKEN;
                    ++shuriken_transition[index];
                }
                else if(map[x+1][y]==BLOCK)
                {
                    delete_element(x,y);
                    shuriken_death(index);
                    block_explosion(x+1,y);
                }
                else
                {
                    shuriken_direction[index]=SHURIKEN_LEFT;
                    
                }
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);
            ++shuriken_x[index];
            display_shuriken(shuriken_x[index],y,index);
        }
    }
}



void delete_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<level_shurikens;++i)
    {
        if(shuriken_status[i])
        {
            delete_element(shuriken_x[i],shuriken_y[i]);
            
            if(shuriken_transition[i])
            {
                if(shuriken_axis[i]==SHURIKEN_HORIZONTAL)
                {
                    if(shuriken_direction[i]==SHURIKEN_RIGHT)
                    {
                        delete_element(shuriken_x[i]+1,shuriken_y[i]);
                    }
                    else
                    {
                        delete_element(shuriken_x[i]-1,shuriken_y[i]);
                    }
                }
                else
                {
                    if(shuriken_direction[i]==SHURIKEN_DOWN)
                    {
                        delete_element(shuriken_x[i],shuriken_y[i]+1);
                    }
                    else
                    {
                        delete_element(shuriken_x[i],shuriken_y[i]-1);
                    }
                }
            }
        }
    }
    
    for(i=0;i<level_mini_shurikens;++i)
    {
        delete_element(mini_shuriken_x[i],mini_shuriken_y[i]);
    }
}



void display_vertical_transition_shuriken(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y-1,SHURIKEN_TILE_UP,SHURIKEN_COLOR);
    _XL_DRAW(x,y,SHURIKEN_TILE_DOWN,SHURIKEN_COLOR);
}


void chase_horizontally(uint8_t index)
{
    shuriken_axis[index] = SHURIKEN_HORIZONTAL;
        
    if(screen_x<shuriken_x[index])
    {
        shuriken_direction[index]=SHURIKEN_LEFT;
    }
    else
    {
        shuriken_direction[index]=SHURIKEN_RIGHT; 
    }    
}


void handle_vertical_shuriken(register uint8_t index)
{
    register uint8_t x = shuriken_x[index];
    register uint8_t y = shuriken_y[index];
    
    if(shuriken_direction[index]==SHURIKEN_UP)
    {
        if(!shuriken_transition[index]) // transition not performed, yet
        {
            if(player_chased())
            {
                chase_horizontally(index);
            }                
            else
            {
                if(!map[x][y-1])
                {
                    // Do up transition
                    display_vertical_transition_shuriken(x,y);
                    map[x][y-1]=SHURIKEN;
                    ++shuriken_transition[index];
                }
                else if(map[x][y-1]==BLOCK)
                {
                    delete_element(x,y);
                    shuriken_death(index);
                    block_explosion(x,y-1);
                }
                else
                {
                    shuriken_direction[index]=SHURIKEN_DOWN;
                }
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);
            --shuriken_y[index];
            display_shuriken(x,shuriken_y[index],index);
        }
    }
    else // direction is DOWN
    {
        if(!shuriken_transition[index]) // transition not performed, yet
        {
            if(player_chased())
            {
                chase_horizontally(index);
            }                
            else
            {
                if(!map[x][y+1])
                {
                    // Do right transition
                    display_vertical_transition_shuriken(x,y+1);
                    map[x][y+1]=SHURIKEN;
                    ++shuriken_transition[index];
                }
                else if(map[x][y+1]==BLOCK)
                {
                    delete_element(x,y);
                    shuriken_death(index);
                    block_explosion(x,y+1);
                }
                else
                {
                    shuriken_direction[index]=SHURIKEN_UP;
                }
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);    
            ++shuriken_y[index];
            display_shuriken(x,shuriken_y[index],index);
        }
    }
}



void handle_shurikens(void)
{
    uint8_t i;
    uint8_t x;
    uint8_t y;
    
    for(i=0;i<MAX_NUMBER_OF_SHURIKENS;++i)
    {
        if(shuriken_status[i])
        {
            if(!shuriken_axis[i])
            {
                handle_horizontal_shuriken(i);
            }
            else
            {
                handle_vertical_shuriken(i);
            }
        }
    }
    
    for(i=0;i<level_mini_shurikens;++i)
    {
        x = mini_shuriken_x[i];
        
        delete_element(x,mini_shuriken_y[i]);
        
        y=++(mini_shuriken_y[i]);

        if(!map[x][y] && y<YSize-2)
        {
            build_element(MINI_SHURIKEN, x, y);
        }
        else
        {    
            if(!map[x][y])
            {
                delete_element(x,y);
            }
            mini_shuriken_y[i] = 2;                            
        }
        
    }
}


void update_player_direction(uint8_t direction)
{
    update_player();
    if(player_direction!=direction)
    {
        force=0;
    }
    else
    {
        player_direction=direction;
    }
}


void handle_player(void)
{
    input = _XL_INPUT();
    
    if(_XL_UP(input) && (!(player_y&1) || allowed_up()))
    {
        if(player_y&1)
        {
            delete_player_down();
            if_block_push_up();
        }
        --player_y;

        update_player_direction(PLAYER_UP);
    }
    else if(_XL_DOWN(input) && ((player_y&1) || allowed_down()))
    {    
        if(!(player_y&1))
        {
            delete_player_up();
            if_block_push_down();
        }
        ++player_y;

        update_player_direction(PLAYER_DOWN);

    }
    else if(_XL_LEFT(input) && ((player_x&1) || allowed_left()))
    {
        if(!(player_x&1))
        {
            delete_player_right();
            if_block_push_left();
        }
        --player_x;

        update_player_direction(PLAYER_LEFT);

    }
    else if(_XL_RIGHT(input) && (!(player_x&1) || allowed_right()))
    {    
        if(player_x&1)
        {
            delete_player_left();
            if_block_push_right();
        }
        ++player_x;

        update_player_direction(PLAYER_RIGHT);

    }
    // REMARK: We need this because shuriken do delete the player despite handle_collisions
    else 
    {
        // force=0;
        if((ring_active&7)==1)
        {
            display_player();
        }
    }
}


#if !defined(_XL_NO_COLOR)
    #define initialize_player() \
    do { \
        alive = 1; \
        player_x = XSize-1; \
        player_y = YSize-1; \
        \
        force = 0; \
        ring_active = START_RING_EFFECT; \
        \
        player_color = _XL_CYAN; \
    } while(0)
#else
    #define initialize_player() \
    do { \
        alive = 1; \
        player_x = XSize-1; \
        player_y = YSize-1; \
        \
        force = 0; \
        ring_active = START_RING_EFFECT; \
        \
    } while(0)
#endif


#define initialize_global_game() \
do \
{ \
    score = 0; \
    lives = INITIAL_LIVES; \
    level = INITIAL_LEVEL; \
    extra_life_counter = 1; \
    new_level = 1; \
    initialize_level_parameters(); \
} while(0)


void handle_freeze_and_shurikens(void)
{
    if((!freeze_active) || (counter&1))
    {
        handle_shurikens();
    }
    else if(freeze_active)
    {
        --freeze_active;
    }
}


#if defined(INVINCIBLE)
    #define handle_ring()
#elif !defined(_XL_NO_COLOR)
    #define handle_ring() \
    do \
    { \
        if(ring_active) \
        { \
            --ring_active; \
            if(!ring_active) \
            { \
                player_color = _XL_WHITE; \
                display_player(); \
            } \
        } \
    } while(0)
#else
    #define handle_ring() \
    do \
    { \
        if(ring_active) \
        { \
            --ring_active; \
            if(!ring_active) \
            { \
                display_player(); \
            } \
        } \
    } while(0)
#endif


#if !defined(_XL_NO_COLOR)
    #define handle_lose_life() \
    do \
    { \
        player_color=_XL_RED; \
        display_player(); \
        _XL_EXPLOSION_SOUND(); \
        \
        --lives; \
        one_second_pause(); \
        _XL_WAIT_FOR_INPUT(); \
        \
        delete_player(); \
        \
        delete_shurikens(); \
    } while(0)
#else
    #define handle_lose_life() \
    do \
    { \
        display_player(); \
        _XL_EXPLOSION_SOUND(); \
        \
        --lives; \
        one_second_pause(); \
        _XL_WAIT_FOR_INPUT(); \
        \
        delete_player(); \
        \
        delete_shurikens(); \
    } while(0)  
#endif

void item_bonus(uint8_t *item_counter_ptr)
{
    if(*item_counter_ptr)
    {
        do
        {
        score+=ITEM_BONUS;
        --(*item_counter_ptr);
        update_item_display();
        _XL_ZAP_SOUND();
        short_pause();
        } while(*item_counter_ptr);
        one_second_pause();
    }
}


#define handle_next_level() \
do \
{ \
    ++level; \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINT(XSize/2-2,YSize/2,"GREAT"); \
    new_level = 1; \
    \
    one_second_pause(); \
    item_bonus(&time_counter); \
    item_bonus(&shuriken_counter); \
    item_bonus(&freeze_counter); \
    item_bonus(&ring_counter); \
} while(0)


#define initialize_player_achievements() \
do \
{ \
    freeze_counter=0; \
    freeze_active=0; \
    \
    ring_counter=0; \
    ring_active=0; \
    \
    shuriken_counter=0; \
} while(0)


#if XSize>=63
    #define TIME_MASK 127
#else
    #define TIME_MASK 63
#endif


#define handle_time() \
do \
{ \
    ++counter; \
    \
    if(!(counter&TIME_MASK)) \
    { \
        if(time_counter) \
        { \
            --time_counter; \
            update_time_counter_display(); \
        } \
    } \
} while(0)


#define handle_extra_life() \
do \
{ \
    if(score>=EXTRA_LIFE_THRESHOLD*extra_life_counter) \
    { \
        ++extra_life_counter; \
        _XL_PING_SOUND(); \
        ++lives; \
        update_lives_display(); \
        _XL_PING_SOUND(); \
    } \
} while(0)


#if defined(SHOW_LEVELS)
    #define continue_level_condition() 0
#else
    #define continue_level_condition() \
        alive && (remaining_diamonds || (remaining_shurikens && challenge_level))
#endif


void animate_shurikens(void)
{
    activate_shurikens();
    build_shurikens();
    counter=XSize/4;
    // #if defined(_XL_NO_TEXT_COLOR)
        // _XL_SET_TEXT_COLOR(_XL_WHITE);
        // #if !defined(_XL_NO_JOYSTICK)
        // _XL_PRINT(XSize/2-5,YSize-1,"PRESS FIRE");
        // #else
        // _XL_PRINT(XSize/2-5,YSize-1,"PRESS SPACE");
        // #endif
    // #endif
    do
    {
        if(counter)
        {
            --counter;
            #if defined(_XL_NO_TEXT_COLOR) || defined(_XL_NO_COLOR)
                if(!counter)
                {
					_XL_SET_TEXT_COLOR(_XL_WHITE);
					#if !defined(_XL_NO_JOYSTICK)
					_XL_PRINT(XSize/2-5,YSize-1,"PRESS FIRE");
					#else
					_XL_PRINT(XSize/2-5,YSize-1,"PRESS SPACE");
					#endif
                }
            #endif
        }
        else
        {
            #if !defined(_XL_NO_TEXT_COLOR) 
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                #if !defined(_XL_NO_JOYSTICK)
                _XL_PRINT(XSize/2-5,YSize-1,"PRESS FIRE");
                #else
                _XL_PRINT(XSize/2-5,YSize-1,"PRESS SPACE");
                #endif
            #endif
        }
        input = _XL_INPUT();
        
        handle_freeze_and_shurikens();
        short_pause();
    } while((!_XL_FIRE(input) || counter));
    _XL_ZAP_SOUND();
    one_second_pause();
}


#if !defined(_XL_NO_COLOR)

    #define title() \
    do \
    { \
        init_map(); \
        \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        \
        _XL_PRINT(XSize/2-7,AUTHOR_Y, "FABRIZIO CARUSO"); \
        \
        _XL_PRINTD(XSize/2-2,1,5,hiscore); \
        \
        _XL_PRINT(XSize/2-8+4,COLLECT_Y, "COLLECT"); \
        \
        print_use_block_against_shurikens(USE_AGAINST_Y); \
        \
        _XL_DRAW(XSize/2-7+11,COLLECT_Y,DIAMOND_TILE, _XL_GREEN); \
        \
        _XL_SET_TEXT_COLOR(_XL_CYAN); \
        \
        _XL_PRINT(XSize/2-7,SHURIKEN_Y, "S H U R I K E N"); \
        \
        screen_color[SHURIKEN]=_XL_YELLOW; \
        animate_shurikens(); \
    } while(0)

        
    #define the_end() \
    do \
    { \
        level=2; \
        challenge_level=0; \
        init_map(); \
        screen_color[SHURIKEN]=_XL_GREEN; \
        \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        _XL_PRINT(XSize/2-3,YSize/2,"THE END"); \
        \
        animate_shurikens(); \
    } while(0)

#else
        #define title() \
    do \
    { \
        init_map(); \
        \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        \
        _XL_PRINT(XSize/2-7,AUTHOR_Y, "FABRIZIO CARUSO"); \
        \
        _XL_PRINTD(XSize/2-2,1,5,hiscore); \
        \
        _XL_PRINT(XSize/2-8+4,COLLECT_Y, "COLLECT"); \
        \
        print_use_block_against_shurikens(USE_AGAINST_Y); \
        \
        _XL_DRAW(XSize/2-7+11,COLLECT_Y,DIAMOND_TILE, _XL_GREEN); \
        \
        _XL_SET_TEXT_COLOR(_XL_CYAN); \
        \
        _XL_PRINT(XSize/2-7,SHURIKEN_Y, "S H U R I K E N"); \
        \
        animate_shurikens(); \
    } while(0)

        
    #define the_end() \
    do \
    { \
        level=2; \
        challenge_level=0; \
        init_map(); \
        \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        _XL_PRINT(XSize/2-3,YSize/2,"THE END"); \
        \
        animate_shurikens(); \
    } while(0)
#endif


#define initialize_level() \
do \
{ \
    initialize_player_achievements(); \
    \
    if(new_level) \
    { \
        initialize_new_level(); \
    } \
    initialize_level_display(); \
    initialize_player(); \
    update_player(); \
    \
    new_level = 0; \
    one_second_pause();  \
    _XL_WAIT_FOR_INPUT(); \
} while(0)


#define handle_speed() \
do \
{ \
    short_pause(); \
} while(0)


#define handle_end_game() \
do \
{ \
    if(alive) \
    { \
        the_end(); \
    } \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    _XL_PRINT(XSize/2-4,YSize/2,"GAME OVER"); \
    \
    if(score>hiscore) \
    { \
        hiscore = score; \
    } \
    one_second_pause(); \
    _XL_WAIT_FOR_INPUT(); \
} while(0)


#define initialize_cross_lib() \
    _XL_INIT_GRAPHICS(); \
    _XL_INIT_INPUT();  \
    _XL_INIT_SOUND();


#define continue_game_condition() \
    lives && (level<=FINAL_LEVEL)


int main(void)
{        
    initialize_cross_lib();

    hiscore = 0;
        
    while(1)
    {
        initialize_global_game();

        title();
                        
        while(continue_game_condition())
        {            
            initialize_level();
            
            while(continue_level_condition())
            {
                handle_player();
                
                if(alive)
                {
                    handle_freeze_and_shurikens();
                    handle_barriers();                    
                    handle_ring();
                    handle_collisions();                    
                    handle_speed();
                    handle_extra_life();
                    handle_time();
                }
            }
            if(alive)
            {
                handle_next_level();
            }
            else
            {
                handle_lose_life();
            }
        }; 
        handle_end_game();
    }
    
    return EXIT_SUCCESS;
}

