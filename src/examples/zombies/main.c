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

#include "images.h"

#define _PRESS_FIRE__STRING "PRESS FIRE"

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
    _XL_DRAW(zombie_index,pos, ZOMBIE_DEATH_TILE_1, _XL_RED);
    _XL_EXPLOSION_SOUND();
    _XL_DELETE(zombie_index,pos);
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
    
    _XL_PRINT((XSize-10)/2,YSize/2,_PRESS_FIRE__STRING);

    _XL_WAIT_FOR_INPUT();

    _XL_CLEAR_SCREEN();

    while(1)
    {
        zombie_index= (uint8_t) _XL_RAND()%XSize;
        
        move_or_die= (uint8_t) _XL_RAND()%XSize;
        
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
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return EXIT_SUCCESS;
}

