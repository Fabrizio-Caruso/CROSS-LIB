#include <stdio.h>
#include <conio.h>

int main()
{
	printf("Press key\n");

	while(kbhit())
		getch();
	while(!kbhit())
	{ 
	}; 
	getch();

	printf("key pressed\n");
	
	return 0;
}

