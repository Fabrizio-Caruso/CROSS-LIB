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

#include "images.h"


#define X_DINO (XSize/8)
#define Y_DINO (YSize/2)

void draw_jump_dino(uint8_t height)
{
    // Initial tiles
    _XL_DRAW(X_DINO+1,Y_DINO-height,TOP_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,BOTTOM_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,TAIL_DINO_0,_XL_WHITE);
}    

void delete_feet(uint8_t height)
{
    _XL_DELETE(X_DINO+1,Y_DINO+1-height);
    _XL_DELETE(X_DINO,Y_DINO+1-height); 
}

void draw_dino_feet_0(void)
{
    _XL_DRAW(X_DINO+1,Y_DINO+1,BOTTOM_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1,TAIL_RAISED_DINO,_XL_WHITE);
}


void draw_dino_feet_1(void)
{
    _XL_DRAW(X_DINO+1,Y_DINO+1,BOTTOM_RAISED_DINO,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1,TAIL_DINO_0,_XL_WHITE);
}


int main(void)
{        
    uint8_t i;
    uint8_t j;
    uint8_t k;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();

    for(i=0;i<9;++i)
    {

        _XL_CLEAR_SCREEN();

        for(j=0;j<XSize;++j)
        {
            _XL_DRAW(j,Y_DINO+2,TERRAIN,_XL_WHITE);
        }


        draw_jump_dino(0);
        
        _XL_WAIT_FOR_INPUT();
 
        for(j=0;j<3;++j)
        {
        
            for(k=0;k<20;++k)
            {
                delete_feet(0);
                
                draw_dino_feet_0();
                
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR>>j);
                
                delete_feet(0);
                
                draw_dino_feet_1();
                
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR>>j);
            }
        }
        
        for(j=0;j<5;++j)
        {
            delete_feet(j);
            
            draw_jump_dino(j+1);
            
            _XL_SLOW_DOWN(4*_XL_SLOW_DOWN_FACTOR);
        }
        
        _XL_WAIT_FOR_INPUT();
     
    }

    _XL_PRINT(XSize/4,YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

