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

#define MIN_X 1
#define MAX_X (XSize-2)
#define MIN_Y 1
#define MAX_Y (YSize-2)

#define DOWN_TILE  _TILE_0
#define UP_TILE    _TILE_1 
#define RIGHT_TILE _TILE_2
#define LEFT_TILE  _TILE_3 
#define FIRE_TILE  _TILE_4

int main(void)
{   

    uint8_t x;
    uint8_t y;
    uint8_t input;
    uint8_t tile;

    _XL_INIT_GRAPHICS();
    _XL_INIT_SOUND();
    _XL_INIT_INPUT();

    _XL_CLEAR_SCREEN();

    x = XSize/2;
    y = YSize/2;
    
    tile = DOWN_TILE;
    
    while(1)
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0,0,3,x);
        _XL_PRINTD(5,0,3,y);
        
        input = _XL_INPUT();
        if(_XL_UP(input))
        {
            _XL_DELETE(x,y);
            tile = UP_TILE;
            --y;
        }
        else if (_XL_DOWN(input))
        {
            _XL_DELETE(x,y);
            tile = DOWN_TILE;
            ++y;
        }
        else if (_XL_LEFT(input))
        {
            _XL_DELETE(x,y);
            tile = LEFT_TILE;
            --x;
        }
        else if(_XL_RIGHT(input))
        {
            _XL_DELETE(x,y);
            tile = RIGHT_TILE;
            ++x;
        }
        else if(_XL_FIRE(input))
        {
            _XL_DRAW(x,y,FIRE_TILE,_XL_WHITE);
            _XL_EXPLOSION_SOUND();
            _XL_SLOW_DOWN(16*_XL_SLOW_DOWN_FACTOR);  
        }
        
        if((y>=MIN_Y)&&(y<=MAX_Y)&&(x>=MIN_X)&&(x<=MAX_X))
        {
            _XL_DRAW(x,y,tile,_XL_WHITE);
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
        else
        {
            _XL_DELETE(x,y);
            _XL_ZAP_SOUND();
            _XL_SLOW_DOWN(16*_XL_SLOW_DOWN_FACTOR);  
            x = XSize/2;
            y = YSize/2;
        }
        
        _XL_SLOW_DOWN(4*_XL_SLOW_DOWN_FACTOR);
        
    }
    
    return EXIT_SUCCESS;
}

