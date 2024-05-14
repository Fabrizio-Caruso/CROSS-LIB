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

const uint8_t tiles[] = {
		_TILE_0, _TILE_1, _TILE_2, _TILE_3, 
		_TILE_4, _TILE_5, _TILE_6, _TILE_7, 
		_TILE_8, _TILE_9, _TILE_10, _TILE_11, 
		_TILE_12, _TILE_13, _TILE_14, _TILE_15, 
		_TILE_16, _TILE_17, _TILE_18, _TILE_19,
		_TILE_20, _TILE_21, _TILE_22, _TILE_23, 
        _TILE_24, _TILE_25,
        _TILE_26
		};

#define NUMBER_OF_COLORS 6
#define MAX_STRING_SIZE 10

#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
    #define FIRST_COLOR _XL_BLACK
#else
    #define FIRST_COLOR _XL_WHITE
#endif

// TODO: This requires _XL_DRAW to be a macro 
// that suppresses the last the color argument when _XL_NO_COLOR is set
#if !defined(_XL_NO_COLOR)
static const uint8_t tile_color[NUMBER_OF_COLORS] = {
    FIRST_COLOR,
    _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE};
#endif

const char strings[NUMBER_OF_COLORS][MAX_STRING_SIZE] = 
{
    _XL_M _XL_i _XL_n _XL_e,
    _XL_E _XL_n _XL_e _XL_r _XL_g _XL_y,
    _XL_B _XL_o _XL_n _XL_u _XL_s,
    _XL_P _XL_o _XL_i _XL_n _XL_t _XL_s,
    _XL_A _XL_p _XL_p _XL_l _XL_e,
    _XL_A _XL_p _XL_p _XL_l _XL_e,
};



const char color_name[NUMBER_OF_COLORS][MAX_STRING_SIZE] = { 
    #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
                                "BLACK",
    #else
                                "WHITE",
    #endif
                                "RED", 
                                "CYAN", 
                                "GREEN", 
                                "YELLOW", 
                                "BLUE", 
                                };


#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3

#if YSize<=15
    #define LINE_SKIP 1
#else
    #define LINE_SKIP 2
#endif    


int main(void)
{        

    uint8_t i;
    uint8_t j;
    uint8_t k;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();


    for(k=0;k<3;++k)
    {
        for(j=0;j<6;++j)
        {
            _XL_CLEAR_SCREEN();
            
            _XL_SET_TEXT_COLOR(tile_color[j]);
            
            _XL_PRINT(COL_OFFSET, 0, (char *) color_name[j]);
            
            _XL_WAIT_FOR_INPUT();

            for(i=0;i<_XL_NUMBER_OF_TILES;++i)
            {
                _XL_DRAW((i&7)*2+COL_OFFSET,(i/8)*LINE_SKIP+ROW_OFFSET,tiles[i],tile_color[j]);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
            
            _XL_SET_TEXT_COLOR(FIRST_COLOR);
            _XL_PRINT(COL_OFFSET,YSize-4, "PRESS FIRE");
            _XL_WAIT_FOR_INPUT();
        }
    }
    _XL_PRINT(COL_OFFSET,YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

