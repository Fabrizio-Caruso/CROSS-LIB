#ifndef _SLEEP_H
#define _SLEEP_H

#include "standard_libs.h"

#if XSize>=32

    // SLOWER SECRETS FOUND DISPLAY
    #define DISPLAY_SPEED_FACTOR 2
    
    // SLOWER BONUS DISPLAY
    #define DISPLAY_SPEED_RIGHT_SHIFT 5
#else
    #define DISPLAY_SPEED_FACTOR 1

    #define DISPLAY_SPEED_RIGHT_SHIFT 6
#endif


// #if defined(_XL_SLOW_DOWN_FACTOR) && _XL_SLOW_DOWN_FACTOR>0
    void SHORT_SLEEP(uint8_t t);
// #else
    // #define SHORT_SLEEP(t)
// #endif

#endif // _SLEEP_H

