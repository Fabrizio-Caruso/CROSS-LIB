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
		_TILE_0,  _TILE_1,  _TILE_2,  _TILE_3, 
		_TILE_4,  _TILE_5,    
        
        #if _XL_NUMBER_OF_TILES>6
                            _TILE_6,  _TILE_7, 
		_TILE_8,  _TILE_9,  _TILE_10, _TILE_11, 
		_TILE_12, _TILE_13, _TILE_14, _TILE_15, 
		_TILE_16, _TILE_17, _TILE_18, _TILE_19,
		_TILE_20, _TILE_21, _TILE_22, _TILE_23, 
        _TILE_24, _TILE_25, _TILE_26,
        #if _XL_NUMBER_OF_TILES>27
                                      _TILE_27, 
        _TILE_28, _TILE_29, _TILE_30, _TILE_31,
 
        #if _XL_NUMBER_OF_TILES>32
        _TILE_32, _TILE_33, _TILE_34, _TILE_35, 
		_TILE_36, _TILE_37, _TILE_38, _TILE_39,
		_TILE_40, _TILE_41, _TILE_42, _TILE_43,
		_TILE_44, _TILE_45, _TILE_46, _TILE_47, 
		_TILE_48, _TILE_49, _TILE_50, _TILE_51, 
		_TILE_52, _TILE_53, _TILE_54, _TILE_55, 
		_TILE_56, _TILE_57, _TILE_58, _TILE_59,
		_TILE_60, _TILE_61, _TILE_62, _TILE_63,
		_TILE_64, _TILE_65, _TILE_66, _TILE_67, 
		_TILE_68, _TILE_69, _TILE_70, _TILE_71, 
		_TILE_72, _TILE_73, _TILE_74, _TILE_75, 
		_TILE_76, _TILE_77, _TILE_78, _TILE_79,
		_TILE_80, _TILE_81, _TILE_82, _TILE_83,
        _TILE_84, _TILE_85, _TILE_86, _TILE_87, 
		_TILE_88, _TILE_89, _TILE_90,
        #endif
        #endif
        #endif
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


#if YSize<=16
    #define COL_OFFSET 0
#else
    #define COL_OFFSET 2
#endif

#if YSize<=16
    #define ROW_OFFSET 1
    #define CHAR_SKIP  1
#else
    
    #define ROW_OFFSET 2
    #if XSize<=16
        #define CHAR_SKIP 1
    #else
        #define CHAR_SKIP 2
    #endif    
#endif


#if YSize<=25
    #define LINE_SKIP 1
#else
    #define LINE_SKIP 2
#endif    

#if defined(_XL_NO_JOYSTICK)
    #define _PRESS "PRESS A KEY"
#else
    #define _PRESS "PRESS FIRE"
#endif

// #define WAIT_FOR_EACH_TILE


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
            
            #if !defined(_XL_NO_COLOR)
            _XL_SET_TEXT_COLOR(tile_color[j]);
            #endif
            
            _XL_PRINT(0, 0, (char *) color_name[j]);
            
            // {
                // for(i=0;i<254;++i)
                // {
                    // POKE(0x0c00+i,i);
                    // POKE(0x0800+i,127);
                // }
                // _XL_WAIT_FOR_INPUT();
                // _XL_CLEAR_SCREEN();
            // }
            
            #if YSize>=20 && XSize>=16
            _XL_PRINT(0,YSize-3,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            _XL_PRINT(0,YSize-2,"0123456789");
            #endif

            #if YSize>=16
            _XL_SET_TEXT_COLOR(FIRST_COLOR);
            _XL_PRINT(COL_OFFSET,YSize-1, _PRESS);
            #endif

            #if YSize<=16
            for(i=0;i<_XL_NUMBER_OF_TILES;++i)
            {
                _XL_DRAW((i&15)*CHAR_SKIP+COL_OFFSET,(i/16)*LINE_SKIP+ROW_OFFSET,tiles[i],tile_color[j]);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                #if defined(WAIT_FOR_EACH_TILE)
                    _XL_WAIT_FOR_INPUT();
                #endif
            }
            #else
            for(i=0;i<_XL_NUMBER_OF_TILES;++i)
            {
                _XL_DRAW((i&7)*CHAR_SKIP+COL_OFFSET,(i/8)*LINE_SKIP+ROW_OFFSET,tiles[i],tile_color[j]);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                #if defined(WAIT_FOR_EACH_TILE)
                    _XL_WAIT_FOR_INPUT();
                #endif

            }
            #endif
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            
            _XL_REFRESH();
            _XL_WAIT_FOR_INPUT();
        }
    }
    _XL_PRINT(COL_OFFSET,YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

