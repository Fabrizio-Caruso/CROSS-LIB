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

#define NUMBER_OF_COLORS 6
#define MAX_STRING_SIZE 10

const uint8_t tile[] = {
		_TILE_0, _TILE_1, _TILE_2, _TILE_3, 
		_TILE_4, _TILE_5, _TILE_6, _TILE_7, 
		_TILE_8, _TILE_9, _TILE_10, _TILE_11, 
		_TILE_12, _TILE_13, _TILE_14, _TILE_15, 
		_TILE_16, _TILE_17, _TILE_18, _TILE_19,
		_TILE_20, _TILE_21, _TILE_22, _TILE_23, 
        _TILE_24, _TILE_25
		};

#if !defined(_XL_NO_COLOR)
static const uint8_t color_code[NUMBER_OF_COLORS] = 
    {_XL_WHITE, _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE};

#endif

const char color_name[NUMBER_OF_COLORS][MAX_STRING_SIZE] = { 
                                "WHITE", 
                                "RED", 
                                "CYAN", 
                                "GREEN", 
                                "YELLOW", 
                                "BLUE", 
                                };


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
        for(j=0;j<NUMBER_OF_COLORS;++j)
        {
            _XL_CLEAR_SCREEN();
            
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            
            _XL_PRINT(0,0,"SET COLOR ");
            
            #if defined(_XL_NO_TEXT_COLOR)
                _XL_PRINT(XSize-1-14,1,"TEXT COLOR OFF");
            #else
                _XL_PRINT(XSize-1-13,1,"TEXT COLOR ON");
            #endif
            #if defined(_XL_NO_COLOR)
                _XL_PRINT(XSize-1-9,2,"COLOR OFF");
            #else
                _XL_PRINT(XSize-1-8, 2,"COLOR ON");
            #endif 
            
            _XL_PRINT((XSize-10)/2,3,"PRESS FIRE");

            _XL_SET_TEXT_COLOR(color_code[j]);
            
            _XL_PRINT(10,0, (char *) color_name[j]);
            
            _XL_WAIT_FOR_INPUT();

            _XL_PRINT((XSize-10)/2,YSize/2,"0123456789");
            
            _XL_WAIT_FOR_INPUT();
            
            _XL_PRINT((XSize-10)/2,YSize/2,"SOME TEXT ");
            
            _XL_WAIT_FOR_INPUT();

            _XL_SET_TEXT_COLOR(_XL_WHITE);
			
			_XL_PRINT((XSize-10)/2,YSize/2,"  TILES   ");
          
            for(i=0;i<_XL_NUMBER_OF_TILES;++i)
            {   
                _XL_DRAW(XSize/2-_XL_NUMBER_OF_TILES/2+i%XSize,YSize/2+2+i/XSize,tile[i],color_code[j]);   
            }       
            
            _XL_WAIT_FOR_INPUT();
        }
    }
    _XL_PRINT((XSize-11)/2, YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

