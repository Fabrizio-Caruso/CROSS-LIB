#ifndef _DISPLAY_TARGET_COLOR
#define _DISPLAY_TARGET_COLOR

#include "cross_lib.h"

#include "display_target_geometry.h"


#if defined(__GAL__) \
    || (defined(__PC6001__) && !(FORCE_SCREEN_MODE==2)) \
    || (defined(__ATARI5200__) && !defined(ATARI_MODE_1_COLOR)) \
    || (defined(__CREATIVISION__) && !defined(CREATIVISION_COLOR)) \
    || defined(__WINCMOC__) || (defined(__COCO__)&&!defined(BIT_MAPPED_4)) || (defined(__DRAGON__)&&!defined(BIT_MAPPED_4)) || defined(__OSIC1P__) \
    || (defined(__MC1000__)  && !defined(FORCE_SCREEN_MODE)) \
    || defined(__LAMBDA__) || (defined(__MSX__) && defined(MEMORY_MAPPED)) \
    || defined(__VZ__) \
    || (defined(__ATARI__) && !defined(ATARI_MODE_1_COLOR)) \
    || defined(__PET__) || defined(__CBM610__) || ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && !defined(APPLE2_HGR)) \
    || defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__) \
    || defined(__TRS80__) || defined(__CPM_80X24__) || defined(__PX8__) || defined(__PX4__) \
    || defined(__ABC80__) || defined(__ABC800__) || defined(__SRR__) \
    || defined(__KC__) || (defined(__ATMOS__) && !defined(ORIC_COLOR)) \
    || defined(__VECTREX__) || defined(__GB__) \
    || defined(__NC100__) || defined(__NC200__)
    #if !defined(_XL_NO_COLOR)
        #define _XL_NO_COLOR 
    #endif
#endif

//  defined(__COMX__) || defined(__PECOM__) || defined(__CIDELSA__) || defined(__MICRO__)
#if defined(_XL_NO_COLOR) || defined(__NES__) \
    || defined(__CREATIVISION__) || defined(__ORIC__) || defined(__ATMOS__) || defined(__TI99__)
    #define _XL_NO_TEXT_COLOR
#endif

#if defined(_BACKGROUND_COLOR) && _BACKGROUND_COLOR==_XL_WHITE

    #if defined(WHITE)
        #define BACKGROUND_COLOR WHITE

        #define BORDER_COLOR WHITE
    #else
        #define BACKGROUND_COLOR _XL_WHITE

        #define BORDER_COLOR _XL_WHITE
    #endif
#else
    #if defined(_XL_BLACK)
        #define BACKGROUND_COLOR _XL_BLACK

        #define BORDER_COLOR _XL_BLACK  
    #else
        #define BACKGROUND_COLOR 0

        #define BORDER_COLOR 0 
    #endif
#endif

#endif // _DISPLAY_TARGET_COLOR
