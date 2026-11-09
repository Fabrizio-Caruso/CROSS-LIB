

#include "sleep.h"

#include "standard_libs.h"
#include "sleep_macros.h"
#include "cross_lib.h"

// #if _XL_SLOW_DOWN_FACTOR>0
void SHORT_SLEEP(uint8_t t)
{
    uint8_t i;
    
    for(i=0;i<t;++i)
    {
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}
// #endif

