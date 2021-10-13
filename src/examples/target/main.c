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
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();

    _XL_CLEAR_SCREEN();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINT(0,   0,"TARGET INFORMATION");

    _XL_PRINT(0,   2,"XSIZE ");
    _XL_PRINTD(6,  2,3,XSize);

    _XL_PRINT(0,   4,"YSIZE ");
    _XL_PRINTD(6,  4,3,YSize);     
    
    _XL_PRINT(0,   6,"GRAPHICS ");
    #if !defined(_XL_NO_UDG)
        _XL_PRINT(9,6,"ON");
    #else
        _XL_PRINT(9,6,"OFF");
    #endif

    _XL_PRINT(0,   8,"COLOR ");
    #if !defined(_XL_NO_COLOR)
        _XL_PRINT(6,8,"ON");
    #else
        _XL_PRINT(6,8,"OFF");
    #endif

    _XL_PRINT(0,   10,"TEXT COLOR ");
    #if !defined(_XL_NO_TEXT_COLOR)
        _XL_PRINT(11,10,"ON");
    #else
        _XL_PRINT(11,10,"OFF");
    #endif

    _XL_PRINT(0,   12,"CONTROLS ");
    #if !defined(_XL_NO_JOYSTICK)
        _XL_PRINT(9,12,"JOY");
    #else
        _XL_PRINT(9,12,"KEYS");
    #endif 

    while(1){};
    
    return EXIT_SUCCESS;
}

