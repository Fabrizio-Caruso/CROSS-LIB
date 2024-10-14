#ifndef _GAME_TEXT_H
#define _GAME_TEXT_H

#define MAX_ENERGY 20

void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text);

#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)


#if XSize<22
    #define HISCORE_OFFSET 2
#else
    #define HISCORE_OFFSET 0
#endif

#define ACHIEVEMENTS_X_OFFSET (((XSize)/2)-5)
#define ACHIEVEMENTS_Y_OFFSET ((YSize)/6)

#if YSize>=20
    #define STAT_OFFSET 4
#else
    #define STAT_OFFSET 2
#endif


#if YSize>=15
    #define LEVEL_OFFSET 2
#else
    #define LEVEL_OFFSET 0
#endif

#if !defined(NO_DISPLAY_RINGS)
    void DISPLAY_RINGS(void);

    #define DELETE_RINGS() \
        _XL_PRINT(ACHIEVEMENTS_X_OFFSET+3, ACHIEVEMENTS_Y_OFFSET+3, "     ")
#else
    #define DISPLAY_RINGS()

    #define DELETE_RINGS()
#endif



#define _NO_ENERGY_STRING _XL_N _XL_O _XL_SPACE _XL_E _XL_N _XL_E _XL_R _XL_G _XL_Y
#define _CLEARED_STRING _XL_C _XL_L _XL_E _XL_A _XL_R _XL_E _XL_D
#define _BONUS_STRING _XL_B _XL_O _XL_N _XL_U _XL_S
#define _THE_END_STRING _XL_T _XL_H _XL_E _XL_SPACE _XL_E _XL_N _XL_D
#define _GAME_OVER_STRING _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R

#if XSize<20
    #define _SECRET_STRING _XL_W _XL_O _XL_W
	#define _SECRET_LEN 3
#else
    #define _SECRET_STRING _XL_S _XL_E _XL_C _XL_R _XL_E _XL_T
	#define _SECRET_LEN 6
#endif
#define _LEVEL_STRING _XL_L _XL_E _XL_V _XL_E _XL_L
#define _SNAKE_STRING _XL_S _XL_N _XL_A _XL_K _XL_E
#define _Fabrizio_Caruso_STRING _XL_F _XL_a _XL_b _XL_r _XL_i _XL_z _XL_i _XL_o _XL_SPACE _XL_C _XL_a _XL_r _XL_u _XL_s _XL_o
#define _PRESS_FIRE_STRING _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E

#if YSize>=20
    #define LINE_SKIP 2
    #define LINE_OFFSET 8
    #define SNAKE_LINE_OFFSET 5
#else
    #define SNAKE_LINE_OFFSET 4
    #define LINE_OFFSET 6
    #define LINE_SKIP 1
#endif

// Adding this sensical line _XL_PRINT(0,i,""); fixes a bug for GCC for TI99
#if !defined(NO_EXTRA_TITLE) && YSize>=16
    #if !defined(_XL_NO_COLOR)
        #define extra_title() \
        { \
            uint8_t i; \
            show_intro_snake(); \
            _XL_DRAW(XSize/4+XSize/2,YSize/8+SNAKE_LINE_OFFSET,APPLE_TILE,_XL_RED); \
            for(i=0;i<NUMBER_OF_STRINGS;++i) \
            { \
                _XL_SET_TEXT_COLOR(_XL_WHITE); \
                _XL_PRINT(XSize/2-4,YSize/8+LINE_OFFSET+LINE_SKIP*i, (char *) strings[NUMBER_OF_STRINGS-1-i] ); \
                _XL_DRAW(XSize/2-6,YSize/8+LINE_OFFSET+LINE_SKIP*i, images[NUMBER_OF_STRINGS-i], image_colors[NUMBER_OF_STRINGS-i]); \
            } \
        }
    #else
        #define extra_title() \
        { \
            uint8_t i; \
            show_intro_snake(); \
            _XL_DRAW(XSize/4+XSize/2,YSize/8+SNAKE_LINE_OFFSET,APPLE_TILE,_XL_RED); \
            for(i=0;i<NUMBER_OF_STRINGS;++i) \
            { \
                _XL_DRAW(XSize/2-6,YSize/8+LINE_OFFSET+LINE_SKIP*i, images[NUMBER_OF_STRINGS-i], 0); \
                _XL_SET_TEXT_COLOR(_XL_WHITE); \
                _XL_PRINT(XSize/2-4,YSize/8+LINE_OFFSET+LINE_SKIP*i, (char *) strings[NUMBER_OF_STRINGS-1-i] ); \
            } \
        }
    #endif
#else
    #define extra_title() \
    { \
        show_intro_snake(); \
        _XL_DRAW(XSize/4+XSize/2,YSize/8+SNAKE_LINE_OFFSET,APPLE_TILE,_XL_RED); \
    }
#endif

#if !defined(NO_CONTROL_INSTRUCTIONS)
    #define TILE_PRESS_KEY() CONTROL_INSTRUCTIONS()
#else
    #define TILE_PRESS_KEY() PRESS_KEY()
#endif

#if !defined(_XL_NO_COLOR)
	#define title() \
		_XL_CLEAR_SCREEN(); \
		_XL_DRAW(XSize/2-4,0,HI_TEXT_TILE,_XL_RED); \
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		_XL_PRINTD(XSize/2-3,0,5,record); \
		_XL_SET_TEXT_COLOR(_XL_RED); \
		PRINT_CENTERED_ON_ROW(YSize/8, _SNAKE_STRING); \
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		PRINT_CENTERED_ON_ROW(YSize/8+2, _Fabrizio_Caruso_STRING); \
		extra_title(); \
		TILE_PRESS_KEY();
#else
	#define title() \
		_XL_CLEAR_SCREEN(); \
		_XL_DRAW(XSize/2-3,0,HI_TEXT_TILE,0); \
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		_XL_PRINTD(XSize/2-2,0,5,record); \
		_XL_SET_TEXT_COLOR(_XL_RED); \
		PRINT_CENTERED_ON_ROW(YSize/8, _SNAKE_STRING); \
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		PRINT_CENTERED_ON_ROW(YSize/8+2, _Fabrizio_Caruso_STRING); \
		extra_title(); \
		TILE_PRESS_KEY();
#endif


#define RED_ENERGY_THRESHOLD ((MAX_ENERGY)/2)


void DISPLAY_ENERGY(void);

void PRESS_KEY(void);

void DISPLAY_POINTS(void);

void DISPLAY_REMAINING_APPLES_COUNT(void);

void DISPLAY_LIVES(void);

void show_intro_snake(void);

#if !defined(NO_CONTROL_INSTRUCTIONS)
void CONTROL_INSTRUCTIONS(void);
#endif

#endif // _GAME_TEXT_H

