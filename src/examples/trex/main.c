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


#define X_DINO (XSize/4)
#define Y_DINO (YSize/2)

void draw_jump_dino(uint8_t height)
{
    // Initial tiles
    _XL_DRAW(X_DINO+1,Y_DINO-height,_TILE_0,_XL_WHITE);
    _XL_DRAW(X_DINO+1,Y_DINO+1-height,_TILE_3,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1-height,_TILE_2,_XL_WHITE);
}    

void delete_feet(uint8_t height)
{
    _XL_DELETE(X_DINO+1,Y_DINO+1-height);
    _XL_DELETE(X_DINO,Y_DINO+1-height); 
}

void draw_dino_feet_0(void)
{
    _XL_DRAW(X_DINO+1,Y_DINO+1,_TILE_3,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1,_TILE_4,_XL_WHITE);
}


void draw_dino_feet_1(void)
{
    _XL_DRAW(X_DINO+1,Y_DINO+1,_TILE_1,_XL_WHITE);
    _XL_DRAW(X_DINO,Y_DINO+1,_TILE_2,_XL_WHITE);
}


int main(void)
{        
    uint8_t i;
    uint8_t j;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();

    _XL_CLEAR_SCREEN();

    for(i=0;i<9;++i)
    {

        draw_jump_dino(0);
        
        _XL_WAIT_FOR_INPUT();
 
        for(j=0;j<99;++j)
        {
        
            delete_feet(0);
            
            draw_dino_feet_0();
            
            _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
            
            delete_feet(0);
            
            draw_dino_feet_1();
            
            _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);
        }
        
        delete_feet(0);
        
        draw_jump_dino(1);
        
        _XL_WAIT_FOR_INPUT();

        _XL_CLEAR_SCREEN();
        
        // _XL_DRAW(XSize/2+1,YSize/2,tiles[0],tile_color[j]);
        // _XL_DRAW(XSize/2+1,YSize/2+1,tiles[1],tile_color[j]);
        // _XL_DRAW(XSize/2,YSize/2+1,tiles[2],tile_color[j]);

        // _XL_WAIT_FOR_INPUT();
        
        // _XL_DRAW(XSize/2+1,YSize/2+1,tiles[3],tile_color[j]);
        // _XL_DRAW(XSize/2,YSize/2+1,tiles[4],tile_color[j]);  

        // _XL_WAIT_FOR_INPUT();
        
        // _XL_DRAW(XSize/2+1,YSize/2+1,tiles[3],tile_color[j]);
        // _XL_DRAW(XSize/2,YSize/2+1,tiles[2],tile_color[j]); 

        // _XL_WAIT_FOR_INPUT();
        
        // _XL_DELETE(XSize/2,YSize/2+1);
        // _XL_DELETE(XSize/2+1,YSize/2+1);
        
        // _XL_DRAW(XSize/2+1,YSize/2-1,tiles[5],tile_color[j]);
        // _XL_DRAW(XSize/2+1,YSize/2,tiles[6],tile_color[j]);
        // _XL_DRAW(XSize/2+1,YSize/2+1,tiles[7],tile_color[j]);

        // _XL_DRAW(XSize/2,YSize/2,tiles[8],tile_color[j]);
        // _XL_DRAW(XSize/2,YSize/2+1,tiles[9],tile_color[j]);
        
        // _XL_WAIT_FOR_INPUT();
        
        // _XL_DELETE(XSize/2,YSize/2+1);
        // _XL_DELETE(XSize/2+1,YSize/2+1);
                        
        // _XL_DRAW(XSize/2+1,YSize/2-1-1,tiles[10],tile_color[j]);
        // _XL_DRAW(XSize/2+1,YSize/2-1,tiles[11],tile_color[j]);
        // _XL_DRAW(XSize/2+1,YSize/2+1-1,tiles[12],tile_color[j]);

        // _XL_DRAW(XSize/2,YSize/2-1,tiles[13],tile_color[j]);
        // _XL_DRAW(XSize/2,YSize/2+1-1,tiles[14],tile_color[j]);   
        
        // _XL_WAIT_FOR_INPUT();
        
        // _XL_CLEAR_SCREEN();
    }

    _XL_PRINT(XSize/4,YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

