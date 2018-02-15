#include <stdio.h>
#include <conio.h>

#define cgetc() getchar()

void WAIT_PRESS(void)
{
	while(kbhit())
		cgetc();
	while(!kbhit())
	{ 
	}; 
	cgetc();
}

int main()
{
	gotoxy(2,2);
	cprintf("Hello world!\n");	

	WAIT_PRESS();
	gotoxy(2,4);
	cprintf("Key pressed 1\n");	

	WAIT_PRESS();
	gotoxy(2,6);
	cprintf("Key pressed 2\n");	

	WAIT_PRESS();
	gotoxy(2,8);
	cprintf("Key pressed 3\n");	
			
	return 0;
}

