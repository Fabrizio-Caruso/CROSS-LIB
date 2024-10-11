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

#if !defined(_XL_NO_UDG) && !defined(NO_TRANSACTION_ANIMATION)
    #define TRANSITION_ANIMATION 1
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
