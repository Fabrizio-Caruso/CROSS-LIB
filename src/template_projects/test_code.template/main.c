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



#if ((YSize)+(Y_OFFSET)-1)>19
    #define MAX_TILES 19
#else
    #define MAX_TILES ((YSize)+(Y_OFFSET)-1)
#endif


#define NUMBER_OF_COLORS 6
#define MAX_STRING_SIZE 10


void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
    _XL_PRINT(((uint8_t) (XSize - strlen(Text))>>1), row, Text);    
}

#define PRINT_CENTERED(Text) \
    PRINT_CENTERED_ON_ROW((YSize>>1), Text)



 
const uint8_t tiles[] = {
        _TILE_0, _TILE_1, _TILE_2, _TILE_3, 
        _TILE_4, _TILE_5, _TILE_6, _TILE_7, 
        _TILE_8, _TILE_9, _TILE_10, _TILE_11, 
        _TILE_12, _TILE_13, _TILE_14, _TILE_15, 
        _TILE_16, _TILE_17, _TILE_18, _TILE_19,
        _TILE_20, _TILE_21, _TILE_22, _TILE_23, 
        _TILE_24, _TILE_25, _TILE_26
        };


#if !defined(_XL_NO_COLOR)
static const uint8_t tile_color[NUMBER_OF_COLORS] = {_XL_WHITE, _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE};
#endif


const char color_name[NUMBER_OF_COLORS][MAX_STRING_SIZE] = { 
                                "WHITE", 
                                "RED", 
                                "CYAN", 
                                "GREEN", 
                                "YELLOW", 
                                "BLUE", 
                                };


#define COL_OFFSET 4
#define ROW_OFFSET 12

int main(void)
{        

    uint8_t i;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();

    _XL_INIT_SOUND();


    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    _XL_PRINT(COL_OFFSET,   1, "PRINTD");
    _XL_PRINTD(COL_OFFSET  ,2,5, 1234U);
    _XL_PRINTD(COL_OFFSET+5,2,5,56789U);


    _XL_PRINT(COL_OFFSET, 4, _XL_a _XL_b _XL_c _XL_d _XL_e _XL_f _XL_g _XL_h _XL_i _XL_j);
    _XL_PRINT(COL_OFFSET, 5, _XL_k _XL_l _XL_m _XL_n _XL_o _XL_p _XL_q _XL_r _XL_s _XL_t);
    _XL_PRINT(COL_OFFSET, 6, _XL_u _XL_v _XL_w _XL_x _XL_y _XL_z);
    _XL_PRINT(COL_OFFSET, 8, _XL_A _XL_B _XL_C _XL_D _XL_E _XL_F _XL_G _XL_H _XL_I _XL_J);
    _XL_PRINT(COL_OFFSET, 9, _XL_K _XL_L _XL_M _XL_N _XL_O _XL_P _XL_Q _XL_R _XL_S _XL_T);
    _XL_PRINT(COL_OFFSET,10, _XL_U _XL_V _XL_W _XL_X _XL_Y _XL_Z);

    
    for(i=0;i<_XL_NUMBER_OF_TILES;++i)
    {
        _XL_DRAW((i&7)*2+COL_OFFSET,(i/8)+ROW_OFFSET,tiles[i],tile_color[i%NUMBER_OF_COLORS]);
       
    }
    
    for(i=0;i<YSize;++i)
    {
        _XL_DRAW(0,i,tiles[0],0);
        _XL_DRAW(XSize-1,i,tiles[0],0);
    }
    for(i=0;i<XSize;++i)
    {
        _XL_DRAW(i,0,tiles[1],0);
        _XL_DRAW(i,YSize-1,tiles[1],0);
    }
    _XL_SET_TEXT_COLOR(_XL_WHITE);

    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

    _XL_PRINT(COL_OFFSET,YSize-2, "END OF TEST");
    _XL_REFRESH();

    
    return EXIT_SUCCESS;
}

