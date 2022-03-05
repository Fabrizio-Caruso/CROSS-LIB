
#ifndef _SETTINGS
#define _SETTINGS

#if defined(TURN_BASED)
	#if !defined(NO_BLINKING)
		#define NO_BLINKING
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

