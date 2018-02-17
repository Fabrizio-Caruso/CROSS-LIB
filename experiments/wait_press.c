#include <stdio.h>
#include <conio.h>

int main()
{
	int i;
	
	for(i=0;i<10;++i)
	{
		printf("Press key\n");

		while(kbhit())
			getch();
		while(!kbhit())
		{ 
		}; 
		getch();

		printf("Key pressed: %d\n\n", i);
	}
	return 0;
}

