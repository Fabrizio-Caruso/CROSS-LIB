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

#define COL_OFFSET ((XSize)/2-9)

#define ITERATIONS 1

void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - (uint8_t) strlen(Text))>>1), row, Text);	
}

#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)




int main(void)
{        
    uint16_t i;
    uint8_t j;
    uint8_t k;
    uint8_t m;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_INPUT();


    for(j=1;j<5;++j)
    {
        _XL_CLEAR_SCREEN();
        
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        
        _XL_PRINT(COL_OFFSET     ,1,"PAUSE");
        _XL_PRINTD(COL_OFFSET+7  ,1,1, j);
        _XL_PRINT(COL_OFFSET+10  ,1,"SECONDS");
        
        PRINT_CENTERED_ON_ROW(3,"PRESS FIRE");

        _XL_WAIT_FOR_INPUT();

        PRINT_CENTERED_ON_ROW(3,"          ");

        _XL_PRINT(COL_OFFSET     ,5,"PAUSING");

        _XL_SLEEP(j);
        
        _XL_PRINT(COL_OFFSET     ,YSize-3,"END OF PAUSE");

        PRINT_CENTERED_ON_ROW(YSize-1,"PRESS FIRE");
        
        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();
        
        i = j*_XL_SLOW_DOWN_FACTOR;

        _XL_PRINT(COL_OFFSET     ,1,"PAUSE");
        _XL_PRINTD(COL_OFFSET+7  ,1,5, i);
        _XL_PRINT(COL_OFFSET+14  ,1,"STEPS");
        
        PRINT_CENTERED_ON_ROW(3,"PRESS FIRE");
        
        _XL_WAIT_FOR_INPUT();
        
        PRINT_CENTERED_ON_ROW(3,"          ");
        
        _XL_PRINT(COL_OFFSET     ,5,"PAUSING");
        
        for(m=0;m<ITERATIONS;++m)
        {
            for(k=1+XSize/5;k<XSize-1-XSize/5;++k)
            {
                _XL_DRAW(k,7+m,_TILE_0,_XL_WHITE);
                _XL_SLOW_DOWN(i);
            }
        }
        
        _XL_PRINT(COL_OFFSET     ,YSize-3,"END OF PAUSE");
        
        PRINT_CENTERED_ON_ROW(YSize-1,"PRESS FIRE");

        _XL_WAIT_FOR_INPUT();

    }
    _XL_PRINT(COL_OFFSET,YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

