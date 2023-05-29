
#ifndef _SETTINGS
#define _SETTINGS

#if defined(_XL_TURN_BASED)
	#if !defined(NO_BLINKING)
		#define NO_BLINKING
	#endif
#endif

#if !defined(NO_PLAYER_ANIMATION)
    #define ANIMATE_PLAYER
#endif

#if !defined(NO_END_SCREEN) && !defined(END_SCREEN)
	#define END_SCREEN
#endif

# if !defined(NO_BETWEEN_LEVEL) && !defined(BEWEEN_LEVEL)
	#define BEWEEN_LEVEL
#endif

#if !defined(TINY_GAME) && !defined(LIGHT_GAME)
	#ifndef FULL_GAME	
		#define FULL_GAME
	#endif
#endif

#include "cross_lib.h"

struct ImageStruct
{
    uint8_t _imageData;
    #if !defined(_XL_NO_COLOR)
        uint8_t _color;
    #endif
};

typedef struct ImageStruct Image;


#if !defined(NO_BLINKING)
    void _blink_draw(uint8_t x,uint8_t y,Image * image, uint8_t * blinkCounter);
#else
    #define _blink_draw(x,y,image,blinkCounter) _draw(x,y,image)
#endif


#include "game_settings.h"

#endif // _SETTINGS

