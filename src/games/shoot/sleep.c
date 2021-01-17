

#include "sleep.h"

#include "standard_libs.h"
#include "sleep_macros.h"
#include "cross_lib.h"

#if SLOW_DOWN>0
void _do_sleep(uint8_t t)
{
    uint8_t i;
    
    for(i=0;i<t;++i)
    {
        _XL_SLOW_DOWN(SLOW_DOWN);
    }
}
#endif

