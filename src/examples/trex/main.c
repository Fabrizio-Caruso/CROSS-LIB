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

uint8_t input;


void draw_jump_dino_0(uint8_t height)
{
    // Initial tiles
    _XL_DRAW(X_DINO+1,Y_DINO-height,TOP_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,BOTTOM_DINO_0,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,TAIL_DINO_0,_XL_WHITE);
}    

void draw_jump_dino_1(uint8_t height)
{
    _XL_DRAW(X_DINO+1,Y_DINO-1-height,TOP_DINO_1,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO-height,MIDDLE_DINO_1,_XL_WHITE);    
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,BOTTOM_DINO_1,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-1-height,TAIL_DINO_1,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,FOOT_DINO_1,_XL_WHITE);    
}   

void draw_jump_dino_2(uint8_t height)
{
    _XL_DRAW(X_DINO+1,Y_DINO-1-height,TOP_DINO_2,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO-height,MIDDLE_DINO_2,_XL_WHITE);    
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,BOTTOM_DINO_2,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-1-height,TAIL_DINO_2,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,FOOT_DINO_2,_XL_WHITE);    
}  

void delete_feet(uint8_t height)
{
    _XL_DELETE(X_DINO+1,Y_DINO+1-height);
    _XL_DELETE(X_DINO,Y_DINO+1-height); 
}

void delete_top(uint8_t height)
{
    _XL_DELETE(X_DINO+1,Y_DINO-1-height);
    _XL_DELETE(X_DINO,Y_DINO-height);

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


#define RUN_SLOW_DOWN 3

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


        draw_jump_dino_0(0);
         
        for(j=0;j<2;++j)
        {
        
            for(k=0;k<6;++k)
            {
                delete_feet(0);
                
                draw_dino_feet_0();
                
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR>>j);
                
                delete_feet(0);
                
                draw_dino_feet_1();
                
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR>>j);
            }
        }
        
        input = _XL_INPUT();
        
        if(_XL_FIRE(input))
        {
            for(j=0;j<4;++j)
            {
                delete_feet(j);
                
                draw_jump_dino_0(j+1);
                
                _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);
                
                draw_jump_dino_1(j+1);
                
                _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);  
            }
                    
            delete_feet(j);
            
            draw_jump_dino_0(j+1);
        
            _XL_SLOW_DOWN((RUN_SLOW_DOWN+RUN_SLOW_DOWN/2)*_XL_SLOW_DOWN_FACTOR);
            
            delete_feet(j);
            
            draw_jump_dino_2(j+1);
            
            _XL_SLOW_DOWN(2*RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);
            
            for(j=4;j>0;--j)
            {
                delete_top(j+1);

                draw_jump_dino_0(j+1);
                
                _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);
                
                delete_top(j); 
                
                draw_jump_dino_1(j);
                
                _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);  
            }
        }
    }

    _XL_PRINT(XSize/4,YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

