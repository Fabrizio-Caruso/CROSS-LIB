#include <psg.h>
#include <stdio.h>

void _ping_sound(uint8_t freq)
{
	uint16_t i;
    
	psg_volume(0,15);
    
    psg_tone(0,freq);

    for(i=0;i<120;i++)
    {
    }

    psg_volume(0,0);
}

void _XL_ZAP_SOUND(void) 
{
    uint8_t i;
    uint8_t j;

	psg_volume(0,15);
    
    for(i=0;i<40;i++)
    {
        for(j=0;j<i;++j)
        {
        }
        psg_tone(0,i*4);
        
    }   
    psg_volume(0,0);
}


void _explosion_sound(uint8_t freq)
{ 
    uint8_t i;
    uint8_t j;

	psg_volume(0,15);
    
    for(i=0;i<freq;i++)
    {
        psg_tone(0,freq);
        for(j=0;j<i;++j)
        {
        }
        psg_tone(0,i&3);
        
    }   
    psg_volume(0,0);
}

int main(void)
{
    psg_init();
    while(1)
    {
        _ping_sound(120);
        getchar();
        _XL_ZAP_SOUND();
        getchar();
        _explosion_sound(80);
        getchar();
    }
    return 0;
}
