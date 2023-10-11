// CPC: zcc +cpc test_joy2.c -subtype=dsk -lndos -otestjoy2.dsk -create-app

#include <stdio.h>

#include <time.h>
#include <unistd.h>
#include <stdlib.h> 

#include <games.h>
#define YSize 20

int input;
int stick = 1;
int time_counter;
int fire_pressed = 0;

uint8_t fire_pressed_after_time(void)
{
	return (!time_counter) && (fire_pressed = (joystick(stick)==MOVE_FIRE));
}


int main(void)
{
	input = 0;
	uint8_t i;

	time_counter = 5;
    do
    {
		if(time_counter)
		{
			--time_counter;
		}
        for(i=3;i<(YSize-5)*4;++i)
        {
			// Do something here
            if(fire_pressed_after_time())
            {
                break;
            }
			// Do something else here

            if(fire_pressed_after_time())
            {
                break;
            }
        }
        if(!fire_pressed)
        {
			// Do something
        }
    }
    while(!fire_pressed);    
	printf("OK1");
	
	sleep(1);
	
	time_counter = 5;
    do
    {
        for(i=5;i<YSize-2;++i)
        {
			
            if(fire_pressed_after_time())
            {
                break;
            }

            if(time_counter)
            {
                --time_counter;
            }
            
        }
    }
    while(!fire_pressed); 
	printf("OK2");

	while(1){};

	return 0;
}
