#include <conio.h>

extern char *screen;

int main(void)
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
	
	return 0;
}

