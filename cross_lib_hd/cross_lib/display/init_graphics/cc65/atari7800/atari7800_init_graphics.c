
#include <atari7800.h>
// #include <conio.h>

#include "cross_lib.h"

extern char screen[];


// 0x48 pink
// 0x38 light red
void _XL_INIT_GRAPHICS(void)
{

    // #if defined(__USE_GREEN)
    
        // TODO: Implement palette switch
        // MARIA.bkgrnd = 0x17;
        // MARIA.p0c1 = 0x00;
        // MARIA.p0c2 = 0xb2;
        // MARIA.p0c3 = 0x05;
        MARIA.bkgrnd = 0x00; // black
        
    #if defined(__USE_GREEN)
        MARIA.p0c2 = 0xD4;
    #else
        MARIA.p0c2 = 0xAA; // darker cyan 0x98
    #endif
        MARIA.p0c1 = 0x33; // red

        MARIA.p0c3 = 0x0F; // white
    // #endif
    
        // MARIA.p0c1 = 0xD4; // green

            // MARIA.p0c1 = 0xB4; // green


}

