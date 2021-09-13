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
// subxect to the following restrictions:

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

#define _GAME_NAME__STRING _XL_M _XL_Y _XL_SPACE _XL_G _XL_A _XL_M _XL_E
#define _BY__STRING _XL_B _XL_Y
#define _AUTHOR__STRING _XL_A _XL_U _XL_T _XL_H _XL_O _XL_R 

#define _PRESS_FIRE__STRING _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E

#define _LEVEL__STRING _XL_L _XL_E _XL_V _XL_E _XL_L
#define _LEVEL_COMPLETED__STRING _LEVEL__STRING _XL_SPACE _XL_C _XL_O _XL_M _XL_P _XL_L _XL_E _XL_T _XL_E _XL_D

#define _LIVES__STRING _XL_L _XL_I _XL_V _XL_E _XL_S

#define _YOU_DIED__STRING _XL_Y _XL_O _XL_U _XL_SPACE _XL_D _XL_I _XL_E _XL_D

#define _GAME_COMPLETED__STRING _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_C _XL_O _XL_M _XL_P _XL_E _XL_T _XL_E _XL_D
#define _GAME_OVER__STRING _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R

#define INITIAL_LEVEL 1
#define FINAL_LEVEL 8

uint16_t hiscore;
uint16_t score;
uint8_t level;
uint8_t alive;
uint8_t lives;
uint8_t input;
uint8_t level_completed;

int main(void)
{        

    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();
    
    hiscore = 0;


    while(1)
    {
        alive = 1;
        score = 0;
        level = INITIAL_LEVEL;
        lives = 3;

        _XL_CLEAR_SCREEN();
            
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT_CENTERED_ON_ROW(2, _GAME_NAME__STRING);
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT_CENTERED_ON_ROW(4, _BY__STRING _XL_SPACE _AUTHOR__STRING);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT_CENTERED_ON_ROW(14, _PRESS_FIRE__STRING);
        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();
        while(lives && (level<FINAL_LEVEL+1))
        {
            // (RE-)START LEVEL
            level_completed = 0;
            _XL_CLEAR_SCREEN();
            
            _XL_PRINT(0,0,_LEVEL__STRING);
            _XL_PRINTD(7,0,1,level);
            
            _XL_PRINT(0,2,_LIVES__STRING);
            _XL_PRINTD(7,2,1,lives);       
            
            _XL_WAIT_FOR_INPUT();
            
            // LEVEL LOOP
            while(!level_completed && alive)
            {
                uint8_t counter;

                _XL_PRINTD(0,4,3,input);
                _XL_PRINTD(0,6,3,counter++);
                input = _XL_INPUT();
                _XL_PRINTD(0,6,3,counter++);    
                
                if(_XL_DOWN(input))
                {
                    alive=0;
                    _XL_EXPLOSION_SOUND();
                }
                else if(_XL_FIRE(input))
                {
                    _XL_TOCK_SOUND();
                    ++score;
                    if(score>=level*10)
                    {
                        level_completed = 1;
                    }
                }
            }
            
            
            // ALIVE OR LEVEL_COMPLETED?
            if(alive)
            {
                ++level;
            }
            else
            {
                --lives;
                _XL_PRINT_CENTERED(_YOU_DIED__STRING);
                _XL_SLEEP(1);
            }
        } // (lives && (level<FINAL_LEVEL+1)) 
        if(lives) // Game completed
        {
            // GAME COMPLETED
            _XL_CLEAR_SCREEN();
            _XL_PRINT_CENTERED(_GAME_COMPLETED__STRING);
            _XL_SLEEP(1);
            _XL_WAIT_FOR_INPUT();
        }
        _XL_CLEAR_SCREEN();
        _XL_PRINT_CENTERED(_GAME_OVER__STRING);
        _XL_WAIT_FOR_INPUT();
    } // while(1) -> restart from level 1

    return EXIT_SUCCESS;
}

