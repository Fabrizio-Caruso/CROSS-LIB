#include <supervision.h>

#include "standard_libs.h"

#include <peekpoke.h>

// struct __sv_tone {
    // unsigned            delay;   // Fhx_Flow and hi
    // unsigned char       control; // Chx_Vol_duty
    // unsigned char       timer; // Chx_Length
// };
// #define SV_RIGHT (*(struct __sv_tone*)0x2010)
// #define SV_LEFT  (*(struct __sv_tone*)0x2014)

// struct __sv_noise {
    // unsigned char       volume;         /* and frequency */
    // unsigned char       timer;
    // unsigned char       control;
// };
// #define SV_NOISE (*(struct __sv_noise*)0x2028)

void play_frequency(uint16_t value)
{
    SV_RIGHT.delay = 10000+value;
    SV_LEFT.delay = 10000+value;
    
    SV_RIGHT.control = 0x3F;
    SV_LEFT.control  = 0x3F;

    SV_RIGHT.timer = 2;
    SV_LEFT.timer = 2;
}

