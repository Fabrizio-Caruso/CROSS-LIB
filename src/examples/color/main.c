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

#define NUMBER_OF_COLORS 7
#define MAX_STRING_SIZE 10

const uint8_t tile[] = {
		_TILE_0, _TILE_1, _TILE_2, _TILE_3, 
		_TILE_4, _TILE_5, _TILE_6, _TILE_7, 
		_TILE_8, _TILE_9, _TILE_10, _TILE_11, 
		_TILE_12, _TILE_13, _TILE_14, _TILE_15, 
		_TILE_16, _TILE_17, _TILE_18, _TILE_19,
		_TILE_20, _TILE_21, _TILE_22, _TILE_23, 
		_TILE_24, _TILE_25, _TILE_26
		};

#if !defined(_XL_NO_COLOR)
static const uint8_t color_code[NUMBER_OF_COLORS] = 
    {_XL_WHITE, _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE, _XL_MAGENTA};

#endif


#if XSize>=38
const char color_name[NUMBER_OF_COLORS][MAX_STRING_SIZE] = { 
                                "WHITE   ", 
                                "RED     ", 
                                "CYAN    ", 
                                "GREEN   ", 
                                "YELLOW  ", 
                                "BLUE    ", 
                                "MAGENTA ",
                                };
    #define NAME_SIZE 7
#else
const char color_name[NUMBER_OF_COLORS][MAX_STRING_SIZE] = { 
                                "WH ", 
                                "RE ", 
                                "CY ", 
                                "GR ", 
                                "YE ", 
                                "BL ", 
                                "MA ",
                                };
    #define NAME_SIZE 2
#endif

#if YSize<=9
    #define SHORT_X_OFFSET 4
    #define SHOOT_Y_OFFSET 8
#elif YSize<=14

    #define SHORT_X_OFFSET 5
    #define SHOOT_Y_OFFSET 5
#else
    #define SHORT_X_OFFSET 0
    #define SHOOT_Y_OFFSET 0
#endif


#if _XL_NUMBER_OF_TILES>=XSize-NAME_SIZE
    #define DISPLAYED_TILES (XSize-NAME_SIZE-1)-SHORT_X_OFFSET
    #define OFFSET (NAME_SIZE+1)
#else
    #define DISPLAYED_TILES _XL_NUMBER_OF_TILES
    #define OFFSET (NAME_SIZE+1)
#endif




int main(void)
{        
    uint8_t i;
    uint8_t j;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();

    // for(;;)
    // {
        _XL_CLEAR_SCREEN();

        for(j=0;j<NUMBER_OF_COLORS;++j)
        {
            
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            
            // _XL_PRINT(0,0,"SET COLOR ");
            


            
            #if XSize>=16 && YSize>=15
                _XL_PRINT(XSize-1-1,0,"TX");
                _XL_PRINT(XSize-1-1,3,"CL");
                #if defined(_XL_NO_TEXT_COLOR)
                    _XL_PRINT(XSize-1-2,1,"OFF");
                #else
                    _XL_PRINT(XSize-1-1,1,"ON");
                #endif
                #if defined(_XL_NO_COLOR)
                    _XL_PRINT(XSize-1-2, 4,"OFF");
                #else
                    _XL_PRINT(XSize-1-1, 4,"ON");
                #endif 
            #endif
            _XL_SET_TEXT_COLOR(color_code[j]);
            
            _XL_PRINT(0,0+j, (char *) color_name[j]);
            _XL_PRINT(NAME_SIZE+1,0+j,"0123456789");
            
        }
        
        for(j=0;j<NUMBER_OF_COLORS;++j)
        {
        
            _XL_SET_TEXT_COLOR(color_code[j]);
            _XL_PRINT(0+SHORT_X_OFFSET,NUMBER_OF_COLORS+1+j-SHOOT_Y_OFFSET, (char *) color_name[j]);
            for(i=0;i<DISPLAYED_TILES;++i)
            {   
                _XL_DRAW(OFFSET+i+SHORT_X_OFFSET,NUMBER_OF_COLORS+1+j-SHOOT_Y_OFFSET,tile[i],color_code[j]);   

            }       
        }
        

    // }

    while(1){};
    
    return EXIT_SUCCESS;
}

