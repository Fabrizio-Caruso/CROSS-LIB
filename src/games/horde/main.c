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

#define NUMBER_OF_ARROWS 1
#define RELOAD_LOOPS 2


static uint8_t zombie_y[XSize];
static uint8_t zombie_shape[XSize];
static uint8_t zombie_x;

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

static uint8_t player_x; // range: 0..2*XSize-2^M
static uint8_t player_shape_tile;

static uint8_t input;

static uint8_t loaded_bow;
static uint8_t active_arrow[NUMBER_OF_ARROWS];
static uint8_t arrow_shape[NUMBER_OF_ARROWS];
static uint8_t arrow_x[NUMBER_OF_ARROWS];
static uint8_t arrow_y[NUMBER_OF_ARROWS];
static uint8_t next_arrow;
static uint8_t arrows_counter;
static uint8_t bow_load_counter;
static uint8_t alive;


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
    uint8_t status = zombie_shape[zombie_x];
    uint8_t pos = zombie_y[zombie_x];
    
    if(!status)
    {
        _XL_DELETE(zombie_x, pos-1);
        _XL_DRAW(zombie_x, pos, ZOMBIE_TILE_0, _XL_WHITE);
    }
    else
    {
        _XL_DRAW(zombie_x, pos, zombie_tile[status<<1], _XL_WHITE);
        _XL_DRAW(zombie_x,1 + pos, zombie_tile[1+(status<<1)], _XL_WHITE);
    }
}


void die(void)
{
    uint8_t pos = zombie_y[zombie_x];
    
    _XL_DELETE(zombie_x,pos-1);    
    _XL_DELETE(zombie_x,pos);
    _XL_DELETE(zombie_x,pos+1);
    _XL_DRAW(zombie_x,pos, ZOMBIE_DEATH_TILE_0, _XL_RED);
    _XL_SHOOT_SOUND();
    // _XL_WAIT_FOR_INPUT();
    _XL_DRAW(zombie_x,pos, ZOMBIE_DEATH_TILE_1, _XL_RED);
    // _XL_WAIT_FOR_INPUT();
    _XL_SHOOT_SOUND();

    _XL_DELETE(zombie_x,pos);
    
    // _XL_WAIT_FOR_INPUT();

    zombie_shape[zombie_x]=0;
    zombie_y[zombie_x]=1;
}

uint8_t compute_next_arrow(void)
{
    uint8_t i;
    for(i=0;i<NUMBER_OF_ARROWS;++i)
    {
        if(!active_arrow[i])
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
        if(active_arrow[i]) // ACTIVE
        {
            _XL_DELETE(arrow_x[i],arrow_y[i]);
            if(arrow_y[i]<2)
            {
                active_arrow[i]=0;
                --arrows_counter;
            }
            else
            {
                _XL_DRAW(arrow_x[i],--arrow_y[i],arrow_shape[i],_XL_YELLOW);
            }
        }
    }
}

uint8_t zombie_hit(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_ARROWS;++i)
    {
        if(active_arrow[i] && arrow_x[i]==zombie_x
          && zombie_y[zombie_x]>=arrow_y[i])
           {
               active_arrow[i]=0;
                --arrows_counter;

               _XL_DELETE(arrow_x[i],arrow_y[i]);
               return 1;
           }
    }
    return 0;
}


void handle_zombies(void)
{
    zombie_x=_XL_RAND()%XSize;
    zombie_display();
    
    if(zombie_y[zombie_x]<YSize-1)
    {
        ++zombie_shape[zombie_x];
        (zombie_shape[zombie_x])&=3;
        if(!zombie_shape[zombie_x])
        {
            ++zombie_y[zombie_x];
        }
    }
    else
    {
        alive = 0;
    }
    
    for(zombie_x=0;zombie_x<XSize;++zombie_x)
    {
        if(zombie_hit())
        {
            die();
        }
    }
}


void handle_player_move(void)
{
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
        active_arrow[next_arrow] = 1;
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
}


void handle_bow(void)
{
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
}

void game_over(void)
{
    _XL_EXPLOSION_SOUND();
    
    _XL_PRINT_CENTERED(_XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R);
    
    _XL_SLEEP(1);
    
    _XL_WAIT_FOR_INPUT();
    
    _XL_CLEAR_SCREEN();
}


void initialize_vars(void)
{
    uint8_t i;

    alive = 1;
    loaded_bow = 1;
    next_arrow = 0;
    arrows_counter = 0;
    bow_load_counter = 0;
    player_x = XSize;
    for(i=0;i<NUMBER_OF_ARROWS;++i)
    {
        active_arrow[i] = 0;
    }
    
    for(zombie_x=0;zombie_x<XSize;++zombie_x)
    {
        zombie_y[zombie_x]=1;
        zombie_shape[zombie_x]=0;
    }
}


void display_initial_screen(void)
{
    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT_CENTERED(_XL_C _XL_SPACE _XL_R _XL_SPACE _XL_O _XL_SPACE _XL_S _XL_SPACE _XL_S 
                       _XL_SPACE _XL_SPACE 
                       _XL_H _XL_SPACE _XL_O _XL_SPACE _XL_R _XL_SPACE _XL_D _XL_SPACE _XL_E);
    
    _XL_SLEEP(1);
}


int main(void)
{       
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();
    
    while(1)
    {
        initialize_vars();
        
        display_initial_screen();

        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();
        
        // _XL_DRAW(XSize/2,PLAYER_Y,LOADED_BOW_LEFT_TILE_0,_XL_GREEN);
        // _XL_DRAW(XSize/2+1,PLAYER_Y,LOADED_BOW_RIGHT_TILE_0,_XL_GREEN);
        
        display_player();
        
        _XL_SLEEP(1);
        
        while(alive)
        {


            handle_zombies();

            handle_player_move();
            
            handle_bow();
            
            handle_arrows();
            
            // _XL_PRINTD(0,0,3,bow_load_counter);
            // _XL_PRINTD(6,0,3,arrows_counter);
            // _XL_PRINTD(12,0,2,next_arrow);   
        }
        game_over();
    }

    return EXIT_SUCCESS;
}

