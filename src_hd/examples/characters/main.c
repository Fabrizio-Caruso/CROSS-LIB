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
#define NUMBER_OF_CHARS 26

#if XSize>=26
    #define X_SPACING 2
#else
    #define X_SPACING 1
#endif 

#if !defined(_XL_NO_COLOR)
static const uint8_t text_color[NUMBER_OF_COLORS] = {_XL_WHITE, _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE};
#endif

const char color_name[NUMBER_OF_COLORS][MAX_STRING_SIZE] = { 
                                _XL_W _XL_H _XL_I _XL_T _XL_E, 
                                _XL_R _XL_E _XL_D, 
                                _XL_C _XL_Y _XL_A _XL_N, 
                                _XL_G _XL_R _XL_E _XL_E _XL_N, 
                                _XL_Y _XL_E _XL_L _XL_L _XL_O _XL_W, 
                                _XL_B _XL_L _XL_U _XL_E, 
                                };

const char chars[NUMBER_OF_CHARS] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'};

#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3


int main(void)
{        
    uint8_t j;
    uint8_t i;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();


    for(j=0;j<NUMBER_OF_COLORS;++j)
    {
        _XL_CLEAR_SCREEN();
        
        _XL_SET_TEXT_COLOR(text_color[j]);

        _XL_PRINT(XSize-12,0,"01234567890");
        
        _XL_PRINT(COL_OFFSET,   1,"PRINTD");
        _XL_PRINTD(COL_OFFSET  ,2,5, 1234U);
        _XL_PRINTD(COL_OFFSET+5,2,5,56789U);
        
        _XL_PRINT(COL_OFFSET, 4, (char *) color_name[j]);
        
        _XL_PRINT(COL_OFFSET,YSize/3-1, " ABCDEFG HIJKLM");
        _XL_PRINT(COL_OFFSET,YSize/3+1, " NOPQRST UVWXYZ");

        for(i=0;i<NUMBER_OF_CHARS;++i)
        {
            _XL_CHAR(X_SPACING*(i%(XSize/2)),YSize-7+2*(i/(XSize/2)),chars[i]);
        }
        
        #if !defined(__EXIT_TO_OS)
        _XL_SLEEP(1);
        #endif

    }
    
    _XL_PRINT(COL_OFFSET,YSize-1, "END OF DEMO");

    _XL_REFRESH();

    #if !defined(__EXIT_TO_OS)
    while(1){};
    #endif
    
    return EXIT_SUCCESS;
}

