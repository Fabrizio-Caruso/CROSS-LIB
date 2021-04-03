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


// #define FRAME_0 0
// #define FRAME_1 1
// #define FRAME_2 2
// #define FRAME_3 3

#define Y_PLAYER_POS ((YSize)-3)

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
    LOADED_BOW_LEFT_TILE_0,
    LOADED_BOW_RIGHT_TILE_0,
    LOADED_BOW_LEFT_TILE_1,
    LOADED_BOW_RIGHT_TILE_1,
    EMPTY_BOW_LEFT_TILE_0,
    EMPTY_BOW_RIGHT_TILE_0,
    EMPTY_BOW_LEFT_TILE_1,
    EMPTY_BOW_RIGHT_TILE_1,
};


static uint8_t player_pos = XSize; // range: 0..2*XSize-2^M

static uint8_t input;


void move_left(void)
{
   
    uint8_t new_tile_shape;

    new_tile_shape = 2*((--player_pos)&1);
    _XL_PRINTD(2,2,3,new_tile_shape);
    if(new_tile_shape)
    {
        _XL_DELETE(player_pos/2+2,Y_PLAYER_POS);
    }
    _XL_DRAW(player_pos/2,Y_PLAYER_POS,bow_tile[0+new_tile_shape],_XL_GREEN);
    _XL_DRAW(player_pos/2+1,Y_PLAYER_POS,bow_tile[1+new_tile_shape],_XL_GREEN);    
}

void move_right(void)
{
   
    uint8_t new_tile_shape;

    new_tile_shape = 2*((++player_pos)&1);
    _XL_PRINTD(2,2,3,new_tile_shape);
    if(!new_tile_shape)
    {
        _XL_DELETE(player_pos/2-1,Y_PLAYER_POS);
    }
    _XL_DRAW(player_pos/2,Y_PLAYER_POS,bow_tile[0+new_tile_shape],_XL_GREEN);
    _XL_DRAW(player_pos/2+1,Y_PLAYER_POS,bow_tile[1+new_tile_shape],_XL_GREEN);    
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
    _XL_EXPLOSION_SOUND();
    _XL_DELETE(zombie_index,pos);
    
    // _XL_WAIT_FOR_INPUT();

    zombie_status[zombie_index]=0;
    zombie_pos[zombie_index]=1;
}


int main(void)
{
    uint8_t move_or_die;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();
    
    for(zombie_index=0;zombie_index<XSize;++zombie_index)
    {
        zombie_pos[zombie_index]=1;
        zombie_status[zombie_index]=0;
    }
    
    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    _XL_WAIT_FOR_INPUT();
    
    
    _XL_DRAW(XSize/2,Y_PLAYER_POS,LOADED_BOW_LEFT_TILE_0,_XL_GREEN);
    _XL_DRAW(XSize/2+1,Y_PLAYER_POS,LOADED_BOW_RIGHT_TILE_0,_XL_GREEN);
    
    while(1)
    {
        zombie_index=_XL_RAND()%XSize;
        
        move_or_die=_XL_RAND()%XSize;
        
        if(move_or_die>XSize-2)
        {
            die();
        }
        else
        {
            zombie_display();
        }

        if(zombie_pos[zombie_index]<YSize-1)
        {
            ++zombie_status[zombie_index];
            (zombie_status[zombie_index])&=3;
            if(!zombie_status[zombie_index])
            {
                ++zombie_pos[zombie_index];
            }
        }
        // _XL_SLOW_DOWN(1500);
        // _XL_WAIT_FOR_INPUT();
        input = _XL_INPUT();
        
        if(_XL_LEFT(input))
        {
            move_left();
        }
        else if (_XL_RIGHT(input))
        {
            move_right();
        }
    }


    return EXIT_SUCCESS;
}

