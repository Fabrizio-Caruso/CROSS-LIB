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


uint8_t state;

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


#define JUMP 2


void handle_state_behavior(void)
{
    switch(state)
    {
        case 0:
             draw_jump_dino_0(0);

            delete_feet(0);
            
            draw_dino_feet_0();
        break;
        
        case 1:
            delete_feet(0);
            
            draw_dino_feet_1();
        break;
        
        case JUMP:
                delete_feet(0);
                
                draw_jump_dino_0(0+1);
                
        break;
        
        case JUMP+1:
        
                draw_jump_dino_1(0+1);
                delete_feet(1);
                
                draw_jump_dino_0(1+1);
                
        break;
        
        case JUMP+2:
                draw_jump_dino_1(1+1);

                delete_feet(2);
                
                draw_jump_dino_0(2+1);
                
        break;
        
        case JUMP+3:
                draw_jump_dino_1(2+1);

                delete_feet(3);
                
                draw_jump_dino_0(3+1);
                
        break;
        
        case JUMP+4:
                draw_jump_dino_1(3+1);

                delete_feet(4);
                
                draw_jump_dino_0(4+1);
                
        break;
        
        case JUMP+5:
            delete_feet(4);
            
            draw_jump_dino_2(4+1);
        break;
        
        case JUMP+6:
            delete_top(5);
            draw_jump_dino_0(5);
        break;
        
        case JUMP+7:
            delete_top(4);
            draw_jump_dino_1(4);
        break;
        
        case JUMP+8:
            delete_top(4);
            draw_jump_dino_0(4);
        break;
        
        case JUMP+9:
            delete_top(3);
            draw_jump_dino_1(3);
        break;
        
        case JUMP+10:
            delete_top(3);
            draw_jump_dino_0(3);
        break;
        
        case JUMP+11:
            delete_top(2);
            draw_jump_dino_1(2);
        break;
        
        case JUMP+12:
            delete_top(2);
            draw_jump_dino_0(2);
        break;
        
        case JUMP+13:
            delete_top(1);
            draw_jump_dino_1(1);
        break;
        
        case JUMP+14:
            delete_top(1);
            draw_jump_dino_0(1);
        break;
        
        case JUMP+15:
            delete_top(0);
            draw_jump_dino_1(0);
        break;

        
        case JUMP+16:
        

			delete_top(0);
			delete_top(1);
            
             draw_jump_dino_0(0);

            delete_feet(0);
            
            draw_dino_feet_0();
        break;

    }
}


void handle_state_transition(void)
{
    switch(state)
    {
        case 0:

            input = _XL_INPUT();
            
            if(_XL_FIRE(input))
            {
                state = JUMP;
            }
            else
            {
                state = 1;
            }
        break;
        
        case 1:
            input = _XL_INPUT();
            
            if(_XL_FIRE(input))
            {
                state = JUMP;
            }
            else
            {
                state = 0;
            }
        break;
        
        case JUMP:
        case JUMP+1:
        case JUMP+2:
        case JUMP+3:
        case JUMP+4:
        case JUMP+5:
        case JUMP+6:
        case JUMP+7:
        case JUMP+8:
        case JUMP+9:
        case JUMP+10:
        case JUMP+11:
        case JUMP+12:
        case JUMP+13:
        case JUMP+14:
        case JUMP+15:
            ++state;
        break;
        
        case JUMP+16:
            state=0;
        break;
    }
}



int main(void)
{        
    // uint8_t i;
    uint8_t j;
    // uint8_t k;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_SOUND();

    _XL_INIT_INPUT();

	_XL_CLEAR_SCREEN();

	for(j=0;j<XSize;++j)
	{
		_XL_DRAW(j,Y_DINO+2,TERRAIN,_XL_WHITE);
	}
    state = 0;
    
    while(1)
    {

    _XL_PRINTD(0,0,3,state);
    
    handle_state_behavior();
    handle_state_transition();
    
    _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);


    }

    return EXIT_SUCCESS;
}

/*
        draw_jump_dino_0(0);
         
        delete_feet(0);
        
        draw_dino_feet_0();
        
        _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);
        
        delete_feet(0);
        
        draw_dino_feet_1();
        
        _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);

        
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
            
			// _XL_WAIT_FOR_INPUT();
            for(j=4;j>0;--j)
            {
                delete_top(j+1);

                draw_jump_dino_0(j+1);
                
                _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);
                
                delete_top(j); 
                
                draw_jump_dino_1(j);
                
                _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);  
            }
			
            delete_top(0);
			delete_top(1);

            draw_jump_dino_2(1);
			_XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);

			delete_top(0);
			delete_top(1);
			

			draw_jump_dino_0(1);
            
            _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);
			delete_top(0);
			delete_top(1);
			
			draw_jump_dino_1(0);
            
            _XL_SLOW_DOWN(RUN_SLOW_DOWN*_XL_SLOW_DOWN_FACTOR);
			delete_top(0);
			delete_top(1);
        }

*/