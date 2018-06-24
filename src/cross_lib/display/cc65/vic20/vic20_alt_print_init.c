#include <peekpoke.h>

void INIT_GRAPHICS(void)
{
	// #if defined(REDEFINED_CHARS)
		// POKE(0x9005,0xFF);			
	// #endif

	POKE(646,1);
	POKE(36879L,9);
}
