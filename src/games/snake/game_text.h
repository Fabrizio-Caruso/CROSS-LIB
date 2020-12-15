#ifndef _GAME_TEXT_H
#define _GAME_TEXT_H

#define MAX_ENERGY 20

#if XSize<22
    #define HISCORE_OFFSET 2
#else
    #define HISCORE_OFFSET 0
#endif

#define NUMBER_OF_STRINGS 5

#define MAX_STRING_SIZE (6+1)


#define __NO_ENERGY__STRING _XL_SPACE _XL_N _XL_O _XL_SPACE _XL_E _XL_N _XL_E _XL_R _XL_G _XL_Y _XL_SPACE
#define __LEVEL_CLEARED__STRING _XL_SPACE _XL_L _XL_E _XL_V _XL_E _XL_L _XL_SPACE _XL_C _XL_L _XL_E _XL_A _XL_R _XL_E _XL_D _XL_SPACE
#define __BONUS__STRING _XL_SPACE _XL_B _XL_O _XL_N _XL_U _XL_S _XL_SPACE
#define __THE_END__STRING _XL_SPACE _XL_T _XL_H _XL_E _XL_SPACE _XL_E _XL_N _XL_D _XL_SPACE
#define __GAME_OVER__STRING _XL_SPACE _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R _XL_SPACE

#if !defined(NO_EXTRA_TITLE)

extern char strings[NUMBER_OF_STRINGS][MAX_STRING_SIZE];

#define extra_title() \
{ \
    uint8_t i; \
    show_intro_snake(); \
    _XLIB_DRAW(XSize/4+XSize/2,YSize/8+5,&APPLE_IMAGE); \
    for(i=0;i<NUMBER_OF_STRINGS;++i) \
    { \
        _XLIB_DRAW(XSize/2-6,YSize/8+8+2*i, images[NUMBER_OF_STRINGS-i]); \
        SET_TEXT_COLOR(COLOR_WHITE); \
        PRINT(XSize/2-4,YSize/8+8+2*i, strings[NUMBER_OF_STRINGS-1-i] ); \
    } \
}
#else
    #define extra_title()
#endif

#define title() \
    _XLIB_DRAW(XSize/2-3,0,&HI_TEXT_IMAGE); \
    SET_TEXT_COLOR(COLOR_WHITE); \
    PRINTD(XSize/2-2,0,5,record); \
    SET_TEXT_COLOR(COLOR_RED); \
    printCenteredMessageOnRow(YSize/8, _XL_C _XL_R _XL_O _XL_S _XL_S _XL_SPACE _XL_S _XL_N _XL_A _XL_K _XL_E); \
    SET_TEXT_COLOR(COLOR_WHITE); \
    printCenteredMessageOnRow(YSize/8+2, _XL_F _XL_a _XL_b _XL_r _XL_i _XL_z _XL_i _XL_o _XL_SPACE _XL_C _XL_a _XL_r _XL_u _XL_s _XL_o); \
    extra_title();


#define initialize_map() \
{ \
    CLEAR_SCREEN(); \
    init_map_to_empty(); \
    DRAW_MAP_BORDERS(); \
    _XLIB_DRAW(XSize-2,0,&VERTICAL_HEAD_IMAGE); \
    _XLIB_DRAW(0,0,&SCORE_TEXT_LEFT_IMAGE); \
    _XLIB_DRAW(1,0,&SCORE_TEXT_RIGHT_IMAGE); \
    _XLIB_DRAW(XSize-10+HISCORE_OFFSET,0,&HI_TEXT_IMAGE); \
    _XLIB_DRAW(8,0,&APPLE_IMAGE); \
    _XLIB_DRAW(0,YSize-1,&LV_TEXT_IMAGE); \
    SET_TEXT_COLOR(COLOR_WHITE); \
    DISPLAY_LIVES(); \
    PRINTD(1,YSize-1,2,level); \
    DISPLAY_REMAINING_APPLES_COUNT(); \
    PRINTD(XSize-9+HISCORE_OFFSET,0,5,record); \
    DISPLAY_ENERGY(); \
    init_snake(); \
    build_level(); \
    DISPLAY_POINTS(); \
}


#define RED_ENERGY_THRESHOLD ((MAX_ENERGY)/2)


void DISPLAY_ENERGY(void);

void PRESS_KEY(void);

void DISPLAY_POINTS(void);

void DISPLAY_REMAINING_APPLES_COUNT(void);

void DISPLAY_LIVES(void);

void show_intro_snake(void);

#endif // _GAME_TEXT_H

