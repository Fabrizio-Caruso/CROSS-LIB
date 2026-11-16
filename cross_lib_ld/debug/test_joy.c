// CPC: zcc +cpc test_joy.c -subtype=dsk -lndos -otestjoy.dsk -create-app

#include <stdio.h>
#include <games.h>

int main(void)
{
	int stick = 1;
	int input;
	
	while(1)
	{
		input = joystick(stick);
		if(input && input==MOVE_FIRE)
		{
			printf("FIRE\n");
		}
	}

	return 0;
}
