#include <peekpoke.h>

void INIT_GRAPHICS(void)
{
	// Select video bank and set the character set pointer to point to $C800
	// where the redefined charset has been placed by the linker
	POKE(56576u,0);
	POKE(53272u,2);
	POKE(648,192);
	
	// Frame color = BLACK
	POKE(53280u,0);
	
	// Background color = BLACK
	POKE(53281u,0);
}