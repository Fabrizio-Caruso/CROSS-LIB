
#include <atari7800.h>
// #include <conio.h>

#include "cross_lib.h"

extern char screen[];

void _XL_INIT_GRAPHICS(void)
{
	char i;
	
    
    // clrscr();
    // cputc('A');
	for(i=0;i<100;++i)
	{
		
		screen[i]=2*i;
		_XL_SLEEP(1);
    }

	while (1) {
		screen[0] = screen[0] + 1;
	}
}

