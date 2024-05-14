#ifndef _DISPLAY_TARGET_COLOR
#define _DISPLAY_TARGET_COLOR

#include "cross_lib.h"

#include "display_target_geometry.h"


#if defined(__GAL__) \
    || (defined(__PC6001__) && !(__SCREEN_MODE==2)) \
    || (defined(__ATARI5200__) && !defined(__ANTIC_MODE6_GRAPHICS)) \
    || (defined(__CREATIVISION__) && !defined(__VDP_MODE1_GRAPHICS)) \
    || defined(__WINCMOC__) || (defined(__COCO3__)&&!defined(__BIT_MAPPED_16_GRAPHICS)) || (defined(__COCO__)&&!defined(__BIT_MAPPED_4_GRAPHICS)) || (defined(__DRAGON__)&&!defined(__BIT_MAPPED_4_GRAPHICS)) || defined(__OSIC1P__) \
    || (defined(__MC1000__)  && !(__SCREEN_MODE==2)) \
    || defined(__LAMBDA__) || (defined(__MSX__) && defined(__MEMORY_MAPPED_GRAPHICS)) \
    || defined(__VZ__) \
    || (defined(__ATARI__) && !defined(__ANTIC_MODE6_GRAPHICS)) \
    || defined(__PET__) || defined(__CBM610__) || ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && !defined(__APPLE2_HGR_GRAPHICS)) \
    || defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__) \
    || defined(__TRS80__) || defined(__PX8__) || defined(__PX4__) \
    || defined(__ABC80__) || defined(__ABC800__) || defined(__SRR__) \
    || defined(__KC__) || (defined(__ATMOS__) && !defined(__ORIC_COLOR_GRAPHICS)) \
    || defined(__VECTREX__) || defined(__GB__) \
    || defined(__NC100__) || defined(__NC200__)
    #if !defined(_XL_NO_COLOR)
        #define _XL_NO_COLOR 
    #endif
#endif

//  defined(__COMX__) || defined(__PECOM__) || defined(__CIDELSA__) || defined(__MICRO__)
#if defined(_XL_NO_COLOR) || defined(__NES__) \
    || defined(__CREATIVISION__) || defined(__ORIC__) || defined(__ATMOS__) || defined(__TI99__) \
	|| defined(__COMX__) || defined(__PECOM__) || defined(__CIDELSA__) || defined(__MICRO__) \
	|| defined(__ATARI7800_COLOR_GRAPHICS)
    #define _XL_NO_TEXT_COLOR
#endif

#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
	#define _XL_BACKGROUND_COLOR _XL_WHITE
	#define __BORDER_COLOR _XL_WHITE
#else
	#define _XL_BACKGROUND_COLOR _XL_BLACK
	#define __BORDER_COLOR _XL_BLACK
#endif


#endif // _DISPLAY_TARGET_COLOR
