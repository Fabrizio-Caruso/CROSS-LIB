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
    uint8_t k;
    uint8_t i;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();


    for(k=0;k<3;++k)
    {
        for(j=0;j<NUMBER_OF_COLORS;++j)
        {
            _XL_CLEAR_SCREEN();
            
            _XL_SET_TEXT_COLOR(text_color[j]);

            _XL_PRINT(XSize-12,0,"01234567890");
            
            _XL_PRINT(COL_OFFSET,   1,"PRINTD");
            _XL_PRINTD(COL_OFFSET  ,2,5, 1234U);
            _XL_PRINTD(COL_OFFSET+5,2,5,56789U);
            
            _XL_PRINT(COL_OFFSET, 4, (char *) color_name[j]);
            
            _XL_PRINT(COL_OFFSET, 6, _XL_a _XL_b _XL_c _XL_d _XL_e _XL_f _XL_g _XL_h _XL_i _XL_j);
            _XL_PRINT(COL_OFFSET, 7, _XL_k _XL_l _XL_m _XL_n _XL_o _XL_p _XL_q _XL_r _XL_s _XL_t);
            _XL_PRINT(COL_OFFSET, 8, _XL_u _XL_v _XL_w _XL_x _XL_y _XL_z);
            _XL_PRINT(COL_OFFSET,10, _XL_A _XL_B _XL_C _XL_D _XL_E _XL_F _XL_G _XL_H _XL_I _XL_J);
            _XL_PRINT(COL_OFFSET,11, _XL_K _XL_L _XL_M _XL_N _XL_O _XL_P _XL_Q _XL_R _XL_S _XL_T);
            _XL_PRINT(COL_OFFSET,12, _XL_U _XL_V _XL_W _XL_X _XL_Y _XL_Z);

            _XL_WAIT_FOR_INPUT();
            
            _XL_CLEAR_SCREEN();

            _XL_PRINT(COL_OFFSET,YSize/2-1, " ABCDEFG HIJKLM");
            _XL_PRINT(COL_OFFSET,YSize/2+1, " NOPQRST UVWXYZ");
            
            _XL_WAIT_FOR_INPUT();
            
            _XL_CLEAR_SCREEN();
            
            
            for(i=0;i<NUMBER_OF_CHARS;++i)
            {
                _XL_CHAR(2*(i%(XSize/2)),YSize/3+2*(i/(XSize/2)),chars[i]);
            }
            
            _XL_WAIT_FOR_INPUT();

        }
    }
    _XL_PRINT(COL_OFFSET,YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

