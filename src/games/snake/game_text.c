
#include "cross_lib.h"
#include "game_text.h"

extern uint8_t energy;
extern uint8_t remaining_apples;
extern uint8_t lives;
extern uint16_t points;

extern Image BODY_IMAGE;
extern Image HORIZONTAL_HEAD_IMAGE;
extern Image APPLE_IMAGE;


#if !defined(SIMPLE_TITLE)

char strings[NUMBER_OF_STRINGS][MAX_STRING_SIZE] = 
{
    _XL_M _XL_i _XL_n _XL_e,
    _XL_S _XL_e _XL_c _XL_r _XL_e _XL_t,
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
        _XLIB_DRAW(XSize/4+i,YSize/8+5,&BODY_IMAGE);
        
    }
    _XLIB_DRAW(XSize/4+XSize/2-2,YSize/8+5,&HORIZONTAL_HEAD_IMAGE);
}


void DISPLAY_ENERGY(void)
{
   if(energy<RED_ENERGY_THRESHOLD)
   {
       SET_TEXT_COLOR(COLOR_RED);
   }
   else
   {
       SET_TEXT_COLOR(COLOR_WHITE);
   }
   PRINTD(XSize/2-1,YSize-1,2,energy); 
}

void PRESS_KEY(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    printCenteredMessageOnRow(YSize-2, _PRESS_FIRE_STRING);
    WAIT_PRESS();
}

void DISPLAY_POINTS(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINTD(2,0,5,points);
}

void DISPLAY_REMAINING_APPLES_COUNT(void)
{
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINTD(9,0,2,remaining_apples);
}

void DISPLAY_LIVES(void)
{
    PRINTD(XSize-1,0,1,lives);
}

