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

#define NUMBER_OF_COLORS 6
#define MAX_STRING_SIZE 10


void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT((uint8_t) (((XSize) - (((uint8_t) strlen(Text))))/2), row, Text);	
}

#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW(((YSize)>>1), Text)



#if !defined(_XL_NO_TEXT_COLOR)
static const uint8_t text_color[NUMBER_OF_COLORS] = {_XL_WHITE, _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE};
#endif

const char color_name[NUMBER_OF_COLORS][MAX_STRING_SIZE] = { 
                                "WHITE", 
                                "RED", 
                                "CYAN", 
                                "GREEN", 
                                "YELLOW", 
                                "BLUE", 
                                };


char *small_letters = 
    _XL_a _XL_b _XL_c _XL_d _XL_e _XL_f _XL_g _XL_h _XL_i _XL_j _XL_k _XL_l _XL_m \
    _XL_n _XL_o _XL_p _XL_q _XL_r _XL_s _XL_t _XL_u _XL_v _XL_w _XL_x _XL_y _XL_z;


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


void small_pause(void)
{
    _XL_SLOW_DOWN(100U);
}


int main(void)
{        
    uint8_t i;
    uint8_t j;
    uint8_t k;
    
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_INPUT();

    for(k=0;k<3;++k)
    {
        for(j=0;j<NUMBER_OF_COLORS;++j)
        {
            _XL_CLEAR_SCREEN();
            #if !defined(_XL_NO_TEXT_COLOR)
            _XL_SET_TEXT_COLOR(text_color[j]);
            #endif
            
            print_text(XSize/2-6,YSize/2-5+0,"Aa Bb Cc Dd");
            print_text(XSize/2-6,YSize/2-5+1,"Ee Ff Gg Hh");
            print_text(XSize/2-6,YSize/2-5+2,"Ii Jj Kk Ll");
            print_text(XSize/2-6,YSize/2-5+3,"Mm Nn Oo Pp");
            print_text(XSize/2-6,YSize/2-5+4,"Qq Rr Ss Tt");
            print_text(XSize/2-6,YSize/2-5+5,"Uu Vv Ww Xx");
            print_text(XSize/2-6,YSize/2-5+6,"Yy Zz");
            print_text(XSize/2-6,YSize/2-5+8," 0 1 2 3 4");
            print_text(XSize/2-6,YSize/2-5+9," 5 6 7 8 9");
            
            _XL_SLEEP(1);
            
            _XL_WAIT_FOR_INPUT();

            _XL_CLEAR_SCREEN();
            
            #if !defined(_XL_NO_TEXT_COLOR)
            _XL_SET_TEXT_COLOR(text_color[j]);
            #endif
            
            _XL_PRINT(0,0, (char *) color_name[j]);
            
            #if defined(_XL_NO_TEXT_COLOR)
                _XL_PRINT(XSize-9,1,"COLOR OFF");
            #else
                _XL_PRINT(XSize-8,1,"COLOR ON");
            #endif
            
            PRINT_CENTERED_ON_ROW(5,"CENTERED");
            
            _XL_WAIT_FOR_INPUT();

            PRINT_CENTERED("0123456789");
            
            _XL_WAIT_FOR_INPUT();
            
            PRINT_CENTERED("SOME TEXT ");
            
            small_pause(); 
            
            _XL_WAIT_FOR_INPUT();

            _XL_CLEAR_SCREEN();
            
            for(i=YSize/6+3;i<YSize-2;i+=3)
            {   
                _XL_SET_TEXT_COLOR(text_color[j]);
                _XL_PRINT(0,0, (char *) color_name[j]);
            
                _XL_PRINT(0,2,"ON ROW");

                _XL_PRINTD(7,2,2,i);
                
                _XL_WAIT_FOR_INPUT();

                PRINT_CENTERED_ON_ROW(i,"0123456789");
                small_pause();
                
                _XL_WAIT_FOR_INPUT();
                
                PRINT_CENTERED_ON_ROW(i-1,"ABCDEFGHIJ");
                PRINT_CENTERED_ON_ROW(i,  "KLMNOPQRST");
                PRINT_CENTERED_ON_ROW(i+1,"UVWXYZ    ");      
                small_pause();
                _XL_WAIT_FOR_INPUT();

                PRINT_CENTERED_ON_ROW(i-1,"          ");
                PRINT_CENTERED_ON_ROW(i,  "          ");
                PRINT_CENTERED_ON_ROW(i+1,"          "); 

                PRINT_CENTERED_ON_ROW(i-1,_XL_a _XL_b _XL_c _XL_d _XL_e _XL_f _XL_g _XL_h _XL_i _XL_j);
                PRINT_CENTERED_ON_ROW(i,  _XL_k _XL_l _XL_m _XL_n _XL_o _XL_p _XL_q _XL_r _XL_s _XL_t);
                PRINT_CENTERED_ON_ROW(i+1,_XL_u _XL_v _XL_w _XL_x _XL_y _XL_z _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE);
                small_pause();
                
                _XL_WAIT_FOR_INPUT();
                
                _XL_CLEAR_SCREEN();
            }    

            print_text(XSize/2-6,YSize/2-5+0,"Aa Bb Cc Dd");
            print_text(XSize/2-6,YSize/2-5+1,"Ee Ff Gg Hh");
            print_text(XSize/2-6,YSize/2-5+2,"Ii Jj Kk Ll");
            print_text(XSize/2-6,YSize/2-5+3,"Mm Nn Oo Pp");
            print_text(XSize/2-6,YSize/2-5+4,"Qq Rr Ss Tt");
            print_text(XSize/2-6,YSize/2-5+5,"Uu Vv Ww Xx");
            print_text(XSize/2-6,YSize/2-5+6,"Yy Zz");
            print_text(XSize/2-6,YSize/2-5+8," 0 1 2 3 4");
            print_text(XSize/2-6,YSize/2-5+9," 5 6 7 8 9");
            
            _XL_WAIT_FOR_INPUT();

        }
    }
    PRINT_CENTERED_ON_ROW(YSize-5, "END OF DEMO");

    while(1){};
    
    return EXIT_SUCCESS;
}

