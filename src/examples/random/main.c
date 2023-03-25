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

uint8_t random_colors[] = { _XL_WHITE, _XL_RED, _XL_GREEN, _XL_YELLOW };

int main(void)
{        
    uint8_t i;
    uint8_t j;
    
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();

    for(j=0;j<9;++j)
    {
        _XL_CLEAR_SCREEN();        
        for(i=1;i<YSize-2;i+=2)
        {
            _XL_SET_TEXT_COLOR(random_colors[_XL_RAND()&3]);
            _XL_PRINTD(XSize/2-3,i,5,_XL_RAND());
        }
        _XL_WAIT_FOR_INPUT();
    }
    
    _XL_CLEAR_SCREEN(); 
    _XL_PRINT((XSize-11)/2,YSize/2,"END OF DEMO");

    while(1){};
    return EXIT_SUCCESS;
}

