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

// TODO: REMOVE THIS
#define MAX_INIT_Y_POS ((YSize)+(Y_OFFSET)-19)

#if ((YSize)+(Y_OFFSET)-1)>19
    #define MAX_TILES 19
#else
    #define MAX_TILES ((YSize)+(Y_OFFSET)-1)
#endif



// Global initialization
//<global>
const uint8_t tiles[NUM_OF_TILES+1] = {
		PLAYER_DOWN_TILE, PLAYER_UP_TILE, PLAYER_RIGHT_TILE, PLAYER_LEFT_TILE, 
		GHOST_TILE, BOMB_TILE, DEAD_GHOST_TILE, SKULL_TILE, 
		CALM_DOWN_TILE, FIRE_POWER_TILE, BULLET_TILE, VERTICAL_BRICK_TILE, 
		HORIZONTAL_BRICK_TILE, LEFT_HORIZONTAL_MISSILE_TILE, RIGHT_HORIZONTAL_MISSILE_TILE, ROCKET_TILE, 
		FREEZE_TILE, SUPER_TILE, INVINCIBILITY_TILE, EXTRA_POINTS_TILE,
		EXTRA1_TILE, EXTRA2_TILE, EXTRA3_TILE, EXTRA4_TILE, EXTRA5_TILE, EXTRA6_TILE
		};
        
const uint8_t tile_colors[NUM_OF_TILES+1] = {
		_PLAYER_COLOR, _PLAYER_COLOR, _PLAYER_COLOR, _PLAYER_COLOR, 
		_XL_WHITE, _XL_RED, _XL_RED, _XL_YELLOW, 
		_XL_GREEN, _XL_YELLOW, _XL_WHITE, _XL_YELLOW, 
		_XL_YELLOW, _XL_WHITE, _XL_WHITE, _XL_WHITE, 
		_XL_CYAN, _XL_RED, _XL_YELLOW, _XL_YELLOW,
		_XL_CYAN, _XL_CYAN, _XL_CYAN, _XL_CYAN, _XL_CYAN, _XL_CYAN
		};
//</global>

#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3


int main(void)
{        

    uint8_t i;
    uint8_t j;

    _XL_INIT_GRAPHICS();

    INIT_INPUT();
    
    for(j=0;j<3;++j)
    {
        CLEAR_SCREEN();
        
        SET_TEXT_COLOR(_XL_WHITE);
        PRINT(COL_OFFSET,   2,"0123456789");
        PRINTD(COL_OFFSET  ,3,5, 1234U);
        PRINTD(COL_OFFSET+5,3,5,56789U);
        PRINT(COL_OFFSET, 4,  "\x24" _XL_SPACE "\x25" _XL_SPACE "\x26" _XL_SPACE "\xA4" _XL_SPACE "\xA5" _XL_SPACE "\xA6");
        PRINT(COL_OFFSET, 6, _XL_a _XL_b _XL_c _XL_d _XL_e _XL_f _XL_g _XL_h _XL_i _XL_j "-");
        PRINT(COL_OFFSET, 7, _XL_k _XL_l _XL_m _XL_n _XL_o _XL_p _XL_q _XL_r _XL_s _XL_t "-");
        PRINT(COL_OFFSET, 8, _XL_u _XL_v _XL_w _XL_x _XL_y _XL_z);
        PRINT(COL_OFFSET,10, _XL_A _XL_B _XL_C _XL_D _XL_E _XL_F _XL_G _XL_H _XL_I _XL_J "-");
        PRINT(COL_OFFSET,11, _XL_K _XL_L _XL_M _XL_N _XL_O _XL_P _XL_Q _XL_R _XL_S _XL_T "-");
        PRINT(COL_OFFSET,12, _XL_U _XL_V _XL_W _XL_X _XL_Y _XL_Z);
        
        _XL_WAIT_FOR_INPUT();
        CLEAR_SCREEN();

        for(i=0;i<NUM_OF_TILES+1;++i)
        {
            _XL_DRAW((i&7)*2+COL_OFFSET,(i/8)*2+ROW_OFFSET,tiles[i],tile_colors[i]);
            DO_SLOW_DOWN(900);
        }

        
        
        SET_TEXT_COLOR(_XL_WHITE);
        PRINT(COL_OFFSET,YSize-5, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
        _XL_WAIT_FOR_INPUT();
    }

    PRINT(COL_OFFSET,YSize-5, _XL_E _XL_N _XL_D _XL_SPACE _XL_O _XL_F _XL_SPACE _XL_D _XL_E _XL_M _XL_O);

    while(1){};
    
    return EXIT_SUCCESS;
}

