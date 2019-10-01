#include <peekpoke.h>
#include <supervision.h>
#include <peekpoke.h>

void INIT_GRAPHICS(void)
{
    SV_LCD.height = 0xA0;
    SV_LCD.width = 0xA0;
    SV_BANK = 0xC9;
}

