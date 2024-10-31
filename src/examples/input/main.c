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

#define PREVIOUS_STATE_X (XSize/2-7)
#define CURRENT_STATE_X  (XSize/2+2)

#define STATE_Y ((YSize)/2-1)


void UPDATE_STATE(char *Text)
{
    _XL_PRINT(CURRENT_STATE_X,STATE_Y, Text);
    _XL_PRINT(PREVIOUS_STATE_X,STATE_Y, Text);
}

int main(void)
{        
    uint8_t counter;
    uint8_t input;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();

    _XL_CLEAR_SCREEN();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINT(XSize/2-6,0, "INPUT TEST");

    _XL_PRINT(CURRENT_STATE_X,STATE_Y-2, "CURRENT");
    _XL_PRINT(PREVIOUS_STATE_X-3,STATE_Y-2, "PREVIOUS");
    
    while(1)
    {
        input = _XL_INPUT();
        _XL_PRINT(CURRENT_STATE_X,STATE_Y, "     ");
        
        if(_XL_FIRE(input))
        {
            UPDATE_STATE("FIRE ");
            ++counter;
        }
        else if(_XL_LEFT(input))
        {
            UPDATE_STATE("LEFT ");
            ++counter;
        }
        else if(_XL_RIGHT(input))
        {
            UPDATE_STATE("RIGHT");
            ++counter;
        }
        else if(_XL_DOWN(input))
        {
            UPDATE_STATE("DOWN ");
            ++counter;
        }
        else if(_XL_UP(input))
        {
            UPDATE_STATE("UP   ");
            ++counter;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
    
    return EXIT_SUCCESS;
}

