
#include <atari7800.h>
// #include <conio.h>

#include "cross_lib.h"

extern char screen[];

void _XL_INIT_GRAPHICS(void)
{

    #if defined(__ALTERNATE_COLORS)
    
        // TODO: Implement palette switch
        // MARIA.bkgrnd = 0x17;
        // MARIA.p0c1 = 0x00;
        // MARIA.p0c2 = 0xb2;
        // MARIA.p0c3 = 0x05;
        MARIA.bkgrnd = 0x00;
        // MARIA.p0c1 = 0x00;
        MARIA.p0c2 = 0xb2;
        MARIA.p0c3 = 0x05;
    #endif

}

