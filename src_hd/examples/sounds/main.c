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

#if YSize<=15
    #define LINE_SKIP 1
#else
    #define LINE_SKIP 2
#endif    


void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - (uint8_t) strlen(Text))>>1), row, Text);	
}

#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)



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
        PRINT_CENTERED_ON_ROW(0, "SOUND");
        _XL_WAIT_FOR_INPUT();
        
        #if defined(_XL_NO_SOUND)
            _XL_PRINT(XSize-1-9,YSize-1,"SOUND OFF");
        #else
            _XL_PRINT(XSize-1-8,YSize-1,"SOUND ON");
        #endif

        PRINT_CENTERED_ON_ROW(1+1*LINE_SKIP, "PING");
        _XL_WAIT_FOR_INPUT();
        _XL_PING_SOUND();
        _XL_WAIT_FOR_INPUT();

        PRINT_CENTERED_ON_ROW(1+2*LINE_SKIP, "TICK");
        _XL_WAIT_FOR_INPUT();
        _XL_TOCK_SOUND();
        _XL_WAIT_FOR_INPUT();

        PRINT_CENTERED_ON_ROW(1+3*LINE_SKIP, "TOCK");
        _XL_WAIT_FOR_INPUT();
        _XL_TICK_SOUND();
        _XL_WAIT_FOR_INPUT();
     
        PRINT_CENTERED_ON_ROW(1+4*LINE_SKIP, "ZAP");
        _XL_WAIT_FOR_INPUT();
        _XL_ZAP_SOUND();
        _XL_WAIT_FOR_INPUT();
        
        PRINT_CENTERED_ON_ROW(1+5*LINE_SKIP, "SHOOT");
        _XL_WAIT_FOR_INPUT();
        _XL_SHOOT_SOUND();
        _XL_WAIT_FOR_INPUT();

        PRINT_CENTERED_ON_ROW(1+6*LINE_SKIP, "EXPLOSION");
        _XL_WAIT_FOR_INPUT();
        _XL_EXPLOSION_SOUND();
        _XL_WAIT_FOR_INPUT();

    }
    PRINT_CENTERED_ON_ROW(YSize-3, "END OF TEST");

    while(1){};
    
    return EXIT_SUCCESS;
}

