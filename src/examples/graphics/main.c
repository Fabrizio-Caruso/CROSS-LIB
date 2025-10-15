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

#define NUMBER_OF_COLORS 7
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
    _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE, _XL_MAGENTA};
#endif


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
                                "MAGENTA",
                                };


#if XSize<20
    #define COL_OFFSET 0
#else
    #define COL_OFFSET ((XSize/2)-3)
#endif

#if YSize<12
    #define  ROW_OFFSET 1
#else
    
    #define ROW_OFFSET (YSize/7)
#endif

#if YSize<=15
    #define CHAR_SKIP 1
    #define LINE_SKIP 1
#else
    #define CHAR_SKIP 2
    #define LINE_SKIP 2
#endif    

#if defined(_XL_NO_JOYSTICK)
    #define _PRESS "PRESS A KEY"
#else
    #define _PRESS "PRESS FIRE"
#endif


int main(void)
{        

    uint8_t i;
    uint8_t j;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();


    for(;;)
    {
        for(j=0;j<NUMBER_OF_COLORS;++j)
        {
            _XL_CLEAR_SCREEN();
            
            _XL_SET_TEXT_COLOR(tile_color[j]);
            
            _XL_PRINT(COL_OFFSET, 0, (char *) color_name[j]);
            
            _XL_WAIT_FOR_INPUT();

            #if YSize>=16
            _XL_SET_TEXT_COLOR(FIRST_COLOR);
            _XL_PRINT(COL_OFFSET,YSize-4, _PRESS);
            #endif

            for(i=0;i<_XL_NUMBER_OF_TILES;++i)
            {
                _XL_DRAW((i&7)*CHAR_SKIP+COL_OFFSET,(i/8)*LINE_SKIP+ROW_OFFSET,tiles[i],tile_color[j]);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
            
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            
            _XL_WAIT_FOR_INPUT();
        }
    }
    _XL_PRINT(COL_OFFSET,YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

