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

#define MAX_1_DIGIT_NUMBER 9
#define MAX_2_DIGIT_NUMBER 99
#define MAX_3_DIGIT_NUMBER 999
#define MAX_4_DIGIT_NUMBER 5000
#define MAX_5_DIGIT_NUMBER 20000

int main(void)
{        
    uint16_t i;
    uint8_t j;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_INPUT();

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    for(j=0;j<3;++j)
    {
        _XL_CLEAR_SCREEN();
        
        for(i=0;i<=MAX_1_DIGIT_NUMBER;++i)
        {
            _XL_PRINTD(XSize/2-3,2,1,i);
        }
        for(i=0;i<=MAX_2_DIGIT_NUMBER;++i)
        {
            _XL_PRINTD(XSize/2-3,4,2,i);
        }
        for(i=0;i<=MAX_3_DIGIT_NUMBER;++i)
        {
            _XL_PRINTD(XSize/2-3,6,3,i);
        }      
        for(i=0;i<=MAX_4_DIGIT_NUMBER;i+=5)
        {
            _XL_PRINTD(XSize/2-3,8,4,i);
        }
        i=0;
        while(i<MAX_5_DIGIT_NUMBER)
        {
            i+=_XL_RAND()&0xFF;
            if(i>MAX_5_DIGIT_NUMBER)
            {
                i=MAX_5_DIGIT_NUMBER;
            }
            _XL_PRINTD(XSize/2-3,10,5,i);
        }
                
        _XL_WAIT_FOR_INPUT();
    }
    
    _XL_CLEAR_SCREEN();
    
    _XL_PRINT((XSize-11)/2,YSize/2,"END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

