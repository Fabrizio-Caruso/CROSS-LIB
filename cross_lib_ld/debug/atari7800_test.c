// cl65.exe -t atari7800 --force-import __EXEHDR__ atari7800_test.c -o atari7800_DEBUG.a78

#include <conio.h>
#include <peekpoke.h>

extern char screen[];

int main(void)
{
	clrscr();

	POKE(7000,1);
	
	while(1){};
	
	return 0;
}

