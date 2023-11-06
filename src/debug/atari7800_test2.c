#include <atari7800.h>
#include <conio.h>

extern char screen[];

void main()
{
	char i;
	
	clrscr();
	
	for(i=0;i<100;++i)
	{
		
		screen[i]=i;
    }
	while (1) {
		screen[0] = screen[0] + 1;
	}
}

