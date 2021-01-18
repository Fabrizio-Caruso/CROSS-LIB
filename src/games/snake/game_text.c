
#include "cross_lib.h"
#include "game_text.h"

#include "variables.h"

#include "init_images.h"

#if !defined(NO_EXTRA_TITLE)

char strings[NUMBER_OF_STRINGS][MAX_STRING_SIZE] = 
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
    uint8_t i;
    
    for(i=0;i<XSize/2-2;++i)
    {
        _XL_DRAW(XSize/4+i,YSize/8+5,BODY_TILE, _XL_GREEN);
        
    }
    _XL_DRAW(XSize/4+XSize/2-2,YSize/8+5,HORIZONTAL_HEAD_TILE, _XL_GREEN);

}


void DISPLAY_ENERGY(void)
{
   if(energy<RED_ENERGY_THRESHOLD)
   {
       SET_TEXT_COLOR(_XL_RED);
   }
   else
   {
       SET_TEXT_COLOR(_XL_WHITE);
   }
   _XL_PRINTD(XSize/2-1,YSize-1,2,energy); 
}

void PRESS_KEY(void)
{
    SET_TEXT_COLOR(_XL_WHITE);
    printCenteredMessageOnRow(YSize-2, _PRESS_FIRE_STRING);
    _XL_WAIT_FOR_INPUT();
}

void DISPLAY_POINTS(void)
{
    SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(2,0,5,points);
}

void DISPLAY_REMAINING_APPLES_COUNT(void)
{
    SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(9,0,2,remaining_apples);
}

void DISPLAY_LIVES(void)
{
    SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize-1,0,1,lives);
}

