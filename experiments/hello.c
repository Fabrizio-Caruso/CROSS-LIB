#include <stdio.h>


void WAIT_PRESS(void)
{
	while(kbhit())
		getk();
	while(!kbhit())
	{ 
	}; 
	getk();
}

int main()
{
	getk();
	printf("Hello world!\n");	

	WAIT_PRESS();
	printf("Key pressed\n");	

			
	return 0;
}

