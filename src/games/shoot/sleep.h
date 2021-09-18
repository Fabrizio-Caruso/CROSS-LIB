#ifndef _SLEEP_H
#define _SLEEP_H

#include "standard_libs.h"

#if _XL_SLOW_DOWN_FACTOR>0
    void _do_sleep(uint8_t t);
    #define SHORT_SLEEP(t) _do_sleep(t)
#else
    #define SHORT_SLEEP(t)
#endif

#endif // _SLEEP_H

