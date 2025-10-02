
#include "cross_lib.h"
#include "game_text.h"

#include "variables.h"

#include "init_images.h"



void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - strlen(Text))>>1), row, Text);	
}



#if !defined(NO_DISPLAY_RINGS) 
void DISPLAY_RINGS(void)
{
    _XL_DRAW(ACHIEVEMENTS_X_OFFSET+3, ACHIEVEMENTS_Y_OFFSET+3, RING_TILE, _XL_WHITE); 
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(ACHIEVEMENTS_X_OFFSET+5,ACHIEVEMENTS_Y_OFFSET+3,3,rings);
}
#endif

#if !defined(NO_EXTRA_TITLE)
const char strings[NUMBER_OF_STRINGS][MAX_STRING_SIZE] = 
{
    _XL_M _XL_i _XL_n _XL_e,
    _XL_E _XL_n _XL_e _XL_r _XL_g _XL_y,
    _XL_B _XL_o _XL_n _XL_u _XL_s,
    _XL_P _XL_o _XL_i _XL_n _XL_t _XL_s,
    _XL_A _XL_p _XL_p _XL_l _XL_e,
};
#endif

void show_intro_snake(void)
{
    for(i=0;i<XSize/2-2;++i)
    {
        _XL_DRAW(XSize/4+i,YSize/8+SNAKE_LINE_OFFSET,BODY_TILE, _XL_GREEN);
        
    }
    _XL_DRAW(XSize/4+XSize/2-2,YSize/8+SNAKE_LINE_OFFSET,HORIZONTAL_HEAD_TILE, _XL_GREEN);

}


void DISPLAY_ENERGY(void)
{
   if(energy<RED_ENERGY_THRESHOLD)
   {
       _XL_SET_TEXT_COLOR(_XL_RED);
   }
   else
   {
       _XL_SET_TEXT_COLOR(_XL_WHITE);
   }
   _XL_PRINTD(XSize/2-1,YSize-1,2,energy); 
}

void PRESS_KEY(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    PRINT_CENTERED_ON_ROW(YSize-2, _PRESS_FIRE_STRING);
    _XL_WAIT_FOR_INPUT();
}


#if defined(_XL_NO_JOYSTICK)
    #define CONTROLS_STRING _XL_U _XL_S _XL_E _XL_SPACE _XL_I _XL_J _XL_K _XL_L _XL_SPACE _XL_S _XL_P _XL_A _XL_C _XL_E
	#define CONTROLS_LEN 14
#else
    #define CONTROLS_STRING _XL_U _XL_S _XL_E _XL_SPACE _XL_J _XL_O _XL_Y _XL_S _XL_T _XL_I _XL_C _XL_K
	#define CONTROLS_LEN 12
#endif

#define CONTROLS_Y (YSize-2)

#if defined(NO_CONTROL_INSTRUCTIONS) && XSize>=14

    #define CONTROL_INSTRUCTIONS()
#else
    void CONTROL_INSTRUCTIONS(void)
        {
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            PRINT_CENTERED_ON_ROW(CONTROLS_Y, CONTROLS_STRING);
            _XL_WAIT_FOR_INPUT();
        }
#endif

void DISPLAY_POINTS(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(2,0,5,points);
}

void DISPLAY_REMAINING_APPLES_COUNT(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(9,0,2,remaining_apples);
}

void DISPLAY_LIVES(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize-1,0,1,lives);
}

