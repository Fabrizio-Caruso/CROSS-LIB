#include <stdio.h>

int main()
{
	printf("Hello world!\n");	
	while(1)
	{
		if(kbhit())
		{
			printf("%c\n",getch());
		}
	}			
	return 0;
}

