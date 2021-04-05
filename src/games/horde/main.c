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

#include "images.h"


#define PLAYER_Y ((YSize)-1)
#define MAX_PLAYER_X ((XSize)*2-3)

#define NUMBER_OF_ARROWS 3
#define RELOAD_LOOPS 7

#define AVAILABLE 0
#define ACTIVE 1

static uint8_t zombie_pos[XSize];
static uint8_t zombie_status[XSize];
static uint8_t zombie_index;

static const uint8_t zombie_tile[7+1] = 
{
    ZOMBIE_TILE_0, // 0
    ZOMBIE_TILE_0, // 1
    ZOMBIE_TILE_1, // 2
    ZOMBIE_TILE_2, // 3
    ZOMBIE_TILE_3, 
    ZOMBIE_TILE_4, 
    ZOMBIE_TILE_5, 
    ZOMBIE_TILE_6
};


static const uint8_t bow_tile[8] =
{
    EMPTY_BOW_LEFT_TILE_0,
    EMPTY_BOW_RIGHT_TILE_0,
    EMPTY_BOW_LEFT_TILE_1,
    EMPTY_BOW_RIGHT_TILE_1,
    LOADED_BOW_LEFT_TILE_0,
    LOADED_BOW_RIGHT_TILE_0,
    LOADED_BOW_LEFT_TILE_1,
    LOADED_BOW_RIGHT_TILE_1,
};

static const uint8_t arrow_tile[2] =
{
    ARROW_TILE_0,
    ARROW_TILE_1,
};

static uint8_t player_x = XSize; // range: 0..2*XSize-2^M
static uint8_t player_shape_tile;

static uint8_t input;

static uint8_t loaded_bow;
static uint8_t arrow_status[NUMBER_OF_ARROWS];
static uint8_t arrow_shape[NUMBER_OF_ARROWS];
static uint8_t arrow_x[NUMBER_OF_ARROWS];
static uint8_t arrow_y[NUMBER_OF_ARROWS];
static uint8_t next_arrow;
static uint8_t arrows_counter;
static uint8_t bow_load_counter;

void display_player(void)
{
    _XL_DRAW(player_x/2,PLAYER_Y,bow_tile[4*loaded_bow+0+player_shape_tile],_XL_GREEN);
    _XL_DRAW(player_x/2+1,PLAYER_Y,bow_tile[1+4*loaded_bow+player_shape_tile],_XL_GREEN);  
}

void move_left(void)
{
    player_shape_tile = 2*((--player_x)&1);
    // _XL_PRINTD(2,2,3,new_tile_shape);
    if(player_shape_tile)
    {
        _XL_DELETE(player_x/2+2,PLAYER_Y);
    }
    // _XL_DRAW(player_x/2,PLAYER_Y,bow_tile[4*loaded_bow+0+new_tile_shape],_XL_GREEN);
    // _XL_DRAW(player_x/2+1,PLAYER_Y,bow_tile[1+4*loaded_bow+new_tile_shape],_XL_GREEN);   
    display_player();
}

void move_right(void)
{
    player_shape_tile = 2*((++player_x)&1);
    // _XL_PRINTD(2,2,3,new_tile_shape);
    if(!player_shape_tile)
    {
        _XL_DELETE(player_x/2-1,PLAYER_Y);
    }
    // _XL_DRAW(player_x/2,PLAYER_Y,bow_tile[4*loaded_bow+new_tile_shape],_XL_GREEN);
    // _XL_DRAW(player_x/2+1,PLAYER_Y,bow_tile[1+4*loaded_bow+new_tile_shape],_XL_GREEN);
    display_player();
}


void zombie_display(void)
{
    uint8_t status = zombie_status[zombie_index];
    uint8_t pos = zombie_pos[zombie_index];
    
    if(!status)
    {
        _XL_DELETE(zombie_index, pos-1);
        _XL_DRAW(zombie_index, pos, ZOMBIE_TILE_0, _XL_WHITE);
    }
    else
    {
        _XL_DRAW(zombie_index, pos, zombie_tile[status<<1], _XL_WHITE);
        _XL_DRAW(zombie_index,1 + pos, zombie_tile[1+(status<<1)], _XL_WHITE);
    }
}


void die(void)
{
    uint8_t pos = zombie_pos[zombie_index];
    
    _XL_DELETE(zombie_index,pos-1);    
    _XL_DELETE(zombie_index,pos);
    _XL_DELETE(zombie_index,pos+1);
    _XL_DRAW(zombie_index,pos, ZOMBIE_DEATH_TILE_0, _XL_RED);
    _XL_SHOOT_SOUND();
    // _XL_WAIT_FOR_INPUT();
    _XL_DRAW(zombie_index,pos, ZOMBIE_DEATH_TILE_1, _XL_RED);
    // _XL_WAIT_FOR_INPUT();
    _XL_SHOOT_SOUND();

    _XL_DELETE(zombie_index,pos);
    
    // _XL_WAIT_FOR_INPUT();

    zombie_status[zombie_index]=0;
    zombie_pos[zombie_index]=1;
}

uint8_t compute_next_arrow(void)
{
    uint8_t i;
    for(i=0;i<NUMBER_OF_ARROWS;++i)
    {
        if(arrow_status[i]==AVAILABLE)
        {
            return i;
        }
    }
    return i;
}


void handle_arrows(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_ARROWS;++i)
    {
        if(arrow_status[i])
        {
            _XL_DELETE(arrow_x[i],arrow_y[i]);
            if(arrow_y[i]<2)
            {
                arrow_status[i]=0;
                --arrows_counter;
            }
            else
            {
                _XL_DRAW(arrow_x[i],--arrow_y[i],arrow_shape[i],_XL_YELLOW);
            }
        }
    }
}


int main(void)
{   
    uint8_t i;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();
    
    
    loaded_bow = 1;
    next_arrow = 0;
    arrows_counter = 0;
    bow_load_counter = 0;
    for(i=0;i<NUMBER_OF_ARROWS;++i)
    {
        arrow_status[i] = AVAILABLE;
        // arrow_x[i] = (i/2)+(i&1);
    }
    
    for(zombie_index=0;zombie_index<XSize;++zombie_index)
    {
        zombie_pos[zombie_index]=1;
        zombie_status[zombie_index]=0;
    }
    
    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    _XL_WAIT_FOR_INPUT();
    
    _XL_DRAW(XSize/2,PLAYER_Y,LOADED_BOW_LEFT_TILE_0,_XL_GREEN);
    _XL_DRAW(XSize/2+1,PLAYER_Y,LOADED_BOW_RIGHT_TILE_0,_XL_GREEN);
    
    _XL_SLEEP(1);
    
    while(1)
    {
        zombie_index=_XL_RAND()%XSize;
        
        zombie_display();
        

        if(zombie_pos[zombie_index]<YSize-1)
        {
            ++zombie_status[zombie_index];
            (zombie_status[zombie_index])&=3;
            if(!zombie_status[zombie_index])
            {
                ++zombie_pos[zombie_index];
            }
        }

        input = _XL_INPUT();
        
        if(_XL_LEFT(input) && player_x>0)
        {
            move_left();
        }
        else if (_XL_RIGHT(input) && player_x<MAX_PLAYER_X)
        {
            move_right();
        }
        else if (_XL_FIRE(input) && loaded_bow)
        {
            loaded_bow = 0;
            arrow_status[next_arrow] = 1;
            ++arrows_counter;
            bow_load_counter = RELOAD_LOOPS;
            arrow_shape[next_arrow] = arrow_tile[player_x&1];
            arrow_y[next_arrow] = PLAYER_Y-1;
            _XL_SHOOT_SOUND();
            arrow_x[next_arrow] = (player_x/2)+(player_x&1);
            // _XL_PRINTD(0,0,3,player_x&1);
            _XL_DRAW(arrow_x[next_arrow],PLAYER_Y-1,arrow_shape[next_arrow],_XL_YELLOW);
            display_player();
        }
        
        if(!loaded_bow && arrows_counter<NUMBER_OF_ARROWS && !bow_load_counter)
        {
            loaded_bow = 1;
            next_arrow = compute_next_arrow();
            display_player();
        }
        if(bow_load_counter)
        {
            --bow_load_counter;
        }
        
        handle_arrows();
        
        _XL_PRINTD(0,0,3,bow_load_counter);
        _XL_PRINTD(6,0,3,arrows_counter);
        // _XL_PRINTD(12,0,2,next_arrow);   
    }


    return EXIT_SUCCESS;
}

