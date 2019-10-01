#include <peekpoke.h>
#include <supervision.h>
#include <peekpoke.h>

void INIT_GRAPHICS(void)
{
    POKE(SV_BANK,PEEK(SV_BANK) | 8);
}

