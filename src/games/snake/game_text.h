#ifndef _GAME_TEXT_H
#define _GAME_TEXT_H

#define MAX_ENERGY 20

#if XSize<22
    #define HISCORE_OFFSET 2
#else
    #define HISCORE_OFFSET 0
#endif



#define _NO_ENERGY_STRING _XL_N _XL_O _XL_SPACE _XL_E _XL_N _XL_E _XL_R _XL_G _XL_Y
#define _CLEARED_STRING _XL_C _XL_L _XL_E _XL_A _XL_R _XL_E _XL_D
#define _BONUS_STRING _XL_B _XL_O _XL_N _XL_U _XL_S
#define _THE_END_STRING _XL_T _XL_H _XL_E _XL_SPACE _XL_E _XL_N _XL_D
#define _GAME_OVER_STRING _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R
#define _SECRET_STRING _XL_S _XL_E _XL_C _XL_R _XL_E _XL_T
#define _LEVEL_STRING _XL_L _XL_E _XL_V _XL_E _XL_L
#define _CROSS_SNAKE_STRING _XL_C _XL_R _XL_O _XL_S _XL_S _XL_SPACE _XL_S _XL_N _XL_A _XL_K _XL_E
#define _Fabrizio_Caruso_STRING _XL_F _XL_a _XL_b _XL_r _XL_i _XL_z _XL_i _XL_o _XL_SPACE _XL_C _XL_a _XL_r _XL_u _XL_s _XL_o
#define _PRESS_FIRE_STRING _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E

#if !defined(NO_EXTRA_TITLE)
    #define extra_title() \
    { \
        uint8_t i; \
        show_intro_snake(); \
        _XLIB_DRAW_TILE(XSize/4+XSize/2,YSize/8+5,APPLE_TILE,_XL_RED); \
        for(i=0;i<NUMBER_OF_STRINGS;++i) \
        { \
            _XLIB_DRAW_TILE(XSize/2-6,YSize/8+8+2*i, images[NUMBER_OF_STRINGS-i], image_colors[NUMBER_OF_STRINGS-i]); \
            SET_TEXT_COLOR(_XL_WHITE); \
            PRINT(XSize/2-4,YSize/8+8+2*i, strings[NUMBER_OF_STRINGS-1-i] ); \
        } \
    }
#else
    #define extra_title() \
    { \
        show_intro_snake(); \
        _XLIB_DRAW_TILE(XSize/4+XSize/2,YSize/8+5,APPLE_TILE,_XL_RED); \
    }
#endif

#define title() \
    CLEAR_SCREEN(); \
    _XLIB_DRAW_TILE(XSize/2-3,0,HI_TEXT_TILE,_XL_RED); \
    SET_TEXT_COLOR(_XL_WHITE); \
    PRINTD(XSize/2-2,0,5,record); \
    SET_TEXT_COLOR(_XL_RED); \
    printCenteredMessageOnRow(YSize/8, _CROSS_SNAKE_STRING); \
    SET_TEXT_COLOR(_XL_WHITE); \
    printCenteredMessageOnRow(YSize/8+2, _Fabrizio_Caruso_STRING); \
    extra_title(); \
    PRESS_KEY();
 


#define RED_ENERGY_THRESHOLD ((MAX_ENERGY)/2)


void DISPLAY_ENERGY(void);

void PRESS_KEY(void);

void DISPLAY_POINTS(void);

void DISPLAY_REMAINING_APPLES_COUNT(void);

void DISPLAY_LIVES(void);

void show_intro_snake(void);

#endif // _GAME_TEXT_H

