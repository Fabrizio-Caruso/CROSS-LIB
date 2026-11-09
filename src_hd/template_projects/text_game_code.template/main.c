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
#define FINAL_LEVEL 4


uint16_t score;
uint8_t level;
uint8_t alive;
uint8_t input;


char *small_letters = 
    _XL_a _XL_b _XL_c _XL_d _XL_e _XL_f _XL_g _XL_h _XL_i _XL_j _XL_k _XL_l _XL_m \
    _XL_n _XL_o _XL_p _XL_q _XL_r _XL_s _XL_t _XL_u _XL_v _XL_w _XL_x _XL_y _XL_z;


uint8_t mystrlen(char *cstr)
{
	uint8_t i;
	while(cstr[i++]!='\0')
	{
	}
	return --i;
}

void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - (uint8_t) mystrlen(Text))>>1), row, Text);	
}

#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)


void print_text(uint8_t x, uint8_t y, const char *str)
{
    uint8_t i;
    char ch;
    
    i=0;
    
    while((ch=str[i]))
    {
        if((ch>='a')&&(ch<='z'))
        {
            _XL_CHAR(x+i,y,small_letters[ch-'a']);
        }
        else
        {
            _XL_CHAR(x+i,y,ch);
        }
        ++i;
    }
}

int main(void)
{        

    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();
    

    // MAIN END-LESS LOOP
    while(1)
    {
        score = 0;
        alive = 1;
        level = INITIAL_LEVEL;

        _XL_CLEAR_SCREEN();
            
        _XL_SET_TEXT_COLOR(_XL_RED);
        PRINT_CENTERED_ON_ROW(4, "_GAME_NAME_CAPITAL");
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        print_text(XSize/2-5,6, "by author");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        
        print_text(XSize/2-6,YSize-1, "Press FIRE");
        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();
        
        alive = 1; 
        // GAME LOOP
        while(alive && (level<FINAL_LEVEL+1))
        {

            _XL_CLEAR_SCREEN();
            
            print_text(0,0,"Level");
            _XL_PRINTD(7,0,1,level);
                  
            
            _XL_SLEEP(1);
            print_text(XSize/2-6,14, "Press FIRE");
            _XL_WAIT_FOR_INPUT();
            
            _XL_CLEAR_SCREEN();
            
            print_text(0,0, "Score");
            _XL_PRINTD(7,0,5,score);
            
            score+=100;
            
            _XL_WAIT_FOR_INPUT();
            
            print_text(0,0, "Score");
            _XL_PRINTD(7,0,5,score);
            
            print_text(XSize/2-6,14, "Press FIRE");
            _XL_WAIT_FOR_INPUT();
            
            ++level;

        }
    } // while(1) -> restart from level 1

    return EXIT_SUCCESS;
}

