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

#include "tile_aliases.h"

#if ((YSize)+(Y_OFFSET)-19)>=1
    #define MAX_INIT_Y_POS ((YSize)+(Y_OFFSET)-19)
#else
    #define MAX_INIT_Y_POS 1
#endif

#if ((YSize)+(Y_OFFSET)-1)>19
    #define MAX_TILES 19
#else
    #define MAX_TILES ((YSize)+(Y_OFFSET)-1)
#endif


static const uint8_t tiles[19] = {
        PLAYER_DOWN_TILE, PLAYER_UP_TILE, PLAYER_RIGHT_TILE, PLAYER_LEFT_TILE, 
        GHOST_TILE, BOMB_TILE, DEAD_GHOST_TILE, SKULL_TILE, 
        CALM_DOWN_TILE, FIRE_POWER_TILE, BULLET_TILE, VERTICAL_BRICK_TILE, 
        HORIZONTAL_BRICK_TILE, LEFT_HORIZONTAL_MISSILE_TILE, RIGHT_HORIZONTAL_MISSILE_TILE, ROCKET_TILE, 
        FREEZE_TILE, SUPER_TILE, INVINCIBILITY_TILE
        };

#if !defined(_XL_NO_COLOR)
static const uint8_t tile_colors[19] = {
        _XL_CYAN, _XL_CYAN, _XL_CYAN, _XL_CYAN, 
        _XL_WHITE, _XL_RED, _XL_RED, _XL_YELLOW, 
        _XL_GREEN, _XL_YELLOW, _XL_WHITE, _XL_YELLOW, 
        _XL_YELLOW, _XL_WHITE, _XL_WHITE, _XL_WHITE, 
        _XL_CYAN, _XL_RED, _XL_YELLOW
        };
#endif


int main(void)
{        
    uint8_t i;
    uint8_t j;
    uint8_t init_y;
    uint8_t max_j;
    
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    
    while(1)
    {
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(4,0, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
        _XL_WAIT_FOR_INPUT();
        _XL_CLEAR_SCREEN();
        while(1)
        {
            i = (uint8_t) (_XL_RAND() % XSize);   
            max_j = (uint8_t) (_XL_RAND() % (MAX_TILES+1));
            init_y = (uint8_t) (_XL_RAND() % MAX_INIT_Y_POS);
            for(j=0;j<max_j;++j)
            {
                _XL_DRAW(i,j+init_y,tiles[j],tile_colors[j]);
                _XL_SLOW_DOWN(900);
            }
        }
    } 
    return EXIT_SUCCESS;
}

