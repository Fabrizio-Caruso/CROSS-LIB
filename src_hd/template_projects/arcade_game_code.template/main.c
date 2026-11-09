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

#include "cross_lib.h"

#define INITIAL_LEVEL 1
#define FINAL_LEVEL 8

uint16_t hiscore;
uint16_t score;
uint8_t level;
uint8_t alive;
uint8_t lives;
uint8_t input;
uint8_t level_completed;
uint8_t counter;


uint8_t x;
uint8_t y;

void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - strlen(Text))>>1), row, Text);	
}

#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)

void display_xy(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(0,YSize-1,2,x);
    _XL_PRINTD(4,YSize-1,2,y);
}

int main(void)
{        

    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();
    
    hiscore = 0;

    // MAIN END-LESS LOOP
    while(1)
    {
        score = 0;
        level = INITIAL_LEVEL;
        lives = 3;

        _XL_CLEAR_SCREEN();
            
        _XL_SET_TEXT_COLOR(_XL_RED);
        PRINT_CENTERED_ON_ROW(4, "_GAME_NAME_CAPITAL");
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        PRINT_CENTERED_ON_ROW(6, "BY AUTHOR");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        
        PRINT_CENTERED_ON_ROW(0, "HISCORE");
        _XL_PRINTD(XSize/2-3,1,5,hiscore);
        
        PRINT_CENTERED_ON_ROW(YSize-1, "PRESS FIRE");
        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();
        
        // GAME LOOP
        while(lives && (level<FINAL_LEVEL+1))
        {
            x = XSize/2;
            y = YSize/2;
            // (RE-)START LEVEL
            level_completed = 0;
            alive = 1;
            counter = 0;
            
            _XL_CLEAR_SCREEN();
            
            _XL_PRINT(0,0,"LEVEL");
            _XL_PRINTD(7,0,1,level);
            
            _XL_PRINT(0,2,"LIVES");
            _XL_PRINTD(7,2,1,lives);       
            
            _XL_SLEEP(1);
            PRINT_CENTERED_ON_ROW(14, "PRESS FIRE");
            _XL_WAIT_FOR_INPUT();
            
            _XL_CLEAR_SCREEN();
            
            _XL_PRINT(0,0, "SCORE");
            _XL_PRINTD(7,0,5,score);
            _XL_PRINTD(XSize-6,YSize-1,5,0);
            display_xy();

            // LEVEL LOOP
            while(!level_completed && alive)
            {
                input = _XL_INPUT();
                
                if(x<1 || x> XSize-2 || y<2 || y>YSize-3)
                {
                    alive=0;
                    _XL_EXPLOSION_SOUND();                    
                }
                _XL_DRAW(x,y,_TILE_0, _XL_WHITE);
                _XL_SLEEP(1);

                if(_XL_DOWN(input))
                {
                    _XL_DELETE(x,y);
                    ++y;
                    display_xy();
                }
                else if (_XL_RIGHT(input))
                {
                    _XL_DELETE(x,y);
                    ++x;
                    display_xy();
                } 
                else if (_XL_LEFT(input))
                {
                    _XL_DELETE(x,y);
                    --x;
                    display_xy();
                } 
                else if (_XL_UP(input))
                {
                    _XL_DELETE(x,y);
                    --y;
                    display_xy();
                }                 
                else if(_XL_FIRE(input))
                {
                    _XL_TOCK_SOUND();
                    ++counter;
                    score+=50;
                    _XL_PRINTD(7,0,5,score);
                    _XL_PRINTD(XSize-6,YSize-1,5,counter);
                }
                if(counter>=100)
                {
                    level_completed = 1;
                    _XL_ZAP_SOUND();
                }
            }
                        
            // ALIVE OR LEVEL_COMPLETED?
            if(alive)
            {
                ++level;
                PRINT_CENTERED("LEVEL COMPLETED");
            }
            else
            {
                --lives;
                PRINT_CENTERED("YOU DIED");
                _XL_SLEEP(1);
            }
        } // (lives && (level<FINAL_LEVEL+1)) 
        if(lives) // Game completed
        {
            // GAME COMPLETED
            _XL_CLEAR_SCREEN();
            PRINT_CENTERED("GAME COMPLETED");
            _XL_SLEEP(1);
            _XL_WAIT_FOR_INPUT();
        }
        _XL_CLEAR_SCREEN();
        PRINT_CENTERED("GAME OVER");
        _XL_WAIT_FOR_INPUT();
        if(score>hiscore) 
        {
            hiscore = score;
        }
    } // while(1) -> restart from level 1

    return EXIT_SUCCESS;
}

