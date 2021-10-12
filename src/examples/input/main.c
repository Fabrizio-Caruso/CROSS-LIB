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



int main(void)
{        

    uint8_t counter;
    uint8_t input;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();

    _XL_INIT_SOUND();

    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT_CENTERED_ON_ROW(YSize/4, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
    _XL_WAIT_FOR_INPUT();
    
    _XL_CLEAR_SCREEN();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINT_CENTERED_ON_ROW(0, _XL_I _XL_N _XL_P _XL_U _XL_T);
    counter = 0;
    while(counter<200)
    {
        input = _XL_INPUT();
        
        if(_XL_FIRE(input))
        {
            _XL_PRINT_CENTERED(_XL_F _XL_I _XL_R _XL_E _XL_SPACE);
            ++counter;
        }
        else if(_XL_LEFT(input))
        {
            _XL_PRINT_CENTERED(_XL_L _XL_E _XL_F _XL_T _XL_SPACE);
            ++counter;
        }
        else if(_XL_RIGHT(input))
        {
            _XL_PRINT_CENTERED(_XL_R _XL_I _XL_G _XL_H _XL_T);
            ++counter;
        }
        else if(_XL_DOWN(input))
        {
            _XL_PRINT_CENTERED(_XL_D _XL_O _XL_W _XL_N _XL_SPACE);
            ++counter;
        }
        else if(_XL_UP(input))
        {
            _XL_PRINT_CENTERED(_XL_U _XL_P _XL_SPACE _XL_SPACE _XL_SPACE);
            ++counter;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
    _XL_PRINT_CENTERED_ON_ROW(YSize-5, _XL_E _XL_N _XL_D _XL_SPACE _XL_O _XL_F _XL_SPACE _XL_T _XL_E _XL_S _XL_T);

    while(1){};
    
    return EXIT_SUCCESS;
}

