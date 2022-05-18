#ifndef _SLEEP_H
#define _SLEEP_H

#include "standard_libs.h"

#if _XL_SLOW_DOWN_FACTOR>0
    void SHORT_SLEEP(uint8_t t);
#else
    #define SHORT_SLEEP(t)
#endif

#endif // _SLEEP_H

