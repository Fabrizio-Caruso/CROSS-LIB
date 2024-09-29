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
    uint8_t i;
        
    _XL_INIT_GRAPHICS();

    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    _XL_PRINT((XSize-11)/2,YSize,"HELLO WORLD");

    for(i=0;i<XSize;++i)
    {
        _XL_DRAW(i,0,_TILE_0,_XL_WHITE);
        _XL_DRAW(i,YSize-1,_TILE_0,_XL_WHITE);
    }
    for(i=0;i<YSize;++i)
    {
        _XL_DRAW(0,i,_TILE_0,_XL_WHITE);
        _XL_DRAW(XSize-1,i,_TILE_0,_XL_WHITE);
    }    

    _XL_PRINT(XSize-4-2,0,"HI");
    _XL_PRINTD(XSize-4,0,4,1234);

    while(1){};
    
    return EXIT_SUCCESS;
}

