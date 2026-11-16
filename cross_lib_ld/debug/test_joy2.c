// CPC: zcc +cpc  test_joy2.c -subtype=dsk -lndos -otestjoy2.dsk -create-app


#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sound.h>

#include <games.h>
#define YSize 20

#include <psg.h>
#include <stdint.h>
#define A_PERIOD_LOW 0
#define A_PERIOD_HI 1
#define B_PERIOD_LOW 2
#define B_PERIOD_HI 3
#define C_PERIOD_LOW 4
#define C_PERIOD_HI 5
#define NOISE 6
#define CONTROL 7
#define A_VOLUME 8
#define B_VOLUME 9
#define C_VOLUME 10  
#define ENV_PERIOD_LOW 11
#define ENV_PERIOD_HI 12
#define ENV_WAVE 13

#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

int input;
int stick = 1;
int time_counter;
int fire_pressed = 0;

void _XL_ZAP_SOUND(void) 
{
    uint8_t i;
    uint8_t j;
    set_psg(A_VOLUME,15);
    set_psg(A_PERIOD_LOW,255);  
    set_psg(A_PERIOD_HI, 15);  
    set_psg(CONTROL, 0xFF  - 0x01 - 0x40);  // -0x40 is the FIX
    for(i=0;i<16;i++)
    {
        set_psg(A_PERIOD_HI,15-i);        
        for(j=0;j<150U;++j)
        {
        }
    }    
    set_psg(A_VOLUME,0);        
}



uint8_t fire_pressed_after_time(void)
{
    return (!time_counter) && (fire_pressed = (joystick(stick)==MOVE_FIRE));
}


void short_sleep(void)
{
    uint16_t i;
    for(i=0;i<200;++i)
    {
    }
}

int main(void)
{
    input = 0;
    uint8_t i;
    time_counter = 5;
    do
    {
        for(i=3;i<(YSize-5)*4;++i)
        {
            if(time_counter)
            {
                --time_counter;
            }            
            // Do something here
            if(fire_pressed_after_time())
            {
                break;
            } 
            // Do something else here
            short_sleep();
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
    
	
	//asm("di"); 
	_XL_ZAP_SOUND(); 
	//asm("ei");
	
    // **********************************
    // _XL_ZAP_SOUND(); // **** BUG HERE *** 
    // **********************************
    
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
