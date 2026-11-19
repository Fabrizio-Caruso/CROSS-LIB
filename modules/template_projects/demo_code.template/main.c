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



#if ((YSize)+(Y_OFFSET)-1)>19
    #define MAX_TILES 19
#else
    #define MAX_TILES ((YSize)+(Y_OFFSET)-1)
#endif


#define NUMBER_OF_COLORS 6
#define MAX_STRING_SIZE 10


void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - strlen(Text))>>1), row, Text);	
}

#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)



 
const uint8_t tiles[] = {
		_TILE_0, _TILE_1, _TILE_2, _TILE_3, 
		_TILE_4, _TILE_5, _TILE_6, _TILE_7, 
		_TILE_8, _TILE_9, _TILE_10, _TILE_11, 
		_TILE_12, _TILE_13, _TILE_14, _TILE_15, 
		_TILE_16, _TILE_17, _TILE_18, _TILE_19,
		_TILE_20, _TILE_21, _TILE_22, _TILE_23, 
        _TILE_24, _TILE_25, _TILE_26
		};

#if !defined(_XL_NO_COLOR)
static const uint8_t tile_color[NUMBER_OF_COLORS] = {_XL_WHITE, _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE};
#endif


const char color_name[NUMBER_OF_COLORS][MAX_STRING_SIZE] = { 
                                "WHITE", 
                                "RED", 
                                "CYAN", 
                                "GREEN", 
                                "YELLOW", 
                                "BLUE", 
                                };


#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3

int main(void)
{        

    uint8_t i;
    uint8_t j;
    uint8_t k;
    uint8_t counter;
    uint8_t input;
    
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();

    _XL_INIT_SOUND();

    for(k=0;k<3;++k)
    {
        for(j=0;j<6;++j)
        {
            _XL_CLEAR_SCREEN();
            
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            
            _XL_PRINT(COL_OFFSET,   4, "PRINTD");
            _XL_PRINTD(COL_OFFSET  ,5,5, 1234U);
            _XL_PRINTD(COL_OFFSET+5,5,5,56789U);

            _XL_SET_TEXT_COLOR(tile_color[j]);
            
            _XL_PRINT(COL_OFFSET, 7, (char *) color_name[j]);
            
            _XL_PRINT(COL_OFFSET, 9, _XL_a _XL_b _XL_c _XL_d _XL_e _XL_f _XL_g _XL_h _XL_i _XL_j);
            _XL_PRINT(COL_OFFSET,10, _XL_k _XL_l _XL_m _XL_n _XL_o _XL_p _XL_q _XL_r _XL_s _XL_t);
            _XL_PRINT(COL_OFFSET,11, _XL_u _XL_v _XL_w _XL_x _XL_y _XL_z);
            _XL_PRINT(COL_OFFSET,13, _XL_A _XL_B _XL_C _XL_D _XL_E _XL_F _XL_G _XL_H _XL_I _XL_J);
            _XL_PRINT(COL_OFFSET,14, _XL_K _XL_L _XL_M _XL_N _XL_O _XL_P _XL_Q _XL_R _XL_S _XL_T);
            _XL_PRINT(COL_OFFSET,15, _XL_U _XL_V _XL_W _XL_X _XL_Y _XL_Z);
            
            _XL_WAIT_FOR_INPUT();

            _XL_CLEAR_SCREEN();
            
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            PRINT_CENTERED_ON_ROW(0, "SOUND");
            _XL_WAIT_FOR_INPUT();

            PRINT_CENTERED_ON_ROW(3, "PING");
            _XL_WAIT_FOR_INPUT();
            _XL_PING_SOUND();
            _XL_WAIT_FOR_INPUT();
   
            PRINT_CENTERED_ON_ROW(5, "TICK");
            _XL_WAIT_FOR_INPUT();
            _XL_TOCK_SOUND();
            _XL_WAIT_FOR_INPUT();

            PRINT_CENTERED_ON_ROW(7, "TOCK");
            _XL_WAIT_FOR_INPUT();
            _XL_TICK_SOUND();
            _XL_WAIT_FOR_INPUT();
         
            PRINT_CENTERED_ON_ROW(9, "ZAP");
            _XL_WAIT_FOR_INPUT();
            _XL_ZAP_SOUND();
            _XL_WAIT_FOR_INPUT();
            
            PRINT_CENTERED_ON_ROW(11, "SHOOT");
            _XL_WAIT_FOR_INPUT();
            _XL_SHOOT_SOUND();
            _XL_WAIT_FOR_INPUT();

            PRINT_CENTERED_ON_ROW(13, "EXPLOSITION");
            _XL_WAIT_FOR_INPUT();
            _XL_EXPLOSION_SOUND();
            _XL_WAIT_FOR_INPUT();
            
            _XL_CLEAR_SCREEN();
            
            for(i=0;i<_XL_NUMBER_OF_TILES;++i)
            {
                _XL_DRAW((i&7)*2+COL_OFFSET,(i/8)*2+ROW_OFFSET,tiles[i],tile_color[j]);
                _XL_TICK_SOUND();
                _XL_WAIT_FOR_INPUT();
            }
            
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(COL_OFFSET,YSize-5, "PRESS FIRE");
            _XL_WAIT_FOR_INPUT();
            
            _XL_CLEAR_SCREEN();
            
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            
            PRINT_CENTERED_ON_ROW(0, "INPUT");
            counter = 0;
            while(counter<200)
            {
                input = _XL_INPUT();
                
                if(_XL_FIRE(input))
                {
                    PRINT_CENTERED("FIRE ");
                    ++counter;
                }
                else if(_XL_LEFT(input))
                {
                    PRINT_CENTERED("LEFT ");
                    ++counter;
                }
                else if(_XL_RIGHT(input))
                {
                    PRINT_CENTERED("RIGHT");
                    ++counter;
                }
                else if(_XL_DOWN(input))
                {
                    PRINT_CENTERED("DOWN ");
                    ++counter;
                }
                else if(_XL_UP(input))
                {
                    PRINT_CENTERED("UP   ");
                    ++counter;
                }

                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
        }
    }
    _XL_PRINT(COL_OFFSET,YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

