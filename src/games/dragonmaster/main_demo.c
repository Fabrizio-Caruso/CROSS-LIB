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



void draw_castle(void)
{
    _XL_DRAW(XSize/2-1,YSize/2,  CASTLE_NW_TILE,_XL_WHITE);
    _XL_DRAW(XSize/2,  YSize/2,  CASTLE_NE_TILE,_XL_WHITE);
    _XL_DRAW(XSize/2-1,YSize/2+1,CASTLE_SW_TILE,_XL_WHITE);
    _XL_DRAW(XSize/2,YSize/2+1,  CASTLE_SE_TILE,_XL_WHITE);
    _XL_DRAW(XSize/2-2,YSize/2+1, BRIDGE_DOWN_TILE,_XL_WHITE);
}


void draw_left_dragon(uint8_t x, uint8_t y, uint8_t color)
{
    _XL_DRAW(x-1,y,  LEFT_NW_TILE,color);
    _XL_DRAW(x,  y,  LEFT_NE_TILE,color);
    _XL_DRAW(x-1,y+1,LEFT_SW_TILE,color);
    _XL_DRAW(x,y+1,  LEFT_SE_TILE,color);
}


void draw_right_dragon(uint8_t x, uint8_t y, uint8_t color)
{
    _XL_DRAW(x-1,y,  RIGHT_NW_TILE,color);
    _XL_DRAW(x,  y,  RIGHT_NE_TILE,color);
    _XL_DRAW(x-1,y+1,RIGHT_SW_TILE,color);
    _XL_DRAW(x,y+1,  RIGHT_SE_TILE,color);
}

#define DRAGONS ((XSize)/5 * ((YSize)/10))


void draw_dragons(void)
{
    uint8_t i;
    
    for(i=0;i<DRAGONS;++i)
    {
        draw_left_dragon(2+_XL_RAND()%(XSize-4),2+_XL_RAND()%(YSize-4),_XL_RED);
        draw_right_dragon(2+_XL_RAND()%(XSize-4),2+_XL_RAND()%(YSize-4),_XL_WHITE);
    }
}

void draw_wall(void)
{
    uint8_t i;
    for(i=0;i<XSize;++i)
    {
        _XL_DRAW(i,0,WALL_TILE, _XL_WHITE);
        _XL_DRAW(i,YSize-1,WALL_TILE, _XL_WHITE);
    }
    for(i=0;i<YSize;++i)
    {
        _XL_DRAW(0,i,WALL_TILE, _XL_WHITE);
        _XL_DRAW(XSize-1,i,WALL_TILE, _XL_WHITE);
    }
}

void draw_player(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y,PLAYER_TILE, _XL_WHITE);
}


void draw_chase(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y,CHASE_TILE, _XL_WHITE);
}

void spawn_chase(void)
{
    uint8_t x;
    uint8_t y;
    
    x = 1+(_XL_RAND()&1)*(XSize-3);
    y = 1+(_XL_RAND()&1)*(YSize-3);
    draw_chase(x,y);
}

int main(void)
{        


    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();

    // _XL_CLEAR_SCREEN();


    while(1)
    {
        _XL_CLEAR_SCREEN();
        draw_wall();
        draw_castle();

        draw_dragons();
        
        draw_player(XSize/2-1,2);
        
        spawn_chase();
        
        _XL_WAIT_FOR_INPUT();
    }
     
    return EXIT_SUCCESS;
}

