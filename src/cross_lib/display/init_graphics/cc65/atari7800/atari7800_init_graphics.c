
#include <atari7800.h>
#include <peekpoke.h>

extern char *screen;

void _XL_INIT_GRAPHICS(void)
{
	char i;
	
	for(i=0;i<100;++i)
	{
		
		screen[i]=i;
    }
	// POKE(0,1);
	// POKE(1000,1);
	// POKE(2000,1);
	// POKE(3000,1);
	// POKE(4000,1);
	// POKE(5000,1);
	// POKE(6500,1);
	// POKE(7000,1);
	// POKE(8000,1);
	// POKE(9000,1);	
	while(1){};
}

