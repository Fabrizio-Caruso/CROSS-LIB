
#ifndef _SPECTRUM_SLEEP
#define _SPECTRUM_SLEEP

#include <z80.h>

void sleep(unsigned int sec)
{
	z80_delay_ms(1000);
}

#endif // _SPECTRUM_SLEEP