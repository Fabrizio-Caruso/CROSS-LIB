
#include <peekpoke.h>

void _XL_INIT_GRAPHICS(void)
{
    // __asm__("SEI");
    POKE(53281,0);
    POKE(53280,0);
}
