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

int main(void)
{        
    uint8_t counter;
    uint8_t input;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();

    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT_CENTERED_ON_ROW(YSize/4, "PRESS FIRE");
    _XL_WAIT_FOR_INPUT();
    
    _XL_CLEAR_SCREEN();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINT_CENTERED_ON_ROW(0, "INPUT");
    counter = 0;
    while(counter<200)
    {
        input = _XL_INPUT();
        
        if(_XL_FIRE(input))
        {
            _XL_PRINT_CENTERED("FIRE ");
            ++counter;
        }
        else if(_XL_LEFT(input))
        {
            _XL_PRINT_CENTERED("LEFT ");
            ++counter;
        }
        else if(_XL_RIGHT(input))
        {
            _XL_PRINT_CENTERED("RIGHT");
            ++counter;
        }
        else if(_XL_DOWN(input))
        {
            _XL_PRINT_CENTERED("DOWN ");
            ++counter;
        }
        else if(_XL_UP(input))
        {
            _XL_PRINT_CENTERED("UP   ");
            ++counter;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
    _XL_PRINT_CENTERED_ON_ROW(YSize-5, "END OF TEST");

    while(1){};
    
    return EXIT_SUCCESS;
}

