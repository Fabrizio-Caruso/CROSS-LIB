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

#if YSize<=15
    #define LINE_SKIP 1
#else
    #define LINE_SKIP 2
#endif    

int main(void)
{        
    uint8_t k;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();

    _XL_INIT_SOUND();

    for(k=0;k<3;++k)
    {

        _XL_CLEAR_SCREEN();
        
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT_CENTERED_ON_ROW(0, _XL_S _XL_O _XL_U _XL_N _XL_D);
        _XL_WAIT_FOR_INPUT();

        _XL_PRINT_CENTERED_ON_ROW(1+1*LINE_SKIP, _XL_P _XL_I _XL_N _XL_G);
        _XL_WAIT_FOR_INPUT();
        _XL_PING_SOUND();
        _XL_WAIT_FOR_INPUT();

        _XL_PRINT_CENTERED_ON_ROW(1+2*LINE_SKIP, _XL_T _XL_I _XL_C _XL_K);
        _XL_WAIT_FOR_INPUT();
        _XL_TOCK_SOUND();
        _XL_WAIT_FOR_INPUT();

        _XL_PRINT_CENTERED_ON_ROW(1+3*LINE_SKIP, _XL_T _XL_O _XL_C _XL_K);
        _XL_WAIT_FOR_INPUT();
        _XL_TICK_SOUND();
        _XL_WAIT_FOR_INPUT();
     
        _XL_PRINT_CENTERED_ON_ROW(1+4*LINE_SKIP, _XL_Z _XL_A _XL_P);
        _XL_WAIT_FOR_INPUT();
        _XL_ZAP_SOUND();
        _XL_WAIT_FOR_INPUT();
        
        _XL_PRINT_CENTERED_ON_ROW(1+5*LINE_SKIP, _XL_S _XL_H _XL_O _XL_O _XL_T);
        _XL_WAIT_FOR_INPUT();
        _XL_SHOOT_SOUND();
        _XL_WAIT_FOR_INPUT();

        _XL_PRINT_CENTERED_ON_ROW(1+6*LINE_SKIP, _XL_E _XL_X _XL_P _XL_L _XL_O _XL_S _XL_I _XL_O _XL_N);
        _XL_WAIT_FOR_INPUT();
        _XL_EXPLOSION_SOUND();
        _XL_WAIT_FOR_INPUT();

    }
    _XL_PRINT_CENTERED_ON_ROW(YSize-3, _XL_E _XL_N _XL_D _XL_SPACE _XL_O _XL_F _XL_SPACE _XL_D _XL_E _XL_M _XL_O);

    while(1){};
    
    return EXIT_SUCCESS;
}

